#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct Node 
{
    char username[50];
    char password[50];
    int status;
    char homepage[50];
    struct Node *pNext;
} NODE;
NODE *head = NULL;
char userSignedIn[50];

void insert(NODE **head, char username[], char password[], int status, char homepage[]) {
    NODE *pNew = (NODE *) malloc (sizeof(NODE));
    strcpy(pNew->username, username);
    strcpy(pNew->password, password);
    strcpy(pNew->homepage, homepage);
    pNew->status = status;
    pNew->pNext = *head;
    *head = pNew;
}

void printList(NODE *head) {
    FILE *fptr;
    fptr = fopen("nguoidung.txt", "w");

    NODE *tmp = head;
    while(NULL!=tmp){
        fprintf(fptr, "%s %s %d %s\n", tmp->username,tmp->password, tmp->status, tmp->homepage);
        tmp = tmp->pNext;
    }
    fclose(fptr);
}

void dangky() {
    char username[255], password[255], homepage[255];
    int status, flag = 0;
    printf("Username: ");
    scanf("%s", username);  
 
    NODE *tmp = head;
    while (NULL != tmp) {
        if (strcmp(tmp->username, username) == 0) {
            printf("Account existed\n");
            flag = 1;
            break;
        }
        tmp = tmp->pNext;
    }
    if (flag == 0) {
        printf("Password: ");
        scanf("%s", password);
        printf("Home page: ");
        scanf("%s", homepage);
        
        insert(&head, username, password, 2, homepage);
        printf("Successful registration. Activation required.\n");
        FILE *fptr;
        fptr = fopen("nguoidung.txt", "r+");
        fseek(fptr, 0, SEEK_END);
        fprintf(fptr, "\n%s %s %d %s", username, password, 2, homepage);
        fclose(fptr);
    }
}

void activate() {
    char username[255], password[255], code[255];
    printf("Username: ");
    scanf("%s", username);
    NODE *tmp = head;
    int flag = 0;
    while (NULL != tmp) {
        if (strcmp(tmp->username, username) == 0) {
            flag = 1;
            break;
        }
        tmp = tmp->pNext;
    }
    if (flag == 0) {
        printf("Cannot find account\n");
        return;
    }
    printf("Password: ");
    scanf("%s", password);
    int count = 0;
    do {
        printf("Code: ");
        scanf("%s", code);
        if (strcmp(code, "LTM121216") == 0) {
            printf("Account is activated.\n");
            NODE *tmp = head;
            while (NULL != tmp) {
                if (strcmp(tmp->username, username) == 0) {
                	tmp->status = 1;
                	printList(head);
                	break;
                }
                tmp = tmp->pNext;
            }     
            break;
        } else {
            count ++;
            if (count > 3) {
                printf("Activation code is incorrect.\n");
                printf("Account is blocked\n");
                NODE *tmp = head;
                while (NULL != tmp) {
                    if (strcmp(tmp->username, username) == 0) {
                    	tmp->status = 0;
                    	printList(head);
                    	break;
                    }
                    tmp = tmp->pNext;
                }
                break;
             } else printf("Account is not activated\n");  
        }
    } while (count <= 3);
}

void signin() {
    char username[255], password[255];
    printf("Username: ");
    scanf("%s", username);
    NODE *tmp = head;
    int flag = 0;
    while (NULL != tmp) {
        if (strcmp(tmp->username, username) == 0) {
            flag = 1;
            if (tmp->status == 0) {
                printf("Account is blocked\n");
                break;
            }
            
            int count = 0;
            do {
                printf("Password: ");
                scanf("%s", password);
                if (strcmp(tmp->password, password) == 0) {
                    strcpy(userSignedIn, tmp->username);
                    printf("Hello %s\n", tmp->username);
                    break;
                }
                else {
                    count ++;
                    printf("Password is incorrect\n");
                    if (count > 3) {
                        printf("Password is incorrect. Account is blocked\n");
                        tmp->status = 0;
                        printList(head);
                        break;
                    }
                    
                }
                
            } while (count <= 3);
        }
        tmp = tmp->pNext;
    }
    if (flag == 0) printf("Cannot find account\n");
}

void search() {
    char username[255];
    if (strcmp(userSignedIn, "") == 0) {
    	printf("Account is not sign in\n");
    	return;
    }
    printf("Username: ");
    scanf("%s", username);
    int flag1 = 0, flag2 = 0;
        
    NODE *tmp = head;
    while (NULL != tmp) {
        if (strcmp(tmp->username, username) == 0) {
            if (tmp->status == 0) printf("Account is blocked\n");
            if (tmp->status == 1) printf("Account is active\n");
            flag1 = 1;
            break;
        }
        tmp = tmp->pNext;
    }
    if (flag1 == 0) printf("Cannot find account\n");
}

void changePassword() {
    char username[255], oldpassword[255], newPassword[255];
    int flag2 = 0;
    
    
   if (strcmp("", userSignedIn) == 0) {
   	printf("Account is not sign in\n");
   	return;
   }
   printf("Username: ");
   scanf("%s", username);
   printf("Password: ");
   scanf("%s", oldpassword);
   printf("NewPassword: ");
   scanf("%s", newPassword);
   NODE *tmp = head;
   while (NULL != tmp) {
   	if (strcmp(tmp->username, username) == 0) {
                if (strcmp(tmp->password, oldpassword) == 0) {
                    strcpy(tmp->password, newPassword);
                    printf("Password is changed\n");
                    printList(head);
                } else printf("Current password is incorrect. Please try again\n");
                break;
        }
        tmp = tmp->pNext;
    }
}

