#ifndef _PARAM_H
#define _PARAM_H
#define KRNEL_MODE 1
#define BOOT_MODE 2
#define LDSK_MODE 3
#define UNDO_MODE 4
typedef struct s_mparam
{
    int mp_argc;
    char **mp_argv;
    uint_t mp_sifnr;
    uint_t mp_eifnr;
    uint_t mp_sofnr;
    uint_t mp_ifcurrnr;
    uint_t mp_ildrhnr;
    uint_t mp_imgmode;
    uint_t mp_ifnr;
} mparam_t;

void init_param();
void exit_param();
void mparam_t_init(mparam_t *initp);
void limg_param(int argc, char *argv[]);
void limg_param_is_inputfile();
void limg_param_is_outfile();
void limg_param_is_ldhfile();
void limg_param_is_mode();
uint_t limg_ret_allfilblk();
uint_t limg_ret_allifimglen();
uint_t limg_ret_allinfilesz();
char *limg_retnext_ipathname();
char *limg_ret_ldrhpathname();
char *limg_retnext_opathname();
uint_t limg_ret_infilenr();
uint_t limg_ret_imgmode();
#endif
