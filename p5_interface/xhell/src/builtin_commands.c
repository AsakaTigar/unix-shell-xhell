#include "../include/xhell.h"

// Check if command is built-in
int is_builtin_command(const char *cmd) {
    const char *builtins[] = {
        "xpwd", "xcd", "xls", "xtouch", "xecho", "xcat",
        "xcp", "xrm", "xmv", "xhistory", "xtee", "xjournalctl", 
        "xsysinfo", "xhelp", "xcalc", "xsh", "xsearch", "quit"
    };
    
    for (int i = 0; i < 18; i++) {
        if (strcmp(cmd, builtins[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Execute built-in command
int execute_builtin(Command *cmd) {
    if (cmd->argc == 0) return -1;
    
    const char *name = cmd->args[0];
    
    if (strcmp(name, "xpwd") == 0) return cmd_xpwd(cmd->argc, cmd->args);
    if (strcmp(name, "xcd") == 0) return cmd_xcd(cmd->argc, cmd->args);
    if (strcmp(name, "xls") == 0) return cmd_xls(cmd->argc, cmd->args);
    if (strcmp(name, "xtouch") == 0) return cmd_xtouch(cmd->argc, cmd->args);
    if (strcmp(name, "xecho") == 0) return cmd_xecho(cmd->argc, cmd->args);
    if (strcmp(name, "xcat") == 0) return cmd_xcat(cmd->argc, cmd->args);
    if (strcmp(name, "xcp") == 0) return cmd_xcp(cmd->argc, cmd->args);
    if (strcmp(name, "xrm") == 0) return cmd_xrm(cmd->argc, cmd->args);
    if (strcmp(name, "xmv") == 0) return cmd_xmv(cmd->argc, cmd->args);
    if (strcmp(name, "xhistory") == 0) return cmd_xhistory(cmd->argc, cmd->args);
    if (strcmp(name, "xtee") == 0) return cmd_xtee(cmd->argc, cmd->args);
    if (strcmp(name, "xjournalctl") == 0) return cmd_xjournalctl(cmd->argc, cmd->args);
    if (strcmp(name, "xsysinfo") == 0) return cmd_xsysinfo(cmd->argc, cmd->args);
    if (strcmp(name, "xhelp") == 0) return cmd_xhelp(cmd->argc, cmd->args);
    if (strcmp(name, "xcalc") == 0) return cmd_xcalc(cmd->argc, cmd->args);
    if (strcmp(name, "xsh") == 0) return cmd_xsh(cmd->argc, cmd->args);
    if (strcmp(name, "xsearch") == 0) return cmd_xsearch(cmd->argc, cmd->args);
    if (strcmp(name, "quit") == 0) return cmd_quit(cmd->argc, cmd->args);
    
    return -1;
}

// --- NEW COMMANDS ---

// xsysinfo - display system information
int cmd_xsysinfo(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("========== Xhell System Info ==========\n");
    
    // CPU Info
    FILE *cpu = fopen("/proc/cpuinfo", "r");
    if (cpu) {
        char line[256];
        int count = 0;
        while (fgets(line, sizeof(line), cpu)) {
            if (strncmp(line, "model name", 10) == 0) {
                printf("CPU Model : %s", strchr(line, ':') + 2);
                count++;
                if (count >= 1) break; // Only show first core
            }
        }
        fclose(cpu);
    }
    
    // Memory Info
    FILE *mem = fopen("/proc/meminfo", "r");
    if (mem) {
        char line[256];
        while (fgets(line, sizeof(line), mem)) {
            if (strncmp(line, "MemTotal", 8) == 0) {
                printf("Memory    : %s", strchr(line, ':') + 2);
            }
            if (strncmp(line, "MemAvailable", 12) == 0) {
                printf("Available : %s", strchr(line, ':') + 2);
            }
        }
        fclose(mem);
    }
    
    // Kernel Version
    FILE *ver = fopen("/proc/version", "r");
    if (ver) {
        char line[256];
        if (fgets(line, sizeof(line), ver)) {
            char *p = strchr(line, '(');
            if (p) *p = '\0'; 
            printf("Kernel    : %s\n", line);
        }
        fclose(ver);
    }
    
    printf("=======================================\n");
    return 0;
}

// xhelp - list commands
int cmd_xhelp(int argc, char **argv) {
    (void)argc; (void)argv;
    printf("Xhell Available Commands:\n");
    printf("  xpwd        - Print working directory\n");
    printf("  xcd [dir]   - Change directory\n");
    printf("  xls [dir]   - List files\n");
    printf("  xtouch file - Create empty file\n");
    printf("  xecho [str] - Print string\n");
    printf("  xcat file   - View file content\n");
    printf("  xcp src dst - Copy file/dir (-r)\n");
    printf("  xrm file    - Remove file/dir (-r)\n");
    printf("  xmv src dst - Move/Rename file\n");
    printf("  xhistory    - View command history\n");
    printf("  xsysinfo    - View system stats\n");
    printf("  xhelp       - Show this help\n");
    printf("  quit        - Exit Xhell\n");
    return 0;
}

// xpwd - print working directory
int cmd_xpwd(int argc, char **argv) {
    (void)argc; (void)argv; 
    char cwd[MAX_PATH_LEN];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        perror("xpwd");
        return -1;
    }
}

// xcd - change directory
int cmd_xcd(int argc, char **argv) {
    char *target_dir;
    // char new_dir[MAX_PATH_LEN]; // Removed unused variable
    
    // Save current directory as previous
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("xcd: getcwd");
        return -1;
    }
    
    if (argc == 1) {
        // No argument - go to home directory
        target_dir = getenv("HOME");
        if (target_dir == NULL) {
            fprintf(stderr, "xcd: HOME not set\n");
            return -1;
        }
    } else if (strcmp(argv[1], "-") == 0) {
        // Go to previous directory
        if (strlen(prev_dir) == 0) {
            fprintf(stderr, "xcd: no previous directory\n");
            return -1;
        }
        target_dir = prev_dir;
    } else {
        target_dir = argv[1];
    }
    
    if (chdir(target_dir) != 0) {
        perror("xcd");
        return -1;
    }
    
    // Update previous directory
    strcpy(prev_dir, current_dir);
    
    // Update current directory
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("xcd: getcwd");
        return -1;
    }
    
    return 0;
}

// xls - list directory contents
int cmd_xls(int argc, char **argv) {
    const char *path = ".";
    int long_format = 0;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            long_format = 1;
        } else if (argv[i][0] != '-') {
            path = argv[i];
        }
    }
    
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("xls");
        return -1;
    }
    
    struct dirent *entry;
    struct stat st;
    char full_path[MAX_PATH_LEN];
    char time_buf[64];
    
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        
        if (stat(full_path, &st) == 0) {
            if (long_format) {
                // Format time
                struct tm *tm = localtime(&st.st_mtime);
                strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", tm);
                
                // Type & Color
                char type = S_ISDIR(st.st_mode) ? 'd' : '-';
                const char *color = "\033[0m"; // Default
                if (S_ISDIR(st.st_mode)) color = "\033[1;34m"; // Blue
                else if (st.st_mode & S_IXUSR) color = "\033[1;32m"; // Green
                
                // Permissions (simplified)
                char perms[4] = "rw-";
                if (st.st_mode & S_IXUSR) perms[2] = 'x';
                
                printf("%c%s %8ld %s %s%s\033[0m", type, perms, st.st_size, time_buf, color, entry->d_name);
                if (S_ISDIR(st.st_mode)) printf("/");
                printf("\n");
            } else {
                if (S_ISDIR(st.st_mode)) {
                    printf("\033[1;34m%s/\033[0m\n", entry->d_name);
                } else if (st.st_mode & S_IXUSR) {
                     printf("\033[1;32m%s*\033[0m\n", entry->d_name);
                } else {
                    printf("%s\n", entry->d_name);
                }
            }
        }
    }
    
    closedir(dir);
    return 0;
}

