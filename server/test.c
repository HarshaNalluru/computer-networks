#include <stdio.h>
#include <stdlib.h> 
#include <string.h>    
#define BUFFER 1024
int main(int argc, char const *argv[])
{
	char input[BUFFER];
	char line[256];
	fgets(input, BUFFER, stdin);
	input[strlen(input)-1]='\0';
	FILE* file = fopen("file1.txt", "r"); 

	FILE* file2 = fopen(input, "w"); 
	
	while (fgets(line, sizeof(line), file)) {
		/*send(new, line, strlen(line), 0);
		printf("Sent Line : %s\n", line);

		temp_len = recv(new, temp, MAX_DATA, 0);
		printf("Client says : %s\n", temp);*/
		fprintf(file2, "%s", line);
	}
	fclose(file);
	fclose(file2);
	return 0;
}