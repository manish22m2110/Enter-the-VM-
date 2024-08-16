// #include <linux/module.h>
// #include <linux/init.h>
// #include <linux/kernel.h>
// #include <linux/sched/signal.h>

// static int __init my_module_init(void)
// {
//    struct task_struct *task;
//    struct mm_struct *mm_;
//    unsigned long base;

//    for_each_process(task)
//    {
//       mm_ = task->mm;
//       if (mm_ == NULL)
//       {
//          printk(KERN_INFO "PID: %d is not assigned a memory\n", task->pid);
//       }
//       if (mm_ != NULL)
//          base = task->mm->mmap_base;

//       printk(KERN_INFO "VM START = %lu VM END = %lu\n",base,base + task->mm->task_size);
//    }
//    return 0;
// }

// static void __exit my_module_exit(void)
// {
//    printk(KERN_INFO "Module unloaded\n");
// }

// module_init(my_module_init);
// module_exit(my_module_exit);

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Manish");
// MODULE_DESCRIPTION("Input a target_pid and output the child pid and process state");
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

static int __init my_module_init(void)
{
    struct task_struct *task;
    struct mm_struct *mm;

    printk(KERN_INFO "Loading my_module\n");

    for_each_process(task)
    {
        mm = get_task_mm(task);
        if (mm == NULL)
        {
            printk(KERN_INFO "PID: %d is not assigned a memory\n", task->pid);
            continue;
        }
        else
        {
            printk(KERN_INFO "PID: %d, base address : %lu, end address : %lu\n",task->pid,mm->mmap_base,mm->mmap_base + mm->task_size);
        }
    }

    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Unloading my_module\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Iterate through memory mappings of all processes");
