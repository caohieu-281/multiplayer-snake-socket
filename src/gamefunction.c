#include "init.h"

void PlayGame(int sockfd)
{
    char choice[MAX_LENGTH];
    do
    {
        ViewFunctionInGameScreen();
        scanf("%s", choice);
        fflush(stdin);
        if (strcmp(choice, "1") == 0)
        {
            printf("\n_______Let's play________\n\n");
        }
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
        else if (strcmp(choice, "3") == 0)
        {
            system("clear");
            ShowProfile(sockfd);
        }
        else if (strcmp(choice, "4") == 0)
        {
            system("clear");
            printf("\n_______This is leaderboard________\n\n");
        }
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
            if (Back("continue change password", "return") == 0){
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
    sprintf(messageClient, "3 %s %s", currentPassword, newPassword);
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
    sprintf(messageClient, "5");
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