#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
	struct sockaddr_in server;
	char buff[1024];
	int mysock;
	int sock;
	int r;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > sock) {
		perror("socket");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);

	r = bind(sock, (struct sockaddr *)&server, sizeof(server));
	if (0 > r) {
		perror("bind");
		exit(1);
	}

	listen(sock, 5);

	do {
		mysock = accept(sock, (struct sockaddr *) 0, 0);
		if (0 > mysock)
			perror("accept");
		else {
			memset(buff, 0, sizeof(buff));
			r = recv(mysock, buff, sizeof(buff), 0);
			if (0 > r)
				perror("recv");
			else if (r == 0)
				printf("Ending connection");
			else
				printf("Got the message: %d\n", r);
			close(mysock);
		}

	} while(1);


	return 0;
}
