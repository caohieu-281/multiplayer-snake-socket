#include "init.h"

void *connection_handler(int *client_socket)
{
	int socket = *(int *)client_socket;
	int send_status;
	int read_len;
	// int play = 0;
	int roomIdPub = -1;
	begin:
	while ((read_len = recv(socket, messageServer, MESSAGE_MAX, 0)) > 0)
	{
		int count;
		char **arr = NULL;
		messageServer[read_len] = 0;
		printf("String receive from client %d: %s\n", socket, messageServer);
		count = string_split(messageServer, ' ', &arr);
		int thUserInList = SearchUser(listUsers, socket, 1);
		// Sign in
		if (strcmp(arr[0], "1") == 0)
		{
			int check = checkSignIn(arr[1], arr[2], socket);
			if (check == 1)
				strcpy(messageServer, "1");
			else if (check == -1)
				strcpy(messageServer, "-1");
			else
				strcpy(messageServer, "0");
			ServerSendToClient(socket);
		}

		// Sign up
		else if (strcmp(arr[0], "2") == 0)
		{
			int check = checkSignUp(arr[1], arr[2], socket);
			if (check)
			{
				addUser(arr[1], arr[2]);
				writeUserToFile();
				strcpy(messageServer, "1");
				ServerSendToClient(socket);
			}
			else
			{
				strcpy(messageServer, "0");
				ServerSendToClient(socket);
			}
		}

		// Create room
		else if (strcmp(arr[0], "3") == 0)
		{
			// Cannot create room cuz max room
			if (numberRooms == MAX_ROOM)
			{
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "0 ");
				ServerSendToClient(socket);
			}
			// Create success
			else
			{
				int roomID = listUsers[thUserInList].socketID;
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "1 %d", roomID);
				ServerSendToClient(socket);
				strcpy(listRooms[numberRooms].usersInRoom[0].password, listUsers[thUserInList].password);
				strcpy(listRooms[numberRooms].usersInRoom[0].username, listUsers[thUserInList].username);
				listRooms[numberRooms].usersInRoom[0].socketID = roomID;
				listRooms[numberRooms].usersInRoom[0].score = listUsers[thUserInList].score;
				listRooms[numberRooms].usersInRoom[0].status = listUsers[thUserInList].status;
				listRooms[numberRooms].numberUsersInRoom = 1;
				listRooms[numberRooms].roomID = roomID;
				// listRooms[numberRooms].map_size = (HEIGHT + 10) * (WIDTH + 10) * sizeof(listRooms[numberRooms].game_map[0][0]);
				numberRooms++;
			}
		}

		// Join waiting-room
		else if (strcmp(arr[0], "4") == 0)
		{
			int thRoom = SearchRoom(atoi(arr[1]));

			// Room not exist
			if (thRoom == -1)
			{
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "-2");
				ServerSendToClient(socket);
			}
			else
			{
				// Cannot join room cuz max player in room
				if (listRooms[thRoom].numberUsersInRoom == MAX_PLAYER)
				{
					memset(messageServer, 0, sizeof(messageServer));
					sprintf(messageServer, "-1");
					ServerSendToClient(socket);
				}
				// Join room successful
				else
				{
					memset(messageServer, 0, sizeof(messageServer));
					sprintf(messageServer, "1");
					ServerSendToClient(socket);
					strcpy(listRooms[thRoom].usersInRoom[listRooms[thRoom].numberUsersInRoom].password, listUsers[thUserInList].password);
					strcpy(listRooms[thRoom].usersInRoom[listRooms[thRoom].numberUsersInRoom].username, listUsers[thUserInList].username);
					listRooms[thRoom].usersInRoom[listRooms[thRoom].numberUsersInRoom].socketID = listUsers[thUserInList].socketID;
					listRooms[thRoom].usersInRoom[listRooms[thRoom].numberUsersInRoom].score = listUsers[thUserInList].score;
					listRooms[thRoom].usersInRoom[listRooms[thRoom].numberUsersInRoom].status = listUsers[thUserInList].status;
					listRooms[thRoom].numberUsersInRoom++;
				}
			}
		}

		// Change Password
		else if (strcmp(arr[0], "5") == 0)
		{
			if (strcmp(listUsers[thUserInList].password, arr[1]) == 0)
			{
				strcpy(listUsers[thUserInList].password, arr[2]);
				writeUserToFile();
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "1");
			}
			else
			{
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "0");
			}
			ServerSendToClient(socket);
		}

		// Show profile
		else if (strcmp(arr[0], "6") == 0)
		{
			memset(messageServer, 0, sizeof(messageServer));
			sprintf(messageServer, "%s %s", listUsers[thUserInList].username, listUsers[thUserInList].password);
			ServerSendToClient(socket);
		}

		// Logout
		else if (strcmp(arr[0], "7") == 0)
		{
			listUsers[thUserInList].status = 0;
			memset(messageServer, 0, sizeof(messageServer));
			sprintf(messageServer, "1");
			ServerSendToClient(socket);
		}

		// // Out room
		// else if (strcmp(arr[0], "10") == 0)
		// {
		// 	UserOutRoom(atoi(arr[1]), socket);
		// }
		// Show list room
		else if (strcmp(arr[0], "13") == 0)
		{
			if (numberRooms == 0)
			{
				printf("No room\n");
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "-3 ");
				ServerSendToClient(socket);
			}
			else
			{
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "13 %d", numberRooms);
				for (int i = 0; i < numberRooms; i++)
				{
					sprintf(messageServer, "%s %d %d", messageServer, listRooms[i].roomID, listRooms[i].numberUsersInRoom);
				}
				ServerSendToClient(socket);
			}
		}
		// Refresh Screen Waiting Room
		else if (strcmp(arr[0], "14") == 0)
		{
			RefreshScreenWaitingRoom(atoi(arr[1]));
		}
		// > 15 is in game play
		// Play game or out game
		else if (strcmp(arr[0], "15") == 0)
		{
			if (strcasecmp(arr[2], "s") == 0)
			{
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "start");
				ServerSendToClient(socket);
				MakeGame(atoi(arr[1]));
				CreateMap(atoi(arr[1]));
				roomIdPub = atoi(arr[1]);
				// break;
			}
			if (strcasecmp(arr[2], "q") == 0)
			{
				UserOutRoom(atoi(arr[1]), socket);
			}
		}
		else if (strcmp(arr[0], "16") == 0)
		{
			roomIdPub = atoi(arr[1]);
			break;
		}
		freeMemory(arr, count);
	}

	// play_game(roomIdPub, socket);
	PlayGame(roomIdPub, socket);	

	while ((read_len = recv(socket, messageServer, MESSAGE_MAX, 0)) > 0) {
		if (read_len <= 0)
			break;
		if (strcmp(messageServer, "17") == 0) goto begin;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	readUserFromFile();
	numberRooms = 0;
	int server_socket = ServerCreateSocket(atoi(argv[1]));
	int no_threads = 0;

	pthread_t threads[MAX_USER];
	while (no_threads < MAX_USER)
	{
		printf("Listening...\n");
		int client_socket = accept(server_socket, NULL, NULL);
		puts("Connection accepted");
		if (pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) < 0)
		{
			perror("Could not create thread");
			return 1;
		}
		if (client_socket < 0)
		{
			printf("server acccept failed...\n");
			exit(0);
		}
		else
			printf("Server acccept the client: client_socket %d\n", client_socket);
		puts("Handler assigned");
		no_threads++;
	}
	int k = 0;
	for (k = 0; k < MAX_USER; k++)
	{
		pthread_join(threads[k], NULL);
	}

	close(server_socket);
	return 0;
}
