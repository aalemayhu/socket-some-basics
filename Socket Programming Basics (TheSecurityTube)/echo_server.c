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

#define ERROR -1
#define MAX_CLIENTS 2
#define MAX_DATA  1024

int main(int argc, char **argv)
{
	struct sockaddr_in server;
	int sockaddr_len = sizeof(struct sockaddr_in);
	struct sockaddr_in client;
	char data[MAX_DATA];
	int sock, new, port;
	int data_len;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == ERROR)
		fail("socket: ");

	if (argc < 2) {
		port = 10000;
		printf("No port supplied, defaulting to %d\n", port);
	} else
		port = atoi(argv[1]);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);

	if (bind(sock, (struct sockaddr *)&server, sockaddr_len) == ERROR)
		fail("bind");

	if ((listen(sock, MAX_CLIENTS)) == ERROR)
		fail("listen");

	while (1)
	{
		new = accept(sock, (struct sockaddr *)&client, &sockaddr_len);
		if (new == ERROR)
			fail("accept");

		printf("New client connected %s:%d\n",
		       inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		data_len = 1;

		while (data_len)
		{
			data_len = recv(new, data, MAX_DATA, 0);

			if (data_len) {
				send(new, data, data_len, 0);
				data[data_len] = '\0';
				printf("Sent msg: %s\n", data);
			}
		}
		printf("Client disconnected\n");
		close(new);
	}

	return EXIT_SUCCESS;
}
