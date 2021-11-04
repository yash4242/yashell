#include "../include/libs.h"

exec_status_t pinfo(char* arg)
{
    tcsetpgrp(STDIN_FILENO, getpid());
    pid_t pid;
    if(arg == NULL)
    {
        pid =  getpid();
        // printf("%d is the pid\n", pid);
    }
    else
    {
        char* token;
        char* save;
        token = strtok_r(arg, " ", &save);
        pid = atoi(token);
        if(pid == 0)
        {
            printf("incorrect usage. takes 1 integer argument or less. returning\n");
            return 1;
        }
        token = strtok_r(NULL, " ", &save);
        if(token != NULL)
        {
            printf("incorrect usage. takes 1 integer argument or less. returning\n");
            return 1;
        }
    }

    char pid_string[7];
    snprintf(pid_string, 7, "%d", pid);
    char procfile[PATH_LEN] = "/proc/";
    strcat(procfile, pid_string);
    char execlink[PATH_LEN];
    strcpy(execlink, procfile);
    strcat(execlink, "/exe");
    strcat(procfile, "/stat");
    FILE* fp = fopen(procfile, "r");
    if(fp == NULL){printf("no such process, returning.\n"); return 1;}

    char procstat[64];
    char pgrpid[10];

    fscanf(fp, "%s", procstat);//1
    printf("pid: %s\n", procstat);
    fscanf(fp, "%s", procstat);//2
    fscanf(fp, "%s", procstat);//3
    printf("running state: %s", procstat); 
    fscanf(fp, "%s", procstat); //4
    fscanf(fp, "%s", pgrpid);//5
    fscanf(fp, "%s", procstat);//6
    fscanf(fp, "%s", procstat);//7
    fscanf(fp, "%s", procstat);//8
    if(strcmp(pgrpid, procstat) == 0){printf("+\n");}
    else{printf("\n");}
    printf("pgrpid: %s\ntgid %s\n", pgrpid, procstat);
    for(int i = 1; i<15; i++)
    {
        fscanf(fp, "%s", procstat);
    }
    printf("virtual memory: %s\n", procstat);
    fclose(fp);

    char execfile[PATH_LEN];
    readlink(execlink, execfile, PATH_LEN);
    printf("Executable path: %s\n", execfile); 
    return 0;

    
}