#include "../include/libs.h"
    extern int realSTDIN;
extern int realSTDOUT;

void reset_streams()
{
    dup2(realSTDOUT, STDOUT_FILENO);
    dup2(realSTDIN, STDIN_FILENO);
}

exec_status_t spawn_proc(int in, int out, char* command)
{
    pid_t pid = fork();
    if(pid == 0)
    {
        if(in != 0)
        {dup2(in, 0); close(in);}
        if(out != 1)
        {dup2(out, 1); close(out);}

        char *argv[MAX_NO_ARGS];
        int argc = 0;
        char *token;
        char *save;
        token = strtok_r(command, " \n", &save);
        while (token != NULL)
        {
            argv[argc] = (char *)malloc(PATH_LEN * sizeof(char));
            strcpy(argv[argc], token);
            token = strtok_r(NULL, " ;\n", &save);
            argc++;
        }
        argv[argc] = NULL;

        execvp(argv[0], argv);
    }
    int status;
    waitpid(pid, &status, WUNTRACED);
    return pid;
}

exec_status_t pipe_parser(char * arg)
{
    char * token;
    char* save;
    char pre_parse[COMMAND_LEN];
    char commands[MAX_PIPED][COMMAND_LEN];
    strcpy(pre_parse, arg);
    token = strtok_r(pre_parse, "|", &save);
    int num_commands = 0;

    while(token)
    {
        strcpy(commands[num_commands], token);
        token = strtok_r(NULL, "|", &save);
        num_commands++;
    }
    
    //commands have been split on the basis of pipes now we have array of strings
 
    int fd[2];
    char outputfilename[PATH_LEN];
    int outputfd = 1;
    char inputfilename[PATH_LEN];
    int inputfd = 0;
    char appendfilename[PATH_LEN];
    int appendfd = 1;

    reset_streams();

    for(int i = 0; i<num_commands-1; i++)
    {
        pipe(fd);
        spawn_proc(inputfd, fd[1], commands[i]);
        close(fd[1]);
        inputfd = fd[0];
    }

    //the last piped command or the only command (when no piping used)
    
    char* input = strstr(commands[num_commands-1], "< ");
    int j = 0; 
    if(input != NULL) 
    {
        *input = ' '; 
        while(*input == ' ')
        {input++; if(*input == '\0' || *input == '\n'){write(1, "bad filename\n",13); return 1;}}
        while(*(input + j) != ' ' && *(input+j) != '\0')
        {
            inputfilename[j] = *(input + j);
            *(input + j) = ' ';
            j++;
        }
        inputfilename[j] = '\0';        
    }

    /////////// if(num_commands > 1){inputfd = fd[0];} //piping was done
    if(num_commands <=1)//piping not done
    {
        if(input != NULL) // no piping, but redir found
        {inputfd = open(inputfilename, O_RDONLY);
            if(inputfd < 0){write(1, "cant open file\n", 15); return 1;}
        }
        //no piping no redir case ==> inputfd = 0 case has alr been taken care of
    }
    
    char* output = strstr(commands[num_commands-1], " > ");
    j = 0; 
    if(output != NULL)
    {
        // printf("line %d\n", __LINE__);
        output++;            
        *output = ' ';
        while(*output == ' ')
        {output++; if(*output == '\0' || *output == '\n'){write(1, "bad filename\n", 13); return 1;}}
        
        while(*(output + j) != ' ' && *(output+j) != '\0')
        {
            outputfilename[j] = *(output + j);
            *(output + j) = ' ';
            j++;
        }
        outputfilename[j] = '\0';
        outputfd = open(outputfilename, O_RDWR | O_TRUNC | O_CREAT, 0644);
    }

    char* append  = strstr(commands[num_commands-1], ">> ");
    int k = 0; 
    if(append != NULL)
    {
        // printf("appending found\n");
        *append = ' '; 
        append++;
        *append = ' ';           
        while(*append == ' ')
        {append++; if(*append == '\0' || *append == '\n'){write(1, "bad filename\n", 13); return 1;}}
        
        while(*(append + k) != ' ' && *(append+k) != '\0')
        {
            appendfilename[k] = *(append + k);
            *(append + k) = ' ';
            k++;
        }
        appendfilename[k] = '\0';
        appendfd = open(appendfilename, O_RDWR | O_APPEND | O_CREAT, 0644);
        // printf("%d\n", appendfd);
    }
    //printf("inputfd %d, outputfd %d, appendfd %d\n", inputfd, outputfd, appendfd);
    if(inputfd != 0){dup2(inputfd, 0);}
    if(outputfd != 1){dup2(outputfd, 1);}
    if(outputfd == 1 && appendfd != 1){dup2(appendfd, 1);}
    executek(commands[num_commands-1]);
    if(inputfd)close(inputfd);
    if(outputfd != 1){close(outputfd);}
    if(appendfd != 1){close(appendfd);}
    // reset_streams();
    return 0;
} 