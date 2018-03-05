#!/bin/bash

sudo ifconfig can0 up
echo "i 0x0014 e" > /dev/pcan32
