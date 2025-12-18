#include "../include/xhell.h"

// Setup redirections for a command
int setup_redirections(Command *cmd) {
    // Redirect output
    if (cmd->output_file != NULL) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append_output ? O_APPEND : O_TRUNC;
        
        int fd = open(cmd->output_file, flags, 0644);
        if (fd == -1) {
            perror("redirection: output");
            return -1;
        }
        
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2: stdout");
            close(fd);
            return -1;
        }
        
        close(fd);
    }
    
    // Redirect error
    if (cmd->error_file != NULL) {
        int flags = O_WRONLY | O_CREAT;
        flags |= cmd->append_error ? O_APPEND : O_TRUNC;
        
        int fd = open(cmd->error_file, flags, 0644);
        if (fd == -1) {
            perror("redirection: error");
            return -1;
        }
        
        if (dup2(fd, STDERR_FILENO) == -1) {
            perror("dup2: stderr");
            close(fd);
            return -1;
        }
        
        close(fd);
    }
    
    return 0;
}

// Restore redirections
void restore_redirections(int saved_stdout, int saved_stderr, int saved_stdin) {
    if (saved_stdout != -1) {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
    
    if (saved_stderr != -1) {
        dup2(saved_stderr, STDERR_FILENO);
        close(saved_stderr);
    }
    
    if (saved_stdin != -1) {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
}
