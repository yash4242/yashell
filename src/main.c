#include "../include/libs.h"

//global vars
exec_status_t status = 0; 
/* the value which is reuturned after completion of a process. the one which $? gives. 0 = all fine, 1 = something wrong */

char* inputBuffer= NULL;
size_t inputBufferLen = 0;

char* username;
char hostname[HOSTNAME_LEN];
char homePath[PATH_LEN];
char currentPath[PATH_LEN];
char oldDir[PATH_LEN];
struct process child_procs[MAX_CHILDREN];
int realSTDIN;
int realSTDOUT;
// char commands[MAX_PIPED][COMMAND_LEN];

int process_number = 1;
pid_t fg_proc_pid;


int main()
{
    //setting up
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGCHLD, zombie_handler);
    realSTDIN = dup(STDIN_FILENO);
    realSTDOUT = dup(STDOUT_FILENO);
    clear();
    promptSetup();
    for(int i = 0; i< MAX_CHILDREN; i++)
    {
        child_procs[i].pid = -1;
        strcpy(child_procs[i].name, "");
    }


    while(1)
    {
        showPrompt(status); 
        status = 0;
        if(getline(&inputBuffer, &inputBufferLen, stdin) == -1){exit(0);}
        //now parsing into individual commands
        char* token;
        char* save;
        token = strtok_r(inputBuffer, ";\n", &save); //should i add & in the delims?
        while(token != NULL)
        {
            status = pipe_parser(token);    
            reset_streams();
            token = strtok_r(NULL, ";\n", &save); //should i add & in the delims?
        }
    }
    //end 
    //over
    //phinis
    //khallas  
}

