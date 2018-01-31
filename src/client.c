/*
	An implementation of Random Chat TCP client
	@Author Giovanni Giglio (@GioGiglio)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXLINE 512

#define ANSI_COLOR_RED     "\x1b[91m"
#define ANSI_COLOR_GREEN   "\x1b[92m"
#define ANSI_COLOR_YELLOW  "\x1b[93m"
#define ANSI_COLOR_BLUE    "\x1b[94m"
#define ANSI_COLOR_MAGENTA "\x1b[95m"
#define ANSI_COLOR_CYAN    "\x1b[96m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Change the following to suit your prefs

#define MY_MSG_COLOR 		ANSI_COLOR_BLUE
#define OTHERS_MSG_COLOR	ANSI_COLOR_GREEN


void error(char *msg){
	fprintf(stderr, "--- %s \n",msg);
	exit(1);
}

void close_and_exit(int sockfd){
	printf(ANSI_COLOR_RED "--- Random chat quitted \n" ANSI_COLOR_RESET);
	close(sockfd);
	exit(0);
}

int main(int argc, char **argv){
	int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char msg[MAXLINE];
    int users, turns_to_write;

    if (argc < 3)		error("ERROR: Please provide the Server's IP and the port number");

    portno = atoi(argv[2]);
    server = gethostbyname(argv[1]);
    if (server == NULL)		error("ERROR: No such host");

    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)		error("ERROR: Cannot create the socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to server
    n = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (n < 0)		error("ERROR: Cannot connect to server");

    // Wait for server to write the CHAT-INFO message.
    memset(msg, 0, MAXLINE);
    n = read(sockfd, msg, MAXLINE-1);
    if (n < 0)		error("ERROR: Cannot read from the socket\nCHAT-INFO message not read");

    sscanf(msg,"%d %d",&users, &turns_to_write);

    while(1){

    	// Read until it's the turn to write
    	while(turns_to_write > 0){
    		memset(msg, 0, MAXLINE);

    		n = read(sockfd, msg, MAXLINE-1);
    		if (n < 0)		error("ERROR: Cannot read from the socket");

    		if(!strcmp(msg,"QUIT\n"))		close_and_exit(sockfd);

    		printf(OTHERS_MSG_COLOR "%s" ANSI_COLOR_RESET, msg);
    		turns_to_write--;
    	}

    	// Now it's the turn to write
    	memset(msg, 0, MAXLINE);

    	printf("your message: " MY_MSG_COLOR);
    	fgets(msg, MAXLINE-1, stdin);
    	printf("\n" ANSI_COLOR_RESET);

    	n = write(sockfd, msg, strlen(msg));
    	if (n < 0)		error("ERROR: Cannot write to the socket");

    	if(!strcmp(msg,"QUIT\n"))		close_and_exit(sockfd);

    	// Reset the turns to write to users -1
    	turns_to_write = users -1;
    }
}