#include<linux/types.h>
#include<linux/module.h>
#include<linux/errno.h>

#define BASE_ADDR 0x3F200000

#define GPFSEL0 0
#define GPFSEL1 1
#define GPFSEL2 2
#define GPFSEL3 3
#define GPFSEL4 4
#define GPFSEL5 5

#define GPSET0  7
#define GPSET1  8

#define GPCLR0  10
#define GPCLR1  11


#define GPLEV0  13
#define GPLEV1  14


#define GPEDS0  16
#define GPEDS1  17


#define GPREN0  19
#define GPREN1  20

#define GPFEN0  22
#define GPFEN1  23

int pin_mode(int pin, int *addr_base,int mode);
int pin_set(int pin , int *addr_base, bool level);
int pin_level(int pin , int *addr_base);

