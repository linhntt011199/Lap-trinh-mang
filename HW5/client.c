#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define MAXLINE 1024 
#define PORT 5500

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	perror("Problem in creating the socket");
    	exit(2);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    
    if (connect(sockfd, (struct sockaddr *)&servaddr, 
    	sizeof(servaddr)) < 0) {
    	perror("Problem in connecting to the server");
    	exit(3);
    }
    
    while (fgets(sendline, MAXLINE, stdin) != NULL) {
    	send(sockfd, sendline, strlen(sendline), 0);
    	printf("Sent\n");
    	
    	/*if (recv(sockfd, recvline, MAXLINE, 0) == 0) {
    	    perror("The server terminated prematurely");
    	    exit(4);
    	} else {
    	    char *tmp = strstr(recvline, "\n");
    	    if (tmp != NULL) *tmp = '\0';
    	    printf("%s\n", recvline);
    	}*/
    }
    return 0;
}
