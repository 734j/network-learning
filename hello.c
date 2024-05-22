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

// BEEJ NETWORKING 5.1

int main (int argc, char *argv[]) {

    if (argc > 2 || argc == 1) {
        fprintf(stderr, "Too many or not enough arguments! \n");
        fprintf(stderr, "USAGE: getip [DOMAIN]\n");
        exit(1);
    }
    char ipstr[INET_ADDRSTRLEN]; // We store the IP address in text form here. INET_ADDRSTRLEN is the length of ipv4 addresses.
    int status;
	struct addrinfo hints; // Struct where we put information in to pass it to getaddrinfo.
    struct addrinfo *res; // The result of getaddrinfo. Not sure why its a pointer but ill figure that out later
    struct addrinfo *rescpy; // Another rescpy struct pointer. This one is used to point to the next element when iterating in a for loop.
    memset(&hints, 0, sizeof hints); // Set all values to 0 in hints.
    hints.ai_family = AF_INET; // Add the necessary info to ai_family, ai_socktype, ai_flags.
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    status = getaddrinfo(argv[1], "62222", &hints, &res); // get addrinfo. Address to hints and res passed.
    if (status != 0) {
        fprintf(stderr, "Error: %s\n", gai_strerror(status));
        exit(1);
    }

    // rescpy = res. Both point to the same result.
    // rescpy != NULL. If its NULL then the next struct in the linked list is the last one.
    // rescpy = rescpy->ai_next. After it did the loop it goes back, checks if its null. If not then rescpy is assigned the pointer to ai_next. And so on and so on.
    // A visual representation:
    // 
    // res --> GAI_RESULT_STRUCT (res points to the result addrinfo struct)
    // rescpy = res (BOTH POINT TO GAI_RESULT_STRUCT)
    // IF rescpy is not NULL 
    // 
    // rescpy = rescpy --> ai_next (Its easier for me to imagine this line with some different syntax: rescpy = rescpy.ai_next. This is technically not correct but this is for my eyes only.)
    // ai_next is a pointer to another addrinfo struct. Which is the next item in the linked list. rescpy = rescpy --> ai_next, this basically means we access the ai_next member
    // which is inside GAI_RESULT_STRUCT. We basically make rescpy point to the next item.
    //
    //

    for(rescpy = res ; rescpy != NULL ; rescpy = rescpy->ai_next) {
        struct sockaddr_in *sa1 = (struct sockaddr_in *)rescpy->ai_addr; // struct sockaddr_in sa1 pointer assigned ai_addr. sa1 now points to ai_addr.
                                                                     // ai_addr may be a sockaddr_in or a sockaddr_in6.
                                                                     // We cast it to sockaddr_in which is supposed to just work. 
        void *addr = &(sa1->sin_addr); // Address of sin_addr. sa1 now points to ai_addr so we access the item sin_addr and make addr point to it.
        inet_ntop(rescpy->ai_family, addr, ipstr, rescpy->ai_addrlen);

        fprintf(stdout, "%s\n", ipstr);

    }

    freeaddrinfo(res);
    
    return 0;

}
