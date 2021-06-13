/**********************************************************
        物理内存管理器初始化文件memmgrinit.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void init_memmgr()
{
	init_msadsc();
	init_memarea();
	init_copy_pagesfvm();
	init_search_krloccupymm(&kmachbsp);
	init_merlove_mem();
	init_memmgrob();

	return;
}

void disp_memmgrob()
{

	test_divsion_pages();
	return;
}

void init_memmgrob()
{
	machbstart_t *mbsp = &kmachbsp;
	memmgrob_t *mobp = &memmgrob;
	memmgrob_t_init(mobp);
	if (NULL == mbsp->mb_e820expadr || 0 == mbsp->mb_e820exnr)
	{
		system_error("mbsp->mb_e820expadr==NULL\n");
	}
	if (NULL == mbsp->mb_memmappadr || 0 == mbsp->mb_memmapnr)
	{
		system_error("mbsp->mb_memmappadr==NULL\n");
	}
	if (NULL == mbsp->mb_memznpadr || 0 == mbsp->mb_memznnr)
	{
		system_error("mbsp->mb_memznpadr==NULL\n");
	}
	mobp->mo_pmagestat = (phymmarge_t *)phyadr_to_viradr((adr_t)mbsp->mb_e820expadr);
	mobp->mo_pmagenr = mbsp->mb_e820exnr;
	mobp->mo_msadscstat = (msadsc_t *)phyadr_to_viradr((adr_t)mbsp->mb_memmappadr);
	mobp->mo_msanr = mbsp->mb_memmapnr;
	mobp->mo_mareastat = (memarea_t *)phyadr_to_viradr((adr_t)mbsp->mb_memznpadr);
	mobp->mo_mareanr = mbsp->mb_memznnr;
	mobp->mo_memsz = mbsp->mb_memmapnr << PSHRSIZE;
	mobp->mo_maxpages = mbsp->mb_memmapnr;
	uint_t aidx = 0;
	for (uint_t i = 0; i < mobp->mo_msanr; i++)
	{
		if (1 == mobp->mo_msadscstat[i].md_indxflgs.mf_uindx &&
			MF_MOCTY_KRNL == mobp->mo_msadscstat[i].md_indxflgs.mf_mocty &&
			PAF_ALLOC == mobp->mo_msadscstat[i].md_phyadrs.paf_alloc)
		{
			aidx++;
		}
	}
	mobp->mo_alocpages = aidx;
	mobp->mo_freepages = mobp->mo_maxpages - mobp->mo_alocpages;
	return;
}

void memmgrob_t_init(memmgrob_t *initp)
{
	list_init(&initp->mo_list);
	knl_spinlock_init(&initp->mo_lock);
	initp->mo_stus = 0;
	initp->mo_flgs = 0;
	initp->mo_memsz = 0;
	initp->mo_maxpages = 0;
	initp->mo_freepages = 0;
	initp->mo_alocpages = 0;
	initp->mo_resvpages = 0;
	initp->mo_horizline = 0;
	initp->mo_pmagestat = NULL;
	initp->mo_pmagenr = 0;
	initp->mo_msadscstat = NULL;
	initp->mo_msanr = 0;
	initp->mo_mareastat = NULL;
	initp->mo_mareanr = 0;
	initp->mo_privp = NULL;
	initp->mo_extp = NULL;
	return;
}

bool_t copy_pages_data(machbstart_t *mbsp)
{

	uint_t topgadr = mbsp->mb_nextwtpadr;

	if (initchkadr_is_ok(mbsp, topgadr, mbsp->mb_subpageslen) != 0)
	{
		return FALSE;
	}

	uint_t *p = (uint_t *)phyadr_to_viradr((adr_t)topgadr);
	uint_t *pdpte = (uint_t *)(((uint_t)p) + 0x1000);
	uint_t *pde = (uint_t *)(((uint_t)p) + 0x2000);
	for (uint_t mi = 0; mi < PGENTY_SIZE; mi++)
	{
		p[mi] = 0;
		pdpte[mi] = 0;
	}
	uint_t adr = 0;
	uint_t pdepd = 0;
	for (uint_t pdei = 0; pdei < 16; pdei++)
	{
		pdepd = (uint_t)viradr_to_phyadr((adr_t)pde);
		pdpte[pdei] = (uint_t)(pdepd | KPDPTE_RW | KPDPTE_P);
		for (uint_t pdeii = 0; pdeii < PGENTY_SIZE; pdeii++)
		{
			pde[pdeii] = 0 | adr | KPDE_PS | KPDE_RW | KPDE_P;
			adr += 0x200000;
		}
		pde = (uint_t *)((uint_t)pde + 0x1000);
	}
	uint_t pdptepd = (uint_t)viradr_to_phyadr((adr_t)pdpte);
	p[((KRNL_VIRTUAL_ADDRESS_START) >> KPML4_SHIFT) & 0x1ff] = (uint_t)(pdptepd | KPML4_RW | KPML4_P);
	p[0] = (uint_t)(pdptepd | KPML4_RW | KPML4_P);
	mbsp->mb_pml4padr = topgadr;
	mbsp->mb_subpageslen = (uint_t)(0x1000 * 16 + 0x2000);
	mbsp->mb_kpmapphymemsz = (uint_t)(0x400000000);
	mbsp->mb_nextwtpadr = PAGE_ALIGN(mbsp->mb_pml4padr + mbsp->mb_subpageslen);
	return TRUE;
}

bool_t copy_fvm_data(machbstart_t *mbsp, dftgraph_t *dgp)
{
	u64_t tofvadr = mbsp->mb_nextwtpadr;
	if (initchkadr_is_ok(mbsp, tofvadr, dgp->gh_fvrmsz) != 0)
	{
		return FALSE;
	}
	sint_t retcl = m2mcopy((void *)((uint_t)dgp->gh_fvrmphyadr), (void *)phyadr_to_viradr((adr_t)(tofvadr)), (sint_t)dgp->gh_fvrmsz);
	if (retcl != (sint_t)dgp->gh_fvrmsz)
	{
		return FALSE;
	}
	dgp->gh_fvrmphyadr = phyadr_to_viradr((adr_t)tofvadr);
	mbsp->mb_fvrmphyadr = tofvadr;
	mbsp->mb_nextwtpadr = PAGE_ALIGN(tofvadr + dgp->gh_fvrmsz);
	return TRUE;
}

void memi_set_mmutabl(uint_t tblpadr, void *edatap)
{
	set_cr3(tblpadr);
	return;
}

void init_copy_pagesfvm()
{
	if (copy_pages_data(&kmachbsp) == FALSE)
	{
		system_error("copy_pages_data fail");
	}
	if (copy_fvm_data(&kmachbsp, &kdftgh) == FALSE)
	{
		system_error("copy_fvm_data fail");
	}
	memi_set_mmutabl(kmachbsp.mb_pml4padr, NULL);
	return;
}

/*void disp_msa(msadsc_t* p)
{
	kprint("msadsc_t sz:%d mf_alcidx:%d mf_pgemax:%d mf_lstty:%d\n",sizeof(*p),p->md_flgs.mf_alcidx,
		p->md_flgs.mf_pgemax,p->md_flgs.mf_lstty);	
	return;
}*/