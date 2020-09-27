#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

void getDomainName(char *IPAddress) {
	struct in_addr addr;
	if (inet_aton(IPAddress, &addr) == 0) {
        	printf("Not found information\n");
        	return;
    	}
    	struct hostent *remoteHost;
    	remoteHost = gethostbyaddr(&addr, 4, AF_INET);
    	if (remoteHost == NULL) {
    		printf("Not found information\n");
    		return;
    	}
    	
    	printf("Official name: %s\n", remoteHost->h_name);
    	printf("Alias name:\n");
    	char **pAlias;
    	for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
            printf("%s\n", *pAlias);
        }
}

int main(int argc, char *argv[]) {
    if (argc == 3) { 
        if (strcmp(argv[1], "1") == 0) {
            if (isValidIpAddress(argv[2]) == 1) getDomainName(argv[2]);
            else printf("Wrong parameter\n");
        } else {
            if (isValidIpAddress(argv[2]) == 1) printf("Wrong parameter\n");
            else getIPAddress(argv[2]);
        }
        
    } else return 0;
}
