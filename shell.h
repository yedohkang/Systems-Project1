void print_args(char** args); // prints out single command

char** parse_args(char* line); // parse whitespace and newline

void parse_redir_in(char** redirlist); // parse the simple redirection

void parse_redir_out(char** redirlist); // parse the simple redirection

void parse_pipe(char** redirlist); // parse the simple pipe

void execute_commands(); // execute

void run(); // run
