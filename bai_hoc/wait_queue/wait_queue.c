#include<linux/module.h>
#include<linux/workqueue.h>
#include<linux/delay.h>
#include<linux/time.h>
#include<linux/sched.h>

void work_handler(struct work_struct *work);
int condition = 0;

static DECLARE_WORK(my_work,work_handler);
static DECLARE_WAIT_QUEUE_HEAD(my_wait_queue);

void work_handler(struct work_struct *work)
{
int i = 0;
	while(i<5)
	{
	
		pr_info("waiting condition %d !!!\n",i);
		msleep(500);
		i++;
	}	

	condition = 1;
	wake_up_interruptible(&my_wait_queue);
}

static int __init wait_queue_init(void)
{
	schedule_work(&my_work);
	pr_info("going to sleep %s !! \n",__FUNCTION__);
	wait_event_interruptible(my_wait_queue,condition > 0);
	
	pr_info("woke up !!! \n");
	return 0;
}

static void __exit wait_queue_exit(void)
{
	cancel_work_sync(&my_work);
	
	pr_info("the module is unloaded !!! \n");
}

module_init(wait_queue_init);
module_exit(wait_queue_exit);

MODULE_LICENSE("GPL");



