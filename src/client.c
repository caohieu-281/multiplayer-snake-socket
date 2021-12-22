#include<stdio.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "login.h"
#include "socket.h"

#define MAXLINE 4096   /*max text line length*/

int main(int argc, char *argv[])
{
    Login();
    int sockfd = ClientCreateSocket(atoi(argv[1]), argv[2]);
    char response[256];
        //receive data from the server
    while(1){
        printf("\nInput (Enter 'q' or 'Q' to quit): ");
        memset(response, 0, sizeof(response));
        if(fgets(response, MAXLINE, stdin) == NULL)
            break;
        if(strlen(response) == 1)
            break;
        response[strlen(response) - 1] = '\0';
        if(strcasecmp(response, "q") == 0)
            break;

        int byteSend = send(sockfd, response, strlen(response), 0);
        if (recv(sockfd, response, MAXLINE, 0) == 0)
        {
            perror("The server terminated prematurely");
            exit(4);
        }
        printf("%s\n%s", "String received from the server: ", response);
    }
    close(sockfd);
    return 0;
}