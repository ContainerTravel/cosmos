/**********************************************************
        物理内存分割合并文件memdivmer.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void mm_update_memmgrob(uint_t realpnr, uint_t flgs)
{
	cpuflg_t cpuflg;
	if (0 == flgs)
	{
		knl_spinlock_cli(&memmgrob.mo_lock, &cpuflg);
		memmgrob.mo_alocpages += realpnr;
		memmgrob.mo_freepages -= realpnr;
		knl_spinunlock_sti(&memmgrob.mo_lock, &cpuflg);
	}
	if (1 == flgs)
	{
		knl_spinlock_cli(&memmgrob.mo_lock, &cpuflg);
		memmgrob.mo_alocpages -= realpnr;
		memmgrob.mo_freepages += realpnr;
		knl_spinunlock_sti(&memmgrob.mo_lock, &cpuflg);
	}
	return;
}

void mm_update_memarea(memarea_t *malokp, uint_t pgnr, uint_t flgs)
{
	if (NULL == malokp)
	{
		return;
	}
	if (0 == flgs)
	{
		malokp->ma_freepages -= pgnr;
		malokp->ma_allocpages += pgnr;
	}
	if (1 == flgs)
	{
		malokp->ma_freepages += pgnr;
		malokp->ma_allocpages -= pgnr;
	}
	return;
}

KLINE sint_t retn_divoder(uint_t pages)
{
	sint_t pbits = search_64rlbits((uint_t)pages) - 1;
	if (pages & (pages - 1))
	{
		pbits++;
	}
	return pbits;
}

memarea_t *onfrmsa_retn_marea(memmgrob_t *mmobjp, msadsc_t *freemsa, uint_t freepgs)
{

	if (MF_OLKTY_ODER != freemsa->md_indxflgs.mf_olkty || NULL == freemsa->md_odlink)
	{
		return NULL;
	}
	msadsc_t *fmend = (msadsc_t *)freemsa->md_odlink;
	if (((uint_t)(fmend - freemsa) + 1) != freepgs)
	{
		return NULL;
	}
	if (freemsa->md_indxflgs.mf_marty != fmend->md_indxflgs.mf_marty)
	{
		return NULL;
	}

	for (uint_t mi = 0; mi < mmobjp->mo_mareanr; mi++)
	{
		if ((uint_t)(freemsa->md_indxflgs.mf_marty) == mmobjp->mo_mareastat[mi].ma_type)
		{
			return &mmobjp->mo_mareastat[mi];
		}
	}
	return NULL;
}

memarea_t *onmrtype_retn_marea(memmgrob_t *mmobjp, uint_t mrtype)
{
	for (uint_t mi = 0; mi < mmobjp->mo_mareanr; mi++)
	{
		if (mrtype == mmobjp->mo_mareastat[mi].ma_type)
		{
			return &mmobjp->mo_mareastat[mi];
		}
	}
	return NULL;
}

bafhlst_t *onma_retn_maxbafhlst(memarea_t *malckp)
{
	for (s64_t li = (MDIVMER_ARR_LMAX - 1); li >= 0; li--)
	{
		if (malckp->ma_mdmdata.dm_mdmlielst[li].af_fobjnr > 0)
		{
			return &malckp->ma_mdmdata.dm_mdmlielst[li];
		}
	}
	return NULL;
}

msadsc_t *mm_divpages_opmsadsc(msadsc_t *msastat, uint_t mnr)
{
	if (NULL == msastat || 0 == mnr)
	{
		return NULL;
	}
	if ((MF_OLKTY_ODER != msastat->md_indxflgs.mf_olkty &&
		 MF_OLKTY_BAFH != msastat->md_indxflgs.mf_olkty) ||
		NULL == msastat->md_odlink ||
		PAF_NO_ALLOC != msastat->md_phyadrs.paf_alloc)
	{
		system_error("mm_divpages_opmsadsc err1\n");
	}

	msadsc_t *mend = (msadsc_t *)msastat->md_odlink;
	if (MF_OLKTY_BAFH == msastat->md_indxflgs.mf_olkty)
	{
		mend = msastat;
	}
	if (mend < msastat)
	{
		system_error("mm_divpages_opmsadsc err2\n");
	}
	if ((uint_t)((mend - msastat) + 1) != mnr)
	{
		system_error("mm_divpages_opmsadsc err3\n");
	}
	if (msastat->md_indxflgs.mf_uindx > (MF_UINDX_MAX - 1) || mend->md_indxflgs.mf_uindx > (MF_UINDX_MAX - 1) ||
		msastat->md_indxflgs.mf_uindx != mend->md_indxflgs.mf_uindx)
	{
		system_error("mm_divpages_opmsadsc err4");
	}
	if (mend == msastat)
	{
		msastat->md_indxflgs.mf_uindx++;
		msastat->md_phyadrs.paf_alloc = PAF_ALLOC;
		msastat->md_indxflgs.mf_olkty = MF_OLKTY_ODER;
		msastat->md_odlink = mend;
		return msastat;
	}
	msastat->md_indxflgs.mf_uindx++;
	msastat->md_phyadrs.paf_alloc = PAF_ALLOC;
	mend->md_indxflgs.mf_uindx++;
	mend->md_phyadrs.paf_alloc = PAF_ALLOC;
	msastat->md_indxflgs.mf_olkty = MF_OLKTY_ODER;
	msastat->md_odlink = mend;
	return msastat;
}

sint_t mm_merpages_opmsadsc(bafhlst_t *bafh, msadsc_t *freemsa, uint_t freepgs)
{
	if (NULL == bafh || NULL == freemsa || 1 > freepgs)
	{
		return 0;
	}
	if (MF_OLKTY_ODER != freemsa->md_indxflgs.mf_olkty ||
		NULL == freemsa->md_odlink)
	{
		system_error("mm_merpages_opmsadsc err1\n");
	}
	msadsc_t *fmend = (msadsc_t *)freemsa->md_odlink;
	if (fmend < freemsa)
	{
		system_error("mm_merpages_opmsadsc err2\n");
	}
	if (bafh->af_oderpnr != freepgs ||
		((uint_t)(fmend - freemsa) + 1) != freepgs)
	{
		system_error("mm_merpages_opmsadsc err3\n");
	}
	if (PAF_NO_ALLOC == freemsa->md_phyadrs.paf_alloc ||
		1 > freemsa->md_indxflgs.mf_uindx)
	{
		system_error("mm_merpages_opmsadsc err4\n");
	}
	if (PAF_NO_ALLOC == fmend->md_phyadrs.paf_alloc ||
		1 > fmend->md_indxflgs.mf_uindx)
	{
		system_error("mm_merpages_opmsadsc err5\n");
	}
	if (freemsa->md_indxflgs.mf_uindx != fmend->md_indxflgs.mf_uindx)
	{
		system_error("mm_merpages_opmsadsc err6\n");
	}
	if (freemsa == fmend)
	{
		freemsa->md_indxflgs.mf_uindx--;
		if (0 < freemsa->md_indxflgs.mf_uindx)
		{
			return 1;
		}
		freemsa->md_phyadrs.paf_alloc = PAF_NO_ALLOC;
		freemsa->md_indxflgs.mf_olkty = MF_OLKTY_BAFH;
		freemsa->md_odlink = bafh;
		return 2;
	}
	freemsa->md_indxflgs.mf_uindx--;
	fmend->md_indxflgs.mf_uindx--;
	if (0 < freemsa->md_indxflgs.mf_uindx)
	{
		return 1;
	}
	freemsa->md_phyadrs.paf_alloc = PAF_NO_ALLOC;
	fmend->md_phyadrs.paf_alloc = PAF_NO_ALLOC;
	freemsa->md_indxflgs.mf_olkty = MF_OLKTY_ODER;
	freemsa->md_odlink = fmend;
	fmend->md_indxflgs.mf_olkty = MF_OLKTY_BAFH;
	fmend->md_odlink = bafh;
	return 2;
}

bool_t onmpgs_retn_bafhlst(memarea_t *malckp, uint_t pages, bafhlst_t **retrelbafh, bafhlst_t **retdivbafh)
{
	if (NULL == malckp || 1 > pages || NULL == retrelbafh || NULL == retdivbafh)
	{
		return FALSE;
	}
	bafhlst_t *bafhstat = malckp->ma_mdmdata.dm_mdmlielst;
	sint_t dividx = retn_divoder(pages);
	if (0 > dividx || MDIVMER_ARR_LMAX <= dividx)
	{
		*retrelbafh = NULL;
		*retdivbafh = NULL;
		return FALSE;
	}
	if (pages > bafhstat[dividx].af_oderpnr)
	{ 
		*retrelbafh = NULL;
		*retdivbafh = NULL;
		return FALSE;
	}
	for (sint_t idx = dividx; idx < MDIVMER_ARR_LMAX; idx++)
	{
		if (bafhstat[idx].af_oderpnr >= pages && 0 < bafhstat[idx].af_fobjnr)
		{
			*retrelbafh = &bafhstat[dividx];
			*retdivbafh = &bafhstat[idx];
			return TRUE;
		}
	}

	*retrelbafh = NULL;
	*retdivbafh = NULL;
	return FALSE;
}

bool_t onfpgs_retn_bafhlst(memarea_t *malckp, uint_t freepgs, bafhlst_t **retrelbf, bafhlst_t **retmerbf)
{
	if (NULL == malckp || 1 > freepgs || NULL == retrelbf || NULL == retmerbf)
	{
		return FALSE;
	}
	bafhlst_t *bafhstat = malckp->ma_mdmdata.dm_mdmlielst;
	sint_t dividx = retn_divoder(freepgs);
	if (0 > dividx || MDIVMER_ARR_LMAX <= dividx)
	{
		*retrelbf = NULL;
		*retmerbf = NULL;
		return FALSE;
	}
	if ((~0UL) <= bafhstat[dividx].af_mobjnr)
	{
		system_error("onfpgs_retn_bafhlst af_mobjnr max");
	}
	if ((~0UL) <= bafhstat[dividx].af_fobjnr)
	{
		system_error("onfpgs_retn_bafhlst af_fobjnr max");
	}

	if (freepgs != bafhstat[dividx].af_oderpnr)
	{
		*retrelbf = NULL;
		*retmerbf = NULL;
		return FALSE;
	}
	*retrelbf = &bafhstat[dividx];
	*retmerbf = &bafhstat[MDIVMER_ARR_LMAX - 1];
	return TRUE;
}

msadsc_t *mm_divipages_onbafhlst(bafhlst_t *bafhp)
{
	if (NULL == bafhp)
	{
		return NULL;
	}
	if (1 > bafhp->af_fobjnr)
	{
		return NULL;
	}
	if (list_is_empty_careful(&bafhp->af_frelst) == TRUE)
	{
		return NULL;
	}
	msadsc_t *tmp = list_entry(bafhp->af_frelst.next, msadsc_t, md_list);
	list_del(&tmp->md_list);
	tmp->md_indxflgs.mf_uindx++;
	tmp->md_phyadrs.paf_alloc = PAF_ALLOC;
	bafhp->af_fobjnr--;
	bafhp->af_mobjnr--;
	bafhp->af_alcindx++;
	return tmp;
}

bool_t mm_retnmsaob_onbafhlst(bafhlst_t *bafhp, msadsc_t **retmstat, msadsc_t **retmend)
{
	if (NULL == bafhp || NULL == retmstat || NULL == retmend)
	{
		return FALSE;
	}
	if (1 > bafhp->af_mobjnr || 1 > bafhp->af_fobjnr)
	{
		*retmstat = NULL;
		*retmend = NULL;
		return FALSE;
	}
	if (list_is_empty_careful(&bafhp->af_frelst) == TRUE)
	{
		*retmstat = NULL;
		*retmend = NULL;
		return FALSE;
	}
	msadsc_t *tmp = list_entry(bafhp->af_frelst.next, msadsc_t, md_list);
	list_del(&tmp->md_list);
	bafhp->af_mobjnr--;
	bafhp->af_fobjnr--;
	bafhp->af_freindx++;
	*retmstat = tmp;
	*retmend = (msadsc_t *)tmp->md_odlink;
	if (MF_OLKTY_BAFH == tmp->md_indxflgs.mf_olkty)
	{
		*retmend = tmp;
	}
	return TRUE;
}

bool_t scan_mapgsalloc_ok(memarea_t *malckp, uint_t pages)
{
	if (NULL == malckp || 1 > pages)
	{
		return FALSE;
	}
	if (malckp->ma_freepages >= pages && malckp->ma_maxpages >= pages)
	{
		return TRUE;
	}
	return FALSE;
}

msadsc_t *mm_maxdivpages_onmarea(memarea_t *malckp, uint_t *retrelpnr)
{
	bafhlst_t *bafhp = onma_retn_maxbafhlst(malckp);
	if (NULL == bafhp)
	{
		*retrelpnr = 0;
		return NULL;
	}
	msadsc_t *retmsa = NULL;

	msadsc_t *retmstat = NULL, *retmend = NULL;
	bool_t rets = mm_retnmsaob_onbafhlst(bafhp, &retmstat, &retmend);
	if (FALSE == rets || NULL == retmstat || NULL == retmend)
	{
		*retrelpnr = 0;
		return NULL;
	}
	retmsa = mm_divpages_opmsadsc(retmstat, bafhp->af_oderpnr);

	if (NULL == retmsa)
	{
		*retrelpnr = 0;
		return NULL;
	}
	*retrelpnr = bafhp->af_oderpnr;
	return retmsa;
}

uint_t chek_divlenmsa(msadsc_t *msastat, msadsc_t *msaend, uint_t mnr)
{
	uint_t ok = 0;
	msadsc_t *ms = msastat, *me = msaend;
	if (NULL == msastat || NULL == msaend || 0 == mnr)
	{
		return 0;
	}
	if ((uint_t)(msaend - msastat + 1) != mnr)
	{
		return 0;
	}
	if (1 == mnr)
	{
		if (0 < msastat->md_indxflgs.mf_uindx)
		{
			return 0;
		}
		if (PAF_NO_ALLOC != msastat->md_phyadrs.paf_alloc)
		{
			return 0;
		}
		if (list_is_empty_careful(&msastat->md_list) == FALSE)
		{
			return 0;
		}
		return ok + 1;
	}
	for (; ms < me; ms++)
	{
		if (ms->md_indxflgs.mf_uindx != 0 ||
			(ms + 1)->md_indxflgs.mf_uindx != 0)
		{
			return 0;
		}
		if (PAF_NO_ALLOC != ms->md_phyadrs.paf_alloc ||
			PAF_NO_ALLOC != (ms + 1)->md_phyadrs.paf_alloc)
		{
			return 0;
		}
		if (list_is_empty_careful(&ms->md_list) == FALSE || list_is_empty_careful(&((ms + 1)->md_list)) == FALSE)
		{
			return 0;
		}
		if (PAGESIZE != (((ms + 1)->md_phyadrs.paf_padrs << PSHRSIZE) - (ms->md_phyadrs.paf_padrs << PSHRSIZE)))
		{
			return 0;
		}
		ok++;
	}
	if (0 == ok)
	{
		return 0;
	}
	if ((ok + 1) != mnr)
	{
		return 0;
	}
	return ok;
}

bool_t mrdmb_add_msa_bafh(bafhlst_t *bafhp, msadsc_t *msastat, msadsc_t *msaend)
{
	if (NULL == bafhp || NULL == msastat || NULL == msaend)
	{
		return FALSE;
	}
	uint_t mnr = (msaend - msastat) + 1;
	if (mnr != (uint_t)bafhp->af_oderpnr)
	{
		return FALSE;
	}
	if (0 < msastat->md_indxflgs.mf_uindx || 0 < msaend->md_indxflgs.mf_uindx)
	{
		return FALSE;
	}
	if (PAF_NO_ALLOC != msastat->md_phyadrs.paf_alloc ||
		PAF_NO_ALLOC != msaend->md_phyadrs.paf_alloc)
	{
		return FALSE;
	}

	if (msastat == msaend && 1 != mnr && 1 != bafhp->af_oderpnr)
	{
		return FALSE;
	}
	msastat->md_indxflgs.mf_olkty = MF_OLKTY_ODER;
	msastat->md_odlink = msaend;

	msaend->md_indxflgs.mf_olkty = MF_OLKTY_BAFH;
	msaend->md_odlink = bafhp;
	list_add(&msastat->md_list, &bafhp->af_frelst);
	bafhp->af_mobjnr++;
	bafhp->af_fobjnr++;
	return TRUE;
}

msadsc_t *mm_reldpgsdivmsa_bafhl(memarea_t *malckp, uint_t pages, uint_t *retrelpnr, bafhlst_t *relbfl, bafhlst_t *divbfl)
{
	msadsc_t *retmsa = NULL;
	bool_t rets = FALSE;
	msadsc_t *retmstat = NULL, *retmend = NULL;
	if (NULL == malckp || 1 > pages || NULL == retrelpnr || NULL == relbfl || NULL == divbfl)
	{
		return NULL;
	}
	if (relbfl > divbfl)
	{
		*retrelpnr = 0;
		return NULL;
	}
	if (relbfl == divbfl)
	{
		rets = mm_retnmsaob_onbafhlst(relbfl, &retmstat, &retmend);
		if (FALSE == rets || NULL == retmstat || NULL == retmend)
		{
			*retrelpnr = 0;
			return NULL;
		}
		if ((uint_t)((retmend - retmstat) + 1) != relbfl->af_oderpnr)
		{
			*retrelpnr = 0;
			return NULL;
		}
		retmsa = mm_divpages_opmsadsc(retmstat, relbfl->af_oderpnr);
		if (NULL == retmsa)
		{
			*retrelpnr = 0;
			return NULL;
		}
		*retrelpnr = relbfl->af_oderpnr;
		return retmsa;
	}
	rets = mm_retnmsaob_onbafhlst(divbfl, &retmstat, &retmend);
	if (FALSE == rets || NULL == retmstat || NULL == retmend)
	{
		*retrelpnr = 0;
		return NULL;
	}
	if ((uint_t)((retmend - retmstat) + 1) != divbfl->af_oderpnr)
	{
		*retrelpnr = 0;
		return NULL;
	}
	uint_t divnr = divbfl->af_oderpnr;
	for (bafhlst_t *tmpbfl = divbfl - 1; tmpbfl >= relbfl; tmpbfl--)
	{
		if (mrdmb_add_msa_bafh(tmpbfl, &retmstat[tmpbfl->af_oderpnr], (msadsc_t *)retmstat->md_odlink) == FALSE)
		{
			system_error("mrdmb_add_msa_bafh fail\n");
		}
		retmstat->md_odlink = &retmstat[tmpbfl->af_oderpnr - 1];
		divnr -= tmpbfl->af_oderpnr;
	}

	retmsa = mm_divpages_opmsadsc(retmstat, divnr);
	if (NULL == retmsa)
	{
		*retrelpnr = 0;
		return NULL;
	}
	*retrelpnr = relbfl->af_oderpnr;
	return retmsa;
}

msadsc_t *mm_reldivpages_onmarea(memarea_t *malckp, uint_t pages, uint_t *retrelpnr)
{
	if (NULL == malckp || 1 > pages || NULL == retrelpnr)
	{
		return NULL;
	}
	if (scan_mapgsalloc_ok(malckp, pages) == FALSE)
	{
		*retrelpnr = 0;
		return NULL;
	}
	bafhlst_t *retrelbhl = NULL, *retdivbhl = NULL;
	bool_t rets = onmpgs_retn_bafhlst(malckp, pages, &retrelbhl, &retdivbhl);
	if (FALSE == rets)
	{
		*retrelpnr = 0;
		return NULL;
	}
	uint_t retpnr = 0;
	msadsc_t *retmsa = mm_reldpgsdivmsa_bafhl(malckp, pages, &retpnr, retrelbhl, retdivbhl);
	if (NULL == retmsa)
	{
		*retrelpnr = 0;
		return NULL;
	}
	*retrelpnr = retpnr;
	return retmsa;
}

msadsc_t *mm_prcdivpages_onmarea(memarea_t *malckp, uint_t pages, uint_t *retrelpnr)
{
	if (NULL == malckp || NULL == retrelpnr || 1 != pages)
	{
		return NULL;
	}
	if (MA_TYPE_PROC != malckp->ma_type)
	{
		*retrelpnr = 0;
		return NULL;
	}
	if (scan_mapgsalloc_ok(malckp, pages) == FALSE)
	{
		*retrelpnr = 0;
		return NULL;
	}
	bafhlst_t *prcbfh = &malckp->ma_mdmdata.dm_onemsalst;
	bool_t rets = FALSE;
	msadsc_t *retmsa = NULL, *retmstat = NULL, *retmend = NULL;
	rets = mm_retnmsaob_onbafhlst(prcbfh, &retmstat, &retmend);
	if (FALSE == rets || NULL == retmstat || NULL == retmend)
	{
		*retrelpnr = 0;
		return NULL;
	}
	if ((uint_t)((retmend - retmstat) + 1) != prcbfh->af_oderpnr)
	{
		*retrelpnr = 0;
		return NULL;
	}
	retmsa = mm_divpages_opmsadsc(retmstat, prcbfh->af_oderpnr);
	if (NULL == retmsa)
	{
		*retrelpnr = 0;
		return NULL;
	}
	*retrelpnr = prcbfh->af_oderpnr;
	return retmsa;
}

msadsc_t *mm_divpages_core(memarea_t *mareap, uint_t pages, uint_t *retrealpnr, uint_t flgs)
{
	uint_t retpnr = 0;
	msadsc_t *retmsa = NULL; //,*tmpmsa=NULL;
	cpuflg_t cpuflg;
	if (DMF_RELDIV != flgs && DMF_MAXDIV != flgs)
	{
		*retrealpnr = 0;
		return NULL;
	}
	if (MA_TYPE_KRNL != mareap->ma_type && MA_TYPE_HWAD != mareap->ma_type)
	{
		*retrealpnr = 0;
		return NULL;
	}
	knl_spinlock_cli(&mareap->ma_lock, &cpuflg);

	if (DMF_MAXDIV == flgs)
	{
		retmsa = mm_maxdivpages_onmarea(mareap, &retpnr);
		goto ret_step;
	}
	if (DMF_RELDIV == flgs)
	{
		retmsa = mm_reldivpages_onmarea(mareap, pages, &retpnr);
		goto ret_step;
	}
	retmsa = NULL;
	retpnr = 0;
ret_step:
	if (NULL != retmsa && 0 != retpnr)
	{
		mm_update_memarea(mareap, retpnr, 0);
		mm_update_memmgrob(retpnr, 0);
	}
	knl_spinunlock_sti(&mareap->ma_lock, &cpuflg);
	*retrealpnr = retpnr;
	return retmsa;
}

msadsc_t *mm_divpages_fmwk(memmgrob_t *mmobjp, uint_t pages, uint_t *retrelpnr, uint_t mrtype, uint_t flgs)
{

	memarea_t *marea = onmrtype_retn_marea(mmobjp, mrtype);
	if (NULL == marea)
	{
		*retrelpnr = 0;
		return NULL;
	}
	uint_t retpnr = 0;
	msadsc_t *retmsa = mm_divpages_core(marea, pages, &retpnr, flgs);
	if (NULL == retmsa)
	{
		*retrelpnr = 0;
		return NULL;
	}
	*retrelpnr = retpnr;
	return retmsa;
}

msadsc_t *mm_division_pages(memmgrob_t *mmobjp, uint_t pages, uint_t *retrealpnr, uint_t mrtype, uint_t flgs)
{
	if (NULL == mmobjp || NULL == retrealpnr || 0 == mrtype)
	{
		return NULL;
	}

	uint_t retpnr = 0;
	msadsc_t *retmsa = mm_divpages_fmwk(mmobjp, pages, &retpnr, mrtype, flgs);
	if (NULL == retmsa)
	{
		*retrealpnr = 0;
		return NULL;
	}
	*retrealpnr = retpnr;
	return retmsa;
}

memarea_t *retn_procmarea(memmgrob_t *mmobjp)
{
	if (NULL == mmobjp)
	{
		return NULL;
	}
	for (uint_t mi = 0; mi < mmobjp->mo_mareanr; mi++)
	{
		if (MA_TYPE_PROC == mmobjp->mo_mareastat[mi].ma_type)
		{
			return &mmobjp->mo_mareastat[mi];
		}
	}
	return NULL;
}

msadsc_t *divpages_procmarea_core(memmgrob_t *mmobjp, uint_t pages, uint_t *retrealpnr)
{
	cpuflg_t cpuflg;
	uint_t retpnr = 0;						  
	msadsc_t *retmsa = NULL, *retmsap = NULL; 
	if (NULL == mmobjp || 1 != pages || NULL == retrealpnr)
	{
		return NULL;
	}
	memarea_t *marp = retn_procmarea(mmobjp);
	if (NULL == marp)
	{
		*retrealpnr = 0;
		return NULL;
	}
	knl_spinlock_cli(&marp->ma_lock, &cpuflg);
	if (scan_mapgsalloc_ok(marp, pages) == FALSE)
	{
		retmsap = NULL;
		retpnr = 0;
		goto ret_step;
	}
	retmsa = mm_prcdivpages_onmarea(marp, pages, &retpnr);

	if (NULL != retmsa && 0 != retpnr)
	{
		mm_update_memarea(marp, retpnr, 0);
		mm_update_memmgrob(retpnr, 0);
		retmsap = retmsa;
		goto ret_step;
	}
	retpnr = 0;
	retmsap = NULL;
ret_step:
	knl_spinunlock_sti(&marp->ma_lock, &cpuflg);
	*retrealpnr = retpnr;
	return retmsap;
}

msadsc_t *mm_divpages_procmarea(memmgrob_t *mmobjp, uint_t pages, uint_t *retrealpnr)
{
	msadsc_t *retmsa = NULL;
	uint_t retpnr = 0;
	if (NULL == mmobjp || 1 != pages || NULL == retrealpnr)
	{
		return NULL;
	}
	retmsa = divpages_procmarea_core(mmobjp, pages, &retpnr);
	if (NULL != retmsa)
	{
		*retrealpnr = retpnr;
		return retmsa;
	}
	retmsa = mm_division_pages(mmobjp, pages, &retpnr, MA_TYPE_KRNL, DMF_RELDIV);
	if (NULL == retmsa)
	{
		*retrealpnr = 0;
		return NULL;
	}
	*retrealpnr = retpnr;
	return retmsa;
}

bool_t scan_freemsa_isok(msadsc_t *freemsa, uint_t freepgs)
{
	if (NULL == freemsa || 1 > freepgs)
	{
		return FALSE;
	}
	if (MF_OLKTY_ODER != freemsa->md_indxflgs.mf_olkty ||
		NULL == freemsa->md_odlink || 1 > freemsa->md_indxflgs.mf_uindx)
	{
		return FALSE;
	}
	msadsc_t *end = (msadsc_t *)freemsa->md_odlink;

	if (PAF_ALLOC != freemsa->md_phyadrs.paf_alloc ||
		PAF_ALLOC != end->md_phyadrs.paf_alloc ||
		1 > end->md_indxflgs.mf_uindx)
	{
		return FALSE;
	}
	if (((uint_t)((end - freemsa) + 1)) != freepgs)
	{
		return FALSE;
	}
	return TRUE;
}

sint_t mm_cmsa1blk_isok(bafhlst_t *bafh, msadsc_t *_1ms, msadsc_t *_1me)
{
	if (NULL == bafh || NULL == _1ms || NULL == _1me)
	{
		return 0;
	}
	if (_1me < _1ms)
	{
		return 0;
	}
	if (_1ms == _1me)
	{
		if (MF_OLKTY_BAFH != _1me->md_indxflgs.mf_olkty)
		{
			return 0;
		}
		if (bafh != (bafhlst_t *)_1me->md_odlink)
		{
			return 0;
		}
		if (PAF_NO_ALLOC != _1me->md_phyadrs.paf_alloc)
		{
			return 0;
		}
		if (0 != _1me->md_indxflgs.mf_uindx)
		{
			return 0;
		}
		if ((_1me->md_phyadrs.paf_padrs - _1ms->md_phyadrs.paf_padrs) != (uint_t)(_1me - _1ms))
		{
			return 0;
		}
		return 2;
	}

	if (MF_OLKTY_ODER != _1ms->md_indxflgs.mf_olkty)
	{
		return 0;
	}
	if (_1me != (msadsc_t *)_1ms->md_odlink)
	{
		return 0;
	}
	if (PAF_NO_ALLOC != _1ms->md_phyadrs.paf_alloc)
	{
		return 0;
	}
	if (0 != _1ms->md_indxflgs.mf_uindx)
	{
		return 0;
	}

	if (MF_OLKTY_BAFH != _1me->md_indxflgs.mf_olkty)
	{
		return 0;
	}
	if (bafh != (bafhlst_t *)_1me->md_odlink)
	{
		return 0;
	}
	if (PAF_NO_ALLOC != _1me->md_phyadrs.paf_alloc)
	{
		return 0;
	}
	if (0 != _1me->md_indxflgs.mf_uindx)
	{
		return 0;
	}
	if ((_1me->md_phyadrs.paf_padrs - _1ms->md_phyadrs.paf_padrs) != (uint_t)(_1me - _1ms))
	{
		return 0;
	}
	return 2;
}

sint_t mm_cmsa2blk_isok(bafhlst_t *bafh, msadsc_t *_1ms, msadsc_t *_1me, msadsc_t *_2ms, msadsc_t *_2me)
{
	if (NULL == bafh || NULL == _1ms || NULL == _1me ||
		NULL == _2ms || NULL == _2me ||
		_1ms == _2ms || _1me == _2me)
	{
		return 0;
	}
	sint_t ret1s = 0, ret2s = 0;
	ret1s = mm_cmsa1blk_isok(bafh, _1ms, _1me);
	if (0 == ret1s)
	{
		system_error("mm_cmsa1blk_isok ret1s == 0\n");
	}
	ret2s = mm_cmsa1blk_isok(bafh, _2ms, _2me);
	if (0 == ret2s)
	{
		system_error("mm_cmsa1blk_isok ret2s == 0\n");
	}
	if (2 == ret1s && 2 == ret2s)
	{
		if (_1ms < _2ms && _1me < _2me)
		{
			if ((_1me + 1) != _2ms)
			{
				return 1;
			}
			if ((_1me->md_phyadrs.paf_padrs + 1) != _2ms->md_phyadrs.paf_padrs)
			{
				return 1;
			}
			return 2;
		}
		if (_1ms > _2ms && _1me > _2me)
		{
			if ((_2me + 1) != _1ms)
			{
				return 1;
			}
			if ((_2me->md_phyadrs.paf_padrs + 1) != _1ms->md_phyadrs.paf_padrs)
			{
				return 1;
			}
			return 4;
		}
		return 0;
	}
	return 0;
}

bool_t chek_cl2molkflg(bafhlst_t *bafh, msadsc_t *_1ms, msadsc_t *_1me, msadsc_t *_2ms, msadsc_t *_2me)
{
	if (NULL == bafh || NULL == _1ms || NULL == _1me || NULL == _2ms || NULL == _2me)
	{
		return FALSE;
	}
	if (_1ms == _2ms || _1me == _2me)
	{
		return FALSE;
	}
	if (((uint_t)(_2me - _1ms) + 1) != bafh->af_oderpnr)
	{
		return FALSE;
	}
	if (_1ms == _1me && _2ms == _2me)
	{
		if (MF_OLKTY_ODER != _1ms->md_indxflgs.mf_olkty || (msadsc_t *)_1ms->md_odlink != _2me)
		{
			return FALSE;
		}
		if (MF_OLKTY_BAFH != _2me->md_indxflgs.mf_olkty || (bafhlst_t *)_2me->md_odlink != bafh)
		{
			return FALSE;
		}
		return TRUE;
	}

	if (MF_OLKTY_ODER != _1ms->md_indxflgs.mf_olkty || (msadsc_t *)_1ms->md_odlink != _2me)
	{
		return FALSE;
	}
	if (MF_OLKTY_INIT != _1me->md_indxflgs.mf_olkty || NULL != _1me->md_odlink)
	{
		return FALSE;
	}
	if (MF_OLKTY_INIT != _2ms->md_indxflgs.mf_olkty || NULL != _2ms->md_odlink)
	{
		return FALSE;
	}
	if (MF_OLKTY_BAFH != _2me->md_indxflgs.mf_olkty || (bafhlst_t *)_2me->md_odlink != bafh)
	{
		return FALSE;
	}
	return TRUE;
}

bool_t mm_clear_2msaolflg(bafhlst_t *bafh, msadsc_t *_1ms, msadsc_t *_1me, msadsc_t *_2ms, msadsc_t *_2me)
{
	if (NULL == bafh || NULL == _1ms || NULL == _1me || NULL == _2ms || NULL == _2me)
	{
		return FALSE;
	}
	if (_1ms == _2ms || _1me == _2me)
	{
		return FALSE;
	}

	_1me->md_indxflgs.mf_olkty = MF_OLKTY_INIT;
	_1me->md_odlink = NULL;
	_2ms->md_indxflgs.mf_olkty = MF_OLKTY_INIT;
	_2ms->md_odlink = NULL;
	_1ms->md_indxflgs.mf_olkty = MF_OLKTY_ODER;
	_1ms->md_odlink = _2me;
	_2me->md_indxflgs.mf_olkty = MF_OLKTY_BAFH;
	_2me->md_odlink = bafh;
	return TRUE;
}

sint_t mm_find_cmsa2blk(bafhlst_t *fbafh, msadsc_t **rfnms, msadsc_t **rfnme)
{
	if (NULL == fbafh || NULL == rfnms || NULL == rfnme)
	{
		return 0;
	}
	msadsc_t *freemstat = *rfnms;
	msadsc_t *freemend = *rfnme;
	if (1 > fbafh->af_fobjnr)
	{
		return 1;
	}
	list_h_t *tmplst = NULL;
	msadsc_t *tmpmsa = NULL, *blkms = NULL, *blkme = NULL;
	sint_t rets = 0;
	list_for_each(tmplst, &fbafh->af_frelst)
	{
		tmpmsa = list_entry(tmplst, msadsc_t, md_list);
		rets = mm_cmsa2blk_isok(fbafh, freemstat, freemend, tmpmsa, &tmpmsa[fbafh->af_oderpnr - 1]);
		if (2 == rets || 4 == rets)
		{
			blkms = tmpmsa;
			blkme = &tmpmsa[fbafh->af_oderpnr - 1];
			list_del(&tmpmsa->md_list);
			fbafh->af_fobjnr--;
			fbafh->af_mobjnr--;
			goto step1;
		}
	}
step1:
	if (0 == rets || 1 == rets)
	{
		return 1;
	}
	if (2 == rets)
	{
		if (mm_clear_2msaolflg(fbafh + 1, freemstat, freemend, blkms, blkme) == TRUE)
		{
			if (chek_cl2molkflg(fbafh + 1, freemstat, freemend, blkms, blkme) == FALSE)
			{
				system_error("chek_cl2molkflg err1\n");
			}
			*rfnms = freemstat;
			*rfnme = blkme;
			return 2;
		}
		return 0;
	}
	if (4 == rets)
	{
		if (mm_clear_2msaolflg(fbafh + 1, blkms, blkme, freemstat, freemend) == TRUE)
		{
			if (chek_cl2molkflg(fbafh + 1, blkms, blkme, freemstat, freemend) == FALSE)
			{
				system_error("chek_cl2molkflg err2\n");
			}
			*rfnms = blkms;
			*rfnme = freemend;
			return 2;
		}

		return 0;
	}
	return 0;
}

bool_t mpobf_add_msadsc(bafhlst_t *bafhp, msadsc_t *freemstat, msadsc_t *freemend)
{
	if (NULL == bafhp || NULL == freemstat || NULL == freemend)
	{
		return FALSE;
	}
	if (freemend < freemstat)
	{
		return FALSE;
	}
	if (bafhp->af_oderpnr != ((uint_t)(freemend - freemstat) + 1))
	{
		return FALSE;
	}
	if ((~0UL) <= bafhp->af_fobjnr || (~0UL) <= bafhp->af_mobjnr)
	{
		system_error("(~0UL)<=bafhp->af_fobjnr\n");
		return FALSE;
	}
	freemstat->md_indxflgs.mf_olkty = MF_OLKTY_ODER;
	freemstat->md_odlink = freemend;
	freemend->md_indxflgs.mf_olkty = MF_OLKTY_BAFH;
	freemend->md_odlink = bafhp;
	list_add(&freemstat->md_list, &bafhp->af_frelst);
	bafhp->af_fobjnr++;
	bafhp->af_mobjnr++;
	return TRUE;
}

bool_t mm_merpages_onbafhlst(msadsc_t *freemsa, uint_t freepgs, bafhlst_t *relbf, bafhlst_t *merbf)
{
	sint_t rets = 0;
	msadsc_t *mnxs = freemsa, *mnxe = &freemsa[freepgs - 1];
	bafhlst_t *tmpbf = relbf;
	for (; tmpbf < merbf; tmpbf++)
	{

		rets = mm_find_cmsa2blk(tmpbf, &mnxs, &mnxe);
		if (1 == rets)
		{
			break;
		}
		if (0 == rets)
		{
			system_error("mm_find_cmsa2blk retn 0\n");
		}
	}

	if (mpobf_add_msadsc(tmpbf, mnxs, mnxe) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

bool_t mm_merpages_onmarea(memarea_t *malckp, msadsc_t *freemsa, uint_t freepgs)
{
	if (NULL == malckp || NULL == freemsa || 1 > freepgs)
	{
		return FALSE;
	}

	bafhlst_t *prcbf = NULL;
	sint_t pocs = 0;
	if (MA_TYPE_PROC == malckp->ma_type)
	{
		prcbf = &malckp->ma_mdmdata.dm_onemsalst;
		pocs = mm_merpages_opmsadsc(prcbf, freemsa, freepgs);
		if (2 == pocs)
		{
			if (mpobf_add_msadsc(prcbf, freemsa, &freemsa[freepgs - 1]) == FALSE)
			{
				system_error("mm_merpages_onmarea proc memarea merge fail\n");
			}
			mm_update_memarea(malckp, freepgs, 1);
			mm_update_memmgrob(freepgs, 1);
			return TRUE;
		}
		if (1 == pocs)
		{
			return TRUE;
		}
		if (0 == pocs)
		{
			return FALSE;
		}
		return FALSE;
	}

	bafhlst_t *retrelbf = NULL, *retmerbf = NULL;
	bool_t rets = FALSE;
	rets = onfpgs_retn_bafhlst(malckp, freepgs, &retrelbf, &retmerbf);
	if (FALSE == rets)
	{
		return FALSE;
	}
	if (NULL == retrelbf || NULL == retmerbf)
	{
		return FALSE;
	}
	sint_t mopms = mm_merpages_opmsadsc(retrelbf, freemsa, freepgs);
	if (2 == mopms)
	{
		rets = mm_merpages_onbafhlst(freemsa, freepgs, retrelbf, retmerbf);
		if (TRUE == rets)
		{
			mm_update_memarea(malckp, freepgs, 1);
			mm_update_memmgrob(freepgs, 1);
			return rets;
		}
		return FALSE;
	}
	if (1 == mopms)
	{
		return TRUE;
	}
	if (0 == mopms)
	{
		return FALSE;
	}
	return FALSE;
}

bool_t mm_merpages_core(memarea_t *marea, msadsc_t *freemsa, uint_t freepgs)
{
	if (NULL == marea || NULL == freemsa || 1 > freepgs)
	{
		return FALSE;
	}
	if (scan_freemsa_isok(freemsa, freepgs) == FALSE)
	{
		return FALSE;
	}
	bool_t rets = FALSE;
	cpuflg_t cpuflg;

	knl_spinlock_cli(&marea->ma_lock, &cpuflg);

	rets = mm_merpages_onmarea(marea, freemsa, freepgs);
	knl_spinunlock_sti(&marea->ma_lock, &cpuflg);
	return rets;
}

bool_t mm_merpages_fmwk(memmgrob_t *mmobjp, msadsc_t *freemsa, uint_t freepgs)
{
	memarea_t *marea = onfrmsa_retn_marea(mmobjp, freemsa, freepgs);
	if (NULL == marea)
	{
		return FALSE;
	}

	bool_t rets = mm_merpages_core(marea, freemsa, freepgs);
	if (FALSE == rets)
	{
		return FALSE;
	}
	return rets;
}

bool_t mm_merge_pages(memmgrob_t *mmobjp, msadsc_t *freemsa, uint_t freepgs)
{
	if (NULL == mmobjp || NULL == freemsa || 1 > freepgs)
	{
		return FALSE;
	}
	bool_t rets = mm_merpages_fmwk(mmobjp, freemsa, freepgs);
	if (FALSE == rets)
	{
		return FALSE;
	}
	return rets;
}

void mchkstuc_t_init(mchkstuc_t *initp)
{
	list_init(&initp->mc_list);
	initp->mc_phyadr = 0;
	initp->mc_viradr = 0;
	initp->mc_sz = 0;
	initp->mc_chkval = 0;
	initp->mc_msa = NULL;
	initp->mc_chksadr = NULL;
	initp->mc_chkeadr = NULL;
	return;
}

void write_one_mchkstuc(msadsc_t *msa, uint_t pnr)
{
	if (NULL == msa || 0 == pnr)
	{
		system_error("write_one_mchkstuc msa pnr 0\n");
	}
	u64_t phyadr = msa->md_phyadrs.paf_padrs << PSHRSIZE;
	uint_t viradr = (uint_t)phyadr_to_viradr((adr_t)phyadr);
	uint_t sz = pnr << PSHRSIZE;
	mchkstuc_t *mchks = (mchkstuc_t *)((uint_t)viradr);
	mchkstuc_t_init(mchks);
	mchks->mc_phyadr = phyadr;
	mchks->mc_viradr = viradr;
	mchks->mc_sz = sz;
	mchks->mc_chkval = phyadr;
	mchks->mc_msa = msa;
	mchks->mc_chksadr = (u64_t *)(mchks + 1);
	mchks->mc_chkeadr = (u64_t *)((uint_t)(viradr + sz - 1)); 
	list_add(&mchks->mc_list, &memmgrob.mo_list);
	return;
}

bool_t chek_one_mchks(mchkstuc_t *mchs)
{
	if (NULL == mchs)
	{
		return FALSE;
	}
	msadsc_t *mstat = mchs->mc_msa, *mend = NULL;
	if (MF_OLKTY_ODER != mstat->md_indxflgs.mf_olkty || NULL == mstat->md_odlink)
	{
		kprint("chek_one_mchks 1\n");
		return FALSE;
	}
	mend = (msadsc_t *)mstat->md_odlink;
	if (((uint_t)(mend - mstat) + 1) != (mchs->mc_sz >> PSHRSIZE))
	{
		kprint("chek_one_mchks 2\n");
		return FALSE;
	}
	return TRUE;
}

void cmp_mchkstuc(mchkstuc_t *smchs, mchkstuc_t *dmchs)
{
	if (chek_one_mchks(smchs) == FALSE)
	{
		system_error("chek_one_mchks smchs fail\n");
	}
	if (chek_one_mchks(dmchs) == FALSE)
	{
		system_error("chek_one_mchks dmchs fail\n");
	}
	if (smchs->mc_chkval == dmchs->mc_chkval)
	{
		system_error("cmp_mchkstuc smchschkval==dmchschkval\n");
	}
	return;
}

void free_one_mchkstuc(mchkstuc_t *mchs)
{
	if (NULL == mchs)
	{
		system_error("free_one_mchkstuc mchs NULL\n");
	}
	uint_t relnr = (uint_t)(mchs->mc_sz >> PSHRSIZE);
	if (mm_merge_pages(&memmgrob, mchs->mc_msa, relnr) == FALSE)
	{
		kprint("mm_merge_pages adr:%x,pnr:%x\n", mchs->mc_msa->md_phyadrs.paf_padrs << PSHRSIZE, relnr);
		system_error("free_one_mchkstuc mm_merge_pages ret FALSE\n");
	}
	list_del(&mchs->mc_list);
	return;
}

void free_all_mchkstuc()
{
	list_h_t *tmlst = NULL;
	mchkstuc_t *mchs = NULL;
	list_for_each_head_dell(tmlst, &memmgrob.mo_list)
	{
		mchs = list_entry(tmlst, mchkstuc_t, mc_list);
		free_one_mchkstuc(mchs);
	}
	kprint("free_all_mchkstuc ok\n");
	return;
}

void chek_all_one_mchkstuc(mchkstuc_t *mchs)
{
	mchkstuc_t *mchsp = NULL;
	list_h_t *tmplst;
	list_for_each(tmplst, &memmgrob.mo_list)
	{
		mchsp = list_entry(tmplst, mchkstuc_t, mc_list);
		if (mchs != mchsp)
		{
			cmp_mchkstuc(mchs, mchsp);
		}
	}
	return;
}
void chek_all_mchkstuc()
{
	mchkstuc_t *mchs = NULL;
	list_h_t *tmplst;
	uint_t i = 0;
	list_for_each(tmplst, &memmgrob.mo_list)
	{
		mchs = list_entry(tmplst, mchkstuc_t, mc_list);
		chek_all_one_mchkstuc(mchs);
		i++;
		kprint("检查完第%d个内存空间块:正确.....sz:%d:pgs:%d\n", i, mchs->mc_sz, mchs->mc_sz >> PSHRSIZE);
	}
	kprint("全部检查完毕:正确!!\n");
	return;
}

uint_t retn_test_sec()
{
	memarea_t *mrp = memmgrob.mo_mareastat, *mar = NULL;
	for (uint_t i = 0; i < memmgrob.mo_mareanr; i++)
	{
		if (mrp[i].ma_type == MA_TYPE_KRNL)
		{
			mar = &mrp[i];
			break;
		}
	}
	if (mar == NULL)
	{
		return 0;
	}
	return (((uint_t)(mar->ma_maxpages) / 2) / 60) / 60;
}

void test_proc_marea(memarea_t *mr)
{
	msadsc_t *retmsa = NULL;
	uint_t retpnr = 0, pages = 1;
	u64_t stsc = 0, etsc = 0;

	for (;;)
	{
		if (MA_TYPE_PROC != mr->ma_type)
		{

			break;
		}
		stsc = x86_rdtsc();
		retmsa = mm_divpages_procmarea(&memmgrob, 1, &retpnr);
		etsc = x86_rdtsc();
		if (NULL == retmsa)
		{
			break;
		}
		write_one_mchkstuc(retmsa, retpnr);
		kprint("所分配连续物理内存页面的首地址:%x,连续物理内存页面数:%d,连续物理内存大小:%dMB,CPU时钟周期:%d,PAGES:%d\n",
			   (uint_t)(retmsa->md_phyadrs.paf_padrs << 12), (uint_t)retpnr, (uint_t)((retpnr << 12) >> 20), (uint_t)(etsc - stsc), (uint_t)pages);
	}

	kprint("TEST PROC_MAREA OK\n");
	return;
}

void test_maxone_marea(memarea_t *mr)
{
	msadsc_t *retmsa = NULL;
	uint_t retpnr = 0, pages = 1;
	u64_t stsc = 0, etsc = 0;
	//kprint("mr:%x\n",(uint_t)mr);
	//die(0);
	for (; mr->ma_freepages > pages;)
	{
		/*if(MA_TYPE_PROC==mr->ma_type)
		{
			stsc=x86_rdtsc();
			retmsa=mm_divpages_procmarea(&memmgrob,1,&retpnr);
			etsc=x86_rdtsc();

		}else*/
		{
			stsc = x86_rdtsc();
			retmsa = mm_division_pages(&memmgrob, pages, &retpnr, mr->ma_type, DMF_RELDIV);
			etsc = x86_rdtsc();
		}

		if (NULL != retmsa)
		{
			write_one_mchkstuc(retmsa, retpnr);
			kprint("所分配连续物理内存页面的首地址:%x,连续物理内存页面数:%d,连续物理内存大小:%dMB,CPU时钟周期:%d,PAGES:%d\n",
				   (uint_t)(retmsa->md_phyadrs.paf_padrs << 12), (uint_t)retpnr, (uint_t)((retpnr << 12) >> 20), (uint_t)(etsc - stsc), (uint_t)pages);
		}
		pages++;
	}
	return;
}

