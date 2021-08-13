/**********************************************************
        内核虚拟地址空间头文件krlvadrsmem.h
***********************************************************
                彭东
**********************************************************/
#ifndef _KRLVADRSMEM_H
#define _KRLVADRSMEM_H
void teststc_t_init(teststc_t *initp);
teststc_t *new_teststc();
void del_teststc(teststc_t *delstc);
void add_new_teststc(adr_t vadr, size_t vsiz);
void vaslknode_t_init(vaslknode_t *initp);
void pgtabpage_t_init(pgtabpage_t *initp);
void kmvarsdsc_t_init(kmvarsdsc_t *initp);
void virmemadrs_t_init(virmemadrs_t *initp);
void kvirmemadrs_t_init(kvirmemadrs_t *initp);
kmvarsdsc_t *new_kmvarsdsc();
bool_t del_kmvarsdsc(kmvarsdsc_t *delkmvd);
virmemadrs_t *new_virmemadrs();
bool_t del_virmemadrs(virmemadrs_t *vmdsc);
mmadrsdsc_t* new_mmadrsdsc();
bool_t del_mmadrsdsc(mmadrsdsc_t* mm);
void init_kvirmemadrs();
adr_t kvma_initdefault_virmemadrs(mmadrsdsc_t* mm, adr_t start, size_t size, u32_t type);
kmvarsdsc_t *vma_find_kmvarsdsc_is_ok(virmemadrs_t *vmalocked, kmvarsdsc_t *curr, adr_t start, size_t vassize);
kmvarsdsc_t *vma_find_kmvarsdsc(virmemadrs_t *vmalocked, adr_t start, size_t vassize, u64_t vaslimits, u32_t vastype);
adr_t vma_new_vadrs_core(mmadrsdsc_t *mm, adr_t start, size_t vassize, u64_t vaslimits, u32_t vastype);
adr_t vma_new_vadrs(mmadrsdsc_t *mm, adr_t start, size_t vassize, u64_t vaslimits, u32_t vastype);
kmvarsdsc_t *vma_del_find_kmvarsdsc(virmemadrs_t *vmalocked, adr_t start, size_t vassize);
void vma_del_set_endcurrkmvd(virmemadrs_t *vmalocked, kmvarsdsc_t *del);
bool_t vma_del_unmapping_phyadrs(mmadrsdsc_t *mm, kmvarsdsc_t *kmvd, adr_t start, adr_t end);
bool_t vma_del_unmapping(mmadrsdsc_t *mm, kmvarsdsc_t *kmvd, adr_t start, size_t vassize);
bool_t vma_del_vadrs_core(mmadrsdsc_t *mm, adr_t start, size_t vassize);
bool_t vma_del_vadrs(mmadrsdsc_t *mm, adr_t start, size_t vassize);

int imrand();
int kvma_rand(int s, int e);
void chk_one_kmva(kmvarsdsc_t *chkkmva);
void check_kmva();
teststc_t *find_tstc_on_rnr(uint_t randnr);
void rand_write_tstc();
void rand_del_tstc();
void check_one_teststc(teststc_t *chktc);
void check_teststc();
void del_all_kvma();
void __test_kmva();
void test_kmva();
void test_vma();

adr_t kmap_retcurentcpu_pml4badrs();
cr3s_t knl_retn_currentcpu_cr3s();
void kmap_fulshcurrcpu_mmutable();

kmvarsdsc_t *vma_map_find_kmvarsdsc(virmemadrs_t *vmalocked, adr_t vadrs);
kvmemcbox_t *vma_map_retn_kvmemcbox(kmvarsdsc_t *kmvd);
bool_t vma_del_usermsa(mmadrsdsc_t *mm, kvmemcbox_t *kmbox, msadsc_t *msa, adr_t phyadr);
msadsc_t *vma_new_usermsa(mmadrsdsc_t *mm, kvmemcbox_t *kmbox);
adr_t vma_map_msa_fault(mmadrsdsc_t *mm, kvmemcbox_t *kmbox, adr_t vadrs, u64_t flags);
adr_t vma_map_phyadrs(mmadrsdsc_t *mm, kmvarsdsc_t *kmvd, adr_t vadrs, u64_t flags);
void vma_full_textbin(mmadrsdsc_t* mm, kmvarsdsc_t* kmvd, adr_t vadr);
sint_t vma_map_fairvadrs_core(mmadrsdsc_t *mm, adr_t vadrs);
sint_t vma_map_fairvadrs(mmadrsdsc_t *mm, adr_t vadrs);
mmadrsdsc_t* krl_curr_mmadrsdsc();
sint_t krluserspace_accessfailed(adr_t fairvadrs);
void test_krl_pages_fault();
void kvmcobjmgr_t_init(kvmcobjmgr_t *initp);
void kvmemcobj_t_init(kvmemcobj_t *initp);
kvmemcobj_t *new_kvmemcobj();
bool_t del_kvmemcobj(kvmemcobj_t *delkvmcop);
void kvmemcbox_t_init(kvmemcbox_t *init);
void kvmemcboxmgr_t_init(kvmemcboxmgr_t *init);
kvmemcbox_t *new_kvmemcbox();
bool_t del_kvmemcbox(kvmemcbox_t *del);
void knl_count_kvmemcbox(kvmemcbox_t *kmbox);
void knl_decount_kvmemcbox(kvmemcbox_t *kmbox);
kvmemcbox_t *knl_get_kvmemcbox();
bool_t knl_put_kvmemcbox(kvmemcbox_t *kmbox);
void dump_kvmemcboxmgr(kvmemcboxmgr_t *dump);
void dump_mem_range(adr_t vadr, uint_t count, uint_t type);
#endif
