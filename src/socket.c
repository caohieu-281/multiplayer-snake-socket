#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 4096

int ClientCreateSocket(int port, char *ip)
{
    int sockfd;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    // Specify an address for the socket
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address)); 
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(ip);

    // Connect the client to the server socket
    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("ERROR connect to server");
        return;
    }

    return sockfd;
}