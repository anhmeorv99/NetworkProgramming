#include <stdio.h>
#include "linkedList.h"
#include<string.h>
#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //for exit(0);
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h>	//hostent
#include<arpa/inet.h>
#include <netinet/in.h>

int menu(){
    int select;
    printf("USER MANAGEMENT PROGRAM\n---------------------------------------\n");
    printf("1. Register \n");
    printf("2. Activate \n");
    printf("3. Sign in \n");
    printf("4. Search \n");
    printf("5. Change password \n");
    printf("6. Sign out \n");
    printf("7. Homepage with domain name\n");
    printf("8. Homepage with IP address\n");
    printf("Your choice (1-6, other to quit) : ");
    
    scanf("%d",&select);
    getchar();
    return select;
}

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
        printf("hostname : %s \n",ip);
    }
    else {
        printf("hostname :%s\n", hbuf);
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




void Register(node users_list){
    ElementType user;
    char re_password[50];
    int select_case1;
     do {
                
                printf("1. Enter username \n");
                printf("2. Exit\n");
                printf("Your choice : ");
                scanf("%d",&select_case1);
                if (select_case1==1){
                    getchar();
                    printf("\nUserName : ");
                        gets(user.username);
                        if (Search(users_list,user.username) == -1){
                       // fflush(stdin);
                        printf("Enter password : ");
                        gets(user.password);
                        printf("Enter password again : ");
                        gets(re_password);
                        if (strcmp(user.password,re_password)==0){
                            strcpy(user.status,"idle");
                            users_list = AddTail(users_list,user);
                            
                            printf("Register successfully !\n");
                        } else
                        {
                            printf("incorrect when try password, pls try again \n");
                        }
                        
                        } else    printf("existed user name. Pls try again ! \n"); 
                        
                }
            } while (select_case1!=2);
}

void Active(node users_list){

    int count = 0;
    const char activeCode[10] = "LTM121216";
    char temp[50];
    ElementType check_user,user;
     do
                {
                    printf("Enter username : ");
                    gets(user.username);
                    printf("Enter password : ");
                    gets(user.password);
                    printf("Enter your active code : ");
                    gets(temp);

                    if (strcmp(activeCode,temp)==0)
                    {
                        if (Search(users_list,user.username)!= -1)
                        {
                            check_user =  Get(users_list,Search(users_list,user.username));
                            if (strcmp(check_user.password,user.password)==0){
                                strcpy(check_user.status,"active");
                                Update(users_list,check_user,Search(users_list,user.username));
                                printf("Active user successfully ! \n");
                                return;
                            }
                            else
                            {
                                printf("incorect password, pls try again ! \n");
                                count++;
                            }
                            
                        }

                        else
                        {
                            printf("invalid username ! \n");
                        }
                       
                        
                    }
                    else
                    {
                        count++;
                        printf("incorrect active code ! pls try again \n");
                    }
                    
                } while (count!=3);

        if (count==3 && Search(users_list,user.username)!=-1 ){
            check_user =  Get(users_list,Search(users_list,user.username));
            strcpy(check_user.status,"blocked");
            Update(users_list,check_user,Search(users_list,check_user.username));
            printf("active failed, account blocked !\n");

        } else
        {
            printf("too much fail request !\n");
        }
        

}

void Signin(node users_list){

    int count = 0;
    char temp[50];
    ElementType check_user,user;
              do
                {
                    printf("Enter username : ");
                    gets(user.username);
                    printf("Enter password : ");
                    gets(user.password);
                    if (Search(users_list,user.username)!= -1)
                        {
                            check_user =  Get(users_list,Search(users_list,user.username));
                            if (strcmp(check_user.password,user.password)==0 && strcmp(check_user.status,"active")==0 && check_user.login_status!=1){
                                check_user.login_status = 1;
                                Update(users_list,check_user,Search(users_list,check_user.username));
                                printf("Login successfully ! \n");
                                
                                return;
                            }
                            else if (strcmp(check_user.status,"active")!=0){
                                    printf("accounts blocked or not active ! \n");
                                    return;
                            }else if (check_user.login_status==1) {
                                printf("This account is already logged in \n");
                                return;
                            }
                            else
                            {
                                printf("incorect password, pls try again ! \n");
                                count++;
                            }
                            
                        }

                        else
                        {
                            printf("invalid username ! \n");
                            count++;
                        }
                       
                        
                    
                    
                } while (count!=3);

        if (count==3 && Search(users_list,user.username)!=-1 ){
            check_user =  Get(users_list,Search(users_list,user.username));
            strcpy(check_user.status,"blocked");
            Update(users_list,check_user,Search(users_list,check_user.username));
            printf("Login failed, account blocked !\n");

        } else
        {
            printf("too much fail request !\n");
        }
        
}


void Search_account(node users_list){
    
    ElementType check_user,user;
   
                    printf("Enter username : ");
                    gets(user.username);

                  
                        if (Search(users_list,user.username)!= -1)
                        {
                            check_user =  Get(users_list,Search(users_list,user.username));
                            printf("Status of %s : %s\n",check_user.username,check_user.status);
                            
                        }
                        else
                        {
                            printf("User name not found !\n");
                        }    
}

void Change_password(node users_list){

    ElementType check_user,user;
    char temp[50];
            
                    printf("Enter username : ");
                    gets(user.username);
                   
                    if (Search(users_list,user.username)!=-1){

                        check_user =  Get(users_list,Search(users_list,user.username));
                        if (check_user.login_status == 1){
                            printf("Enter old password : ");
                            gets(user.password);
                            if (strcmp(check_user.password,user.password)==0){
                                printf("Enter new password : ");
                                gets(user.password);
                                printf("Enter new password again : ");
                                gets(temp);
                                if (strcmp(temp,user.password)==0){
                                    strcpy(check_user.password,temp);
                                    Update(users_list,check_user,Search(users_list,check_user.username));
                                    printf("Change password successfully ! \n");
                                } else   printf("incorrect re-enter your password\n");  
                        } else printf("incorrect old password \n");
                        }else printf("Account is not login\n");
                   
                   
                    }  
                    
                    else printf("cant find username \n");
                    
                    
                        
                    
             


}

void Signout(node users_list){
    
    ElementType check_user,user;
                    printf("Enter username : ");
                    gets(user.username);
                    if (Search(users_list,user.username)!= -1)
                        {
                            check_user =  Get(users_list,Search(users_list,user.username));
                            if (check_user.login_status == 1){
                                
                                printf("Logout successfully ! \n");
                                check_user.login_status = 0;
                                Update(users_list,check_user,Search(users_list,user.username));
                                return;
                            }
                            else
                            {
                                printf("Accounts is not sign in \n");
                              
                            }
                                                 
                        }
                                
       
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Expression is not a valid instance of the grammar, pls check argument ex : ./filename text.txt \n");
        return 0;
    } 
    
    FILE *f;
    char str[100],ip[100];

    ElementType user, check_user;

   
   node users_list = NULL;
    if((f = fopen(argv[1],"r+")) == NULL) {
            printf("No such file or directory !!!");
            return 0;
        }

   
        while ( fscanf(f,"%s %s %s %s",user.username,user.password,user.status,user.homepage)!=EOF)
        {
                            users_list = AddTail(users_list,user);

        }
    printList(users_list);
   
    int select, select_case1;
    do{
        select = menu();
        if (select<1 || select>8) printf("Quit !!!");
        
        switch (select)
        {
            case 1:
                Register(users_list);
            break;
            case 2:
            
                Active(users_list);               
                
            break;
            case 3:
                Signin(users_list);
            break;
            case 4:
                Search_account(users_list);
            break;
            case 5:
                Change_password(users_list);
            break;
            case 6:
                Signout(users_list);
            break;
            case 7:
                    printf("Enter username : ");
                    gets(user.username);
                    if (Search(users_list,user.username)!= -1)
                        {
                            check_user =  Get(users_list,Search(users_list,user.username));
                            if (check_user.login_status == 1){
                                
                               convert_ip_to_hostname(check_user.homepage);
                                
                            }
                            else
                            {
                                printf("Accounts is not sign in \n");
                              
                            }
                                                 
                        }
                
            break;
            case 8:
		       
                printf("Enter username : ");
                    gets(user.username);
                    if (Search(users_list,user.username)!= -1)
                        {
                            check_user =  Get(users_list,Search(users_list,user.username));
                            if (check_user.login_status == 1){
                                
                               convert_hostname_to_ip(check_user.homepage,ip);
                               printf("IP : %s", ip);
                            }
                            else
                            {
                                printf("Accounts is not sign in \n");
                              
                            }
                                                 
                        }
    

            break;
        }



    } while (select >= 1 && select <= 8);
    
    rewind(f);
    node p = users_list;

    while (p != NULL)
    {
        fprintf(f,"%s %s %s %s\n",p->eltype.username,p->eltype.password,p->eltype.status,p->eltype.homepage);
        p = p->next;
    }
    

    fclose(f);
    
    return 0;
    
}