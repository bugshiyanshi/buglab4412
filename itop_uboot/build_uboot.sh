#!/bin/sh
#./build_uboot.sh ---> build the uboot images for tc4
#./build_uboot.sh tc4_plus -->build the uboot images for tc4_plus
#./build_uboot.sh clean ----> clean the images
#./build_uboot.sh windows ----> encrypt the uboot image in window pc



sec_path="../CodeSign4SecureBoot_SCP/"
CPU_JOB_NUM=$(grep processor /proc/cpuinfo | awk '{field=$NF};END{print field+1}')
ROOT_DIR=$(pwd)
CUR_DIR=${ROOT_DIR##*/}


case "$1" in
	clean)
		echo make clean
		make mrproper
		;;
		
	*)
			
		if [ ! -d $sec_path ]
		then
			echo "**********************************************"
			echo "[ERR]please get the CodeSign4SecureBoot first"
			echo "**********************************************"
			return
		fi
		
		if [ -z $1 ]
		then
			make itop4412_linux_config
		else
			echo please input right parameter.
			exit 0
		fi
		
		make -j$CPU_JOB_NUM
		
		if [ ! -f checksum_bl2_14k.bin ]
		then
			echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
			echo "There are some error(s) while building uboot, please use command make to check."
			echo "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
			exit 0
		fi
		
		cp -rf checksum_bl2_14k.bin $sec_path
		cp -rf u-boot.bin $sec_path
		rm checksum_bl2_14k.bin
		
		cd $sec_path
		# gernerate the uboot bin file support trust zone
		cat E4412_N.bl1.SCP2G.bin bl2.bin all00_padding.bin u-boot.bin tzsw_SMDK4412_SCP_2GB.bin > u-boot-itop-4412.bin	
		
		mv u-boot-itop-4412.bin $ROOT_DIR
	
		rm checksum_bl2_14k.bin
		rm u-boot.bin

		echo 
		echo 
		;;
		
esac
