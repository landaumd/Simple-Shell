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
// --------------------------------------------  	where were these supposed to go? ****************************

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

// When user begins shell.
void printWelcome(){
	printf("⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬\n");
	printf("	 MASH (Megan's Awesome Shell) ☺\n");
	printf("⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬\n");
}

// When shell is quit / exited.
void printBye(){
	printf("⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬\n");
	printf("		     BYE! ☹\n");
	printf("⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬⚬\n");
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
	//  	else 
		   //<-^if (is _builtin(...))
	//			do_builtin(...)
	//		else if (find_fullpath(...))
		//  		<-execute(...)
	//		else
	//			error()
//	 	->cleanup(...)
	//
	
	printWelcome();	

	char readin[100];
	int done = FALSE; 

	while (!done){
		printf("⌨   "); 
		fgets(readin, 100, stdin);

		if(myStrLen(readin) != 0 && readin[0] != ' ' && readin[0] != '\n'){
			command_t* cptr;
			cptr = (command_t*) malloc(sizeof(command_t));
			parse(readin, cptr);

			if (myStrCmp(cptr->name, "exit") == 0 || myStrCmp(cptr->name, "quit") == 0){
				printBye();
				done = TRUE;
			}else{
				if(is_builtin(cptr)){ //if the command is builtin, like "cd"
					int outcome = do_builtin(cptr);
					if(outcome != SUCCESSFUL)
						printf("%s: error completing command\n", cptr->name);
				}
				else if(find_fullpath("", cptr)){ //not sure what to do with fullpath?**********************
										// where does that for loop go? ************
					//if path has been found and cptr->name has been changed, then just execute!********
					execute(cptr);
	
				}else{
					//error() ?? what function is this?
					printf("%s: command not found\n", cptr->name);
				}
			}


		cleanup(cptr); //is this supposed to be here? ***************not working?**********************

		}
	}
	return 0;

} // end main function
