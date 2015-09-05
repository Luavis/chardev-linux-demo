#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>


int init_module(void);
void cleanup_module(void);

static int device_open(
  struct inode *,
  struct file *
);

static int device_release(
  struct inode *,
  struct file *
);

static ssize_t device_read(
  struct file *,
  char *,
  size_t, loff_t *
);

static ssize_t device_write(
  struct file *,
  const char *,
  size_t, loff_t *
);

int sprintf(char * buf, const char * fmt, ...);

#define OK 0
#define DEV_NAME "chardev"
#define BUF_LEN 80

#define TEMP_DEV_NUM 0

static int device_number = 0;
static bool is_open = false;
static char buf[BUF_LEN];

static char * device_msg;

static struct file_operations f_ops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};


/*
 * Impl device driver
 *
 */

int init_module(void) {

    device_number = register_chrdev(
        TEMP_DEV_NUM, 
        DEV_NAME,
        &f_ops
    );

    printk(KERN_INFO "Init chardev\n");

    return OK;
}

void cleanup_module(void) {
  unregister_chrdev(
    device_number,
    DEV_NAME
  );

  printk(KERN_INFO "Close chardev\n");
}

/*
 * char dev open callback
 */

static int device_open(
    struct inode * inode,
    struct file * flip
  ) {
  static int counter = 0;

  printk(KERN_INFO "Open device\n");

  if(is_open)
    return -EBUSY;

  is_open = true;

  sprintf(buf, "I already told you %d times, Hello World", counter++);
  device_msg = buf;

  // lock module

  try_module_get(THIS_MODULE);

  return OK;
}

static int device_release(
    struct inode * inode,
    struct file * file
  ) {
  
  is_open = false;

  module_put(THIS_MODULE);
  
  return OK;
}

static ssize_t device_read(
    struct file * filp,
    char * buffer,
    size_t length,
    loff_t * offset
  ) {
  
  int read_len = 0;

  if(*device_msg == NULL) {
    return OK;
  }

  while(length && *device_msg != NULL) {
    put_user(*(device_msg++), buffer++);
    length = 0;
    read_len++;
  }

  return read_len;
}

static ssize_t device_write(
    struct file * flip,
    const char * buffer,
    size_t length,
    loff_t * offset
  ) {
  printk(KERN_ALERT "Sorry this chardev is readonly\n");
  return -EINVAL;
}


int sprintf(char * buf, const char * fmt, ...) {
  va_list args;
  int i;

  va_start(args, fmt);
  i = vsprintf(buf, fmt, args);

  va_end(args);

  return i;
}

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Luavis Kang");
MODULE_DESCRIPTION(
  "This module for studying "
  "character device driver(or module) "
  "meaning serail port device generally."
);
