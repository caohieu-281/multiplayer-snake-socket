#include "init.h"

void GameFunction(int sockfd)
{
    char choice[MAX_LENGTH];
    do
    {
        ViewFunctionInGameScreen();
        scanf("%s", choice);
        fflush(stdin);
        // Create room
        if (strcmp(choice, "1") == 0)
        {
            system("clear");
            CreateRoom(sockfd);
            printf("----------------------Create Room------------------\n");
        }
        // Join waiting-room
        else if (strcmp(choice, "2") == 0)
        {
            system("clear");
            ViewWelcomeScreen();
            JoinRoom(sockfd);
            printf("\n_______Let's play________\n\n");
        }
        // Change Password
        else if (strcmp(choice, "3") == 0)
        {
            system("clear");
            printf("\n _________________Change Password_________________\n\n");
            if (ChangePassword(sockfd))
                GameFunction(sockfd);
            else
            {
                printf(" ____________Password is not exactly!!!___________\n\n");
                ChangePassword(sockfd);
            }
        }
        // Show Profile
        else if (strcmp(choice, "4") == 0)
        {
            system("clear");
            ShowProfile(sockfd);
        }
        // Show leaderboard
        else if (strcmp(choice, "5") == 0)
        {
            system("clear");
            printf("\n_______This is leaderboard________\n\n");
        }
        // Exit
        else if (strcmp(choice, "6") == 0)
        {
            system("clear");
            if (LogOut(sockfd))
            {
                system("clear");
                LoginGame(sockfd);
            }
            else
                GameFunction(sockfd);
        }
        else
            printf(" ___________We don't have this choice!!!__________\n\n");
    } while (choice != 6);
}

void CreateRoom(int sockfd)
{
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "3 ");
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    char **arr = NULL;
    int count = string_split(messageClient, ' ', &arr);
    // Cannot create room cuz max room
    if (strcmp(arr[0], "0") == 0)
    {
        printf("\nCannot create room cuz max room\n");
        CountTime("Return play game screen", 5);
        system("clear");
    }
    // Create success
    else
    {
        int play = 0;
        char command[MAX_LENGTH];
        do{
            printf("\n           Room Id: %s\n", arr[1]);
            printf("You are host of the room, let's start game!\n");
            printf("        Press [S] to start game\n");
            printf("        Press [Q] to quit game!\n");
            printf("====>");
            scanf("%s", command);
            memset(messageClient, 0, sizeof(messageClient));
            sprintf(messageClient, "15 %s %s", arr[1], command);
            ClientSendMessageToServer(sockfd);
            ClientReceiveMessageFromServer(sockfd);
            if(strcmp(messageClient, "start") == 0){
                play = 1;
                return InGamePlay(sockfd);
            }
            
        }while(play != 1);
    }
    
    int back = 0;
    do
    {
        back = Back("return", "continue stay here");
        if (back)
        {
            system("clear");
            memset(messageClient, 0, sizeof(messageClient));
            sprintf(messageClient, "10 %s", arr[1]);
            ClientSendMessageToServer(sockfd);
            ClientReceiveMessageFromServer(sockfd);
            return GameFunction(sockfd);
        }
    } while (back != 1);
}

void InGamePlay(int sockfd){
    printf("Playing game\n");
}

void JoinRoom(int sockfd)
{
    char inputRoomID[MAX_LENGTH];

    printf("Input room id to join room: ");
    scanf("%s", inputRoomID);
    memset(messageClient, 0, sizeof(messageClient));
    sprintf(messageClient, "4 %s", inputRoomID);
    ClientSendMessageToServer(sockfd);
    ClientReceiveMessageFromServer(sockfd);
    // Room not exist
    if (strcmp(messageClient, "-2") == 0)
    {
        printf("\nRoomID is not exist\n");
        CountTime("Return play game screen", 4);
        system("clear");
    }
    // Cannot join room cuz max player in room
    else if (strcmp(messageClient, "-1") == 0)
    {
        printf("\nCannot join room cuz max player in this room\n");
        CountTime("Return play game screen", 5);
        system("clear");
    }
    // Player in room
    else
    {
        memset(messageClient, 0, sizeof(messageClient));
        sprintf(messageClient, "14 %s", inputRoomID);
        ClientSendMessageToServer(sockfd);
        // printf("\n Game will be started by host\n");
        // printf(" Press [Ctr + C] to quit game!\n");
        // printf(" Press any key to wait for more players...\n");
        
        //// Tu thoat
        // int back = 0;
        // do
        // {
        //     back = Back("return", "continue stay here");
        //     if (back)
        //     {
        //         system("clear");
        //         memset(messageClient, 0, sizeof(messageClient));
        //         sprintf(messageClient, "10 ");
        //         ClientSendMessageToServer(sockfd);
        //         return GameFunction(sockfd);
        //     }
        // } while (back != 1);

        // Waiting mess from server to do something after
        int recvBytes;
        // Cho
        while ((recvBytes = recv(sockfd, messageClient, MESSAGE_MAX, 0)) > 0)
        {
            messageClient[recvBytes] = 0;
            char **arr = NULL;
            int count = string_split(messageClient, ' ', &arr);
            if (strcmp(messageClient, "-1") == 0)
            {
                printf("\nHost out room so you must out the room\n");
                CountTime("Return play game screen", 5);
                system("clear");
                return GameFunction(sockfd);
            }
            else if(strcmp(messageClient, "14")){
                system("clear");
                ViewWelcomeScreen();
                printf("\n_____________Room Id: %s_____________\n", inputRoomID);
                for(int i = 1; i<=atoi(arr[1]); i++){
                    printf("%s. %s\n", arr[2*i], arr[2*i+1]);
                }
                printf("_____________________________________\n");
                printf("\n Game will be started by host\n");
                printf(" Press [Ctr + C] to quit game!\n");
            }

            else if (strcmp(messageClient, "15") == 0)
            {
                InGamePlay(sockfd);
                break;
            }
        }
        
    }
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
                GameFunction(sockfd);
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
    sprintf(messageClient, "5 %s %s", currentPassword, newPassword);
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
    sprintf(messageClient, "6 ");
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