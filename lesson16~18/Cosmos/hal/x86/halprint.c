/**********************************************************
		屏幕上格式化输出文件halprint.c
***********************************************************
				彭东
**********************************************************/

#include "cosmostypes.h"
#include "cosmosmctrl.h"

void kprint(const char_t *fmt, ...)
{
	char_t buf[512];
	va_list ap;
	va_start(ap, fmt);
	vsprintfk(buf, fmt, ap);
	gstr_write(&kdftgh, buf);
	va_end(ap);
	return;
}

char_t *strcopyk(char_t *buf, char_t *str_s)
{
	while (*str_s)
	{
		*buf = *str_s;
		buf++;
		str_s++;
	}
	return buf;
}

void vsprintfk(char_t *buf, const char_t *fmt, va_list args)
{
	char_t *p = buf;
	while (*fmt)
	{
		if (*fmt != '%')
		{
			*p++ = *fmt++;
			continue;
		}
		fmt++;
		switch (*fmt)
		{
		case 'x':
			p = numberk(p, va_arg(args, uint_t), 16);
			fmt++;
			break;
		case 'd':
			p = numberk(p, va_arg(args, uint_t), 10);
			fmt++;
			break;
		case 's':
			p = strcopyk(p, (char_t *)va_arg(args, uint_t));
			fmt++;
			break;
		default:
			break;
		}
	}
	*p = 0;
	return;
}

char_t *numberk(char_t *str, uint_t n, sint_t base)
{
	register char_t *p;
	char_t strbuf[36];
	p = &strbuf[36];
	*--p = 0;
	if (n == 0)
	{
		*--p = '0';
	}
	else
	{
		do
		{
			*--p = "0123456789abcdef"[n % base];
		} while (n /= base);
	}
	while (*p != 0)
	{
		*str++ = *p++;
	}
	return str;
}
