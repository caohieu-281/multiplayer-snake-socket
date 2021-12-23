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
        int status;
        if(choice == 1)
        {
            do
            {
                status = SignIn(sockfd);
                if (status == 1)
                {
                    printf("Login successful!!!\n\n");
                    break;
                }
                if (status == 0)
                    printf("Wrong password!!!\n\n");
                else
                {
                    printf("User not exist!!!\n");
                    printf("If you dont have account, please press [s] to sign up!!!\n");
                    printf("Or press any key to continue sign in: ");
                    char *s;
                    scanf("%s", s);
                    if(strcasecmp(s, "s") == 0) {
                        choice = 2;
                        break;
                    }
                }     
            }while(status != 1);
            break;
        }
        else if (choice == 2)
        {
            printf("sign up le\n");
            break;
        }
    }

    close(sockfd);

    return 0;
}