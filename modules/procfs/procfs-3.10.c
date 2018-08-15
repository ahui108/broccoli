#include <linux/module.h>  
#include <linux/kernel.h>  
#include <linux/init.h>  
#include <linux/proc_fs.h>  
#include <linux/jiffies.h>  
#include <asm/uaccess.h>  

#define MODULE_VERS "1.0"
#define MODULE_NAME "procfs_example"
#define FOOBAR_LEN 8  

struct fb_data_t {
	char name[FOOBAR_LEN + 1];  
	char value[FOOBAR_LEN + 1];  
};  

static struct proc_dir_entry *example_dir, *foo_file;
struct fb_data_t foo_data;

static int proc_read_foobar(char *page, char **start,  
	off_t off, int count,   
	int *eof, void *data)  
{  
	int len;  
	struct fb_data_t *fb_data = (struct fb_data_t *)data;
	/* DON'T DO THAT - buffer overruns are bad */
	len = sprintf(page, "%s = '%s'\n", fb_data->name, fb_data->value);  
	return len;  
}  

static int proc_write_foobar(struct file *file,  
	const char *buffer,  
	unsigned long count,   
	void *data)  
{  
	int len;  
	struct fb_data_t *fb_data = (struct fb_data_t *)data;
	if(count > FOOBAR_LEN)  
		len = FOOBAR_LEN;  
	else
		len = count;  
	if(copy_from_user(fb_data->name, buffer, len))  
		return -EFAULT;  
	fb_data->value[len] = '\0';  
	return len;  
}  

static int __init init_procfs_example(void)
{  
	int rv = 0;  
	/* create directory */
	example_dir = proc_mkdir(MODULE_NAME, NULL);  
	if(example_dir == NULL) {  
		rv = -ENOMEM;  
		goto out;  
	}  

	/* create foo and bar files using same callback  
	* functions   
    * create_proc_entry can be used before v3.10, after kernel v3.10, you should use proc_create
	*/
	foo_file = create_proc_entry("foo", 0644, example_dir);  
	if(foo_file == NULL) {  
		rv = -ENOMEM;  
		goto no_foo;  
	}  

	strcpy(foo_data.name, "foo");  
	strcpy(foo_data.value, "foo");  
    foo_file->data = &foo_data;  
    foo_file->read_proc = proc_read_foobar;  
    foo_file->write_proc = proc_write_foobar;  
    /* everything OK */
    printk(KERN_INFO "%s %s initialised\n",  MODULE_NAME, MODULE_VERS);  
    return 0;  

no_foo:  
    remove_proc_entry("jiffies", example_dir);  

out:  
return rv;  
}  

static void __exit cleanup_procfs_example(void)
{  
    remove_proc_entry("foo", example_dir);    
    remove_proc_entry(MODULE_NAME, NULL);  
    printk(KERN_INFO "%s %s removed\n",  
           MODULE_NAME, MODULE_VERS);  
}  

module_init(init_procfs_example);  
module_exit(cleanup_procfs_example);  
MODULE_AUTHOR("Erik Mouw");  
MODULE_DESCRIPTION("procfs examples");  
MODULE_LICENSE("GPL");  

