;*****************************************************************************
;*		底层核心文件Kernel.asm				    *
;*				彭东 
;*****************************************************************************

%include "kernel.inc"
[section .hwint.text]
[BITS 64]

ALIGN	16
exc_divide_error:
	SRFTFAULT 0
ALIGN	16
exc_single_step_exception:
	SRFTFAULT 1
ALIGN	16
exc_nmi:
	SRFTFAULT 2
ALIGN	16
exc_breakpoint_exception:
	SRFTFAULT 3
ALIGN	16
exc_overflow:
	SRFTFAULT 4
ALIGN	16
exc_bounds_check:
	SRFTFAULT 5
ALIGN	16
exc_inval_opcode:
	SRFTFAULT 6
ALIGN	16
exc_copr_not_available:
	SRFTFAULT 7
ALIGN	16
exc_double_fault:
	SRFTFAULT_ECODE 8
ALIGN	16
exc_copr_seg_overrun:
	SRFTFAULT 9
ALIGN	16
exc_inval_tss:
	SRFTFAULT_ECODE 10
ALIGN	16
exc_segment_not_present:
	SRFTFAULT_ECODE 11
ALIGN	16
exc_stack_exception:
	SRFTFAULT_ECODE 12
ALIGN	16
exc_general_protection:
	SRFTFAULT_ECODE 13
ALIGN	16
exc_page_fault:
	SRFTFAULT_ECODE 14
ALIGN	16
exc_copr_error:
	SRFTFAULT 16
ALIGN	16
exc_alignment_check:
	SRFTFAULT_ECODE 17
ALIGN	16
exc_machine_check:
	SRFTFAULT 18
ALIGN	16
exc_simd_fault:
	SRFTFAULT 19
ALIGN	16
hxi_exc_general_intpfault:
	SRFTFAULT 256
	
	
ALIGN	16
hxi_hwint00:
	HARWINT	(INT_VECTOR_IRQ0+0)
ALIGN	16
hxi_hwint01:
	HARWINT	(INT_VECTOR_IRQ0+1)
ALIGN	16
hxi_hwint02:
	HARWINT	(INT_VECTOR_IRQ0+2)
ALIGN	16
hxi_hwint03:
	HARWINT	(INT_VECTOR_IRQ0+3)
ALIGN	16
hxi_hwint04:
	HARWINT	(INT_VECTOR_IRQ0+4)
ALIGN	16
hxi_hwint05:
	HARWINT	(INT_VECTOR_IRQ0+5)
ALIGN	16
hxi_hwint06:
	HARWINT	(INT_VECTOR_IRQ0+6)
ALIGN	16
hxi_hwint07:
	HARWINT	(INT_VECTOR_IRQ0+7)
ALIGN	16
hxi_hwint08:
	HARWINT	(INT_VECTOR_IRQ0+8)
ALIGN	16
hxi_hwint09:
	HARWINT	(INT_VECTOR_IRQ0+9)
ALIGN	16
hxi_hwint10:
	HARWINT	(INT_VECTOR_IRQ0+10)
ALIGN	16
hxi_hwint11:
	HARWINT	(INT_VECTOR_IRQ0+11)
ALIGN	16
hxi_hwint12:
	HARWINT	(INT_VECTOR_IRQ0+12)

ALIGN	16
hxi_hwint13:
	HARWINT	(INT_VECTOR_IRQ0+13)
ALIGN	16
hxi_hwint14:
	HARWINT	(INT_VECTOR_IRQ0+14)
ALIGN	16
hxi_hwint15:
	HARWINT	(INT_VECTOR_IRQ0+15)
ALIGN	16
hxi_hwint16:
	HARWINT	(INT_VECTOR_IRQ0+16)
ALIGN	16
hxi_hwint17:
	HARWINT	(INT_VECTOR_IRQ0+17)
	
ALIGN	16
hxi_hwint18:
	HARWINT	(INT_VECTOR_IRQ0+18)
	
ALIGN	16
hxi_hwint19:
	HARWINT	(INT_VECTOR_IRQ0+19)
	
ALIGN	16
hxi_hwint20:
	HARWINT	(INT_VECTOR_IRQ0+20)
	
ALIGN	16
hxi_hwint21:
	HARWINT	(INT_VECTOR_IRQ0+21)

ALIGN	16
hxi_hwint22:
	HARWINT	(INT_VECTOR_IRQ0+22)
ALIGN	16
hxi_hwint23:
	HARWINT	(INT_VECTOR_IRQ0+23)
				
ALIGN	16
hxi_apic_ipi_schedul:
	HARWINT	INT_VECTOR_APIC_IPI_SCHEDUL		
ALIGN	16
hxi_apic_svr:
	HARWINT	INT_VECTOR_APIC_SVR	
ALIGN	16
hxi_apic_timer:
	HARWINT	INT_VECTOR_APIC_TIMER	
ALIGN	16
hxi_apic_thermal:
	HARWINT	INT_VECTOR_APIC_THERMAL	
ALIGN	16
hxi_apic_performonitor:
	HARWINT	INT_VECTOR_APIC_PERFORM	
ALIGN	16
hxi_apic_lint0:
	HARWINT	INT_VECTOR_APIC_LINTO	
ALIGN	16
hxi_apic_lint1:
	HARWINT	INT_VECTOR_APIC_LINTI	
ALIGN	16
hxi_apic_error:
	HARWINT	INT_VECTOR_APIC_ERROR

ALIGN	16
exi_sys_call:
	EXI_SCALL
ALIGN	16
asm_ret_from_user_mode:
	RESTOREALL


