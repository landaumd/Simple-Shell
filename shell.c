// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

// Copyright (c) 2017 Megan Landau <landaumd@g.cofc.edu>

// --------------------------------------------
// Currently only two builtin commands for this
// assignment exist, however in future, more could 
// be added to the builtin command array.
// --------------------------------------------
const char* valid_builtin_commands[] = {"cd", "exit", NULL};


// --------------------------------------------
// My helper functions:
// -------------------------------------------- 

// Count the number of arguments with a passed in delimeter.
static int myStrTok(char* cmd, char d){
	int counter = 0;
	//need to edit so that multiple delimeters in a row don't count
	while(*cmd != '\0'){
		if(*cmd == d)
			counter++;
		cmd++;
	}
	if(d == ' ' || d == ':')//for commands with spaces or : in between = one extra!
		counter++;
	return counter;	
}

// Count the number of chars in a string.
static int myCountChar(char* str){
	int counter = 0;
	while(*str != '\0'){
		counter++;
		str++;
	}
	return counter;
}

// Compare two strings.
static int myOtherStrCmp(const char* str1, const char* str2){
	
	//printf("1  str1 = %s	str2 = %s \n", str1, str2);

	int i = 0;
	while (str1[i] == str2[i] && str1[i] != '\0')
  		i++;
	if (str1[i] > str2[i]){
      		//printf("str1 > str2\n");
		return 1;
   	}else if (str1[i] < str2[i]){
      		//printf("str1 < str2\n");
		return -1;
   	}else{
      		//printf("str1 == str2\n");
		return 0;
	}

}

// --------------------------------------------
// Implementation of interface functions:
// --------------------------------------------

void parse(char* line, command_t* p_cmd){
	//find argc - num of args
	p_cmd->argc = myStrTok(line, ' ');
	//printf("p_cmd->argc = %d\n", p_cmd->argc);

	//malloc enough space for num of arg strings
	p_cmd->argv = (char**)malloc((p_cmd->argc)*sizeof(char*));

	//make array out of all arguments
	int savej = 0;
	for(int i = 0; i < (p_cmd->argc+1); i++){ //loops through argv array of malloced space
		int counter = 0; //count num letters in each argument so we can malloc properly
		for(int j = savej; j < myCountChar(line); j++){ //loop through whole str,
			if(line[j] == ' ' || line[j+1] == '\0'){
				p_cmd->argv[i] = (char*)malloc(counter+1);//add an extra for the '\0' char
				for(int k = 0; k < counter; k++){ //save all chars into argv[i]				
					p_cmd->argv[i][k] = line[savej];
					savej++;
				}p_cmd->argv[i][counter+1] = '\0';
				savej = j+1; //save place in string +1 to skip the space
				break;
			}else
				counter++;
		}
	}

	//to print argv - array of args
	//for(int i = 0; i < p_cmd->argc; i++)
	//	printf("argv[%d] = '%s'\n", i, p_cmd->argv[i]);

	//argv[0] is the name
	p_cmd->name = p_cmd->argv[0];
	
	//printf("p_cmd->name = %s\n", p_cmd->name); 
	//printf("p_cmd->argc = %d\n", p_cmd->argc);
}

int execute(command_t* p_cmd){

	pid_t pidn_1;         // pid for child process
	int status;           // status used in waitpid()

	int fnd = FALSE;
	char* fullpath = (char*)malloc(100);

	fnd = find_fullpath(fullpath, p_cmd);
	//printf("FULLPATH = %s\n", fullpath);
		
	if (fnd) {

		if ((pidn_1 = fork()) == 0){  // only "child" enters the if statement
			execv(fullpath, p_cmd->argv);
			perror("Execute terminated with an error condition: "); 
			exit(1);
		}

		while (waitpid(pidn_1, &status, 0) > 0) {
			if (WIFEXITED(status)){	// if child terminated normally
				//printf("child process terminated normally\n");
			}else{
				printf("error: child process did not terminate normally\n");
			}
		}
	}
	return WEXITSTATUS(&status);
}

int find_fullpath(char* fullpath, command_t* p_cmd){

	int found = FALSE;

	struct stat buffer;
	int exists;

	char* path_env_variable;
	path_env_variable = getenv("PATH");

	int savej = 0;
	for(int i = 0; i < myStrTok(path_env_variable, ':'); i++){
		int counter = 0; //count num letters in each argument so we can malloc properly
		for(int j = savej; j < myCountChar(path_env_variable); j++){ //loop through whole str,
			if(path_env_variable[j] == ':' || path_env_variable[j+1] == '\0'){
				char* file_or_dir = (char*)malloc(counter+1);
				char* dir_cmd = (char*)malloc(counter+20);//could be more precise here
				for(int k = 0; k < counter; k++){ 				
					file_or_dir[k] = path_env_variable[savej];
					savej++;
				}file_or_dir[counter+1] = '\0';

				savej = j+1; //save place in string +1 to skip the :
				sprintf(dir_cmd, "%s/%s", file_or_dir, p_cmd->name); 
				//printf("dir_cmd = %s\n", dir_cmd);

				exists = stat(dir_cmd, &buffer);
				if ( exists == 0 && (S_IFREG & buffer.st_mode)) {
					// File exists
					//printf("file exists!\n");
					int l = myCountChar(dir_cmd);
					for(int x=0; x<l; x++){	
						fullpath[x] = dir_cmd[x];
					}fullpath[l+1] = '\0';
					//printf("dir_cmd = %s\n", dir_cmd);
					//printf("fullpath = %s\n", fullpath);
					return TRUE;
					
				} else {
					// Not a valid file or directory
					//printf("not a valid file or directory\n");
					found = FALSE;	
				}


				break;
			}else
				counter++;
		}
	}
	return found;	
}

int is_builtin(command_t* p_cmd){
	int arraySize = sizeof(valid_builtin_commands)/sizeof(valid_builtin_commands[0]);
	for(int i = 0; i < arraySize-1; i++){
		if(myOtherStrCmp(p_cmd->name, valid_builtin_commands[i]) == 0){
			return TRUE;
		}
	}	
	return FALSE;
}

int do_builtin(command_t* p_cmd){ 
	if(myOtherStrCmp(p_cmd->name, "cd") == 0){ 
		if(chdir(p_cmd->argv[1]) == SUCCESSFUL)
			return SUCCESSFUL;
		else
			return ERROR;
	}
	return ERROR;
}

void cleanup(command_t* p_cmd){ 

	//free(p_cmd->name); - same as argv[0]

	for(int i = 0; i < p_cmd->argc; i++){
		free(p_cmd->argv[i]);
	}
	free(p_cmd->argv);
	
	p_cmd->name = NULL;
	for(int i = 0; i < p_cmd->argc; i++){
		p_cmd->argv[i] = NULL;
	}
	p_cmd->argv = NULL;
	p_cmd->argc = 0;
	p_cmd = NULL;
}
