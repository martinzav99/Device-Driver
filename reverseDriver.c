#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define BUF_LEN 80
#define DEVICE_NAME "device_reverso"


static char msg[BUF_LEN];
static char *msg_Ptr;
static char *msgend_Ptr;

static int Major;
static int Device_Open = 0;

// Limpia el buffer, evitando escribir basura
int clean_buff(void){
	int i;
	for (i=0;i<BUF_LEN;i++)
	{
	  msg[i]=0 ;
	}
	return 0;
}

// Ordena la cadena, cambiando los valores con punteros
int sort(void){
	char aux;
	
	while (msg_Ptr < msgend_Ptr)
	{
	  aux = *msg_Ptr;
	  *msg_Ptr = *msgend_Ptr;
	  *msgend_Ptr = aux;
	  msg_Ptr++;
	  msgend_Ptr--;
	}
 return 0;
}


static int device_open(struct inode *inode, struct file *file)
{
  Device_Open++;
  
  msg_Ptr = msg; // apunto al incio del mensaje
  
  printk(KERN_ALERT "se abrió el dirver.\n");
  
 return 0; 
}
static int device_release(struct inode *inode, struct file *file)
{
 Device_Open--;

 printk(KERN_ALERT "se cerro el driver.\n");
 return 0;
}
static ssize_t device_read(struct file *filp, char *buffer, size_t length,loff_t * offset)
{
 int bytes_read = 0;

if (*msg_Ptr == 0)                
     return 0;

sort();
msg_Ptr = msg;

while (length && *msg_Ptr) // mientras no se llene el buffer o no sea fin del mensaje
{
 put_user(*(msg_Ptr++),buffer++);	
 length--;
 bytes_read++;
}

	
printk(KERN_ALERT "se leyó.\n");
return bytes_read;
}
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
 int i;
 clean_buff();
 
 for (i = 0; i < len && i < BUF_LEN; i++) // mientras no se llene el buffer del mensaje o el del device
 {
  get_user(msg[i], buff + i);		
 }
		
 msgend_Ptr = &msg[i-2]; // apunto a ultima letra, ya que las ultimas 2 posiciones son salto de linea y fin de cadena
 printk(KERN_ALERT "se escribio.\n");
 return i; //bytes escritos
}

struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
};

int init_module(void)
{ 
  Major = register_chrdev(0,DEVICE_NAME,&fops);
  
  printk(KERN_INFO "UNGS : Driver registrado\n");
  printk(KERN_INFO "I was assigned major number %d.To talk to the driver create a dev file with", Major);
  printk(KERN_INFO "mknod -m 666 /dev/%s c %d 0\n", DEVICE_NAME, Major);
  printk(KERN_INFO "Try to cat and echo to the device file.\n");
  printk(KERN_INFO "Remove the device file and module when done.\n");

  return 0;
}

void cleanup_module(void)
{ 
  unregister_chrdev(Major,DEVICE_NAME);
  printk(KERN_INFO "UNGS : Driver desregistrado\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("UNGS");
MODULE_DESCRIPTION("Un primer driver");
