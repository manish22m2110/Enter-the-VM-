#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

// Module parameter to accept PID
static int target_pid = -1;
module_param(target_pid, int, S_IRUSR);
MODULE_PARM_DESC(target_pid, "Target process ID");

static int __init my_module_init(void)
{
    struct task_struct *task;
    printk(KERN_INFO "Module loaded. Target PID: %d\n", target_pid);
    for_each_process(task)
    {
        if(task->pid == target_pid)
        {
            struct task_struct *child_task;
            printk(KERN_INFO "Parent Process: %s [PID: %d]\n", task->comm, task->pid);

            list_for_each_entry(child_task, &task->children, sibling)
            {
                printk(KERN_INFO "Child Process: %s [PID: %d], process state : %u\n", child_task->comm, child_task->pid,child_task->stats);
            }
            break; 
        }
    }
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manish");
MODULE_DESCRIPTION("Input a target_pid and output the child pid and process state");
