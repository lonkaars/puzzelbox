#include <pico/stdio.h>

#include <lwip/init.h>
#include <lwip/tcp.h>

#include "init.h"

#include "config.h"

err_t recv_handler(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
	if (p == NULL) return ERR_VAL;

	printf("recv: %s\n", (char *) p->payload);

	tcp_recved(pcb, p->len);
	pbuf_free(p);
	return ERR_OK;
}

err_t accept_handler(void *arg, struct tcp_pcb *pcb, err_t err) {
	tcp_recv(pcb, recv_handler);

	return ERR_OK;
}

void serve_task() {
	await_init();

	// TODO: why does this hang???
	// printf("starting lwip...\n");
	// lwip_init();

	printf("starting server...\n");

	struct tcp_pcb *pcb = tcp_new();
	tcp_bind(pcb, IP_ADDR_ANY, CONF_SRV_PORT);
	pcb = tcp_listen(pcb);

	printf("listening on %s:%d\n", ip4addr_ntoa(netif_ip4_addr(netif_list)), CONF_SRV_PORT);

	// connection accept callback
	tcp_accept(pcb, accept_handler);

	printf("server started!\n");
}

