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

int main (void) {

	int sockfd;
    struct addrinfo *res;
    struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int result = getaddrinfo("192.168.1.18", "62000", &hints, &res);
	if (result != 0) {
		exit(0);
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	int connect_res = connect(sockfd, res->ai_addr, res->ai_addrlen);
	if (connect_res == -1) {
		fprintf(stderr, "connect_res: -1\n");
		exit(1);
	}
	
	
    return 0;  
}
