#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "login.h"
#include "socket.h"


int main(int argc, char *argv[])
{
    int sockfd = ClientCreateSocket(atoi(argv[1]));
    while (1)
    {
        ViewSnake();
        ViewLogin();
        int choice;
        scanf("%d", &choice);
        // int send_status = send(sockfd, response, sizeof(response), 0);
        if(choice == 1) 
        {   
            SignIn(sockfd);

        }
        break;
        if(choice == 2)
            break;
    }

    close(sockfd);

    return 0;
}
