//Các file header
//For socket()
#include <sys/types.h>
#include <sys/socket.h>
//For inet_addr()
#include <netinet/in.h>
#include <arpa/inet.h>
// For ..
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

struct myStruct {
        int a,b;
    }myStruct1;
   

int checkNumber(char* str){
    int i;
    if(strlen(str) == 0) return 0;
    for(i = 0; i< strlen(str); i++){
        if(!(str[i] >= '0' && str[i] <= '9'))
            return 0;
    }
    return 1;
}

int checkString_Number(char *str){
    int i;
    if(strlen(str) == 0) return 0;
    for(i =0; i< strlen(str); i++){
        if(!((str[i] >= '0' && str[i] <= '9') ||
            (str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
            return 0;
    }
    return 1;
}


int main(int argc, char* argv[])
{

    int recvBytes;



    if(argc != 2){
        printf("Error: sai cu phap\n");
        printf("Cu phap: ./server <port-number>\n");
        return 0;
    }
    
    if(checkNumber(argv[1]) == 0){
        printf("Error: parameter1 (port) phai la so\n");
        return 0;
    }
    
   
    //printList(root);
    int port = atoi(argv[1]);

    // Tao server socket
    int serverSocket =  socket(PF_INET,SOCK_STREAM,0);
    if (serverSocket == -1)
    {
        perror("CREATE SOCKET");
        exit(0);
    }
    // Set reuse option
    int i = 1;
    int check_bind = setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int));
    if (check_bind == -1)
    {
	perror("Set reuse");
    }
    // Binding to a address
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
        // Neu chay trong mang thi phai dung :
        // serverAddress.sin_addr.s_addr = inet_addr(address);
	socklen_t len = sizeof(struct sockaddr_in);
	check_bind = bind(serverSocket,(struct sockaddr*)&serverAddress,len);
    printf("%d\n",serverSocket);
	if (check_bind == -1)
        {
            perror("BIND");
            exit(0);
        }
        //Gio ta cho server lang nghe ket noi
        check_bind = listen(serverSocket,3);
	if (check_bind == -1)
	{
		perror("Listen");
		exit(1);
	}
        struct sockaddr_in clientAddress;
        struct timeval timeout;
        len = sizeof(struct sockaddr_in);
        fd_set readfds;
        fd_set masterfds; // tập readfds để check các socket, 1 tập để lưu lại nhưng thay đổi của tập readfds.
        FD_ZERO(&masterfds);
        FD_ZERO(&readfds);
        FD_SET(serverSocket,&masterfds); // add serverSock vào tập masterfds.
        int max_fd = serverSocket;
        
        timeout.tv_sec = 90;
        timeout.tv_usec = 0;
        int n_select;
        do
    {
        memcpy(&readfds,&masterfds,sizeof(masterfds));
        n_select = select(max_fd + 1, &readfds,NULL,NULL,NULL);
        if (n_select < 0)
        {
                perror("SELECT");
                exit(0);
        }
        else
        if (n_select == 0)
        {
            printf("Time out\n");
            sleep(3);
        }
        else
        {
            
              for (i = 0; i <= max_fd;i++)
                {   
                    
                    if (FD_ISSET(i,&readfds))
                    {
                        char client_id[100];
                        printf("i-isset = %d\n",i);
                    	int close_fd = 0;
                        if (i == serverSocket)
                        {                          
                          int newCon = accept(serverSocket,(struct sockaddr*)&clientAddress,&len);
                          printf("New connection - newcon = %d\n",newCon);
                          FD_SET(newCon,&masterfds);
                          if (newCon > max_fd ) max_fd = newCon;
                        }
                        else
                        {
                            myStruct1.a = 5;
                            void* myPtr = &myStruct1;
                            char message[1000], buff[]= "you are connect !";
                              recvBytes = recv(i, message,sizeof(message),0);
                                message[recvBytes] = '\0';
                                if(recvBytes == -1 || recvBytes == 0){
                                    printf("In socket %d\n",i);
                                    perror("RECEIVE");
                                    close_fd = 1;
                                }
                                

                                send(i,(int*)myPtr,sizeof(myStruct1),0);
                                


                              if (close_fd == 1) 
                                {
                                    FD_CLR(i,&masterfds);
                                    close(i);
                                }


                        }
                        
                    }
                }
    }
        
    }
    while(1);
    close(serverSocket);
    return 0;
}

