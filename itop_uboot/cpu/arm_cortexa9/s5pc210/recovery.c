/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * EXYNOS4x12 - RECOVERY Driver for U-Boot
 * jun.ma@samsung.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
//#include <common.h>
#include <s5pc210.h>
//#include <asm/io.h>
//#include <mmc.h>
//#include <s3c_hsmmc.h>
#include <command.h>
#include "recovery.h"
#include <movi.h>
/*******************************************************************/
/* recovery - recovery the system */
/*******************************************************************/
#if 1
#define RECOVERY_IMAGE_MAGIC 0x52444E41
int recovery_enter = 0;
int do_recovery (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong addr;
	int magic_num;
	int recovery_start_addr,recovery_size;
	int kernel_start_addr,kernel_size;
	int addr_mode=0; //0 use default addr.  1: use the addr in image header..
	char run_cmd[100];



	printf("\n** go to extract the image file.. **\n");
	addr = simple_strtoul(argv[1], NULL, 16);
	if (*(ulong *)(addr) == RECOVERY_IMAGE_MAGIC)
	{
		//boot_img_hdr *hdr;
		int size;

		size = sizeof(boot_img_hdr);
		printf("\n** This is a Valid image file.[%x]. **\n",size);
		if(0)//(!recovery_enter)
		{
				
		}
		else
		{
			kernel_start_addr = addr+0x800;;
			kernel_size = *(ulong *)(addr+0x8);   // M
			printf("kernel_start_addr: 0x%8x\n",kernel_start_addr);
			printf("kernel_size      : 0x%8x\n",kernel_size);
			if(kernel_size>PART_SIZE_KERNEL)
			{
				printf("[ERROR]The kenerl size is larger than limit[0x%x]\n",PART_SIZE_KERNEL);		
			}
			/* copy the kernel img */
			memcpy (0x40008000, kernel_start_addr, kernel_size); 
			
			recovery_start_addr = kernel_start_addr+kernel_size+0x180;//0x40d00000;
			recovery_size =  *(ulong *)(addr+0x10);
			
			printf("recovery_start_addr: 0x%8x\n",recovery_start_addr);
			printf("recovery_size      : 0x%8x\n",recovery_size);

			/* copy the recovery img */
			memcpy (0x40d00000, recovery_start_addr, recovery_size); 

			sprintf(run_cmd, "bootm 40008000 40d00000");
			setenv("bootcmd", run_cmd);
			return 0;
			
		}
	
	}
	else
	{
		printf("\n** Invalid  image file.. **\n");

	}
	return 0;
}

U_BOOT_CMD(
	recovery,	2,		2,	do_recovery,
	"recovey the system ...",
	"\n"
	"   \n"
	" "
);
#endif

int factory_reset(int mode)
{
	block_dev_desc_t *dev_desc=NULL;
	dev_desc = get_dev("mmc", 0);
	if (dev_desc==NULL) {
		printf("\n** Invalid boot device **\n");
		return -1;
	}
	/*for t34h, 1:fat, 2,3,4:ext4*/
	if (ext2fs_format(dev_desc, 3) != 0)
	{
		printf("ext2format failed for data\n");
		return -1;
	}
	
	if(ext2fs_format(dev_desc, 4))
	{
		printf("ext2format failed for cache\n");
		return -1;
	}

	if(!mode)
	{
		//printf("FAT format failed for cache\n");
		//format SD card...
		if (fat_format_device(dev_desc, 1) != 0) 
		{	
			printf("FAT Format EMMC  Failure!!!\n");
			return -1;
		}
		
		//mj : there is need some emergency..for example no sd card....
		dev_desc = get_dev("mmc", 1); //find the sd card..
		if (dev_desc==NULL) 
		{
			printf("\n** Invalid boot device **\n");
			return -1;
		}
		if (fat_format_device(dev_desc, 1) != 0) 
		{	
			printf("FAT Format SD  Failure!!!\n");
			return -1;
		}

	}
	return 0;
}
extern signed int key1_pulldown, key2_pulldown,power_mode;
extern void LCD_turnon(void);