void test_onedivmer_all(memarea_t *ma)
{
	uint_t pages = 1, retpnr = 0;
	u64_t stsc = 0, etsc = 0;
	msadsc_t *retmsa = NULL;
	//for(;pages<ma->ma_maxpages;pages++)
	{
		for (; pages < ma->ma_maxpages;)
		{
			stsc = x86_rdtsc();
			retmsa = mm_division_pages(&memmgrob, pages, &retpnr, ma->ma_type, DMF_RELDIV);
			etsc = x86_rdtsc();
			if (NULL == retmsa)
			{
				kprint("in for retmsa==NULL:%x\n", pages);
				break;
			}
			write_one_mchkstuc(retmsa, retpnr);
			kprint("所分配连续物理内存页面的首地址:%x,连续物理内存页面数:%d,连续物理内存大小:%dMB,CPU时钟周期:%d,PAGES:%d\n",
				   (uint_t)(retmsa->md_phyadrs.paf_padrs << 12), (uint_t)retpnr, (uint_t)((retpnr << 12) >> 20), (uint_t)(etsc - stsc), (uint_t)pages);
		}
		chek_all_mchkstuc();
		free_all_mchkstuc();
	}
	kprint("test_onedivmer_all 0k\n");
	return;
}

void test_maxdiv_all()
{
	for (uint_t i = 0; i < 3; i++)
	{
		/*if(MA_TYPE_PROC==memmgrob.mo_mareastat[i].ma_type)
		{
			test_proc_marea(&memmgrob.mo_mareastat[i]);
		}*/
		test_onedivmer_all(&memmgrob.mo_mareastat[i]);
		//test_maxone_marea(&memmgrob.mo_mareastat[i]);
	}
	return;
}


