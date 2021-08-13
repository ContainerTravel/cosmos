/**********************************************************
        时间文件krltime.c
***********************************************************
                彭东 
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void ktime_t_init(ktime_t *initp)
{
    krlspinlock_init(&initp->kt_lock);
    initp->kt_year = 0;
    initp->kt_mon = 0;
    initp->kt_day = 0;
    initp->kt_date = 0;
    initp->kt_hour = 0;
    initp->kt_min = 0;
    initp->kt_sec = 0;
    initp->kt_datap = NULL;
    return;
}

void init_ktime()
{
    ktime_t_init(&osktime);
    return;
}


void krlupdate_times_from_cmos()
{
    ktime_t *initp = &osktime;
    u8_t tmptm;
    cpuflg_t cpufg;
    krlspinlock_cli(&initp->kt_lock, &cpufg);
	CMOS_READ(tmptm,CMOS_SEC_ADR);
    initp->kt_sec = bcd_to_bin(tmptm);
    CMOS_READ(tmptm,CMOS_MIN_ADR);
    initp->kt_min = bcd_to_bin(tmptm);
    CMOS_READ(tmptm,CMOS_HOUR_ADR);
    initp->kt_hour = bcd_to_bin(tmptm);
    CMOS_READ(tmptm,CMOS_DAY_ADR);
    initp->kt_day = bcd_to_bin(tmptm);
    CMOS_READ(tmptm,CMOS_MON_ADR);
    initp->kt_mon = bcd_to_bin(tmptm);
    CMOS_READ(tmptm,CMOS_YEAR_ADR);
    initp->kt_year = bcd_to_bin(tmptm);
    // kprint("osktime y:%d,m:%x,d:%d,h:%d,m:%d,s:%d\n", initp->kt_year, initp->kt_mon,initp->kt_day,
    //                                                     initp->kt_hour, initp->kt_min, initp->kt_sec);
    krlspinunlock_sti(&initp->kt_lock, &cpufg);
    return;
}



void krlupdate_times(uint_t year, uint_t mon, uint_t day, uint_t date, uint_t hour, uint_t min, uint_t sec)
{
    ktime_t *initp = &osktime;
    cpuflg_t cpufg;
    krlspinlock_cli(&initp->kt_lock, &cpufg);
    initp->kt_year = year;
    initp->kt_mon = mon;
    initp->kt_day = day;
    initp->kt_date = date;
    initp->kt_hour = hour;
    initp->kt_min = min;
    initp->kt_sec = sec;
    krlspinunlock_sti(&initp->kt_lock, &cpufg);
    return;
}

sysstus_t krlsvetabl_time(uint_t inr, stkparame_t *stkparv)
{
    if (inr != INR_TIME)
    {
        return SYSSTUSERR;
    }
    return krlsve_time((time_t *)stkparv->parmv1);
}

sysstus_t krlsve_time(time_t *time)
{
    if (time == NULL)
    {
        return SYSSTUSERR;
    }

    ktime_t *initp = &osktime;
    cpuflg_t cpufg;
    krlspinlock_cli(&initp->kt_lock, &cpufg);
    time->year = initp->kt_year;
    time->mon = initp->kt_mon;
    time->day = initp->kt_day;
    time->date = initp->kt_date;
    time->hour = initp->kt_hour;
    time->min = initp->kt_min;
    time->sec = initp->kt_sec;
    krlspinunlock_sti(&initp->kt_lock, &cpufg);

    return SYSSTUSOK;
}
