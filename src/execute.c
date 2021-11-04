#include "../include/libs.h"

exec_status_t executek(char* arg)
{
    if(arg == NULL)
    {printf("empty command supplied, returning\n"); return 0;}
    char* token;
    char* save;
    char command[256];
    strcpy(command, arg);
    
    token = strtok_r(command, " \n", &save);

    if(!strcmp("cd", token))
    {
        token = strtok_r(NULL, " ", &save);
        return builtin_cd(token);
    }
    if(!strcmp("echo", token))
    {
        token = strtok_r(NULL,"\n",  &save);
        return builtin_echo(token);
    }
    if(!strcmp("pwd", token))
    {
        token = strtok_r(NULL, "\n", &save);
        return builtin_pwd(token);
    }
    if(! strcmp("ls", token))
    {
        token = strtok_r(NULL,"\n",  &save);
        return builtin_ls(token);
    }
    if(!strcmp("clear", token))
    {
        clear();
        return 0;
    }
    if(!strcmp("pinfo", token))
    {
        token = strtok_r(NULL,"\n",  &save);
        return pinfo(token);
    }
    if(!strcmp("repeat", token))
    {
        token = strtok_r(NULL, " ", &save);
        int times = atoi(token);
        token = strtok_r(NULL, "\n", &save);
        char rep[COMMAND_LEN];
        strcpy(rep, token);
        // printf("rep is %d char long\n", strlen(rep));
        for(int i = 1; i<times; i++)
        {   
            executek(rep);
        }
        return executek(rep);
    }
    if(!strcmp("jobs", token))
    {
        token = strtok_r(NULL,"\n",  &save);
        return jobs(token);
    }
    if(!strcmp("sig", token))
    {
        token = strtok_r(NULL,"\n",  &save);
        return sig(token);
    }

    if(!strcmp("bg", token))
    {
        token = strtok_r(NULL,"\n",  &save);
        return bg(token);
    }
    if(!strcmp("fg", token))
    {
        token = strtok_r(NULL,"\n",  &save);
        return my_fg(token);
    }


     if(!strcmp("quit", token))
    {
        exit(0);
    }

    else // these commands will go to execvp
    {
        char fullcommand[COMMAND_LEN];
        strcpy(fullcommand, token);
        
        token = strtok_r(NULL,"\n",  &save);
        if(token != NULL)
        {
            strcat(fullcommand, " ");
            strcat(fullcommand, token);
        }
        return sendToExec(fullcommand);
    }

    return 1;
    
}