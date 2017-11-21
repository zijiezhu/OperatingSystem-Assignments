#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>

#define SHMSIZE 1024
#define QUIT "quit"
int main(void){
    
    /*--------variable declarition--------*/
    key_t key;
    int shmid;
    char *shm;
    char *input=malloc(sizeof(char)*BUFSIZ);

    /*--------set up shared memory--------*/
    key=getuid();
    
    //1.creat shared momery space using shmget()
    shmid=shmget(key, SHMSIZE, IPC_CREAT|0666);
    
    //error detected for shared memory creation
    if(shmid<0){
        perror("shared memory segment creation failed!");
        exit(EXIT_FAILURE);
    }
    
    //2.attach data space to the shared memory segment
    shm=shmat(shmid, NULL, 0);
    //error detected for data space attachment
    if(shm==(char*)-1){
        perror("data space attachment failed!");
        exit(EXIT_FAILURE);
    }
    
    /*--------recieve user input--------*/
    while(1){
        //1.get user input
        printf("Enter(quit to stop):");
        fgets(input,BUFSIZ,stdin);
        
        //2.check for exit
        if(strcmp(input,QUIT)==0){
            break;
        }
        
        //3.check for secret code and copy to the shared memory
        if(strstr(input, "C00L")){
            
        //3.1 add the termination mark-'@'to the end of the input
        char*s=shm;
        s+=strlen(input);
        *s='@';
    
        //3.2 copy the input to the shared memory
        memcpy(shm, input, strlen(input));

        //3.3 block to wait the signal(the head marked '&')from processor.c
        while(*shm!='&'){
            sleep(1);
        }
        
       }
    }
    
    //--------close shared memory--------/
    //detach data space from the shared memory
    if(shmdt(shm)==-1){
        perror("data space dettachment fialed!");
        exit(EXIT_FAILURE);
    }
    
    //close ipc communication
    if(shmctl(shmid, IPC_RMID, 0)==-1){
        perror("IPC close failed!");
        exit(EXIT_FAILURE);
    }
}

