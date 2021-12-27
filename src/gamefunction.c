#include "init.h"

void PlayGame(int sockfd)
{
    char choice[MAX_LENGTH];
    do
    {
        ViewFunctionInGameScreen();
        scanf("%s", choice);
        fflush(stdin);
        // Join room
        if (strcmp(choice, "1") == 0)
        {
            system("clear");
            ViewWelcomeScreen();
            JoinRoom(sockfd);
            printf("\n_______Let's play________\n\n");
        }
        // Change Password
        else if (strcmp(choice, "2") == 0)
        {
            system("clear");
            printf("\n _________________Change Password_________________\n\n");
            if (ChangePassword(sockfd))
                PlayGame(sockfd);
            else
            {
                printf(" ____________Password is not exactly!!!___________\n\n");
                ChangePassword(sockfd);
            }
        }
        // Show Profile
        else if (strcmp(choice, "3") == 0)
        {
            system("clear");
            ShowProfile(sockfd);
        }
        // Show leaderboard
        else if (strcmp(choice, "4") == 0)
        {
            system("clear");
            printf("\n_______This is leaderboard________\n\n");
        }
        // Exit
        else if (strcmp(choice, "5") == 0)
        {
            system("clear");
            if (LogOut(sockfd))
            {
                system("clear");
                LoginGame(sockfd);
            }
            else
                PlayGame(sockfd);
        }
        else
            printf(" ___________We don't have this choice!!!__________\n\n");
    } while (choice != 5);
}

void JoinRoom(int sockfd)
{
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "3 ");
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    // Cannot join room cuz max player in room
    if (strcmp(messageClient, "-1") == 0){
        printf("Cannot join room cuz max player in this room\n\n");
        CountTime("Return play game screen", 5);
        system("clear");
    }
    // Host the room
    else if(strcmp(messageClient, "0") == 0){
        printf("You are host of the room, let's start game!\n\n");
        printf("        Press [S] to start game\n");
        printf("        Press [Q] to quit game!\n");
        printf(" Press any key to wait for more players...\n");
    }
    // Player in room
    else{
        printf("\n Game will be started by host\n");
        printf(" Press [Ctr + C] to quit game!\n");
        printf(" Press any key to wait for more players...\n");
    }
    int back = 0;
    do
    {
        back = Back("return", "continue stay here");
        if (back){
            system("clear");
            return PlayGame(sockfd);
        }
    } while (back != 1);
}

int ChangePassword(sockfd)
{
    char currentPassword[PASSWORD_MAX], newPassword[PASSWORD_MAX], renewPassword[PASSWORD_MAX];
    int check = 0;
    printf("Current password: ");
    scanf("%s", currentPassword);
    fflush(stdin);
    do
    {
        printf("New password: ");
        scanf("%s", newPassword);
        fflush(stdin);
        printf("Confirm new password: ");
        scanf("%s", renewPassword);
        fflush(stdin);
        if (strcmp(newPassword, renewPassword) == 0)
            check = 1;
        else
        {
            printf("New password and confirm new password not match!!!\n");
            if (Back("continue change password", "return") == 0)
            {
                system("clear");
                PlayGame(sockfd);
            }
            else
            {
                system("clear");
                printf("\n _________________Change Password_________________\n");
                printf("Current password: %s\n", currentPassword);
            }
        }
    } while (check != 1);

    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "4 %s %s", currentPassword, newPassword);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);

    if (strcmp(messageClient, "1") == 0)
    {
        printf(" ____________Change password success!!!___________\n");
        int back = 0;
        do
        {
            back = Back("return", "continue stay here");
            if (back)
                system("clear");
            else
            {
                system("clear");
                printf("\n _________________Change Password_________________\n\n");
                printf("Current password: %s\n", currentPassword);
                printf("New password: %s\n", newPassword);
                printf("Confirm new password: %s\n", newPassword);
                printf(" ____________Change password success!!!___________\n\n");
            }
        } while (back != 1);
        return 1;
    }
    else
        return 0;
}

void ShowProfile(int sockfd)
{
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "5 ");
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    char **arr = NULL;
    int count = string_split(messageClient, ' ', &arr);
    printf("\n _______________This is your profile______________\n");
    printf("|     Username: %-21s             |\n", arr[0]);
    printf("|     Password: %-21s             |\n", arr[1]);
    printf("|_________________________________________________|\n");
    freeMemory(arr, count);
    if (Back("return", "continue stay here"))
        system("clear");
    else
        ShowProfile(sockfd);
}

int LogOut(int sockfd)
{
    if (Back("logout", "continue"))
    {
        memset(messageClient, 0, sizeof(messageClient));
        sprintf(messageClient, "7 ");
        ClientSendMessageToServer(sockfd);
        ClientReceiveMessageFromServer(sockfd);
        if (strcmp(messageClient, "1") == 0)
            return 1;
        else
            return 0;
    }
    return 0;
}

int Back(char *want1, char *want2)
{
    char input[MAX_LENGTH];
    printf("\nDo you want %s?\n", want1);
    printf("Press [y] or [Y] to %s, other to %s: ", want1, want2);
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