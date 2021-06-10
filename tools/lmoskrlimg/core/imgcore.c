#include "imgmctrl.h"
void init_imgcore()
{
    return;
}

void exit_imgcore()
{
    return;
}

void limg_core()
{
    limg_core_in_mode_run(limg_ret_imgmode());
    return;
}

void limg_boot_mode_run()
{
    return;
}

void limg_krnl_mode_run()
{
    sint_t sz = 0, retsz = -1, ldrsz = -1;
    limg_config_fzone();
    new_outimg_file();
    if (limg_write_imginitblk() == -1)
    {
        limg_error("write initblk");
    }
    ldrsz = limg_write_ldrheadfile();
    if (ldrsz == -1)
    {
        limg_error("write ldrheadfile");
    }

    char *istr = limg_retnext_ipathname();
    while (istr != NULL)
    {
        retsz = run_rw_func_oninfile(istr);
        if (retsz == -1)
        {
            limg_error("run error");
        }
        sz += retsz;
        istr = limg_retnext_ipathname();
    }
    if (limg_write_mlosrddsc() == -1)
    {
        limg_error("write mlosrddsc");
    }
    del_outimg_file();
    return;
}

void limg_ldsk_mode_run()
{
    return;
}

void limg_undo_mode_run()
{
    limg_undomain();
    return;
}

void limg_core_in_mode_run(uint_t mode)
{

    if (mode == BOOT_MODE)
    {
        limg_boot_mode_run();
        return;
    }
    if (mode == KRNEL_MODE)
    {
        limg_krnl_mode_run();
        return;
    }
    if (mode == LDSK_MODE)
    {
        limg_ldsk_mode_run();
        return;
    }
    if (mode == UNDO_MODE)
    {
        limg_undo_mode_run();
        return;
    }
    limg_error("no mode");
    return;
}

sint_t limg_write_mlosrddsc()
{
    binfhead_t *oubhp = ret_outfhead();
    uint_t fnr = limg_ret_infilenr();
    void *buf = img_mem(BFH_BUF_SZ, NULL, MFLG_ALLOC);
    if (buf == NULL)
    {
        return -1;
    }
    limg_memclr(buf, 0, BFH_BUF_SZ);
    mlosrddsc_t *mdp = (mlosrddsc_t *)buf;
    mlosrddsc_t_init(mdp);
    imgzone_t *imgzp = ret_imgzone();

    mdp->mdc_sfsoff = imgzp->mftlzn.fstartpos;
    mdp->mdc_sfeoff = imgzp->mftlzn.fendpos;
    mdp->mdc_sfrlsz = sizeof(mlosrddsc_t);
    mdp->mdc_ldrbk_s = imgzp->bldrzn.fstartpos;
    mdp->mdc_ldrbk_e = imgzp->bldrzn.fendpos;
    mdp->mdc_ldrbk_rsz = imgzp->bldrzn.fcurrepos - imgzp->bldrzn.fstartpos;
    mdp->mdc_fhdbk_s = imgzp->fhedzn.fstartpos;
    mdp->mdc_fhdbk_e = imgzp->fhedzn.fendpos;
    mdp->mdc_fhdbk_rsz = imgzp->fhedzn.fcurrepos - imgzp->fhedzn.fstartpos;
    mdp->mdc_filbk_s = imgzp->filezn.fstartpos;
    mdp->mdc_filbk_e = imgzp->filezn.fendpos;
    mdp->mdc_filbk_rsz = imgzp->filezn.fcurrepos - imgzp->filezn.fstartpos;
    mdp->mdc_ldrcodenr = 1;
    mdp->mdc_fhdnr = fnr;
    mdp->mdc_filnr = fnr;
    uint_t sum = computer_sum(buf, sizeof(mlosrddsc_t));
    mdp->mdc_sfsum = sum;

    limg_set_mlosrddsccurrpos(oubhp);
    ssize_t sz = limg_writefile((int)oubhp->bfh_fd, buf, (size_t)BFH_BUF_SZ);
    if (sz == -1)
    {

        if (img_mem(0, buf, MFLG_ALLOC) == NULL)
        {
            limg_error("img_mem error");
            return -1;
        }
        return -1;
    }
    printf("文件数：%d\n", (int)mdp->mdc_filnr);
    printf("映像文件大小：%d\n", (int)mdp->mdc_filbk_rsz);
    limg_upd_mlosrddsccurrpos(oubhp);
    if (img_mem(0, buf, MFLG_ALLOC) == NULL)
    {
        limg_error("img_mem error");
        return -1;
    }
    return 1;
}

