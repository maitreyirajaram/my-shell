#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


#define MAXLENGTH 1024 


void parse(char* command, char* token){
	//printf("%s", command); 
                token = strtok(command, " ");
                char* cmd = (char *) malloc (sizeof(char)*20);
                //clean command
                if ((token[0] == '"') &&(token[strlen(token)-1]=='"')){
                        cmd = strtok(token, "\"");
                }
                else if ((token[0] == '\'') &&(token[strlen(token)-1]=='\'')){
                        cmd = strtok(token, "\'");
                }
                else{

                        strcpy(cmd, token);
                }


                char** arg= (char**) malloc(20*sizeof(char*));
                int arg_counter = 0;
                while (token != NULL) {
                        arg[arg_counter]=token;
                        token = strtok(NULL, " ");
                        arg_counter++;
                }
                arg[arg_counter+1] = NULL;


                execute(cmd,arg);

}

void  execute(char *cmd,char**arg)
{
	pid_t  pid;
	int    status;

	if ((pid = fork()) < 0) {     
		printf("Fork failed\n");
		exit(1);
	}
	else if (pid == 0) {          // child process:         
		if (strcmp("cd", cmd)==0){
			if (chdir(arg[1])==-1){
				printf("Cannot change directory");
			}
		}
		else if (strcmp(cmd, "exit")==0){
			kill(pid, SIGKILL);
		}
		else if (execvp(cmd, arg) < 0) {     // execute 
			printf("Command not found\n");
		}
	}
	else {                                  
		while (wait(&status) != pid)       
			;
	}
}


int main(int argc, char **argv) {


	while(1)
	{			

		char command[MAXLENGTH];
		char *token = command;//largest token can be max length

		char* buf; 
		long size = pathconf(".", _PC_PATH_MAX);
		buf = (char *) malloc ((size_t)size);
		char* pwd = getcwd(buf, size); 
		printf("%s%s", pwd, "$ ");          //prompt for shell (pwd) 
		fgets(command, MAXLENGTH, stdin);   
		//clean new line from fgets() 
		size_t length = strlen(command);
		if (command[length - 1] == '\n'){
			command[length - 1] = '\0';     
		}

		//check if exit
		if (strcmp(command, "exit")==0){
			//kill process in execute()
		}
		

		//parse by ;
		char ** command_list = (char**) malloc(500*sizeof(char*)); 
		int command_counter = 0; 
		char* sub_token = strtok(command, ";");
		//printf("%s", sub_token); 
		while (sub_token != NULL) {
			//printf("%s", sub_token);
			command_list[command_counter]= sub_token;
			 command_counter++;
			sub_token = strtok(NULL, ";"); 
		}
		//iterate through all commands
		for (int i = 0; i < command_counter; i++) {
			parse(command_list[i], token); 
		}






	}


	printf("\n");


	return 0;

	}
