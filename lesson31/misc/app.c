
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEV_NAME "/dev/devicesinfo"

int main(void)
{
    char buf[] = {0, 0, 0, 0};
    int i = 0;
    int fd;

    //打开设备文件 O_RDWR,  O_RDONLY, O_WRONLY,
    fd = open(DEV_NAME, O_RDWR);
    if (fd < 0)
    {
        printf("打开 :%s 失败!\n", DEV_NAME);
    }
    //写数据到内核空间
    write(fd, buf, 4);

    //从内核空间中读取数据
    read(fd, buf, 4);

    //关闭设备 可以不调用，程序关闭时系统自动调用
    close(fd);

    return 0;
}