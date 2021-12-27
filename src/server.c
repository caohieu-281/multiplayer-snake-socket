#include "init.h"

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

		// Join room
		else if (strcmp(arr[0], "3") == 0)
		{
			printf("JoinRoom\n");
			// Cannot join room cuz max player in room
			if(oneRoom.numberUsersInRoom == MAX_PLAYER){
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "-1");
				ServerSendToClient(socket);
			}
			else{
				// Host the room
				if(oneRoom.numberUsersInRoom == 0){
					printf("aaaa\n");
					memset(messageServer, 0, sizeof(messageServer));
					sprintf(messageServer, "0");
				}
				// Player in room
				else{
					memset(messageServer, 0, sizeof(messageServer));
					sprintf(messageServer, "1");
				}
				ServerSendToClient(socket);
				strcpy(oneRoom.usersInRoom[oneRoom.numberUsersInRoom].password, listUsers[thUserInList].password);
				strcpy(oneRoom.usersInRoom[oneRoom.numberUsersInRoom].username, listUsers[thUserInList].username);
				oneRoom.usersInRoom[oneRoom.numberUsersInRoom].socketID = listUsers[thUserInList].socketID;
				oneRoom.usersInRoom[oneRoom.numberUsersInRoom].score = listUsers[thUserInList].score;
				oneRoom.usersInRoom[oneRoom.numberUsersInRoom].status = listUsers[thUserInList].status;
				oneRoom.numberUsersInRoom++;
			}
		}

		// Change Password
		else if (strcmp(arr[0], "4") == 0)
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
		else if (strcmp(arr[0], "5") == 0)
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
		freeMemory(arr, count);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	readUserFromFile();
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