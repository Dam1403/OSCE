#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {
	struct list_head list;
	int day;
	int month; 
	int year;
};

static LIST_HEAD(birthday_list);

struct birthday* new_birthday(int day,int month, int year){
	struct birthday* newborn = kmalloc(sizeof(*newborn), GFP_KERNEL);
	newborn->day = day;
	newborn->month = month;
	newborn->year = year;	
	INIT_LIST_HEAD(&newborn->list);
	return newborn;
}

int simple_init(void){
	printk(KERN_INFO "Loading Module\n");
	struct birthday* curr_birth;
	int i;
	for(i = 1; i < 6; i++){
		curr_birth = new_birthday(i*5,i*2,i+1990);
		list_add_tail(&curr_birth->list,&birthday_list);
	}

	list_for_each_entry(curr_birth,&birthday_list,list){
		printk(KERN_INFO "Birthday: %d/%d/%d",
				curr_birth->month,
				curr_birth->day,
				curr_birth->year);
	}
	return 0;
}

void simple_exit(void){
	printk(KERN_INFO "Removing Module\n");
	struct birthday *curr_birth,*next_birth;
	list_for_each_entry_safe(curr_birth,next_birth,&birthday_list,list){
		printk(KERN_INFO "Removing Birthday: %d/%d/%d",
				curr_birth->month,
				curr_birth->day,
				curr_birth->year);
		list_del(&curr_birth->list);
		kfree(curr_birth);
	}
}



module_init(simple_init);
module_exit(simple_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("Some Guy");


