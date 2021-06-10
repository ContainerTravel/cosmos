#ifndef _FILE_H
#define _FILE_H
void init_file();
void exit_file();
int limg_newfile(const char *pathname, int flags, mode_t mode);
int limg_openfile(const char *pathname, int flags);
int limg_closefile(int fd);
ssize_t limg_readfile(int fd, void *buf, size_t count);
ssize_t limg_writefile(int fd, const void *buf, size_t count);
off_t limg_lseekfile(int fd, off_t offset, int whence);
uint_t limg_retszfile(const char *pathname);
sint_t limg_retfcurpos(int fd);
uint_t limg_createfile_setval(char* file,size_t blksz,uint_t blknr,int setval);
#endif