// xtouch - create file if not exists
int cmd_xtouch(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "xtouch: missing file operand\n");
        return -1;
    }
    
    const char *filename = argv[1];
    
    // Check if file exists
    if (access(filename, F_OK) == 0) {
        // File exists, do nothing
        return 0;
    }
    
    // Create file
    int fd = open(filename, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("xtouch");
        return -1;
    }
    
    close(fd);
    return 0;
}

// xecho - echo string
int cmd_xecho(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

// xcat - display file contents
int cmd_xcat(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "xcat: missing file operand\n");
        return -1;
    }
    
    const char *filename = argv[1];
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        perror("xcat");
        return -1;
    }
    
    char buffer[4096];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytes, stdout);
    }
    
    fclose(file);
    return 0;
}

// xcp - copy files/directories
int cmd_xcp(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "xcp: missing file operand\n");
        return -1;
    }
    
    int recursive = 0;
    int src_idx = 1;
    
    // Check for -r flag
    if (strcmp(argv[1], "-r") == 0) {
        recursive = 1;
        src_idx = 2;
        if (argc < 4) {
            fprintf(stderr, "xcp: missing file operand\n");
            return -1;
        }
    }
    
    const char *src = argv[src_idx];
    const char *dst = argv[src_idx + 1];
    
    struct stat st;
    if (stat(src, &st) != 0) {
        perror("xcp");
        return -1;
    }
    
    if (S_ISDIR(st.st_mode)) {
        if (!recursive) {
            fprintf(stderr, "xcp: %s is a directory (not copied)\n", src);
            return -1;
        }
        return copy_directory(src, dst);
    } else {
        return copy_file(src, dst);
    }
}

