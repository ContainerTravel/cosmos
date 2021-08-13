/**********************************************************
        串口驱动文件drvuart.c
***********************************************************
                彭东
**********************************************************/
#include "cosmostypes.h"
#include "cosmosmctrl.h"

static u16_t keymap[NR_SCAN_CODES * MAP_COLS] = {
/* scan-code			!Shift		Shift		E0 XX	*/
/* ==================================================================== */
/* 0x00 - none		*/	0,		0,		0,
/* 0x01 - ESC		*/	ESC,		ESC,		0,
/* 0x02 - '1'		*/	'1',		'!',		0,
/* 0x03 - '2'		*/	'2',		'@',		0,
/* 0x04 - '3'		*/	'3',		'#',		0,
/* 0x05 - '4'		*/	'4',		'$',		0,
/* 0x06 - '5'		*/	'5',		'%',		0,
/* 0x07 - '6'		*/	'6',		'^',		0,
/* 0x08 - '7'		*/	'7',		'&',		0,
/* 0x09 - '8'		*/	'8',		'*',		0,
/* 0x0A - '9'		*/	'9',		'(',		0,
/* 0x0B - '0'		*/	'0',		')',		0,
/* 0x0C - '-'		*/	'-',		'_',		0,
/* 0x0D - '='		*/	'=',		'+',		0,
/* 0x0E - BS		*/	BACKSPACE,	BACKSPACE,	0,
/* 0x0F - TAB		*/	TAB,		TAB,		0,
/* 0x10 - 'q'		*/	'q',		'Q',		0,
/* 0x11 - 'w'		*/	'w',		'W',		0,
/* 0x12 - 'e'		*/	'e',		'E',		0,
/* 0x13 - 'r'		*/	'r',		'R',		0,
/* 0x14 - 't'		*/	't',		'T',		0,
/* 0x15 - 'y'		*/	'y',		'Y',		0,
/* 0x16 - 'u'		*/	'u',		'U',		0,
/* 0x17 - 'i'		*/	'i',		'I',		0,
/* 0x18 - 'o'		*/	'o',		'O',		0,
/* 0x19 - 'p'		*/	'p',		'P',		0,
/* 0x1A - '['		*/	'[',		'{',		0,
/* 0x1B - ']'		*/	']',		'}',		0,
/* 0x1C - CR/LF		*/	ENTER,		ENTER,		PAD_ENTER,
/* 0x1D - l. Ctrl	*/	CTRL_L,		CTRL_L,		CTRL_R,
/* 0x1E - 'a'		*/	'a',		'A',		0,
/* 0x1F - 's'		*/	's',		'S',		0,
/* 0x20 - 'd'		*/	'd',		'D',		0,
/* 0x21 - 'f'		*/	'f',		'F',		0,
/* 0x22 - 'g'		*/	'g',		'G',		0,
/* 0x23 - 'h'		*/	'h',		'H',		0,
/* 0x24 - 'j'		*/	'j',		'J',		0,
/* 0x25 - 'k'		*/	'k',		'K',		0,
/* 0x26 - 'l'		*/	'l',		'L',		0,
/* 0x27 - ';'		*/	';',		':',		0,
/* 0x28 - '\''		*/	'\'',		'"',		0,
/* 0x29 - '`'		*/	'`',		'~',		0,
/* 0x2A - l. SHIFT	*/	SHIFT_L,	SHIFT_L,	0,
/* 0x2B - '\'		*/	'\\',		'|',		0,
/* 0x2C - 'z'		*/	'z',		'Z',		0,
/* 0x2D - 'x'		*/	'x',		'X',		0,
/* 0x2E - 'c'		*/	'c',		'C',		0,
/* 0x2F - 'v'		*/	'v',		'V',		0,
/* 0x30 - 'b'		*/	'b',		'B',		0,
/* 0x31 - 'n'		*/	'n',		'N',		0,
/* 0x32 - 'm'		*/	'm',		'M',		0,
/* 0x33 - ','		*/	',',		'<',		0,
/* 0x34 - '.'		*/	'.',		'>',		0,
/* 0x35 - '/'		*/	'/',		'?',		PAD_SLASH,
/* 0x36 - r. SHIFT	*/	SHIFT_R,	SHIFT_R,	0,
/* 0x37 - '*'		*/	'*',		'*',    	0,
/* 0x38 - ALT		*/	ALT_L,		ALT_L,  	ALT_R,
/* 0x39 - ' '		*/	' ',		' ',		0,
/* 0x3A - CapsLock	*/	CAPS_LOCK,	CAPS_LOCK,	0,
/* 0x3B - F1		*/	F1,		F1,		0,
/* 0x3C - F2		*/	F2,		F2,		0,
/* 0x3D - F3		*/	F3,		F3,		0,
/* 0x3E - F4		*/	F4,		F4,		0,
/* 0x3F - F5		*/	F5,		F5,		0,
/* 0x40 - F6		*/	F6,		F6,		0,
/* 0x41 - F7		*/	F7,		F7,		0,
/* 0x42 - F8		*/	F8,		F8,		0,
/* 0x43 - F9		*/	F9,		F9,		0,
/* 0x44 - F10		*/	F10,		F10,		0,
/* 0x45 - NumLock	*/	NUM_LOCK,	NUM_LOCK,	0,
/* 0x46 - ScrLock	*/	SCROLL_LOCK,	SCROLL_LOCK,	0,
/* 0x47 - Home		*/	PAD_HOME,	'7',		HOME,
/* 0x48 - CurUp		*/	PAD_UP,		'8',		UP,
/* 0x49 - PgUp		*/	PAD_PAGEUP,	'9',		PAGEUP,
/* 0x4A - '-'		*/	PAD_MINUS,	'-',		0,
/* 0x4B - Left		*/	PAD_LEFT,	'4',		LEFT,
/* 0x4C - MID		*/	PAD_MID,	'5',		0,
/* 0x4D - Right		*/	PAD_RIGHT,	'6',		RIGHT,
/* 0x4E - '+'		*/	PAD_PLUS,	'+',		0,
/* 0x4F - End		*/	PAD_END,	'1',		END,
/* 0x50 - Down		*/	PAD_DOWN,	'2',		DOWN,
/* 0x51 - PgDown	*/	PAD_PAGEDOWN,	'3',		PAGEDOWN,
/* 0x52 - Insert	*/	PAD_INS,	'0',		INSERT,
/* 0x53 - Delete	*/	PAD_DOT,	'.',		DELETE,
/* 0x54 - Enter		*/	0,		0,		0,
/* 0x55 - ???		*/	0,		0,		0,
/* 0x56 - ???		*/	0,		0,		0,
/* 0x57 - F11		*/	F11,		F11,		0,	
/* 0x58 - F12		*/	F12,		F12,		0,	
/* 0x59 - ???		*/	0,		0,		0,	
/* 0x5A - ???		*/	0,		0,		0,	
/* 0x5B - ???		*/	0,		0,		GUI_L,	
/* 0x5C - ???		*/	0,		0,		GUI_R,	
/* 0x5D - ???		*/	0,		0,		APPS,	
/* 0x5E - ???		*/	0,		0,		0,	
/* 0x5F - ???		*/	0,		0,		0,
/* 0x60 - ???		*/	0,		0,		0,
/* 0x61 - ???		*/	0,		0,		0,	
/* 0x62 - ???		*/	0,		0,		0,	
/* 0x63 - ???		*/	0,		0,		0,	
/* 0x64 - ???		*/	0,		0,		0,	
/* 0x65 - ???		*/	0,		0,		0,	
/* 0x66 - ???		*/	0,		0,		0,	
/* 0x67 - ???		*/	0,		0,		0,	
/* 0x68 - ???		*/	0,		0,		0,	
/* 0x69 - ???		*/	0,		0,		0,	
/* 0x6A - ???		*/	0,		0,		0,	
/* 0x6B - ???		*/	0,		0,		0,	
/* 0x6C - ???		*/	0,		0,		0,	
/* 0x6D - ???		*/	0,		0,		0,	
/* 0x6E - ???		*/	0,		0,		0,	
/* 0x6F - ???		*/	0,		0,		0,	
/* 0x70 - ???		*/	0,		0,		0,	
/* 0x71 - ???		*/	0,		0,		0,	
/* 0x72 - ???		*/	0,		0,		0,	
/* 0x73 - ???		*/	0,		0,		0,	
/* 0x74 - ???		*/	0,		0,		0,	
/* 0x75 - ???		*/	0,		0,		0,	
/* 0x76 - ???		*/	0,		0,		0,	
/* 0x77 - ???		*/	0,		0,		0,	
/* 0x78 - ???		*/	0,		0,		0,	
/* 0x78 - ???		*/	0,		0,		0,	
/* 0x7A - ???		*/	0,		0,		0,	
/* 0x7B - ???		*/	0,		0,		0,	
/* 0x7C - ???		*/	0,		0,		0,	
/* 0x7D - ???		*/	0,		0,		0,	
/* 0x7E - ???		*/	0,		0,		0,
/* 0x7F - ???		*/	0,		0,		0
};

