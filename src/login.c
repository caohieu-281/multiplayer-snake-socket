#include "init.h"

int SignIn(int sockfd)
{
    system("clear");
    printf("\nInput username and password to sign in\n");
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    printf("Username: ");
    scanf("%s", username);
    fflush(stdin);
    printf("Password: ");
    scanf("%s", password);
    fflush(stdin);

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

void SignUp(int sockfd)
{
    system("clear");
    printf("\nInput username and password to sign up\n");
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    printf("Username: ");
    scanf("%s", username);
    fflush(stdin);
    printf("Password: ");
    scanf("%s", password);
    fflush(stdin);
    
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "2 %s %s", username, password);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    if (strcmp(messageClient, "1") == 0)
    {
        printf("Sign up successful!!!\n");
        int back = 0;
        do
        {
            back = Back("sign in", "continue stay here");
            if (back)
            {
                system("clear");
                LoginGame(sockfd);
            }
            else
            {
                printf("\nInput username and password to sign up\n");
                printf("Username: %s\n", username);
                printf("Password: %s\n", password);
                printf("Sign up successful!!!\n");
            }
        } while (back != 1);
    }
    else
    {
        printf("\nSign up fail!!!\nAccount already exist!!!\n");
        int back = 0;
        do
        {
            back = Back("continue sign up", "return home page");
            if (back)
            {
                system("clear");
                SignUp(sockfd);
            }
            else
            {
                system("clear");
                LoginGame(sockfd);
            }
        } while (back != 1);
    }
}

void LoginGame(int sockfd)
{
    char choice[MAX_LENGTH];
    do
    {
        ViewWelcomeScreen();
        ViewLoginScreen();
        scanf("%s", choice);
        fflush(stdin);
        if (strcmp(choice, "1") == 0)
        {
            int status = SignIn(sockfd);
            if (status == 1)
            {
                system("clear");
                printf(" _______________Sign in successful!!!_____________\n");
                PlayGame(sockfd);
                break;
            }
            if (status == 0)
            {
                printf("Wrong password!!!\n\n");
                CountTime("Return home screen", 3);
                system("clear");
            }
            else
            {
                printf("User not exist!!!\n\n");
                CountTime("Return home screen", 3);
                system("clear");
            }
        }
        else if (strcmp(choice, "2") == 0)
            SignUp(sockfd);
        else if (strcmp(choice, "3") == 0)
        {
            if (Back("quit game", "continue"))
            {
                printf("\n________________Bye bye, see ya!!!_______________\n\n");
                close(sockfd);
                exit(0);
            }
            else
                LoginGame(sockfd);
        }
        else
        {
            printf("We don't have this choice!!!\n");
        }
    } while (strcmp(choice, "3") != 0);
}