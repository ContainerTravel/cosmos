/**********************************************************
        内核虚拟地址空间文件kvadrsmem.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void teststc_t_init(teststc_t *initp)
{
	list_init(&initp->tst_list);
	initp->tst_vadr = 0;
	initp->tst_vsiz = 0;
	initp->tst_type = 0;
	initp->tst_lime = 0;
	return;
}

teststc_t *new_teststc()
{
	teststc_t *t = (teststc_t *)kmsob_new(sizeof(teststc_t));
	if (NULL == t)
	{
		return NULL;
	}
	teststc_t_init(t);
	return t;
}

void del_teststc(teststc_t *delstc)
{

	if ((NULL != delstc))
	{
		teststc_t_init(delstc);
		if (TRUE == kmsob_delete((void *)delstc, sizeof(teststc_t)))
		{
			return;
		}
	}
	system_error("del_teststc err\n");
	return;
}

void add_new_teststc(adr_t vadr, size_t vsiz)
{
	if (NULL == vadr || 1 > vsiz)
	{
		system_error("add_new_teststc parm err\n");
	}
	teststc_t *t = NULL;
	t = new_teststc();
	if (NULL == t)
	{
		system_error("add_new_teststc new_teststc NULL\n");
	}
	t->tst_vadr = vadr;
	t->tst_vsiz = vsiz;
	list_add(&t->tst_list, &krlvirmemadrs.kvs_testhead);
	krlvirmemadrs.kvs_tstcnr++;
	return;
}

void vaslknode_t_init(vaslknode_t *initp)
{
	if (NULL == initp)
	{
		system_error("vaslknode_t_init pram err\n");
	}
	initp->vln_color = 0;
	initp->vln_flags = 0;
	initp->vln_left = NULL;
	initp->vln_right = NULL;
	initp->vln_prev = NULL;
	initp->vln_next = NULL;
	return;
}

void pgtabpage_t_init(pgtabpage_t *initp)
{
	knl_spinlock_init(&initp->ptp_lock);
	list_init(&initp->ptp_msalist);
	initp->ptp_msanr = 0;
	return;
}

void virmemadrs_t_init(virmemadrs_t *initp)
{
	if (NULL == initp)
	{
		return;
	}
	knl_spinlock_init(&initp->vs_lock);
	initp->vs_resalin = 0;
	list_init(&initp->vs_list);
	initp->vs_flgs = 0;
	initp->vs_kmvdscnr = 0;
	initp->vs_mm = NULL;
	initp->vs_startkmvdsc = 0;
	initp->vs_endkmvdsc = NULL;
	initp->vs_currkmvdsc = NULL;
	initp->vs_krlmapdsc = NULL;
	initp->vs_krlhwmdsc = NULL;
	initp->vs_krlolddsc = NULL;
	initp->vs_isalcstart = 0;
	initp->vs_isalcend = 0;
	initp->vs_privte = 0;
	initp->vs_ext = 0;
	return;
}

void kmvarsdsc_t_init(kmvarsdsc_t *initp)
{
	if (NULL == initp)
	{
		system_error("kmvarsdsc_t_init pram err\n");
	}
	knl_spinlock_init(&initp->kva_lock);
	initp->kva_maptype = 0;
	list_init(&initp->kva_list);
	initp->kva_flgs = 0;
	initp->kva_limits = 0;
	vaslknode_t_init(&initp->kva_lknode);
	initp->kva_mcstruct = NULL;
	initp->kva_start = 0;
	initp->kva_end = 0;
	initp->kva_kvmbox = NULL;
	initp->kva_kvmcobj = NULL;
	return;
}

void kvirmemadrs_t_init(kvirmemadrs_t *initp)
{
	if (NULL == initp)
	{
		system_error("kvirmemadrs_t_init pram err\n");
	}
	knl_spinlock_init(&initp->kvs_lock);
	initp->kvs_flgs = 0;
	initp->kvs_kmvdscnr = 0;
	initp->kvs_startkmvdsc = NULL;
	initp->kvs_endkmvdsc = NULL;
	initp->kvs_krlmapdsc = NULL;
	initp->kvs_krlhwmdsc = NULL;
	initp->kvs_krlolddsc = NULL;
	initp->kvs_isalcstart = 0;
	initp->kvs_isalcend = 0;
	initp->kvs_privte = NULL;
	initp->kvs_ext = NULL;
	list_init(&initp->kvs_testhead);
	initp->kvs_tstcnr = 0;
	initp->kvs_randnext = 1;
	pgtabpage_t_init(&initp->kvs_ptabpgcs);
	kvmcobjmgr_t_init(&initp->kvs_kvmcomgr);
	kvmemcboxmgr_t_init(&initp->kvs_kvmemcboxmgr);
	return;
}

kmvarsdsc_t *new_kmvarsdsc()
{
	kmvarsdsc_t *kmvdc = NULL;
	kmvdc = (kmvarsdsc_t *)kmsob_new(sizeof(kmvarsdsc_t));
	if (NULL == kmvdc)
	{
		return NULL;
	}
	kmvarsdsc_t_init(kmvdc);
	return kmvdc;
}

bool_t del_kmvarsdsc(kmvarsdsc_t *delkmvd)
{
	if (NULL == delkmvd)
	{
		return FALSE;
	}
	return kmsob_delete((void *)delkmvd, sizeof(kmvarsdsc_t));
}

virmemadrs_t *new_virmemadrs()
{
	virmemadrs_t *vmdsc = NULL;
	vmdsc = (virmemadrs_t *)kmsob_new(sizeof(virmemadrs_t));
	if (NULL == vmdsc)
	{
		return NULL;
	}
	virmemadrs_t_init(vmdsc);
	return vmdsc;
}

bool_t del_virmemadrs(virmemadrs_t *vmdsc)
{
	if (NULL == vmdsc)
	{
		return FALSE;
	}
	return kmsob_delete((void *)vmdsc, sizeof(virmemadrs_t));
}

void kvma_seting_kvirmemadrs(kvirmemadrs_t *kvma)
{
	kmvarsdsc_t *kmvdc = NULL;
	if (NULL == kvma)
	{
		system_error("kvma_seting_kvirmemadrs parm err\n");
	}
	kmvdc = new_kmvarsdsc();
	if (NULL == kmvdc)
	{
		system_error("kvma_seting_kvirmemadrs nomem err\n");
	}
	kvma->kvs_isalcstart = KRNL_VIRTUAL_ADDRESS_START + KRNL_MAP_VIRTADDRESS_SIZE;
	kvma->kvs_isalcend = KRNL_VIRTUAL_ADDRESS_END;
	kmvdc->kva_start = KRNL_VIRTUAL_ADDRESS_START;
	kmvdc->kva_end = KRNL_VIRTUAL_ADDRESS_START + KRNL_MAP_VIRTADDRESS_SIZE;
	kmvdc->kva_mcstruct = kvma;
	kvma->kvs_startkmvdsc = kmvdc;
	kvma->kvs_endkmvdsc = kmvdc;
	kvma->kvs_krlmapdsc = kmvdc;
	kvma->kvs_kmvdscnr++;
	return;
}

bool_t kvma_inituserspace_virmemadrs(virmemadrs_t *vma)
{
	kmvarsdsc_t *kmvdc = NULL, *stackkmvdc = NULL;
	if (NULL == vma)
	{
		return FALSE;
	}
	kmvdc = new_kmvarsdsc();
	if (NULL == kmvdc)
	{
		return FALSE;
	}
	stackkmvdc = new_kmvarsdsc();
	if (NULL == stackkmvdc)
	{
		del_kmvarsdsc(kmvdc);
		return FALSE;
	}
	kmvdc->kva_start = USER_VIRTUAL_ADDRESS_START + 0x1000;
	kmvdc->kva_end = kmvdc->kva_start + 0x4000;
	kmvdc->kva_mcstruct = vma;

	stackkmvdc->kva_start = PAGE_ALIGN(USER_VIRTUAL_ADDRESS_END - 0x40000000);
	stackkmvdc->kva_end = USER_VIRTUAL_ADDRESS_END;
	stackkmvdc->kva_mcstruct = vma;

	knl_spinlock(&vma->vs_lock);
	vma->vs_isalcstart = USER_VIRTUAL_ADDRESS_START;
	vma->vs_isalcend = USER_VIRTUAL_ADDRESS_END;
	vma->vs_startkmvdsc = kmvdc;
	vma->vs_endkmvdsc = stackkmvdc;
	list_add_tail(&kmvdc->kva_list, &vma->vs_list);
	list_add_tail(&stackkmvdc->kva_list, &vma->vs_list);
	vma->vs_kmvdscnr += 2;
	knl_spinunlock(&vma->vs_lock);
	return TRUE;
}
void mmadrsdsc_t_init(mmadrsdsc_t* initp)
{
	if(NULL == initp)
	{
		return;
	}
	knl_spinlock_init(&initp->msd_lock);
	list_init(&initp->msd_list);
	initp->msd_flag = 0;
	initp->msd_stus = 0;
	initp->msd_scount = 0;
	krlsem_t_init(&initp->msd_sem);
	krlsem_set_sem(&initp->msd_sem, SEM_FLG_MUTEX, SEM_MUTEX_ONE_LOCK);
	mmudsc_t_init(&initp->msd_mmu);
	virmemadrs_t_init(&initp->msd_virmemadrs);
	initp->msd_stext = 0;
	initp->msd_etext = 0;
	initp->msd_sdata = 0;
	initp->msd_edata = 0;
	initp->msd_sbss = 0;
	initp->msd_ebss = 0;
	initp->msd_sbrk = 0;
	initp->msd_ebrk = 0;
	return; 
}

void test_vadr()
{
	adr_t vadr = vma_new_vadrs(&initmmadrsdsc, NULL, 0x1000, 0, 0);
	if(NULL == vadr)
	{
		kprint("分配虚拟地址空间失败\n");
	}
	kprint("分配虚拟地址空间地址:%x\n", vadr);
	kprint("开始写入分配虚拟地址空间\n");
	hal_memset((void*)vadr, 0, 0x1000);
	kprint("结束写入分配虚拟地址空间\n");
	return;
}

void init_kvirmemadrs()
{
	mmadrsdsc_t_init(&initmmadrsdsc);
	kvirmemadrs_t_init(&krlvirmemadrs);
	kvma_seting_kvirmemadrs(&krlvirmemadrs);
	kvma_inituserspace_virmemadrs(&initmmadrsdsc.msd_virmemadrs);
	hal_mmu_init(&initmmadrsdsc.msd_mmu);
	hal_mmu_load(&initmmadrsdsc.msd_mmu);
	test_vadr();
	return;
}


kmvarsdsc_t *vma_find_kmvarsdsc_is_ok(virmemadrs_t *vmalocked, kmvarsdsc_t *curr, adr_t start, size_t vassize)
{
	kmvarsdsc_t *nextkmvd = NULL;
	adr_t newend = start + (adr_t)vassize;

	if (list_is_last(&curr->kva_list, &vmalocked->vs_list) == FALSE)
	{
		nextkmvd = list_next_entry(curr, kmvarsdsc_t, kva_list);
		if (NULL == start)
		{
			if ((curr->kva_end + (adr_t)vassize) <= nextkmvd->kva_start)
			{
				return curr;
			}
		}
		else
		{
			if ((curr->kva_end <= start) && (newend <= nextkmvd->kva_start))
			{
				return curr;
			}
		}
	}
	else
	{
		if (NULL == start)
		{
			if ((curr->kva_end + (adr_t)vassize) < vmalocked->vs_isalcend)
			{
				return curr;
			}
		}
		else
		{
			if ((curr->kva_end <= start) && (newend < vmalocked->vs_isalcend))
			{
				return curr;
			}
		}
	}
	return NULL;
}

kmvarsdsc_t *vma_find_kmvarsdsc(virmemadrs_t *vmalocked, adr_t start, size_t vassize)
{
	kmvarsdsc_t *kmvdcurrent = NULL, *curr = vmalocked->vs_currkmvdsc;
	adr_t newend = start + vassize;
	list_h_t *listpos = NULL;
	if (0x1000 > vassize)
	{
		return NULL;
	}

	if (newend > vmalocked->vs_isalcend)
	{
		return NULL;
	}

	if (NULL != curr)
	{
		kmvdcurrent = vma_find_kmvarsdsc_is_ok(vmalocked, curr, start, vassize);
		if (NULL != kmvdcurrent)
		{
			return kmvdcurrent;
		}
	}

	list_for_each(listpos, &vmalocked->vs_list)
	{
		curr = list_entry(listpos, kmvarsdsc_t, kva_list);
		kmvdcurrent = vma_find_kmvarsdsc_is_ok(vmalocked, curr, start, vassize);
		if (NULL != kmvdcurrent)
		{
			return kmvdcurrent;
		}
	}
	return NULL;
}

adr_t vma_new_vadrs_core(mmadrsdsc_t *mm, adr_t start, size_t vassize, u64_t vaslimits, u32_t vastype)
{
	adr_t retadrs = NULL;
	kmvarsdsc_t *newkmvd = NULL, *currkmvd = NULL;
	virmemadrs_t *vma = &mm->msd_virmemadrs;
	knl_spinlock(&vma->vs_lock);

	currkmvd = vma_find_kmvarsdsc(vma, start, vassize);
	if (NULL == currkmvd)
	{
		retadrs = NULL;
		goto out;
	}

	if (((NULL == start) || (start == currkmvd->kva_end)) && (vaslimits == currkmvd->kva_limits) && (vastype == currkmvd->kva_maptype))
	{
		retadrs = currkmvd->kva_end;
		currkmvd->kva_end += vassize;
		vma->vs_currkmvdsc = currkmvd;
		goto out;
	}

	newkmvd = new_kmvarsdsc();
	if (NULL == newkmvd)
	{
		retadrs = NULL;
		goto out;
	}

	if (NULL == start)
	{
		newkmvd->kva_start = currkmvd->kva_end;
	}
	else
	{
		newkmvd->kva_start = start;
	}

	newkmvd->kva_end = newkmvd->kva_start + vassize;
	newkmvd->kva_limits = vaslimits;
	newkmvd->kva_maptype = vastype;
	newkmvd->kva_mcstruct = vma;
	vma->vs_currkmvdsc = newkmvd;
	list_add(&newkmvd->kva_list, &currkmvd->kva_list);
	if (list_is_last(&newkmvd->kva_list, &vma->vs_list) == TRUE)
	{
		vma->vs_endkmvdsc = newkmvd;
	}

	retadrs = newkmvd->kva_start;
out:
	knl_spinunlock(&vma->vs_lock);
	return retadrs;
}

adr_t vma_new_vadrs(mmadrsdsc_t *mm, adr_t start, size_t vassize, u64_t vaslimits, u32_t vastype)
{
	if (NULL == mm || 1 > vassize)
	{
		return NULL;
	}
	if (NULL != start)
	{
		if (((start & 0xfff) != 0) || (0x1000 > start) || (USER_VIRTUAL_ADDRESS_END < (start + vassize)))
		{
			return NULL;
		}
	}

	return vma_new_vadrs_core(mm, start, VADSZ_ALIGN(vassize), vaslimits, vastype);
}

kmvarsdsc_t *vma_del_find_kmvarsdsc(virmemadrs_t *vmalocked, adr_t start, size_t vassize)
{
	kmvarsdsc_t *curr = vmalocked->vs_currkmvdsc;
	adr_t newend = start + (adr_t)vassize;
	list_h_t *listpos = NULL;
	if (0x1000 > vassize)
	{
		return NULL;
	}

	if (NULL != curr)
	{
		if ((curr->kva_start) <= start && (newend <= curr->kva_end))
		{
			return curr;
		}
	}
	list_for_each(listpos, &vmalocked->vs_list)
	{
		curr = list_entry(listpos, kmvarsdsc_t, kva_list);
		if ((start >= curr->kva_start) && (newend <= curr->kva_end))
		{
			return curr;
		}
	}
	return NULL;
}

void vma_del_set_endcurrkmvd(virmemadrs_t *vmalocked, kmvarsdsc_t *del)
{
	kmvarsdsc_t *prevkmvd = NULL, *nextkmvd = NULL;
	if (list_is_last(&del->kva_list, &vmalocked->vs_list) == TRUE)
	{
		if (list_is_first(&del->kva_list, &vmalocked->vs_list) == FALSE)
		{
			prevkmvd = list_prev_entry(del, kmvarsdsc_t, kva_list);
			vmalocked->vs_endkmvdsc = prevkmvd;
			vmalocked->vs_currkmvdsc = prevkmvd;
		}
		else
		{
			vmalocked->vs_endkmvdsc = NULL;
			vmalocked->vs_currkmvdsc = NULL;
		}
	}
	else
	{
		nextkmvd = list_next_entry(del, kmvarsdsc_t, kva_list);
		vmalocked->vs_currkmvdsc = nextkmvd;
	}
	return;
}

bool_t vma_del_unmapping_phyadrs(mmadrsdsc_t *mm, kmvarsdsc_t *kmvd, adr_t start, adr_t end)
{
	adr_t phyadrs;
	bool_t rets = TRUE;
	mmudsc_t *mmu = &mm->msd_mmu;
	kvmemcbox_t *kmbox = kmvd->kva_kvmbox;

	for (adr_t vadrs = start; vadrs < end; vadrs += VMAP_MIN_SIZE)
	{
		phyadrs = hal_mmu_untransform(mmu, vadrs);
		if (NULL != phyadrs && NULL != kmbox)
		{
			if (vma_del_usermsa(mm, kmbox, NULL, phyadrs) == FALSE)
			{
				rets = FALSE;
			}
		}
	}

	return rets;
}

bool_t vma_del_unmapping(mmadrsdsc_t *mm, kmvarsdsc_t *kmvd, adr_t start, size_t vassize)
{
	adr_t end;

	if (NULL == mm || NULL == kmvd)
	{
		return FALSE;
	}

	end = start + (adr_t)vassize;

	return vma_del_unmapping_phyadrs(mm, kmvd, start, end);
}

bool_t vma_del_vadrs_core(mmadrsdsc_t *mm, adr_t start, size_t vassize)
{
	bool_t rets = FALSE;
	kmvarsdsc_t *newkmvd = NULL, *delkmvd = NULL;
	virmemadrs_t *vma = &mm->msd_virmemadrs;
	knl_spinlock(&vma->vs_lock);

	delkmvd = vma_del_find_kmvarsdsc(vma, start, vassize);
	if (NULL == delkmvd)
	{
		rets = FALSE;
		goto out;
	}

	if ((delkmvd->kva_start == start) && (delkmvd->kva_end == (start + (adr_t)vassize)))
	{
		vma_del_unmapping(mm, delkmvd, start, vassize);
		vma_del_set_endcurrkmvd(vma, delkmvd);
		knl_put_kvmemcbox(delkmvd->kva_kvmbox);
		list_del(&delkmvd->kva_list);
		del_kmvarsdsc(delkmvd);
		vma->vs_kmvdscnr--;
		rets = TRUE;
		goto out;
	}

	if ((delkmvd->kva_start == start) && (delkmvd->kva_end > (start + (adr_t)vassize)))
	{
		delkmvd->kva_start = start + (adr_t)vassize;
		vma_del_unmapping(mm, delkmvd, start, vassize);
		rets = TRUE;
		goto out;
	}

	if ((delkmvd->kva_start < start) && (delkmvd->kva_end == (start + (adr_t)vassize)))
	{
		delkmvd->kva_end = start;
		vma_del_unmapping(mm, delkmvd, start, vassize);
		rets = TRUE;
		goto out;
	}

	if ((delkmvd->kva_start < start) && (delkmvd->kva_end > (start + (adr_t)vassize)))
	{
		newkmvd = new_kmvarsdsc();
		if (NULL == newkmvd)
		{
			rets = FALSE;
			goto out;
		}

		newkmvd->kva_end = delkmvd->kva_end;
		newkmvd->kva_start = start + (adr_t)vassize;
		newkmvd->kva_limits = delkmvd->kva_limits;
		newkmvd->kva_maptype = delkmvd->kva_maptype;
		newkmvd->kva_mcstruct = vma;
		delkmvd->kva_end = start;

		knl_count_kvmemcbox(delkmvd->kva_kvmbox);
		newkmvd->kva_kvmbox = delkmvd->kva_kvmbox;

		vma_del_unmapping(mm, delkmvd, start, vassize);

		list_add(&newkmvd->kva_list, &delkmvd->kva_list);
		vma->vs_kmvdscnr++;
		if (list_is_last(&newkmvd->kva_list, &vma->vs_list) == TRUE)
		{
			vma->vs_endkmvdsc = newkmvd;
			vma->vs_currkmvdsc = newkmvd;
		}
		else
		{
			vma->vs_currkmvdsc = newkmvd;
		}
		rets = TRUE;
		goto out;
	}

	rets = FALSE;

out:
	knl_spinunlock(&vma->vs_lock);
	return rets;
}

bool_t vma_del_vadrs(mmadrsdsc_t *mm, adr_t start, size_t vassize)
{
	if (NULL == mm || 1 > vassize || NULL == start)
	{
		return FALSE;
	}
	return vma_del_vadrs_core(mm, start, VADSZ_ALIGN(vassize));
}


int imrand()
{

	krlvirmemadrs.kvs_randnext = krlvirmemadrs.kvs_randnext * 1103515245 + 12345;
	return ((unsigned)(krlvirmemadrs.kvs_randnext / 65536) % 32768);
}


int kvma_rand(int s,int e)
{
	return s + imrand() % e;
}


teststc_t *find_tstc_on_rnr(uint_t randnr)
{
	list_h_t *tmplst;

	teststc_t *tc = NULL;
	//bool_t rets=FALSE;
	//adr_t tmpendadr1=NULL,tmpendadr2=NULL;
	uint_t i = 0;
	if (randnr > krlvirmemadrs.kvs_tstcnr)
	{
		return NULL;
	}
	list_for_each(tmplst, &krlvirmemadrs.kvs_testhead)
	{
		if (i == randnr)
		{
			tc = list_entry(tmplst, teststc_t, tst_list);
			return tc;
		}

		i++;
	}
	return NULL;
}

void rand_write_tstc()
{
	teststc_t *t;
	int j = (int)(krlvirmemadrs.kvs_tstcnr / 2);
	//adr_t tmpadr;
	uint_t rd = 0;
	for (int i = 0; i < j; ++i)
	{
		rd = (uint_t)kvma_rand(0, (int)krlvirmemadrs.kvs_tstcnr);
		t = find_tstc_on_rnr(rd);
		if (NULL == t)
		{
			system_error("rand_del_tstc t null\n");
		}
		memset((void *)t->tst_vadr, 0xaa, t->tst_vsiz);
		kprint("成功随机写入第%x个虚拟地址空间:%x\n", rd, t->tst_vadr);
		//krlvirmemadrs.kvs_tstcnr--;
	}
	return;
}


void check_one_teststc(teststc_t *chktc)
{
	list_h_t *tmplst;

	teststc_t *tc = NULL;
	//bool_t rets=FALSE;
	adr_t tmpendadr1 = NULL, tmpendadr2 = NULL;
	list_for_each(tmplst, &krlvirmemadrs.kvs_testhead)
	{
		tc = list_entry(tmplst, teststc_t, tst_list);
		if (tc != chktc)
		{
			tmpendadr1 = chktc->tst_vadr + chktc->tst_vsiz;
			tmpendadr2 = tc->tst_vadr + tc->tst_vsiz;
			if ((chktc->tst_vadr > tc->tst_vadr && tmpendadr1 < tmpendadr2) ||
				(tc->tst_vadr > chktc->tst_vadr && tmpendadr2 < tmpendadr1) ||
				(chktc->tst_vadr > tc->tst_vadr && chktc->tst_vadr < tmpendadr2) ||
				(tc->tst_vadr > chktc->tst_vadr && tc->tst_vadr < tmpendadr1))
			{
				system_error("check_one_teststc err\n");
			}
		}
	}
	return;
}

void check_teststc()
{
	list_h_t *tmplst;

	teststc_t *tc = NULL;
	//bool_t rets=FALSE;
	uint_t i = 0;
	list_for_each(tmplst, &krlvirmemadrs.kvs_testhead)
	{
		tc = list_entry(tmplst, teststc_t, tst_list);
		check_one_teststc(tc);
		i++;
		kprint("TC检查完第%x个虚拟地址空间:%x %x %x\n", i, tc->tst_vadr, tc->tst_vsiz, krlvirmemadrs.kvs_kmvdscnr);
	}
	return;
}


void disp_kvirmemadrs()
{
	kprint("krlvirmemadrs.kvs_kmvdscnr:%x\n", krlvirmemadrs.kvs_kmvdscnr);
	kprint("krlvirmemadrs.kom_kvmcobjnr:%x :%x\n", krlvirmemadrs.kvs_kvmcomgr.kom_kvmcobjnr, krlvirmemadrs.kvs_kvmcomgr.kom_kvmcodelnr);
	kprint("krlvirmemadrs.kvs_tstcnr:%x\n", krlvirmemadrs.kvs_tstcnr);
	kprint("krlvirmemadrs.ptp_msanr:%x\n", krlvirmemadrs.kvs_ptabpgcs.ptp_msanr);
	die(0x200);
	return;
}


void vma_del_all_kvma(mmadrsdsc_t *mm)
{
	list_h_t *tmplst;

	teststc_t *tc = NULL;
	bool_t rets = FALSE;
	list_for_each_head_dell(tmplst, &krlvirmemadrs.kvs_testhead)
	{
		tc = list_entry(tmplst, teststc_t, tst_list);
		rets = vma_del_vadrs(mm, tc->tst_vadr, tc->tst_vsiz);
		if (FALSE == rets)
		{
			system_error("vma_del_all_kvma err\n");
		}
		list_del(&tc->tst_list);
		del_teststc(tc);
		krlvirmemadrs.kvs_tstcnr--;
		kprint("成功删除第%x个虚拟地址空间:%x\n", krlvirmemadrs.kvs_tstcnr, tc);
	}
	return;
}

void vma_chk_one_kmva(virmemadrs_t *vma, kmvarsdsc_t *chkkmva)
{
	kmvarsdsc_t *head;
	if (NULL == chkkmva || NULL == vma)
	{
		system_error("vma_chk_one_kmva chkkmva NULL\n");
	}
	list_h_t *pos;
	list_for_each(pos, &vma->vs_list)
	{
		head = list_entry(pos, kmvarsdsc_t, kva_list);

		if (head != chkkmva)
		{
			if ((chkkmva->kva_start > head->kva_start && chkkmva->kva_end < head->kva_end) ||
				(chkkmva->kva_start < head->kva_start && chkkmva->kva_end > head->kva_end) ||
				(chkkmva->kva_start > head->kva_start && chkkmva->kva_start < head->kva_end) ||
				(chkkmva->kva_start < head->kva_start && chkkmva->kva_end > head->kva_start))
			{
				kprint("chkkmva->kva_start:%x head->kva_start:%x chkkmva->kva_end:%x head->kva_end:%x\n",
						chkkmva->kva_start, head->kva_start, chkkmva->kva_end, head->kva_end);
				system_error("vma_chk_one_kmva err\n");
			}
		}
	}
	return;
}

void vma_check_kmva(mmadrsdsc_t *mm)
{
	virmemadrs_t *vma = &mm->msd_virmemadrs;
	kmvarsdsc_t *head;
	uint_t i = 0;
	list_h_t *pos;
	list_for_each(pos, &vma->vs_list)
	{
		head = list_entry(pos, kmvarsdsc_t, kva_list);
		vma_chk_one_kmva(vma, head);
		kprint("检查完第%x个虚拟地址空间:%x %x %x\n", i, head->kva_start, head->kva_end, krlvirmemadrs.kvs_kmvdscnr);
		i++;
	}
	return;
}

void vma_rand_del_tstc(mmadrsdsc_t *mm)
{
	teststc_t *t;
	int j = (int)(krlvirmemadrs.kvs_tstcnr / 2);
	adr_t tmpadr;
	uint_t rd = 0;
	for (int i = 0; i < j; ++i)
	{
		rd = (uint_t)kvma_rand(0, (int)krlvirmemadrs.kvs_tstcnr);
		t = find_tstc_on_rnr(rd);
		if (NULL == t)
		{
			system_error("vma_rand_del_tstc t null\n");
		}
		if (FALSE == vma_del_vadrs(mm, t->tst_vadr, t->tst_vsiz))
		{
			system_error("vma_rand_del_tstc terr\n");
		}
		tmpadr = t->tst_vadr;
		list_del(&t->tst_list);
		del_teststc(t);
		kprint("成功随机删除第%x个虚拟地址空间:%x\n", rd, tmpadr);
		krlvirmemadrs.kvs_tstcnr--;
	}
	return;
}

void vma_rand_write_tstc()
{
	teststc_t *t;
	int j = (int)(krlvirmemadrs.kvs_tstcnr / 2);
	//adr_t tmpadr;
	uint_t rd = 0;

	for (int i = 0; i < j; ++i)
	{
		rd = (uint_t)kvma_rand(0, (int)krlvirmemadrs.kvs_tstcnr);
		t = find_tstc_on_rnr(rd);
		if (NULL == t)
		{
			system_error("vma_rand_del_tstc t null\n");
		}
		hal_memset((void *)t->tst_vadr, 0xaa, t->tst_vsiz);
		kprint("成功随机写入第%x个虚拟地址空间:%x\n", rd, t->tst_vadr);
		//krlvirmemadrs.kvs_tstcnr--;
	}
	return;
}

void __test_vma(mmadrsdsc_t *mm)
{
	size_t vsz = 0x4000UL << 12;
	adr_t retadr;
	uint_t max = 0x2000;

	for (uint_t i = 0; i < max; i++)
	{
		vsz = (size_t)kvma_rand(0x1, 0x4UL << 12);
		retadr = vma_new_vadrs(mm, NULL, vsz, i, 0);
		if (NULL != retadr)
		{
			add_new_teststc(retadr, VADSZ_ALIGN(vsz));
			kprint("分配的虚拟:%x:大小:%x:个数:%x\n", retadr, vsz, i);
		}
		else
		{
			break;
		}
	}
	return;
}

// void test_vma()
// {
// 	mmadrsdsc_t *mm = knl_current_mmadrsdsc();
// 	__test_vma(mm);
// 	vma_rand_write_tstc(mm);
// 	vma_check_kmva(mm);
// 	vma_rand_del_tstc(mm);
// 	vma_rand_write_tstc(mm);
// 	vma_check_kmva(mm);
// 	vma_del_all_kvma(mm);
// 	dump_kvmemcboxmgr(&krlvirmemadrs.kvs_kvmemcboxmgr);
// 	dump_mmu(&mm->msd_mmu);
// 	return;
// }


cr3s_t knl_retn_currentcpu_cr3s()
{
	cr3s_t cr3;
	cr3.c3s_entry = (u64_t)read_cr3();
	return cr3;
}

void kmap_fulshcurrcpu_mmutable()
{
	//adr_t pml4badr=0UL;
	cr3s_t cr3;
	cr3.c3s_entry = (u64_t)read_cr3();
	write_cr3(cr3.c3s_entry);
	return;
}


kmvarsdsc_t *vma_map_find_kmvarsdsc(virmemadrs_t *vmalocked, adr_t vadrs)
{
	list_h_t *pos = NULL;
	kmvarsdsc_t *curr = vmalocked->vs_currkmvdsc;

	if (NULL != curr)
	{
		if ((vadrs >= curr->kva_start) && (vadrs < curr->kva_end))
		{
			return curr;
		}
	}
	list_for_each(pos, &vmalocked->vs_list)
	{
		curr = list_entry(pos, kmvarsdsc_t, kva_list);
		if ((vadrs >= curr->kva_start) && (vadrs < curr->kva_end))
		{
			return curr;
		}
	}
	return NULL;
}

kvmemcbox_t *vma_map_retn_kvmemcbox(kmvarsdsc_t *kmvd)
{
	kvmemcbox_t *kmbox = NULL;
	if (NULL == kmvd)
	{
		return NULL;
	}

	if (NULL != kmvd->kva_kvmbox)
	{
		return kmvd->kva_kvmbox;
	}
	kmbox = knl_get_kvmemcbox();
	if (NULL == kmbox)
	{
		return NULL;
	}
	kmvd->kva_kvmbox = kmbox;
	return kmvd->kva_kvmbox;
}

bool_t vma_del_usermsa(mmadrsdsc_t *mm, kvmemcbox_t *kmbox, msadsc_t *msa, adr_t phyadr)
{
	bool_t rets = FALSE;
	msadsc_t *tmpmsa = NULL, *delmsa = NULL;
	list_h_t *pos;

	if (NULL == mm || NULL == kmbox || NULL == phyadr)
	{
		return FALSE;
	}

	knl_spinlock(&kmbox->kmb_lock);

	if (NULL != msa)
	{
		if (msadsc_ret_addr(msa) == phyadr)
		{
			delmsa = msa;
			list_del(&msa->md_list);
			kmbox->kmb_msanr--;
			rets = TRUE;
			goto out;
		}
	}

	list_for_each(pos, &kmbox->kmb_msalist)
	{
		tmpmsa = list_entry(pos, msadsc_t, md_list);
		if (msadsc_ret_addr(tmpmsa) == phyadr)
		{
			delmsa = tmpmsa;
			list_del(&tmpmsa->md_list);
			kmbox->kmb_msanr--;
			rets = TRUE;
			goto out;
		}
	}

	delmsa = NULL;
	rets = FALSE;

out:
	knl_spinunlock(&kmbox->kmb_lock);

	if (NULL != delmsa)
	{
		if (mm_merge_pages(&memmgrob, delmsa, onfrmsa_retn_fpagenr(delmsa)) == FALSE)
		{
			system_error("vma_del_usermsa err\n");
			return FALSE;
		}
	}

	return rets;
}

msadsc_t *vma_new_usermsa(mmadrsdsc_t *mm, kvmemcbox_t *kmbox)
{
	u64_t pages = 1, retpnr = 0;
	msadsc_t *msa = NULL;

	if (NULL == mm || NULL == kmbox)
	{
		return NULL;
	}

	msa = mm_divpages_procmarea(&memmgrob, pages, &retpnr);
	if (NULL == msa)
	{
		return NULL;
	}

	knl_spinlock(&kmbox->kmb_lock);

	list_add(&msa->md_list, &kmbox->kmb_msalist);
	kmbox->kmb_msanr++;

	knl_spinunlock(&kmbox->kmb_lock);
	return msa;
}

adr_t vma_map_msa_fault(mmadrsdsc_t *mm, kvmemcbox_t *kmbox, adr_t vadrs, u64_t flags)
{
	msadsc_t *usermsa;
	adr_t phyadrs = NULL;
	if (NULL == mm || NULL == kmbox || NULL == vadrs)
	{
		return NULL;
	}

	usermsa = vma_new_usermsa(mm, kmbox);
	if (NULL == usermsa)
	{
		return NULL;
	}

	phyadrs = msadsc_ret_addr(usermsa);

	if (hal_mmu_transform(&mm->msd_mmu, vadrs, phyadrs, flags) == TRUE)
	{
		return phyadrs;
	}

	vma_del_usermsa(mm, kmbox, usermsa, phyadrs);
	return NULL;
}

adr_t vma_map_phyadrs(mmadrsdsc_t *mm, kmvarsdsc_t *kmvd, adr_t vadrs, u64_t flags)
{
	kvmemcbox_t *kmbox = kmvd->kva_kvmbox;
	if (NULL == kmbox)
	{
		return NULL;
	}
	return vma_map_msa_fault(mm, kmbox, vadrs, flags);
}

sint_t vma_map_fairvadrs_core(mmadrsdsc_t *mm, adr_t vadrs)
{
	sint_t rets = FALSE;
	adr_t phyadrs = NULL;
	virmemadrs_t *vma = &mm->msd_virmemadrs;
	kmvarsdsc_t *kmvd = NULL;
	kvmemcbox_t *kmbox = NULL;
	knl_spinlock(&vma->vs_lock);

	kmvd = vma_map_find_kmvarsdsc(vma, vadrs);
	if (NULL == kmvd)
	{
		rets = -EFAULT;
		goto out;
	}

	kmbox = vma_map_retn_kvmemcbox(kmvd);
	if (NULL == kmbox)
	{
		rets = -ENOMEM;
		goto out;
	}

	phyadrs = vma_map_phyadrs(mm, kmvd, vadrs, (0 | PML4E_US | PML4E_RW | PML4E_P));
	if (NULL == phyadrs)
	{
		rets = -ENOMEM;
		goto out;
	}

	rets = EOK;

out:
	knl_spinunlock(&vma->vs_lock);
	return rets;
}

sint_t vma_map_fairvadrs(mmadrsdsc_t *mm, adr_t vadrs)
{
	if ((0x1000 > vadrs) || (USER_VIRTUAL_ADDRESS_END < vadrs) || (NULL == mm))
	{
		return -EPARAM;
	}
	return vma_map_fairvadrs_core(mm, vadrs);
}

sint_t krluserspace_accessfailed(adr_t fairvadrs)
{
	mmadrsdsc_t* mm = &initmmadrsdsc;

	if(USER_VIRTUAL_ADDRESS_END < fairvadrs)
	{
		return -EACCES;
	}

	return vma_map_fairvadrs(mm, fairvadrs);
}

void test_krl_pages_fault()
{
	/*uint_t vsz=0x4000UL<<12;
	adr_t viradr; 
	for(int i=0;i<(10000);i++)
	{
		vsz=kvma_rand(0x1,0x4000UL<<12);
		viradr=kvma_vadrspace_new(vsz,0,0);
		kprint("分配的虚拟:%x:大小:%x:%x\n",viradr,vsz,i);
		//memset((void *)(viradr),0xaa,vsz);
		kprint("开始删除虚拟地址空:%x大小:%x\n",viradr,vsz);
		if(FALSE==kvma_vadrspace_del(viradr,vsz))
		{
			system_error("test_krl_pages_fault err\n");
		}
	}*/
	//test_kmva();
	return;
}

