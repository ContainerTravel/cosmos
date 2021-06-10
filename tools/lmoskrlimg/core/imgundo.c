#include "imgmctrl.h"

imgundo_t mundostc;
udoutf_t mudooutfile;
void init_imgundo()
{
    imgundo_t_init(&mundostc);
    udoutf_t_init(&mudooutfile);
    return;
}
void exit_imgundo()
{
    return;
}

void imgundo_t_init(imgundo_t *initp)
{
    initp->iu_fd = -1;
    initp->iu_fsz = 0;
    initp->iu_fname = NULL;
    initp->iu_buf = NULL;
    initp->iu_bufsz = BFH_BUF_SZ;
    initp->iu_mdscbuf = NULL;
    initp->iu_mdscbufsz = BFH_BUF_SZ;
    initp->iu_mdscinfilepos_s = 0x1000;
    initp->iu_fhdnr = 0;
    initp->iu_fhdbuf = NULL;
    initp->iu_fhdbufsz = BFH_BUF_SZ;
    initp->iu_fhdrwnr = 0;
    initp->iu_fhdrwstus = 0;
    initp->iu_fhdinfilepos_s = 0;
    initp->iu_fhdinfilepos_c = 0;
    initp->iu_fhdinfilepos_e = 0;
    return;
}

void udoutf_t_init(udoutf_t *initp)
{
    initp->uf_fd = -1;
    initp->uf_fsz = 0;
    initp->uf_fname = NULL;
    initp->uf_buf = NULL;
    initp->uf_bufsz = BFH_BUF_SZ;
    initp->uf_onerwsz = 0;
    initp->uf_currrwsz = 0;
    initp->uf_iudoscp = NULL;
    initp->uf_curfhdscp = NULL;
    return;
}

void limg_undomain()
{
    open_undofile();
    allocbuf_onimundo();
    read_mlosrddsc();
    limg_udfidinfo_omlosrddsc();
    limg_run_undoimg();
    close_undofile();
    return;
}

void limg_udfidinfo_omlosrddsc()
{
    mlosrddsc_t *mdcp = (mlosrddsc_t *)mundostc.iu_mdscbuf;
    mundostc.iu_fhdnr = mdcp->mdc_fhdnr;
    mundostc.iu_fhdinfilepos_s = mdcp->mdc_fhdbk_s;
    mundostc.iu_fhdinfilepos_c = mdcp->mdc_fhdbk_s;
    mundostc.iu_fhdinfilepos_e = mdcp->mdc_fhdbk_e;
    return;
}

void limg_run_undoimg()
{
    fhdsc_t *fhdp = limg_undo_retnextfhdsc();
    while (fhdp != NULL)
    {
        //
        if (-1 == limg_undoimg_onfhdsc(fhdp, &mudooutfile))
        {
            limg_error("undofile err");
        }

        fhdp = limg_undo_retnextfhdsc();
    }
    return;
}

sint_t limg_undoimg_onfhdsc(fhdsc_t *fhdp, udoutf_t *oufp)
{
    if (fhdp == NULL || oufp == NULL)
    {
        limg_error("no fhdp oufp");
        return -1;
    }
    alloc_undofile(fhdp, oufp);

    off_t ufs = fhdp->fhd_intsfsoff; 
    if (limg_lseekfile(oufp->uf_iudoscp->iu_fd, ufs, SEEK_SET) == -1)
    {
        limg_error("set leek");
        return -1;
    }
 
    for (;;)
    {
        if (oufp->uf_currrwsz >= oufp->uf_fsz)
        {
            break;
        }

        limg_memclr(oufp->uf_buf, 0, oufp->uf_bufsz);

        ssize_t sz = limg_readfile(oufp->uf_iudoscp->iu_fd, oufp->uf_buf, oufp->uf_onerwsz);
        if (sz == -1)
        {
            limg_error("read fhdsc");
            return -1;
        }
        oufp->uf_currrwsz += sz;
        size_t wsz = sz;
        
        if (oufp->uf_currrwsz > oufp->uf_fsz)
        {
            wsz = oufp->uf_bufsz - (oufp->uf_currrwsz - oufp->uf_fsz);
        }
        
        if (limg_writefile(oufp->uf_fd, oufp->uf_buf, wsz) == -1)
        {
            limg_error("write file err");
        }
    }

    free_undofile(oufp);
    return 1;
}

fhdsc_t *limg_undo_retnextfhdsc()
{
    if (mundostc.iu_fhdrwnr < mundostc.iu_fhdnr)
    {
        if (undo_read_nextfhdsc(&mundostc) == -1)
        {
            limg_error("read nextfhdsc");
            return NULL;
        }
        return (fhdsc_t *)mundostc.iu_fhdbuf;
    }
    return NULL;
}

sint_t undo_read_nextfhdsc(imgundo_t *iudp)
{
    off_t off = limg_lseekfile(iudp->iu_fd, iudp->iu_fhdinfilepos_c, SEEK_SET);
    if (off == -1)
    {
        limg_error("set leek");
        return -1;
    }
    limg_memclr(iudp->iu_fhdbuf, 0, iudp->iu_fhdbufsz);
    ssize_t sz = limg_readfile(iudp->iu_fd, iudp->iu_fhdbuf, sizeof(fhdsc_t));
    if (sz < (ssize_t)(sizeof(fhdsc_t)))
    {
        limg_error("read fhdsc");
        return -1;
    }
    off = limg_lseekfile(iudp->iu_fd, 0, SEEK_CUR);
    if (off == -1)
    {
        limg_error("set leek");
        return -1;
    }
    iudp->iu_fhdinfilepos_c = off;
    iudp->iu_fhdrwnr++;
    return (sint_t)sz;
}

