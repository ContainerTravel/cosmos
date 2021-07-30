/**********************************************************
        CPU相关的宏定义文件cpu_t.h
***********************************************************
                彭东
**********************************************************/
#ifndef _CPU_T_H
#define _CPU_T_H

#ifdef CFG_X86_PLATFORM
/*
Push(EAX);
Push(ECX);
Push(EDX);
Push(EBX);
Push(Temp);
Push(EBP);
Push(ESI);
Push(EDI);
push    ds
push    es
push    fs
push    gs
    pop     gs
    pop     fs
    pop     es
    pop     ds
    pop     edi
    pop     esi
    pop     ebp
    pop     ebx
    pop     edx
    pop     ecx
    pop     eax
    iretd

    u32_t r_gs;     //+0//pop gs            
    u32_t r_fs;     //+4     
    u32_t r_es;     //+8
    u32_t r_ds;     //+12
    u32_t r_edi;        //+16
    u32_t r_esi;        //+20
    u32_t r_ebp;        //+24
    u32_t r_temp;       //+28
    u32_t r_ebx;        //+32
    u32_t r_edx;        //+36
    u32_t r_ecx;        //+40
    u32_t r_eax;        //+44
    u32_t r_eip_old;    //+48
    u32_t r_cs_old;     //+52
    u32_t r_flags_old;  //+56
    u32_t r_esp_old;    //+60
    u32_t r_ss_old;     //+64
*/
typedef struct s_INTSTKREGS
{
	uint_t r_gs;
	uint_t r_fs;
	uint_t r_es;
	uint_t r_ds;
	uint_t r_r15;
	uint_t r_r14;
	uint_t r_r13;
	uint_t r_r12;
	uint_t r_r11;
	uint_t r_r10;
	uint_t r_r9;
	uint_t r_r8;
	uint_t r_rdi;
	uint_t r_rsi;
	uint_t r_rbp;
	uint_t r_rdx;
	uint_t r_rcx;
	uint_t r_rbx;
	uint_t r_rax;
	uint_t r_rip_old;
	uint_t r_cs_old;
	uint_t r_rflgs;
	uint_t r_rsp_old;
	uint_t r_ss_old;
}intstkregs_t;

#endif


#endif // CPU_T_H
