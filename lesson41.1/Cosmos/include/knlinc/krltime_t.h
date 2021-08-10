/**********************************************************
        时间头文件krltime_t.h
***********************************************************
                彭东
**********************************************************/
#ifndef _KRLTIME_T_H
#define _KRLTIME_T_H

typedef struct s_KTIME
{
    spinlock_t  kt_lock;
    uint_t      kt_year;
    uint_t      kt_mon;
    uint_t      kt_day;
    uint_t      kt_date;
    uint_t      kt_hour;
    uint_t      kt_min;
    uint_t      kt_sec;
    void*       kt_datap;
    
}ktime_t;

typedef struct s_TIME
{
    
    uint_t      year;
    uint_t      mon;
    uint_t      day;
    uint_t      date;
    uint_t      hour;
    uint_t      min;
    uint_t      sec;
    
}time_t;

#define CMOS_PROT_ADR 0x70
#define CMOS_PROT_DATE 0x71
#define CMOS_SEC_ADR 0x00
#define CMOS_MIN_ADR 0x02
#define CMOS_HOUR_ADR 0x04
#define CMOS_DAY_ADR 0x07
#define CMOS_MON_ADR 0x08
#define CMOS_YEAR_ADR 0x09
#define CMOS_READ(val,adr) ({out_u8(CMOS_PROT_ADR,adr);val=in_u8(CMOS_PROT_DATE);}) 

#endif // KRLTIME_T_H
