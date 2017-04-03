#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#define SERVER_PORT 5432
#define MAX_PENDING 5
#define MAX_LINE 256
int main()
{
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
	int s, new_s;
	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);
	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("simplex-talk: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);
	/* wait for connection, then receive and print text */
	while(1) {
		if ((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0) {
			perror("simplex-talk: accept");
			exit(1);
		}
		while (len = recv(new_s, buf, sizeof(buf), 0)){
			printf("buf is %s\n", buf);
			if(strlen(buf) > 6 && buf[0] == 'i' && buf[1] == 'W' && buf[2] == 'a' && buf[3] == 'n' && buf[4] == 't' && buf[5] == ' '){
				printf("format is iWant\n");
				printf("what is going on\n");
				//check whether the file is exist
			    char temp[20];
			    memset(temp, 0, 20);
			    strcpy(temp, buf+6);
			    

				char filename[20];
				memset(filename, 0, 20);

				strncpy(filename, temp, strlen(temp)-1);
				printf("buf+6 is %s\n", buf+6);
				// printf("filename is %s\n", filename);
				FILE* f;
				// I have some difficulty to open a file.....
				f = fopen(filename, "r");
				// f = fopen("test.txt", "r");
				if(f!=NULL){
					printf("file exists\n");

					//read the file to a char array
					fseek(f, 0, SEEK_END);
					long fsize = ftell(f);
					fseek(f, 0, SEEK_SET);  //same as rewind(f);

					char *string = malloc(fsize + 2);
					string[0] = '1';
					fread(string+1, fsize, 1, f);
					fclose(f);

					string[fsize+1] = 0;
					printf("file content is %s\n", string);

					//send the file out
					send(new_s, string, fsize, 0);

				}else{
					// printf("file doesn't exist\n");
					char* res = "0file doesn't exist!";
					send(new_s, res, strlen(res), 0);

				}
			}else{
				// printf("That just ain't right!\n");
				char* res = "0That just ain't right!";
				send(new_s, res, strlen(res), 0);
			}
		    memset(buf, 0, MAX_LINE);
		}
		close(new_s);
	}
}
//when sending something, the first char is 0 or 1.