void kvmcobjmgr_t_init(kvmcobjmgr_t* initp)
{
	if(NULL==initp)
	{
		system_error("kvmcobjmgr_t_init parm NULL\n");
	}
	knl_spinlock_init(&initp->kom_lock);
	initp->kom_flgs=0;
	initp->kom_kvmcobjnr=0;
	list_init(&initp->kom_kvmcohead);
	initp->kom_kvmcocahenr=0;
	list_init(&initp->kom_kvmcocahe);
	initp->kom_kvmcodelnr=0;
	list_init(&initp->kom_kvmcodelhead);
	return;
}

void kvmemcobj_t_init(kvmemcobj_t* initp)
{
	if(NULL==initp)
	{
		system_error("kvmemcobj_t_init parm NULL\n");
	}
	list_init(&initp->kco_list);
	knl_spinlock_init(&initp->kco_lock);
	initp->kco_cont=0;
	initp->kco_flgs=0;
	initp->kco_type=0;
	initp->kco_msadnr=0;
	list_init(&initp->kco_msadlst);
	initp->kco_filenode=NULL;
	initp->kco_pager=NULL;
	initp->kco_extp=NULL;
	return;
}

kvmemcobj_t* new_kvmemcobj()
{
	kvmemcobj_t* kvcop=(kvmemcobj_t*)kmsob_new(sizeof(kvmemcobj_t));
	if(NULL==kvcop)
	{
		return NULL;
	}
	kvmemcobj_t_init(kvcop);
	return kvcop;
}

