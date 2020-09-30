#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

char buffer[1024];
char *msg = "hello world";

int main()
{
	memset(buffer,0,sizeof(buffer));
	int fd = open("/dev/my_device-2",O_WRONLY);
	if(fd < 0)
	{
		printf("cant open the file !!! \n");
		exit(1);
	}

	int ret = write(fd,msg,100);
	if(ret < 0)
	{
		printf("EOF\n");
		exit(1);
	}
	
	//printf("%s\n",buffer);

	close(fd);
	
	return 0;
}
