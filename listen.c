#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// int listen(int sockfd, int backlog); 

#define BACKLOG 2

int main () {

	int sockfd;
	int gai_result;
	int bind_result;
	struct addrinfo *res;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	gai_result = getaddrinfo(NULL, "62000", &hints, &res);
	if (gai_result != 0) {
		fprintf(stderr, "gai_result: %d\n", gai_result);
		exit(1);
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) {
		fprintf(stderr, "sockfd: %d\n", sockfd);
		exit(1);
	}

	bind_result = bind(sockfd, res->ai_addr, res->ai_addrlen);
	if (bind_result != 0) {
		fprintf(stderr, "bind_result: %d\n", bind_result);
		exit(1);
	}

	listen(sockfd, BACKLOG);
	socklen_t addr_size;
	struct sockaddr_storage their_addr;
	addr_size = sizeof(their_addr);
	int newfd;
	newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
	if (newfd == -1) {
		fprintf(stderr, "newfd: %d\n", newfd);
		exit(1);
	}

	char *msg = "Server says hi!\n";
	int len;
	int bytes_sent;
	len = strlen(msg);
	bytes_sent = send(newfd, msg, len, 0);

	int rlen = 1000;
	int recv_ret;
	size_t bytes_recieved = 0;
	while(true) {

		char recvmsg[rlen]; 
		recv_ret = recv(newfd, recvmsg, rlen, 0);
		bytes_recieved = bytes_recieved+recv_ret;
		if (recv_ret < 1) {
			break;
		}
		recvmsg[recv_ret] = '\0';
		fprintf(stdout, "%s", recvmsg);
		memset(&recvmsg, 0, (size_t)rlen);
	}
	fprintf(stdout, "bytes recieved: %ld\nbytes sent: %d\n", bytes_recieved, bytes_sent);
 
	return 0;
}
