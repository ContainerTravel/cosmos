#include "libtypes.h"
#include "libdev.h"
#include "stdio.h"
typedef struct shellkbbuff
{
	unsigned long skb_tail;
	unsigned long skb_start;
	unsigned long skb_curr;
	char skb_buff[512];
}shellkbbuff_t;

static shellkbbuff_t shellkbuff;

hand_t open_keyboard()
{
	devid_t dev;
	dev.dev_mtype = UART_DEVICE;
	dev.dev_stype = 0;
	dev.dev_nr = 0;

	hand_t fd = open(&dev, RW_FLG | FILE_TY_DEV, 0);
	if (fd == -1)
	{
		return -1;
	}
	return fd;
}

void close_keyboard(hand_t hand)
{
	close(hand);
	return;
}

u16_t read_keyboard_code(sint_t hand)
{
	u16_t code_buf[8];
	sysstus_t rets = read(hand, (char_t*)code_buf, 8, 0);
	if(0 > rets)
	{
		printf(" read err:%x\n", rets);
	}
	if(0 == code_buf[0])
	{
		return code_buf[2];
	}
	return 0xffff;	
}

void skb_buff_write(shellkbbuff_t* skb, char_t code)
{
	if(skb->skb_tail >= 511)
	{
		skb->skb_tail = 0;
	}

	skb->skb_buff[skb->skb_tail] = code;
	skb->skb_tail++;
	skb->skb_buff[skb->skb_tail] = 0;
	return;
}

void skb_buff_disp(shellkbbuff_t* skb)
{
	if(skb->skb_curr >= 511)
	{
		skb->skb_curr = 0;
	}
	printf("%s", &skb->skb_buff[skb->skb_curr]);
	skb->skb_curr++;
	return;
}

void skb_buff_clear(shellkbbuff_t* skb)
{
	memset((void*)skb, 0, sizeof(shellkbbuff_t));
	return;
}

sint_t shc_cmd_run(char_t* cmdstr)
{
	if(NULL == cmdstr)
	{
		return -1;
	}
	if(exel(cmdstr, 0) != SYSSTUSERR)
	{
		return 0;
	}
	return -2;
}

void skb_buff_enter(shellkbbuff_t* skb)
{
	printf("\n");
	if(0 == shc_cmd_run(&skb->skb_buff[skb->skb_start]))
	{
		return;
	}
	printf("Cosmos@LMOS:>");
	return;
}

void skb_buff_backspace(shellkbbuff_t* skb)
{
	if(1 > skb->skb_tail)
	{
		return;
	}
	if(0 < skb->skb_curr)
	{
		skb->skb_curr--;
	}
	skb->skb_tail--;
	skb->skb_buff[skb->skb_tail] = 0;
	printf("Cosmos@LMOS:>");
	printf("%s", &skb->skb_buff[skb->skb_start]);
	return;	
}


void shell()
{
	hand_t hand = -1;
	char_t charbuff[2] = {0,0};
	u16_t kbcode; 
	hand = open_keyboard();
	memset((void*)&shellkbuff, 0, sizeof(shellkbbuff_t));
	
	printf("Cosmos shell is init!\n");

	printf("Cosmos@LMOS:>");
	for(;;)
	{
		kbcode = read_keyboard_code(hand);
		if(32 <= kbcode && 127 >= kbcode)
		{
			charbuff[0] = (char_t)kbcode;
			skb_buff_write(&shellkbuff, charbuff[0]);
			skb_buff_disp(&shellkbuff);
			continue;
		}
		if(0x103 == kbcode)
		{
			skb_buff_enter(&shellkbuff);
			
			skb_buff_clear(&shellkbuff);
			continue;
		}
		if(0x104 == kbcode)
		{
			skb_buff_backspace(&shellkbuff);		
			continue;
		}
	}
	close_keyboard(hand);
	return;
}

int main(void)
{
    shell();
    return 0;
}