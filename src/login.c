#include "init.h"

int SignIn(int sockfd)
{
    printf("\nInput username and password to sign in\n");
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "1 %s %s", username, password);

    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    // Login success
    if (strcmp(messageClient, "1") == 0)
        return 1;
    // Wrong password
    if (strcmp(messageClient, "0") == 0)
        return 0;
    // User not exist
    else
        return -1;
}

int SignUp(int sockfd)
{
    printf("\nInput username and password to sign up\n");
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "2 %s %s", username, password);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    if (strcmp(messageClient, "1") == 0)
        return 1;
    else
        return 0;
}

void LoginGame(int sockfd)
{
    int choice;
    do
    {
        ViewLoginScreen();
        scanf("%d", &choice);
        if (choice == 1)
        {
            int status = SignIn(sockfd);
            if (status == 1)
            {
                PlayGame();
                break;
            }
            if (status == 0)
                printf("Wrong password!!!\n\n");
            else
            {
                printf("User not exist!!!\n");
                return LoginGame(sockfd);
            }
        }
        else if (choice == 2)
        {
            int signup;
            do
            {
                signup = SignUp(sockfd);
                if (signup)
                {
                    printf("Sign up successful!!!\n");
                    return LoginGame(sockfd);
                }
                else printf("Sign up fail!!!\nAccount already exist!!!\n");
            } while (signup != 1);
            break;
        }
        else if (choice == 3){
            printf("\n____________Bye bye, see ya!!!______________\n\n");
            exit(0);
        }
        else{
            printf("We don't have this choice!!!\n");
        }
    } while (choice != 3);
}