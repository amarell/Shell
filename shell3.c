#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>

int counter = 1;
HISTORY_STATE *myhist;
HIST_ENTRY **mylist;
void number_of_hist_commands();
void print_history();
void add_history_with_id(char *cmd);
char* get_command_by_id(int id);
char* get_last_nth_command(int n);
void print_last_n_commands(int n);

int main(){
	using_history();
	stifle_history(10);


	char s[256];
	char *args[80];
	char current_dir[256];

	while(1){
		printf("%s>$ ", getcwd(current_dir, 256));
		fflush(stdout);
		gets(s);


		if(strcmp(s, "exit") == 0){
			printf("Bye!\n");
			break;
			return 0;
			exit(0);
		}

		int i = 0;
		// we are using a temp variable to parse input
		// so that we don't alter the variable s
		char temp[256];
		strcpy(temp, s);
		char *token = strtok(temp, " ");
		while(token != NULL){
			args[i] = token;
			//printf("%d. %s", i, args[i]);
			i++;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;
		if(strcmp(args[0], "history")==0){
			if(args[1] != NULL){
				if(strcmp(args[1], "-s")==0){
					int new_size = atoi(args[2]);
					stifle_history(new_size);
					continue;
				} else {
					int n = atoi(args[1]);
					if(n == 0){
						printf("Invalid command :(\n");
					}
					add_history_with_id(s);
					print_last_n_commands(n);
					continue;
				}
			} else{
				add_history_with_id(s);
				print_history();
				continue;
			}
		}
/*
		if(strcmp(args[0],"history")==0 && strcmp(args[1], "-s")==0){
			int new_size = atoi(args[2]);
			stifle_history(new_size);
			continue;
		}
		else{
			printf("\n\n%s \n\n", args[0]);
			return 0;
		}*/
		if(strcmp(args[0], "print_hist")==0){
			add_history_with_id(s);
			print_history();
			continue;
		}
		if(strcmp(args[0],"print_last")==0){
			int n=atoi(args[1]);
			add_history_with_id(s);
			print_last_n_commands(n);
			continue;
		}
		if(strcmp(args[0], "cd") == 0){
			char path[256];
			getcwd(path, 256);
			strcat(path, "/");
			strcat(path, args[1]);
			if(chdir(path) != 0){
				perror("chdir() failed");
			}
			continue;
		}
		if(args[0][0]=='!'){
			char first_arg[256];
			strcpy(first_arg,args[0]);
			memmove(first_arg,first_arg+1,strlen(first_arg));
			//Gets the command number that we need
			//to retrieve
			int cmd_num = atoi(first_arg);
			//This is where our full command will be saved:
			char* str21;
			//These are new custom args that will be parsed
			//from the str21
			char *c_args[256];
			if(cmd_num>0){
				str21 = get_command_by_id(cmd_num);
			}
			else if(cmd_num<0){
				str21 = get_last_nth_command(cmd_num);
			}
			//c is the counter used to iterate through str21
			int c=0;
			char *wrd = strtok(str21," ");
			while(wrd!=NULL){
				c_args[c++] = wrd;
				wrd=strtok(NULL," ");
			}
			c_args[c]=NULL;
			if(str21!=NULL){
				add_history_with_id(str21);
			}
			//c_pid is the value returned by fork
			int c_pid = fork();
			if(c_pid==0){
				execvp(c_args[0], c_args);
			}
			else{
				wait(NULL);
				continue;
			}
		}

		add_history_with_id(s);

		int pid = fork();

		if(pid == 0){
			if(execvp(args[0], args)!=0){
				perror("Invalid command!\n");
				exit(0);
			}
		} else {
			wait(NULL);
		}
	}
	return 0;
}


void number_of_hist_commands(){
	myhist = history_get_history_state();
	printf("There are %d items in history",myhist->length);
}

void print_history(){
	myhist = history_get_history_state();
	mylist = history_list();

	printf("History of commands in this session: \n");
	for(int i = 0; i < myhist->length; i++){
		printf("[%d] %s\n", *((int*)mylist[i]->data), mylist[i]->line);
	}
}

void add_history_with_id(char *cmd){
	add_history(cmd);
	myhist = history_get_history_state();
	mylist = history_list();
	mylist[myhist->length-1]->data = malloc(sizeof(int));
	*((int*)mylist[myhist->length-1]->data) = counter++;

}

char* get_command_by_id(int id){
	myhist = history_get_history_state();
	mylist = history_list();

	for(int i = 0; i < myhist->length; i++){
		if(*((int*)mylist[i]->data) == id){
			return mylist[i]->line;
		}
	}
	return NULL;
}

char* get_last_nth_command(int n){
	myhist = history_get_history_state();
	mylist = history_list();
	if(myhist->length+n<0){
		printf("Invalid command, exceded limit!\n");
		return NULL;
	}

	return mylist[myhist->length+n]->line;
}

void print_last_n_commands(int n){
	myhist = history_get_history_state();
	mylist = history_list();
	if(myhist->length<n){
		print_history();
	}
	else{
		for(int i=myhist->length-n;i<myhist->length;i++){
			printf("[%d] %s\n",*((int*)mylist[i]->data), mylist[i]->line);
		}
	}
}

/*void print_nth_command(int n){
	mylist = history_list();
	printf("%s\n", mylist[n]->line);
}*/

