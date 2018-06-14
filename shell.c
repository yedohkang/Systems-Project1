#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "shell.h"


/*======== void print_args() ==========
Inputs: char** args
Returns: void

Prints the arguments given, args.
====================*/
void print_args(char** args) {
	int i = 0;
	while (args[i]) {
		printf("%s\n", args[i]);
		i++;
	}
}


/*======== char **parse_args() ==========
Inputs: char* line
Returns: char** args

Takes in a line that the user inputted, and breaks it 
into a char**, args. It also gets rid of new lines.
====================*/
char** parse_args(char* line) {
	char** args = (char**)calloc(6, sizeof(char*));
	int i = 0;
	char* tp;
	while (line) {
		tp = strsep(&line, " ");
		args[i] = tp;
		//checks for new line
		tp = strsep(&tp, "\n");
		if (!tp){
			args[i] = 0;
		}
		i++;
	}
	return args;
}


/*======== void parse_redir_out() ==========
Inputs: char** redirlist
Returns: void

Given the user-inputted command, it checks to see if it needs 
to redirect stdout -- if so, it executes the given command, 
and redirects stdout to the given file. (Creates a new file 
if it does not already exist).
====================*/
void parse_redir_out(char** redirlist) {
	char* filename[512];
	if (redirlist[1]) {
		int i = 0;
		char* prev_cmd[512];
		// separate by >
		while (redirlist[i]) {
			if (strcmp(redirlist[i], ">") != 0){
				prev_cmd[i] = redirlist[i];
			} else {
				break;
			}
			i++;
		}
		// file name
		filename[0] = redirlist[i + 1];
		// stdout
		if (redirlist[i]) {
			int fd = open(filename[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			int stdout_fd = dup(1);
			dup2(fd, 1);
			close(stdout_fd);
			redirlist[i] = 0;
		}
	}
}


/*======== void parse_redir_in() ==========
Inputs: char** redirlist
Returns: void

Given the user-inputted command, it checks to see if it needs 
to redirect stdin -- if so, it reads the given file, and 
executes the command in the shell.
====================*/
void parse_redir_in(char** redirlist) {
	char* filename[512];
	if (redirlist[1]) {
		int i = 0;
		char* prev_cmd[512];
		// separate by <
		while (redirlist[i]) {
			if(strcmp(redirlist[i], "<") != 0){
				prev_cmd[i] = redirlist[i];
			} 
			else {
				break;
			}
			i++;
		}
		// file name
		filename[0] = redirlist[i + 1];
		// stdin
		if (redirlist[i]) {
			int fd = open(filename[0], O_RDONLY);
			dup2(fd, 0);
			close(fd);
			redirlist[i] = 0;
		}
	}
}


/*======== void parse_pipe() ==========
Inputs: char** redirlist
Returns: void

Executes simple pipes, given by the user-inputted command. It
checks to see if it has a |, then runs the first command and 
then uses the output from the first command as the input
for the second command. 
====================*/
void parse_pipe(char** redirlist) {
	// if ls | wc, then ls > out then wc < out
  int i = 0;
	int j = 0;
  while (redirlist[i]) {
    if (strcmp(&redirlist[i][0],"|") == 0) {
      char commands[512] = "";
			while (j < i) {
        strcat(commands, redirlist[j]);
        strcat(commands, " ");
				j++;
			}
			// piping
      FILE *file = popen(commands, "r");
      int fd = fileno(file);
      dup2(fd, 0);
      close(fd);
		  redirlist += i + 1;
      execvp(redirlist[0], redirlist);
    }
		i++;		
  }
}


/*======== void execute_commands() ==========
Inputs: char* args[256]
Returns: void

Executes the inputted command by forking, and deals with 
cd, exit, and redirection.
====================*/
void execute_commands(char* args[256]){	
	// exit
	if (strcmp(args[0], "exit") == 0){
		exit(0);
	}
	// cd
	if (strcmp(args[0], "cd") == 0) {
		if (args[1]) {
			if (chdir(args[1]) == -1) {
				printf("whomst such directory.\n");
			}
			return;
		}
		else {
			chdir(getenv("HOME"));
			return;
		}
	}
	// forking
	int f = fork();
	if (f == 0) {
		parse_redir_out(args);
		parse_redir_in(args);
		parse_pipe(args);
		execvp(args[0], args);
		printf("invalid command\n");
		exit(1);
	}
	else {
		int status;
		wait(&status);
	}
}


/*======== void run() ==========
Inputs: None
Returns: void

Gets the command the user has imputted, parses that line, 
and runs the execute_commands() function on the parsed
arguments, including semicolons.
====================*/
void run() {
	while (1) {
		printf("$ ");
		char line[256];
		fgets(line, 256, stdin);
		char** args = parse_args(line);
		// deals with semicolons
		int i = 0;
		char* cmd0[256] = {};
		char* cmd1[256] = {};
		int semicol = 0;
		int j = 0;
		while (args[i]) {
			if (semicol == 0) {
				if (strcmp(args[i], ";") == 0) {
					semicol = 1;
				}
				else {
					cmd0[i] = args[i];
				}
			}
			if (semicol == 1 && strcmp(args[i], ";") != 0) {
				cmd1[j] = args[i];
				j++;
			}
			i++;
		}
		execute_commands(cmd0);
		if (semicol == 1) {
			execute_commands(cmd1);
		}
		free(args);
	}
}