sint_t limg_write_ldrheadfile()
{

    return run_rw_func_onldfile(limg_ret_ldrhpathname());
}

int limg_write_imginitblk()
{
    uint_t fnr = limg_ret_infilenr();
    if (fnr == 0)
    {
        limg_error("no file");
    }
    uint_t pfhblkn = BLK_ALIGN(fnr * 256) >> 12;
    pfhblkn += 0x2;
    void *buf = img_mem(BFH_BUF_SZ, NULL, MFLG_ALLOC);
    if (buf == NULL)
    {
        return -1;
    }
    limg_memclr(buf, 0, BFH_BUF_SZ);
    binfhead_t *bfhdp = ret_outfhead();
    for (uint_t bi = 0; bi < pfhblkn; bi++)
    {
        ssize_t sz = limg_writefile((int)bfhdp->bfh_fd, buf, (size_t)BFH_BUF_SZ);
        if (sz == -1)
        {

            if (img_mem(0, buf, MFLG_ALLOC) == NULL)
            {
                limg_error("img_mem error");
                return -1;
            }
            return -1;
        }
    }

    if (img_mem(0, buf, MFLG_ALLOC) == NULL)
    {
        limg_error("img_mem error");
        return -1;
    }

    return 1;
}

void limg_config_subzn(fzone_t *fznp, uint_t fstartpos, uint_t fcurrepos, uint_t fendpos)
{
    fznp->fstartpos = fstartpos;
    fznp->fcurrepos = fcurrepos;
    fznp->fendpos = fendpos;
    return;
}

void limg_config_fzone()
{
    imgzone_t *imgzp = ret_imgzone();
    limg_config_subzn(&imgzp->bldrzn, 0, 0, 0xfff);
    limg_config_subzn(&imgzp->mftlzn, 0x1000, 0x1000, 0x1fff);
    uint_t fnr = limg_ret_infilenr();

    if (fnr == 0)
    {
        limg_error("no file");
    }

    uint_t pfhblkn = BLK_ALIGN(fnr * 256);
    limg_config_subzn(&imgzp->fhedzn, 0x2000, 0x2000, (0x2000 + pfhblkn) - 1);
    limg_config_subzn(&imgzp->filezn, (0x2000 + pfhblkn), (0x2000 + pfhblkn), (0x2000 + pfhblkn) + limg_ret_allifimglen() - 1);
    return;
}

void limg_write_bldrzn()
{
    return;
}

void limg_write_mftlzn()
{
    return;
}

void limg_write_filezn()
{
    return;
}

void limg_set_mlosrddsccurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    uint_t pos = imgzp->mftlzn.fcurrepos;
    //printf("pos:%x\n",(int)pos);
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, pos, SEEK_SET);
    if (off == -1)
    {
        limg_error("set leek");
    }

    return;
}

void limg_upd_mlosrddsccurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, 0, SEEK_CUR);
    if (off == -1)
    {
        limg_error("set leek");
    }
    imgzp->mftlzn.fcurrepos = off;
    return;
}

void limg_set_ldrfilecurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    uint_t pos = imgzp->bldrzn.fcurrepos;
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, pos, SEEK_SET);
    if (off == -1)
    {
        limg_error("set leek");
    }

    return;
}

void limg_upd_ldrfilecurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, 0, SEEK_CUR);
    if (off == -1)
    {
        limg_error("set leek");
    }
    imgzp->bldrzn.fcurrepos = off;
    return;
}

