/**********************************************************
		全局／中断描述符头文件halgdtidt.h
***********************************************************
				彭东
**********************************************************/
#ifndef _HALGDTIDT_H
#define _HALGDTIDT_H

void	exc_divide_error();
void	exc_single_step_exception();
void	exc_nmi();
void	exc_breakpoint_exception();
void	exc_overflow();
void	exc_bounds_check();
void	exc_inval_opcode();
void	exc_copr_not_available();
void	exc_double_fault();
void	exc_copr_seg_overrun();
void	exc_inval_tss();
void	exc_segment_not_present();
void	exc_stack_exception();
void	exc_general_protection();
void	exc_page_fault();
void	exc_copr_error();
void	exc_alignment_check();
void	exc_machine_check();
void	exc_simd_fault();
void	hxi_exc_general_intpfault();
void	hxi_hwint00();
void	hxi_hwint01();
void	hxi_hwint02();
void	hxi_hwint03();
void	hxi_hwint04();
void	hxi_hwint05();
void	hxi_hwint06();
void	hxi_hwint07();
void	hxi_hwint08();
void	hxi_hwint09();
void	hxi_hwint10();
void	hxi_hwint11();
void	hxi_hwint12();
void	hxi_hwint13();
void	hxi_hwint14();
void	hxi_hwint15();
void	hxi_hwint16();
void	hxi_hwint17();
void	hxi_hwint18();
void	hxi_hwint19();
void	hxi_hwint20();
void	hxi_hwint21();
void	hxi_hwint22();
void	hxi_hwint23();
void	hxi_apic_svr();
void	hxi_apic_ipi_schedul();
void	hxi_apic_timer();
void	hxi_apic_thermal();
void	hxi_apic_performonitor();
void	hxi_apic_lint0();
void	hxi_apic_lint1();
void	hxi_apic_error();
void	exi_sys_call();
void	_ret_from_user_mode();


PUBLIC void init_descriptor();
PUBLIC  void init_idt_descriptor();
PUBLIC void set_idt_desc(u8_t vector, u8_t desc_type, inthandler_t handler, u8_t privilege);

#endif
//90
