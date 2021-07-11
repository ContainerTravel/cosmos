/**********************************************************
        cp15协处理器芯片操作头文件halmmu.h
***********************************************************
                彭东 ＠ 2013.09.10.21.40
**********************************************************/
#ifndef _HALMMU_H
#define _HALMMU_H
void mmudsc_t_init(mmudsc_t* init);
msadsc_t* mmu_new_tdirearr(mmudsc_t* mmulocked);
bool_t mmu_del_tdirearr(mmudsc_t* mmulocked, tdirearr_t* tdirearr, msadsc_t* msa);
msadsc_t* mmu_new_sdirearr(mmudsc_t* mmulocked);
bool_t mmu_del_sdirearr(mmudsc_t* mmulocked, sdirearr_t* sdirearr, msadsc_t* msa);
msadsc_t* mmu_new_idirearr(mmudsc_t* mmulocked);
bool_t mmu_del_idirearr(mmudsc_t* mmulocked, idirearr_t* idirearr, msadsc_t* msa);
msadsc_t* mmu_new_mdirearr(mmudsc_t* mmulocked);
bool_t mmu_del_mdirearr(mmudsc_t* mmulocked, mdirearr_t* mdirearr, msadsc_t* msa);
adr_t mmu_untransform_msa(mmudsc_t* mmulocked, mdirearr_t* mdirearr, adr_t vadrs);
bool_t mmu_transform_msa(mmudsc_t* mmulocked, mdirearr_t* mdirearr, adr_t vadrs, adr_t padrs, u64_t flags);
bool_t mmu_untransform_mdire(mmudsc_t* mmulocked, idirearr_t* idirearr, msadsc_t* msa, adr_t vadrs);
mdirearr_t* mmu_transform_mdire(mmudsc_t* mmulocked, idirearr_t* idirearr, adr_t vadrs, u64_t flags, msadsc_t** outmsa);
bool_t mmu_untransform_idire(mmudsc_t* mmulocked, sdirearr_t* sdirearr, msadsc_t* msa, adr_t vadrs);
idirearr_t* mmu_transform_idire(mmudsc_t* mmulocked, sdirearr_t* sdirearr, adr_t vadrs, u64_t flags, msadsc_t** outmsa);
bool_t mmu_untransform_sdire(mmudsc_t* mmulocked, tdirearr_t* tdirearr, msadsc_t* msa, adr_t vadrs);
sdirearr_t* mmu_transform_sdire(mmudsc_t* mmulocked, tdirearr_t* tdirearr, adr_t vadrs, u64_t flags, msadsc_t** outmsa);
bool_t hal_mmu_transform_core(mmudsc_t* mmu, adr_t vadrs, adr_t padrs, u64_t flags);
bool_t hal_mmu_transform(mmudsc_t* mmu, adr_t vadrs, adr_t padrs, u64_t flags);
adr_t mmu_find_msaadr(mdirearr_t* mdirearr, adr_t vadrs);
mdirearr_t* mmu_find_mdirearr(idirearr_t* idirearr, adr_t vadrs);
idirearr_t* mmu_find_idirearr(sdirearr_t* sdirearr, adr_t vadrs);
sdirearr_t* mmu_find_sdirearr(tdirearr_t* tdirearr, adr_t vadrs);
adr_t hal_mmu_untransform_core(mmudsc_t* mmu, adr_t vadrs);
adr_t hal_mmu_untransform(mmudsc_t* mmu, adr_t vadrs);
void hal_mmu_load(mmudsc_t* mmu);
void hal_mmu_refresh();
bool_t hal_mmu_init(mmudsc_t* mmu);
bool_t mmu_clean_mdirearrmsas(mmudsc_t* mmulocked);
bool_t mmu_clean_idirearrmsas(mmudsc_t* mmulocked);
bool_t mmu_clean_sdirearrmsas(mmudsc_t* mmulocked);
bool_t mmu_clean_tdirearrmsas(mmudsc_t* mmulocked);
bool_t hal_mmu_clean(mmudsc_t* mmu);
void dump_mmu(mmudsc_t* dump);


KLINE uint_t mmu_tdire_index(adr_t vadrs)
{
	return (uint_t)((vadrs >> TDIRE_IV_RSHTBIT) & TDIRE_IV_BITMASK);
}

KLINE uint_t mmu_sdire_index(adr_t vadrs)
{
	return (uint_t)((vadrs >> SDIRE_IV_RSHTBIT) & SDIRE_IV_BITMASK);
}

KLINE uint_t mmu_idire_index(adr_t vadrs)
{
	return (uint_t)((vadrs >> IDIRE_IV_RSHTBIT) & IDIRE_IV_BITMASK);
}