void limg_set_infilecurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    uint_t pos = imgzp->filezn.fcurrepos;
    //printf("pos:%x\n",(int)pos);
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, pos, SEEK_SET);
    if (off == -1)
    {
        limg_error("set leek");
    }

    return;
}
uint_t limg_ret_infilecurrpos()
{
    imgzone_t *imgzp = ret_imgzone();
    uint_t pos = imgzp->filezn.fcurrepos;

    return pos;
}

void limg_upd_infilecurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, 0, SEEK_CUR);
    if (off == -1)
    {
        limg_error("set leek");
    }
    imgzp->filezn.fcurrepos = off;
    return;
}

void limg_set_fheadcurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    uint_t pos = imgzp->fhedzn.fcurrepos;
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, pos, SEEK_SET);
    if (off == -1)
    {
        limg_error("set leek");
    }

    return;
}

void limg_upd_fheadcurrpos(binfhead_t *bfhp)
{
    imgzone_t *imgzp = ret_imgzone();
    off_t off = limg_lseekfile((int)bfhp->bfh_fd, 0, SEEK_CUR);
    if (off == -1)
    {
        limg_error("set leek");
    }
    imgzp->fhedzn.fcurrepos = off;
    return;
}

int limg_write_onefhdsc(binfhead_t *bfhp, uint_t inimgoff, uint_t inimgend, uint_t frealsz, char *fname)
{
    if (sizeof(fhdsc_t) != FHDSC_SZMAX)
    {
        return -1;
    }
    size_t slen = strlen(fname);
    if (slen >= FHDSC_NMAX)
    {
        return -1;
    }
    fhdsc_t *fdbuf = (fhdsc_t *)img_mem(sizeof(fhdsc_t), NULL, MFLG_ALLOC);
    if (fdbuf == NULL)
    {
        return -1;
    }

    fhdsc_t_init(fdbuf);
    fdbuf->fhd_intsfsoff = inimgoff;
    fdbuf->fhd_intsfend = inimgend;
    fdbuf->fhd_frealsz = frealsz;
    fdbuf->fhd_fsum = bfhp->bfh_fsum;
    strcpy(fdbuf->fhd_name, fname);

    limg_set_fheadcurrpos(bfhp);
    ssize_t sz = limg_writefile((int)bfhp->bfh_fd, (void *)fdbuf, (size_t)sizeof(fhdsc_t));
    if (sz == -1)
    {
        limg_error("limg_write");
        return -1;
    }
    limg_upd_fheadcurrpos(bfhp);
    if (img_mem(0, fdbuf, MFLG_FREE) == NULL)
    {
        limg_error("img_mem");
        return -1;
    }

    return 1;
}
int limg_rw_bldr_file(binfhead_t *ibfhp, binfhead_t *obfhp)
{
    int rets = -1;
    limg_set_ldrfilecurrpos(obfhp);
    for (; ibfhp->bfh_rwretstus != BFH_RWALL_OK;)
    {
        read_imgfile_to_buf(ibfhp);
        if (ibfhp->bfh_rwretstus == BFH_RWONE_ER)
        {
            rets = -1;
            limg_error("read_imgfile");
            goto err_ret;
        }
        copy_infbuf_to_oufbuf(ibfhp, obfhp);
        write_imgfile_fr_buf(obfhp);
        if (obfhp->bfh_rwretstus == BFH_RWONE_ER)
        {
            rets = -1;
            limg_error("write_imgfile");
            goto err_ret;
        }
    }
    limg_upd_ldrfilecurrpos(obfhp);
    rets = 1;
err_ret:
    return rets;
}

int limg_rw_one_file(binfhead_t *ibfhp, binfhead_t *obfhp)
{
    int rets = -1;
    limg_set_infilecurrpos(obfhp);
    for (; ibfhp->bfh_rwretstus != BFH_RWALL_OK;)
    {
        read_imgfile_to_buf(ibfhp);
        if (ibfhp->bfh_rwretstus == BFH_RWONE_ER)
        {
            rets = -1;
            limg_error("read_imgfile");
            goto err_ret;
        }
        copy_infbuf_to_oufbuf(ibfhp, obfhp);
        write_imgfile_fr_buf(obfhp);
        if (obfhp->bfh_rwretstus == BFH_RWONE_ER)
        {
            rets = -1;
            limg_error("write_imgfile");
            goto err_ret;
        }
    }
    limg_upd_infilecurrpos(obfhp);
    rets = 1;
err_ret:
    return rets;
}

