/**********************************************************
        内核虚拟地址空间头文件krlvadrsmem_t.h
***********************************************************
                彭东
**********************************************************/
#ifndef _KRLVADRSMEM_T_H
#define _KRLVADRSMEM_T_H

#define RET4PT_PML4EVDR (1)
#define RET4PT_PDPTEVDR (2)
#define RET4PT_PDETEVDR (3)
#define RET4PT_PTETEVDR (4)
#define RET4PT_PFAMEPDR (5)

#define VMAP_MIN_SIZE (MSA_SIZE)

#define KMBOX_CACHE_MAX (0x1000)
#define KMBOX_CACHE_MIN (0x40)
typedef struct KVMCOBJMGR
{
	spinlock_t kom_lock;
	u32_t kom_flgs;
	uint_t kom_kvmcobjnr;
	list_h_t kom_kvmcohead;
	uint_t kom_kvmcocahenr;
	list_h_t kom_kvmcocahe;
	uint_t kom_kvmcodelnr;
	list_h_t kom_kvmcodelhead;
}kvmcobjmgr_t;

typedef struct KVMEMCOBJ
{
	list_h_t kco_list;
	spinlock_t kco_lock;
	u32_t kco_cont;
	u32_t kco_flgs;
	u32_t kco_type;
	uint_t kco_msadnr;
	list_h_t kco_msadlst;
	void* kco_filenode;
	void* kco_pager;
	void* kco_extp;
}kvmemcobj_t;


typedef struct KVMEMCBOXMGR 
{
	list_h_t kbm_list;
	spinlock_t kbm_lock;
	u64_t kbm_flgs;
	u64_t kbm_stus;	
	uint_t kbm_kmbnr;
	list_h_t kbm_kmbhead;
	uint_t kbm_cachenr;
	uint_t kbm_cachemax;
	uint_t kbm_cachemin;
	list_h_t kbm_cachehead;
	void* kbm_ext;
}kvmemcboxmgr_t;

typedef struct KVMEMCBOX 
{
	list_h_t kmb_list;
	spinlock_t kmb_lock;
	refcount_t kmb_cont;
	u64_t kmb_flgs;
	u64_t kmb_stus;
	u64_t kmb_type;
	uint_t kmb_msanr;
	list_h_t kmb_msalist;
	kvmemcboxmgr_t* kmb_mgr;
	void* kmb_filenode;
	void* kmb_pager;
	void* kmb_ext;
}kvmemcbox_t;

typedef struct VASLKNODE
{
	u32_t  vln_color;
	u32_t  vln_flags;
	void*  vln_left;
	void*  vln_right;
	void*  vln_prev;
	void*  vln_next;
}vaslknode_t;

typedef struct TESTSTC
{
	list_h_t tst_list;
	adr_t    tst_vadr;
	size_t   tst_vsiz;
	uint_t   tst_type;
	uint_t   tst_lime;
}teststc_t;

typedef struct PGTABPAGE
{
	spinlock_t ptp_lock;
	list_h_t   ptp_msalist;
	uint_t     ptp_msanr;
}pgtabpage_t;



typedef struct KMVARSDSC
{
	spinlock_t kva_lock;
	u32_t  kva_maptype;
	list_h_t kva_list;
	u64_t  kva_flgs;
	u64_t  kva_limits;
	vaslknode_t kva_lknode;
	void*  kva_mcstruct;
	adr_t  kva_start;
	adr_t  kva_end;
	kvmemcbox_t* kva_kvmbox;
	void*  kva_kvmcobj;
}kmvarsdsc_t;

typedef struct KVIRMEMADRS
{
	spinlock_t kvs_lock;
	u64_t  kvs_flgs;
	uint_t kvs_kmvdscnr;
	kmvarsdsc_t* kvs_startkmvdsc;
	kmvarsdsc_t* kvs_endkmvdsc;	
	kmvarsdsc_t* kvs_krlmapdsc;
	kmvarsdsc_t* kvs_krlhwmdsc;
	kmvarsdsc_t* kvs_krlolddsc;
	adr_t kvs_isalcstart;
	adr_t kvs_isalcend;
	void* kvs_privte;
	void* kvs_ext;
	list_h_t kvs_testhead;
	uint_t   kvs_tstcnr;
	uint_t   kvs_randnext;
	pgtabpage_t kvs_ptabpgcs;
	kvmcobjmgr_t kvs_kvmcomgr;
	kvmemcboxmgr_t kvs_kvmemcboxmgr;
}kvirmemadrs_t;

typedef struct s_MMADRSDSC mmadrsdsc_t;

typedef struct s_VIRMEMADRS
{
	spinlock_t vs_lock;
	u32_t  vs_resalin;
	list_h_t vs_list;
	uint_t vs_flgs;
	uint_t vs_kmvdscnr;
	mmadrsdsc_t* vs_mm;
	kmvarsdsc_t* vs_startkmvdsc;
	kmvarsdsc_t* vs_endkmvdsc;
	kmvarsdsc_t* vs_currkmvdsc;
	kmvarsdsc_t* vs_krlmapdsc;
	kmvarsdsc_t* vs_krlhwmdsc;
	kmvarsdsc_t* vs_krlolddsc;
	adr_t vs_isalcstart;
	adr_t vs_isalcend;
	void* vs_privte;
	void* vs_ext;
}virmemadrs_t;

typedef struct s_MMADRSDSC
{
	spinlock_t msd_lock;
	list_h_t msd_list;
	uint_t msd_flag;
	uint_t msd_stus;
	uint_t msd_scount;
	sem_t  msd_sem;
	mmudsc_t msd_mmu;
	virmemadrs_t msd_virmemadrs;
	adr_t msd_stext;
	adr_t msd_etext;
	adr_t msd_sdata;
	adr_t msd_edata;
	adr_t msd_sbss;
	adr_t msd_ebss;
	adr_t msd_sbrk;
	adr_t msd_ebrk;
}mmadrsdsc_t;


#define VADSZ_ALIGN(x) ALIGN(x,0x1000)
#define KVMCOBJ_FLG_DELLPAGE (1)
#define KVMCOBJ_FLG_UDELPAGE (2)

#endif