#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>

int main()
{
	char *send = "hello world ";
	char buffer[1000];
	memset(buffer,0,sizeof(buffer));

	int fd = open("/dev/my_device",O_RDWR);
	if(fd < 0)
	{
		printf("cant open the file !!! \n");
		return -1;
	}

	int ret = read(fd,buffer,sizeof(buffer));
	if(ret < 0)
	{
		printf("cant read the file !!! \n");
		return -1;
	}

	printf("%s \n",buffer);

	close(fd);
	return 0;
}

