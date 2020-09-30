#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>


char buffer[1000];

int main()
{
	memset(buffer,0,sizeof(buffer));
	int fd = open("/dev/pcdev-3",O_RDWR);
	if(fd < 0)
	{
		printf("cant open the file !! \n");
		return -1;
	}

	int ret = read(fd,buffer,100);
	if(ret<0)
	{
		printf("EOF\n");
		return -1;
	}

	printf("%s\n",buffer);
	close(fd);
	return 0;
}
