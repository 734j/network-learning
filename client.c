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
#define __FAIL EXIT_FAILURE
#define BACKLOG 2
#define __PORT "62000"
#define BUF_SZ_2 2048

int talk_to_server (int sockfd) {
	
	while (1) {
        char editbuffer[BUF_SZ_2];
        fprintf(stdout, ": ");

        char *fgs = fgets(editbuffer, BUF_SZ_2, stdin);
		socklen_t fgs_len = strlen(editbuffer);
		if (fgs == NULL) {
			return 0;
		}

        if (editbuffer[0] == '\n') { 
			send(sockfd, editbuffer, fgs_len, 0);
			continue;
		} else {
			send(sockfd, editbuffer, fgs_len, 0);
			continue;
		}
	}
	return 0;
}

int main (int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "USAGE: %s [IP]\n", argv[0]);
		exit(__FAIL);
	}
	
	int sockfd = -1;
	int gai_result;
	int connect_result = -1;
	struct addrinfo hints;
	struct addrinfo *res;
	struct addrinfo *p;
	memset(&hints, 0, sizeof(hints));
	//hints.ai_flags = ;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	gai_result = getaddrinfo(argv[1], __PORT, &hints, &res);
	if (gai_result != 0) {
		fprintf(stderr, "getaddrinfo: %d\n", gai_result);
		exit(__FAIL);
	}

	for (p = res ; p != NULL ; p = p->ai_next) {

		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			fprintf(stderr, "socket fail\n");
			continue;
		}
		
		connect_result = connect(sockfd, p->ai_addr, p->ai_addrlen);
		if (connect_result == -1) {
			fprintf(stderr, "connect fail\n");
			continue;
		}

	}
	if (connect_result == -1 || sockfd == -1) {
		exit(__FAIL);
	}

	freeaddrinfo(res);
	talk_to_server(sockfd);
	
	return 0;
}
