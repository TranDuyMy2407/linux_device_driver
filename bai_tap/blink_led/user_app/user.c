#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>

int main()
{
	int level;
	printf("enter level : ");
	scanf("%d",&level);

	int fd = open("/dev/my_device", O_RDWR)	;
	if(fd < 0)
	{
		printf("cant open the file !!! \n");
		exit(-1);
	}

	ioctl(fd,1,level);
	close(fd);
	return 0;
}


