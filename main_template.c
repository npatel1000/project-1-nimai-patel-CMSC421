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

/* 
DEFINE THE FUNCTION PROTOTYPES
user_prompt_loop()
get_user_command()
parse_command()
execute_command()
*/

int main(int argc, char **argv)
{
    /*
    Write the main function that checks the number of argument passed to ensure 
    no command-line arguments are passed; if the number of argument is greater 
    than 1 then exit the shell with a message to stderr and return value of 1
    otherwise call the user_prompt_loop() function to get user input repeatedly 
    until the user enters the "exit" command.
    */

    if (argc > 1) {
    	fprintf(stderr, "Arguments not supported. Exiting.\n");
    	return 1;
    }
    
    user_prompt_loop; //enter main shell loop
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

    char *command = NULL;
    char **parsed_command = NULL;

    while (1) {
        printf(">> ");
        command = get_user_command();

        if (!command) continue; //for empty inputs

        parsed_command = parse_command(command);

        //check built-in commands 
        if (strcmp(parsed_command[0], "exit") == 0) {
            if (parsed_command[1]) {
                fprintf(stderr, "Invalid exit command. Usage: exit\n");
            } else {
                free(command);
                free(parsed_command);
                exit(0);  //exit shell
            }
        } else if (strncmp(parsed_command[0], "/proc", 5) == 0) {
            handle_proc_command(parsed_command);
        } else {
            execute_command(parsed_command);
        }

        free(command);
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

char *command = NULL;
    size_t len = 0;

    if (getline(&command, &len, stdin) == -1) {
        if (feof(stdin)) {
            exit(0); 
        } else {
            perror("getline");
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

/*parse_command()*/
{
    /*
    Functions you may need: 
        malloc(), realloc(), free(), strlen(), first_unquoted_space(), unescape()
    */

    /*
    ENTER YOUR CODE HERE
    */
}

/*
execute_command():
Execute the parsed command if the commands are neither /proc nor exit;
fork a process and execute the parsed command inside the child process
*/

/*execute_command()*/
{
    /*
    Functions you may need: 
        fork(), execvp(), waitpid(), and any other useful function
    */

    /*
    ENTER YOUR CODE HERE
    */
}