// xrm - remove files/directories
int cmd_xrm(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "xrm: missing operand\n");
        return -1;
    }
    
    int recursive = 0;
    int target_idx = 1;
    
    // Check for -r flag
    if (strcmp(argv[1], "-r") == 0) {
        recursive = 1;
        target_idx = 2;
        if (argc < 3) {
            fprintf(stderr, "xrm: missing operand\n");
            return -1;
        }
    }
    
    const char *target = argv[target_idx];
    
    struct stat st;
    if (stat(target, &st) != 0) {
        perror("xrm");
        return -1;
    }
    
    if (S_ISDIR(st.st_mode)) {
        if (!recursive) {
            fprintf(stderr, "xrm: cannot remove '%s': Is a directory\n", target);
            return -1;
        }
        return remove_directory(target);
    } else {
        if (unlink(target) != 0) {
            perror("xrm");
            return -1;
        }
        return 0;
    }
}

// xmv - move files/directories
int cmd_xmv(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "xmv: missing file operand\n");
        return -1;
    }
    
    const char *src = argv[1];
    const char *dst = argv[2];
    
    if (rename(src, dst) != 0) {
        perror("xmv");
        return -1;
    }
    
    return 0;
}

// xhistory - show command history
int cmd_xhistory(int argc, char **argv) {
    (void)argc; (void)argv;
    for (int i = 0; i < history_count; i++) {
        printf("%4d  %s\n", i + 1, history[i]);
    }
    return 0;
}

// xtee - read from stdin, write to stdout and file
int cmd_xtee(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "xtee: missing file operand\n");
        return -1;
    }
    
    const char *filename = argv[1];
    FILE *file = fopen(filename, "w");
    
    if (file == NULL) {
        perror("xtee");
        return -1;
    }
    
    char buffer[4096];
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Write to stdout
        fputs(buffer, stdout);
        // Write to file
        fputs(buffer, file);
    }
    
    fclose(file);
    return 0;
}

// xjournalctl - view xhell logs
int cmd_xjournalctl(int argc, char **argv) {
    (void)argc; (void)argv;
    FILE *file = fopen(LOG_FILE, "r");
    
    if (file == NULL) {
        perror("xjournalctl");
        return -1;
    }
    
    char buffer[4096];
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    
    fclose(file);
    return 0;
}

// quit - exit shell
int cmd_quit(int argc, char **argv) {
    (void)argc; (void)argv;
    save_history();
    printf("######### Quiting Xhell #############\n");
    exit(0);
}

// xcalc - simple calculator
int cmd_xcalc(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: xcalc <num1> <op> <num2>\n");
        printf("Example: xcalc 10 + 20\n");
        return 0;
    }
    
    double n1 = atof(argv[1]);
    char op = argv[2][0];
    double n2 = atof(argv[3]);
    double res = 0;
    
    switch (op) {
        case '+': res = n1 + n2; break;
        case '-': res = n1 - n2; break;
        case '*': res = n1 * n2; break;
        case '/': 
            if (n2 == 0) { printf("Error: Div by zero\n"); return -1; }
            res = n1 / n2; 
            break;
        default:
            printf("Error: Unknown operator '%c'\n", op);
            return -1;
        }
    
    printf("%.2f\n", res);
    return 0;
}

// xsh - execute script file
int cmd_xsh(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: xsh <script.x>\n");
        return 0;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("xsh");
        return -1;
    }
    
    char line[MAX_CMD_LEN];
    Pipeline pipeline;
    
    // Simple line-by-line execution
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Skip comments and empty lines
        if (strlen(line) == 0 || line[0] == '#') continue;
        
        // Echo
        printf("+ %s\n", line);
        
        if (parse_command_line(line, &pipeline) == 0) {
            execute_pipeline(&pipeline);
        }
        fflush(stdout);
    }
    fclose(fp);
    return 0;
}

// xsearch - search string in file (grep-like)
int cmd_xsearch(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        printf("Usage: xsearch <term> [file]\n");
        return -1;
    }
    
    char *term = argv[1];
    FILE *fp;
    
    if (argc == 2) {
        fp = stdin;
    } else {
        fp = fopen(argv[2], "r");
        if (!fp) {
            perror("xsearch");
            return -1;
        }
    }
    
    char line[MAX_CMD_LEN];
    int line_num = 1;
    int found = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        if (strstr(line, term)) {
            // Check if we are in a pipe or file
            // For now, keep ANSI colors. If displaying in web, it might show raw codes but that's acceptable for "raw terminal" emulation.
            printf("\033[1;33m%d\033[0m: %s\n", line_num, line);
            found++;
        }
        line_num++;
    }
    
    if (found == 0) {
        // Only print "No matches" if not in a pipe (to avoid clutter)? 
        // Or just print it. Standard grep is silent on no match.
        // Let's be silent on no match to be cleaner in pipes.
        // printf("No matches found for '%s'\n", term); 
    }
    
    if (fp != stdin) {
        fclose(fp);
    }
    return 0;
}
