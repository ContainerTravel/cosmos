/**********************************************************
		原子操作头文件atomic.h
***********************************************************
		彭东
**********************************************************/
#ifndef _ATOMIC_H
#define _ATOMIC_H

KLINE s32_t atomic_read(const atomic_t *v)
{
        return (*(volatile u32_t *)&(v)->a_count);
}
KLINE void atomic_set(atomic_t *v, int i)
{
        v->a_count = i;
}

KLINE void atomic_add(int i, atomic_t *v)
{
        __asm__ __volatile__("lock;"
                             "addl %1,%0"
                             : "+m"(v->a_count)
                             : "ir"(i));
}

KLINE void atomic_sub(int i, atomic_t *v)
{
        __asm__ __volatile__("lock;"
                             "subl %1,%0"
                             : "+m"(v->a_count)
                             : "ir"(i));
}

KLINE int atomic_sub_and_test(int i, atomic_t *v)
{
        unsigned char c;

        __asm__ __volatile__("lock;"
                             "subl %2,%0; sete %1"
                             : "+m"(v->a_count), "=qm"(c)
                             : "ir"(i)
                             : "memory");
        return c;
}

KLINE void atomic_inc(atomic_t *v)
{
        __asm__ __volatile__("lock;"
                             "incl %0"
                             : "+m"(v->a_count));
}

KLINE void atomic_dec(atomic_t *v)
{
        __asm__ __volatile__("lock;"
                             "decl %0"
                             : "+m"(v->a_count));
}

KLINE int atomic_dec_and_test(atomic_t *v)
{
        unsigned char c;

        __asm__ __volatile__("lock;"
                             "decl %0; sete %1"
                             : "+m"(v->a_count), "=qm"(c)
                             :
                             : "memory");
        return c != 0;
}

KLINE int atomic_inc_and_test(atomic_t *v)
{
        unsigned char c;

        __asm__ __volatile__("lock;"
                             "incl %0; sete %1"
                             : "+m"(v->a_count), "=qm"(c)
                             :
                             : "memory");
        return c != 0;
}

KLINE void refcount_init(refcount_t *initp)
{
        atomic_set(&initp->ref_count, 0);
        return;
}

KLINE void refcount_inc(refcount_t *refc)
{
        atomic_inc(&refc->ref_count);
        return;
}

KLINE void refcount_dec(refcount_t *refc)
{
        atomic_dec(&refc->ref_count);
}

KLINE s32_t refcount_read(refcount_t *refc)
{
        return atomic_read(&refc->ref_count);
}

#endif
