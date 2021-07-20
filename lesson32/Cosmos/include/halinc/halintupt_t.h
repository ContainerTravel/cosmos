/**********************************************************
        hal层中断处理头文件halintupt_t.h
***********************************************************
                彭东
**********************************************************/
#ifndef _HALINTUPT_T_H
#define _HALINTUPT_T_H


#ifdef CFG_X86_PLATFORM

typedef struct s_ILNEDSC
{
    u32_t ild_modflg;
    u32_t ild_devid;
    u32_t ild_physid;
    u32_t ild_clxsubinr;
}ilnedsc_t;





typedef struct s_INTFLTDSC
{
    spinlock_t  i_lock;
    u32_t       i_flg;
    u32_t       i_stus;
    uint_t      i_prity;        //中断优先级
    uint_t      i_irqnr;        //中断号
    uint_t      i_deep;         //中断嵌套深度
    u64_t       i_indx;         //中断计数
    list_h_t    i_serlist;
    uint_t      i_sernr;
    list_h_t    i_serthrdlst;   //中断线程链表头
    uint_t      i_serthrdnr;    //中断线程个数
    void*       i_onethread;    //只有一个中断线程时直接用指针
    void*       i_rbtreeroot;   //如果中断线程太多则按优先级组成红黑树
    list_h_t    i_serfisrlst;   //也可以使用中断回调函数的方式
    uint_t      i_serfisrnr;    //中断回调函数个数
    void*       i_msgmpool;     //可能的中断消息池
    void*       i_privp;
    void*       i_extp;
}intfltdsc_t;

typedef struct s_INTSERDSC
{
    list_h_t    s_list;
    list_h_t    s_indevlst;
    u32_t       s_flg;
    intfltdsc_t* s_intfltp;
    void*       s_device;
    uint_t      s_indx;
    intflthandle_t s_handle;
}intserdsc_t;

typedef struct s_KITHREAD
{
    spinlock_t  kit_lock;
    list_h_t    kit_list; 
    u32_t       kit_flg;
    u32_t       kit_stus;
    uint_t      kit_prity;
    u64_t       kit_scdidx;
    uint_t      kit_runmode;
    uint_t      kit_cpuid;
    u16_t       kit_cs;
    u16_t       kit_ss;
    uint_t      kit_nxteip;
    uint_t      kit_nxtesp;
    void*       kit_stk;
    size_t      kit_stksz;
    void*       kit_runadr;
    void*       kit_binmodadr;
    void*       kit_mmdsc;
    void*       kit_privp;
    void*       kit_extp;
}kithread_t;

#endif


#endif // HALINTUPT_T_H
