#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

typedef struct {
    unsigned long total_mem;
    unsigned long free_mem;
    unsigned long cached_mem;
    unsigned long used_mem;
    unsigned long cpu_usage;
    unsigned long cpu_idle;
    
    struct {
        int pid;
        char name[256];
    } processes[1000];
    int process_count;
} SystemStats;


void format_memory(unsigned long kb, char *buffer) {
    if (kb > 1024*1024) {
        sprintf(buffer, "%.2f GB", (float)kb / (1024*1024));
    } else if (kb > 1024) {
        sprintf(buffer, "%.2f MB", (float)kb / 1024);
    } else {
        sprintf(buffer, "%lu KB", kb);
    }
}

void draw_progress_bar(WINDOW *win, int y, int x, int width, int percentage) {
    int fill_width = (percentage * width) / 100;
    mvwprintw(win, y, x, "[");
    
    for (int i = 0; i < width; i++) {
        if (i < fill_width) {
            wattron(win, COLOR_PAIR(1));
            wprintw(win, "|");
            wattroff(win, COLOR_PAIR(1));
        } else {
            wattron(win, COLOR_PAIR(2));
            wprintw(win, " ");
            wattroff(win, COLOR_PAIR(2));
        }
    }
    wprintw(win, "] %d%%", percentage);
}


void parse_stats(const char *content, SystemStats *stats) {
    char *line = strdup(content);
    char *saveptr;
    char *token = strtok_r(line, "\n", &saveptr);
    
    stats->process_count = 0;
    
    while (token != NULL) {
        if (strstr(token, "Total Memory:") != NULL) {
            sscanf(token, "Total Memory: %lu", &stats->total_mem);
        } else if (strstr(token, "Free Memory:") != NULL) {
            sscanf(token, "Free Memory: %lu", &stats->free_mem);
        } else if (strstr(token, "Cached Memory:") != NULL) {
            sscanf(token, "Cached Memory: %lu", &stats->cached_mem);
        } else if (strstr(token, "CPU Usage:") != NULL) {
            sscanf(token, "CPU Usage: %lu", &stats->cpu_usage);
        } else if (strstr(token, "CPU Idle:") != NULL) {
            sscanf(token, "CPU Idle: %lu", &stats->cpu_idle);
        } else if (strstr(token, "PID") == NULL && strlen(token) > 1) {
            // Parse PID and name separately
            sscanf(token, "%d\t%[^\n]", &stats->processes[stats->process_count].pid,
                   stats->processes[stats->process_count].name);
            stats->process_count++;
            if (stats->process_count >= 1000) break;
        }
        token = strtok_r(NULL, "\n", &saveptr);
    }
    
    free(line);
    stats->used_mem = stats->total_mem - stats->free_mem - stats->cached_mem;
}

int main() {
    /* Initializing ncurses */
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);  // Enable keyboard input
    timeout(20);          // Set non-blocking input with 20ms timeout
    
    // Initialize color pairs
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    
    /* Create a window*/
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    WINDOW *header_win = newwin(3, max_x, 0, 0);
    WINDOW *memory_win = newwin(6, max_x/2, 3, 0);
    WINDOW *cpu_win = newwin(6, max_x/2, 3, max_x/2);
    WINDOW *process_win = newwin(max_y-9, max_x, 9, 0);
    
    /* scroll position */
    int scroll_position = 0;
    
    while (1) {
        /* Handle keyboard user input*/
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (scroll_position > 0) scroll_position--;
                break;
            case KEY_DOWN:
                scroll_position++;  // We'll check the maximum later
                break;
            case 'q':  // Add quit functionality
                goto cleanup;
            default:
                break;
        }
        
        FILE *fp = fopen("/proc/system_stats", "r");
        if (!fp) {
            endwin();
            fprintf(stderr, "Error: Cannot open /proc/system_stats\n");
            fprintf(stderr, "Please make sure the kernel module is loaded.\n");
            return 1;
        }
        
        char content[10000];
        size_t bytes_read = fread(content, 1, sizeof(content)-1, fp);
        content[bytes_read] = '\0';
        fclose(fp);
        
        SystemStats stats;
        parse_stats(content, &stats);
        
        /*Limit scroll position based on actual processes count*/
        int max_display = max_y - 13;       
        int max_scroll = (stats.process_count > max_display) ? 
                        stats.process_count - max_display : 0;
        if (scroll_position > max_scroll) scroll_position = max_scroll;
        
        // Update header
        wclear(header_win);
        box(header_win, 0, 0);
        wattron(header_win, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(header_win, 1, (max_x-20)/2, "System Statistics Monitor");
        wattroff(header_win, COLOR_PAIR(3) | A_BOLD);
        
        // Update memory window
        wclear(memory_win);
        box(memory_win, 0, 0);
        wattron(memory_win, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(memory_win, 1, 2, "Memory Usage");
        wattroff(memory_win, COLOR_PAIR(3) | A_BOLD);
        
        char mem_str[64];
        format_memory(stats.used_mem, mem_str);
        mvwprintw(memory_win, 2, 2, "Used: %s", mem_str);
        format_memory(stats.free_mem, mem_str);
        mvwprintw(memory_win, 3, 2, "Free: %s", mem_str);
        format_memory(stats.cached_mem, mem_str);
        mvwprintw(memory_win, 4, 2, "Cached: %s", mem_str);
        
        // Update CPU window
        wclear(cpu_win);
        box(cpu_win, 0, 0);
        wattron(cpu_win, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(cpu_win, 1, 2, "CPU Usage");
        wattroff(cpu_win, COLOR_PAIR(3) | A_BOLD);
        
        draw_progress_bar(cpu_win, 3, 2, 30, stats.cpu_usage);
        
        // Update process window
        wclear(process_win);
        box(process_win, 0, 0);
        wattron(process_win, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(process_win, 1, 2, "Running Processes (Total: %d)", stats.process_count);
        wattroff(process_win, COLOR_PAIR(3) | A_BOLD);
        
        // Draw column headers
        wattron(process_win, A_BOLD);
        mvwprintw(process_win, 2, 2, "%-10s %-50s", "PID", "PROCESS NAME");
        wattroff(process_win, A_BOLD);
        mvwhline(process_win, 3, 1, ACS_HLINE, max_x-2);  // Separator line
        
        // Display processes with scrolling
        int display_count = (stats.process_count < max_display) ? 
                           stats.process_count : max_display;
        
        for (int i = 0; i < display_count; i++) {
            int idx = i + scroll_position;
            if (idx < stats.process_count) {
                mvwprintw(process_win, i+4, 2, "%-10d %-50s",
                         stats.processes[idx].pid,
                         stats.processes[idx].name);
            }
        }
        
        // scroll indicators 
        if (scroll_position > 0) {
            mvwprintw(process_win, 4, max_x-3, "↑");
        }
        if (scroll_position < max_scroll) {
            mvwprintw(process_win, max_y-11, max_x-3, "↓");
        }
        
        // Refresh all windows
        wrefresh(header_win);
        wrefresh(memory_win);
        wrefresh(cpu_win);
        wrefresh(process_win);
        
        usleep(5000);  // Sleep time
    }
    
cleanup:
    // Cleanup
    delwin(header_win);
    delwin(memory_win);
    delwin(cpu_win);
    delwin(process_win);
    endwin();
    
    return 0;
}
