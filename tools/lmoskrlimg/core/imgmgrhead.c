#include "imgmctrl.h"
mfh_t mfhead;
binfhead_t outfhead;
binfhead_t inpfhead;
imgzone_t imgzone;
void init_imgmgrhead()
{
    imgzone_t_init(&imgzone);
    mfh_init(&mfhead);
    binfhead_init(&outfhead);
    binfhead_init(&inpfhead);
    return;
}


void exit_imgmgrhead()
{
    return;
}

void fhdsc_t_init(fhdsc_t* initp)
{
    initp->fhd_type=0;
    initp->fhd_subtype=0;
    initp->fhd_stuts=0;
    initp->fhd_id=0;
    initp->fhd_intsfsoff=0;
    initp->fhd_intsfend=0;
    initp->fhd_frealsz=0;
    initp->fhd_fsum=0;
    for(uint_t i=0;i<FHDSC_NMAX;i++)
    {
        initp->fhd_name[i]=0;
    }
    return;
}


void mfh_init(mfh_t* initp)
{
    list_init(&initp->mfh_list);
    initp->mfh_nr=0;
    initp->mfh_bfhstart=NULL;
    initp->mfh_bfhnr=0;
    initp->mfh_curprocbfhnr=0;
    return;
}
void fzone_t_init(fzone_t* initp)
{
    initp->fstartpos=0;
    initp->fcurrepos=0;
    initp->fendpos=0;
    return;
}

void imgzone_t_init(imgzone_t* initp)
{
    fzone_t_init(&initp->bldrzn);
    fzone_t_init(&initp->mftlzn);
    fzone_t_init(&initp->fhedzn);
    fzone_t_init(&initp->filezn);
    return;
}
void mlosrddsc_t_init(mlosrddsc_t* initp)
{
    char* chp=(char*)initp;
    chp[0]='L';//LMOSMDSK
    chp[1]='M';
    chp[2]='O';
    chp[3]='S';
    chp[4]='M';
    chp[5]='D';
    chp[6]='S';
    chp[7]='K';
    initp->mdc_sfsum=0;
    initp->mdc_sfsoff=0;
    initp->mdc_sfeoff=0;
    initp->mdc_sfrlsz=sizeof(mlosrddsc_t);
    initp->mdc_ldrbk_s=0;
    initp->mdc_ldrbk_e=0;
    initp->mdc_ldrbk_rsz=0;
    initp->mdc_ldrbk_sum=0;
    initp->mdc_fhdbk_s=0;
    initp->mdc_fhdbk_e=0;
    initp->mdc_fhdbk_rsz=0;
    initp->mdc_fhdbk_sum=0;
    initp->mdc_filbk_s=0;
    initp->mdc_filbk_e=0;
    initp->mdc_filbk_rsz=0;
    initp->mdc_filbk_sum=0;
    initp->mdc_ldrcodenr=0;
    initp->mdc_fhdnr=0;
    initp->mdc_filnr=0;
    initp->mdc_endgic=MDC_ENDGIC;
    initp->mdc_rv=MDC_RVGIC;
    return;
}

void newalloc_allinputfilehead()
{
    void* adr;
    uint_t fnr=limg_ret_infilenr();
    if(fnr==0)
    {
        limg_error("ret_infilenr");
    }
    adr=img_mem(((sizeof(binfhead_t))*fnr),NULL,MFLG_ALLOC);
    if(adr==NULL)
    {
        limg_error("img_mem");
    }
    mfhead.mfh_bfhstart=(binfhead_t*)adr;
    binfhead_t* bhp=(binfhead_t*)adr;
    mfhead.mfh_bfhnr=fnr;
    for(uint_t i=0;i<fnr;i++)
    {
        binfhead_init(&bhp[i]);
    }
    return;
}


void binfhead_init(binfhead_t* initp)
{
    list_init(&initp->bfh_list);
    initp->bfh_rw=0;
    initp->bfh_rwretstus=0;
    initp->bfh_sfadr=initp;
    initp->bfh_sfsz=sizeof(binfhead_t);
    initp->bfh_fd=-1;
    initp->bfh_fname=NULL;
    initp->bfh_fsz=0;
    initp->bfh_rwfcurrbyte=0;
    initp->bfh_fonerwbyte=0;
    initp->bfh_rwcount=0;
    initp->bfh_fsum=0;
    initp->bfh_buf=NULL;
    initp->bfh_bufsz=0;
    initp->bfh_rbcurrp=NULL;
    return;
}
binfhead_t* new_binfhead()
{
    void* bfh=img_mem(sizeof(binfhead_t),NULL,MFLG_ALLOC);
    if(bfh==NULL)
    {
        limg_error("new_binfhead allocbfhmm error");
        return NULL;
    }
    binfhead_init((binfhead_t*)bfh);
    return (binfhead_t*)bfh;
}

void add_binfhead(binfhead_t* bfhp)
{
    list_add(&bfhp->bfh_list,&mfhead.mfh_list);
    mfhead.mfh_nr++;
    return;
}

binfhead_t* ret_outfhead()
{
    return &outfhead;
}
binfhead_t* ret_inpfhead()
{
    return &inpfhead;
}

imgzone_t* ret_imgzone()
{
    return &imgzone;
}
void set_fzone_fspos(fzone_t* fznp,uint_t val)
{
    fznp->fstartpos=val;
    return;
}

void set_fzone_fcpos(fzone_t* fznp,uint_t val)
{
    fznp->fcurrepos=val;
    return;
}

void set_fzone_fepos(fzone_t* fznp,uint_t val)
{
    fznp->fendpos=val;
    return;
}
