#include <stdio.h>
#include <stdlib.h>      
#include <sys/socket.h>
#include <sys/types.h>    
#include <netinet/in.h>
#include <error.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define ERROR -1
#define MAX_CLIENTS 2
#define MAX_DATA 1024
#define PORT_NUMBER 9000

int main(int argc, char const *argv[])
{
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock;
	int new;
	int sockaddr_len = sizeof(struct sockaddr_in);
	int data_len;
	int temp_len;
	char data[MAX_DATA];
	char temp[MAX_DATA];
	char username[MAX_DATA];
	char password[MAX_DATA];

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR){
		perror("server socket: ");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT_NUMBER);
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero, 8);

	if ((bind(sock, (struct sockaddr *)&server, sockaddr_len)) == ERROR){
		perror("bind : ");
		exit(-1);
	}

	if ((listen(sock, MAX_CLIENTS)) == ERROR){
		perror("listen");
		exit(-1);
	}

	while(1){
		if ((new = accept(sock, (struct sockaddr *)&client, &sockaddr_len)) == ERROR){
			perror("accept");
			exit(-1);
		}

		printf("New client connected from port no. %d and IP %s\n", ntohs(client.sin_port), inet_ntoa(client.sin_addr) );
		
		char const* const filename = "users.txt";
		FILE* file = fopen(filename, "r"); 
	    char line[256];
	    data_len = 1;
	    data_len = recv(new, username, MAX_DATA, 0);
	    char * id;
	    char * pass;
	    int auth = 0;

	    while (fgets(line, sizeof(line), file)) {
	        id = strtok(line,"@");
	        pass = strtok(NULL,"@");
	        if (pass[strlen(pass)-1]=='\n'){
	        	pass[strlen(pass)-1] = '\0';
	        }
	        if (strcmp(username,id) == 0){

	        	char userok[MAX_DATA] = "Checking for the password...";
	        	send(new, userok, strlen(userok), 0);
	    		data_len = recv(new, password, MAX_DATA, 0);
	        	
	        	if (strcmp(password,pass)==0){
	        		auth = 1;
	        		char *success = malloc(strlen("Hello ")+strlen(username)+1);
	        		strcpy(success, "Hello ");
    				strcat(success, username);
		        
		        	send(new, success, strlen(success), 0);
		        	break;
	        	}
	        	else{
		        	char failed[MAX_DATA] = "Authentication Failure1!!!";
		        	send(new, failed, strlen(failed), 0);

					printf("Client disconnected\n");
					close(new);
	        	}
	        }
	    }

	    fclose(file);

	    if (auth!=1){
        	char failed[MAX_DATA] = "Authentication Failure!!!";
        	send(new, failed, strlen(failed), 0);
			
			printf("Client disconnected\n");
			close(new);
	    }
	    else{
			data_len = 1;
			
			data_len = recv(new, data, MAX_DATA, 0);
			data[data_len-1] = '\0';
			char const* const filename = data;

			FILE* file = fopen(filename, "r"); 
			if (file){	
				char download[MAX_DATA] = "Download Initiated...";
				send(new, download , strlen(download), 0);
				
				data_len = recv(new, data, MAX_DATA, 0);
				data[data_len-1] = '\0';

				while (fgets(line, sizeof(line), file)) {
					send(new, line, strlen(line), 0);
					//printf("Sent Line : %s\n", line);
					temp_len = recv(new, temp, MAX_DATA, 0);
					//printf("Client says : %s\n", temp);
					//printf("%s", line);
				}
				fclose(file);
			}
			else{
				char download[MAX_DATA] = "File Not Found";
				send(new, download , strlen(download), 0);
			}

			printf("Client disconnected\n");
			close(new);
	    }
	}
	return 0;
}