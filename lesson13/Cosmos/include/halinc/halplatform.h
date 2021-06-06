/**********************************************************
        平台相关的宏定义文件halplatform.h
***********************************************************
                彭东
**********************************************************/
#ifndef _PLATFORM_H
#define _PLATFORM_H
adr_t viradr_to_phyadr(adr_t kviradr);
adr_t phyadr_to_viradr(adr_t kphyadr);
void machbstart_t_init(machbstart_t *initp);
void init_machbstart();
void init_halplaltform();
fhdsc_t *get_fileinfo(char_t *fname, machbstart_t *mbsp);
void get_file_rvadrandsz(char_t *fname, machbstart_t *mbsp, u64_t *retadr, u64_t *retsz);
void move_img2maxpadr(machbstart_t *mbsp);
int adrzone_is_ok(u64_t sadr, u64_t slen, u64_t kadr, u64_t klen);
int initchkadr_is_ok(machbstart_t *mbsp, u64_t chkadr, u64_t cksz);

#endif
