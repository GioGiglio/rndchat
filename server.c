/*
	An implementation of a Random Chat TCP relay server
	@Author Giovanni Giglio (@GioGiglio)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAXLINE 512
#define MAXUSERS 3

void error(char *msg){
	fprintf(stderr,"--- %s \n",msg);
	exit(1);
}

int main(int argc, char **argv){
	int welcomesockfd, sockfd[MAXUSERS], clilen[MAXUSERS];
	int portno, n, index;
	char msg[MAXLINE];
	struct sockaddr_in serv_addr, cli_addr[MAXUSERS];
	pid_t pid;

	if (argc < 2)		error("ERROR: Please provide the port number as an argument");

	portno = atoi(argv[1]);
	index = 0;

	// Create the welcome socket
	welcomesockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (welcomesockfd < 0)		error("Error while creating socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the welcome socket to the port number
    n = bind(welcomesockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (n < 0)		error("Error while binding socket to the provided port number");

    puts("Server up and running");

    // Listen for connections
    listen(welcomesockfd, 1);

    while(1){

    	// Accept all connetions
    	while(index < MAXUSERS){
    		clilen[index] = sizeof(cli_addr);
    		sockfd[index] = accept(welcomesockfd, (struct sockaddr *)&cli_addr[index], (socklen_t *)&clilen[index]);
    		if (sockfd[index] < 0)		error("Error while accepting connection from one host");
    		index++;
    	}

    	index = 0;

    	// Send the chat-info message to all the users
    	int i=0;
    	for(; i<MAXUSERS; i++){
    		char chatinfo[8];
    		sprintf(chatinfo, "%d %d",MAXUSERS, i); 
    		n = write(sockfd[i], chatinfo, strlen(chatinfo));
    		if (n < 0)		error("Error while writing to client\nCHAT-INFO message not sent");
    	}

    	pid = fork();

    	// Child process handles the chat between users
    	if (pid == 0){
    		close(welcomesockfd);

    		while(1){
    			memset(msg, 0, MAXLINE);

    			// Read from the @index user
    			n = read(sockfd[index], msg, MAXLINE-1);
    			if (n < 0)		error("Error while reading from socket");

    			// Write to all the other users, except @index

    			int i=0;
    			for(; i < MAXUSERS; i++){
    				if (i == index)		continue;

    				n = write(sockfd[i], msg, strlen(msg));
    				if (n < 0)		error("Error while writing to socket");
    			}
    			// 	Increment @index so that the other users can write too 
				//	or set it to 0 to restart the loop 	
    			index = (index == MAXUSERS-1)? 0: index+1;

    			// Break if a user writes "QUIT"
    			if(!strcmp(msg,"QUIT\n"))	break;
    		}
    		// Close all @sockfd[] connections
	    	i=0;
	    	for(; i < MAXUSERS; i++){
	    		close (sockfd[i]);
	    	}

	    	puts("--- Random chat closed");

    		// IMPORTANT: kill the process
    		exit(0);
    	}

    	// Parent process waits for new connections from @welcomesockfd

    	// Close all @sockfd[] connections
    	i=0;
    	for(; i < MAXUSERS; i++){
    		close (sockfd[i]);
    	}
    }
    return 0;
}