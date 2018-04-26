/**************************************************
 * Sample 1 - Using the networking library.       *
 **************************************************
 * Created by Philip R. Simonson (aka 5n4k3)      *
 **************************************************
 */

#include "../common/sock_help.h"

#define MAXLINE		1024

/* program to handle multiple clients simple chat server */
int main(int argc, char *argv[])
{
	struct sockaddr_in addr;
	char line[MAXLINE];
	char message[MAXLINE];
	int serverfd;
	int addrlen;
	int newfd;
	int bytes;

	/* create server socket */
	serverfd = create_server(0, 5555, "0.0.0.0");
	if (serverfd < 0)	/* handle error */
		return -1;
	addrlen = sizeof(addr);
#ifdef WIN32
	newfd = accept(serverfd, (struct sockaddr*)&addr,
		(int*)&addrlen);
#else
	newfd = accept(serverfd, (struct sockaddr*)&addr,
		(unsigned int*)&addrlen);
#endif
	if (newfd < 0)
		perror("accept");
	puts("Client connected!");
	send_msg(newfd, "Type 'exit' without quotes to quit...\r\n"
		"Anything else will echo back to you.\r\n");

	while (1) {
		if ((bytes = get_cmd(newfd, line, sizeof(line))) > 0) {
			if (strcompare(line, "exit") == 0) {
				close_conn(newfd);
				break;
			}
			bytes = send_msg(newfd, "You gave me...\r\n");
			sprintf(message, "%s\r\n", line);
			bytes = send_msg(newfd, message);
		} else {
			/* handle sending data */
			if (bytes <= 0) {
				if (bytes == 0) {
					puts("Client disconnected!");
				} else {
					perror("recv");
				}
				close_conn(newfd);
				break;
			}
		}
	}
	close_conn(serverfd);	/* close server socket */
	printf("Connection closed.\n");
	return 0;
}

