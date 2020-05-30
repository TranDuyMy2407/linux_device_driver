#include<linux/module.h>
#include<linux/interrupt.h>
#include<linux/delay.h>

void tasklet_func(unsigned long data);

int count = 0;

DECLARE_TASKLET(task,tasklet_func,0);

void tasklet_func(unsigned long data)
{
	msleep(1000);
	pr_info("this is tasklet function !!! \n");
}

irqreturn_t my_isr(int irq , void *dev_id)
{
	msleep(1);
	tasklet_schedule(&task);
	pr_info("the count value  = %d \n",count);
	pr_info("interrupt function is done !!! \n");
	return IRQ_HANDLED;
	
}

static int  __init task_init(void)
{
	request_irq(1,my_isr,IRQF_SHARED,"my_isr",(void*)my_isr);
	return 0;
}

static void __exit task_exit(void)
{
	tasklet_kill(&task);
	free_irq(1,(void*)my_isr);
	pr_info("the module is unloaded !!! \n");
}

module_init(task_init);
module_exit(task_exit);
MODULE_LICENSE("GPL");


