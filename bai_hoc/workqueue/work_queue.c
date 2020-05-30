#include<linux/module.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>
#include<linux/delay.h>

static void work_handler(struct work_struct *work);
static DECLARE_WORK(my_work,work_handler);
static void work_handler(struct work_struct *work)
{
	msleep(1000);
	pr_info("this is work function !!! \n");
	
}

static irqreturn_t irq_handler(int irq, void *dev_id)
{
	schedule_work(&my_work);
	pr_info("this is interrupt handler !! \n");
	pr_info("the interrupt handler is done !! \n");
	return IRQ_HANDLED;
}


static int __init my_work_init(void)
{
	request_irq(1,irq_handler,IRQF_SHARED,"my_isr",(void*)irq_handler);
	return 0;
}

static void __exit my_work_exit(void)
{
	free_irq(1,(void*)irq_handler);
	cancel_work_sync(&my_work);	
}

module_init(my_work_init);
module_exit(my_work_exit);
MODULE_LICENSE("GPL");



