#!/bin/bash

echo "删除 'lib_*' 非arm generic 目录"
ls | grep 'lib_' | egrep -v 'arm|generic' | xargs rm  -rf

echo "删除 include/asm-* 非arm generic 目录"
cd include
ls | grep 'asm-' | egrep -v 'arm|generic' | xargs rm  -rf

echo "删除 include/asm-arm/arch-* 非s5pc210 目录"
cd asm-arm
ls | grep 'arch-' | grep -v 's5pc210' | xargs rm  -rf

echo "删除 include/configs/ 下非tc4* 的文件和目录"
cd ../configs
ls | grep -v 'tc4' | xargs rm  -rf

echo "删除 cpu/ 下非 arm_cortexa9 目录"
cd ../../cpu
ls | grep -v 'arm_cortexa9' | xargs rm -rf

echo "删除 board/ 非三星的目录"
cd ../board
ls | grep -v 'samsung' | xargs rm  -rf

echo "删除 /board/samsung/ 非smdkc210 目录"
cd samsung
ls | grep -v 'smdkc210' | xargs rm  -rf

cd ../../

echo "rm file finish"
