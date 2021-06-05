#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int counter = 1;
int buffer_size = 10;


struct command {
	char data[256];
	int id;
	struct command *next;
};

struct command *head = NULL;

void set_buffer_size(int size){
	buffer_size = size;
}

int size();

void insert(char *data, int id){

	struct command *cmd = (struct command*) malloc(sizeof(struct command));

	cmd->id = id;
	strcpy(cmd->data, data);

	cmd->next = head;
	head = cmd;
}


void delete_first(){
	struct command *first = head;
	head = head->next;
	free(first);
}

struct command* get_nth_command(int n){
	if(n>size()){
		//This will create a problem when there are no commands
		//in the buffer!
		return get_nth_command(size());
	}
	int count=1;
	struct command *current=head;
	while(count<n){
		current=current->next;
		count++;
	}
	return current;
}

int size(){
	int counter = 0;
	struct command *current = head;

	while(current != NULL) {
		counter++;
		current = current->next;
	}

	return counter;
}

void print_history(){
	struct command *current = head;
	while(current!=NULL){
		printf("%s",current->data);
		current=current->next;
	}
}


int main(){

	char s[256];
	char *args[80];

	while(1){
		int pid = fork();
		char *directory[80];
		directory[0] = "pwd";
		directory[1] = NULL;
		if(pid == 0){
			execvp(directory[0], directory);
		}
		else{
			wait(NULL);
			fflush(stdout);
			printf(">");
			gets(s);

			if(strcmp(s, "exit") == 0){
				printf("Bye!\n");
				break;
				exit(0);
			}
			if(strcmp(s,"hst")==0){
				print_history();
				exit(0);
			}

			int i = 0;
			char *token = strtok(s, " ");
			while(token != NULL){
				args[i] = token;
				i++;
				token = strtok(NULL, " ");
			}
			args[i] = NULL;
			if(strcmp(args[0],"history")==0 && strcmp(args[1],"-s")==0){
				int new_size = atoi(args[2]);
				set_buffer_size(new_size);
				//We will not store custom commands in the
				//buffer as it would cause a problem later!
				if(size()>buffer_size){
					delete_first();
				}
			}
			if(args[0][0]=='!'){
				char first_arg[256];
				strcpy(first_arg,args[0]);
				memmove(first_arg,first_arg+1,strlen(first_arg));
				//Gets the command number that we need
				//to retrieve
				int cmd_num = atoi(first_arg);
				struct command *retrieved;
				retrieved=get_nth_command(cmd_num);
				//Get command string from the structure
				char str[256];
				char *exec_args[256];
				strcpy(str,retrieved->data);
				char *word = strtok(str," ");
				int a=0;
				while(word!=NULL){
					exec_args[a]=word;
					a++;
					word=strtok(NULL," ");
				}
				exec_args[a]=NULL;
				int new_process = fork();
				if(new_process==0){
					execvp(exec_args[0],exec_args);
				}
				else{
					wait(NULL);
				}
			}
			pid = fork();

			if(pid == 0){
				insert(s,counter++);
				execvp(args[0], args);
			} else {
				wait(NULL);
			}
		}
	}

	return 0;
}
