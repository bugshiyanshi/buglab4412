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


1.3 build_uboot.sh修改：

1.4 Makefile 修改：
编译器：CROSS_COMPILE = arm-none-linux-gnueabi-
生成镜像：删除生成镜像信息--354行


1.5 建立itop4412 目标板配置：
Makefile 中添加：
itop_4412_linux_config: unconfig
    @$(MKCONFIG) $(@:_config=) arm arm_cortexa9 smdkc210 samsung s5pc210
    
include/configs/目录下创建板级配置文件：
cp tc4_android.h  itop_4412_linux.h

1.6 mkuboot 修改：修改烧录到sd卡的脚本


