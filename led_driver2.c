#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>       

#define AUTOR           "Dennis/ALberto"
#define DESCRIPCION     "Driver para encender/apagar un led"
#define DEVICE_NAME     "leddriver"
#define MAX             1024

/*variables*/
static int num_mayor;
static unsigned long procfs_buffer_size = 0;
static char buffer_data[MAX];
static char msg[MAX];      
static char *msg_Ptr;

/*Metodos*/
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);


static struct file_operations fops= {
        .open = device_open,
        .release = device_release,
        .read = device_read,
        .write = device_write,
};


static int __init hola_inicio(void)
{

        printk(KERN_INFO "Yeiiiiiiii funciona \n");

        num_mayor=register_chrdev(0, DEVICE_NAME, &fops);
        printk(KERN_INFO "El archivo se genero\
                mknod /dev/%s c %d 0 \n",DEVICE_NAME,num_mayor);

        return 0 ;

}

static void __exit hola_fin(void)
{
        unregister_chrdev(num_mayor, DEVICE_NAME);
}

module_init(hola_inicio);
module_exit(hola_fin);

static int device_open(struct inode *inode, struct file *file)
{
        sprintf(msg,buffer_data);
        msg_Ptr = msg;
        return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
        return 0;
}

static ssize_t device_read(struct file *filp, char *buf, size_t len, loff_t * off)
{
        int bytes_read = 0;
        if (*msg_Ptr == 0)
                return 0;

        while (len && *msg_Ptr) {
                put_user(*(msg_Ptr++), buf++);
                len--;
                bytes_read++;
        }

        return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buf, size_t len, loff_t * off)
{

        procfs_buffer_size = len;

        if(buffer_data[0] == '1'){

	        if ( copy_from_user(buffer_data, buf, procfs_buffer_size) ) {
	            return -EFAULT;
	        }
	        pr_info("LED ON %s\n",buffer_data);

	    }else if(buffer_data[0] == '0'){
	        if ( copy_from_user(buffer_data, buf, procfs_buffer_size) ) {
	            return -EFAULT;
	        }

	        pr_info("LED Off %s\n",buffer_data);
	    }else if(buffer_data[0] != '0' || buffer_data[0] != '1'){
	        if ( copy_from_user(buffer_data, buf, procfs_buffer_size) ) {
	            return -EFAULT;
	        }

	        pr_info("Intenta de nuevo %s\n",buffer_data);
	    }
		*off += len;
        pr_info("user input string: %s\n",buffer_data);
        pr_info("user input string len: %lu\n",procfs_buffer_size);

        return procfs_buffer_size;
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR(AUTOR);                      
MODULE_DESCRIPTION(DESCRIPCION);       

MODULE_SUPPORTED_DEVICE("leddriver");
