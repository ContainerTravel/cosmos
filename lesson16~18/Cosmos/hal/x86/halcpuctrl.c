/**********************************************************
        cpu控制文件cpuctrl.c
***********************************************************
                彭东 
**********************************************************/

#include "cosmostypes.h"
#include "cosmosmctrl.h"

void hal_spinlock_init(spinlock_t *lock)
{
    lock->lock = 0;
    return;
}

void hal_spinlock_lock(spinlock_t *lock)
{
    __asm__ __volatile__(
        "1:         \n"
        "lock; xchg  %0, %1 \n"
        "cmpl   $0, %0      \n"
        "jnz    2f      \n"
        ".section .spinlock.text,"
        "\"ax\""
        "\n"                    // 重新定义一个代码段所以jnz 2f下面并不是
        "2:         \n"         //cmpl $0,%1 事实上下面的代码不会常常执行,
        "cmpl   $0, %1      \n" //这是为了不在cpu指令高速缓存中填充无用代码
        "jne    2b      \n"     //要知道那可是用每位6颗晶体管做的双极性静态
        "jmp    1b      \n"     //储存器,比内存条快几千个数量级
        ".previous      \n"
        :
        : "r"(1), "m"(*lock));
    return;
}

void hal_spinlock_unlock(spinlock_t *lock)
{
    __asm__ __volatile__(
        "movl   $0, %0\n"
        :
        : "m"(*lock));
    return;
}

void hal_spinlock_saveflg_cli(spinlock_t *lock, cpuflg_t *cpuflg)
{
    __asm__ __volatile__(
        "pushfq             \n\t"
        "cli                \n\t"
        "popq %0            \n\t"

        "1:                 \n\t"
        "lock; xchg  %1, %2 \n\t"
        "cmpl   $0,%1       \n\t"
        "jnz    2f          \n\t"
        ".section .spinlock.text,"
        "\"ax\""
        "\n\t"                    //重新定义一个代码段所以jnz 2f下面并不是
        "2:                 \n\t" //cmpl $0,%1 事实上下面的代码不会常常执行,
        "cmpl   $0,%2       \n\t" //这是为了不在cpu指令高速缓存中填充无用代码
        "jne    2b          \n\t"
        "jmp    1b          \n\t"
        ".previous          \n\t"
        : "=m"(*cpuflg)
        : "r"(1), "m"(*lock));
    return;
}

void hal_spinunlock_restflg_sti(spinlock_t *lock, cpuflg_t *cpuflg)
{
    __asm__ __volatile__(
        "movl   $0, %0\n\t"
        "pushq %1 \n\t"
        "popfq \n\t"
        :
        : "m"(*lock), "m"(*cpuflg));
    return;
}

void knl_spinlock_init(spinlock_t *lock)
{
    lock->lock = 0;
    return;
}

void knl_spinlock_lock(spinlock_t *lock)
{
    __asm__ __volatile__(
        "1:         \n"
        "lock; xchg  %0, %1 \n"
        "cmpl   $0, %0      \n"
        "jnz    2f      \n"
        ".section .spinlock.text,"
        "\"ax\""
        "\n"                    // 重新定义一个代码段所以jnz 2f下面并不是
        "2:         \n"         //cmpl $0,%1 事实上下面的代码不会常常执行,
        "cmpl   $0, %1      \n" //这是为了不在cpu指令高速缓存中填充无用代码
        "jne    2b      \n"     //要知道那可是用每位6颗晶体管做的双极性静态
        "jmp    1b      \n"     //储存器,比内存条快几千个数量级
        ".previous      \n"
        :
        : "r"(1), "m"(*lock));
    return;
}

void knl_spinlock_unlock(spinlock_t *lock)
{
    __asm__ __volatile__(
        "movl   $0, %0\n"
        :
        : "m"(*lock));
    return;
}

void knl_spinlock_cli(spinlock_t *lock, cpuflg_t *cpuflg)
{
    __asm__ __volatile__(
        "pushfq             \n\t"
        "cli                \n\t"
        "popq %0            \n\t"

        "1:                 \n\t"
        "lock; xchg  %1, %2 \n\t"
        "cmpl   $0,%1       \n\t"
        "jnz    2f          \n\t"
        ".section .spinlock.text,"
        "\"ax\""
        "\n\t"                    //重新定义一个代码段所以jnz 2f下面并不是
        "2:                 \n\t" //cmpl $0,%1 事实上下面的代码不会常常执行,
        "cmpl   $0,%2       \n\t" //这是为了不在cpu指令高速缓存中填充无用代码
        "jne    2b          \n\t"
        "jmp    1b          \n\t"
        ".previous          \n\t"
        : "=m"(*cpuflg)
        : "r"(1), "m"(*lock));
    return;
}

void knl_spinunlock_sti(spinlock_t *lock, cpuflg_t *cpuflg)
{
    __asm__ __volatile__(
        "movl   $0, %0\n\t"
        "pushq %1 \n\t"
        "popfq \n\t"
        :
        : "m"(*lock), "m"(*cpuflg));
    return;
}

void hal_memset(void *setp, size_t n, u8_t setval)
{
    u8_t *_p = (u8_t *)setp;
    for (uint_t i = 0; i < n; i++)
    {
        _p[i] = setval;
    }
    return;
}

void hal_memcpy(void *src, void *dst, size_t n)
{
    u8_t *_s = (u8_t *)src, *_d = (u8_t *)dst;
    for (uint_t i = 0; i < n; i++)
    {
        _d[i] = _s[i];
    }
    return;
}

void hal_sysdie(char_t *errmsg)
{
    kprint("COSMOS SYSTEM IS DIE %s", errmsg);
    for (;;)
        ;
    return;
}

void system_error(char_t *errmsg)
{
    hal_sysdie(errmsg);
    return;
}

uint_t hal_retn_cpuid()
{
    return 0;
}