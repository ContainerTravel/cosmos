#include "imgmctrl.h"

mparam_t mparam;
void mparam_t_init(mparam_t* initp)
{
    initp->mp_argc=0;
    initp->mp_argv=NULL;
    initp->mp_sifnr=0;
    initp->mp_eifnr=0;
    initp->mp_sofnr=0;
    initp->mp_ifcurrnr=0;
    initp->mp_ildrhnr=0;
    initp->mp_imgmode=0;
    initp->mp_ifnr=0;

    return;
}

void init_param()
{
    mparam_t_init(&mparam);
    return;
}

void exit_param()
{
    return;
}

void limg_param(int argc, char *argv[])
{
    mparam.mp_argc=argc;
    mparam.mp_argv=argv;

    limg_param_is_mode();
    if(limg_ret_imgmode()==UNDO_MODE)
    {
        limg_param_is_inputfile();
        return;
    }


    limg_param_is_ldhfile();
    limg_param_is_inputfile();
    limg_param_is_outfile();

 
    return;
}

void limg_param_is_inputfile()
{
    uint_t sifi=0,eifi=0;
    for(uint_t agi=1;agi<(uint_t)mparam.mp_argc;agi++)
    {
        if((strcmp(mparam.mp_argv[agi],"-f")==0)||
                (strcmp(mparam.mp_argv[agi],"-F")==0))
        {
            sifi=agi+1;
            goto ok_t_lable;
        }
    }
    sifi=0;
ok_t_lable:
    if(sifi==0||sifi>=(uint_t)mparam.mp_argc)
    {
        limg_error("limg_param_is_inputfile not -t");
    }

    eifi=sifi;
    uint_t agii=eifi;
    for(;agii<(uint_t)mparam.mp_argc;agii++)
    {
        if(mparam.mp_argv[agii][0]=='-')
        {
            eifi=agii-1;
            goto ok_lable;
        }
    }
    eifi=agii-1;

ok_lable:
    if(eifi<sifi)
    {
        limg_error("limg_param_is_inputfile not inputfilename");
    }
    mparam.mp_sifnr=sifi;
    mparam.mp_eifnr=eifi;
    return;
}

void limg_param_is_outfile()
{
    uint_t ofi=0;
    for(uint_t agi=1;agi<(uint_t)mparam.mp_argc;agi++)
    {
        if((strcmp(mparam.mp_argv[agi],"-o")==0)||
                (strcmp(mparam.mp_argv[agi],"-O")==0))
        {
            ofi=agi+1;
            goto ok_lable;
        }
    }
    ofi=0;
ok_lable:
    if(ofi==0||ofi>=(uint_t)mparam.mp_argc)
    {
        limg_error("limg_param_is_outfile not -o");
    }
    if(mparam.mp_argv[ofi][0]=='-')
    {
        limg_error("limg_param_is_outfile not outfilename");
    }
    mparam.mp_sofnr=ofi;
    return;
}


void limg_param_is_ldhfile()
{
    uint_t ofi=0;
    for(uint_t agi=1;agi<(uint_t)mparam.mp_argc;agi++)
    {
        if((strcmp(mparam.mp_argv[agi],"-lhf")==0)||
                (strcmp(mparam.mp_argv[agi],"-LHF")==0))
        {
            ofi=agi+1;
            goto ok_lable;
        }
    }
    ofi=0;
ok_lable:
    if(ofi==0||ofi>=(uint_t)mparam.mp_argc)
    {
        limg_error("limg_param_is_outfile not -o");
    }
    if(mparam.mp_argv[ofi][0]=='-')
    {
        limg_error("limg_param_is_outfile not outfilename");
    }
    mparam.mp_ildrhnr=ofi;
    return;
}




