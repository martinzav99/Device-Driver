#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>


#define longitud_alfabeto 26
#define inicio_alfabeto 97
#define BUF_LEN 80

const char *alfabeto = "abcdefghijklmnopqrstuvwxyz";
static char Message[BUF_LEN];
static char *Message_Ptr;

char cesar(char letra,  int n) {
		
	if (letra == ' '){
		return letra;
	}
	else {
		letra = alfabeto[((int)letra - inicio_alfabeto + n) % longitud_alfabeto];
	}
	
	return letra;
}



static int device_open(struct inode *inodep, struct file *filep)
{
	Message_Ptr = Message;	
	printk(KERN_ALERT "se abrió.\n");
	return 0;
}

static int device_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_ALERT "se acabó.\n");
	return 0;
}


static ssize_t device_read(struct file *filp,char *buffer,size_t length,loff_t * offset)
{         
        
        int bytes_read = 0;
        if (*Message_Ptr == 0)                
           return 0;
        
        while (length && *Message_Ptr) {
        	put_user(*(Message_Ptr++),buffer++);                
        	length--;                
        	bytes_read++;
        }
        
        printk(KERN_ALERT "se leyó.\n");
	        
        return bytes_read;
}


static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{        	
	
	int i;
	
	for (i = 0; i < len && i < BUF_LEN; i++){
		get_user(Message[i], buff + i);
		
		Message[i] = cesar(Message[i] , 3);
	}
	
	
	Message_Ptr = Message;
	
	printk(KERN_ALERT "se escribió.\n");
	return i;	
}




struct file_operations fops = {
	.open    = device_open,
	.read    = device_read,
	.write   = device_write,
	.release = device_release
};

int init_module(void)
{ 
   /* Constructor */

    register_chrdev(999,"char_driver_encriptador",&fops);
   
   printk(KERN_INFO "UNGS : Driver registrado\n");
   return 0;
}

void cleanup_module(void)
{ 
  /* Destructor */
   unregister_chrdev(999,"char_driver_encriptador");  
  printk(KERN_INFO "UNGS : Driver desregistrado\n");
}



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Martin Zavalla");
MODULE_DESCRIPTION("Mi primer driver encriptador");