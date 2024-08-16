#include <linux/module.h> //Needed by all modules
#include <linux/init.h>
#include <linux/kernel.h>       //KERN_INFO
#include <linux/sched/signal.h> //for_each_process, task_struct
#include <linux/mm.h>           //for mm_struct
#include <linux/pgtable.h>
#include <linux/pid.h>
#include <linux/mm_types.h>
#include <linux/io.h>
#include <linux/xa.h>  // Assuming XA_ZERO_ENTRY is defined here

#define MT_FOR_EACH(__tree, __entry, __index, __max) \
    for ((__index) = (__index); (__index) < (__max); (__index)++) \
        if (((__entry) = mt_entry((__tree), (__index)))) \
            if ((__entry) != XA_ZERO_ENTRY)  // Skip entries that resolve to NULL


static unsigned long target_va = -1;
static int target_pid = -1;
module_param(target_pid, int, S_IRUSR);
MODULE_PARM_DESC(target_pid, "Target process ID");

static void print_node_data(struct maple_node *node)
{
    switch (node->enode.type) {
        case MAPLE_TYPE_PNODE:
            printk(KERN_INFO "PNODE: parent=%p, slot[0]=%p\n", node->pnode.parent, node->pnode.slot[0]);
            break;

        case MAPLE_TYPE_ENODE:
            printk(KERN_INFO "ENODE: pad=%p, piv_parent=%p, parent_slot=%hhu\n",
                   node->enode.pad, node->enode.piv_parent, node->enode.parent_slot);
            break;

        case MAPLE_TYPE_MR64:
            printk(KERN_INFO "MR64: field1=%p, field2=%p\n", node->mr64.field1, node->mr64.field2);
            break;

        case MAPLE_TYPE_MA64:
            printk(KERN_INFO "MA64: field1=%p, field2=%p\n", node->ma64.field1, node->ma64.field2);
            break;

        case MAPLE_TYPE_ALLOC:
            printk(KERN_INFO "ALLOC: field1=%p, field2=%p\n", node->alloc.field1, node->alloc.field2);
            break;

        default:
            printk(KERN_INFO "Unknown node type\n");
            break;
    }
}


static int __init my_module_init(void)
{
    struct task_struct *task;
    struct mm_struct *mm;
    struct maple_tree mt;

    task = pid_task(find_get_pid(target_pid), PIDTYPE_PID);
    mm = task->mm;
    mt = mm->mm_mt;

    MT_FOR_EACH(&mt, entry, index, mt_max_index(&mt))
    {
        // Print data for each node during traversal
        print_node_data(entry);
    }
    

    return 0;
}

static void __exit my_module_exit(void)
{
    printk(KERN_INFO "Size of allocated VA and PA unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Manish");
MODULE_DESCRIPTION("Kernel module with PID parameter");