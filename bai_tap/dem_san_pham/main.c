#include<linux/interrupt.h>
#include<linux/gpio.h>
#include<linux/io.h>
#include<linux/jiffies.h>
#include<linux/irq.h>
#include<linux/irqreturn.h>
#include "dem_san_pham.h"


int count =0;
volatile unsigned int *led_on;
volatile unsigned int *led_off;

unsigned int  *gpio_base;
unsigned int irq_num;
int pin_irq = 26;

bool current_state=true;
int prev_state=0;
long old_jiffies=0;

unsigned int led_7_seg[10] = { 0x3F , 0x06 , 0x5B , 0x4F , 0x66 , 0x6D , 0x7D , 0x07 , 0x7F , 0x6F };

static irqreturn_t ISR(unsigned int irq, void *dev_id)
{


	if((jiffies - old_jiffies) < msecs_to_jiffies(300))
	{
		return IRQ_HANDLED;
	}	

	else
	{
			int i;
			count++;
			if(count > 9)
				count = 0;



			led_off  = (unsigned int *)(gpio_base + GPCLR0);
			*led_off = (*led_off & 0xFFFFFE03) | 0x1FE ;
				
			led_on = (unsigned int *)(gpio_base + GPSET0);
			*led_on =  (*led_on & 0xFFFFFE03) |   (0xFFFFFE03 | (led_7_seg[count]<<2));

			pr_info("count value is : %d \n",count);
			for(i = 2 ; i<=8 ; i++)
			{
				if(pin_level(i,gpio_base))
					pr_info("pin %d status : HIGH  \n",i);

				else
					pr_info("pin %d status : LOW \n", i);
			}

			current_state = !current_state;

	}

	
	old_jiffies = jiffies;


	return IRQ_HANDLED;
} 


static int __init my_driver_init(void)
{
	int i,ret;
	gpio_base  = ioremap(BASE_ADDR, 100);
	irq_num = gpio_to_irq(pin_irq);
	irq_set_irq_type(irq_num, IRQ_TYPE_EDGE_RISING);
	ret = request_irq(irq_num, (irq_handler_t)ISR,IRQ_TYPE_LEVEL_HIGH,"my_irq",NULL);


	for(i = 2 ; i<=8 ; i++)
		pin_mode(i,gpio_base,1);


	led_off = (unsigned int *)(gpio_base + GPCLR0);
	*led_off = (*led_off & 0xFFFFFE03) | 0x1FE;

	led_on = (unsigned int *)(gpio_base + GPSET0);
	*led_on =  (*led_on  & 0xFFFFFE03) | (0xFFFFFE03 | led_7_seg[0]<<2);


	return 0;
	
}

static void __exit my_driver_exit(void)
{
	free_irq(irq_num,NULL);
	
	led_off = (unsigned int *)(gpio_base + GPCLR0);
	*led_off = (*led_off & 0xFFFFFE03) | 0x1FE;

	iounmap((void*)gpio_base);
	pr_info("module is unloaded !!! ");
}

module_init(my_driver_init);
module_exit(my_driver_exit);
MODULE_LICENSE("GPL");










