/******************************************************************
*		ld自动化编译配置文件 Makefile			  *
*				彭东  	  *
******************************************************************/

#include "config.h"

#ifdef CFG_X86_PLATFORM
#include "buildfile.h"

VIRTUAL_ADDRESS = APPRUN_START_VITRUALADDR;
OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

SECTIONS
{
  
    . = VIRTUAL_ADDRESS;
    __begin_start_text = .;
        .start.text ALIGN(4) : AT(ADDR(.start.text)-VIRTUAL_ADDRESS) { *(.start.text) }
	__end_start_text = .;
	
	__begin_text = .;
        .text  ALIGN(4) : AT(ADDR(.text)-VIRTUAL_ADDRESS) { *(.text) }
	__end_text = .;

	__begin_data = .;
        .data ALIGN(4) : AT(ADDR(.data)-VIRTUAL_ADDRESS) { *(.data) *(.bss) }
 	__end_data = .;
	
	__begin_rodata = .;
	.rodata ALIGN(4) : AT(ADDR(.rodata)-VIRTUAL_ADDRESS) { *(.rodata) *(.rodata.*) }
	__end_rodata = .;

	__begin_kstrtab = .;
	.kstrtab ALIGN(4) : AT(ADDR(.kstrtab)-VIRTUAL_ADDRESS) { *(.kstrtab) }
	__end_kstrtab = .;

	__begin_kmodsattrb_data = .;
	.kmodsattrbdata ALIGN(4) : AT(ADDR(.kmodsattrbdata)-VIRTUAL_ADDRESS) { *(.kmodsattrbdata) }
	__end_kmodsattrb_data = .;

	__begin_bss = .;
      .bss ALIGN(4) : AT(ADDR(.bss)-VIRTUAL_ADDRESS) { *(.bss) }
	__end_bss = .;
	

}

#endif