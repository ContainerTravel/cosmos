/**********************************************************
        字符串头文件krlstr.h
***********************************************************
                彭东 
**********************************************************/
#ifndef _KRLSTR_H
#define _KRLSTR_H
int krlimrand();
int krlrand(int min,int max);
sint_t krlstrcmp(char_t* str_s,char_t* str_d);
sint_t krlstrlen(char* str_s);
sint_t krlstrcpy(char_t* str_s,char_t* str_d);
sint_t krlmemcopy(void* src, void* dest,uint_t count);
sint_t krlmemset(void* s,u8_t c,uint_t count);
#endif
