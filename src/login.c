#include "init.h"

int SignIn(int sockfd)
{
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "1 %s %s", username, password);

    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);

    if (strcmp(messageClient, "1") == 0)
        return 1;
    else
        return 0;
}

void SignUp(int sockfd)
{
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "2 %s %s", username, password);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    if (strcmp(messageClient, "1") != 0)
        return 1;
    else
        return 0;

}
