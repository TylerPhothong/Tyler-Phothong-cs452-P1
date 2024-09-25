#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>  
#include <readline/readline.h>
#include <readline/history.h>
#include "lab.h"

char *get_prompt(const char *env) {
    char *prompt = getenv(env);
    if (prompt == NULL) {
        prompt = "shell>";
    }
    char *result = malloc(strlen(prompt) + 1);
    strcpy(result, prompt);
    return result;
}

int change_dir(char **dir) {
    if (dir[1] == NULL) {
        const char *home = getenv("HOME");
        if (home == NULL) {
            struct passwd *pw = getpwuid(getuid());
            home = pw->pw_dir;
        }
        return chdir(home);
    } else {
        return chdir(dir[1]);
    }
}

char **cmd_parse(char const *line) {
    size_t arg_max = sysconf(_SC_ARG_MAX);
    char **args = malloc(sizeof(char *) * arg_max);
    char *token;
    char *line_copy = strdup(line);
    int i = 0;

    token = strtok(line_copy, " ");
    while (token != NULL) {
        args[i] = strdup(token);
        token = strtok(NULL, " ");
        i++;
    }
    args[i] = NULL;

    free(line_copy);
    return args;
}

void cmd_free(char **line) {
    if (!line) return;
    for (int i = 0; line[i] != NULL; i++) {
        free(line[i]);
    }
    free(line);
}

char *trim_white(char *line) {
    char *end;
    while (*line == ' ') line++;
    if (*line == 0) return line;
    end = line + strlen(line) - 1;
    while (end > line && *end == ' ') end--;
    end[1] = '\0';
    return line;
}

bool do_builtin(struct shell *sh, char **argv) {
    UNUSED(sh);

    if (strcmp(argv[0], "exit") == 0) {
        exit(0);
    }

    if (strcmp(argv[0], "cd") == 0) {
        change_dir(argv);
        return true;
    }

    if (strcmp(argv[0], "history") == 0) {
        HIST_ENTRY **history_list = history_list;
        if (history_list) {
            for (int i = 0; history_list[i]; i++) {
                printf("%d: %s\n", i + history_base, history_list[i]->line);
            }
        }
        return true;
    }

    return false;
}

void sh_init(struct shell *sh) {
    sh->shell_terminal = STDIN_FILENO;
    sh->shell_is_interactive = isatty(sh->shell_terminal);

    if (sh->shell_is_interactive) {
        while (tcgetpgrp(sh->shell_terminal) != (sh->shell_pgid = getpgrp()))
            kill(-sh->shell_pgid, SIGTTIN);
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        sh->shell_pgid = getpid();
        if (setpgid(sh->shell_pgid, sh->shell_pgid) < 0) {
            perror("Couldn't put the shell in its own process group");
            exit(1);
        }
        tcsetpgrp(sh->shell_terminal, sh->shell_pgid);
        tcgetattr(sh->shell_terminal, &sh->shell_tmodes);
    }
}

void sh_destroy(struct shell *sh) {
    tcsetattr(sh->shell_terminal, TCSADRAIN, &sh->shell_tmodes);
}

void parse_args(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                printf("Shell version %d.%d\n", lab_VERSION_MAJOR, lab_VERSION_MINOR);
                exit(0);
            default:
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}
