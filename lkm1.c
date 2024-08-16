#include <linux/module.h> //Needed by all modules
#include <linux/init.h>
#include <linux/kernel.h>       //KERN_INFO
#include <linux/sched/signal.h> //for_aech_process

MODULE_LICENSE("GPL");


static int lkm1_init(void)
{
    struct task_struct *task;

    pr_info("Listing all processes:\n");
    for_each_process(task)
    {
        pr_info("PID: %d\n", task->pid);
    }

    return 0;
}

static void lkm1_exit(void)
{
    //pr_info("Exiting lkm1 module\n");
    printk("Exiting this module\n");
}

module_init(lkm1_init);
module_exit(lkm1_exit);