void signOut() {
    char username[255];
    int flag1 = 0, flag2 = 0;
    printf("Username: ");
    scanf("%s", username);

    
    if (strcmp("", userSignedIn) == 0) {
        printf("Account is not sign in\n");
        return;
    }
        
    NODE *tmp = head;
    while (NULL != tmp) {
        if (strcmp(tmp->username, username) == 0) {
            if (strcmp(username, userSignedIn) == 0) {
            	printf("Goodbye %s\n", username);
            	strcpy(userSignedIn, "");
            }
            else printf("Account is not sign in\n");
            flag1 = 1;
            break;
        }
        tmp = tmp->pNext;
    }
    if (flag1 == 0) printf("Cannot find account\n");
}

int isValidIpAddress(char *ipAddress) {
    int segs = 0, chcnt = 0, accum = 0;  
    if (ipAddress == NULL) return 0;
    while (*ipAddress != '\0') {
        if (*ipAddress == '.') {
            if (chcnt == 0) return 0;
            if (++segs == 4) return 0;
            chcnt = accum = 0;
            ipAddress++;
            continue;
        }
        if ((*ipAddress < '0') || (*ipAddress > '9')) return 0;
        //if ((accum = accum * 10 + *ipAddress - '0') > 255) return 0;
        chcnt++;
        ipAddress++;
    }
    //if (segs != 3) return 0;
    if (chcnt == 0) return 0;
    return 1;
}

void getDomainName(char *IPAddress) {
	struct in_addr addr;
	if (inet_aton(IPAddress, &addr) == 0) {
        	printf("Not found information\n");
        	return;
    	}
    	struct hostent *remoteHost = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    	if (remoteHost == NULL) {
    		printf("Not found information\n");
    		return;
    	}
    	
    	printf("Official name: %s\n", remoteHost->h_name);
    	printf("Alias name:\n");
    	for (int i = 0; remoteHost->h_aliases[i] != NULL; i ++) printf("%s\n", remoteHost->h_aliases[i]);
}

void domainName() {
    if (strcmp(userSignedIn, "") == 0) {
	printf("Account is not sign in\n");
	return;
    }
    NODE *tmp = head;
    while (NULL != tmp) {
        if (strcmp(tmp->username, userSignedIn) == 0) {
        	if (isValidIpAddress(tmp->homepage) == 1) getDomainName(tmp->homepage);
            	else printf("%s\n", tmp->homepage);
        	break;
        }
        tmp = tmp->pNext;
    }
}

void getIPAddress(char *domainName) {
    struct hostent* pHostInfo;
    long nHostAddress;
    pHostInfo = gethostbyname(domainName);
    if(pHostInfo){
        
        char *address = inet_ntoa(*(struct in_addr *)pHostInfo->h_addr);
        printf("Official IP: %s\n", address);
        printf("Alias IP: \n");
        int i = 1;
        while(pHostInfo->h_addr_list[i] != NULL) {
        	char *address = inet_ntoa(*(struct in_addr *)pHostInfo->h_addr_list[i]);
        	printf("%s\n", address);
        	i ++;
        }
        return;
    }
    printf("Not found information\n");
}

void IPAddress() {
    if (strcmp(userSignedIn, "") == 0) {
	printf("Account is not sign in\n");
	return;
    }
    NODE *tmp = head;
    while (NULL != tmp) {
        if (strcmp(tmp->username, userSignedIn) == 0) {
        	if (isValidIpAddress(tmp->homepage) == 0) getIPAddress(tmp->homepage);
            	else printf("%s\n", tmp->homepage);
        	break;
        }
        tmp = tmp->pNext;
    }
}

void showMenu() {
    int choice;
    char c[255], cc[255];
    printf("USER MANAGEMENT PROGRAM\n");
    printf("-----------------------\n");
    printf("1. Register\n");
    printf("2. Activate\n");
    printf("3. Sign in\n");
    printf("4. Search\n");
    printf("5. Change password\n");
    printf("6. Sign out\n");
    printf("7. Homepage with domain name\n");
    printf("8. Homepage with IP address\n");
    do {
        printf("Your choice (1-8, other to quit): ");
        scanf(" %[^\n]", c);
        //scanf("%s", c);
        if (strlen(c) > 1) return;
        if (c[0] < 48 || c[0] > 57) return;
        choice = c[0] - 48;
        switch (choice) {
        case 1: dangky(); break;
        case 2: activate(); break;
        case 3: signin(); break;
        case 4: search(); break;
        case 5: changePassword(); break;
        case 6: signOut(); break;
        case 7: domainName(); break;
        case 8: IPAddress(); break;
        default:
            return;
        }
    } while (choice >= 1 && choice <= 8);
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
        char username[255], password[255], homepage[255];
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
        j = 0;
        for (int i = last + 3; i < strlen(str) - 1; i ++) homepage[j ++] = str[i];
        homepage[j] = '\0';
        insert(&head, username, password, status, homepage);
    }
    fclose(fptr);
}
 
int main() {
    openFile();
    showMenu();
    
    return 0;
 
}
