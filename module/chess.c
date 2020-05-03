/*
 *  Hugues Nelson Iradukunda	hiraduk1@umbc.edu 	github username:IradukundaHN
 *
 *  chess.c: implements a virtual character device that interacts with the file in a userspace to play chess
*/

/* Header and libraries imports*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/uaccess.h> 	/*for copy_to_user and copy_from_user*/
#include <linux/string.h>

#define MODULE_NAME "chess"			/* Dev name as it appears in /proc/devices*/
#define DRIVER_AUTHOR "Hugues Nelson Iradukunda"
#define DRIVER_DESCRIPTION "Chess Character device driver"


/*
 * Global variables are declared as static as they are within the file. 
 */
static int major;			/* Major number assigned to our device driver */
static struct class* dev_class=NULL; 	/* class for our device driver */
static struct device* my_device;
//static struct cdev char_dev;

static int  is_open;  		/* Is device open?  
				 * Used to prevent multiple access to device */

#define BUF_LEN 256             /* Max length of the message from the device */
static char cmd[BUF_LEN];       /* The cmd data the device will give when asked */

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/*
 * This function is called when the module is loaded
 */
int __init my_start(void)
{
	printk(KERN_INFO "Module loaded\n");

	//assigning a major number to register a device in /proc/devices
        major = register_chrdev(0, MODULE_NAME, &fops);
	if (major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", major);
	  return -1;
	}

	 //class for register a device in ls /sys/class
	dev_class = class_create( THIS_MODULE, MODULE_NAME);
	if( IS_ERR(dev_class) )
    	{
        	printk( KERN_ERR "class_create failed, error %ld\n", PTR_ERR(dev_class) );
        	unregister_chrdev( major, MODULE_NAME );
        	return PTR_ERR(dev_class);
    	} 

	//Then, create a dev
    	my_device = device_create( dev_class, NULL, MKDEV(major, 0), NULL, MODULE_NAME);
	if (IS_ERR(my_device))
    	{
        	printk( KERN_ERR "device_create failed, error %ld\n", PTR_ERR(my_device) );
        	//reversely
		class_destroy( dev_class );
        	unregister_chrdev( major, MODULE_NAME );
        	return PTR_ERR(my_device);
    	}
	return 0;

}

/*
 * This function is called when the module is unloaded
 */
void __exit my_end(void)
{
	printk(KERN_INFO "Module unloaded\n" );
	device_destroy(dev_class, MKDEV(major,0));
    	class_destroy( dev_class );
    	unregister_chrdev( major, MODULE_NAME );
}

/*
 * I/O Operations Methods
 */

/*
 * Called when a process tries to open the device file
 */
static int device_open(struct inode *inode, struct file *file)
{
	if (is_open == 1){
		printk(KERN_INFO "Error, device already open\n");
		return -EBUSY;
	}
	is_open =1;

	//prevent module from being unloaded
	try_module_get(THIS_MODULE);

	return 0;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	if (is_open == 0){
                printk(KERN_INFO "Error, device did not open\n");
                return -EBUSY;
        }
	is_open = 0;

	//prevent module from being unloaded
        module_put(THIS_MODULE);

	return 0;
}

/*
 * Called when a process, which already opened the dev file, attempts to read from it.
 */
static ssize_t device_read(struct file *filp, char __user *outbuffer, size_t len, loff_t * offset)
{
	int retval=0;

        /*copy to user function*/
	retval = copy_to_user(outbuffer, cmd, len);

	/*check validity and parse commands for execution*/
	/*and return the size of response*/
	if(strcmp(cmd, "OK\n")==0)
	{
		printk("OK\n");
		return strlen(cmd);
	}
	if(strcmp(cmd, "CHECK\n")==0)
        {
                printk("CHECK\n");
                return strlen(cmd);
        }
	if(strcmp(cmd, "MATE\n") == 0)
        {
                printk("MATE\n");
                return strlen(cmd);
        }
	if (strcmp(cmd, "ILLMOVE\n") ==0)
        {
                printk("ILLMOVE\n");
                return strlen(cmd);
        }
	if (strcmp(cmd, "OOT\n") ==0)
        {
                printk("OOT\n");
                return strlen(cmd);
        }
	if (strcmp(cmd, "NOGAME\n") ==0)
        {
                printk("NOGAME\n");
                return strlen(cmd);
        }
	if (strcmp(cmd, "INVFMT\n") ==0)
        {
                printk("INVFMT\n");
                return strlen(cmd);
        }
	
	/*Error in read*/
	if (retval<0){
		printk(KERN_ALERT "Read isn't supported.\n");
                return -EINVAL;
	}
	return len;
}

/*
 * Called when a process writes to dev file
 */
static ssize_t device_write(struct file *filp, const char __user *inbuffer, size_t len, loff_t * off)
{
	int retval=0;

	if (len > sizeof(cmd)-1 ){
		len = sizeof(cmd)-1;
	}
	/*Copy from user function*/
	retval = copy_from_user(cmd, inbuffer, len);

	/*Error in Write*/
	if (retval < 0){
                printk(KERN_ALERT "Write isn't supported.\n");
                return -EINVAL;
        }

	return len;//return length given
}

/*
* Module Metadata
*/
module_init(my_start);
module_exit(my_end);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);

