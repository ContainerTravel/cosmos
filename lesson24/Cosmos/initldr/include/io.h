/**********************************************************
		输入/输出头文件io.h
***********************************************************
				彭东
**********************************************************/
#ifndef _IO_H
#define _IO_H
KLINE void out_u8(const u16_t port, const u8_t val)
{

    __asm__ __volatile__("outb  %1, %0\n"
             :
             : "dN"(port), "a"(val));
}

KLINE u8_t in_u8(const u16_t port)
{
    u8_t tmp;
    __asm__ __volatile__("inb %1, %0\n"
             : "=a"(tmp)
             : "dN"(port));
    return tmp;
}


KLINE void out_u16(const u16_t port, const u16_t val)
{

    __asm__ __volatile__("outw  %1, %0\n"
             :
             : "dN"(port), "a"(val));
}


KLINE u16_t in_u16(const u16_t port)
{
    u16_t tmp;

    __asm__ __volatile__("inw %1, %0\n"
             : "=a"(tmp)
             : "dN"(port));
    return tmp;
};


KLINE sint_t m2mcopy(void* sadr,void* dadr,sint_t len)
{
    if(NULL==sadr||NULL==dadr||1>len)
    {
        return 0;
    }
    u8_t* s=(u8_t*)sadr,*d=(u8_t*)dadr;
    if(s<d)
    {
        for(sint_t i=(len-1);i>=0;i--)
        {
            d[i]=s[i];
        }
        return len;
    }
    if(s>d)
    {
        for(sint_t j=0;j<len;j++)
        {
            d[j]=s[j];
        }
        return len;
    }
    if(s==d)
    {
        return len;
    }
    return 0;
}

KLINE void memset(void* src,u8_t val,uint_t count)
{

    u8_t* ss=src;
    for(uint_t i=0;i<count;i++)
    {
        ss[i]=val;
    }
    return;
}

#endif
