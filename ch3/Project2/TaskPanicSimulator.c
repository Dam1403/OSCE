#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>





int simple_init(void){
	printk(KERN_INFO "Loading Module\n");
	
	struct task_struct *task;
	for_each_process(task)
	{
		int pid = task->pid;
		char* exe_name = task->comm;	
		char* exe_state;
		int state = task->state;
		if(state > 0)
		{
			exe_state = "STOPPED";
		}
		else if (state == 0)
		{
			exe_state = "RUNNABLE";
		}
		else
		{
			exe_state = "UNRUNNABLE";
		}
		printk(KERN_INFO "PID: %d EXE_NAME: %s EXE_STATE: %s");	
	}
	return 0;
}

void simple_exit(void){
	printk(KERN_INFO "Removing Module\n");

}



module_init(simple_init);
module_exit(simple_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("Some Guy");