static u8_t kbd_dowup_stat = 0;

void init_i8042()
{

    // wait_kb_buf(B_K_STUS_I_B_F);
    // out_u8(KB_CMD_REG,KB842_OPENIF);
    //init_ps2mouse();
    // kb_reset();
    // kb_self_check();
    // wait_kb_buf(B_K_STUS_I_B_F);
    // out_u8(KB_CMD_REG,KB842_OPENIF);
    // //kbwrt_cmdbyte(KB842_OPENIF);
    // kbwrt_cmdbyte(0x47);
//    // 通知 8042,下个字节的发向 0x60 的数据应放向 8042 的命令寄存器
//    wait_kb_buf(B_K_STUS_I_B_F);
//    out_u8( 0x64 , 0x60 ) ;

//    // 许可键盘及 鼠标 接口及中断
//    wait_kb_buf(B_K_STUS_I_B_F);
//    out_u8( 0x60 , 0x57 ) ;

    //kbred_cmdbyte(0x41);

    return;
}

void kboard_t_init(kboard_t* initp)
{
    list_init(&initp->kb_list);
    initp->kb_flg = 0;
    initp->kb_stus = 0;
    krlspinlock_init(&initp->kb_lock);
    krlsem_t_init(&initp->kb_sem);
    krlsem_set_sem(&initp->kb_sem, SEM_FLG_MUTEX, 0);
    initp->kb_btail = 0;
    hal_memset((void*)initp->kb_buff, 0, sizeof(initp->kb_buff[64]));
    return; 
}

