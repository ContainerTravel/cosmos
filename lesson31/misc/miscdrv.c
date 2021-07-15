
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/list.h>
#define DEV_NAME  "devicesinfo"
#define BUS_DEV_NAME  "devicesinfobus"

struct kset* ret_buskset(void);
struct subsys_private {
	struct kset subsys;
	struct kset *devices_kset;
	struct list_head interfaces;
	struct mutex mutex;

	struct kset *drivers_kset;
	struct klist klist_devices;
	struct klist klist_drivers;
	struct blocking_notifier_head bus_notifier;
	unsigned int drivers_autoprobe:1;
	struct bus_type *bus;

	struct kset glue_dirs;
	struct class *class;
};
#define to_subsys_private(obj) container_of(obj, struct subsys_private, subsys.kobj)


static int misc_find_match(struct device *dev, void *data)
{
	struct bus_type* b = (struct bus_type*)data;
	printk(KERN_EMERG "%s---->device name is:%s\n", b->name, dev->kobj.name);
	return 0;
}

static ssize_t misc_read (struct file *pfile, char __user *buff, size_t size, loff_t *off)
{
	struct kobject* kobj;
	struct kset* kset;
	struct subsys_private* p;
	kset = ret_buskset();
	if(!kset)
		return 0;
	printk(KERN_EMERG "line:%d,%s is call\n",__LINE__,__FUNCTION__);
	
	list_for_each_entry(kobj, &kset->list, entry)
	{
		p = to_subsys_private(kobj);
		printk(KERN_EMERG "Bus name is:%s\n",p->bus->name);
		bus_for_each_dev(p->bus, NULL, p->bus, misc_find_match);
		
	}
	return 0;
}
 
static ssize_t misc_write(struct file *pfile, const char __user *buff, size_t size, loff_t *off)
{
	printk(KERN_EMERG "line:%d,%s is call\n",__LINE__,__FUNCTION__);	
	return 0;
}
 
static int  misc_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_EMERG "line:%d,%s is call\n",__LINE__,__FUNCTION__);
 
	return 0;
}
 
static int misc_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_EMERG "line:%d,%s is call\n",__LINE__,__FUNCTION__);
 
	return 0;
}

static int devicesinfo_bus_match(struct device *dev, struct device_driver *driver)
{
        return !strncmp(dev->kobj.name, driver->name, strlen(driver->name));
}

static const  struct file_operations misc_fops = {
	.read     = misc_read,
	.write    = misc_write,
	.release  = misc_release,
    .open     = misc_open,
};

static struct miscdevice  misc_dev =  {
    .fops  =  &misc_fops,         /* 设备文件操作方法 */
    .minor =  255,                /* 次设备号 */
    .name  =  DEV_NAME,           /* 设备名/dev/下的设备节点名   */
};

struct bus_type devicesinfo_bus = {
        .name = BUS_DEV_NAME, //总线名字
        .match = devicesinfo_bus_match, //总线match函数指针
};

static int busok = 1;

struct kset *ret_buskset(void)
{
	struct subsys_private *p;
	if(busok)
		return NULL;
	if(!devicesinfo_bus.p)
		return NULL;

	p = devicesinfo_bus.p;

	if(!p->subsys.kobj.kset)
		return NULL;
	return p->subsys.kobj.kset;
}


static int __init miscdrv_init(void)
{
    misc_register(&misc_dev);
	printk(KERN_EMERG "INIT misc busok\n");
	busok = bus_register(&devicesinfo_bus);
    printk(KERN_EMERG "INIT misc busok = %x buskset:%llx\n", busok, (long long)ret_buskset());
    return 0;
}

static void  __exit miscdrv_exit(void)
{
    misc_deregister(&misc_dev);
	bus_unregister(&devicesinfo_bus);
    printk(KERN_EMERG "EXIT,misc\n");
}
 
module_init(miscdrv_init);
module_exit(miscdrv_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("LMOS");