int recovery_preboot(void)
{
	unsigned int reset_mode;
	int value  =0 ;
	reset_mode = INF_REG5_REG;
	char run_cmd[100];
		
	printf("reset_mode: 0x%x\n", reset_mode);
	if(reset_mode ==FACTORY_RESET_MODE)
	{
			printf("SYSTEM ENTER FACTORY RESET MODE[0x%x]\n",reset_mode);	
			INF_REG5_REG = reset_mode&(~0xff);
			if(factory_reset(1))
			{
				printf("[ERROR]: Factory Reset Fail..");
				return -1;
			}
			return 0;
	}
	else if (reset_mode ==CHARGING_RESET_MODE)
	{
			INF_REG5_REG = reset_mode&(~0xff);
			printf("SYSTEM IN CHARGEING MODE-->GO SLEEP\n");	
			#ifdef CONFIG_LOGO_DISPLAY
			exynos_display_pic(BATTERY_LOGO);
			#endif
			value = __REG(S5P_PS_HOLD_CONTROL);
			value = 0x5200;// shutdown
			__REG(S5P_PS_HOLD_CONTROL)= value;
	}
	#if  0
	else if (recovery_mode ==RECOVERY_MODE)
	{
			printf("SYSTEM ENTER RECOVERY MODE[0x%x]\n",recovery_mode);	
			//char boot_cmd[100];
			//sprintf(boot_cmd, "movi read kernel 40008000;movi read rootfs 40d00000 100000;bootm 40008000 40d00000");
			//setenv("bootcmd", boot_cmd);
			setenv("bootcmd","movi read kernel 40008000;movi read recovery 40d00000 300000;bootm 40008000 40d00000");
			return 0;

	}
	#endif
	else
	{		
			
			value = __REG(GPX2DAT);
			key1_pulldown =((value & 0x1) == 0x1);
			key2_pulldown =((value & 0x2) == 0x2);
			printf("KEY_DET!%d, %d, %x\n",key1_pulldown,key2_pulldown,value);
			
			if(key1_pulldown) //mj for temp...
			{	
				printf("SYSTEM ENTER RECOVERY MODE1[0x%x]\n",reset_mode);	
				if(1)
				{
					#ifdef CONFIG_LOGO_DISPLAY
					exynos_display_pic(RECOVERY_LOGO);
					#endif
					
					setenv("bootcmd","movi read kernel 40008000;movi read Recovery 40d00000 300000;bootm 40008000 40d00000");
				}
				else
				{
					sprintf(run_cmd, "movi read Recovery 41000000 600000;recovery 41000000");
					//sprintf(run_cmd, "movi read Recovery 41000000 600000");
					run_command(run_cmd,0);
				}

			}
			else if (key2_pulldown)
			{
				printf("SYSTEM ENTER Updating MODE1[0x%x]\n",reset_mode);	
				sprintf(run_cmd, "sdfuse flashall");
				run_command(run_cmd, 0);			
			}
			else
			{
				if(power_mode == 0)
				{
					printf("SYSTEM ENTER NORMAL BOOT MODE[BATTERY]\n");	
					#ifdef CONFIG_LOGO_DISPLAY
					exynos_display_pic(BOOT_LOGO);
					#endif
				}
				else if(power_mode == 2) //SHORT PUSH .GO TO CHARGING
				{
						printf("SYSTEM IN CHARGEING MODE-->GO CHARGING IN POWER OFF MODE\n");	

						#ifdef CONFIG_LOGO_DISPLAY
						exynos_display_pic(BATTERY_LOGO);
						value = __REG(S5P_PS_HOLD_CONTROL);
						value = 0x5200;// shutdown
						__REG(S5P_PS_HOLD_CONTROL)= value;
						#endif

				}
				else if(power_mode == 3)
				{	
					printf("SYSTEM ENTER NORMAL BOOT MODE[CHARGING]\n");	
					#ifdef CONFIG_LOGO_DISPLAY
					exynos_display_pic(BOOT_LOGO);
					#endif
				}
				else if (power_mode == 1)
				{
						#ifdef CONFIG_LOGO_DISPLAY
						printf("[ERROR]SYSTEM IN UNKNOWN POWER MODE\n");
						#else
						printf("SYSTEM ENTER NORMAL BOOT MODE1[CHARGING]\n");	
						#endif
						//value = __REG(S5P_PS_HOLD_CONTROL);
						//value = 0x5200;// shutdown
						//__REG(S5P_PS_HOLD_CONTROL)= value;
				}
				else
				{
						printf("[ERROR]SYSTEM IN UNKNOWN POWER MODE1\n");
				}
			}
			
			return 0;
	}


}
