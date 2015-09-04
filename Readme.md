Character device in linux
===

This project for studying character device driver(or module) meaning serail
port device generally.

Installation
===

```sh
$ make
$ insmod chardev-linux-demo
```

Then, you can check major device number in ```/proc/devices/``` which device
name is ```chardev```. If you check it then run ```mknod``` command to create device.

```sh
$ mknod /dev/hello c <MAJOR_NUMBER> <MINOR_NUMBER>
$ cat /dev/hello
I already told you 0 times, Hello World
```

