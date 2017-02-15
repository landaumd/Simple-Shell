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
// -------------------------------------------- ****** i doubled up helper functions because tony mentioned that 
						// he might run out program with his own main... where to put them? ************

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
static int myOtherStrCmp(char* s, char* t){
	/*int c = 0;
	
	while(s[c] == t[c]){
		if(s[c] == '\0' || t[c] == '\0')
			break;
		c++;
	}
	if(s[c] == '\0' && t[c] == '\0')
		return 0;
	else
		return -1;*/

	if(s != '\0' || t != '\0'){
		while(s == t && s != '\0' && t != '\0'){
			s++; t++;
		}
		if( s == '\0')
			return *s-*t;
		else{
			printf("error comparing strings\n");
			return -1;
		}
	}else{
		printf("one or both strings are null\n");
		return -1;
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
	for(int i = 0; i < p_cmd->argc; i++)
		printf("argv[%d] = '%s'\n", i, p_cmd->argv[i]);

	//argv[0] is the name
	p_cmd->name = p_cmd->argv[0];
	
	//printf("p_cmd->name = %s\n", p_cmd->name); 
	//printf("p_cmd->argc = %d\n", p_cmd->argc);
}

int execute(command_t* p_cmd){

	/*int fnd = FALSE;
	char* fullpath;

	fnd = find_fullpath(fullpath, p_cmd);
		
	if (fnd) {
		if (fork() == 0) {
			// This is the child
			// Execute in same environment as parent

			execv(fullpath, p_cmd->argv);
			// determine proper arguments...
			perror("Execute terminated with an error condition!\n");
			exit(1);
		}
		// This is the parent - wait for the child to terminate
		wait( ... );
		// waitpid() could be used instead
	} else {
		// display to user cmd not found
	}*/


	pid_t pidn_1;         // pid for child process
	int status;           // status used in waitpid()

	int fnd = FALSE;
	char* fullpath = (char*)malloc(100);

	fnd = find_fullpath(fullpath, p_cmd);
	printf("%s\n", fullpath);
		
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
				sprintf(dir_cmd, "%s/%s", file_or_dir, p_cmd->name); //no!
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

void cleanup(command_t* p_cmd){ // keeps dying when I try to free things! ***************************************************!!!!!

	//free(p_cmd->name);
	//free(p_cmd->argv);
	//free(p_cmd);
	
	//free(p_cmd->argv[0]); //BREAKS!
	for(int i = 0; i < p_cmd->argc; i++){
		free(p_cmd->argv[i]);
	}
	free(p_cmd->argv);

	
	/*p_cmd->name = NULL;
	for(int i = 0; i < p_cmd->argc; i++){
		p_cmd->argv[i] = NULL;
	}
	p_cmd->argv = NULL;
	p_cmd->argc = 0;
	p_cmd = NULL;*/
}

