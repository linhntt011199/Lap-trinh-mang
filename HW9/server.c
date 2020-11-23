#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/wait.h>
#include <stdint.h>
  
#define MAXLINE 1024 

int count = 0;

char* itoalinh(int value, char* result, int base) {
        if (base < 2 || base > 36) { *result = '\0'; return result; }

        char* ptr = result, *ptr1 = result, tmp_char;
        int tmp_value;

        do {
            tmp_value = value;
            value /= base;
            *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
        } while ( value );
        if (tmp_value < 0) *ptr++ = '-';
        *ptr-- = '\0';
        while(ptr1 < ptr) {
            tmp_char = *ptr;
            *ptr--= *ptr1;
            *ptr1++ = tmp_char;
        }
        return result;
    }

void printFile(int id, char addr[MAXLINE], int port) {
    FILE *fptr;
    char fileName[MAXLINE] = "nguoidung";
    char dem[MAXLINE];
    itoalinh(id, dem, 10);
    strcat(fileName, dem);
    strcat(fileName, ".txt");
    fptr = fopen(fileName, "w");
    fprintf(fptr, "%s %d", addr, port);
    fclose(fptr);
}   


int main(int argc, char *argv[]) {
    int opt = 1;

    char *portNumber;
    if (argc == 2) { 
        portNumber = argv[1];
        
    } else return 0;
    
    int listenfd, connfd, n, PORT = atoi(portNumber);

    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    	perror("socket failed");
    	exit(EXIT_FAILURE);
    }

    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    	perror("bind failed");
    	exit(EXIT_FAILURE);
    }
    
    if (listen(listenfd, 3) < 0) {
    	perror("listen");
    	exit(EXIT_FAILURE);
    }
    
    printf("%s\n", "Server running...waiting for connections.");
    for (;;) {
    	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    	if (connfd < 0) exit(-1);
    	char buf[MAXLINE];
        int n;
        printf("%s\n", "Received request...");
        count ++;
        printFile(count, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
        char prin[MAXLINE];
        strcpy(prin, "Information added");
        send(connfd, prin, strlen(prin), 0);
        if (n < 0) {
            perror("Read error");
            exit(1);
        }
        close(connfd);
    } 
    close(listenfd);
    return 0;
 }