sint_t run_rw_func_onldfile(char *pathname)
{
    sint_t rets = -1;
    binfhead_t *inbhp = ret_inpfhead(), *oubhp = ret_outfhead();
    if (limg_retszfile(pathname) > BFH_BUF_SZ || limg_retszfile(pathname) == 0)
    {
        return -1;
    }
    if (alloc_new_inputfile(pathname, inbhp) == -1)
    {
        limg_error("alloc_new_inputfile");
        return -1;
    }

    if (limg_rw_bldr_file(inbhp, oubhp) == -1)
    {
        rets = -1;
        limg_error("limg_rw_one_file");
    }

    rets = inbhp->bfh_fsz;
    if (free_new_inputfile(inbhp) == -1)
    {
        rets = -1;
        limg_error("free_new_inputfile");
        return -1;
    }
    return rets;
}

sint_t run_rw_func_oninfile(char *pathname)
{
    sint_t rets = -1;
    binfhead_t *inbhp = ret_inpfhead(), *oubhp = ret_outfhead();

    if (alloc_new_inputfile(pathname, inbhp) == -1)
    {
        limg_error("alloc_new_inputfile");
    }

    uint_t soff = limg_ret_infilecurrpos();
    if (limg_rw_one_file(inbhp, oubhp) == -1)
    {
        rets = -1;
        limg_error("limg_rw_one_file");
    }
    uint_t eoff = limg_ret_infilecurrpos();

    if (limg_write_onefhdsc(oubhp, soff, eoff - 1, inbhp->bfh_fsz, inbhp->bfh_fname) == -1)
    {
        rets = -1;
        limg_error("limg_rw_one_file");
    }
    rets = inbhp->bfh_fsz;
    if (free_new_inputfile(inbhp) == -1)
    {
        rets = -1;
        limg_error("free_new_inputfile");
    }

    return rets;
}


void del_outimg_file()
{

    binfhead_t *bfhdp = ret_outfhead();
    int fd = limg_closefile((int)bfhdp->bfh_fd);
    if (fd == -1)
    {
        limg_error("close file error");
        return;
    }
    return;
}
void new_outimg_file()
{
    char *outfna = limg_retnext_opathname();
    binfhead_t *bfhdp = ret_outfhead();

    if (bfhdp == NULL || outfna == NULL)
    {
        limg_error("not outfile binfhead or not outfile");
    }
    int fd = open_newoutimgfile(outfna);
    if (fd == -1)
    {
        limg_error("not open outfile");
    }

    void *binbuf = img_mem(BFH_BUF_SZ, NULL, MFLG_ALLOC);
    if (binbuf == NULL)
    {
        if (limg_closefile(fd) == -1)
        {
            limg_error("close1 file error");
        }
        limg_error("allocbuf error");
        return;
    }
    bfhdp->bfh_rw = BFH_RW_W;
    bfhdp->bfh_fd = fd;
    bfhdp->bfh_fname = outfna;
    bfhdp->bfh_fonerwbyte = BFH_ONERW_SZ;
    bfhdp->bfh_fsum = 0;
    bfhdp->bfh_buf = binbuf;
    bfhdp->bfh_bufsz = BFH_BUF_SZ;
    bfhdp->bfh_rbcurrp = binbuf;

    return;
}
int alloc_new_inputfile(char *pathname, binfhead_t *inpbfhp)
{
    if (pathname == NULL || inpbfhp == NULL)
    {
        return -1;
    }
    int fd = limg_openfile(pathname, O_RDWR);
    if (fd == -1)
    {
        limg_error("open file error");
        return -1;
    }
    uint_t fsz = limg_retszfile(pathname);
    if (fsz == 0)
    {
        if (limg_closefile(fd) == -1)
        {
            limg_error("close1 file error");
        }
        limg_error("retfilesz error");
        return -1;
    }
    void *binbuf = img_mem(BFH_BUF_SZ, NULL, MFLG_ALLOC);
    if (binbuf == NULL)
    {
        if (limg_closefile(fd) == -1)
        {
            limg_error("close2 file error");
        }
        limg_error("allocbinbuf error");
        return -1;
    }
    inpbfhp->bfh_rw = BFH_RW_R;
    inpbfhp->bfh_fd = fd;
    inpbfhp->bfh_fname = pathname;
    inpbfhp->bfh_fsz = fsz;
    inpbfhp->bfh_fonerwbyte = BFH_ONERW_SZ;
    inpbfhp->bfh_fsum = 0;
    inpbfhp->bfh_buf = binbuf;
    inpbfhp->bfh_bufsz = BFH_BUF_SZ;
    inpbfhp->bfh_rbcurrp = binbuf;
    return fd;
}

