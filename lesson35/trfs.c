#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MAXLEN 32
#define MAX_FILES	1024
#define MAX_BLOCKSIZE  4096

// 定义每一个目录项的格式
struct dir_entry {
	char filename[MAXLEN];
	uint8_t idx;
};

// 定义每一个文件的格式。
struct fileinfo {
	uint8_t	busy;
	mode_t mode;
	uint8_t idx;
	int datalen;
	union {
		uint8_t file_size;
		uint8_t dir_children;
	};
	void* data;
};


struct fileinfo finfo_arr[MAX_FILES+1];
int curr_count = 0; // 我勒个去，竟然使用了全局变量！

// 获得一个尚未使用的文件块，保存新创建的文件或者目录
static int get_finfo_arr(void)
{
	int i;

	// 就是一个遍历，但实现快速。
	for (i = 2; i < MAX_FILES; i++) {
		if (!finfo_arr[i].busy) {
			finfo_arr[i].busy = 1;
			return i;
		}
	}
	return -1;
}

static struct inode_operations trfs_inode_ops;
// 读取目录的实现
static int trfs_readdir(struct file *filp, struct dir_context *ctx)
{
	loff_t pos;
	struct fileinfo *blk;
	struct dir_entry *entry;
	int i;

	pos = filp->f_pos;
	if (pos)
		return 0;

	blk = (struct fileinfo *)filp->f_path.dentry->d_inode->i_private;

	if (!S_ISDIR(blk->mode)) {
		return -ENOTDIR;
	}

	// 循环获取一个目录的所有文件的文件名
	entry = (struct dir_entry *)&blk->data;
	for (i = 0; i < blk->dir_children; i++) {
		if(!dir_emit(ctx, entry[i].filename, MAXLEN, entry[i].idx, DT_UNKNOWN))
			return 0;
		ctx->pos++;
		filp->f_pos += sizeof(struct dir_entry);
	}

	return 0;
}

// read实现
ssize_t trfs_read(struct file * filp, char __user * buf, size_t len, loff_t *ppos)
{
	struct fileinfo *blk;
	char *buffer;

	blk = (struct fileinfo *)filp->f_path.dentry->d_inode->i_private;
	if (*ppos >= blk->file_size)
		return 0;

	buffer = (char *)blk->data;
	len = min((size_t) blk->file_size, len);

	if (copy_to_user(buf, buffer, len)) {
		return -EFAULT;
	}
	*ppos += len;
	printk(KERN_EMERG"trfs is read\n");
	return len;
}

// write实现
ssize_t trfs_write(struct file * filp, const char __user * buf, size_t len, loff_t * ppos)
{
	struct fileinfo *blk;
	char *buffer;

	blk = filp->f_path.dentry->d_inode->i_private;

	buffer = (char *)blk->data;
	buffer += *ppos;

	if (copy_from_user(buffer, buf, len)) {
		return -EFAULT;
	}
	*ppos += len;
	blk->file_size = *ppos;
	printk(KERN_EMERG"trfs is write\n");
	return len;
}

const struct file_operations trfs_file_operations = {
	.read = trfs_read,
	.write = trfs_write,
};

const struct file_operations trfs_dir_operations = {
	.owner = THIS_MODULE,
	.iterate_shared  = trfs_readdir,
};

// 创建文件的实现
static int trfs_do_create(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	struct inode *inode;
	struct super_block *sb;
	struct dir_entry *entry;
	struct fileinfo *blk, *pblk;
	int idx;

	sb = dir->i_sb;

	if (curr_count >= MAX_FILES) {
		return -ENOSPC;
	}

	if (!S_ISDIR(mode) && !S_ISREG(mode)) {
		return -EINVAL;
	}

	inode = new_inode(sb);
	if (!inode) {
		return -ENOMEM;
	}

	inode->i_sb = sb;
	inode->i_op = &trfs_inode_ops;
	inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);

	idx = get_finfo_arr(); // 获取一个空闲的文件块保存新文件

	blk = &finfo_arr[idx];
	inode->i_ino = idx;
	blk->mode = mode;
	curr_count ++;

	if (S_ISDIR(mode)) {
		blk->dir_children = 0;
		inode->i_fop = &trfs_dir_operations;
	} else if (S_ISREG(mode)) {
		blk->file_size = 0;
		inode->i_fop = &trfs_file_operations;
	}

	inode->i_private = blk;
	pblk = (struct fileinfo *)dir->i_private;

	entry = (struct dir_entry *)pblk->data;
	entry += pblk->dir_children;
	pblk->dir_children ++;

	entry->idx = idx;
	strcpy(entry->filename, dentry->d_name.name);

	// VFS穿针引线的关键步骤，将VFS的inode链接到链表
	inode_init_owner(inode, dir, mode); 
	d_add(dentry, inode);
	printk(KERN_EMERG"trfs is create\n");
	return 0;
}

static int trfs_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	return trfs_do_create(dir, dentry, S_IFDIR | mode);
}

