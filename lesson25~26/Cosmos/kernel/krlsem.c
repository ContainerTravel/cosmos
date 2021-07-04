/**********************************************************
        信号量文件krlsem.c
***********************************************************
                彭东 
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void krlsem_t_init(sem_t* initp)
{
    krlspinlock_init(&initp->sem_lock);
    initp->sem_flg=0;
    initp->sem_count=0;
    kwlst_t_init(&initp->sem_waitlst);
    return;
}

void krlsem_set_sem(sem_t* setsem,uint_t flg,sint_t conut)
{
    cpuflg_t cpufg;
    krlspinlock_cli(&setsem->sem_lock,&cpufg);
    setsem->sem_flg=flg;
    setsem->sem_count=conut;
    krlspinunlock_sti(&setsem->sem_lock,&cpufg);
    return;
}    

void krlsem_down(sem_t* sem)
{
    cpuflg_t cpufg;
start_step:    
    krlspinlock_cli(&sem->sem_lock,&cpufg);
    if(sem->sem_count<1)
    {
    
        krlwlst_wait(&sem->sem_waitlst);
        krlspinunlock_sti(&sem->sem_lock,&cpufg);
        krlschedul();
        goto start_step; 
    }
    sem->sem_count--;
    krlspinunlock_sti(&sem->sem_lock,&cpufg);
    return;
}

void krlsem_up(sem_t* sem)
{
    cpuflg_t cpufg;

    krlspinlock_cli(&sem->sem_lock,&cpufg);
    sem->sem_count++;
    if(sem->sem_count<1)
    {
        krlspinunlock_sti(&sem->sem_lock,&cpufg);
        hal_sysdie("sem up err");
    }
    krlwlst_allup(&sem->sem_waitlst);
    krlspinunlock_sti(&sem->sem_lock,&cpufg);
    krlsched_set_schedflgs();
    return;
}