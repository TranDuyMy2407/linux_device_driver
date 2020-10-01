#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

char buffer1[1024];
char buffer2[1024];
char *msg = "Hello world";

int main()
{
	int fd = open("/dev/my_device-2",O_RDWR);
	if(fd < 0)
	{
		printf("cant open the file !!! \n");
		exit(1);
	}

	
	
	int ret = read(fd,buffer1,100);
	if(ret < 0)
	{
		printf("EOF");
		return 0;
	}


	close(fd);
	
	int fd2 = open("/dev/my_device-3",O_RDWR);
	if(fd2 < 0)
	{
		printf("cant open the file !! \n");
		exit(1);
	}
	
	int ret2 = read(fd,buffer2,100);
	if(ret2 < 0)
	{
		printf("EOF");
		return 0;
	}
	close(fd2);



	printf("my_dev-3 : %s\n",buffer1);
	printf("my_dev-4 : %s\n ", buffer2);
	
	return 0;
}
