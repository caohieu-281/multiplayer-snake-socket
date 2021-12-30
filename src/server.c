#include "init.h"

pthread_mutex_t map_lock = PTHREAD_MUTEX_INITIALIZER;
int map_size = (HEIGHT + 10) * (WIDTH + 10) * sizeof(game_map[0][0]);

void *connection_handler(int *client_socket)
{
	int socket = *(int *)client_socket;
	int send_status;
	int read_len;
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
				numberRooms++;
				for (int i = 0; i < numberRooms; i++)
				{
					printf("room id %d\n", listRooms[i].roomID);
					printf("number room %d\n", listRooms[i].numberUsersInRoom);
				}
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
				break;
			}
			if (strcasecmp(arr[2], "q") == 0)
			{
				UserOutRoom(atoi(arr[1]), socket);
			}
		}
		freeMemory(arr, count);
	}

	int i;
	// MakeGame(atoi(arr[1]));
	//Set game borders
	for (i = 0; i < HEIGHT; i++)
		game_map[i][0] = game_map[i][WIDTH - 2] = BORDER;
	for (i = 0; i < WIDTH; i++)
		game_map[0][i] = game_map[HEIGHT - 1][i] = BORDER;

	//Randomly add five fruit
	srand(time(NULL));
	for (i = 0; i < 5; i++)
		AddFruit(map_lock);
	for (i = 0; i < 3; i++)
		AddWall(map_lock);
	for (i = 0; i < 2; i++)
		AddWall2(map_lock);

	//Find three consecutive zeros in map for starting snake position
	int head_y, head_x;
	srand(time(NULL));
	do
	{
		head_y = rand() % (HEIGHT - 6) + 3;
		head_x = rand() % (WIDTH - 6) + 3;
	} while (!(((game_map[head_y][head_x] == game_map[head_y + 1][head_x]) == game_map[head_y + 2][head_x]) == 0));

	//Variables for user input
	char key = UP;
	char key_buffer;
	char map_buffer[map_size];
	int bytes_sent, n;

	//Copy map to buffer, and send to client
	memcpy(map_buffer, game_map, map_size);
	bytes_sent = 0;
	while (bytes_sent < map_size)
	{
		bytes_sent += send(socket, game_map, map_size, 0);
		if (bytes_sent < 0)
			error("ERROR writing to socket");
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