void test_divsion_pages()
{
	//uint_t max=2;
	test_maxdiv_all();
	//chek_all_mchkstuc();
	free_all_mchkstuc();

	/*uint_t pages=1;
	msadsc_t* retmsa=NULL;
	u64_t stsc=0,etsc=0;
	uint_t retpnr=0;
	


	stsc=x86_rdtsc();
	retmsa=mm_division_pages(&memmgrob, pages,&retpnr, MA_TYPE_KRNL,DMF_RELDIV);
	etsc=x86_rdtsc();
	if(NULL==retmsa)
	{
		system_error("test_divsion_pages retmsa NULL\n");
	}
	write_one_mchkstuc(retmsa,retpnr);
	kprint("所分配连续物理内存页面的首地址:%x,连续物理内存页面数:%d,连续物理内存大小:%dMB,CPU时钟周期:%d,PAGES:%d\n",
		((uint_t)retmsa->md_phyadrs.paf_padrs<<12),(uint_t)retpnr,(uint_t)((retpnr<<12)>>20),(uint_t)(etsc-stsc),(uint_t)pages);
	*/
	/*uint_t pi=1;pi<max;pi++*/
	/*uint_t retpnr=0;
	uint_t max=2;
	uint_t pages=1;
	msadsc_t* retmsa=NULL;
	uint_t stsc=0,etsc=0;

	//kprint("下面开始测试内存分配代码的正确性,根据您的内存容量估算测试时间大约为:%d小时\n您可以选择喝杯茶或者看一看窗外的风景.......\n",retn_test_sec());
	//die(0x800);执行内存分配函数所用的
	//for(;pages<2000;pages++)
	{
	for(;;pages++)
	{
		stsc=x86_rdtsc();
		retmsa=mm_division_pages(&memmgrob, pages,&retpnr, MA_TYPE_KRNL,DMF_RELDIV);
		etsc=x86_rdtsc();
		if(NULL==retmsa)
		{
			break;//system_error("test_divsion_pages retmsa NULL\n");
		}
		write_one_mchkstuc(retmsa,retpnr);
		kprint("所分配连续物理内存页面的首地址:%x,连续物理内存页面数:%d,连续物理内存大小:%dMB,CPU时钟周期:%d,PAGES:%d\n",
		retmsa->md_phyadrs.paf_padrs<<12,retpnr,(retpnr<<12)>>20,etsc-stsc,pages);
		if(mm_merge_pages(&memmgrob,retmsa,retpnr,MA_TYPE_KRNL)==FALSE)
		{
			system_error("test_divsion_pages free FALSE\n");
		}
	}
	}*/
	//chek_all_mchkstuc();
	//kprint("剩余物理内存总量:%d页面 :%d逼 :%d兆逼\n",memmgrob.mo_freepages,memmgrob.mo_freepages<<PSHRSIZE,(memmgrob.mo_freepages<<PSHRSIZE)>>20);
	return;
}
