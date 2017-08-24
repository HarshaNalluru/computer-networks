#include <stdio.h>
#include <stdlib.h>      
#include <sys/socket.h>
#include <sys/types.h>    
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define ERROR -1
#define BUFFER 1024
#define PORT_NUMBER 9000
// ./client harshan:123456789@127.0.0.1
int main(int argc, char *argv[])
{
	struct sockaddr_in remote_server;
	int sock;
	char input[BUFFER];
	char output[BUFFER];
	int len;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == ERROR){
		perror("socket : ");
		exit(-1);
	}

	char * str = argv[1];
	//printf("%s\n", str);
	char * username = strtok(str,":");
	char * password = strtok(NULL, "@");
	char * ip_addr = strtok(NULL, "@");

	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(PORT_NUMBER);
	remote_server.sin_addr.s_addr = inet_addr(ip_addr);
	bzero(&remote_server.sin_zero, 8);

	if ((connect(sock, (struct sockaddr *)&remote_server, sizeof(struct sockaddr_in))) == ERROR){
		perror("connect : ");
		exit(-1);
	}

	send(sock, username, strlen(username), 0);
	len = recv(sock, output, BUFFER, 0);
	output[len] = '\0';
	printf("%s\n", output);

	send(sock, password, strlen(password), 0);
	len = recv(sock, output, BUFFER, 0);
	output[len] = '\0';
	printf("%s\n", output);

	fgets(input, BUFFER, stdin);
	send(sock, input, strlen(input), 0);

	len = recv(sock, output, BUFFER, 0);
	output[len] = '\0';
	printf("%s\n", output);	

	char thank[BUFFER] = "Thanks ";
	send(sock, thank, strlen(thank), 0);

	if (strcmp(output, "File Not Found") != 0){
		input[strlen(input)-1] = '\0';
		FILE* file = fopen(input, "w"); 

		int i=1;
		while(1){
			len = recv(sock, output, BUFFER, 0);
			output[len] = '\0';

			//printf("%s\n", output);	
			if (len==0){
				break;
			}
			fprintf(file, "%s", output);

			char success[BUFFER] = "Received Line ";
			send(sock, success, strlen(success), 0);
			//printf("%s : %d\n", success, i);	
			//printf(".\n");
			i++;
		}

		printf("------------------------%s Downloaded..!\n", input);
		fclose(file);
	}

	close(sock);

	return 0;
}