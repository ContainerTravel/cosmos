/**********************************************************
        时钟管理头文件drvtick.h
***********************************************************
                彭东
**********************************************************/
#ifndef _DRVTICK_H
#define _DRVTICK_H
void systick_set_driver(driver_t* drvp);
void systick_set_device(device_t* devp,driver_t* drvp);
drvstus_t systick_entry(driver_t* drvp,uint_t val,void* p);
drvstus_t systick_exit(driver_t* drvp,uint_t val,void* p);
drvstus_t systick_handle(uint_t ift_nr,void* devp,void* sframe);
drvstus_t systick_open(device_t* devp,void* iopack);
drvstus_t systick_close(device_t* devp,void* iopack);
drvstus_t systick_read(device_t* devp,void* iopack);
drvstus_t systick_write(device_t* devp,void* iopack);
drvstus_t systick_lseek(device_t* devp,void* iopack);
drvstus_t systick_ioctrl(device_t* devp,void* iopack);
drvstus_t systick_dev_start(device_t* devp,void* iopack);
drvstus_t systick_dev_stop(device_t* devp,void* iopack);
drvstus_t systick_set_powerstus(device_t* devp,void* iopack);
drvstus_t systick_enum_dev(device_t* devp,void* iopack);
drvstus_t systick_flush(device_t* devp,void* iopack);
drvstus_t systick_shutdown(device_t* devp,void* iopack);
#endif
