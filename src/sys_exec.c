#include "../include/libs.h"

extern struct process child_procs[MAX_CHILDREN];
extern int process_number;
extern pid_t fg_proc_pid;

void one()
{printf("1\n");}
void two()
{printf("2\n");}

void zombie_handler()
{
    int status;
    pid_t p = waitpid(-1, &status, WNOHANG | WUNTRACED);
    if(p == 0) // runs when a stopped bg -> running bg
    {
        printf("diagnostic: stopped bg -> running bg\n");
        for(int i = 0; i<MAX_CHILDREN; i++)
        {
            if(child_procs[i].pid == fg_proc_pid)
            {child_procs[i].isrunning = 0;}
        }
        fg_proc_pid = 0;
    }
    if(p == -1)//when a fg process dies
    {
        // printf("diagnostic: fg process dies\n");
        for(int i = 0; i<MAX_CHILDREN; i++)
        if(child_procs[i].pid == fg_proc_pid)
            {child_procs[i].pid = -1; strcpy(child_procs[i].name, "");}
        fg_proc_pid = 0;
    }
    if (p > 0) //when bg running-> bg stopped or bg(runnin or stopped doesnr matter) process is killed
    {
        // printf("diagnostic: bg running -> bg stopped or some bg process killed\n");
        int i = 0;
        while (i < MAX_CHILDREN)    
        {
            if (child_procs[i].pid == p)
                break;
            i++;
        }
    
        if (WIFEXITED(status))
        {
            printf("%s pid %d exited normally lmao\n", child_procs[i].name, p);
            child_procs[i].pid = -1;
        }
        else if (WIFSTOPPED(status))
        {
            printf("did u just stop %s with pid %d?\n", child_procs[i].name, p);
        }
         else 
        {
            printf("%s pid %d exited abnormally bruh\n", child_procs[i].name, p);
            child_procs[i].pid = -1;
        }

        
    }   
}

exec_status_t sendToExec(char *command)
{
    char *argv[MAX_NO_ARGS];

    int argc = 0;
    int return_value = 0;
    char *token;
    char *save;

    token = strtok_r(command, " ;\n", &save);

    while (token != NULL)
    {
        argv[argc] = (char *)malloc(PATH_LEN * sizeof(char));
        strcpy(argv[argc], token);
        token = strtok_r(NULL, " ;\n", &save);
        argc++;
    }

    argv[argc] = NULL;

    pid_t p = fork();

    if (p == 0) //child
    {
        setpgid(0, 0);
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        // signal(SIGTSTP, two);
        if (strcmp(argv[argc - 1], "&") == 0)
        {
            argv[argc - 1] = NULL;
        }
        // printf("argv[0] is %s\n", argv[0]);
        
        if (execvp(argv[0], argv) == -1)
        {
            perror("Error while executing");
            exit(1);
        }
    }

    if (p > 0) //parent
    {

        if (strcmp(argv[argc - 1], "&") != 0)
        {
            //having foreground child
            fg_proc_pid = p;
            for (int i = 0; i < MAX_CHILDREN; i++)
            {
                if (child_procs[i].pid == -1)
                {
                    child_procs[i].pid = p;
                    strcpy(child_procs[i].name, argv[0]);
                    child_procs[i].isrunning = 1;
                    child_procs[i].number = process_number;
                    process_number++;
                    break;
                }
            }

            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            tcsetpgrp(0, p);
            tcsetpgrp(1, p);
            int status;
            waitpid(p, &status, WUNTRACED);
            tcsetpgrp(0, getpgid(getpid()));
            tcsetpgrp(1, getpgid(getpid()));
            signal(SIGTTOU, SIG_DFL);
            signal(SIGTTIN, SIG_DFL);
            if(WIFSTOPPED(status))
            {
                kill(p, SIGSTOP);
            }

        }
        else
        {
            //background

            setpgid(p, p);
            for (int i = 0; i < MAX_CHILDREN; i++)
            {
                if (child_procs[i].pid == -1)
                {
                    child_procs[i].pid = p;
                    strcpy(child_procs[i].name, argv[0]);
                    child_procs[i].isrunning = 1;
                    child_procs[i].number = process_number;
                    process_number++;
                    break;
                }
            }
            printf("%d\n", p);
        }

        for (int i = 0; i < argc; i++)
            free(argv[i]);
        tcsetpgrp(STDIN_FILENO, getpid());
        return 0;
    }

    if (p < 0)
    {
        perror("could not open program");
        return 1;
    }
}