void init_kboard(device_t* dev)
{
    void* ext;
    ext = (void*)krlnew(sizeof(kboard_t));

    if(NULL == ext)
    {
        kprint("init kboard fail\n");
        return;
    }
    dev->dev_extdata = ext;
	kboard_t_init((kboard_t*)dev->dev_extdata);
	init_i8042();
    return;
}



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

uint_t read_keybd()
{
    cpuflg_t flags;
    u8_t scan;

    save_flags_cli(&flags);

    scan=in_u8(KB_DATA_REG);
 
    restore_flags_sti(&flags);
    return (uint_t) scan;
}

u16_t kbd_map_code(u16_t scan, u16_t cols)
{
    return keymap[scan * cols];
}

void kbd_write_scan(kboard_t* ikbd, u8_t scan)
{
    if(0 == (scan & (1 << 7)))
    {
        ikbd->kb_buff[0] = scan;
        ikbd->kb_buff[1] = 0;
        ikbd->kb_buff[2] = kbd_map_code(scan, 3);
        kbd_dowup_stat = 1;
    }
    if(0x80 == (scan & (1 << 7)))
    {   
        ikbd->kb_buff[0] = 0;
        ikbd->kb_buff[1] = scan;
        ikbd->kb_buff[2] = kbd_map_code(scan & 0x7f, 3);
        kbd_dowup_stat = 2;
    }
    return;
}

drvstus_t uart_handle(uint_t ift_nr, void *devp, void *sframe)
{
    device_t* idev = (device_t*)devp;
    kboard_t* ikbd = (kboard_t*)idev->dev_extdata;
    u8_t scan;
    if(NULL == idev || NULL == ikbd)
    {
        return DFCERRSTUS;
    }
    scan = (u8_t)read_keybd();
    kbd_write_scan(ikbd, scan);
  
    krlsem_up(&ikbd->kb_sem);
	return DFCOKSTUS;
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
    
    if (krlnew_devhandle(devp, uart_handle, 0x21) == DFCERRSTUS)
    {

        return DFCERRSTUS; //注意释放资源。
    }
    init_kboard(devp);
    if (krlenable_intline(21) == DFCERRSTUS)
    {
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
    uint_t sz = 6; 
    kboard_t* kbd = (kboard_t*)devp->dev_extdata;
    uint_t len;
    buf_t retbuf;
    if (krldev_retn_rqueparm(iopack, &retbuf, NULL, &len, NULL, NULL, NULL) == DFCERRSTUS)
    {
        return DFCERRSTUS;
    }
    // kprint("uart_read:%x:%x\n",retbuf, len);

    krlsem_down(&kbd->kb_sem);
    
    if(sz < len)
    {
        sz = sz;
    }
    else
    {
        sz = len;
    }
    hal_memcpy((void*)kbd->kb_buff, retbuf, sz);
	return DFCOKSTUS;
    
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
