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

	int sockfd; // socket fd
	int gai_result; // result from getaddrinfo
	int bind_result; // result from bind
	struct addrinfo *res; // Pointer to linked list sent back by gai
	struct addrinfo hints; // Options we pass to gai
	memset(&hints, 0, sizeof(hints)); // We set this struct to 0 and then give it all the options we want below
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	gai_result = getaddrinfo(NULL, "62000", &hints, &res); // NULL because we are going to listen
	if (gai_result != 0) {
		fprintf(stderr, "gai_result: %d\n", gai_result);
		exit(1);
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol); // Socket, get the family, socktype and protocol.
	if (sockfd == -1) {
		fprintf(stderr, "sockfd: %d\n", sockfd);
		exit(1);
	}

	bind_result = bind(sockfd, res->ai_addr, res->ai_addrlen); // Bind to socket fd. pass our own address (ai_addr) and the length
	if (bind_result != 0) {
		fprintf(stderr, "bind_result: %d\n", bind_result);
		exit(1);
	}

	freeaddrinfo(res); // free the info because we dont need it anymore

	fprintf(stdout, "Waiting for a connection...\n");
	listen(sockfd, BACKLOG); // Actually listen for connections with the sockfd
	
	socklen_t addr_size; // Size of address
	struct sockaddr_storage their_addr; // pass this struct to accept because it will hold their address
	addr_size = sizeof(their_addr); // Check the size of their addr
	int newfd; // New fd
	newfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size); // Actually accept. Pass the sockfd, cast their_addr to sockaddr and send the address to the function. Pass the addr_size address so we can handle the length.
	fprintf(stdout, "Connection is being established...\n");
	if (newfd == -1) {
		fprintf(stderr, "newfd: %d\n", newfd);
		exit(1);
	}

	fprintf(stdout, "Connection established!\n");
	char *msg = "Server says hi!\n";
	int len;
	int bytes_sent;
	len = strlen(msg);
	bytes_sent = send(newfd, msg, len, 0);

	size_t rlen = 1024;
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
		//fprintf(stdout, "loop");
		memset(&recvmsg, 0, rlen);
	}
	fprintf(stdout, "bytes recieved: %ld\nbytes sent: %d\n", bytes_recieved, bytes_sent);
 
	return 0;
}
