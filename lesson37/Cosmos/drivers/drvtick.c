/**********************************************************
        时钟管理文件krltick.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

void init_8254()
{

    out_u8_p(PTIPROTM, TIMEMODE);

    out_u8_p(PTIPROT1, HZLL);

    out_u8_p(PTIPROT1, HZHH);

    return;
}
void systick_set_driver(driver_t *drvp)
{
    drvp->drv_dipfun[IOIF_CODE_OPEN] = systick_open;
    drvp->drv_dipfun[IOIF_CODE_CLOSE] = systick_close;
    drvp->drv_dipfun[IOIF_CODE_READ] = systick_read;
    drvp->drv_dipfun[IOIF_CODE_WRITE] = systick_write;
    drvp->drv_dipfun[IOIF_CODE_LSEEK] = systick_lseek;
    drvp->drv_dipfun[IOIF_CODE_IOCTRL] = systick_ioctrl;
    drvp->drv_dipfun[IOIF_CODE_DEV_START] = systick_dev_start;
    drvp->drv_dipfun[IOIF_CODE_DEV_STOP] = systick_dev_stop;
    drvp->drv_dipfun[IOIF_CODE_SET_POWERSTUS] = systick_set_powerstus;
    drvp->drv_dipfun[IOIF_CODE_ENUM_DEV] = systick_enum_dev;
    drvp->drv_dipfun[IOIF_CODE_FLUSH] = systick_flush;
    drvp->drv_dipfun[IOIF_CODE_SHUTDOWN] = systick_shutdown;
    drvp->drv_name = "systick0drv";
    return;
}

void systick_set_device(device_t *devp, driver_t *drvp)
{

    devp->dev_flgs = DEVFLG_SHARE;
    devp->dev_stus = DEVSTS_NORML;
    devp->dev_id.dev_mtype = SYSTICK_DEVICE;
    devp->dev_id.dev_stype = 0;
    devp->dev_id.dev_nr = 0;

    devp->dev_name = "systick0";
    return;
}

drvstus_t systick_entry(driver_t *drvp, uint_t val, void *p)
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
    systick_set_driver(drvp);
    systick_set_device(devp, drvp);
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

    if (krlnew_devhandle(devp, systick_handle, 0x20) == DFCERRSTUS)
    {

        return DFCERRSTUS; //注意释放资源。
    }
    init_8254();

    if (krlenable_intline(20) == DFCERRSTUS)
    {
        return DFCERRSTUS;
    }
    return DFCOKSTUS;
}

drvstus_t systick_exit(driver_t *drvp, uint_t val, void *p)
{
    return DFCERRSTUS;
}

drvstus_t systick_handle(uint_t ift_nr, void *devp, void *sframe)
{
    krlthd_inc_tick(krlsched_retn_currthread());
    //kprint("systick_handle run devname:%s intptnr:%d\n", ((device_t *)devp)->dev_name, ift_nr);
    // hal_sysdie("systick_hand\n");
    return DFCOKSTUS;
}

drvstus_t systick_open(device_t *devp, void *iopack)
{
    krldev_inc_devcount(devp);
    return DFCOKSTUS;
}

drvstus_t systick_close(device_t *devp, void *iopack)
{
    krldev_dec_devcount(devp);
    return DFCOKSTUS;
}

drvstus_t systick_read(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_write(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_lseek(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_ioctrl(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_dev_start(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_dev_stop(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_set_powerstus(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_enum_dev(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_flush(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}

drvstus_t systick_shutdown(device_t *devp, void *iopack)
{
    return DFCERRSTUS;
}
