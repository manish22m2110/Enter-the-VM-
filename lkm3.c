#include <linux/module.h> //Needed by all modules
#include <linux/init.h>
#include <linux/kernel.h>       //KERN_INFO
#include <linux/sched/signal.h> //for_each_process, task_struct
#include <linux/mm.h>           //for mm_struct
#include <linux/pgtable.h>
#include <linux/pid.h>
#include <linux/mm_types.h>
#include <linux/io.h>



static unsigned long target_va = -1;
static int target_pid = -1;
module_param(target_pid, int, S_IRUSR);
module_param(target_va, ulong, S_IRUSR);
MODULE_PARM_DESC(target_pid, "Target process ID");
MODULE_PARM_DESC(target_va, "Target VA");

static int __init my_module_init(void)
{
    struct task_struct *task;
    struct page *pg;
    struct mm_struct *mm;
    unsigned long phys;
    unsigned long phys2;
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    
    
    task = pid_task(find_get_pid(target_pid), PIDTYPE_PID);
    phys2  = virt_to_phys(target_va);
    printk(KERN_INFO "Physical Address corresponding to target_va using virt_to_phys is %lu\n",phys2);
    mm = task->mm;
    if (mm == NULL)
    {
        printk(KERN_INFO "PID: %d  | PROCESS: %s \n", task->pid, task->comm);
    }

    pgd = pgd_offset(mm, (unsigned long)target_va);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
    {
        printk(KERN_ERR "lkm3: PGD entry not found\n");
        printk("Control reached till pgd\n");
        return -1;
    }
    else
    {
        printk("pgd entry is valid\n");
    }

    p4d = p4d_offset(pgd, (unsigned long)target_va);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
    {
        printk(KERN_ERR "lkm3: P4D entry not found\n");
        printk("Control reached till p4d\n");
        return -1;
    }
    else
    {
        printk("p4d entry is valid\n");
    }
    
    pud = pud_offset(p4d, (unsigned long)target_va);
    if (pud_none(*pud) || pud_bad(*pud))
    {
        printk(KERN_ERR "lkm3: PUD entry not found\n");
        printk("Control reached till pud\n");
        return -1;
    }
    else
    {
        printk("pud entry is valid\n");
    }

    pmd = pmd_offset(pud, (unsigned long)target_va);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
    {
        printk(KERN_ERR "lkm3: PMD entry not found\n");
        printk("Control reached till pmd\n");
        return -1;
    }
    else
    {
        printk("pmd entry is valid\n");
    }

    pte = pte_offset_map(pmd, (unsigned long)target_va);
    if (pte_none(*pte))
    {
        printk(KERN_ERR "lkm3: PTE not found\n");
        printk("Control reached till pte\n");
        return -1;
    }
    else
    {
        printk(KERN_INFO "VALID PTE ADDRESS = %lu\n", (unsigned long)pte);
        pg = pte_page(*pte);
        phys = page_to_phys(pg);
        printk(KERN_INFO "In the process with PID: %d, The virtual address %lu maps to physical address %lu\n",target_pid, target_va, (unsigned long)phys);
    }
    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Page table walk Module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manish");
MODULE_DESCRIPTION("Kernel module with PID parameter");


// #include <linux/module.h>
// #include <linux/init.h>
// #include <linux/kernel.h>
// #include <linux/sched/signal.h>

// // Module parameter to accept PID
// static int target_pid = -1;
// module_param(target_pid, int, S_IRUSR);
// MODULE_PARM_DESC(target_pid, "Target process ID");

// static int __init my_module_init(void)
// {
//     struct task_struct *task;
//     printk(KERN_INFO "Module loaded. Target PID: %d\n", target_pid);
//     for_each_process(task)
//     {
//         if(task->pid == target_pid)
//         {
//             struct task_struct *child_task;
//             printk(KERN_INFO "Parent Process: %s [PID: %d]\n", task->comm, task->pid);

//             list_for_each_entry(child_task, &task->children, sibling)
//             {
//                 printk(KERN_INFO "Child Process: %s [PID: %d], process state : %u\n", child_task->comm, child_task->pid,child_task->stats);
//             }
//             break; 
//         }
//     }
//     return 0;
// }

// static void __exit my_module_exit(void)
// {
//     printk(KERN_INFO "Module unloaded\n");
// }

// module_init(my_module_init);
// module_exit(my_module_exit);

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Manish");
// MODULE_DESCRIPTION("Input a target_pid and output the child pid and process state");