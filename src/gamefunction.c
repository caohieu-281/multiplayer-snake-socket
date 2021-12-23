#include "init.h"

void PlayGame(int sockfd)
{
    int choice;
    do
    {
        ViewFunctionInGameScreen();
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("\n_______Let's play________\n\n");
        }
        else if (choice == 2)
        {
            printf("\n_______Change Password________\n\n");
        }
        else if (choice == 3)
        {
            printf("\n_______This is your profile________\n\n");
            ShowProfile(sockfd);
        }
        else if (choice == 4)
        {
            printf("\n_______This is leaderboard________\n\n");
        }
        else if (choice == 5)
        {
            if(LogOut(sockfd))
            {
                LoginGame(sockfd); 
            }
            else
                PlayGame(sockfd);
        }
        else
        {
            printf("We don't have this choice!!!\n");
        }
    } while (choice != 5);
}
int LogOut(int sockfd)
{
    int choice;
    char input[MAX_LENGTH];
    printf("Do you want logout?\n");
    printf("Press [y] or [Y] to logout, other to continue: ");
    scanf("%s", input);
    fflush(stdin);
    if(strcasecmp(input, "y") == 0)
        return 1;
    else
        return 0;
}

void ShowProfile(int sockfd) {
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "5");
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    printf("Info from server %s\n", messageClient);
}