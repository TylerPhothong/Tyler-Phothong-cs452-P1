// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lab.h"

static struct shell my_shell;

int main(int argc, char **argv) {
    // Initialize the shell
    sh_init(&my_shell);
    parse_args(argc, argv);

    char *line;
    using_history();
    
    my_shell.prompt = get_prompt("MY_PROMPT");

    while ((line = readline(my_shell.prompt))) {
        if (*line) {
            add_history(line);

            // Parse command
            char **args = cmd_parse(line);

            // Check for built-in commands
            if (!do_builtin(&my_shell, args)) {
                pid_t pid = fork();

                if (pid == 0) { 
                    signal(SIGINT, SIG_DFL);
                    signal(SIGQUIT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execvp(args[0], args);
                    perror("exec failed");
                    exit(EXIT_FAILURE);
                } else if (pid > 0) { 
                    waitpid(pid, NULL, 0);
                } else {
                    perror("fork failed");
                }
            }

            cmd_free(args);
        }
        free(line);
    }

    sh_destroy(&my_shell);
    free(my_shell.prompt);

    return 0;
}
