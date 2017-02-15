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
static int myStrLen(char *line){
	int i;
	for(i = 0; line[i] != '\0'; ++i)
	  ;
	return i;
}

// Compare two strings.
// Compare two strings.
int myStrCmp(char* str1, char* str2){
	
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

// When user begins shell.
void printWelcome(){
	printf("-----------------------------------------------------------\n");
	printf("	    MASH (Megan's Awesome Shell) ☺\n");
	printf("-----------------------------------------------------------\n");
}

// When shell is quit / exited.
void printBye(){
	printf("-----------------------------------------------------------\n");
	printf("		         BYE! ☹\n");
	printf("-----------------------------------------------------------\n");
}

// --------------------------------------------------------
// Main function:
// Arguments:	argc = number of arguments suppled by user
//		argv = array of argument values
// --------------------------------------------------------

int main(int argc, char** argv) {

	// ----------------------------------
	// Pseudocode 				*************** I rearranged this *******************************
	// ----------------------------------
	// while (!done)
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
        //      if (parsed command is "exit")
        //      	done = true
	//  	else if (is _builtin(...))
	//		do_builtin(...)
	//	else if (find_fullpath(...))
	//  		execute(...)
	//	else
	//		error()
	//	cleanup(...)
	//
	
	printWelcome();	

	char readin[100];
	int done = FALSE; 

	command_t* cptr;
	cptr = (command_t*) malloc(sizeof(command_t));
	char* fullpath = malloc(100);


	while (!done){
		printf("⌨   "); 
		fgets(readin, 100, stdin);

		if(myStrLen(readin) != 0 && readin[0] != ' ' && readin[0] != '\n'){
			parse(readin, cptr);

			if (myStrCmp(cptr->name, "exit") == 0){
				printBye();
				done = TRUE;
			}else if(is_builtin(cptr)){ //if the command is builtin, like "cd"
				int outcome = do_builtin(cptr);
				if(outcome != SUCCESSFUL)
					printf("%s: error completing command\n", cptr->name);
				
			}else if(find_fullpath(fullpath, cptr)){ 
				execute(cptr);

			}else{
				printf("%s: command not found\n", cptr->name);
			}

	
			for(int i = 0; i < cptr->argc; i++)
				printf("argv[%d] = '%s'\n", i, cptr->argv[i]);
	
			printf("p_cmd->name = %s\n", cptr->name); 
			printf("p_cmd->argc = %d\n", cptr->argc);

			cleanup(cptr);
		}

		
	}
	free(cptr);
	return 0;

} // end main function