KLINE uint_t mmu_mdire_index(adr_t vadrs)
{
	return (uint_t)((vadrs >> MDIRE_IV_RSHTBIT) & MDIRE_IV_BITMASK);
}


KLINE void cr3s_t_init(cr3s_t* init)
{
    if(NULL == init)
    {
        return;
    }
    init->c3s_entry = 0;
    return;
}

KLINE bool_t sdirearr_is_allzero(sdirearr_t* sdirearr)
{
    for(uint_t i = 0; i < SDIRE_MAX; i++)
    {
        if(0 != sdirearr->sde_arr[i].s_entry)
        {
            return FALSE;
        }
    }
    return TRUE;
}

KLINE bool_t sdire_is_have(tdire_t* tdire)
{
    if(0 < tdire->t_flags.t_sdir)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE bool_t sdire_is_presence(tdire_t* tdire)
{
    if(1 == tdire->t_flags.t_p)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE adr_t sdire_ret_padr(tdire_t* tdire)
{
    return (adr_t)(tdire->t_flags.t_sdir << SDIRE_PADR_LSHTBIT);
}

KLINE adr_t sdire_ret_vadr(tdire_t* tdire)
{
    return phyadr_to_viradr(sdire_ret_padr(tdire));
}

KLINE sdirearr_t* tdire_ret_sdirearr(tdire_t* tdire)
{
    return (sdirearr_t*)(sdire_ret_vadr(tdire));
}

KLINE bool_t idirearr_is_allzero(idirearr_t* idirearr)
{
    for(uint_t i = 0; i < IDIRE_MAX; i++)
    {
        if(0 != idirearr->ide_arr[i].i_entry)
        {
            return FALSE;
        }
    }
    return TRUE;
}

KLINE bool_t idire_is_have(sdire_t* sdire)
{
    if(0 < sdire->s_flags.s_idir)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE bool_t idire_is_presence(sdire_t* sdire)
{
    if(1 == sdire->s_flags.s_p)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE adr_t idire_ret_padr(sdire_t* sdire)
{
    return (adr_t)(sdire->s_flags.s_idir << IDIRE_PADR_LSHTBIT);
}

KLINE adr_t idire_ret_vadr(sdire_t* sdire)
{
    return phyadr_to_viradr(idire_ret_padr(sdire));
}

KLINE idirearr_t* sdire_ret_idirearr(sdire_t* sdire)
{
    return (idirearr_t*)(idire_ret_vadr(sdire));
}

KLINE bool_t mdirearr_is_allzero(mdirearr_t* mdirearr)
{
    for(uint_t i = 0; i < MDIRE_MAX; i++)
    {
        if(0 != mdirearr->mde_arr[i].m_entry)
        {
            return FALSE;
        }
    }
    return TRUE;
}

KLINE bool_t mdire_is_have(idire_t* idire)
{
    if(0 < idire->i_flags.i_mdir)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE bool_t mdire_is_presence(idire_t* idire)
{
    if(1 == idire->i_flags.i_p)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE adr_t mdire_ret_padr(idire_t* idire)
{
    return (adr_t)(idire->i_flags.i_mdir << MDIRE_PADR_LSHTBIT);
}

KLINE adr_t mdire_ret_vadr(idire_t* idire)
{
    return phyadr_to_viradr(mdire_ret_padr(idire));
}

KLINE mdirearr_t* idire_ret_mdirearr(idire_t* idire)
{
    return (mdirearr_t*)(mdire_ret_vadr(idire));
}

KLINE bool_t mmumsa_is_have(mdire_t* mdire)
{
    if(0 < mdire->m_flags.m_msa)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE bool_t mmumsa_is_presence(mdire_t* mdire)
{
    if(1 == mdire->m_flags.m_p)
    {
        return TRUE;
    }
    return FALSE;
}

KLINE adr_t mmumsa_ret_padr(mdire_t* mdire)
{
    return (adr_t)(mdire->m_flags.m_msa << MSA_PADR_LSHTBIT);
}

KLINE void tdirearr_t_init(tdirearr_t* init)
{
    if(NULL == init)
    {
        return;
    }
    hal_memset((void*)init, 0, sizeof(tdirearr_t));
    return;
}

KLINE void sdirearr_t_init(sdirearr_t* init)
{
    if(NULL == init)
    {
        return;
    }
    hal_memset((void*)init, 0, sizeof(sdirearr_t));
    return;
}

KLINE void idirearr_t_init(idirearr_t* init)
{
    if(NULL == init)
    {
        return;
    }
    hal_memset((void*)init, 0, sizeof(idirearr_t));
    return;
}

KLINE void mdirearr_t_init(mdirearr_t* init)
{
    if(NULL == init)
    {
        return;
    }
    hal_memset((void*)init, 0, sizeof(mdirearr_t));
    return;
}

#endif // HALMMU_H
