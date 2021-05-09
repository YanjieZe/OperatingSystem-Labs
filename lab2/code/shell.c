#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>    
#include <fcntl.h>

#define MAX_LINE    80
#define READ_END    0
#define WRITE_END   1


int main()
{
    char *args[MAX_LINE/2 + 1];
    for(int k=0;k<MAX_LINE/2+1;k++)args[k]=NULL;

    char input_sentence[81];
    char history_sentence[81];
    char stop_word[]="exit";
    

    int should_run = 1;
    int history_exist = 0;
    int fd[2];

    while (should_run)
    {
       printf("shell>") ;

        //input
       fgets(input_sentence, 81, stdin);
       if(input_sentence[0]=='\n')
               continue;

        if(input_sentence[0]=='!'&&input_sentence[1]=='!'&&input_sentence[2]=='\n') {
            if(history_exist==0){
                printf("No commands in history!\n");
                continue;
                }
            else{
                int k=0;
                for(int w=0;history_sentence[w]!='\n';++w)
                {
                    printf("%c", history_sentence[w]);
                }
                printf("\n");
                for(;history_sentence[k]!='\n';k++)
                    input_sentence[k]=history_sentence[k];
                input_sentence[k]='\n';
                input_sentence[k+1]='\0';
            }
        }

        //parsing
        int arg_num=0;
        for(int i=0;input_sentence[i]!='\n';)
        {
            args[arg_num] = (char*)malloc(sizeof(char)*20);
            int j=0;
            for(; input_sentence[i]!='\n'&&input_sentence[i]!=' ';++j,++i){
                args[arg_num][j]=input_sentence[i];
            }
            args[arg_num][j]='\0';

            arg_num++;
            if(input_sentence[i]=='\n')
                break;
            if(input_sentence[i]==' ')
                i ++;
        }
        arg_num--;


        if(strcmp(args[0], stop_word)==0) break;
        
        if(strcmp(args[0], "!!")==0) 
        {
            printf("No commands in history!!\n");
            continue;
        }


        // divide pipe args
        int pipe_exist=0;
        int pipe_position;
        for(int n=0;n<=arg_num;++n)
        {
            if(strcmp(args[n],"|")==0)
                {
                    pipe_exist = 1;
                    pipe_position = n;
                }

        } 
        char *pipe_args[MAX_LINE/2+1];      
        if(pipe_exist==1)
        {
            
            // create pipe args
            for(int w=0;w<MAX_LINE/2+1;w++)pipe_args[w]=NULL;
            int total_num = arg_num;
            
            for(int w=pipe_position+1,i=0;w<=total_num;++w,++i)
            {
                int q=0;
                pipe_args[i]=(char*)malloc(sizeof(char)*20);
                for(;args[w][q]!='\0';++q)
                {
                    pipe_args[i][q]=args[w][q];
                }
                pipe_args[i][q]='\0';

                free(args[w-1]);
                args[w-1]=NULL;
                arg_num--;
            }
            
            free(args[total_num]);
            args[total_num]=NULL;
            arg_num--;

        }
        
        //process
        pid_t pid;
        pid = fork();
        if (pid < 0) { 
                fprintf(stderr, "Fork Failed");
                return 1;
                }
        else if (pid == 0) {

                if(strcmp(args[arg_num],"&")==0)
                        args[arg_num]=NULL, arg_num--;

                if(arg_num>=1)
                {
                    if(strcmp(args[arg_num-1],">")==0)
                    {
                        //situation >
                        int origin_point = dup(STDOUT_FILENO);
                        int file_write = open(args[arg_num], O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
                        if (file_write==-1)
                            fprintf(stderr, "Create file Failed");
                        dup2(file_write, STDOUT_FILENO);
                        args[arg_num-1]=NULL;
                        args[arg_num]=NULL;
                        execvp(args[0], args);
                        close(file_write);
                        dup2(origin_point, STDOUT_FILENO);
                        exit(0);
                    }
                    else if(strcmp(args[arg_num-1],"<")==0)
                    {
                        //situation <
                        int origin_point = dup(STDIN_FILENO);
                        int file_read = open(args[arg_num], O_RDONLY);
                        dup2(file_read, STDIN_FILENO);
                        args[arg_num-1]=NULL;
                        args[arg_num]=NULL;
                        execvp(args[0], args);
                        close(file_read);
                        dup2(origin_point, STDIN_FILENO);
                        exit(0);
                    }
                    else if(pipe_exist==1)
                    {
                        
                        
                        //situation using pipe
                        
                        pid_t pid2;
                        if (pipe(fd) == -1) 
                        {
                            fprintf(stderr,"Pipe failed");
                            return 1;
                        }
                        // new  child
                        pid2 = fork();
                        if (pid2 < 0) 
                        { 
                            fprintf(stderr, "Fork Failed");
                            return 1;
                        }
                        else if (pid2 == 0) { 
                        //child do former
                        close(fd[READ_END]);
                        dup2(fd[WRITE_END], STDOUT_FILENO);
                        execvp(args[0], args);
                        close(fd[WRITE_END]);
                        exit(0);
                        }
                        else { 
                            //parent do latter
                        close(fd[WRITE_END]);
                        dup2(fd[READ_END], STDIN_FILENO);
                        execvp(pipe_args[0], pipe_args);
                        close(fd[READ_END]);
                        wait(NULL);
                        }
                    }
                    else
                    {  
                        execvp(args[0], args);
                    }
                }
                else
                    execvp(args[0], args);
                }
        else { //parent process
                if(strcmp(args[arg_num],"&")!=0){
                    wait(NULL);
                }
            }

        //clear
        for(int i = 0;i < MAX_LINE/2 + 1; ++i) free(args[i]);	
        
        //save history
        int m = 0;
        for(; input_sentence[m]!='\n'; ++m)history_sentence[m]=input_sentence[m];
        history_sentence[m]='\n';
        history_sentence[m+1]='\0';
        history_exist = 1; 

    }
    
}