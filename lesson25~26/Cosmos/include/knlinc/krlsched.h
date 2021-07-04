/**********************************************************
        线程调度头文件krlsched.h
***********************************************************
                彭东
**********************************************************/
#ifndef _KRLSCHED_H
#define _KRLSCHED_H
void thrdlst_t_init(thrdlst_t* initp);
void schdata_t_init(schdata_t* initp);
void schedclass_t_init(schedclass_t* initp);
void init_krlsched();
thread_t* krlsched_retn_currthread();
void krlsched_wait(kwlst_t* wlst);
void krlsched_up(kwlst_t* wlst);
void krlsched_set_schedflgs();
void krlsched_chkneed_pmptsched();
thread_t* krlsched_select_thread();
void krlschedul();
void krlschdclass_add_thread(thread_t* thdp);
#ifdef CFG_X86_PLATFORM
void __to_new_context(thread_t* next,thread_t* prev);
#else
void __to_new_context(thread_t* next,thread_t* prev);
#endif
void save_to_new_context(thread_t* next,thread_t* prev);
void retnfrom_first_sched(thread_t* thrdp);
#endif //schedule _KRLSCHED_H
