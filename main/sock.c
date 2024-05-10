#include <pico/stdio.h>

#include <lwip/opt.h>
#include <lwip/sys.h>
#include <lwip/api.h>

#include "init.h"

#include "config.h"

void recv_handler(struct netconn* conn, struct netbuf* buf) {
	void *data;
	uint16_t len;

	do {
		netbuf_data(buf, &data, &len);
		printf("got %d bytes!\n", len);
	} while (netbuf_next(buf) >= 0);

	netbuf_delete(buf);
}

void accept_handler(struct netconn* conn) {
	printf("new connection!\n");

	struct netbuf* buf;

	while (netconn_recv(conn, &buf) == ERR_OK)
		recv_handler(conn, buf);

	netconn_close(conn);
	netconn_delete(conn);

	printf("connection closed!\n");
}

void serve_task() {
	await_init();

	printf("starting server...\n");
	struct netconn* conn = netconn_new(NETCONN_TCP);
	netconn_bind(conn, IP_ADDR_ANY, CONF_SRV_PORT);
	netconn_listen(conn);

	printf("listening on %s:%d\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), CONF_SRV_PORT);
	while (1) {
		struct netconn* incoming;
		if (netconn_accept(conn, &incoming) == ERR_OK)
			accept_handler(incoming);
	}
}

