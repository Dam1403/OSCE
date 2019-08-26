#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/sched/task.h>


void print_task(struct task_struct *task)
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
	printk(KERN_INFO "PID: %d EXE_NAME: %s EXE_STATE: %s",pid,exe_name,exe_state);	
}


void dfs(struct list_head *list, struct task_struct* task, int level)
{

	list_for_each(list, &task->children)
	{
		task = list_entry(list,struct task_struct,sibling);
		print_task(task);
		dfs(list,task,level + 1);
	}


}
int simple_init(void){
	printk(KERN_INFO "Loading Module\n");
	

	struct list_head *list = NULL;
	

	dfs(list,&init_task,0);

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


