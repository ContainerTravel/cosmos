/**********************************************************
        平台相关的文件halplatform.c
***********************************************************
                彭东
**********************************************************/

#include "cosmostypes.h"
#include "cosmosmctrl.h"

adr_t viradr_to_phyadr(adr_t kviradr)
{
    if (kviradr < KRNL_MAP_VIRTADDRESS_START || kviradr > KRNL_MAP_VIRTADDRESS_END)
    {
        system_error("viradr_to_phyadr err\n");
        return KRNL_ADDR_ERROR;
    }
    return kviradr - KRNL_MAP_VIRTADDRESS_START;
}

adr_t phyadr_to_viradr(adr_t kphyadr)
{
    if (kphyadr >= KRNL_MAP_PHYADDRESS_END)
    {
        system_error("phyadr_to_viradr err\n");
        return KRNL_ADDR_ERROR;
    }
    return kphyadr + KRNL_MAP_VIRTADDRESS_START;
}

void machbstart_t_init(machbstart_t *initp)
{
    memset(initp, 0, sizeof(machbstart_t));
    //initp->mb_migc=MBS_MIGC;
    return;
}

void init_machbstart()
{
    machbstart_t *kmbsp = &kmachbsp;
    machbstart_t *smbsp = MBSPADR;
    machbstart_t_init(kmbsp);
    memcopy((void *)phyadr_to_viradr((adr_t)smbsp), (void *)kmbsp, sizeof(machbstart_t));
    return;
}

void init_halplaltform()
{
    init_machbstart();
    init_bdvideo();
    return;
}

int strcmpl(const char *a, const char *b)
{

    while (*b && *a && (*b == *a))
    {

        b++;

        a++;
    }

    return *b - *a;
}

fhdsc_t *get_fileinfo(char_t *fname, machbstart_t *mbsp)
{
    mlosrddsc_t *mrddadrs = (mlosrddsc_t *)phyadr_to_viradr((adr_t)(mbsp->mb_imgpadr + MLOSDSC_OFF));
    if (mrddadrs->mdc_endgic != MDC_ENDGIC ||
        mrddadrs->mdc_rv != MDC_RVGIC ||
        mrddadrs->mdc_fhdnr < 2 ||
        mrddadrs->mdc_filnr < 2)
    {
        system_error("no mrddsc");
    }
    s64_t rethn = -1;
    fhdsc_t *fhdscstart = (fhdsc_t *)((uint_t)((mrddadrs->mdc_fhdbk_s) + (phyadr_to_viradr((adr_t)mbsp->mb_imgpadr))));

    for (u64_t i = 0; i < mrddadrs->mdc_fhdnr; i++)
    {
        if (strcmpl(fname, fhdscstart[i].fhd_name) == 0)
        {
            rethn = (s64_t)i;
            goto ok_l;
        }
    }
    rethn = -1;
ok_l:
    if (rethn < 0)
    {
        system_error("not find file");
    }
    return &fhdscstart[rethn];
}

void get_file_rvadrandsz(char_t *fname, machbstart_t *mbsp, u64_t *retadr, u64_t *retsz)
{
    u64_t padr = 0, fsz = 0;
    if (NULL == fname || NULL == mbsp)
    {
        *retadr = 0;
        return;
    }
    fhdsc_t *fhdsc = get_fileinfo(fname, mbsp);
    if (fhdsc == NULL)
    {
        *retadr = 0;
        return;
    }
    padr = fhdsc->fhd_intsfsoff + phyadr_to_viradr((adr_t)mbsp->mb_imgpadr);
    fsz = fhdsc->fhd_frealsz;

    *retadr = padr;
    *retsz = fsz;
    return;
}

e820map_t *get_maxmappadr_e820map(machbstart_t *mbsp, u64_t mappadr)
{
    if (NULL == mbsp)
    {
        return NULL;
    }
    u64_t enr = mbsp->mb_e820nr;
    e820map_t *emp = (e820map_t *)phyadr_to_viradr((adr_t)mbsp->mb_e820padr);
    e820map_t *retemp = NULL;
    u64_t maxadr = emp[0].saddr;
    for (u64_t i = 0; i < enr; i++)
    {
        if (emp[i].type == RAM_USABLE)
        {
            if (emp[i].saddr > maxadr && (mappadr > (emp[i].saddr + emp[i].lsize)))
            {
                maxadr = emp[i].saddr;
                retemp = &emp[i];
            }
        }
    }
    return retemp;
}
e820map_t *ret_kmaxmpadrcmpsz_e820map(machbstart_t *mbsp, u64_t mappadr, u64_t cpsz)
{
    if (NULL == mbsp)
    {
        return NULL;
    }
    u64_t enr = mbsp->mb_e820nr;
    e820map_t *emp = (e820map_t *)phyadr_to_viradr((adr_t)mbsp->mb_e820padr);
    e820map_t *retemp = NULL;
    u64_t maxadr = emp[0].saddr;
    for (u64_t i = 0; i < enr; i++)
    {
        if (emp[i].type == RAM_USABLE)
        {
            if (emp[i].saddr >= maxadr && (mappadr > (emp[i].saddr + emp[i].lsize)) &&
                (emp[i].lsize >= cpsz))
            {
                maxadr = emp[i].saddr;
                retemp = &emp[i];
            }
        }
    }
    if ((mappadr > (retemp->saddr + retemp->lsize)) && (retemp->lsize >= cpsz))
    {
        return retemp;
    }
    return NULL;
}

