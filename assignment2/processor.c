#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHMSIZE 1024

int main(void){
    /*--------variables declarition--------*/
    key_t key;
    int shmid;
    int count;
    char*shm,*s;
    char write[BUFSIZ];
    FILE*file;
    
    /*--------creat output file--------*/
    file=fopen("secrets.out","w");
    
    /*--------set up shared memory--------*/
    //use the same key of receiver.c so that processor.c can share memory space with receiver.c
    key=getuid();
    
    //1.creat shared momery space using shmget()
    shmid=shmget(key, SHMSIZE, 0666);
    
    //error detected when created shared memory
    if(shmid<0){
        perror("shared memory segment creation failed!");
        exit(EXIT_FAILURE);
    }
    
    //2.attach data space to the shared memory segment
    shm=shmat(shmid, NULL, 0);

    //error detected when attach data space
    if(shm==(char*)-1){
        perror("data space attachment failed!");
        exit(EXIT_FAILURE);
    }
    
    /*--------receive and write to output file--------*/
    while(1){
        //when the head of shared memory is not '&',can read the shared memory and write to output file
        if(*shm!=(int)NULL&&*shm!='&'){
            //1.initialize the variable for write
            count=0;
            memset(write,0,sizeof(write));
            
            //2.read the shared memory and count digits
            s=shm;
            while(*s!='@'){//'@'marks the end of input string
                write[count]=*s;
                s++;
                count++;
            }
            
            //3.write to outfile
            file=fopen("secrets.out","a");
            fprintf(file,"%d: ",count-1);
            fputs(write,file);
            fclose(file);
            
            //4.mark the head with '&' to signal receiver.c to continue next turn
            *shm='&';

      }
    }
 }



