/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<ctype.h>
#include<signal.h>
#include"rpc.h"

//global variable
int totalCount,lineCount;

//count digits in a string
int 
countDigits(str)
char*str;
{
    int count=0;
    while(*str!='$'){
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

void *
rpc_process_1_svc(argp, rqstp)
	char *argp;
	struct svc_req *rqstp;
{

	static char* result="success";
     
	//creat output file
    FILE*file=fopen("secrets.txt","w");

    //catch control+c to display a summary
    signal(SIGINT, controlCHandler);
    
    char*identifier='$';
    strcat(argp,identifier);
    
    int digitsCount;
    
    //2.count digits,totalDigits and lines
    digitsCount=countDigits(argp);
    lineCount++;
    totalCount+=digitsCount;
  
    //3.write to outputfile
    file=fopen("secrets.txt","a");
    fprintf(file,"%d: ", digitsCount);
    fputs(argp,file);

    fclose(file);
    
	return((void*) &result);
}
