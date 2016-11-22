#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

int main(int argc, const char *argv[])
{
	struct sockaddr_in server;
	struct hostent *hp;
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

	hp = gethostbyname(argv[1]);
	if (hp == 0) {
		perror("gethostbyname");
		exit(1);
	}

	memcpy(&server.sin_addr, hp->h_addr, hp->h_length);
	server.sin_port = htons(5000);

	r = connect(sock, (struct sockaddr *)&server, sizeof(server));
	if (0 > r) {
		perror("connect");
		exit(1);
	}

	char *payload = "hello there";
	if (send(sock, payload, sizeof(payload), 0) < 0) {
		close(sock);
		perror("send");
		exit(1);
	}

	printf("sent something\n");
	close(sock);


	return 0;
}
