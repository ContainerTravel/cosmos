/**********************************************************
		转换显示字符串头文件vgastr.h
***********************************************************
				彭东
**********************************************************/

#ifndef _VGASTR_H
#define _VGASTR_H

void init_curs();
void set_curs(u32_t x, u32_t y);
void GxH_strwrite(char_t *str, cursor_t *cursptr);
char *numberk(char_t *str, uint_t n, sint_t base);
void clear_screen(u16_t srrv);
void put_one_char(char_t cr, uint_t x, uint_t y);
void close_curs();
void vsprintfk(char_t *buf, const char_t *fmt, va_list_t args);
void kprint(const char_t *fmt, ...);

KLINE void current_curs(cursor_t *cp, u32_t c_flg)
{

	if (c_flg == VGACHAR_LR_CFLG)
	{
		cp->y++;
		cp->x = 0;
		if (cp->y > 24)
		{
			cp->y = 0;
			clear_screen(VGADP_DFVL);
		}

		return;
	}
	if (c_flg == VGACHAR_DF_CFLG)
	{
		cp->x += 2;
		if (cp->x > 159)
		{
			cp->x = 0;
			cp->y++;
			if (cp->y > 24)
			{
				cp->y = 0;
				clear_screen(VGADP_DFVL);
			}
			return;
		}
	}
	return;
}
#endif
