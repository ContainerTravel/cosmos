/**********************************************************
		屏幕上格式化输出头文件halprint.h
***********************************************************
				彭东
**********************************************************/
#ifndef _HALPRINT_H
#define _HALPRINT_H
void printfk(const char* fmt,...);
void kprint(const char* fmt,...);
char_t* strcopyk(char_t* buf,char_t* str_s);
void vsprintfk(char* buf,const char* fmt,va_list args);
char_t* numberk(char_t* str,uint_t n, sint_t base);
void char_write_uart(char_t* buf);
#endif
