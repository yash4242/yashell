#include "../include/libs.h"
extern process child_procs[MAX_CHILDREN];

int cmpfunc(const void * a, const void* b)
{
    process* ax = (process*) a;
    process* bx = (process*) b;
    return strcmp(ax->name, bx->name);
}

exec_status_t jobs(char* arg)
{
    int r =0;
    int s =0;
    if(arg == NULL){r =1 ; s = 1;}
    else
    {
        char* ispresent_r = strstr(arg, "-r");
        if(ispresent_r){r = 1;}
        char* ispresent_s = strstr(arg, "-s");
        if(ispresent_s){s = 1;} 
    }
    
    qsort(child_procs, MAX_CHILDREN, sizeof(process), cmpfunc);
    for(int i = 0; i< MAX_CHILDREN; i++)
    {
        if(child_procs[i].pid == -1)
        continue;

        if(child_procs[i].isrunning && r)
        printf("[%d] Running %s [%d]\n", child_procs[i].number, child_procs[i].name, child_procs[i].pid);

        if(child_procs[i].isrunning == 0 && s)
        printf("[%d] Stopped %s [%d]\n", child_procs[i].number, child_procs[i].name, child_procs[i].pid);
    }
    return 0;
}

exec_status_t sig(char* arg)
{
    char* token;
    char* save;
    char copyofarg[256];
    strcpy(copyofarg, arg);
    token = strtok_r(copyofarg, " \t", &save);
    int number = atoi(token);
    token = strtok_r(NULL, " \t", &save);
    int signal = atoi(token);
 //   printf("job nuber %d, signal sent is %d\n", number, signal);
   int flag = -1;
   int i = 0;
   for(;i<MAX_CHILDREN; i++)
   {
       if(child_procs[i].number == number && child_procs[i].pid != -1)
       {
           flag = 1;
           break;
       }
   }
   if(flag == -1) {printf("there isnt any job with this job number\n"); return 1;}
   if(kill(child_procs[i].pid, signal) < 0)
   {perror("cant send signal, returning\n"); return 1;}
   
   if(signal == SIGCONT)
   {child_procs[i].isrunning = 1;}
   if(signal == SIGSTOP || signal == SIGTSTP)
   {child_procs[i].isrunning = 0;}
   
   
   
    return 0;
}

exec_status_t bg(char* arg)
{
    char* token;
    char* save;
    char copyofarg[256];
    strcpy(copyofarg, arg);
    token = strtok_r(copyofarg, " \t", &save);
    int number = atoi(token);
    int signal = SIGCONT;
 //   printf("job nuber %d, signal sent is %d\n", number, signal);
   int flag = -1;
   int i = 0;
   for(;i<MAX_CHILDREN; i++)
   {
       if(child_procs[i].number == number && child_procs[i].pid != -1)
       {
           flag = 1;
           break;
       }
   }
   if(flag == -1) {printf("there isnt any job with this job number, returning\n"); return 1;}
   if(kill(child_procs[i].pid, signal) < 0)
   {perror("cant send signal, returning\n"); return 1;}
   
    child_procs[i].isrunning = 1; 
   
   
    return 0;
}

exec_status_t my_fg(char * arg)
{
    char* token;
    char* save;
    char copyofarg[256];
    strcpy(copyofarg, arg);
    token = strtok_r(copyofarg, " \t", &save);
    int number = atoi(token);


    int flag = -1;
   int i = 0;
   for(;i<MAX_CHILDREN; i++)
   {
       if(child_procs[i].number == number && child_procs[i].pid != -1)
       {
           flag = 1;
           break;
       }
   }
   if(flag == -1) {printf("there isnt any job with this job number, returning\n"); return 1;}
   if(kill(child_procs[i].pid, SIGCONT) < 0)
   {perror("cant send signal, returning\n"); return 1;}
   child_procs[i].isrunning = 1; 
   int status;
   int p = child_procs[i].pid;
            signal(SIGTTOU, SIG_IGN);
            signal(SIGTTIN, SIG_IGN);
            tcsetpgrp(0, p);
            tcsetpgrp(1, p);
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
