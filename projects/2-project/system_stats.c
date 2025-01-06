#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/cpumask.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/time_namespace.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("System Statistics Module");

static struct proc_dir_entry *stats_entry;

// Structure to store CPU statistics
struct cpu_stats {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
    unsigned long long steal;
    unsigned long long guest;
    unsigned long long guest_nice;
};

static struct cpu_stats last_cpu_stats;
static bool first_reading = true;

// Helper function to get memory statistics
static void get_memory_stats(unsigned long *total, unsigned long *free, unsigned long *cached)
{
    struct sysinfo si;
    si_meminfo(&si);
    
    *total = si.totalram << (PAGE_SHIFT - 10);  // Convert to KB
    *free = si.freeram << (PAGE_SHIFT - 10);
    *cached = global_node_page_state(NR_FILE_PAGES) << (PAGE_SHIFT - 10);
}

// Helper function to read and parse CPU statistics from /proc/stat
static void get_cpu_stats(struct cpu_stats *stats)
{
    struct file *fp;
    char buf[256];
    char *ptr;
    ssize_t bytes_read;
    loff_t pos = 0;

    // Initialize stats to 0
    memset(stats, 0, sizeof(struct cpu_stats));

    // Open /proc/stat
    fp = filp_open("/proc/stat", O_RDONLY, 0);
    if (IS_ERR(fp)) {
        printk(KERN_ERR "Failed to open /proc/stat\n");
        return;
    }

    // Read the first line (CPU total statistics)
    bytes_read = kernel_read(fp, buf, sizeof(buf) - 1, &pos);
    if (bytes_read > 0) {
        buf[bytes_read] = '\0';
        
        // Skip "cpu " prefix
        ptr = buf + 4;
        
        // Parse CPU statistics
        sscanf(ptr, "%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
               &stats->user, &stats->nice, &stats->system, &stats->idle,
               &stats->iowait, &stats->irq, &stats->softirq, &stats->steal,
               &stats->guest, &stats->guest_nice);
    }

    filp_close(fp, NULL);
}

// Calculate CPU usage percentage
static void calculate_cpu_usage(struct cpu_stats *prev, struct cpu_stats *curr,
                              unsigned long *cpu_use, unsigned long *cpu_idle)
{
    unsigned long long total_time_prev, total_time_curr;
    unsigned long long idle_prev, idle_curr;
    unsigned long long active_time;
    unsigned long long total_time;
    
    // Calculate total time and idle time for previous reading
    idle_prev = prev->idle + prev->iowait;
    total_time_prev = idle_prev + prev->user + prev->nice + prev->system +
                      prev->irq + prev->softirq + prev->steal;
    
    // Calculate total time and idle time for current reading
    idle_curr = curr->idle + curr->iowait;
    total_time_curr = idle_curr + curr->user + curr->nice + curr->system +
                      curr->irq + curr->softirq + curr->steal;
    
    // Calculate the difference
    active_time = (total_time_curr - total_time_prev) - (idle_curr - idle_prev);
    total_time = total_time_curr - total_time_prev;
    
    // Calculate percentage
    if (total_time > 0) {
        *cpu_use = (active_time * 100) / total_time;
        *cpu_idle = 100 - *cpu_use;
    } else {
        *cpu_use = 0;
        *cpu_idle = 100;
    }
}

// Callback function for reading the proc file
static int stats_show(struct seq_file *m, void *v)
{
    unsigned long total_mem, free_mem, cached_mem;
    unsigned long cpu_use = 0, cpu_idle = 0;
    struct cpu_stats curr_cpu_stats;
    struct task_struct *task;
    
    // Get memory statistics
    get_memory_stats(&total_mem, &free_mem, &cached_mem);
    
    // Get current CPU statistics
    get_cpu_stats(&curr_cpu_stats);
    
    // Calculate CPU usage if we have previous readings
    if (!first_reading) {
        calculate_cpu_usage(&last_cpu_stats, &curr_cpu_stats, &cpu_use, &cpu_idle);
    }
    
    // Store current reading for next time
    memcpy(&last_cpu_stats, &curr_cpu_stats, sizeof(struct cpu_stats));
    first_reading = false;
    
    // Print memory information
    seq_printf(m, "Memory Statistics:\n");
    seq_printf(m, "Total Memory: %lu KB\n", total_mem);
    seq_printf(m, "Free Memory: %lu KB\n", free_mem);
    seq_printf(m, "Cached Memory: %lu KB\n\n", cached_mem);
    
    // Print CPU information
    seq_printf(m, "CPU Statistics:\n");
    seq_printf(m, "CPU Usage: %lu%%\n", cpu_use);
    seq_printf(m, "CPU Idle: %lu%%\n\n", cpu_idle);
    
    // Print process information
    seq_printf(m, "Process List:\n");
    seq_printf(m, "PID\tCOMMAND\n");
    
    for_each_process(task) {
        seq_printf(m, "%d\t%s\n", task->pid, task->comm);
    }
    
    return 0;
}

static int stats_open(struct inode *inode, struct file *file)
{
    return single_open(file, stats_show, NULL);
}

static const struct proc_ops stats_fops = {
    .proc_open = stats_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init stats_init(void)
{
    stats_entry = proc_create("system_stats", 0444, NULL, &stats_fops);
    if (!stats_entry) {
        return -ENOMEM;
    }
    first_reading = true;
    printk(KERN_INFO "System Statistics Module loaded\n");
    return 0;
}

static void __exit stats_exit(void)
{
    proc_remove(stats_entry);
    printk(KERN_INFO "System Statistics Module unloaded\n");
}

module_init(stats_init);
module_exit(stats_exit);
