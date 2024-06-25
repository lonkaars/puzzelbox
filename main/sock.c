#include <pico/stdio.h>

#include <lwip/opt.h>
#include <lwip/sys.h>
#include <lwip/api.h>
#include <string.h>

#include "init.h"
#include "config.h"
#include "i2ctcpv1.h"
#include "sock.h"
#include "pb-mod.h"

struct netconn* current_connection = NULL;
i2ctcp_msg_t recv_msg;

static void sock_dump_msg(i2c_addr_t addr, const uint8_t * data, size_t data_size) {
	if (current_connection == NULL) return;

	i2ctcp_msg_t send_msg = {
		.addr = addr,
		.data = (char *) data,
		.length = data_size,
	};

	char * buf;
	size_t buf_sz;

	if (!i2ctcp_write(&send_msg, &buf, &buf_sz)) return;

	// NOTE: netconn does return an error code, but the data needs to be freed
	// whether netconn throws an error or not, so it remains unused
	netconn_write(current_connection, buf, buf_sz, NETCONN_COPY);

	free(buf);
}

bool pb_hook_i2c_send(i2c_addr_t addr, const uint8_t * data, size_t data_size) {
	sock_dump_msg(addr, data, data_size);
	return addr == PB_MOD_ADDR; // stop processing message if it is sent to myself
}

bool pb_hook_i2c_recv(const uint8_t * data, size_t data_size) {
	sock_dump_msg(0x00, data, data_size);
	return false;
}

static void sock_fwd_msg(i2c_addr_t addr, const uint8_t * data, size_t data_size) {
	if (addr == PB_MOD_ADDR) {
		// addressed to me = act as recieved
		pb_i2c_recv(data, data_size);
	} else {
		// addressed to other puzzle module = forward
		pb_i2c_send(addr, data, data_size);
	}
}

void recv_handler(struct netconn* conn, struct netbuf* buf) {
	i2ctcp_read_reset(&recv_msg);

	do {
		char* data;
		uint16_t len;
		netbuf_data(buf, (void**)&data, &len);
		
		// continue early if more data is needed to complete message
		if (i2ctcp_read(&recv_msg, data, len) != 0) continue;

		// forward received message to puzzle bus
		sock_fwd_msg(recv_msg.addr, (uint8_t *) recv_msg.data, recv_msg.length);
		free(recv_msg.data);
	} while (netbuf_next(buf) >= 0);

	netbuf_delete(buf);
}

void accept_handler(struct netconn* conn) {
	current_connection = conn;

	struct netbuf* buf;
	while (netconn_recv(conn, &buf) == ERR_OK)
		recv_handler(conn, buf);

	netconn_close(conn);
	netconn_delete(conn);

	current_connection = NULL;
}

void serve_task() {
	printf("starting server...\n");
	struct netconn* conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, IP_ADDR_ANY, CFG_SRV_PORT);
	netconn_listen(conn);

	printf("listening on %s:%d\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), CFG_SRV_PORT);
	while (1) {
		struct netconn* incoming;
		if (netconn_accept(conn, &incoming) == ERR_OK)
			accept_handler(incoming);
	}
}

