#ifndef _LIMGERROR_H
#define _LIMGERROR_H
#define limg_error(x)                                                                    \
    do                                                                                   \
    {                                                                                    \
        printf("LKRLIMG ERR:%s ", x);                                                    \
        printf("C file:%s C line:%d C function:%s\n", __FILE__, __LINE__, __FUNCTION__); \
        exit(0);                                                                         \
    } while (0);

void init_error();
void exit_error();
#endif
