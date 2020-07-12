#!/bin/bash

PATH=$PATH:/Users/jacomostert/Documents/WP43S/DM42/gcc-arm-none-eabi-8-2019-q3-update/bin

./cleanjm
//make -j8 mrproper
make -j8 V=1
make -j8 testSuite

cd DMCP_build
./compilDMCP
