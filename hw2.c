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
static int myStrCmp(char* s, char* t){

/*	int c = 0;
	printf("s = '%s' t = '%s'\n", s, t);
	while(s[c] == t[c]){
		if(s[c] == '\0' || t[c] == '\0')
			break;
		c++;
	}
	if(s[c] == '\0' && t[c] == '\0')
		return 0;
	else
		return -1;
*/


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

// When user begins shell.
void printWelcome(){
	//⚬⚬
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

			cleanup(cptr);
		}

		
	}
	free(cptr);
	return 0;

} // end main function
