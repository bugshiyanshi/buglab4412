# buglab4412
课程：u-boot移植新手实践入门

第一节：移植环境搭建
开发板：itop4412 
CPU：cortex A9系列的exynos 4412

1.1 u-boot来源：Samsung官方

1.2 删除无关的目录和文件：
顶层目下：
删除 lib_arm、lib_generic 外其他 'lib_*' 目录

include/：
删除 asm-arm 、asm-generic 外其他 'asm-*' 目录

include/asm-arm:
删除 arch-s5pc210 外其他'arch-*' 目录

include/configs/:
删除 'tc4_*' 外的其他文件和目录

cpu/:
删除 'arm_cortexa9' 外的其他目录

board/:
删除 'samsung' 外的其他目录

board/samsung/:
删除 'smdkc210' 外的其他目录
脚本文件：rm_nuse_file.sh


*************************************初识u-boot移植实践*************************************
1、创建目标板编译配置
a、Makefile 添加编译规则
itop4412_linux_config:  unconfig
    @$(MKCONFIG) $(@:_config=) arm arm_cortexa9 smdkc210 samsung s5pc210

b、在include/configs/添加板级配置头文件：
   demo板是tc4，所以以tc4板级的配置头文件为基础：
   cp tc4_android.h itop4412_linux.h

2、修改过编译脚本build_uboot.sh:
修改加密文件路径；
生成目标镜像加密文件： cat E4412_N.bl1.SCP2G.bin bl2.bin all00_padding.bin u-boot.bin tzsw_SMDK4412_SCP_2GB.bin > u-boot-itop-4412.bin

3、修改交叉编译工具所在的路劲：
CROSS_COMPILE = arm-none-linux-gnueabi-

4、修改烧录到SD卡脚本mkuboot

5、修改打印信息显示的板子名称
U-Boot 2010.03 (Sep 22 2018 - 11:12:28) for TC4 Android 
将 ‘for TC4 Android’ 改成 ‘for itop linux’

Board:  TC4- 修改为 Board:  itop4412


include/config/itop4412_linux.h
-#define CONFIG_DEVICE_STRING    "TC4-"
+#define CONFIG_DEVICE_STRING    "itop4412"

-#define CONFIG_IDENT_STRING   " for TC4 Android"
+#define CONFIG_IDENT_STRING " for itop linux"


6、适配电源管理芯片
PMIC:   Pls check the i2c @ pmic, id = 21,error 添加id = 21 的适配，板子上使用的电源管理芯片是S5M8767
在cpu/arm_cortexa9/s5pc210/pmic.c --> PMIC_InitIp()函数中，芯片为S5M8767中添加id = 21

7、修改过u-boot命令提示符
命令提示符：TC4 #  修改为：itop4412 # 

-#define CONFIG_SYS_PROMPT              "TC4 # "   
+#define CONFIG_SYS_PROMPT              "itop4412 # "  

*************************************初识u-boot移植实践*************************************


