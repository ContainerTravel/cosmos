;*****************************************************************************
;*		内核初始化入口文件init_entry.asm				    *
;*				彭东	     
;*****************************************************************************
%define MBSP_ADR 0x100000
%define IA32_EFER 0C0000080H
%define PML4T_BADR 0x1000000  
%define KRLVIRADR 0xffff800000000000
%define KINITSTACK_OFF 16
global _start
global x64_GDT
extern hal_start

[section .start.text]
[BITS 32]
_start:
	cli
	mov ax,0x10
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov fs,ax
	mov gs,ax
    lgdt [eGdtPtr]        
;开启 PAE
    mov eax, cr4
    bts eax, 5                      ; CR4.PAE = 1
    mov cr4, eax
    mov eax, PML4T_BADR
    mov cr3, eax	
;开启 64bits long-mode
    mov ecx, IA32_EFER
    rdmsr
    bts eax, 8                      ; IA32_EFER.LME =1
    wrmsr
;开启 PE 和 paging
    mov eax, cr0
    bts eax, 0                      ; CR0.PE =1
    bts eax, 31
;开启 CACHE       
    btr eax,29						; CR0.NW=0
    btr eax,30						; CR0.CD=0  CACHE
        
    mov cr0, eax                    ; IA32_EFER.LMA = 1
    jmp 08:entry64
[BITS 64]
entry64:
	mov ax,0x10
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov fs,ax
	mov gs,ax
	xor rax,rax
	xor rbx,rbx
	xor rbp,rbp
	xor rcx,rcx
	xor rdx,rdx
	xor rdi,rdi
	xor rsi,rsi
	xor r8,r8
	xor r9,r9
	xor r10,r10
	xor r11,r11
	xor r12,r12
	xor r13,r13
	xor r14,r14
	xor r15,r15
    mov rbx,MBSP_ADR
    mov rax,KRLVIRADR
    mov rcx,[rbx+KINITSTACK_OFF]
    add rax,rcx
    xor rcx,rcx
	xor rbx,rbx
	mov rsp,rax
	push 0
	push 0x8
    mov rax,hal_start   			; 调用内核主函数
	push rax
    dw 0xcb48						; iret返回指令
    jmp $

		
[section .start.data]
[BITS 32]
ex64_GDT:
enull_x64_dsc:	dq 0	
ekrnl_c64_dsc:  dq 0x0020980000000000           ; 64-bit 内核代码段
ekrnl_d64_dsc:  dq 0x0000920000000000           ; 64-bit 内核数据段

euser_c64_dsc:  dq 0x0020f80000000000           ; 64-bit 用户代码段
euser_d64_dsc:  dq 0x0000f20000000000           ; 64-bit 用户数据段
eGdtLen			equ	$ - enull_x64_dsc			; GDT长度
eGdtPtr:		dw eGdtLen - 1					; GDT界限
				dq ex64_GDT

