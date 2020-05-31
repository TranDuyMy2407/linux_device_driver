#include "dem_san_pham.h"

int pin_mode(int pin, int *addr_base,int mode)
{

	if( pin > 40  || pin < 0 )
	{
		pr_info("pin number  is  invalid !!!");
		return EINVAL;
	}

	else
	{	
		int *temp = addr_base + pin/10;

		if(mode > 0)
			*temp = *temp | (1<<(pin%10*3));

		else
			*temp = *temp | (0<<(pin%10*3));
		return 0;
	}
}

int pin_set(int pin, int *addr_base,int level)
{
	if( pin > 40 || pin < 0 )
	{
		pr_info("pin number is invalid !!! ");
		return EINVAL;
	}

	else
	{
		if(pin <= 31)
		{
			if(level)
			{
				int *temp = addr_base + GPSET0;
				*temp = *temp | 1<<pin;
			}
			
			else
			{
				int *temp = addr_base + GPCLR0;
				*temp = *temp | 1<<pin;
			}
		}

		else
		{
			if(level)
			{
				int *temp = addr_base + GPSET1;
				*temp = *temp | 1<<pin;
			}

			else
			{
				int *temp = addr_base + GPCLR1;
				*temp = *temp | 1<<pin;	
			}
		}


	}

	return 0;

}



int pin_level(int pin, int *addr_base)
{
	
	if( pin != (int)pin || (pin > 40) || (pin < 0))
	{
		pr_info("pin number is invalid !!! ");
		return EINVAL;
	}
	

	else
	{
		int *temp;
		if(pin <= 31)
		{
			temp = addr_base + GPLEV0;
			*temp = *temp & (1<<pin);
		}

		else
		{
			temp = addr_base + GPLEV1;
			*temp = *temp & (1<<pin);
		}
		
		if( *temp > 0)
			return 1;
		else
			return 0;
	}
	
}
