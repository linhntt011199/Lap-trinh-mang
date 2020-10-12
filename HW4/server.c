#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define MAXLINE 1024 

typedef struct Node 
{
    char username[50];
    char password[50];
    int status;
    struct Node *pNext;
} NODE;
NODE *head = NULL;

void insert(NODE **head, char username[], char password[], int status) {
    NODE *pNew = (NODE *) malloc (sizeof(NODE));
    strcpy(pNew->username, username);
    strcpy(pNew->password, password);
    pNew->status = status;
    pNew->pNext = *head;
    *head = pNew;
}

void openFile() {
    FILE *fptr;
    char str[255];
    int count = 0, status;
    fptr = fopen("nguoidung.txt", "r");

    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    while (fgets(str, 255, fptr) != NULL) {
        int first, last, j = 0;
        char username[255], password[255];
        for (int i = 0; i < strlen(str); i++) {
            if (str[i] == ' ') {
                first = i;
                break;
            }
            username[j ++] = str[i];
        }
        username[j] = '\0';
        j = 0;
        for (int i = first + 1; i < strlen(str); i++) {
            if (str[i] == ' ') {
                last = i;
                break;
            }
            password[j ++] = str[i];
        }
        password[j] = '\0';
        status = (int)str[last + 1] - 48;
        insert(&head, username, password, status);
    }
    fclose(fptr);
}

void printList(NODE *head) {
    FILE *fptr;
    fptr = fopen("nguoidung.txt", "w");

    NODE *tmp = head;
    while(NULL!=tmp){
        fprintf(fptr, "%s %s %d\n", tmp->username,tmp->password, tmp->status);
        tmp = tmp->pNext;
    }
    fclose(fptr);
}


int main(int argc, char *argv[]) {
    openFile();
    char *portNumber;
    int signedIn = 0;
    if (argc == 2) { 
        portNumber = argv[1];
        
    } else return 0;
    int sockfd, PORT = atoi(portNumber); 
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
        perror("Error: "); 
        return 0; 
    } 
      
    bzero(&servaddr, sizeof(servaddr)); 
    //memset(&cliaddr, 0, sizeof(cliaddr)); 
    servaddr.sin_family = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if (bind(sockfd, (const struct sockaddr *)&servaddr, 			sizeof(servaddr))) { 
        perror("Error: "); 
        return 0; 
    } 
    printf("Server started\n");
    char username[255], password[255];
    for ( ; ; ) {
    	int len, n; 
    	
    	len = sizeof(cliaddr);  //len is value/resuslt 
    	
    	
    	if (signedIn == 0) {
    	    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  			0, ( struct sockaddr *) &cliaddr, &len); 
    	    buffer[n] = '\0'; 
    	    if (n < 0) {
    	        perror("Error: "); 
    	        return 0;
    	    }
    	    strcpy(username, buffer);
    	    NODE *tmp = head;
    	    int flag = 0;
    	    while (NULL != tmp) {
                if (strcmp(tmp->username, username) == 0) {
            	    printf("[%s:%d]: %s\n", inet_ntoa(cliaddr.sin_addr), 			    ntohs(cliaddr.sin_port), "Insert password");
            	    flag = 1;
            	    int count = 0;
            	    do {
            		n = recvfrom(sockfd, (char *)buffer, MAXLINE,  		    	0, ( struct sockaddr *) &cliaddr, &len); 
    			buffer[n] = '\0';
    			strcpy(password, buffer);
            		if (strcmp(tmp->password, password) == 0) {
            	    	    if (tmp->status != 1) printf("[%s:%d]: %s\n", 						inet_ntoa(cliaddr.sin_addr), 						ntohs(cliaddr.sin_port), 						"Account not ready");
            	    	    else {
            	    		printf("[%s:%d]: %s\n", 						inet_ntoa(cliaddr.sin_addr), 						ntohs(cliaddr.sin_port), 						"OK");
            	    		signedIn = 1;
            	    		
            	    	    }		
            	            break;
                	} else {
            	    	    count ++;
            	    	    printf("[%s:%d]: %s\n", 						inet_ntoa(cliaddr.sin_addr), 						ntohs(cliaddr.sin_port), 						"not OK");
            	    		
            	 	}
            	    } while (count != 3);
            	if (count == 3) {
            		printf("[%s:%d]: %s\n", 						inet_ntoa(cliaddr.sin_addr), 						ntohs(cliaddr.sin_port), 						"Account is blocked");
            		tmp->status = 0;
                	printList(head);
            	}	
            	break;
             }
             tmp = tmp->pNext;
    	   }
    	   if (flag == 0) printf("[%s:%d]: %s\n", 				inet_ntoa(cliaddr.sin_addr), 		    			ntohs(cliaddr.sin_port), "Cannot find account");
    	}
    	if (signedIn == 1) {
    	    int flag;
    	    n = recvfrom(sockfd, (char *)buffer, MAXLINE,  			0, ( struct sockaddr *) &cliaddr, &len); 
    	    buffer[n] = '\0'; 
    	    
    	    if (strcmp(buffer, "bye") != 0) {
    	        flag = 0;
    	        for (int i = 0; i < strlen(buffer); i ++) {
    	    	     if ((buffer[i] >= 48 && buffer[i] <= 57) || 				(buffer[i] >= 65 && buffer[i] <= 90) || 				(buffer[i] >= 97 && buffer[i] <= 122)) {
    	    	    } else {
    	    	        printf("[%s:%d]: %s\n", 					inet_ntoa(cliaddr.sin_addr), 		    					ntohs(cliaddr.sin_port), "Error");
    	    	         flag = 1;
    	    	         break;
    	    	    }
    	        }
    	    	if (flag == 0) {
    	    	    char mahoa1[255], mahoa2[255];
    	    	    int m = 0, n = 0;
    	    	    
    	    	    for (int i = 0; i < strlen(buffer); i ++)
    	    	        if (buffer[i] >= 48 && buffer[i] <= 57) {
    	    	   	    mahoa1[m] = buffer[i];
    	    	   	    m ++;
    	    	        }
    	    	        else {
    	    	   	    mahoa2[n] = buffer[i];
    	    	   	    n ++;
    	    	        }
    	    	    mahoa1[m] = '\0';
    	    	    mahoa2[n] = '\0';
    	    	    printf("[%s:%d]: %s%s\n", 						inet_ntoa(cliaddr.sin_addr), 						ntohs(cliaddr.sin_port), 						mahoa1, mahoa2);
    	        }
    	    } else {
    	        printf("[%s:%d]: %s %s\n", inet_ntoa(cliaddr.sin_addr), 		    ntohs(cliaddr.sin_port), "Goodbye", username);
    	        return 0;
    	    }
        }
    }
    
    return 0;
}