void limg_param_is_mode()
{
    uint_t mi=0;
    for(uint_t agi=1;agi<(uint_t)mparam.mp_argc;agi++)
    {
        if((strcmp(mparam.mp_argv[agi],"-m")==0)||
                (strcmp(mparam.mp_argv[agi],"-M")==0))
        {
            mi=agi+1;
            goto ok_lable;
        }
    }
    mi=0;
ok_lable:
    if(mi==0||mi>=(uint_t)mparam.mp_argc)
    {
        limg_error("limg_param_is_mode not -m");
    }
    if((strcmp(mparam.mp_argv[mi],"b")==0))
    {
        mparam.mp_imgmode=BOOT_MODE;
        return;
    }
    if((strcmp(mparam.mp_argv[mi],"k")==0))
    {
        mparam.mp_imgmode=KRNEL_MODE;
        return;
    }
    if((strcmp(mparam.mp_argv[mi],"u")==0))
    {
        mparam.mp_imgmode=UNDO_MODE;
        return;
    }
    limg_error("limg_param_is_mode mode is not");
    return;
}

uint_t limg_ret_allfilblk()
{
    uint_t afsz=limg_ret_allinfilesz();
    if(afsz==0)
    {
        return 0;
    }
    uint_t agbn=BLK_ALIGN(afsz);
    return (agbn>>12);
}

uint_t limg_ret_allifimglen()
{
    uint_t retlen=0;
    uint_t fz=0;
    uint_t i=mparam.mp_sifnr,j=mparam.mp_eifnr;
    if(i==0||j<i)
    {
        return 0;
    }
    for(;i<=j;i++)
    {
        fz=limg_retszfile(mparam.mp_argv[i]);
        if(fz==0)
        {
            return 0;
        }
        retlen+=BLK_ALIGN(fz);
    }
    return retlen;
}

uint_t limg_ret_allinfilesz()
{
    uint_t afz=0,fz=0;
    uint_t i=mparam.mp_sifnr,j=mparam.mp_eifnr;
    if(i==0||j<i)
    {
        return 0;
    }
    for(;i<=j;i++)
    {
        fz=limg_retszfile(mparam.mp_argv[i]);
        if(fz==0)
        {
            return 0;
        }
        afz+=fz;
    }
    return afz;
}

uint_t limg_ret_infilenr()
{
    if(mparam.mp_eifnr<mparam.mp_sifnr)
    {
        return 0;
    }

    return ((mparam.mp_eifnr-mparam.mp_sifnr)+1);
}

uint_t limg_ret_fileinitblknr()
{
    uint_t fnr=limg_ret_infilenr();


    if(fnr==0)
    {
        limg_error("no file");
    }

    uint_t pfhblkn=BLK_ALIGN(fnr*256)>>12;
    return pfhblkn;
}


char* limg_retnext_ipathname()
{
    if(mparam.mp_sifnr==0||mparam.mp_eifnr==0||
            (mparam.mp_sifnr+mparam.mp_ifcurrnr)>mparam.mp_eifnr)
    {
        return NULL;
    }
    uint_t nextifnr=mparam.mp_sifnr+mparam.mp_ifcurrnr;
    if(nextifnr>=(uint_t)mparam.mp_argc)
    {
        return NULL;
    }
    mparam.mp_ifcurrnr++;
    return mparam.mp_argv[nextifnr];
}

char* limg_ret_ldrhpathname()
{
    if(mparam.mp_ildrhnr==0)
    {
        return NULL;
    }
    uint_t nextifnr=mparam.mp_ildrhnr;
    if(nextifnr>=(uint_t)mparam.mp_argc)
    {
        return NULL;
    }
    return mparam.mp_argv[nextifnr];
}


char* limg_retnext_opathname()
{
    if(mparam.mp_sofnr==0)
    {
        return NULL;
    }
    uint_t nextifnr=mparam.mp_sofnr;
    if(nextifnr>=(uint_t)mparam.mp_argc)
    {
        return NULL;
    }
    return mparam.mp_argv[nextifnr];
}

uint_t limg_ret_imgmode()
{
    return mparam.mp_imgmode;
}
