#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SERVER_PORT 5432
#define MAX_LINE 256
int
main(int argc, char * argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE];
	int s;
	int len;
	char filename[20];
	if (argc==2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	}
	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}
	/* build address data structure */
	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);
	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	/* main loop: get and send lines of text */
	while (fgets(buf, sizeof(buf), stdin)) {
		printf("user input is:%s", buf);
		if(strlen(buf) > 6 && buf[0] == 'i' && buf[1] == 'W' && buf[2] == 'a' && buf[3] == 'n' && buf[4] == 't' && buf[5] == ' '){
			memset(filename, 0, 20);
			strncpy(filename, buf, strlen(buf)-1);
			printf("filename is:%s\n", filename+6);
		}
		buf[MAX_LINE-1] = '\0';
		len = strlen(buf) + 1;
		send(s, buf, len, 0);
		memset(buf, 0, MAX_LINE);
		while (len = recv(s, buf, sizeof(buf), 0)){
			if(buf[0] == '0'){
				//fail
				// printf("This is not a file");
				printf("From server: %s\n", buf+1);
			}else{
				printf("Receiving file from server...\n");
				// printf("I have receive: %s\n", buf+1);
				FILE *fp;
				fp = fopen(filename+6,"w+");
				fputs(buf+1, fp);
				fclose(fp);
				printf("Receiving file from server finished!\n");

			}
			break;

		}
	}
}