void alloc_undofile(fhdsc_t *fhdscp, udoutf_t *oufp)
{
    if (fhdscp == NULL || oufp == NULL)
    {
        limg_error("no fhdp oufp");
        return;
    }
    void *buf = img_mem(oufp->uf_bufsz, NULL, MFLG_ALLOC);
    if (buf == NULL)
    {
        limg_error("img_mem");
        return;
    }
    int fd = limg_newfile(fhdscp->fhd_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
    {
        if (img_mem(0, buf, MFLG_FREE) == NULL)
        {
            limg_error("img_mem");
            return;
        }
        limg_error("new file");
        return;
    }
    oufp->uf_fd = fd;
    oufp->uf_fname = fhdscp->fhd_name;
    oufp->uf_fsz = fhdscp->fhd_frealsz;
    oufp->uf_curfhdscp = fhdscp;
    oufp->uf_iudoscp = &mundostc;

    oufp->uf_buf = buf;
    oufp->uf_onerwsz = oufp->uf_bufsz;
    return;
}

void free_undofile(udoutf_t *oufp)
{
    if (limg_closefile(oufp->uf_fd) == -1)
    {
        limg_error("close file");
        return;
    }
    if (img_mem(0, oufp->uf_buf, MFLG_FREE) == NULL)
    {
        limg_error("img_mem");
        return;
    }
    udoutf_t_init(oufp);
    return;
}


int sum_mlorddsc(mlosrddsc_t *mrddcp)
{
    //LMOSMDSK
    char *strmgic = (char *)mrddcp;
    u64_t sum = 0;

    if (strmgic[0] == 'L' && strmgic[1] == 'M' && strmgic[2] == 'O' &&
        strmgic[3] == 'S' && strmgic[4] == 'M' && strmgic[5] == 'D' &&
        strmgic[6] == 'S' && strmgic[7] == 'K')
    {
        if (mrddcp->mdc_endgic != MDC_ENDGIC || mrddcp->mdc_rv != MDC_RVGIC)
        {
            return -1;
        }
        sum = mrddcp->mdc_sfsum;
        mrddcp->mdc_sfsum = 0;

        if (sum != computer_sum((void *)mrddcp, sizeof(mlosrddsc_t)))
        {
           
            limg_error("sum err");
            return -1;
        }
        mrddcp->mdc_sfsum = sum;
        return 1;
    }
    return -1;
}

void read_mlosrddsc()
{
    if (mundostc.iu_fd == -1 || mundostc.iu_fsz < 0x1000 ||
        mundostc.iu_mdscinfilepos_s != 0x1000)
    {
        limg_error("no undofile");
        return;
    }
    if (mundostc.iu_buf == NULL || mundostc.iu_mdscbuf == NULL || mundostc.iu_bufsz < BFH_BUF_SZ || mundostc.iu_mdscbufsz < BFH_BUF_SZ)
    {
        limg_error("no undofile");
        return;
    }
    if (limg_lseekfile(mundostc.iu_fd, 0x1000, SEEK_SET) != 0x1000)
    {
        limg_error("lseek file");
        return;
    }
    if (limg_readfile(mundostc.iu_fd, mundostc.iu_buf, mundostc.iu_bufsz) == -1)
    {
        limg_error("read file");
        return;
    }

    limg_memcpy(mundostc.iu_mdscbuf, mundostc.iu_buf, mundostc.iu_mdscbufsz);
    if (sum_mlorddsc((mlosrddsc_t *)mundostc.iu_mdscbuf) == -1)
    {
        limg_error("sum file");
        return;
    }

    return;
}

void allocbuf_onimundo()
{
    void *buf = img_mem(mundostc.iu_bufsz, NULL, MFLG_ALLOC);
    if (buf == NULL)
    {
        limg_error("img_mem");
        return;
    }
    mundostc.iu_buf = buf;
    limg_memclr(mundostc.iu_buf, 0, mundostc.iu_bufsz);
    buf = img_mem(mundostc.iu_mdscbufsz, NULL, MFLG_ALLOC);
    if (buf == NULL)
    {
        limg_error("img_mem");
        return;
    }
    mundostc.iu_mdscbuf = buf;
    limg_memclr(mundostc.iu_mdscbuf, 0, mundostc.iu_mdscbufsz);
    buf = img_mem(mundostc.iu_fhdbufsz, NULL, MFLG_ALLOC);
    if (buf == NULL)
    {
        limg_error("img_mem");
        return;
    }
    mundostc.iu_fhdbuf = buf;
    limg_memclr(mundostc.iu_fhdbuf, 0, mundostc.iu_fhdbufsz);
    return;
}

void open_undofile()
{
    char *pna = limg_retnext_ipathname();
    if (pna == NULL)
    {
        limg_error("on file");
        return;
    }
    int fd = limg_openfile(pna, O_RDWR);
    if (fd == -1)
    {
        limg_error("openfile");
        return;
    }
    mundostc.iu_fsz = limg_retszfile(pna);
    mundostc.iu_fname = pna;
    mundostc.iu_fd = fd;
    return;
}

void close_undofile()
{
    if (limg_closefile(mundostc.iu_fd) == -1)
    {
        limg_error("openfile");
        return;
    }
    return;
}
