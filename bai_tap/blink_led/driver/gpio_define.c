#include"gpio_define.h"
#include<linux/module.h>

int pin_mode(unsigned int *gpio_base , unsigned int pin , unsigned int mode)
{
	if(pin < 0 || pin > 40)
	{
		pr_info("the pin number is invalid !!! \n");
		return EINVAL;
	}

	else
	{

		if(mode > 0)
		{
			unsigned int *set_mode = gpio_base + pin/10;
			*set_mode = *set_mode | 1<<(pin%10*3);
		}
			
	}
	
	return 0 ;


}

int pin_set(unsigned int *gpio_base, unsigned int pin ,unsigned int value)
{

	unsigned int *set_value;
	if(pin > 40 || pin < 0)
	{
		pr_info("the pin number is invalid !!! \n");
		return EINVAL;
	}

	else
	{
		if(value)
		{

			if( pin < 32)
			{
				set_value = gpio_base + GPSET0;
				*set_value = *set_value | 1<<pin;
			}

			else
			{
				set_value = gpio_base + GPSET1;
				*set_value = *set_value | 1<<pin;
			}
		}

		else if(value == 0)
		{
			if(pin < 32)
			{
				set_value = gpio_base + GPCLR0;
				*set_value = *set_value | 1<<pin;
			}
			else
			{
				set_value = gpio_base + GPCLR1;
				*set_value = *set_value | 1<<pin;
			}
		}
	}

	
	return 0;

}


unsigned int pin_level(unsigned int *gpio_base, unsigned int pin)
{
	if(pin < 0 || pin > 40)
	{
		pr_info("the pin number is invalid !!! \n");
		return EINVAL;
	}

	else
	{
		unsigned int *level;
		if(pin < 32)
		{
			level = gpio_base + GPLEV0;
			if( (*level & 1<<pin) > 0)
			{
				return 1;
			}
			else
				return 0;
		}

		else
		{
			level = gpio_base + GPLEV1;
			if( (*level & 1<<pin) > 0)
				return 1;
			else
				return 0;
		}
	}
}
