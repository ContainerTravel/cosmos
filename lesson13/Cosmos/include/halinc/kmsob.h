/**********************************************************
        内核内存空间对象分配释放头文件kmsob.h
***********************************************************
                彭东
**********************************************************/
#ifndef _KMSOB_H
#define _KMSOB_H
void msclst_t_init(msclst_t* initp,uint_t pnr);
void msomdc_t_init(msomdc_t* initp);
void freobjh_t_init(freobjh_t* initp,uint_t stus,void* stat);
void kmsob_t_init(kmsob_t* initp);
void kmbext_t_init(kmbext_t* initp,adr_t vstat,adr_t vend,kmsob_t*kmsp);
void koblst_t_init(koblst_t* initp,size_t koblsz);
void kmsobmgrhed_t_init(kmsobmgrhed_t* initp);
void disp_kmsobmgr();
void disp_kmsob(kmsob_t* kmsp);
void init_kmsob();
void kmsob_updata_cache(kmsobmgrhed_t* kmobmgrp,koblst_t* koblp,kmsob_t* kmsp,uint_t flgs);
kmsob_t* scan_newkmsob_isok(kmsob_t* kmsp,size_t msz);
kmsob_t* scan_delkmsob_isok(kmsob_t *kmsp,void* fadrs,size_t fsz);
bool_t scan_nmszkmsob_isok(kmsob_t* kmsp,size_t msz);
bool_t scan_fadrskmsob_isok(adr_t fostat,adr_t vend,void* fadrs,size_t objsz);
bool_t scan_dfszkmsob_isok(kmsob_t* kmsp,void* fadrs,size_t fsz);
uint_t scan_kmsob_objnr(kmsob_t* kmsp);
kmsob_t* onkoblst_retn_newkmsob(koblst_t* koblp,size_t msz);
kmsob_t* onkoblst_retn_delkmsob(koblst_t *koblp,void* fadrs,size_t fsz);
koblst_t* onmsz_retn_koblst(kmsobmgrhed_t* kmmgrhlokp,size_t msz);
bool_t kmsob_add_koblst(koblst_t* koblp,kmsob_t* kmsp);
kmsob_t* _create_init_kmsob(kmsob_t* kmsp,size_t objsz,adr_t cvadrs,adr_t cvadre,msadsc_t* msa,uint_t relpnr);
kmsob_t* _create_kmsob(kmsobmgrhed_t* kmmgrlokp,koblst_t* koblp,size_t objsz);
void* kmsob_new_opkmsob(kmsob_t* kmsp,size_t msz);
bool_t kmsob_extn_pages(kmsob_t* kmsp);
void* kmsob_new_onkmsob(kmsob_t* kmsp,size_t msz);
void* kmsob_new_core(size_t msz);
void* kmsob_new(size_t msz);
uint_t scan_freekmsob_isok(kmsob_t* kmsp);
bool_t _destroy_kmsob_core(kmsobmgrhed_t* kmobmgrp,koblst_t* koblp,kmsob_t* kmsp);
bool_t _destroy_kmsob(kmsobmgrhed_t* kmobmgrp,koblst_t* koblp,kmsob_t* kmsp);
bool_t kmsob_del_opkmsob(kmsob_t* kmsp,void* fadrs,size_t fsz);
bool_t kmsob_delete_onkmsob(kmsob_t* kmsp,void* fadrs,size_t fsz);
bool_t kmsob_delete_core(void* fadrs,size_t fsz);
bool_t kmsob_delete(void* fadrs,size_t fsz);
bool_t chek_kmbext_findmsa(kmsob_t* kmsp,kmbext_t* cpbexp);
bool_t chek_one_kmbext(kmsob_t* kmsp,kmbext_t* cpbexp);
bool_t chek_onekmsob_mbext(kmsob_t* kmsp);
void chek_one_kmsob(kmsob_t* kmsp,size_t objsz);
void chek_all_kmsobstruc();
void kobcks_init(kobcks_t* initp,void* vadr,size_t sz);
void write_kobcks(kmsobmgrhed_t* kmmgrp,void* ptr,size_t sz);
void chek_one_kobcks(kobcks_t* kkp);
void chek_all_kobcks();
void free_one_kobcks(kobcks_t* kkp);
void free_all_kobcks();
void test_kmsob();
#endif