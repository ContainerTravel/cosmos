/**********************************************************
		输入/输出头文件io.h
***********************************************************
				彭东
**********************************************************/
#ifndef _IO_H
#define _IO_H

#define ICW1 0x11
#define ZICW2 0x20
#define SICW2 0x28

#define ZICW3 0x04
#define SICW3 0x02

#define ICW4 0x01

#define ZIOPT 0x20
#define ZIOPT1 0x21

#define SIOPT 0xA0
#define SIOPT1 0xA1

#define _INTM_CTL 0x20     // I/O port for interrupt controller         <Master>
#define _INTM_CTLMASK 0x21 // setting bits in this port disables ints   <Master>
#define _INTS_CTL 0xA0     //; I/O port for second interrupt controller  <Slave>
#define _INTS_CTLMASK 0xA1 //; setting bits in this port disables ints   <Slave>
#define _EOI 0x20

#define PTIPROT1 0x40
#define PTIPROT2 0x41
#define PTIPROT3 0x42
#define PTIPROTM 0x43

#define TIMEMODE 0x34      //;00-11-010-0
#define TIMEJISU 1194000UL //1193182UL
#define HZ 1000UL          //0x3e8

#define HZLL ((TIMEJISU / HZ) & 0xff)        //0xa9//0x9b     //;1001  1011
#define HZHH (((TIMEJISU / HZ) >> 8) & 0xff) //0x04//0x2e     //;0010  1110

#define NORETURN __attribute__((noreturn))
#define SYSRCALL __attribute__((regparm(3)))
#define HINTCALL __attribute__((regparm(2)))
#define FAUTCALL __attribute__((regparm(2)))

#define REGCALL __attribute__((regparm(3))) //让GCC使用(EAX, EDX, ECX)寄存器传递参数
#define __SAVE_FLAGS(x) __asm__ __volatile__("pushfl ; popl %0" \
                                             : "=g"(x)          \
                                             : /* no input */)
#define __RESTORE_FLAGS(x) __asm__ __volatile__("pushl %0 ; popfl" \
                                                : /* no output */  \
                                                : "g"(x)           \
                                                : "memory")
#define CLI() __asm__ __volatile__("cli" \
                                   :     \
                                   :     \
                                   : "memory")
#define STI() __asm__ __volatile__("sti" \
                                   :     \
                                   :     \
                                   : "memory")
/*used in he idle loop; sti takes one instruction cycle to complete */
#define HALT() __asm__ __volatile__("hlt" \
                                    :     \
                                    :     \
                                    : "memory")
#define STI_HALT() __asm__ __volatile__("sti; hlt" \
                                        :          \
                                        :          \
                                        : "memory")
#define CLI_HALT() __asm__ __volatile__("cli; hlt" \
                                        :          \
                                        :          \
                                        : "memory")

KLINE void out_u8(const u16_t port, const u8_t val)
{

    __asm__ __volatile__("outb  %1, %0\n"
                         :
                         : "dN"(port), "a"(val));
}

KLINE void out_u8_p(const u16_t port, const u8_t val)
{

    __asm__ __volatile__("outb  %1, %0\n\t"
                         "nop \n\t"
                         "nop \n\t"

                         :
                         : "dN"(port), "a"(val));
}

KLINE u8_t in_u8(const u16_t port)
{
    u8_t tmp;
    /* GCC can optimize here if constant */
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

KLINE void out_u32(const u16_t port, const u32_t val)
{

    __asm__ __volatile__("outl  %1, %0\n"
                         :
                         : "dN"(port), "a"(val));
}

KLINE u32_t in_u32(const u16_t port)
{
    u32_t tmp;

    __asm__ __volatile__("inl %1, %0\n"
                         : "=a"(tmp)
                         : "dN"(port));
    return tmp;
}

KLINE u64_t read_msr(const u32_t reg)
{
    u32_t eax, edx;

    __asm__ __volatile__(
        "rdmsr"
        : "=a"(eax), "=d"(edx)
        : "c"(reg));

    return (((u64_t)edx) << 32) | (u64_t)eax;
}

KLINE void write_msr(const u32_t reg, const u64_t val)
{
    __asm__ __volatile__(
        "wrmsr"
        :
        : "a"((u32_t)val), "d"((u32_t)(val >> 32)), "c"(reg));
}

KLINE void memcopy(void *src, void *dest, uint_t count)
{

    u8_t *ss = src, *sd = dest;
    for (uint_t i = 0; i < count; i++)
    {
        sd[i] = ss[i];
    }
    return;
}

KLINE sint_t m2mcopy(void *sadr, void *dadr, sint_t len)
{
    if (NULL == sadr || NULL == dadr || 1 > len)
    {
        return 0;
    }
    u8_t *s = (u8_t *)sadr, *d = (u8_t *)dadr;
    if (s < d)
    {
        for (sint_t i = (len - 1); i >= 0; i--)
        {
            d[i] = s[i];
        }
        return len;
    }
    if (s > d)
    {
        for (sint_t j = 0; j < len; j++)
        {
            d[j] = s[j];
        }
        return len;
    }
    if (s == d)
    {
        return len;
    }
    return 0;
}

KLINE void memset(void *s, u8_t c, uint_t count)
{
    u8_t *st = s;
    for (uint_t i = 0; i < count; i++)
    {
        st[i] = c;
    }
    return;
}

KLINE void save_flags_cli(cpuflg_t *flags)
{
    __asm__ __volatile__(
        "pushfq \t\n"
        "cli    \t\n"
        "popq %0 \t\n"

        : "=m"(*flags)
        :
        : "memory");
    return;
}

KLINE void restore_flags_sti(cpuflg_t *flagsres)
{
    __asm__ __volatile__(
        "pushq %0 \t\n"
        "popfq \t\n"
        :
        : "m"(*flagsres)
        : "memory");
    return;
}

KLINE u64_t x86_rdpmc(const int ctrsel)
{
    u32_t eax, edx;

    __asm__ __volatile__(
        "rdpmc"
        : "=a"(eax), "=d"(edx)
        : "c"(ctrsel));

    return (((u64_t)edx) << 32) | (u64_t)eax;
}

KLINE u64_t x86_rdtsc(void)
{
    u32_t leax, ledx;

    __asm__ __volatile__(
        "rdtsc"
        : "=a"(leax), "=d"(ledx));

    return (((u64_t)ledx) << 32) | (u64_t)leax;
}

KLINE sint_t search_64rlbits(u64_t val)
{
    sint_t retbitnr = -1;
    __asm__ __volatile__(
        "bsrq %1,%q0 \t\n"
        : "+r"(retbitnr)
        : "rm"(val));
    return retbitnr + 1;
}

KLINE sint_t search_32rlbits(u32_t val)
{
    sint_t retbitnr = -1;
    __asm__ __volatile__(
        "bsrl %1,%0 \t\n"
        : "+r"(retbitnr)
        : "rm"(val));
    return retbitnr + 1;
}

KLINE u32_t read_kesp()
{
    u32_t esp;

    __asm__ __volatile__(
        "movl %%esp,%0"
        : "=g"(esp)
        :
        : "memory");

    return esp;
}

KLINE u32_t read_kcr2()
{
    u32_t cr2;

    __asm__ __volatile__(
        "movl %%cr2,%0"
        : "=g"(cr2)
        :
        : "memory");

    return cr2;
}
KLINE void set_cr3(u64_t pl4adr)
{
    __asm__ __volatile__(

        "movq %0,%%cr3 \n\t"
        :
        : "r"(pl4adr)
        : "memory" //, "edx"
    );
    return;
}

#endif
