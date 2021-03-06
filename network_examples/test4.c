#include "../common/sock_help.h"

int main(void)
{
	SERVER_INFO;	/* contains sockfd,newfd */
	char address[INET6_ADDRSTRLEN];
	char msg[256];
	int port;
	
	sockfd = create_server(0, 5000, "0.0.0.0");
	if (sockfd == BAD_SOCKET)
		return 1;
	newfd = accept_conn(sockfd);
	if (newfd == BAD_SOCKET)
		return 1;
	
	get_addr_info(newfd, address, &port);
	sprintf(msg, "%s:%d connected!", address, port);
	puts(msg);
	send_msg(newfd, "Welcome to sock help library.\r\n");
	close_conn(newfd);
	sprintf(msg, "%s:%d disconnected!", address, port);
	puts(msg);
	close_conn(sockfd);
	return 0;
}
