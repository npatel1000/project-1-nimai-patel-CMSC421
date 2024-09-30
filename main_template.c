/*
project: 01
author: Nimai Patel 
email: npatel17@umbc.edu
student id: GN38452
description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"

/*
In this project, you are going to implement a number of functions to 
create a simple linux shell interface to perform basic linux commands
*/

void display_history();
void user_prompt_loop();
char *get_user_command();
char **parse_command(char *input);
void execute_command(char **args);
void handle_proc_command(char **args);
void add_to_history(char *command);

int main(int argc, char **argv)
{
    /*
    Write the main function that checks the number of argument passed to ensure 
    no command-line arguments are passed; if the number of argument is greater 
    than 1 then exit the shell with a message to stderr and return value of 1
    otherwise call the user_prompt_loop() function to get user input repeatedly 
    until the user enters the "exit" command.
    */
    
    (void)argv; //remove warning
    
    if (argc > 1) { //check if more than 1 arguement is passed to shell
    	fprintf(stderr, "Arguments not supported. Exiting.\n"); //print error message
    	return 1;
    }
    
    user_prompt_loop(); //enter main shell loop
    return 0;
}

/*
user_prompt_loop():
Get the user input using a loop until the user exits, prompting the user for a command.
Gets command and sends it to a parser, then compares the first element to the two
different commands ("/proc", and "exit"). If it's none of the commands, 
send it to the execute_command() function. If the user decides to exit, then exit 0 or exit 
with the user given value. 
*/

void user_prompt_loop()
{
    // initialize variables

    /*
    loop:
        1. prompt the user to type command by printing >>
        2. get the user input using get_user_command() function 
        3. parse the user input using parse_command() function 
        Example: 
            user input: "ls -la"
            parsed output: ["ls", "-la", NULL]
        4. compare the first element of the parsed output to "/proc"and "exit"
        5. if the first element is "/proc" then you have the open the /proc file system 
           to read from it
            i) concat the full command:
                Ex: user input >>/proc /process_id_no/status
                    concated output: /proc/process_id_no/status
            ii) read from the file line by line. you may user fopen() and getline() functions
            iii) display the following information according to the user input from /proc
                a) Get the cpu information if the input is /proc/cpuinfo
                - Cpu Mhz
                - Cache size
                - Cpu cores
                - Address sizes
                b) Get the number of currently running processes from /proc/loadavg
                c) Get how many seconds your box has been up, and how many seconds it has been idle from /proc/uptime
                d) Get the following information from /proc/process_id_no/status
                - the vm size of the virtual memory allocated the vbox 
                - the most memory used vmpeak 
                - the process state
                - the parent pid
                - the number of threads
                - number of voluntary context switches
                - number of involuntary context switches
                e) display the list of environment variables from /proc/process_id_no/environ
                f) display the performance information if the user input is /proc/process_id_no/sched
        6. if the first element is "exit" the use the exit() function to terminate the program
        7. otherwise pass the parsed command to execute_command() function 
        8. free the allocated memory using the free() function
    */

    /*
    Functions you may need: 
        get_user_command(), parse_command(), execute_command(), strcmp(), strcat(), 
        strlen(), strncmp(), fopen(), fclose(), getline(), isdigit(), atoi(), fgetc(), 
        or any other useful functions
    */

    char *command = NULL; //user's input command
    char **parsed_command = NULL; //parsed components of command

    while (1) { //infinite loop prompting user for commands
        printf(">> "); //print shell prompt
        command = get_user_command(); //get user command

        if (!command || strlen(command) == 0 || strcmp(command, "\n") == 0) { //if input is empty (spaces and tabs)
    		free(command);  //free memory for empty imputs
    		continue;  //prompt again
	}

        parsed_command = parse_command(command); //parse command into arguements

        if (parsed_command[0] == NULL) {
            free(command);  //free allocated memory
            free(parsed_command);
            continue;  //prompt again 
        }
	
	add_to_history(command); //add command to history
	
        if (strcmp(parsed_command[0], "exit") == 0) { //check built-in commands 
        	if (parsed_command[1]) {
        		fprintf(stderr, "Invalid exit command! Try: exit\n"); //print this if the command has arguments
    		} else {
        		free(command); //free allocated memory
        		free(parsed_command);
        		exit(0);  //exit shell
    		}
        } else if (strcmp(parsed_command[0], "display_history") == 0) {
		display_history();  //display history
        } else if (strncmp(parsed_command[0], "/proc", 5) == 0 && parsed_command[1] != NULL) { //if command starts with "/proc"
		handle_proc_command(parsed_command);
        } else {
		execute_command(parsed_command); //execute external command
        }

        free(command); //free allocated memory
        free(parsed_command);
    }
}

