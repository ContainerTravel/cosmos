/**********************************************************
		屏幕上格式化输出头文件halprint_t.h
***********************************************************
				彭东
**********************************************************/
#ifndef _HALPRINTFK_T_H
#define _HALPRINTFK_T_H



typedef __builtin_va_list va_list;
#define va_start(ap,np) __builtin_va_start(ap,np)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap,ty) __builtin_va_arg(ap,ty)

#endif
