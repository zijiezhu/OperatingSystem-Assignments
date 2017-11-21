#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_NUMBER_OF_PARAMS 10

//Format commend(user input) into array of parameters
void parseCmd(char* cmd, char** params)
{       
    for(int i = 0; i < MAX_NUMBER_OF_PARAMS; i++) {
        params[i] = strsep(&cmd, " ");
        if(params[i] == NULL) break;
    }
}

//Execute splited commend
int executeCmd(char** params)
{
    // Fork current process
    pid_t pid = fork();
    char*error;
    int childStatus;

    switch(pid){
        /*Fork error*/
        case -1:
        error = strerror(errno);
        printf("fork: %s\n", error);
        return 1;
        
        /*Child process*/
        case 0:
        //Execute command by replace parent process with child process
        execvp(params[0], params);  

        //Execvp error 
        error = strerror(errno);
        printf("shell: %s: %s\n", params[0], error);
        return 0;

        /*Parent process*/
        default:
        //wait child process to finish
        waitpid(pid, &childStatus, 0);
        return 1;
    }
}

int main()
{
    char cmd[MAX_COMMAND_LENGTH + 1];
    char* params[MAX_NUMBER_OF_PARAMS + 1];

    int cmdCount = 0;

    while(1) {
        // Print command prompt
        char* username = getenv("USER");
        printf("%s@shell > ", username);

        /*1.Read command from user input*/
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        //Remove trailing newline character, if any
        if(cmd[strlen(cmd)-1] == '\n') {
            cmd[strlen(cmd)-1] = '\0';
        }

        /*2.Format cmd into array of parameters*/
        parseCmd(cmd, params);

        // Check Exit?
        if(strcmp(params[0], "exit") == 0) break;

        /*3 Execute command*/
        if(executeCmd(params) == 0) break;
    }

    return 0;
}