#ifndef _IMGMGRHEAD_H
#define _IMGMGRHEAD_H
#define BFH_RW_R 1
#define BFH_RW_W 2

#define BFH_BUF_SZ 0x1000
#define BFH_ONERW_SZ 0x1000
#define BFH_RWONE_OK 1
#define BFH_RWONE_ER 2
#define BFH_RWALL_OK 3

#define FHDSC_NMAX 192
#define FHDSC_SZMAX 256
#define MDC_ENDGIC 0xaaffaaffaaffaaff
#define MDC_RVGIC 0xffaaffaaffaaffaa

typedef struct s_binfhead
{
    list_h_t bfh_list;
    uint_t bfh_rw;
    uint_t bfh_rwretstus;
    void *bfh_sfadr;
    uint_t bfh_sfsz;
    sint_t bfh_fd;
    char *bfh_fname;
    uint_t bfh_fsz;
    uint_t bfh_rwfcurrbyte;
    uint_t bfh_fonerwbyte;
    uint_t bfh_rwcount;
    uint_t bfh_fsum;
    void *bfh_buf;
    uint_t bfh_bufsz;
    void *bfh_rbcurrp;
} binfhead_t;

typedef struct s_fhdsc
{
    uint_t fhd_type;
    uint_t fhd_subtype;
    uint_t fhd_stuts;
    uint_t fhd_id;
    uint_t fhd_intsfsoff;
    uint_t fhd_intsfend;
    uint_t fhd_frealsz;
    uint_t fhd_fsum;
    char fhd_name[FHDSC_NMAX];
} fhdsc_t;

typedef struct s_mfh
{
    list_h_t mfh_list;
    uint_t mfh_nr;
    binfhead_t *mfh_bfhstart;
    uint_t mfh_bfhnr;
    uint_t mfh_curprocbfhnr;

} mfh_t;

typedef struct s_mlosrddsc
{
    u64_t mdc_mgic;
    u64_t mdc_sfsum;
    u64_t mdc_sfsoff;
    u64_t mdc_sfeoff;
    u64_t mdc_sfrlsz;
    u64_t mdc_ldrbk_s;
    u64_t mdc_ldrbk_e;
    u64_t mdc_ldrbk_rsz;
    u64_t mdc_ldrbk_sum;
    u64_t mdc_fhdbk_s;
    u64_t mdc_fhdbk_e;
    u64_t mdc_fhdbk_rsz;
    u64_t mdc_fhdbk_sum;
    u64_t mdc_filbk_s;
    u64_t mdc_filbk_e;
    u64_t mdc_filbk_rsz;
    u64_t mdc_filbk_sum;
    u64_t mdc_ldrcodenr;
    u64_t mdc_fhdnr;
    u64_t mdc_filnr;
    u64_t mdc_endgic;
    u64_t mdc_rv;
} mlosrddsc_t;

typedef struct s_fzone
{
    uint_t fstartpos;
    uint_t fcurrepos;
    uint_t fendpos;
} fzone_t;
typedef struct s_imgzone
{
    fzone_t bldrzn;
    fzone_t mftlzn;
    fzone_t fhedzn;
    fzone_t filezn;
} imgzone_t;

void set_fzone_fspos(fzone_t *fznp, uint_t val);
void set_fzone_fcpos(fzone_t *fznp, uint_t val);
void set_fzone_fepos(fzone_t *fznp, uint_t val);
void fhdsc_t_init(fhdsc_t *initp);
void fzone_t_init(fzone_t *initp);
void imgzone_t_init(imgzone_t *initp);
void init_imgmgrhead();
void exit_imgmgrhead();
void mfh_init(mfh_t *initp);
void mlosrddsc_t_init(mlosrddsc_t *initp);
void newalloc_allinputfilehead();
void binfhead_init(binfhead_t *initp);
binfhead_t *new_binfhead();
void add_binfhead(binfhead_t *bfhp);
binfhead_t *ret_outfhead();
binfhead_t *ret_inpfhead();
imgzone_t *ret_imgzone();
#endif