static int trfs_create(struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
{
	return trfs_do_create(dir, dentry, mode);
}

static struct inode *trfs_iget(struct super_block *sb, int idx)
{
	struct inode *inode;
	struct fileinfo *blk;

	inode = new_inode(sb);
	inode->i_ino = idx;
	inode->i_sb = sb;
	inode->i_op = &trfs_inode_ops;

	blk = &finfo_arr[idx];

	if (S_ISDIR(blk->mode))
		inode->i_fop = &trfs_dir_operations;
	else if (S_ISREG(blk->mode))
		inode->i_fop = &trfs_file_operations;

	inode->i_atime = inode->i_mtime = inode->i_ctime = current_time(inode);
	inode->i_private = blk;

	return inode;
}

struct dentry *trfs_lookup(struct inode *parent_inode, struct dentry *child_dentry, unsigned int flags)
{
	struct super_block *sb = parent_inode->i_sb;
	struct fileinfo *blk;
	struct dir_entry *entry;
	int i;

	blk = (struct fileinfo *)parent_inode->i_private;
	entry = (struct dir_entry *)blk->data;
	for (i = 0; i < blk->dir_children; i++) {
		if (!strcmp(entry[i].filename, child_dentry->d_name.name)) {
			struct inode *inode = trfs_iget(sb, entry[i].idx);
			struct fileinfo *inner = (struct fileinfo *)inode->i_private;
			inode_init_owner(inode, parent_inode, inner->mode);
			d_add(child_dentry, inode);
			return NULL;
		}
	}

	return NULL;
}

int trfs_rmdir(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;
	struct fileinfo *blk = (struct fileinfo *)inode->i_private;

	blk->busy = 0;
	return simple_rmdir(dir, dentry);
}

int trfs_unlink(struct inode *dir, struct dentry *dentry)
{
	int i;
	struct inode *inode = dentry->d_inode;
	struct fileinfo *blk = (struct fileinfo *)inode->i_private;
	struct fileinfo *pblk = (struct fileinfo *)dir->i_private;
	struct dir_entry *entry;

	// 更新其上层目录
	entry = (struct dir_entry *)pblk->data;
	for (i = 0; i < pblk->dir_children; i++) {
		if (!strcmp(entry[i].filename, dentry->d_name.name)) {
			int j;
			for (j = i; j < pblk->dir_children - 1; j++) {
				memcpy(&entry[j], &entry[j+1], sizeof(struct dir_entry));
			}
			pblk->dir_children --;
			break;
		}
	}

	blk->busy = 0;
	return simple_unlink(dir, dentry);
}

static struct inode_operations trfs_inode_ops = {
	.create = trfs_create,
	.lookup = trfs_lookup,
	.mkdir = trfs_mkdir,
	.rmdir = trfs_rmdir,
	.unlink = trfs_unlink,
};

int trfs_fill_super(struct super_block *sb, void *data, int silent)
{
	struct inode *root_inode;
	int mode = S_IFDIR;

	root_inode = new_inode(sb);
	root_inode->i_ino = 1;
	inode_init_owner(root_inode, NULL, mode);
	root_inode->i_sb = sb;
	root_inode->i_op = &trfs_inode_ops;
	root_inode->i_fop = &trfs_dir_operations;
	root_inode->i_atime = root_inode->i_mtime = root_inode->i_ctime = current_time(root_inode);

	finfo_arr[1].mode = mode;
	finfo_arr[1].dir_children = 0;
	finfo_arr[1].idx = 1;
	finfo_arr[1].busy = 1;
	root_inode->i_private = &finfo_arr[1];

	sb->s_root = d_make_root(root_inode);
	curr_count ++;
	printk(KERN_EMERG"trfs is new rootdir\n");
	return 0;
}

static struct dentry *trfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data)
{
	printk(KERN_EMERG"trfs is mount\n");
	return mount_nodev(fs_type, flags, data, trfs_fill_super);
}

static void trfs_kill_superblock(struct super_block *sb)
{
	kill_anon_super(sb);
}

struct file_system_type trfs_fs_type = {
	.owner = THIS_MODULE,
	.name = "trfs",
	.mount = trfs_mount,
	.kill_sb = trfs_kill_superblock,
};

void init_fileinfo(void)
{
	int i;
	memset(finfo_arr, 0, sizeof(finfo_arr));
	for (i = 0; i < MAX_FILES; i++)
	{
		finfo_arr[i].data = kzalloc(MAX_BLOCKSIZE, GFP_ATOMIC);
		if(!finfo_arr[i].data)
			printk(KERN_EMERG"init_trfs alloc data failed\n");
	}
	return;
}

void exit_fileinfo(void)
{
	int i;
	for (i = 0; i < MAX_FILES; i++)
	{
		if(finfo_arr[i].data)
			kfree(finfo_arr[i].data);
	}
	return;
}


static int trfs_init(void)
{
	int ret;
	init_fileinfo();
	
	ret = register_filesystem(&trfs_fs_type);
	if (ret)
		printk(KERN_EMERG"register trfs failed\n");
	printk(KERN_EMERG"trfs is ok\n");
	return ret;
}

static void trfs_exit(void)
{
	exit_fileinfo();
	unregister_filesystem(&trfs_fs_type);
}

module_init(trfs_init);
module_exit(trfs_exit);

MODULE_LICENSE("GPL");
