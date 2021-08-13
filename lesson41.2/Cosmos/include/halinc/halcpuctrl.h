/**********************************************************
        cpu控制头文件cpuctrl.h
***********************************************************
                彭东 
**********************************************************/
#ifndef _CPUCTRL_H
#define _CPUCTRL_H
void hal_wbinvd();
void hal_invd();
void hal_sti_cpuflag(cpuflg_t* cpuflg);
void hal_cli_cpuflag(cpuflg_t* cpuflg);
void hal_cpuflag_sti(cpuflg_t* cpuflg);
void hal_cpuflag_cli(cpuflg_t* cpuflg);
void hal_spinlock_init(spinlock_t *lock);
void hal_spinlock_lock(spinlock_t *lock);
void hal_spinlock_unlock(spinlock_t *lock);
void hal_spinlock_saveflg_cli(spinlock_t *lock, cpuflg_t *cpuflg);
void knl_spinlock(spinlock_t * lock);
void knl_spinunlock(spinlock_t * lock);
void hal_spinunlock_restflg_sti(spinlock_t *lock, cpuflg_t *cpuflg);
void knl_spinlock_init(spinlock_t *lock);
void knl_spinlock_lock(spinlock_t *lock);
void knl_spinlock_unlock(spinlock_t *lock);
void knl_spinlock_cli(spinlock_t *lock, cpuflg_t *cpuflg);
void knl_spinunlock_sti(spinlock_t *lock, cpuflg_t *cpuflg);
void hal_memset(void *setp, u8_t setval, size_t n);
void hal_memcpy(void *src, void *dst, size_t n);
void hal_sysdie(char_t *errmsg);
void system_error(char_t *errmsg);
uint_t hal_retn_cpuid();
#endif // CPUCTRL_H
