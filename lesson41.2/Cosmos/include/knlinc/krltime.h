/**********************************************************
        时间头文件krltime.h
***********************************************************
                彭东
**********************************************************/
#ifndef _KRLTIME_H
#define _KRLTIME_H
void ktime_t_init(ktime_t* initp);
void init_ktime();
void krlupdate_times_from_cmos();
void krlupdate_times(uint_t year,uint_t mon,uint_t day,uint_t date,uint_t hour,uint_t min,uint_t sec);
sysstus_t krlsvetabl_time(uint_t inr,stkparame_t* stkparv);
sysstus_t krlsve_time(time_t* time);
KLINE u32_t bcd_to_bin(u32_t val)
{
	return (val & 0xf) + ((val >> 4) * 10);
}
#endif // KRLTIME_H
