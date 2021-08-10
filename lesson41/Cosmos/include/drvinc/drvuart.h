/**********************************************************
        串口驱动头文件drvuart.h
***********************************************************
                彭东 
**********************************************************/
#ifndef _DRVUART_H
#define _DRVUART_H
void uart_set_driver(driver_t* drvp);
void uart0_set_device(device_t* devp,driver_t* drvp);
drvstus_t uart_entry(driver_t* drvp,uint_t val,void* p);
drvstus_t uart_exit(driver_t* drvp,uint_t val,void* p);

drvstus_t uart_open(device_t* devp,void* iopack);
drvstus_t uart_close(device_t* devp,void* iopack);
drvstus_t uart_read(device_t* devp,void* iopack);
drvstus_t uart_write(device_t* devp,void* iopack);
drvstus_t uart_lseek(device_t* devp,void* iopack);
drvstus_t uart_ioctrl(device_t* devp,void* iopack);
drvstus_t uart_dev_start(device_t* devp,void* iopack);
drvstus_t uart_dev_stop(device_t* devp,void* iopack);
drvstus_t uart_set_powerstus(device_t* devp,void* iopack);
drvstus_t uart_enum_dev(device_t* devp,void* iopack);
drvstus_t uart_flush(device_t* devp,void* iopack);
drvstus_t uart_shutdown(device_t* devp,void* iopack);

#endif // DRVUART_H
