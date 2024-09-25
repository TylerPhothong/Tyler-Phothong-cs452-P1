// lab.h
#ifndef LAB_H
#define LAB_H

#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define lab_VERSION_MAJOR 1
#define lab_VERSION_MINOR 0
#define UNUSED(x) (void)x

#ifdef __cplusplus
extern "C"
{
#endif

    struct shell
    {
        int shell_is_interactive;
        pid_t shell_pgid;
        struct termios shell_tmodes;
        int shell_terminal;
        char *prompt;
    };

    char *get_prompt(const char *env);
    int change_dir(char **dir);
    char **cmd_parse(char const *line);
    void cmd_free(char **line);
    char *trim_white(char *line);
    bool do_builtin(struct shell *sh, char **argv);
    void sh_init(struct shell *sh);
    void sh_destroy(struct shell *sh);
    void parse_args(int argc, char **argv);

#ifdef __cplusplus
} 
#endif

#endif
