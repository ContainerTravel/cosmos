#ifndef _IMGUNDO_H
#define _IMGUNDO_H

typedef struct s_imgundo
{

    int iu_fd;
    uint_t iu_fsz;
    char *iu_fname;
    void *iu_buf;
    size_t iu_bufsz;
    void *iu_mdscbuf;
    size_t iu_mdscbufsz;
    off_t iu_mdscinfilepos_s;

    uint_t iu_fhdnr;
    void *iu_fhdbuf;
    size_t iu_fhdbufsz;
    uint_t iu_fhdrwnr;
    sint_t iu_fhdrwstus;
    off_t iu_fhdinfilepos_s;
    off_t iu_fhdinfilepos_c;
    off_t iu_fhdinfilepos_e;

} imgundo_t;

typedef struct s_udoutf
{
    int uf_fd;
    size_t uf_fsz;
    char *uf_fname;
    void *uf_buf;
    size_t uf_bufsz;
    size_t uf_onerwsz;
    size_t uf_currrwsz;
    imgundo_t *uf_iudoscp;
    fhdsc_t *uf_curfhdscp;
} udoutf_t;
void init_imgundo();
void exit_imgundo();
void imgundo_t_init(imgundo_t *initp);
void udoutf_t_init(udoutf_t *initp);
void limg_undomain();
void limg_udfidinfo_omlosrddsc();
void limg_run_undoimg();
sint_t limg_undoimg_onfhdsc(fhdsc_t *fhdp, udoutf_t *oufp);
fhdsc_t *limg_undo_retnextfhdsc();
sint_t undo_read_nextfhdsc(imgundo_t *iudp);
void alloc_undofile(fhdsc_t *fhdscp, udoutf_t *oufp);
void free_undofile(udoutf_t *oufp);
int sum_mlorddsc(mlosrddsc_t *mrddcp);
void read_mlosrddsc();
void allocbuf_onimundo();
void open_undofile();
void close_undofile();
#endif
