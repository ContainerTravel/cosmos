/**********************************************************
        物理内存分割合并文件memdivmer.h
***********************************************************
                彭东
**********************************************************/
#ifndef _MEMDIVMER_H
#define _MEMDIVMER_H
void mm_update_memmgrob(uint_t realpnr,uint_t flgs);
void mm_update_memarea(memarea_t* malokp,uint_t pgnr,uint_t flgs);
memarea_t* onfrmsa_retn_marea(memmgrob_t* mmobjp,msadsc_t* freemsa,uint_t freepgs);
memarea_t* onmrtype_retn_marea(memmgrob_t* mmobjp,uint_t mrtype);
u64_t onfrmsa_retn_fpagenr(msadsc_t* freemsa);
bafhlst_t* onma_retn_maxbafhlst(memarea_t* malckp);
msadsc_t* mm_divpages_opmsadsc(msadsc_t* msastat,uint_t mnr);
sint_t mm_merpages_opmsadsc(bafhlst_t* bafh,msadsc_t* freemsa,uint_t freepgs);
bool_t onmpgs_retn_bafhlst(memarea_t* malckp,uint_t pages,bafhlst_t** retrelbafh,bafhlst_t** retdivbafh);
bool_t onfpgs_retn_bafhlst(memarea_t* malckp,uint_t freepgs,bafhlst_t** retrelbf,bafhlst_t** retmerbf);
msadsc_t* mm_divipages_onbafhlst(bafhlst_t* bafhp);
bool_t mm_retnmsaob_onbafhlst(bafhlst_t* bafhp,msadsc_t** retmstat,msadsc_t** retmend);
bool_t scan_mapgsalloc_ok(memarea_t* malckp,uint_t pages);
msadsc_t* mm_maxdivpages_onmarea(memarea_t* malckp,uint_t* retrelpnr);
uint_t chek_divlenmsa(msadsc_t* msastat,msadsc_t* msaend,uint_t mnr);
bool_t mrdmb_add_msa_bafh(bafhlst_t* bafhp,msadsc_t* msastat,msadsc_t* msaend);
msadsc_t* mm_reldpgsdivmsa_bafhl(memarea_t *malckp, uint_t pages, uint_t *retrelpnr,bafhlst_t* relbfl,bafhlst_t* divbfl);
msadsc_t* mm_reldivpages_onmarea(memarea_t* malckp,uint_t pages,uint_t* retrelpnr);
msadsc_t* mm_prcdivpages_onmarea(memarea_t *malckp, uint_t pages, uint_t *retrelpnr);
msadsc_t* mm_divpages_core(memarea_t* mareap,uint_t pages,uint_t* retrealpnr,uint_t flgs);
msadsc_t* mm_divpages_fmwk(memmgrob_t* mmobjp,uint_t pages,uint_t* retrelpnr,uint_t mrtype,uint_t flgs);
msadsc_t* mm_division_pages(memmgrob_t* mmobjp,uint_t pages,uint_t*retrealpnr,uint_t mrtype,uint_t flgs);
memarea_t* retn_procmarea(memmgrob_t *mmobjp);
msadsc_t* divpages_procmarea_core(memmgrob_t *mmobjp, uint_t pages, uint_t *retrealpnr);
msadsc_t* mm_divpages_procmarea(memmgrob_t *mmobjp, uint_t pages, uint_t *retrealpnr);
bool_t scan_freemsa_isok(msadsc_t* freemsa,uint_t freepgs);
sint_t mm_cmsa1blk_isok(bafhlst_t* bafh,msadsc_t* _1ms,msadsc_t* _1me);
sint_t mm_cmsa2blk_isok(bafhlst_t* bafh,msadsc_t* _1ms,msadsc_t* _1me,msadsc_t* _2ms,msadsc_t* _2me);
bool_t chek_cl2molkflg(bafhlst_t* bafh,msadsc_t* _1ms,msadsc_t* _1me,msadsc_t* _2ms,msadsc_t* _2me);
bool_t mm_clear_2msaolflg(bafhlst_t* bafh,msadsc_t* _1ms,msadsc_t* _1me,msadsc_t* _2ms,msadsc_t* _2me);
sint_t mm_find_cmsa2blk(bafhlst_t* fbafh,msadsc_t** rfnms,msadsc_t** rfnme);
bool_t mpobf_add_msadsc(bafhlst_t* bafhp,msadsc_t* freemstat,msadsc_t* freemend);
bool_t mm_merpages_onbafhlst(msadsc_t* freemsa,uint_t freepgs,bafhlst_t* relbf,bafhlst_t* merbf);
bool_t mm_merpages_onmarea(memarea_t* malckp,msadsc_t* freemsa,uint_t freepgs);
bool_t mm_merpages_core(memarea_t* marea,msadsc_t* freemsa,uint_t freepgs);
bool_t mm_merpages_fmwk(memmgrob_t* mmobjp,msadsc_t* freemsa,uint_t freepgs);
bool_t mm_merge_pages(memmgrob_t* mmobjp,msadsc_t* freemsa,uint_t freepgs);
void mchkstuc_t_init(mchkstuc_t* initp);
void write_one_mchkstuc(msadsc_t* msa,uint_t pnr);
bool_t chek_one_mchks(mchkstuc_t* mchs);
void cmp_mchkstuc(mchkstuc_t* smchs,mchkstuc_t* dmchs);
void free_one_mchkstuc(mchkstuc_t* mchs);
void free_all_mchkstuc();
void chek_all_one_mchkstuc(mchkstuc_t* mchs);
void chek_all_mchkstuc();
uint_t retn_test_sec();
void test_proc_marea(memarea_t* mr);
void test_maxone_marea(memarea_t* mr);
void test_onedivmer_all(memarea_t* ma);
void test_maxdiv_all();
void test_divsion_pages();
#endif