# Simple Shell
Build a simple shell for Operating Systems HW2 Assignment.   
   
## Objectives   
In this assignment you will develop a simple command line interpreter (or shell) for the Linux kernel using
the C programming language. This assignment will allow you to gain experience in the following areas:  
   
&nbsp;&nbsp;&nbsp;&nbsp;• **More Basic Programming:** This includes variable declaration, data types, arrays, pointers, oper-
ators, expressions, selection statements, looping statements, functions, structs, and header files.  
&nbsp;&nbsp;&nbsp;&nbsp;• **System Calls:** Become familiar with system calls that are capable of executing external programs,
getting and changing the directory, working with environmental variables, and working with processes
(i.e. fork, exec, and wait).  
   
## System and Standard Lib Functions  
This assignment will use the system and standard library functions listed below. Please ensure your familiar
with the syntax, and usage of them. Detailed information about each function listed below can be found
using the man command from the console: i.e. man chdir, will show the man page (short for manual
page) for the chdir function.  
   
&nbsp;&nbsp;&nbsp;&nbsp;• **Change Directory:** int chdir(const char* path)  
&nbsp;&nbsp;&nbsp;&nbsp;• **Execute External Command:** int execv(const char* file, char* const argv[])  
&nbsp;&nbsp;&nbsp;&nbsp;• **Get Environmental Variable**: char* getenv(const char* name)  
&nbsp;&nbsp;&nbsp;&nbsp;• **Error Mapping:** void perror(const char* string)  
&nbsp;&nbsp;&nbsp;&nbsp;• **Fork a Process:** pid_t fork(void)  
&nbsp;&nbsp;&nbsp;&nbsp;• **Memory Allocation:** void* malloc(size t_size)  
&nbsp;&nbsp;&nbsp;&nbsp;• **Free Memory:** void free(void* ptr)   
&nbsp;&nbsp;&nbsp;&nbsp;• **File/Directory Status:** int stat(const char* path, struct stat* buffer)  
&nbsp;&nbsp;&nbsp;&nbsp;• **Blocking Operation:** pid t wait(int *status)  
  
## Provided Files  
The three files listed below are provided to you.  
   
&nbsp;&nbsp;&nbsp;&nbsp;• **shell.h:** Header file that defines the command struct used in this assignment, constants, and the
function prototypes to be completed by you. Please note: You may not add new function definitions
to this header file.  
&nbsp;&nbsp;&nbsp;&nbsp;• **shell.c:** The file containing the implementation of the functions listed in shell.h. Having a different file for the implementation separates interface (the include file) from the implementation (the .c file).  
&nbsp;&nbsp;&nbsp;&nbsp;• **hw2.c:** Source code file that includes a stubbed out version of the main function, and defines the
libraries and constants used in this assignment. Please note: You may not remove, modify, or add (i.e. #include) additional libraries. The ones that are provided, are the only libraries needed for this
assignment.  
   
The hw2.c, shell.h and shell.c files contain many comments that provide basic definitions and step-by-step
instructions. Please read the comments carefully and follow the instructions. 
   
## To Do
Using the shell.h file, you must provide working implementations within the corresponding shell.c file for
the following function prototypes:  
    
&nbsp;&nbsp;&nbsp;&nbsp;1. void parse( char* line, command_t* p_cmd )  
&nbsp;&nbsp;&nbsp;&nbsp;2. int execute( command_t* p_cmd )  
&nbsp;&nbsp;&nbsp;&nbsp;3. int find_fullpath( char* fullpath, command_t* p_cmd )  
&nbsp;&nbsp;&nbsp;&nbsp;4. int is_bultin( command_t* p_cmd )  
&nbsp;&nbsp;&nbsp;&nbsp;5. int do_builtin( command_t* p_cmd )  
&nbsp;&nbsp;&nbsp;&nbsp;6. void cleanup( command_t* p_cmd )  
   
For each function prototype listed above, numerous comments are provided in the header file to guide you
in this assignment. Please read them carefully, they either provide basic step-by-step instructions, or basic
calculations.  
