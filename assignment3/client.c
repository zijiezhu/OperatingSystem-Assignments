#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>

void error(char*msg){
    perror(msg);
    exit(0);
}

int main(int argc,char*argv[]){
    
    if(argc<3){
        error("Error:no hostname or hostport provided");
    }
    
    int sockfd,portno;
    struct sockaddr_in saddr;
    struct hostent*server;
    char*input=malloc(sizeof(char)*BUFSIZ);
    
    portno=atoi(argv[2]);
    
    /*--------Socket communication(connect to server)--------*/
    //1.create socket for client
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        error("Error:creation socket fail");
    }
    
    //2.get server by servername
    server=gethostbyname(argv[1]);
    if(server==NULL){
        error("Error:no such host exists");
    }
    
    //3.get server address
    bzero((char*)&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    bcopy((char*)server->h_addr,(char*)&saddr.sin_addr.s_addr,server->h_length);
    saddr.sin_port=htons(portno);
    
    //4.connect to server
    if(connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr))<0){
        error("Errror:connect fail");
    }
    
    /*--------Process and send information to server--------*/
    while(1){
        
        //1.get user input
        bzero(input,sizeof(input));
        printf("Enter(input'quit'to stop):");
        fgets(input,BUFSIZ,stdin);
        
        //2.check for exit
        if(strcmp(input,"QUIT")==0){
            break;
        }
        //3.send to server
        if(strstr(input,"C00L")){
            if(send(sockfd,input,strlen(input),0)<0){
                error("Error:send fail");
            }
        }
    }
}