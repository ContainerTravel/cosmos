#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
int main(int argc, char const *argv[])
{
    long cpus = syscall(441);
    printf("cpu num is:%d\n", cpus);
    return 0;
}
