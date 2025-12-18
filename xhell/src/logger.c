#include "../include/xhell.h"

static FILE *log_file = NULL;

// Initialize logger
void init_logger(void) {
    log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
        perror("init_logger");
    }
}

// Get current timestamp
static char *get_timestamp(void) {
    static char buffer[64];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    return buffer;
}

// Log command execution
void log_command(const char *command, int status) {
    if (log_file == NULL) {
        return;
    }
    
    fprintf(log_file, "[%s] CMD: %s (status: %d)\n", get_timestamp(), command, status);
    fflush(log_file);
}

// Log error
void log_error(const char *command, const char *error) {
    if (log_file == NULL) {
        return;
    }
    
    fprintf(log_file, "[%s] ERROR: %s - %s\n", get_timestamp(), command, error);
    fflush(log_file);
}