bool_t del_kvmemcobj(kvmemcobj_t* delkvmcop)
{
	if(NULL==delkvmcop)
	{
		return FALSE;
	}
	if(kmsob_delete((void*)delkvmcop,sizeof(kvmcobjmgr_t))==FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

void kvmemcbox_t_init(kvmemcbox_t* init)
{
	if(NULL == init)
	{
		return;
	}
	list_init(&init->kmb_list);
	knl_spinlock_init(&init->kmb_lock);
	refcount_init(&init->kmb_cont);
	init->kmb_flgs = 0;
	init->kmb_stus = 0;
	init->kmb_type = 0;
	init->kmb_msanr = 0;
	list_init(&init->kmb_msalist);
	init->kmb_mgr = NULL;
	init->kmb_filenode = NULL;
	init->kmb_pager = NULL;
	init->kmb_ext = NULL;
	return;
}

void kvmemcboxmgr_t_init(kvmemcboxmgr_t* init)
{
	if(NULL == init)
	{
		return;
	}
	list_init(&init->kbm_list);
	knl_spinlock_init(&init->kbm_lock);
	init->kbm_flgs = 0;
	init->kbm_stus = 0;
	init->kbm_kmbnr = 0;
	list_init(&init->kbm_kmbhead);
	init->kbm_cachenr = 0;
	init->kbm_cachemax = KMBOX_CACHE_MAX;
	init->kbm_cachemin = KMBOX_CACHE_MIN;
	list_init(&init->kbm_cachehead);
	init->kbm_ext = NULL;
	return;
}

kvmemcbox_t* new_kvmemcbox()
{
	kvmemcbox_t* kmbox = NULL;
	
	kmbox = (kvmemcbox_t*)kmsob_new(sizeof(kvmemcbox_t));
	if(NULL == kmbox)
	{
		return NULL;
	}
	kvmemcbox_t_init(kmbox);
	return kmbox;
}

bool_t del_kvmemcbox(kvmemcbox_t* del)
{
	if(NULL == del)
	{
		return FALSE;
	}
	return kmsob_delete((void*)del, sizeof(kvmemcbox_t));
}

void knl_count_kvmemcbox(kvmemcbox_t* kmbox)
{
	if(NULL == kmbox)
	{
		return;
	}
	refcount_inc(&kmbox->kmb_cont);
	return;
}

void knl_decount_kvmemcbox(kvmemcbox_t* kmbox)
{
	if(NULL == kmbox)
	{
		return;
	}
	refcount_dec(&kmbox->kmb_cont);
	return;
}

kvmemcbox_t* knl_get_kvmemcbox()
{
	kvmemcbox_t* kmb = NULL;
	kvmemcboxmgr_t* kmbmgr = &krlvirmemadrs.kvs_kvmemcboxmgr;
	knl_spinlock(&kmbmgr->kbm_lock);
	if(0 < kmbmgr->kbm_cachenr)
	{
		if(list_is_empty_careful(&kmbmgr->kbm_cachehead) == FALSE)
		{
			kmb = list_first_oneobj(&kmbmgr->kbm_cachehead, kvmemcbox_t, kmb_list);
			list_del(&kmb->kmb_list);
			kmbmgr->kbm_cachenr--;
			
			kvmemcbox_t_init(kmb);
			list_add(&kmb->kmb_list, &kmbmgr->kbm_kmbhead);
			kmbmgr->kbm_kmbnr++;
			refcount_inc(&kmb->kmb_cont);
			kmb->kmb_mgr = kmbmgr;
			kmb = kmb;
			goto out; 
		}
	}

	kmb = new_kvmemcbox();
	if(NULL == kmb)
	{
		goto out;
	}

	list_add(&kmb->kmb_list, &kmbmgr->kbm_kmbhead);
	kmbmgr->kbm_kmbnr++;
	refcount_inc(&kmb->kmb_cont);
	kmb->kmb_mgr = kmbmgr;

out:
	knl_spinunlock(&kmbmgr->kbm_lock);	
	return kmb;
}

bool_t knl_put_kvmemcbox(kvmemcbox_t* kmbox)
{
	kvmemcboxmgr_t* kmbmgr = &krlvirmemadrs.kvs_kvmemcboxmgr;
	bool_t rets = FALSE;
	if(NULL == kmbox)
	{
		return FALSE;
	}

	knl_spinlock(&kmbmgr->kbm_lock);
	
	refcount_dec(&kmbox->kmb_cont);
	if(refcount_read(&kmbox->kmb_cont) >= 1)
	{
		rets = TRUE;
		goto out;
	}
	
	if(kmbmgr->kbm_cachenr >= kmbmgr->kbm_cachemax)
	{
		list_del(&kmbox->kmb_list);
		if(del_kvmemcbox(kmbox) == FALSE)
		{
			rets = FALSE;
			goto out;
		}
		else
		{
			kmbmgr->kbm_kmbnr--;
			rets = TRUE;
			goto out;
		}
	}

	list_move(&kmbox->kmb_list, &kmbmgr->kbm_cachehead);
	kmbmgr->kbm_cachenr++;
	kmbmgr->kbm_kmbnr--;
	
	rets = TRUE;
out:
	knl_spinunlock(&kmbmgr->kbm_lock);
	return rets;
} 

void dump_kvmemcboxmgr(kvmemcboxmgr_t* dump)
{
	if(NULL == dump)
	{
		return;
	}
	kprint("kvmemcboxmgr_t.kmb_kmbnr:%x\n", dump->kbm_kmbnr);
	kprint("kvmemcboxmgr_t.kmb_kmbhead:%x\n", list_is_empty_careful(&dump->kbm_kmbhead));
	kprint("kvmemcboxmgr_t.kmb_cachenr:%x\n", dump->kbm_cachenr);
	kprint("kvmemcboxmgr_t.kmb_cachemax:%x\n", dump->kbm_cachemax);
	kprint("kvmemcboxmgr_t.kmb_cachemin:%x\n", dump->kbm_cachemin);
	kprint("kvmemcboxmgr_t.kmb_cachehead:%x\n", list_is_empty_careful(&dump->kbm_cachehead));
	return;
}
