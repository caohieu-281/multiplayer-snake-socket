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
            printf("\n _________________Change Password_________________\n\n");
            if(ChangePassword(sockfd))
                PlayGame(sockfd);
            else{
                printf(" ____________Password is not exactly!!!___________\n\n");
                ChangePassword(sockfd);
            }
        }
        else if (choice == 3)
        {
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

int ChangePassword(sockfd) {
    char oldPassword[PASSWORD_MAX], newPassword[PASSWORD_MAX], renewPassword[PASSWORD_MAX];
    int check = 0;
    printf("Current password: ");
    scanf("%s", oldPassword);
    fflush(stdin);
    do {
        printf("New password: ");
        scanf("%s", newPassword);
        fflush(stdin);
        printf("Confirm new password: ");
        scanf("%s", renewPassword);
        fflush(stdin);
        if(strcmp(newPassword, renewPassword) == 0)
            check = 1;
        else
            printf("New password and confirm new password not match!!!\n\n");
    }while(check != 1);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "3 %s %s", oldPassword, newPassword);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);

    if (strcmp(messageClient, "1") == 0){
        printf(" ____________Change password success!!!___________\n\n");
        return 1;
    }
    else
        return 0;
}

void ShowProfile(int sockfd) {
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "5");
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    char **arr = NULL;
    int count = string_split(messageClient, ' ', &arr);
    printf("\n _______________This is your profile______________\n");
    printf("|     Username: %-21s             |\n", arr[0]);
    printf("|     Password: %-21s             |\n", arr[1]);
    printf("|_________________________________________________|\n\n");
}

int LogOut(int sockfd)
{
    int choice;
    char input[MAX_LENGTH];
    printf("Do you want logout?\n");
    printf("Press [y] or [Y] to logout, other to continue: ");
    scanf("%s", input);
    fflush(stdin);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "7 %s", input);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);

    if (strcmp(messageClient, "1") == 0)
        return 1;
    else
        return 0;
}

int Return(char *want)
{
    char input[MAX_LENGTH];
    printf("Do you want %s?\n", want);
    printf("Press [y] or [Y] to %s, other to continue: ", want);
    scanf("%s", input);
    fflush(stdin);
    if (strcasecmp(input, "y") == 0)
        return 1;
    else
    {
        system("clear");
        return 0;
    }
}