void move_img2maxpadr(machbstart_t *mbsp)
{
    u64_t kmapadrend = mbsp->mb_kpmapphymemsz;
    e820map_t *emp = ret_kmaxmpadrcmpsz_e820map(mbsp, kmapadrend, mbsp->mb_imgsz);
    if (NULL == emp)
    {
        system_error("move_img2maxpadr1 emp not ok");
    }

    u64_t imgtoadr = (emp->saddr + (emp->lsize - mbsp->mb_imgsz));
    imgtoadr &= ~(0xfffUL);
    if (initchkadr_is_ok(mbsp, imgtoadr, mbsp->mb_imgsz) != 0)
    {
        system_error("initchkadr_is_ok not ok\n");
    }
    void *sadr = (void *)phyadr_to_viradr((adr_t)mbsp->mb_imgpadr);
    void *dadr = (void *)phyadr_to_viradr((adr_t)imgtoadr);
    if (m2mcopy(sadr, dadr, (sint_t)(mbsp->mb_imgsz)) != ((sint_t)(mbsp->mb_imgsz)))
    {
        system_error("move_img2maxpadr1 m2mcopy not ok");
    }
    mbsp->mb_imgpadr = imgtoadr;
    return;
}

//函数:move_lmosimg2maxpadr中检查相应的地址有问题
int adrzone_is_ok(u64_t sadr, u64_t slen, u64_t kadr, u64_t klen)
{
    if (kadr >= sadr && kadr <= (sadr + slen))
    {
        return -1;
    }
    if (kadr <= sadr && ((kadr + klen) >= sadr))
    {
        return -2;
    }

    return 0;
}

int initchkadr_is_ok(machbstart_t *mbsp, u64_t chkadr, u64_t cksz)
{
    //u64_t len=chkadr+cksz;
    if (adrzone_is_ok((mbsp->mb_krlinitstack - mbsp->mb_krlitstacksz), mbsp->mb_krlitstacksz, chkadr, cksz) != 0)
    {
        return -1;
    }
    if (adrzone_is_ok(mbsp->mb_imgpadr, mbsp->mb_imgsz, chkadr, cksz) != 0)
    {
        return -2;
    }
    if (adrzone_is_ok(mbsp->mb_krlimgpadr, mbsp->mb_krlsz, chkadr, cksz) != 0)
    {
        return -3;
    }
    if (adrzone_is_ok(mbsp->mb_bfontpadr, mbsp->mb_bfontsz, chkadr, cksz) != 0)
    {
        return -4;
    }
    if (adrzone_is_ok(mbsp->mb_e820padr, mbsp->mb_e820sz, chkadr, cksz) != 0)
    {
        return -5;
    }
    if (adrzone_is_ok(mbsp->mb_memznpadr, mbsp->mb_memznsz, chkadr, cksz) != 0)
    {
        return -6;
    }
    if (adrzone_is_ok(mbsp->mb_memmappadr, mbsp->mb_memmapsz, chkadr, cksz) != 0)
    {
        return -7;
    }
    if (adrzone_is_ok(mbsp->mb_e820expadr, mbsp->mb_e820exsz, chkadr, cksz) != 0)
    {
        return -8;
    }
    if ((chkadr + cksz) >= mbsp->mb_kpmapphymemsz)
    {
        return -9;
    }
    return 0;
}


#pragma GCC push_options
#pragma GCC optimize("O0")
void die(u32_t dt)
{

    u32_t dttt = dt, dtt = dt;
    if (dt == 0)
    {
        for (;;)
            ;
    }

    for (u32_t i = 0; i < dt; i++)
    {
        for (u32_t j = 0; j < dtt; j++)
        {
            for (u32_t k = 0; k < dttt; k++)
            {
                ;
            }
        }
    }

    return;
}

#pragma GCC pop_options