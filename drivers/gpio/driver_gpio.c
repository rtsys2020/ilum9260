/* Module for controling GPIO pins in userspace */
/* Please leave the author of the module and add yourself after a  comma if you edit the code */
/* Last edit: 2008-06-11 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <asm/gpio.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("Tomas 'Socrates' D."); 
MODULE_LICENSE("GPL"); 

/* Globals */
unsigned int pin = AT91_PIN_PB3; /*Pin selection*/
            
/* Begin */   
static ssize_t user_gpio_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
       int length = 3; /* Length of the symbol */
       int level = at91_get_gpio_value(pin); /* Getting pin value */
       char *string;
       sprintf(string, "%d\n",level); /*Integer to string*/
       copy_to_user(buf,string,length); /* Taking data from kernel and parsing to user */
       if (*ppos == 0) { 
       *ppos+=1; 
        return length; /* We don't want overflow and kernel panic */
  } else { 
    return 0; 
  }
};

static ssize_t user_gpio_write(struct file * file,char *buf, size_t count, loff_t *loft) {
  char string[1024]; /* Taking enough space in the memory */
  int level; /* Assign a value of the pin */     
   copy_from_user(string,buf,sizeof(string)); /* Taking data from user and parsing to kernel */
   sscanf(string,"%d",&level); /* String to integer */
   at91_set_gpio_value(pin, level); /*Setting pin value */
   
  return sizeof(string); /* We don't want overflow and kernel panic */
  
};

static const struct file_operations user_gpio_file_fops = { /* Operations for controling the device */
        .owner             = THIS_MODULE,
        .read              = user_gpio_read,
        .write             = user_gpio_write,
};

static struct miscdevice gpio = { /*Creating a misc device called gpio */
        MISC_DYNAMIC_MINOR,
        "gpio",                   /* Name sets here,  will be shown in /dev */
        &user_gpio_file_fops
};

static int modinit(void)  /* Module init */
{
   gpio_direction_output(pin,1); /*Setting chosen pin as output */
int ret;
   printk(KERN_ALERT "Loading Socrates GPIO\n");
     ret = misc_register(&gpio); /* Registering the device */
   return ret;
};

static void modexit(void) /* Module exit */
{
   printk(KERN_ALERT "Unloading Socrates GPIO\n");
   misc_deregister(&gpio); /* Unregistering the device */
}

module_init(modinit);
module_exit(modexit); 
