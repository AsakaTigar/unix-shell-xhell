#ifndef XHELL_H
#define XHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

// Constants
#define MAX_CMD_LEN 1024
#define MAX_ARGS 64
#define MAX_PATH_LEN 512
#define MAX_HISTORY 1000
#define LOG_FILE ".xhell_log"
#define HISTORY_FILE ".xhell_history"

// Command structure
typedef struct {
    char *args[MAX_ARGS];
    int argc;
    char *input_file;
    char *output_file;
    char *error_file;
    int append_output;
    int append_error;
} Command;

// Pipeline structure
typedef struct {
    Command commands[MAX_ARGS];
    int num_commands;
} Pipeline;

// Global variables
extern char *history[MAX_HISTORY];
extern int history_count;
extern char prev_dir[MAX_PATH_LEN];
extern char current_dir[MAX_PATH_LEN];

// Parser functions
int parse_command_line(char *line, Pipeline *pipeline);
void free_pipeline(Pipeline *pipeline);

// Built-in command functions
int cmd_xpwd(int argc, char **argv);
int cmd_xcd(int argc, char **argv);
int cmd_xls(int argc, char **argv);
int cmd_xtouch(int argc, char **argv);
int cmd_xecho(int argc, char **argv);
int cmd_xcat(int argc, char **argv);
int cmd_xcp(int argc, char **argv);
int cmd_xrm(int argc, char **argv);
int cmd_xmv(int argc, char **argv);
int cmd_xhistory(int argc, char **argv);
int cmd_xtee(int argc, char **argv);
int cmd_xjournalctl(int argc, char **argv);
int cmd_xsysinfo(int argc, char **argv);
int cmd_xhelp(int argc, char **argv);
int cmd_xcalc(int argc, char **argv);
int cmd_xsh(int argc, char **argv);
int cmd_xsearch(int argc, char **argv);
int cmd_quit(int argc, char **argv);

int is_builtin_command(const char *cmd);
int execute_builtin(Command *cmd);

// External program execution
int execute_external(Command *cmd);
char *find_in_path(const char *program);

// Redirection functions
int setup_redirections(Command *cmd);
void restore_redirections(int saved_stdout, int saved_stderr, int saved_stdin);

// Pipe functions
int execute_pipeline(Pipeline *pipeline);

// Logger functions
void log_command(const char *command, int status);
void log_error(const char *command, const char *error);
void init_logger(void);

// Utility functions
void trim_whitespace(char *str);
char *get_prompt(void);
void add_to_history(const char *command);
void save_history(void);
void load_history(void);
int copy_file(const char *src, const char *dst);
int copy_directory(const char *src, const char *dst);
int remove_directory(const char *path);

#endif // XHELL_H
