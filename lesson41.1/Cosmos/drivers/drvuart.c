/**********************************************************
        串口驱动文件drvuart.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void uart_set_driver(driver_t *drvp)
{
    drvp->drv_dipfun[IOIF_CODE_OPEN] = uart_open;
    drvp->drv_dipfun[IOIF_CODE_CLOSE] = uart_close;
    drvp->drv_dipfun[IOIF_CODE_READ] = uart_read;
    drvp->drv_dipfun[IOIF_CODE_WRITE] = uart_write;
    drvp->drv_dipfun[IOIF_CODE_LSEEK] = uart_lseek;
    drvp->drv_dipfun[IOIF_CODE_IOCTRL] = uart_ioctrl;
    drvp->drv_dipfun[IOIF_CODE_DEV_START] = uart_dev_start;
    drvp->drv_dipfun[IOIF_CODE_DEV_STOP] = uart_dev_stop;
    drvp->drv_dipfun[IOIF_CODE_SET_POWERSTUS] = uart_set_powerstus;
    drvp->drv_dipfun[IOIF_CODE_ENUM_DEV] = uart_enum_dev;
    drvp->drv_dipfun[IOIF_CODE_FLUSH] = uart_flush;
    drvp->drv_dipfun[IOIF_CODE_SHUTDOWN] = uart_shutdown;
    drvp->drv_name = "uartdrv";
    return;
}

void uart0_set_device(device_t *devp, driver_t *drvp)
{

    devp->dev_flgs = DEVFLG_SHARE;
    devp->dev_stus = DEVSTS_NORML;
    devp->dev_id.dev_mtype = UART_DEVICE;
    devp->dev_id.dev_stype = 0;
    devp->dev_id.dev_nr = 0;

    devp->dev_name = "uart0";
    return;
}



drvstus_t uart_entry(driver_t *drvp, uint_t val, void *p)
{
    if (drvp == NULL)
    {
        return DFCERRSTUS;
    }
    device_t *devp = new_device_dsc();
    if (devp == NULL)
    {
        return DFCERRSTUS;
    }
    uart_set_driver(drvp);
    uart0_set_device(devp, drvp);
    if (krldev_add_driver(devp, drvp) == DFCERRSTUS)
    {
        if (del_device_dsc(devp) == DFCERRSTUS) //注意释放资源。
        {
            return DFCERRSTUS;
        }
        return DFCERRSTUS;
    }
    if (krlnew_device(devp) == DFCERRSTUS)
    {
        if (del_device_dsc(devp) == DFCERRSTUS) //注意释放资源
        {
            return DFCERRSTUS;
        }
        return DFCERRSTUS;
    }

    return DFCOKSTUS;
}

drvstus_t uart_exit(driver_t *drvp, uint_t val, void *p)
{
    return DFCERRSTUS;
}

drvstus_t uart_open(device_t *devp, void *iopack)
{
    return krldev_inc_devcount(devp);
}

drvstus_t uart_close(device_t *devp, void *iopack)
{
    return krldev_dec_devcount(devp);
}

drvstus_t uart_read(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_write(device_t *devp, void *iopack)
{

    uint_t len;
    buf_t retbuf;
    if (krldev_retn_rqueparm(iopack, &retbuf, NULL, &len, NULL, NULL, NULL) == DFCERRSTUS)
    {
        return DFCERRSTUS;
    }
    char_t *p = (char_t *)retbuf;
    kprint("%s", p);
    return DFCOKSTUS;
}

drvstus_t uart_lseek(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_ioctrl(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_dev_start(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_dev_stop(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_set_powerstus(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_enum_dev(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_flush(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t uart_shutdown(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}
