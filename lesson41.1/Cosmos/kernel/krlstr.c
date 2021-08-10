/**********************************************************
        字符串文件krlstr.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"
static uint_t rand_next = 0;

int krlimrand()
{
	
    rand_next = rand_next *1103515245+12345;
    return ((unsigned)(rand_next/65536) % 32768);
}

int krlrand(int min,int max)
{
	return min + krlimrand() % max;
}

sint_t krlstrcmp(char_t* str_s,char_t* str_d)
{
	for(;;)
	{
		if(*str_s!=*str_d)
		{
			return STR_CMP_ERR;
		}
		if(*str_s==0)
		{
			break;
		}
		str_s++;
		str_d++;
	}
	return STR_CMP_OK;
}

sint_t krlstrlen(char* str_s)
{
	sint_t chaidx=0;
	while(*str_s!=0)
	{
		str_s++;
		chaidx++;
	}
	return chaidx;
}

sint_t krlstrcpy(char_t* str_s,char_t* str_d)
{
	sint_t chaidx=0;
	while(*str_s!=0)
	{
		*str_d=*str_s;
		str_s++;
		str_d++;
		chaidx++;
		
	}
	*str_d=*str_s;
	return chaidx;
}

sint_t krlmemcopy(void* src, void* dest,uint_t count)
{
	u8_t* ss = src,*sd = dest;	
	uint_t i;
	if(NULL == src || NULL == dest)
	{
		return -EPERM;
	}
	for(i = 0; i < count; i++)
	{
		sd[i] = ss[i];
	}
	return (sint_t)(i - 1);
}


sint_t krlmemset(void* s,u8_t c,uint_t count)
{
	u8_t* st=s;
	uint_t i;
	if(NULL == s)
	{
		return -EPERM;
	}
	for(i = 0; i < count; i++)
	{
		st[i] = c;
	}
	return (sint_t)(i - 1);
}

