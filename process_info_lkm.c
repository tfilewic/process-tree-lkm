/**
 * process_info_lkm.c
 *
 * A Loadable Kernel Module (LKM) for the Linux kernel that displays details 
 * for all the processes whose PID is greater than a user-supplied value (inp_pid).
 *
 * @author  tfilewic
 * @version 2024-11-07	
 * 
 * To compile, run makefile by entering "make"
 *
 */


#include <linux/kernel.h>	// printk,  KERN_INFO
#include <linux/sched/signal.h> // for_each_process
#include <linux/sched.h> 	// task_struct
#include <linux/list.h>		// list_for_each_entry, list_empty
#include <linux/module.h>	// everything else


/* Module metadata */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("P5 -  LKM for the Linux kernel to display process details");
MODULE_AUTHOR("Tyler Filewich");
MODULE_VERSION("2024-11-7");

/* Function declarations */
static int fire_it_up(void); 
static void shut_it_down(void);
static void print_header(void);
static void print_info(struct task_struct* task);
static void print_parent(struct task_struct* task);
static void print_children(struct task_struct* task);

/* Macros for registering module entry and exit points. */
module_init(fire_it_up);		
module_exit(shut_it_down);		

/* Define module parameter */
static int inp_pid = 0;
module_param(inp_pid, int, 0444);  //read only for everyone 

/* This function is called when the module is loaded. */
int fire_it_up(void) {
	struct task_struct *task;		//pointer for traversal 
    
	printk(KERN_INFO "Loading Module\n");	//loading message
	
	for_each_process(task) {
		
		if (task->pid > inp_pid) {
			printk(KERN_INFO "\n");
			print_header();
			print_info(task);
			print_children(task);
			print_parent(task);
		}
	}

       return 0;
}

/* This function is called when the module is removed. */
void shut_it_down(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Prints the header to display processes */
static void print_header(void) {
	printk(KERN_INFO "%-16s %-8s %-8s %-8s %-8s %-8s\n",
		"PROCESS", "PID", "STATE", "PRIO", "ST_PRIO", "NORM_PRIO");
}

/* Prints the specified info for a task */
static void print_info(struct task_struct* task) {
	if (!task ||  task->pid <= 0) return;
	printk(KERN_INFO "%-16s %-8d %-8d %-8d %-8d %-8d\n",
		task->comm, task->pid, task->__state, task->prio, task->static_prio, task->normal_prio);
}

/* Prints the specified info for the parent of a task */
static void print_parent(struct task_struct* task) {
	if (!task || !task->parent) return; 	//null check
	printk(KERN_INFO "PARENT\n");		//print parent header
	print_info(task->parent);		//print parent info
}

/* Prints the specified info for the children of a task */
static void print_children(struct task_struct* task){
	if (!task || list_empty(&task->children)) return; 	// null/empty check
	printk(KERN_INFO "CHILDREN\n");				//print children header

	struct task_struct *child;	// traversal pointer
	list_for_each_entry(child, &task->children, sibling) {	//iterate children
		print_info(child);	// print child info
	}
}

