#include "init.h"

int SignIn(int sockfd)
{
    char username[USERNAME_MAX], password[PASSWORD_MAX];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    memset(message, 0, sizeof(message));
    sprintf(message, "1 %s %s",username, password);

    ClientSendMessageToServer(sockfd);
    printf("asdasdasda\n");
    ClientReceiveMessageFromServer(sockfd);
    printf("%s\n", message);

    if(strcmp(message, "1"))
        return 1;
    else 
        return 0;
}

