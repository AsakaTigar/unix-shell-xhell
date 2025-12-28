#include "../include/xhell.h"

// Find program in PATH
char *find_in_path(const char *program) {
    // If program contains /, use it directly
    if (strchr(program, '/') != NULL) {
        if (access(program, X_OK) == 0) {
            return strdup(program);
        }
        return NULL;
    }
    
    // Search in PATH
    char *path_env = getenv("PATH");
    if (path_env == NULL) {
        return NULL;
    }
    
    char *path_copy = strdup(path_env);
    char *dir = strtok(path_copy, ":");
    char full_path[MAX_PATH_LEN];
    
    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, program);
        
        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            return strdup(full_path);
        }
        
        dir = strtok(NULL, ":");
    }
    
    free(path_copy);
    return NULL;
}

// Execute external program
int execute_external(Command *cmd) {
    if (cmd->argc == 0) {
        return -1;
    }
    
    // Find program in PATH
    char *program_path = find_in_path(cmd->args[0]);
    if (program_path == NULL) {
        fprintf(stderr, "%s: command not found\n", cmd->args[0]);
        return -1;
    }
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        free(program_path);
        return -1;
    }
    
    if (pid == 0) {
        // Child process
        execv(program_path, cmd->args);
        perror("execv");
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        free(program_path);
        
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return -1;
    }
}
