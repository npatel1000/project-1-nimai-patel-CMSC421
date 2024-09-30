[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/OHwIpFF9)
# Project 1
Answer these questions please:

Project Overview:
-----------------------------------
What is the project about? Provide a brief overview that includes the project's name, purpose, and target audience.
It was to create a simple Linux Shell. Its purpose is to provide a command line interface where users can execute system commands, interact with /proc, retrieve information, and log command history into a history file. The target audience would be those learning shell programming. 

Solution Approach:
------------------------------------------------------------
How did you approach the problem? Outline the steps or phases in your project development process?
I created a shell prompt loop, wrote a command parser for user input, used fork() and execvp() to create a child process and replace it with the command provided by user input, implemented logging into .421sh file for user command history, a display command for this. I allowed the user to read data from the /proc file system. Finally, I ensured all dynamically allocted memory was freed.

Challenges and Solutions?
---------------------------
What challenges did you encounter while completeing your project, and how did you overcome them?
It took me a long time to figure this out, but the biggest challenge I had was that \n was causing the commands to be read with \n at the end. I added:

    if (command[strlen(command) - 1] == '\n') {
        command[strlen(command) - 1] = '\0';
    }

    return command;
}

in get_user_command() to fix it. 

LLM Disclosure
--------------
Are there any external resources or references that might be helpful?
Stack Overflow threads and GeeksforGeeks.org.

Testing and Validation
-----------------------
Testing Methodology: How did you test your project? Discuss any unit tests, integration tests, or user acceptance testing conducted.
Are there any specific commands or scripts that should be known?
First wrote, then did trail and error after. A lot of debugging to figure out the issue mentioned above. 
exit: exits shell
display_history: displays history
/proc related commands - Example: /proc/cpuinfo

Example session
---------------
Provide examples on running your project
Screenshot link of several command inputs: https://gyazo.com/609d8cc7e18eef0f47036f10877f794d
