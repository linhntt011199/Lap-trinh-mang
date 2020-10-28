#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/wait.h>
  
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

void sig_chld(int signo) {
    	pid_t pid;
    	int stat;
    	pid = waitpid(-1, &stat, WNOHANG);
    	printf("child %d terminated\n", pid);
    }


int main(int argc, char *argv[]) {
    openFile();
    char *portNumber;
    int signedIn = 0;
    pid_t pid;
    if (argc == 2) { 
        portNumber = argv[1];
        
    } else return 0;
    
    int listenfd, connfd, n, PORT = atoi(portNumber);
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    char buf[MAXLINE];
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    	perror("socket failed");
    	exit(EXIT_FAILURE);
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if (bind(listenfd, (struct sockaddr *)&servaddr, 
    	sizeof(servaddr)) < 0) {
    	perror("bind failed");
    	exit(EXIT_FAILURE);
    }
    
    if (listen(listenfd, 3) < 0) {
    	perror("listen");
    	exit(EXIT_FAILURE);
    }
    
    printf("%s\n", "Server running...waiting for connections.");
    signal(SIGCHLD, sig_chld);
    for (;;) {
    	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
    	if (connfd < 0) exit(-1);
    	if ( (pid = fork()) == 0) {
    	    printf("%s\n", "Received request...");
    	    char username[255], password[255];
    	    
    	    while ((n = recv(connfd, buf, MAXLINE, 0)) > 0) {
    	        char prin[MAXLINE];
    	    	buf[n] = '\0';
    	    	if (signedIn == 0) {
    	    	    strcpy(username, buf);
    	    	    NODE *tmp = head;
    	    	    int flag = 0;
    	    	    while (NULL != tmp) {
                        if (strcmp(tmp->username, username) == 0) {
            	            strcpy(prin, "Insert password");
            	            send(connfd, prin, strlen(prin), 0);
            	    	    flag = 1;
            	    	    int count = 0;
            	    	    do {
            		    	n = recv(connfd, buf, MAXLINE, 0); 
            		    	buf[n] = '\0';
    			    	strcpy(password, buf);
            		    	if (strcmp(tmp->password, password) == 0) {
            	    	            if (tmp->status != 1) { 
            	    	               strcpy(prin, "Account not ready");
            	        	       send(connfd, prin, strlen(prin), 0);
            	        	     }
            	    	    	 else { 
            	    	    	     strcpy(prin, "OK");
            	        	     send(connfd, prin, strlen(prin), 0);
            	    		     signedIn = 1;
            	    		 }		
            	            	break;
                	    	} else {
            	    	            count ++;
            	    	            strcpy(prin, "not OK\n");
            	        	    send(connfd, prin, strlen(prin), 0);
            	        	    if (count != 3) {
            	        	        strcpy(prin, "Insert password");
            	        	    	send(connfd, prin, strlen(prin), 0);
            	        	    }
            	    	       }
            	    	} while (count != 3);
            	 	if (count == 3) {
            		    strcpy(prin, "Account is blocked");
            	            send(connfd, prin, strlen(prin), 0);
            		    tmp->status = 0;
                	    printList(head);
            		}	
            		break;
             	    }
             	tmp = tmp->pNext;
    	   	}
    	   	if (flag == 0) { 				
    	   	    strcpy(prin, "Wrong account");
            	    send(connfd, prin, strlen(prin), 0);}
    	    }
    	    
    	    else {
    	        int flag;
    	    	if (strcmp(buf, "bye") != 0) {
    	            flag = 0;
    	            for (int i = 0; i < strlen(buf); i ++) {
    	    	    	if ((buf[i] >= 48 && buf[i] <= 57) || 				(buf[i] >= 65 && buf[i] <= 90) || 					(buf[i] >= 97 && buf[i] <= 122)) {
    	    	    	} else {
    	    	            strcpy(prin, "Error");
            	            send(connfd, prin, strlen(prin), 0);
    	    	            flag = 1;
    	    	            break;
    	    	    	}
    	            }
    	    	    if (flag == 0) {
    	    	        char mahoa1[255], mahoa2[255];
    	    	        int m = 0, k = 0;
    	    	        for (int i = 0; i < strlen(buf); i ++)
    	    	            if (buf[i] >= 48 && buf[i] <= 57) {
    	    	   	    	mahoa1[m] = buf[i];
    	    	   	    	m ++;
    	    	            }
    	    	            else {
    	    	   	    	mahoa2[k] = buf[i];
    	    	   	    	k ++;
    	    	            }
    	    	    	mahoa1[m] = '\0';
    	    	    	mahoa2[k] = '\0';
    	    	    	strcat(mahoa1, mahoa2);
    	    	    	strcpy(prin, mahoa1);
            	        send(connfd, prin, strlen(prin), 0);
    	           }
    	        } else {
    	            
    	            strcpy(prin, "Goodbye ");
    	            strcat(prin, username);
    	            send(connfd, prin, strlen(prin), 0);
    	            signedIn = 0;
    	            //return 0;
    	        }
            }
    	}
    	if (n < 0) {
    	    perror("Read error");
    	    exit(1);
    	}
    	close(connfd);
    } }
    close(listenfd);
    return 0;
 }
