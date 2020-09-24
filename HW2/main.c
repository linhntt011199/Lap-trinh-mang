#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <netdb.h>

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
        if ((accum = accum * 10 + *ipAddress - '0') > 255) return 0;
        chcnt++;
        ipAddress++;
    }
    if (segs != 3) return 0;
    if (chcnt == 0) return 0;
    return 1;
}

void getIPAddress(char *domainName) {
    struct hostent* pHostInfo;
    long nHostAddress;
    pHostInfo = gethostbyname(domainName);
    if(pHostInfo){
        
        char *address = inet_ntoa(*(struct in_addr *)pHostInfo->h_name);
        printf("%s\n", address);
        return;
    }
    printf("Not found information\n");
}

int main(int argc, char *argv[]) {
    if (argc == 3) { 
        if (strcmp(argv[1], "1") == 0) {
            if (isValidIpAddress(argv[2]) == 1) printf("OK\n");
            else printf("Wrong parameter\n");
        } else {
            if (isValidIpAddress(argv[2]) == 1) printf("Wrong parameter\n");
            else getIPAddress(argv[2]);
        }
        
    } else return 0;
}