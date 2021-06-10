#include "imgmctrl.h"

void init_file()
{
    return;
}

void exit_file()
{
    return;
}

int limg_newfile(const char *pathname, int flags, mode_t mode)
{
    return open(pathname, flags, mode);
}
int limg_openfile(const char *pathname, int flags)
{
    return open(pathname, flags);
}

int limg_closefile(int fd)
{
    return close(fd);
}

ssize_t limg_readfile(int fd, void *buf, size_t count)
{
    return read(fd, buf, count);
}

ssize_t limg_writefile(int fd, const void *buf, size_t count)
{
    return write(fd, buf, count);
}

off_t limg_lseekfile(int fd, off_t offset, int whence)
{
    return lseek(fd, offset, whence);
}

uint_t limg_retszfile(const char *pathname)
{
    struct stat statbuf;
    if (stat(pathname, &statbuf) < 0)
    {
        return 0;
    }
    return (unsigned long)statbuf.st_size;
}

sint_t limg_retfcurpos(int fd)
{
    off_t retf = limg_lseekfile(fd, 0, SEEK_CUR);
    if (retf == -1)
    {
        return -1;
    }
    return (sint_t)retf;
}
uint_t limg_createfile_setval(char *file, size_t blksz, uint_t blknr, int setval)
{
    uint_t retsz = 0;
    ssize_t sz = 0;
    int fd = limg_newfile(file, O_RDWR | O_CREAT | O_TRUNC, 0777);
    if (fd == -1)
    {
        return 0;
    }
    void *retbs = NULL, *buf = img_mem(blksz, NULL, MFLG_ALLOC);
    if (buf == NULL)
    {
        if (limg_closefile(fd) == -1)
        {
            limg_error("close file");
            return 0;
        }
        return 0;
    }
    limg_memclr(buf, setval, blksz);
    for (uint_t wi = 0; wi < blknr; wi++)
    {
        sz = limg_writefile(fd, buf, blksz);
        if (sz == -1)
        {
            if (limg_closefile(fd) == -1)
            {
                limg_error("close file");
                return 0;
            }
            retbs = img_mem(0, buf, MFLG_FREE);
            if (retbs == NULL)
            {
                limg_error("free mem");
                return 0;
            }
            return 0;
        }
        retsz += (uint_t)sz;
    }
    if (limg_closefile(fd) == -1)
    {
        limg_error("close file");
        return 0;
    }
    retbs = img_mem(0, buf, MFLG_FREE);
    if (retbs == NULL)
    {
        limg_error("free mem");
        return 0;
    }
    return retsz;
}
