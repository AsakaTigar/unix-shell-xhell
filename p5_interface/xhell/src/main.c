#include "../include/xhell.h"

// Global variables
char *history[MAX_HISTORY];
int history_count = 0;
char prev_dir[MAX_PATH_LEN] = "";
char current_dir[MAX_PATH_LEN];

int main(int argc, char **argv) {
    char input[MAX_CMD_LEN];
    Pipeline pipeline;
    
    // Initialize
    init_logger();
    load_history();
    
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd");
        return 1;
    }
    
    // Welcome message
    printf("######### Welcome to Xhell! #############\n");
    
    // Main REPL loop
    while (1) {
        // Display prompt
        char *prompt = get_prompt();
        printf("%s", prompt);
        free(prompt);
        
        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Remove newline
        input[strcspn(input, "\n")] = 0;
        
        // Skip empty lines
        trim_whitespace(input);
        if (strlen(input) == 0) {
            continue;
        }
        
        // Add to history
        add_to_history(input);
        
        // Parse command
        if (parse_command_line(input, &pipeline) != 0) {
            log_error(input, "Parse error");
            continue;
        }
        
        // Execute pipeline
        int status = execute_pipeline(&pipeline);
        
        // Log command
        log_command(input, status);
        
        // Free pipeline
        free_pipeline(&pipeline);
    }
    
    // Cleanup
    printf("######### Quiting Xhell #############\n");
    save_history();
    
    return 0;
}
