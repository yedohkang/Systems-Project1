# my_shell
## by Jennifer Zhang and Yedoh Kang, period 4

### Features:
  * Forks and executes commands (including cd and exit).
  * Parses up to two commands on one line.
  * Redirects using >, <.
  * Simple, single pipes (such as ls | wc).

### Attempted:
  * Parsing more than two commands on one line.

### Bugs:
  * If commands are not separated with spaces, they will not run properly.
  * If there are multiple commands adjacent to each other, it will not run properly. For example, two commas adjacent to each other. 

### Files & Function Headers:
main.c
  * Runs the shell running command.

shell.c
  * Contains all the functions for shell.

```
		/*======== void print_args() ==========
		Inputs: char** args
		Returns: void
		
		Prints the arguments given, args.
		====================*/
		
		/*======== char** parse_args() ==========
		Inputs: char* line
		Returns: char** args
		
		Takes in a line that the user inputted, and breaks it 
		into a char**, args. It also gets rid of new lines.
		====================*/
		
		/*======== void parse_redir_out() ==========
		Inputs: char** redirlist
		Returns: void
		
		Given the user-inputted command, it checks to see if it needs 
		to redirect stdout -- if so, it executes the given command, 
		and redirects stdout to the given file. (Creates a new file 
		if it does not already exist).
		====================*/
		
		/*======== void parse_redir_in() ==========
		Inputs: char** redirlist
		Returns: void
		
		Given the user-inputted command, it checks to see if it needs 
		to redirect stdin -- if so, it reads the given file, and 
		executes the command in the shell.
		====================*/
		
		/*======== void parse_pipe() ==========
		Inputs: char** redirlist
		Returns: void
		
		Given the user-inputted command, it checks to see if it has a |
		for simple pipes. If so, it runs the first command and then 
		uses the output from the first command as the input for the 
		second command. 
		====================*/
		
		/*======== void execute_commands() ==========
		Inputs: char* args[256]
		Returns: void
		
		Executes the inputted command by forking, and deals with 
		cd, exit, and redirection.
		====================*/
		
		/*======== void run() ==========
		Inputs: None
		Returns: void
		
		Gets the command the user has imputted, parses that line, 
		and runs the execute_commands() function on the parsed
		arguments, including semicolons.
		====================*/
```

shell.h
  * Contains all the headers for shell.c
