#include <netinet/in.h>
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //for exit(0);
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h>	//hostent
#include<arpa/inet.h>

void convert_ip_to_hostname(char* ip)
{
    struct sockaddr_in sa;    
    socklen_t len;         
    char hbuf[NI_MAXHOST];

    memset(&sa, 0, sizeof(struct sockaddr_in));

    
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    len = sizeof(struct sockaddr_in);

    if (getnameinfo((struct sockaddr *) &sa, len, hbuf, sizeof(hbuf), 
        NULL, 0, NI_NAMEREQD)) {
        printf("could not resolve hostname\n");
    }
    else {
        printf("host = %s\n", hbuf);
    }                                            
}

int convert_hostname_to_ip(char * hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;

	int i;
	char test[100];
	if ( (he = gethostbyname( hostname ) ) == NULL) 
	{
		// get the host info
		herror("gethostbyname");
		return 1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;
	
	for(i = 0; addr_list[i] != NULL; i++) 
	{
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		strcat(ip,"\n");
	}

	
	return 1;
}

//int hostname_to_ip(char *  , char *);

int main(int argc , char *argv[])
{
	if(argc <3)
	{
		printf("Please provide a hostname to resolve \n");
		exit(1);
	}
	
	if (strcmp(argv[1],"1")){
		convert_ip_to_hostname(argv[2]);
	}
	else if (atoi(argv[1])==2){
		
		char *hostname = argv[2];
		char ip[1000];
		
		convert_hostname_to_ip(hostname , ip);
		printf("%s resolved to %s" , hostname , ip);
		printf("\n");
	} else
	{
		printf("command not found pls check again !\n");
	}
	
	
}




