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
    ViewSnake();
    ViewLogin();

    while (1)
    {
        int choice;
        scanf("%d", &choice);
        // int send_status = send(sockfd, response, sizeof(response), 0);
        if(choice == 1) 
        {   
            if(SignIn(sockfd))
                printf("Dang nhap thanh cong \n");
            else printf("Dang nhap that bai");
        }
        else if(choice == 2)
        {
            if(SignUp(sockfd))
                printf("Dang ky thanh cong \n");
            else printf("Dang ky that bai");
        }
    }

    close(sockfd);

    return 0;
}
