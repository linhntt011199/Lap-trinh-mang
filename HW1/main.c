#include<stdio.h>
#include<stdlib.h>
#include<string.h> 

typedef struct Node 
{
    char username[50];
    char password[50];
    int status;
    struct Node *pNext;
} NODE;
NODE *head = NULL;
char userSignedIn[10][50];
int numSignedIn = 0, num = 0;

void insert(NODE **head, char username[], char password[], int status) {
    NODE *pNew = (NODE *) malloc (sizeof(NODE));
    strcpy(pNew->username, username);
    strcpy(pNew->password, password);
    pNew->status = status;
    pNew->pNext = *head;
    *head = pNew;
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

void dangky() {
    char username[255], password[255];
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
        
        insert(&head, username, password, 2);
        printf("Successful registration. Activation required.\n");
        FILE *fptr;
        fptr = fopen("nguoidung.txt", "r+");
        fseek(fptr, 0, SEEK_END);
        //fputs(username, fptr);
        fprintf(fptr, "\n%s %s %d", username, password, 2);
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
                if (strcmp(tmp->username, username) == 0) tmp->status = 1;
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
                    if (strcmp(tmp->username, username) == 0) tmp->status = 0;
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
                    strcpy(userSignedIn[numSignedIn], tmp->username);
                    numSignedIn ++;
                    num = numSignedIn;
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
    printf("Username: ");
    scanf("%s", username);
    if (numSignedIn > 0) {
        
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
        
        
    } else printf("Account is not sign in\n");
    
}

void changePassword() {
    char username[255], oldpassword[255], newPassword[255];
    int flag2 = 0;
    printf("Username: ");
    scanf("%s", username);
    
    for (int i = 0; i < numSignedIn; i ++)
        if (strcmp(username, userSignedIn[i]) == 0) {
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
                    } else printf("Current password is incorrect. Please try again\n");
                    break;
                }
                tmp = tmp->pNext;
            }
            flag2 = 1;
            break;
        }
    if (flag2 == 0) printf("Account is not sign in\n");
}

void signOut() {
    char username[255];
    int flag1 = 0, flag2 = 0;
    printf("Username: ");
    scanf("%s", username);
    if (num == 0) {
        printf("Account is not sign in\n");
        numSignedIn = num;
        return;
    }
    
    
    NODE *tmp = head;
    while (NULL != tmp) {
        if (strcmp(tmp->username, username) == 0) {
            for (int i = 0; i < numSignedIn; i ++)
                if (strcmp(username, userSignedIn[i]) == 0) {
                    printf("Goodbye %s\n", username);
                    flag1 = 1;
                    strcpy(userSignedIn[i], "");
                    num --;
                    break;
                }
            if (flag1 == 0) printf("Account is not sign in\n");
            flag2 = 1;
            break;
            
        }
        tmp = tmp->pNext;
    }
    if (flag2 == 0) printf("Cannot find account\n");
}

void showMenu() {
    int choice;
    char c[255];
    printf("USER MANAGEMENT PROGRAM\n");
    printf("-----------------------\n");
    printf("1. Register\n");
    printf("2. Activate\n");
    printf("3. Sign in\n");
    printf("4. Search\n");
    printf("5. Change password\n");
    printf("6. Sign out\n");
    do {
        printf("Your choice (1-6, other to quit): ");
        scanf("%s", c);
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
        default:
            return;
        }
    } while (choice >= 1 && choice <= 6);
    
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
 
int main() {
    openFile();
    showMenu();
    
    return 0;
 
}
