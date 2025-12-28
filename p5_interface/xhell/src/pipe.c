#include "../include/xhell.h"

// Execute a single command
static int execute_single_command(Command *cmd, int input_fd, int output_fd) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    
    if (pid == 0) {
        // Child process
        
        // Setup input redirection
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        
        // Setup output redirection
        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }
        
        // Setup file redirections
        setup_redirections(cmd);
        
        // Execute command
        if (is_builtin_command(cmd->args[0])) {
            int status = execute_builtin(cmd);
            exit(status);
        } else {
            char *program_path = find_in_path(cmd->args[0]);
            if (program_path == NULL) {
                fprintf(stderr, "%s: command not found\n", cmd->args[0]);
                exit(127);
            }
            execv(program_path, cmd->args);
            perror("execv");
            exit(1);
        }
    }
    
    return pid;
}

// Execute pipeline
// Execute pipeline
int execute_pipeline(Pipeline *pipeline) {
    if (pipeline->num_commands == 0) {
        return -1;
    }
    
    // Single command (no pipe)
    if (pipeline->num_commands == 1) {
        Command *cmd = &pipeline->commands[0];
        
        // Save original file descriptors
        int saved_stdout = dup(STDOUT_FILENO);
        int saved_stderr = dup(STDERR_FILENO);
        int saved_stdin = dup(STDIN_FILENO);
        
        // Setup redirections
        if (setup_redirections(cmd) != 0) {
            restore_redirections(saved_stdout, saved_stderr, saved_stdin);
            return -1;
        }
        
        int status;
        
        // Execute built-in or external
        if (is_builtin_command(cmd->args[0])) {
            status = execute_builtin(cmd);
            // CRITICAL: Flush stdout to ensure data is written to the file
            // before we restore the file descriptors!
            fflush(stdout);
        } else {
            status = execute_external(cmd);
        }
        
        // Restore file descriptors
        restore_redirections(saved_stdout, saved_stderr, saved_stdin);
        
        return status;
    }
    
    // Multiple commands (pipeline)
    int num_cmds = pipeline->num_commands;
    int pipes[num_cmds - 1][2];
    pid_t pids[num_cmds];
    
    // Create all pipes
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            // Cleanup already created pipes
            for (int j = 0; j < i; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return -1;
        }
    }
    
    // CRITICAL FIX: Flush stdout before forking to prevent buffer duplication
    // This fixes the bug where 'wc -w' counts prompt characters inherited from parent
    fflush(stdout);
    
    // Execute each command
    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();
        
        if (pids[i] == -1) {
            perror("fork");
            // Should probably kill existing children and cleanup, 
            // but for simplicity just break/return error handling here is complex
            return -1;
        }
        
        if (pids[i] == 0) {
            // Child process
            
            // 1. Setup Input: Pipe from previous command (if not first)
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }
            
            // 2. Setup Output: Pipe to next command (if not last)
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            
            // 3. Close ALL pipe fds - CRITICAL for preventing deadlocks
            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            
            // 4. Setup Redirections (Handle < > >> for this specific command)
            // Note: This might override the pipe STDIN/STDOUT if user mixed pipe & redirect
            // e.g. "ls | cat > file". cat has pipe IN, but file OUT.
            setup_redirections(&pipeline->commands[i]);
            
            // 5. Execute Command
            Command *cmd = &pipeline->commands[i];
            if (is_builtin_command(cmd->args[0])) {
                int status = execute_builtin(cmd);
                fflush(stdout); // Flush just in case
                exit(status);
            } else {
                char *program_path = find_in_path(cmd->args[0]);
                if (program_path == NULL) {
                    fprintf(stderr, "%s: command not found\n", cmd->args[0]);
                    exit(127);
                }
                execv(program_path, cmd->args);
                perror("execv");
                exit(1);
            }
        }
    }
    
    // Parent Process: Close all pipe FDs immediately
    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
    
    // Wait for all children
    int status = 0;
    for (int i = 0; i < num_cmds; i++) {
        int child_status;
        waitpid(pids[i], &child_status, 0);
        if (i == num_cmds - 1) {
            status = WIFEXITED(child_status) ? WEXITSTATUS(child_status) : -1;
        }
    }
    
    return status;
}
