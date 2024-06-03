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
#include <signal.h>
 
#define __FAIL EXIT_FAILURE
#define BACKLOG 2

size_t bytes_recieved = 0;
int sockfd = -1;

void INThandler() {

   	fprintf(stderr, " SIGINT\n");
	fprintf(stdout, "bytes recieved: %ld\n", bytes_recieved);
	close(sockfd);
	exit(0);
}

int main (void) {

	int gai_result; // result from getaddrinfo
	int bind_result = -1; // result from bind
	struct addrinfo *res; // Pointer to linked list sent back by gai
	struct addrinfo *p; // for linked list stuff
	struct addrinfo hints; // Options we pass to gai
	memset(&hints, 0, sizeof(hints)); // We set this struct to 0 and then give it all the options we want below
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	gai_result = getaddrinfo(NULL, "62000", &hints, &res); // NULL because we are going to listen
	if (gai_result != 0) {
   		fprintf(stderr, "gai_result: %d\n", gai_result);
   		exit(__FAIL);
	}
	
	for (p = res ; p != NULL ; p = p->ai_next) { // Loop 

		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); // Socket, get the family, socktype and protocol.
		if (sockfd == -1) {
			fprintf(stderr, "sockfd: %d\n", sockfd);
			continue;
		}

		bind_result = bind(sockfd, p->ai_addr, p->ai_addrlen);
		if (bind_result != 0) {
			fprintf(stderr, "bind_result: %d\n", bind_result);
			close(sockfd);
			continue;
		}
	}
	if (sockfd == -1 || bind_result == -1) {
		exit(__FAIL);
	}
	
	freeaddrinfo(res); // free the info because we dont need it anymore

	fprintf(stdout, "Listening...\n");
	
	struct sockaddr_storage from;
	socklen_t fromlen = sizeof(struct sockaddr_storage);
	size_t rlen = 1024;
	int recv_ret;
	
	signal(SIGINT, INThandler);
	while(true) {

		char recvmsg[rlen]; 
		recv_ret = recvfrom(sockfd, recvmsg, rlen, 0, (struct sockaddr *)&from, &fromlen);
		bytes_recieved = bytes_recieved+recv_ret;
		if (recv_ret < 1) {
			break;
		}
		recvmsg[recv_ret] = '\0';
		fprintf(stdout, "%s", recvmsg);
		//fprintf(stdout, "loop");
		memset(&recvmsg, 0, rlen);
	}
	 
	return 0;
}
