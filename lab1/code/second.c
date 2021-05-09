#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <asm/param.h>
#define BUFFER_SIZE 128

#define PROC_NAME "seconds"

static ssize_t seq_read(struct file *file, char *buf, size_t count, loff_t *pos);

static struct proc_ops my_fops={
    .proc_read = seq_read,
};

unsigned long init_jif;

static int proc_init(void)
{
        init_jif = jiffies;
        proc_create(PROC_NAME, 0, NULL, &my_fops);

        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

static void proc_exit(void) {

        
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

static ssize_t seq_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
        char message[25];
        unsigned long current_jif = jiffies;
        unsigned long second_pass;

        second_pass = (current_jif - init_jif)/HZ;

        if (completed) {
                completed = 0;
                return 0;
        }
        
        completed = 1; 

        sprintf(message, "second pass: %lu\n", second_pass);
        rv = sprintf(buffer, message);//rv is the number of string

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}


module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Jiffies Module");
MODULE_AUTHOR("Yanjie Ze");