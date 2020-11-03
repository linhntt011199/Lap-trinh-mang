#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <pthread.h>
  
#define MAXLINE 1024 

char msg[500];

void *recvmg(void *my_sock) {
    int sock = *((int *)my_sock);
    int len;
    while ((len = recv(sock, msg, 500, 0)) > 0) {
        msg[len] = '\0';
        fputs(msg, stdout);
    }
}

int main(int argc, char **argv) {
    char *ip_address, *port_number, SERV_ADDR[255];
    int PORT;
    if (argc == 3) {
    	ip_address = argv[1];
    	port_number = argv[2];
    	PORT = atoi(port_number);
    } else return 0;

    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];
    pthread_t recvt;
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    	perror("Problem in creating the socket");
    	exit(2);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
    servaddr.sin_port = htons(PORT);
    
    if (connect(sockfd, (struct sockaddr *)&servaddr, 
    	sizeof(servaddr)) < 0) {
    	perror("Problem in connecting to the server");
    	exit(3);
    }
    pthread_create(&recvt, NULL, (void *)recvmg, &sockfd);
    while (fgets(sendline, MAXLINE, stdin) != NULL) {
    	char *tmp = strstr(sendline, "\n");
    	if (tmp != NULL) *tmp = '\0';
        int flag = 0;
    	for (int i = 0; i < strlen(sendline); i ++) 
    	    if (sendline[i] != ' ' && sendline[i] != '\0')  {
    	    	flag = 1;
    	    	break;
    	    }
    	if (flag == 0) return 0; 
    	int len = send(sockfd, sendline, strlen(sendline), 0);
    	if (len < 0) {
    	    printf("\n message not send \n");
    	}
    	int n = recv(sockfd, recvline, MAXLINE, 0);
    	if (n == 0) {
    	    perror("The server terminated prematurely");
    	    exit(4);
    	    return 0;
    	} else {
    	    recvline[n] = '\0';
    	    printf("%s\n", recvline);
    	}
    }
    pthread_join(recvt, NULL);
    close(sockfd);
    return 0;
}
