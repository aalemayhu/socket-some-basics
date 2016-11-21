#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void fail(char *prefix) {
	perror(prefix);
	exit(EXIT_FAILURE);
}

int main()
{
	char msg[] = "Hello to the world of socket programming !";
	struct sockaddr_in server, client;
	unsigned int len;
	int sock, cli;
	int sent;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		fail("socket: ");

	server.sin_family = AF_INET;
	server.sin_port = htons(10000);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);

	len = sizeof(struct sockaddr_in);

	if (bind(sock, (struct sockaddr *)&server, len) == -1)
		fail("bind");

	if ((listen(sock, 5)) == -1)
		fail("listen");

	while (1) {
		cli = accept(sock, (struct sockaddr *)&client, &len);
		if (cli == -1)
			fail("accept");

		sent = send(cli, msg, strlen(msg), 0);

		printf("Sent %d bytes to client : %s\n", sent,
		       inet_ntoa(client.sin_addr));
		close(cli);
	}

	return EXIT_SUCCESS;
}
