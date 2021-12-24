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
		printf("String receive from client: %s\n", messageServer);
		count = string_split(messageServer, ' ', &arr);
		if (strcmp(arr[0], "1") == 0)
		{
			int check = checkSignIn(arr[1], arr[2], socket);
			if (check == 1)
			{
				strcpy(messageServer, "1");
			}
            else if (check == -1)
			{
				strcpy(messageServer, "-1");
			}
			else
			{
				strcpy(messageServer, "0");
			}
			ServerSendToClient(socket);
		}
		else if(strcmp(arr[0], "2") == 0)
		{
			int check = checkSignIn(arr[1], arr[2], socket);
			if (check == -1)
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
		else if(strcmp(arr[0], "3") == 0){
			for (int i = 0; i < numberUsers; i++) {
				if (listUsers[i].socketID == socket && listUsers[i].status == 1) {
					if(strcmp(listUsers[i].password, arr[1]) == 0){
						strcpy(listUsers[i].password, arr[2]);
						memset(messageServer, 0, sizeof(messageServer));
						sprintf(messageServer, "1");
					}
					else{
						memset(messageServer, 0, sizeof(messageServer));
						sprintf(messageServer, "0");
					}
					ServerSendToClient(socket);
					break;
				}
			}
		}
		else if(strcmp(arr[0], "5") == 0) {
			for (int i = 0; i < numberUsers; i++) {
				if (listUsers[i].socketID == socket && listUsers[i].status == 1) {
					memset(messageServer, 0, sizeof(messageServer));
					sprintf(messageServer, "%s %s", listUsers[i].username, listUsers[i].password);
					ServerSendToClient(socket);
					break;
				}
			}
		}
		else if(strcmp(arr[0], "7") == 0) {
			if(strcasecmp(arr[1], "y") == 0) {
				for (int i = 0; i < numberUsers; i++) {
					if (listUsers[i].socketID == socket && listUsers[i].status == 1) {
						listUsers[i].status = 0;
						memset(messageServer, 0, sizeof(messageServer));
						sprintf(messageServer, "1");
						ServerSendToClient(socket);
						break;
					}
				}
			}
			else {
				memset(messageServer, 0, sizeof(messageServer));
				sprintf(messageServer, "0");
				ServerSendToClient(socket);
			}
		}
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