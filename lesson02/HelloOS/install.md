# 把下面的代码复制，然后插入到/boot/grub/grub.cfg文件中 
# 然后把HelloOS.bin文件复制到/boot/目录下
# 最后重启计算机就可以看到HelloOS启动选项了


menuentry 'HelloOS' {
    insmod part_msdos
    insmod ext2
    set root='hd0,msdos4' #注意boot目录挂载的分区，这是我机器上的情况
    multiboot2 /boot/HelloOS.bin
    boot
}