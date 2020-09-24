#include <stdio.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    if (argc == 3) { 
        if (strcmp(argv[1], "1") == 0) {
            if (isValidIpAddress(argv[2]) == 1) printf("OK\n");
            else printf("Wrong parameter\n");
        } else {
            if (isValidIpAddress(argv[2]) == 1) printf("Wrong parameter\n");
            else printf("OK\n");
        }
        
    } else return 0;
}