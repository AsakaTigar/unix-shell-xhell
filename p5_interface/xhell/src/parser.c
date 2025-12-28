#include "../include/xhell.h"

// Parse a single command (handles redirections)
static int parse_single_command(char *cmd_str, Command *cmd) {
    cmd->argc = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->error_file = NULL;
    cmd->append_output = 0;
    cmd->append_error = 0;
    
    char *token;
    char *saveptr;
    int i = 0;
    
    // Tokenize by whitespace
    token = strtok_r(cmd_str, " \t", &saveptr);
    while (token != NULL && i < MAX_ARGS - 1) {
        // Check for output redirection
        if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token == NULL) {
                fprintf(stderr, "Error: missing filename after >\n");
                return -1;
            }
            cmd->output_file = strdup(token);
            cmd->append_output = 0;
        }
        // Check for append redirection
        else if (strcmp(token, ">>") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token == NULL) {
                fprintf(stderr, "Error: missing filename after >>\n");
                return -1;
            }
            cmd->output_file = strdup(token);
            cmd->append_output = 1;
        }
        // Check for error redirection
        else if (strcmp(token, "2>") == 0) {
            token = strtok_r(NULL, " \t", &saveptr);
            if (token == NULL) {
                fprintf(stderr, "Error: missing filename after 2>\n");
                return -1;
            }
            cmd->error_file = strdup(token);
            cmd->append_error = 0;
        }
        // Regular argument
        else {
            cmd->args[i++] = strdup(token);
        }
        
        token = strtok_r(NULL, " \t", &saveptr);
    }
    
    cmd->args[i] = NULL;
    cmd->argc = i;
    
    return 0;
}

// Parse command line (handles pipes)
int parse_command_line(char *line, Pipeline *pipeline) {
    pipeline->num_commands = 0;
    
    char *line_copy = strdup(line);
    char *cmd_str;
    char *saveptr;
    
    // Split by pipe
    cmd_str = strtok_r(line_copy, "|", &saveptr);
    while (cmd_str != NULL && pipeline->num_commands < MAX_ARGS) {
        trim_whitespace(cmd_str);
        
        if (parse_single_command(cmd_str, &pipeline->commands[pipeline->num_commands]) != 0) {
            free(line_copy);
            return -1;
        }
        
        pipeline->num_commands++;
        cmd_str = strtok_r(NULL, "|", &saveptr);
    }
    
    free(line_copy);
    
    if (pipeline->num_commands == 0) {
        return -1;
    }
    
    return 0;
}

// Free pipeline resources
void free_pipeline(Pipeline *pipeline) {
    for (int i = 0; i < pipeline->num_commands; i++) {
        Command *cmd = &pipeline->commands[i];
        
        for (int j = 0; j < cmd->argc; j++) {
            free(cmd->args[j]);
        }
        
        if (cmd->input_file) free(cmd->input_file);
        if (cmd->output_file) free(cmd->output_file);
        if (cmd->error_file) free(cmd->error_file);
    }
}
