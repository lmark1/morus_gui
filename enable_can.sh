#!/bin/bash

sudo modprobe pcan
sudo ifconfig can0 up
echo "i 0x0014 e" > /dev/pcan32
cat /proc/pcan
