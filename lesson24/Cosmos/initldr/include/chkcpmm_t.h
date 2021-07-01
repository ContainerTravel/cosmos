/**********************************************************
		系统全局内存检查文件头chkcpmm_t.h
***********************************************************
				彭东=
**********************************************************/
#ifndef _CHKCPMM_T_H
#define _CHKCPMM_T_H
#define EMAP_PTR E80MAP_ADR
#define EMAP_NR_PTR E80MAP_NR

#define BASE_MEM_SZ 0x3f80000

#define PML4T_BADR 0x20000
#define PDPTE_BADR 0x21000

#define PDE_BADR 0x22000
#define PTE_BADR 0x23000

#define PG_SIZE 512





#define PDT_S_PNT	0x1 
#define PDT_S_RW	0x2
#define PDT_S_US	0x4
#define PDT_S_PWT	0x8
#define PDT_S_PCD	0x10
#define PDT_S_ACC	0x20
#define PDT_S_DITYRE	0x40
#define PDT_S_SIZE	0x80
#define PDT_S_GLOP	0x100

#define PT_S_PNT	0x1 
#define PT_S_RW		0x2
#define PT_S_US		0x4
#define PT_S_PWT	0x8
#define PT_S_PCD	0x10
#define PT_S_ACC	0x20
#define PT_S_DITY	0x40
#define PT_S_PTARE	0x80
#define PT_S_GLOP	0x100

typedef unsigned long long pt64_t;

#endif 
