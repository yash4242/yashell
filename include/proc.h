#ifndef _PROC_
#define _PROC_

struct process
{
    pid_t pid;
    char name[32];
    int isrunning;
    int number;
};

typedef struct process process;

#endif
