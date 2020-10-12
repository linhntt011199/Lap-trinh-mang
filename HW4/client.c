#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define BUFF_SIZE 1024 
  
// Driver code 
int main(int argc, char *argv[]) { 
    char *ip_address, *port_number, SERV_ADDR[255];
    int PORT;
    if (argc == 3) {
    	ip_address = argv[1];
    	port_number = argv[2];
    	PORT = atoi(port_number);
    }
    else return 0;
    int sockfd; 
    struct sockaddr_in servaddr; 
  
    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        perror("Error: "); 
        return 0; 
        } 
  
    bzero(&servaddr, sizeof(servaddr)); 
    strcpy(SERV_ADDR, ip_address);
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    inet_aton(SERV_ADDR, &servaddr.sin_addr); 
      
    int n, len = sizeof(servaddr); 
    char buffer[BUFF_SIZE];
    for (;;) {
    	int flag = 0;
    	printf("Send to server: ");
    	fgets(buffer, sizeof(buffer), stdin);
    	char *tmp = strstr(buffer, "\n");
    	if (tmp != NULL) *tmp = '\0';
    	
    	for (int i = 0; i < strlen(buffer); i ++) 
    	    if (buffer[i] != ' ' || buffer[i] != '\0') {
    	    	flag = 1;
    	    	break;
    	    }
    	if (flag == 0) break; 
    	
	int sendBytes = sendto(sockfd, buffer, strlen(buffer), 0, 			(const struct sockaddr *)&servaddr, len); 
    	if (sendBytes < 0) {
    	    perror("Error: ");
    	    return 0;
    	}    
    }
  
    close(sockfd); 
    return 0; 
} 
