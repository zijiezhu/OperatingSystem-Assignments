#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<ctype.h>
#include<signal.h>

//global variable
int totalCount,lineCount;

void error(char*msg){
    perror(msg);
    exit(1);
}

//count digits in a string
int countDigits(char*str){
    int count=0;
    while(*str!='\0'){
        if(isdigit(*str)){
            count++;
        }
        str++;
    }
    return count;
}

//handle control+c to display summary
void controlCHandler(int sig){
    
    signal(sig,SIG_IGN);//ignore other contol+c during handling
    
    printf("total digits:%d,line count:%d\n",totalCount,lineCount);
    
    signal(SIGINT,controlCHandler);//resume signal handle for control+c
    
    exit(1);//Although remove exit() can continue execute from wherever it left off,this feature is not work on many platforms include mine.So I kept it here.
    
}

int main(int argc,char*argv[]){
    
    if(argc<2){
        error("Error:no port provided");
    }
    
    //catch control+c to display a summary
    signal(SIGINT, controlCHandler);
    
    //variable declaration
    int sockfd,newsockfd,portno,clientlen,buflen,digitsCount;
    struct sockaddr_in saddr,caddr;
    FILE*file;
    char buf[BUFSIZ];
    
    //creat output file
    file=fopen("secrets.out","w");
    
    /*---------Socket communication(connect to client)--------*/
    //1.create socket for server
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        error("Error:creation socket fail");
    }
    
    //2.initiate server address information
    bzero((char*)&saddr,sizeof(saddr));
    portno=atoi(argv[1]);
    saddr.sin_family=AF_INET;
    saddr.sin_addr.s_addr=INADDR_ANY;
    saddr.sin_port=htons(portno);
    
    //3.bind server socket to address information
    if(bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr))<0){
        error("Error:bind fail");
    }
    
    //4.listen
    listen(sockfd,5);
    
    //5.accept to implement handshake protocal
    clientlen=sizeof(caddr);
    newsockfd=accept(sockfd,(struct sockaddr*)&caddr,&clientlen);
    if(newsockfd<0){
        error("Error:accept fail");
    }
    
    /*--------Receive and process information from client--------*/
    while(1){
        
        //1.receive string from client
        if((buflen=recv(newsockfd,buf,BUFSIZ-1,0))<0){
            error("Error:receive fail");
        }
        
        //mark the end of the string for countDigits() to process
        buf[buflen]='\0';
        
        //2.count digits,totalDigits and lines
        digitsCount=countDigits(buf);
        lineCount++;
        totalCount+=digitsCount;
        
        //3.write to outputfile
        file=fopen("secrets.out","a");
        fprintf(file,"%d: ", digitsCount);
        fputs(buf,file);
        
        fclose(file);
    }
    
    close(newsockfd);
}



