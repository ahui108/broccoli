#!/bin/bash
#set -x
/bin/rm -f /home/linux/test.txt
echo "starting..." >>/home/linux/test.txt
module="SG_DMA"
device="SG_DMA"
#/sbin/insmod /opt/sg_dma_driver/$module.ko $* || exit 1
echo "rm dev mode" >>/home/linux/test.txt
/bin/rm -f /dev/${device}
major=`cat /proc/devices | awk "\\$2==\"$module\" {print \\$1}"`
echo "make dev mode" >>/home/linux/test.txt
/bin/mknod -m  a+rw /dev/${device}0 c $major 0
echo $? >>/home/linux/test.txt
echo "mknod succeed!" >>/home/linux/test.txt
