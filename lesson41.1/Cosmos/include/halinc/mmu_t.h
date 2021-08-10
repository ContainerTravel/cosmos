/**********************************************************
		内存mmu头文件halmmu_t.h
***********************************************************
				彭东
**********************************************************/
#ifndef _HALMMU_T_H
#define _HALMMU_T_H

#define TDIRE_MAX (512)
#define SDIRE_MAX (512)
#define IDIRE_MAX (512)
#define MDIRE_MAX (512)

#define MDIRE_IV_RSHTBIT (12)
#define MDIRE_IV_BITMASK (0x1ffUL)
#define MSA_PADR_LSHTBIT (12)
#define MDIRE_PADR_LSHTBIT (12)
#define IDIRE_IV_RSHTBIT (21)
#define IDIRE_IV_BITMASK (0x1ffUL)
#define IDIRE_PADR_LSHTBIT (12)
#define SDIRE_IV_RSHTBIT (30)
#define SDIRE_IV_BITMASK (0x1ffUL)
#define SDIRE_PADR_LSHTBIT (12)
#define TDIRE_IV_RSHTBIT (39)
#define TDIRE_IV_BITMASK (0x1ffUL)

#define PML4E_P (1<<0)
#define PML4E_RW (1<<1)
#define PML4E_US (1<<2)
#define PML4E_PWT (1<<3)
#define PML4E_PCD (1<<4)

#define PDPTE_P (1<<0)
#define PDPTE_RW (1<<1)
#define PDPTE_US (1<<2)
#define PDPTE_PWT (1<<3)
#define PDPTE_PCD (1<<4)
#define PDPTE_PS (1<<7)

#define PDTE_P (1<<0)
#define PDTE_RW (1<<1)
#define PDTE_US (1<<2)
#define PDTE_PWT (1<<3)
#define PDTE_PCD (1<<4)
#define PDTE_PS (1<<7)

#define PTE_P (1<<0)
#define PTE_RW (1<<1)
#define PTE_US (1<<2)
#define PTE_PWT (1<<3)
#define PTE_PCD (1<<4)
#define PTE_PAT (1<<7)
#define PDPTEPHYADR_IP_LSHTBIT (12)
#define PDTEPHYADR_IP_LSHTBIT (12)
#define PTEPHYADR_IP_LSHTBIT (12)
#define PFMPHYADR_IP_LSHTBIT (12)
#define PTE_HAVE_MASK (~0xfff)
#define PDE_HAVE_MASK (~0xfff)
#define PDPTE_HAVE_MASK (~0xfff)
#define PML4E_HAVE_MASK (~0xfff)

typedef struct MDIREFLAGS
{
	u64_t m_p:1;//0
	u64_t m_rw:1;//1
	u64_t m_us:1;//2
	u64_t m_pwt:1;//3
	u64_t m_pcd:1;//4
	u64_t m_a:1;//5
	u64_t m_d:1;//6
	u64_t m_pat:1;//7
	u64_t m_g:1;//8
	u64_t m_ig1:3;//9\10\11
	u64_t m_msa:40;//12
	u64_t m_ig2:11;//52
	u64_t m_xd:1;//63
}__attribute__((packed)) mdireflags_t;

typedef struct MDIRE
{
	union
	{
		mdireflags_t m_flags;
		u64_t m_entry;
	}__attribute__((packed));
	
}__attribute__((packed)) mdire_t;

typedef struct IDIREFLAGS
{
	u64_t i_p:1;//0
	u64_t i_rw:1;//1
	u64_t i_us:1;//2
	u64_t i_pwt:1;//3
	u64_t i_pcd:1;//4
	u64_t i_a:1;//5
	u64_t i_ig1:1;//6
	u64_t i_ps:1;//74kb==0
	u64_t i_ig2:4;//8\9\10\11
	u64_t i_mdir:40;//12
	u64_t i_ig3:11;//52
	u64_t i_xd:1;//63
}__attribute__((packed)) idireflags_t;

typedef struct IDIRE
{
	union
	{
		idireflags_t i_flags;
		u64_t i_entry;
	}__attribute__((packed));
	
}__attribute__((packed)) idire_t;

typedef struct SDIREFLAGS
{
	u64_t s_p:1;//0
	u64_t s_rw:1;//1
	u64_t s_us:1;//2
	u64_t s_pwt:1;//3
	u64_t s_pcd:1;//4
	u64_t s_a:1;//5
	u64_t s_ig1:1;//6
	u64_t s_ps:1;//74kb==0
	u64_t s_ig2:4;//8\9\10\11
	u64_t s_idir:40;//12
	u64_t s_ig3:11;//52
	u64_t s_xd:1;//63
}__attribute__((packed)) sdireflags_t;

typedef struct SDIRE
{
	union
	{
		sdireflags_t s_flags;
		u64_t s_entry;
	}__attribute__((packed));
	
}__attribute__((packed)) sdire_t;


typedef struct TDIREFLAGS
{
	u64_t t_p:1;//0
	u64_t t_rw:1;//1
	u64_t t_us:1;//2
	u64_t t_pwt:1;//3
	u64_t t_pcd:1;//4
	u64_t t_a:1;//5
	u64_t t_ig1:1;//6
	u64_t t_rv1:1;//7
	u64_t t_ig2:4;//8\9\10\11
	u64_t t_sdir:40;//12
	u64_t t_ig3:11;//52
	u64_t t_xd:1;//63
}__attribute__((packed)) tdireflags_t;

typedef struct TDIRE
{
	union
	{
		tdireflags_t t_flags;
		u64_t t_entry;
	}__attribute__((packed));
	
}__attribute__((packed)) tdire_t;

typedef struct MDIREARR
{
	mdire_t mde_arr[MDIRE_MAX];
}__attribute__((packed)) mdirearr_t;

typedef struct IDIREARR
{
	idire_t ide_arr[IDIRE_MAX];
}__attribute__((packed)) idirearr_t;

typedef struct SDIREARR
{
	sdire_t sde_arr[SDIRE_MAX];
}__attribute__((packed)) sdirearr_t;

typedef struct TDIREARR
{
	tdire_t tde_arr[TDIRE_MAX];
}__attribute__((packed)) tdirearr_t;

typedef struct CR3SFLGS
{
	u64_t c3s_pcid:12;//0
	u64_t c3s_plm4a:40;//12
	u64_t c3s_rv:11;//52
	u64_t c3s_tbc:1;//63
}__attribute__((packed)) cr3sflgs_t;

typedef struct CR3S
{
	union
	{
		cr3sflgs_t c3s_c3sflgs;
		u64_t c3s_entry;
	}__attribute__((packed));
	
}__attribute__((packed)) cr3s_t;

typedef struct MMUDSC
{
    spinlock_t mud_lock;
    u64_t mud_stus;
    u64_t mud_flag;
    tdirearr_t* mud_tdirearr;
    cr3s_t mud_cr3;
    list_h_t mud_tdirhead;
    list_h_t mud_sdirhead;
    list_h_t mud_idirhead;
    list_h_t mud_mdirhead;
    uint_t mud_tdirmsanr;
    uint_t mud_sdirmsanr;
    uint_t mud_idirmsanr;
    uint_t mud_mdirmsanr;
}mmudsc_t;

#endif
