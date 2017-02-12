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
// My helper functions:
// --------------------------------------------

// Find the length of a string.
int myStrLen(char *line){
	int i = 0;
	for(i; line[i] != '\0'; ++i);
	return i;
}

// Compare two strings.
int myStrCmp(const char* s, const char* t){
	if(s != '\0' || t != '\0'){	
		while(s == t && s != '\0'){
			s++; t++;
		}
		return *s-*t;
	}else{
		printf("One or both strings are null.");
	}
}

// --------------------------------------------------------
// Main function:
// Arguments:	argc = number of arguments suppled by user
//		argv = array of argument values
// --------------------------------------------------------

int main(int argc, char** argv) {

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while (!done)
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
        //      if (parsed command is "exit")
        //      	done = true
	//  	else 
		   //<-^if (is _builtin(...))
	//			do_builtin(...)
	//		else if (find_fullpath(...))
	//  			//<-execute(...)
	//		else
	//			error()
	// 	->cleanup(...)
	//

	printf("CSCI 340 Homework Assignment 2 - Have Fun!\n");
	
	char readin[100];
	int done = FALSE;

	while (!done){
		printf(">>> ");
		fgets(readin, 100, stdin);

		if(myStrLen(readin) != 0 && readin[0] != ' ' && readin[0] != '\n'){
			command_t* cptr;
			cptr = (command_t*) malloc(sizeof(command_t));
			parse(readin, cptr);
	
			char* path_env_variable;
			path_env_variable = getenv("PATH");
			//printf("PATH = %s\n", path_env_variable);

			if (myStrCmp(cptr->name, "exit") == 0)
				done = TRUE;
			else{
				if(is_builtin(cptr)){ //if the command is built in, like "cd"
					int outcome = do_builtin(cptr);
					if(outcome == SUCCESSFUL)
						printf("%s: command completed successfully\n", cptr->name);
					else
						printf("%s: error completing command\n", cptr->name);
				}
				else if(find_fullpath("/bin/lsjlk", cptr)){
				
					printf("found full path\n");

					//execute
	
				}else{
					//error() ?? what function is this?
					printf("%s: command not found\n", cptr->name);
				}
			}


		cleanup(cptr);

		}
	}
	return 0;

} // end main function
