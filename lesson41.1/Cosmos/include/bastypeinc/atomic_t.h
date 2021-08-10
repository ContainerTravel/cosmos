/**********************************************************
		原子操作头文件atomic.h
***********************************************************
				彭东
**********************************************************/
#ifndef _ATOMIC_T_H
#define _ATOMIC_T_H

typedef struct s_ATOMIC{
	volatile s32_t a_count;
}atomic_t;

typedef struct s_REFCOUNT
{
	atomic_t ref_count;
}refcount_t;

#endif
