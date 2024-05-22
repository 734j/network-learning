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

#define _FAIL_EXIT -1

// Bind section of beej networking

int main () {

    struct addrinfo hints;
    struct addrinfo *res;
    int sockfd;
    int status;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    status = getaddrinfo(NULL, "61000", &hints, &res);
    if(status != 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(status));
        exit(_FAIL_EXIT);
    } 

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        exit(_FAIL_EXIT);
    }

    int bstatus;
    bstatus = bind(sockfd, res->ai_addr, res->ai_addrlen);

    fprintf(stdout, "%d\n", bstatus);
    return 0;

}
