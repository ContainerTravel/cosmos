#include "stdio.h"
int main(void)
{
    hand_t id = 0;
    //建立100000进程压压惊!!
    for(int i = 0; i < 10000; i++)
    {
        id = exel("helloworld.app", i);
        printf("新建进程:%d, id:%x\n", i, (long)id);
    }
    return 0;
}