/*
get_user_command():
Take input of arbitrary size from the user and return to the user_prompt_loop()
*/

char *get_user_command()
{
    /*
    Functions you may need: 
        malloc(), realloc(), getline(), fgetc(), or any other similar functions
    */
    
    char *command = NULL; //pointer for unput
    size_t len = 0; //intial size of buffer

    if (getline(&command, &len, stdin) == -1) { //read command from stdin
        if (feof(stdin)) {
            free(command); //free allocated memory
            exit(0); //exit in case of Ctrl + D
        } else {
            perror("getline"); //print error message
            free(command);  //free allocated memory
            return NULL; //return null if error
        }
    }

    return command;
}

/*
parse_command():
Take command grabbed from the user and parse appropriately.
Example: 
    user input: "ls -la"
    parsed output: ["ls", "-la", NULL]
Example: 
    user input: "echo     hello                     world  "
    parsed output: ["echo", "hello", "world", NULL]
*/

char **parse_command(char *input)
{
    /*
    Functions you may need: 
        malloc(), realloc(), free(), strlen(), first_unquoted_space(), unescape()
    */

    int pos = 0;
    int buffer_size = 10;  //set buffer size to 10
    char **tokens = malloc(sizeof(char *) * buffer_size); //allocate memory array
    if (!tokens) {  //check for malloc fail
        fprintf(stderr, "Error! Memory allocation failed!\n");
        return NULL;
    }

    char *token = strtok(input, " ");  //split input with spaces

    while (token != NULL) {
        tokens[pos++] = strdup(token);  //duplicate into arrays

        if (pos >= buffer_size) { //if buffer size is exceeded, reallocate
            buffer_size += 10;
            char **new_tokens = realloc(tokens, sizeof(char *) * buffer_size);
            if (!new_tokens) {  //check realloc
                fprintf(stderr, "Error! Memory allocation failed!\n");
                free(tokens);  //free allocated memory
                return NULL;
            }
            tokens = new_tokens;  //assign new reallocated memory
        }

        token = strtok(NULL, " "); //get next token
    }
    tokens[pos] = NULL;  //terminate array

    return tokens;
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/

void execute_command(char **args)
{
    /*
    Functions you may need: 
        fork(), execvp(), waitpid(), and any other useful function
    */

    pid_t pid = fork(); //creating child process

    if (pid == 0) { //replace process with command for child process
        if (execvp(args[0], args) == -1) {
            perror("execvp"); //error message
        }
        exit(EXIT_FAILURE); //exit child process
    } else if (pid < 0) {
        perror("fork"); //if error forking
    } else {
        int status; //parent process waits on child process
        waitpid(pid, &status, 0); //using waitpid for better control
    }
}

void handle_proc_command(char **args) {
    char path[256]; //file path buffer
    
    snprintf(path, sizeof(path), "%s", args[0]); //create file path

    FILE *file = fopen(path, "r"); //open /proc to read
    if (!file) {
        fprintf(stderr, "Error! Could not open %s! It may not exist or permission denied.\n", path);
        return;
    }

    char *line = NULL; //buffer for each line
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
	printf("%s", line);  //print lines
    }

    fclose(file); //close the file
    free(line); //free allocated buffer
}

void add_to_history(char *command) {
    FILE *history_file = fopen(".421sh", "a"); //open file in append mode
    if (!history_file) {
        fprintf(stderr, "Error! Could not open history file for writing!\n");
        return;
    }

    fprintf(history_file, "%s\n", command); //write command to file
    fclose(history_file); //close file
}

void display_history() {
    FILE *history_file = fopen(".421sh", "r"); //open file in read mode
    if (!history_file) {
        fprintf(stderr, "Error! Could not open history file for reading!\n");
        return;
    }

    char *line = NULL; //buffer for each command
    size_t len = 0; //length of buffer
    int count = 0; //counting number of commands
    
    while (getline(&line, &len, history_file) != -1 && count < 10) { //read and display up to 10 lines
        printf("%s", line);
        count++;
    }

    fclose(history_file); //close file
    free(line); //free allocated buffer
}