int free_new_inputfile(binfhead_t *inpbfhp)
{
    if (inpbfhp == NULL)
    {
        return -1;
    }

    int fd = limg_closefile((int)inpbfhp->bfh_fd);
    if (fd == -1)
    {
        limg_error("close file error");
        return -1;
    }

    void *binbuf = img_mem(0, inpbfhp->bfh_buf, MFLG_FREE);
    if (binbuf == NULL)
    {

        limg_error("freebinbuf error");
        return -1;
    }
    binfhead_init(inpbfhp);
    return fd;
}

uint_t computer_sum(void *buf, uint_t sz)
{
    uint_t sum = 0;
    u8_t *svp = (u8_t *)buf;
    for (uint_t i = 0; i < sz; i++)
    {
        sum += svp[i];
    }
    return sum;
}

void read_imgfile_to_buf(binfhead_t *bfhdp)
{
    limg_memclr(bfhdp->bfh_buf, 0, bfhdp->bfh_fonerwbyte);
    ssize_t sz = limg_readfile((int)bfhdp->bfh_fd, bfhdp->bfh_buf, (size_t)bfhdp->bfh_fonerwbyte);
    if (sz == -1)
    {
        bfhdp->bfh_rwretstus = BFH_RWONE_ER;
        return;
    }
    bfhdp->bfh_rwcount++;
    bfhdp->bfh_rwfcurrbyte += sz;
    bfhdp->bfh_fsum = computer_sum(bfhdp->bfh_buf, bfhdp->bfh_fonerwbyte);
    if (bfhdp->bfh_rwfcurrbyte == bfhdp->bfh_fsz)
    {
        bfhdp->bfh_rwretstus = BFH_RWALL_OK;
        return;
    }
    bfhdp->bfh_rwretstus = BFH_RWONE_OK;
    return;
}

void copy_infbuf_to_oufbuf(binfhead_t *ibfhdp, binfhead_t *obfhdp)
{
    limg_memcpy(obfhdp->bfh_buf, ibfhdp->bfh_buf, ibfhdp->bfh_fonerwbyte);
    return;
}

void write_imgfile_fr_buf(binfhead_t *bfhdp)
{

    ssize_t sz = limg_writefile((int)bfhdp->bfh_fd, bfhdp->bfh_buf, (size_t)bfhdp->bfh_fonerwbyte);
    if (sz == -1)
    {
        bfhdp->bfh_rwretstus = BFH_RWONE_ER;
        return;
    }
  
    bfhdp->bfh_rwcount++;
    bfhdp->bfh_rwfcurrbyte += sz;
    bfhdp->bfh_fsum = computer_sum(bfhdp->bfh_buf, bfhdp->bfh_fonerwbyte);
    bfhdp->bfh_rwretstus = BFH_RWONE_OK;
    return;
}

int open_newoutimgfile(char *pathname)
{
    return limg_newfile(pathname, O_RDWR | O_CREAT | O_TRUNC, 0777);
}
