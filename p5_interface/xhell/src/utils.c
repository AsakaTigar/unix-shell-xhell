#include "../include/xhell.h"

// Trim leading and trailing whitespace
void trim_whitespace(char *str) {
    if (str == NULL) return;
    
    // Trim leading
    char *start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\n' || *start == '\r')) {
        start++;
    }
    
    // Trim trailing
    char *end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    // Write back
    size_t len = end - start + 1;
    memmove(str, start, len);
    str[len] = '\0';
}

// Get command prompt
char *get_prompt(void) {
    // Format: [lad]#
    char *prompt = malloc(32);
    snprintf(prompt, 32, "[lad]# ");
    
    return prompt;
}

// Add command to history
void add_to_history(const char *command) {
    if (history_count < MAX_HISTORY) {
        history[history_count++] = strdup(command);
    } else {
        // Shift history and add new command
        free(history[0]);
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            history[i] = history[i + 1];
        }
        history[MAX_HISTORY - 1] = strdup(command);
    }
}

// Save history to file
void save_history(void) {
    FILE *file = fopen(HISTORY_FILE, "w");
    if (file == NULL) {
        return;
    }
    
    for (int i = 0; i < history_count; i++) {
        fprintf(file, "%s\n", history[i]);
    }
    
    fclose(file);
}

// Load history from file
void load_history(void) {
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return;
    }
    
    char line[MAX_CMD_LEN];
    while (fgets(line, sizeof(line), file) != NULL && history_count < MAX_HISTORY) {
        line[strcspn(line, "\n")] = 0;
        history[history_count++] = strdup(line);
    }
    
    fclose(file);
}

// Copy file
int copy_file(const char *src, const char *dst) {
    FILE *src_file = fopen(src, "rb");
    if (src_file == NULL) {
        perror("copy_file: source");
        return -1;
    }
    
    FILE *dst_file = fopen(dst, "wb");
    if (dst_file == NULL) {
        perror("copy_file: destination");
        fclose(src_file);
        return -1;
    }
    
    char buffer[4096];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
        if (fwrite(buffer, 1, bytes, dst_file) != bytes) {
            perror("copy_file: write");
            fclose(src_file);
            fclose(dst_file);
            return -1;
        }
    }
    
    fclose(src_file);
    fclose(dst_file);
    
    return 0;
}

// Copy directory recursively
int copy_directory(const char *src, const char *dst) {
    // Create destination directory
    if (mkdir(dst, 0755) != 0 && errno != EEXIST) {
        perror("copy_directory: mkdir");
        return -1;
    }
    
    DIR *dir = opendir(src);
    if (dir == NULL) {
        perror("copy_directory: opendir");
        return -1;
    }
    
    struct dirent *entry;
    struct stat st;
    char src_path[MAX_PATH_LEN];
    char dst_path[MAX_PATH_LEN];
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dst_path, sizeof(dst_path), "%s/%s", dst, entry->d_name);
        
        if (stat(src_path, &st) != 0) {
            perror("copy_directory: stat");
            continue;
        }
        
        if (S_ISDIR(st.st_mode)) {
            copy_directory(src_path, dst_path);
        } else {
            copy_file(src_path, dst_path);
        }
    }
    
    closedir(dir);
    return 0;
}

// Remove directory recursively
int remove_directory(const char *path) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("remove_directory: opendir");
        return -1;
    }
    
    struct dirent *entry;
    struct stat st;
    char full_path[MAX_PATH_LEN];
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        if (stat(full_path, &st) != 0) {
            perror("remove_directory: stat");
            continue;
        }
        
        if (S_ISDIR(st.st_mode)) {
            remove_directory(full_path);
        } else {
            unlink(full_path);
        }
    }
    
    closedir(dir);
    
    if (rmdir(path) != 0) {
        perror("remove_directory: rmdir");
        return -1;
    }
    
    return 0;
}
