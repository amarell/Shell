# Shell

## Project description

A Linux shell with some basic functionalities written in the C language.
Some of the commands our shell supports are:
 - `cd` - to change current directory
 - other general linux shell commands such as `ls, ls -l, ps, ps -a` etc
 - `history` - used to display n recently used commands where n is the size of the history buffer
 - `history -s {n}` - used to change the history buffer so that it stores n recently used commands
 - `history {n}` - used to display n recently used commands
 - `!{n}` - used to execute the the nth command in the buffer. For example, !2 would mean repeating the command within the buffer which has the id of n.
 - `!-{n}` - used to execute the nth most recent command. For example,  !-1 would mean to repeat the last command.
 - `exit` - which terminates the shell

Just like the majority of shells, our shell is designed to display the current working directory before asking for input.

## Students who worked on this project
 - Nedim Bandžović
 - Adnan Mujagić
 - Muhamed Begić
 - Amar Mujagić

## Files within the submission
 - README.md - this file
 - shell3.c - the final version of the shell. It's called shell3 because it was the third version
 - OS.docx - the document that contains the answers to the theoretical questions in our task

## Compile with
	> gcc -Wall -o shell3 shell3.c -lreadline
## Run with
	> ./shell3
  
## Possible issues
When using the `!{n}` or `!-{n}` commands it is possible to get an error in case that the command to be repeated is one of the custom-made commands due to some code limitations.
