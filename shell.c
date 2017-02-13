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
int myStrTok(char* cmd, char d){
	int counter = 0;
	//need to edit so that multiple delimeters in a row don't count
	while(*cmd != '\0'){
		if(*cmd == d)
			counter++;
		*cmd++;
	}
	if(d == ' ')//for commands with spaces in between = one extra!
		counter++;
	return counter;	
}

// Count the number of chars in a string.
int myCountChar(char* str){
	int counter = 0;
	while(*str != '\0'){
		counter++;
		*str++;
	}
	return counter;
}

// Compare two strings.
int myOtherStrCmp(const char* s, const char* t){
	if(s != '\0' || t != '\0'){	
		while(s == t && s != '\0'){
			s++; t++;
		}
		return *s-*t;
	}else{
		printf("One or both strings are null.");
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
		int counter = 0; 
		for(int j = savej; j < myCountChar(line); j++){ //loop through whole str,
			if(line[j] == ' ' || line[j+1] == '\0'){
				p_cmd->argv[i] = (char*)malloc(counter+1);
				for(int k = 0; k < counter; k++){ //save chars into argv[i]				
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

	/*
	int fnd = FALSE;
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

}

int find_fullpath(char* fullpath, command_t* p_cmd){
	//Confused about this... why do I care if the directory exists? Don't
	// I just want to know if the file exists so I can execute it? cd? ls?
	//Also, why is the fullpath a parameter? Don't I find the full path in here?
	// Where is the for loop supposed to go? Why is p_cmd a parameter??

	struct stat buffer;
	int exists;

	// string that represents the fully qualified
	// path of a file or directory on the file system
	char* file_or_dir = fullpath;

	exists = stat(file_or_dir, &buffer);
	if (exists == 0 && (S_IFDIR & buffer.st_mode)) {
		// Directory exists
		printf("Directory exists\n");
		return TRUE;
	} else if ( exists == 0 && (S_IFREG & buffer.st_mode)) {
		// File exists
		printf("File exists\n");
		return TRUE;
	} else {
		// Not a valid file or directory
		printf("not a valid file or directory\n");
		return FALSE;	
	}	
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

int do_builtin(command_t* p_cmd){ //do i need find_fullpath for this??
	if(myOtherStrCmp(p_cmd->name, "cd") == 0){ //is this too specific for cd?
		if(chdir(p_cmd->argv[1]) == SUCCESSFUL)
			return SUCCESSFUL;
		else
			return ERROR;
	}
	return ERROR;
}

void cleanup(command_t* p_cmd){

	free(p_cmd->name);
	free(p_cmd->argv);
	free(p_cmd);
	
	//free(p_cmd->argv[0]); //BREAKS!
	//for(int i = 0; i < p_cmd->argc; i++){
	//	free(p_cmd->argv[i]);
	//}
	
	p_cmd->name = NULL;
	for(int i = 0; i < p_cmd->argc; i++){
		p_cmd->argv[i] = NULL;
	}
	p_cmd->argv = NULL;
	p_cmd->argc = 0;
	p_cmd = NULL;
}

