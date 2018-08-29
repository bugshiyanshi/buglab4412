/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * S5PC110 - LCD Driver for U-Boot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <s5pc210.h>
#include <movi.h>
#define LCD_BGCOLOR		0x1428A0

static unsigned int gFgColor = 0xFF;
static unsigned int gLeftColor = LCD_BGCOLOR;

#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Delay(_a)		udelay(_a*1000)


#if 0//defined(CFG_LCD_TL2796)
/*
 * TL2796
 */

#define LCD_WIDTH		480
#define LCD_HEIGHT		800

#define IIC_SDA_GPIO_ADDRESS		(0xE02000C0)	//GPB
#define IIC_SDA_GPIO_BIT			(0)		//GPB[7]
#define IIC_SCL_GPIO_ADDRESS		(0xE02000C0)	//GPB
#define IIC_SCL_GPIO_BIT			(1)		//GPB[4]
#define SDA_GPIO_ADDRESS			(0xE0200040)	//GPB
#define SDA_GPIO_BIT				(7)		//GPB[7]
#define SCL_GPIO_ADDRESS			(0xE0200040)	//GPB
#define SCL_GPIO_BIT				(4)		//GPB[4]
#define CS_GPIO_ADDRESS				(0xE0200040)	//GPB
#define CS_GPIO_BIT					(5)		//GPB[5]
#define BACKLIGHT_GPIO_ADDRESS		(0xE02000A0)	//GPD0
#define BACKLIGHT_GPIO_BIT			(3)		//GPD0[3]
#define RESET_GPIO_ADDRESS			(0xE0200C00)	//GPH0
#define RESET_GPIO_BIT				(6)		//GPH0.6
#define POWER_GPIO_ADDRESS			(0xE0200040)
#define POWER_GPIO_BIT				(6)
#define POWER_GPIO_ADDRESS2			(0xE0200040)
#define POWER_GPIO_BIT2				(7)
#define BACKLIGHT_ACTIVE 			HIGH_ACTIVE
#define RESET_ACTIVE 				LOW_ACTIVE
#define POWER_ACTIVE 				HIGH_ACTIVE

//do not modify
#define LCDM_SDA_HIGH			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)))
#define LCDM_SDA_LOW			Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) &= ~(1<<SDA_GPIO_BIT)))
#define LCDM_SCL_HIGH			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)))
#define LCDM_SCL_LOW			Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) &= ~(1<<SCL_GPIO_BIT)))
#define LCDM_CS_HIGH			Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) |=(1<<CS_GPIO_BIT)))
#define LCDM_CS_LOW				Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) &= ~(1<<CS_GPIO_BIT)))
#define LCDM_BACKLIGHT_HIGH 	Outp32(BACKLIGHT_GPIO_ADDRESS+4,(Inp32(BACKLIGHT_GPIO_ADDRESS+4) |= (1<<BACKLIGHT_GPIO_BIT)))
#define LCDM_BACKLIGHT_LOW	Outp32(BACKLIGHT_GPIO_ADDRESS+4,(Inp32(BACKLIGHT_GPIO_ADDRESS+4) &= ~(1<<BACKLIGHT_GPIO_BIT)))
#define LCDM_RESET_HIGH		 	Outp32(RESET_GPIO_ADDRESS+4,(Inp32(RESET_GPIO_ADDRESS+4) |= (1<<RESET_GPIO_BIT)))
#define LCDM_RESET_LOW			Outp32(RESET_GPIO_ADDRESS+4,(Inp32(RESET_GPIO_ADDRESS+4) &= ~(1<<RESET_GPIO_BIT)))
#define LCDM_POWER_HIGH		 	Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) |= (1<<POWER_GPIO_BIT)))
#define LCDM_POWER_LOW			Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) &= ~(1<<POWER_GPIO_BIT)))
#define LCDM_POWER2_HIGH	 	Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) |= (1<<POWER_GPIO_BIT)))
#define LCDM_POWER2_LOW	 		Outp32(POWER_GPIO_ADDRESS+4,(Inp32(POWER_GPIO_ADDRESS+4) &= ~(1<<POWER_GPIO_BIT)))
#define LCDM_IIC_SCL_HIGH		Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)))
#define LCDM_IIC_SCL_LOW		Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) &= ~(1<<SCL_GPIO_BIT)))
#define LCDM_IIC_SDA_HIGH		Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)))
#define LCDM_IIC_SDA_LOW		Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) &= ~(1<<SDA_GPIO_BIT)))


void Write_LDI_TL2796(u16 uAddr, u16 uData)
{
	s32 j;

	if(uAddr != 0x0000)
	{
		LCDM_CS_HIGH;
		LCDM_SCL_HIGH;
		LCDM_SDA_HIGH;

		LCDM_CS_LOW;
		LCDM_SCL_LOW;

	    // Command
	    for (j = 15 ; j >= 0; j--)
		{
			LCDM_SCL_LOW;

			if ((uAddr >> j) & 0x0001)
			{
				LCDM_SDA_HIGH;
			}
			else
			{
				LCDM_SDA_LOW;
			}
			LCDM_SCL_HIGH;
		}
	}

	if (uAddr == 0x0000)
	{
		LCDM_SDA_HIGH;
	}

	LCDM_CS_HIGH;
	LCDM_SCL_HIGH;
	LCDM_CS_LOW;
	LCDM_SCL_LOW;

	// parameter
	for (j = 15 ; j >= 0; j--)
	{
		LCDM_SCL_LOW;

		if ((uData >> j) & 0x0001)
		{
			LCDM_SDA_HIGH;
		}
		else
		{
			LCDM_SDA_LOW;
		}
		LCDM_SCL_HIGH;
	}
}

void LCDM_InitTL2796(void)
{
    Delay(50); // more than 20ms

	//[1] Panel Condition Set
	Write_LDI_TL2796(0x7031, 0x7208);
	Write_LDI_TL2796(0x7032, 0x7214);
	Write_LDI_TL2796(0x7030, 0x7202);

#if 1
	Write_LDI_TL2796(0x7027, 0x7203);
#else
	Write_LDI_TL2796(0x7027, 0x7201);
#endif

	//[2] Display Condition Set(RGB Data Interface)
	Write_LDI_TL2796(0x7012, 0x7208); //0x7208
	Write_LDI_TL2796(0x7013, 0x7208);
#if 1
	Write_LDI_TL2796(0x7015, 0x7210); //0x10
#else
	Write_LDI_TL2796(0x7015, 0x7200); //0x00
#endif
	Write_LDI_TL2796(0x7016, 0x7200);	//00 24bit 02 16bit RGB interface 
	Write_LDI_TL2796(0x70ef, 0x72d0);
	Write_LDI_TL2796(0x0000, 0x72e8);

	// set gamma control 230cd
	Write_LDI_TL2796(0x7039, 0x7244);
	Write_LDI_TL2796(0x7040, 0x7200);
	Write_LDI_TL2796(0x7041, 0x723F);
	Write_LDI_TL2796(0x7042, 0x722B);
	Write_LDI_TL2796(0x7043, 0x721f);
	Write_LDI_TL2796(0x7044, 0x7224);
	Write_LDI_TL2796(0x7045, 0x721b);
	Write_LDI_TL2796(0x7046, 0x7229);
	Write_LDI_TL2796(0x7050, 0x7200);
	Write_LDI_TL2796(0x7051, 0x7200);
	Write_LDI_TL2796(0x7052, 0x7200);
	Write_LDI_TL2796(0x7053, 0x721b);
	Write_LDI_TL2796(0x7054, 0x7222);
	Write_LDI_TL2796(0x7055, 0x721b);
	Write_LDI_TL2796(0x7056, 0x722a);
	Write_LDI_TL2796(0x7060, 0x7200);
	Write_LDI_TL2796(0x7061, 0x723F);
	Write_LDI_TL2796(0x7062, 0x7225);
	Write_LDI_TL2796(0x7063, 0x721c);
	Write_LDI_TL2796(0x7064, 0x7221);
	Write_LDI_TL2796(0x7065, 0x7218);
	Write_LDI_TL2796(0x7066, 0x723e);

	//Analog Power Condition Set
	Write_LDI_TL2796(0x7017, 0x7222);	//Boosting Freq
	Write_LDI_TL2796(0x7018, 0x7233);	//power AMP Medium
	Write_LDI_TL2796(0x7019, 0x7203);	//Gamma Amp Medium
	Write_LDI_TL2796(0x701a, 0x7201);	//Power Boosting
	Write_LDI_TL2796(0x7022, 0x72a4);	//Vinternal = 0.65*VCI
	Write_LDI_TL2796(0x7023, 0x7200);	//VLOUT1 Setting = 0.98*VCI
	Write_LDI_TL2796(0x7026, 0x72a0);	//Display Condition LTPS signal generation : Reference= DOTCLK

	//Stand-by Off Comman
	Write_LDI_TL2796(0x701d, 0x72a0);

	Delay(500); // more than 250ms

	//LCD ON
	Write_LDI_TL2796(0x7014, 0x7203);
}

void LCD_Initialize_TL2796(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;

	u32 i,uTmpReg;
	u32* pBuffer = (u32*)uFbAddr;


	LCD_setprogress(0);


	uTmpReg = Inp32(0xE0107008);		// need to be changed later (09.01.23 WJ.Kim)	
	uTmpReg = (uTmpReg & ~(0x3)) | 0x2 ;
	Outp32(0xE0107008, uTmpReg);

	uTmpReg = Inp32(0x11c0027c);
	uTmpReg = (uTmpReg & ~(0x3)) | 0 ;
	Outp32(0x11c0027c,uTmpReg);


	Outp32(0xE0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200128,0x0);			//set pull-up,down disable
	Outp32(0xE0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xE0200148,0x0);			//set pull-up,down disable
	Outp32(0xE0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xE0200168,0x0);			//set pull-up,down disable
	Outp32(0xE0200180, 0x00002222);	//set GPF3 as VD[23:20]
	Outp32(0xE0200188,0x0);			//set pull-up,down disable

	Outp32(0xe02000A0, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe02000A8,0x0);			//set pull-up,down disable
	Outp32(0xe02000A4,(1<<3));

	Outp32(0xE0200040, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200048,0x0);			//set pull-up,down disable
	Outp32(0xE0200044,0);

	Outp32(0xE02000c0, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE02000c8,0x0);			//set pull-up,down disable
	Outp32(0xE02000c4,0);

	Outp32(0xE0200C00, 0x11111111);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xE0200C08,0x0);			//set pull-up,down disable
	Outp32(0xE0200C04,0);
	Delay(50);
	Outp32(0xE0200C04,0xffffffff);

	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(CS_GPIO_ADDRESS+4,(Inp32(CS_GPIO_ADDRESS+4) |=(1<<CS_GPIO_BIT)));
	Outp32(SCL_GPIO_ADDRESS+4,(Inp32(SCL_GPIO_ADDRESS+4) |= (1<<SCL_GPIO_BIT)));
	Outp32(SDA_GPIO_ADDRESS+4,(Inp32(SDA_GPIO_ADDRESS+4) |= (1<<SDA_GPIO_BIT)));

	Outp32(0x11c00004, 0xf0);
	Outp32(0x11c00010, 0x50505);
	Outp32(0x11c00014, 0x50505);
	Outp32(0x11c00170, 0x0);
	Outp32(0x11c00018, 0x18f9df);
	Outp32(0x11c00000, 0x0);
	Outp32(0x11c00000, 0x190);
	Outp32(0x11c00130, 0x20);
	Outp32(0x11c00020, 0x0);
	Outp32(0x11c00024, 0x0);
	Outp32(0x11c00028, 0x0);
	Outp32(0x11c0002c, 0x0);
	Outp32(0x11c00030, 0x0);
	Outp32(0x11c00034, 0x0);
	Outp32(0x11c00180, 0x0);
	Outp32(0x11c00184, 0x0);
	Outp32(0x11c00188, 0x0);
	Outp32(0x11c0018c, 0x0);
	Outp32(0x11c00190, 0x0);
	Outp32(0x11c00140, 0x0);
	Outp32(0x11c00148, 0x0);
	Outp32(0x11c00150, 0x0);
	Outp32(0x11c00158, 0x0);
	Outp32(0x11c00058, 0x0);
	Outp32(0x11c00208, 0x0);
	Outp32(0x11c0020c, 0x0);
	Outp32(0x11c00068, 0x0);
	Outp32(0x11c00210, 0x0);
	Outp32(0x11c00214, 0x0);
	Outp32(0x11c00078, 0x0);
	Outp32(0x11c00218, 0x0);
	Outp32(0x11c0021c, 0x0);
	Outp32(0x11c00088, 0x0);
	Outp32(0x11c00220, 0x0);
	Outp32(0x11c00224, 0x0);
	Outp32(0x11c00260, 0x1);
	Outp32(0x11c00034, 0x0);
	Outp32(0x11c000a4, uFbAddr + 0x0);
	Outp32(0x11c000d4, uFbAddr + 0x177000);
	Outp32(0x11c000a0, uFbAddr + 0x0);
	Outp32(0x11c000d0, uFbAddr + 0x177000);
	Outp32(0x11c020a0, uFbAddr + 0x0);
	Outp32(0x11c020d0, uFbAddr + 0x177000);
	Outp32(0x11c00100, 0x780);
	Outp32(0x11c00020, 0x8000);
	Outp32(0x11c00020, 0x802c);
	Outp32(0x11c00040, 0x0);
	Outp32(0x11c00044, 0xefb1f);
	Outp32(0x11c00200, 0xffffff);
	Outp32(0x11c00204, 0xffffff);
	Outp32(0x11c00034, 0x400);
	Outp32(0x11c00020, 0x802d);
	Outp32(0x11c00034, 0x1);
	Outp32(0x11c00034, 0x1);
	Outp32(0x11c00034, 0x401);
	Outp32(0x11c00020, 0x802d);
	Outp32(0x11c00034, 0x1);
	Outp32(0x11c00034, 0x1);

	Outp32(0x11c00000, 0x193);

//	LCDM_InitLdi(LCD_TL2796, 0);
	LCDM_InitTL2796();
}
#elif defined(CFG_LCD_NONAME1)
/*
 *
 */
  	
#define LCD_WIDTH		1024
#define LCD_HEIGHT		800	

void lcd_gpio_init(void)
{
	// 3.3V--10ms-->LVDS PD--200ms-->VLED--10ms-->PWM--10ms-->LED_EN

	//---1) output 3v3

	/* Seting LVDS_PWDN OUTPUT HIGH*/
	Outp32(GPL1CON, ((Inp32(GPL1CON) & (~(0xf <<0)))|0x01));
	Outp32(GPL1DAT, (Inp32(GPL1DAT) | (0x1)));
	//printf("2) Enable LVDS trans!-->GPL1[0]: output high\n");
	//Delay(200000);
	/*--Turn on Backlight-->GPC1[2] VLED_EN: output high -->On 12V---------------*/
	Outp32(GPC1CON,(Inp32(GPC1CON)&(~(0xf << 8)))|(0x1 << 8));
	Outp32(GPC1DAT,Inp32(GPC1DAT)&(~(0x1 << 2))|(0x1 << 2));
	//printf("3) Turn on Backlight!-->GPC1[2]: output high -->On 12V\n");
	//Delay(10000);	
	/*---->GPC0[1]:VLED_ON output high -->On 12V---------------*/
	Outp32(GPC0CON,(Inp32(GPC0CON)&(~(0xf << 4)))|(0x1 << 4));
	Outp32(GPC0DAT,(Inp32(GPC0DAT)&(~(0x1 << 1)))|(0x1 << 1));
	//printf("4) Turn on Backlight!-->GPC0[1]: output high -->On 12V\n");
	//Delay(10);	
	
	/*Output PWM!-->GPD0[1]: output high -->100%%*/
	Outp32(GPD0CON,(Inp32(GPD0CON) & (~(0xf<<4)))|(0x1<<4)); //stop timer0
	Outp32(GPD0DAT,(Inp32(GPD0DAT) & (~(0x1 << 1)))|(0x1 << 1)); // prescaler to 80
	//Delay(10000);
	//printf("5) Output PWM!-->GPD0[1]: output high -->100%%\n");

	/*-Enable Backlight!-->GPC1[1] LED_EN18: output high -->enable-*/
	Outp32(GPC1CON,(Inp32(GPC1CON)&(~(0xf << 4)))|(0x1 << 4));
	Outp32(GPC1DAT,(Inp32(GPC1DAT)&(~(0x1 << 1)))|(0x1 << 1));

	//printf("6) Enable Backlight!-->GPC1[1]: output high -->enable\n");

	/*---------GPIO CONTROL---------*/
	Outp32(GPF0CON, (Inp32(GPF0CON) & (~(0xFF<<16)))|0x22002222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	//Outp32(0xe0200128,0x0);			//set pull-up,down disable
	
	Outp32(GPF1CON, (Inp32(GPF1CON) & (~(0xFF<<16)))|0x22002222);	//set GPF1 as VD[11:4]
	//Outp32(0xe0200148,0x0);			//set pull-up,down disable

	Outp32(GPF2CON, (Inp32(GPF2CON) & (~(0xFF<<16)))|0x22002222);	//set GPF2 as VD[19:12]
	//Outp32(0xe0200168,0x0);			//set pull-up,down disable

	Outp32(GPF3CON, (Inp32(GPF3CON) & (~(0xFF<<16)))|0x22002222);	//set GPF3 as VD[23:20]
	//Outp32(0xe0200188,0x0);			//set pull-up,down disable

	//printf("7) Enable Backlight!-->GPC1[1]: output high -->enable\n");
	
	//--------- S5PC110 EVT0 needs MAX drive strength	---------//
	//Outp32(0xe020012c,0xffffffff);			//set GPF0 drive strength max by WJ.KIM(09.07.17)
	//Outp32(0xe020014c,0xffffffff);			//set GPF1 drive strength max by WJ.KIM(09.07.17)
	//Outp32(0xe020016c,0xffffffff);			//set GPF2 drive strength max by WJ.KIM(09.07.17)
	//Outp32(0xe020018c,0x3ff);					//set GPF3 drive strength max by WJ.KIM(09.07.17)

}
void fimd_setting(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;

	u32 i;
	u32* pBuffer = (u32*)uFbAddr;

/**/Outp32(0x11c00004, 0x60);
	Outp32(0x11c00010, 0xe0e0305);
	Outp32(0x11c00014, 0x3103020);
	Outp32(0x11c00170, 0x0);
	Outp32(0x11c00018, 0x12bbff);
	Outp32(0x11c00000, 0x0);
	Outp32(0x11c00000, 0x254);
	Outp32(0x11c00130, 0x20);


	Outp32(0x11c00020, 0x0);
	Outp32(0x11c00024, 0x0);
	Outp32(0x11c00028, 0x0);
	Outp32(0x11c0002c, 0x0);
	Outp32(0x11c00030, 0x0);
	Outp32(0x11c00034, 0x0);
	Outp32(0x11c00180, 0x0);
	Outp32(0x11c00184, 0x0);
	Outp32(0x11c00188, 0x0);
	Outp32(0x11c0018c, 0x0);
	Outp32(0x11c00190, 0x0);
	Outp32(0x11c00140, 0x0);
	Outp32(0x11c00148, 0x0);
	Outp32(0x11c00150, 0x0);
	Outp32(0x11c00158, 0x0);
	Outp32(0x11c00058, 0x0);
	Outp32(0x11c00208, 0x0);
	Outp32(0x11c0020c, 0x0);
	Outp32(0x11c00068, 0x0);
	Outp32(0x11c00210, 0x0);
	Outp32(0x11c00214, 0x0);
	Outp32(0x11c00078, 0x0);
	Outp32(0x11c00218, 0x0);
	Outp32(0x11c0021c, 0x0);
	Outp32(0x11c00088, 0x0);
	Outp32(0x11c00220, 0x0);
	Outp32(0x11c00224, 0x0);

/**/	
	Outp32(0x11c00260, 0x1);
	Outp32(0x11c00048, 0x100200);
	Outp32(0x11c00200, 0xffffff);
	Outp32(0x11c00204, 0xffffff);
	Outp32(0x11c00034, 0x0);
	Outp32(0x11c00020, 0x802c);
	
	Outp32(0x11c000a0, uFbAddr + 0x00000000);
	Outp32(0x11c000d0, uFbAddr + 0x00400800);
	Outp32(0x11c000a4, uFbAddr + 0x00000000);
	Outp32(0x11c000d4, uFbAddr + 0x00400800);	
	Outp32(0x11c020a0, uFbAddr + 0x00000000); 
	Outp32(0x11c020d0, uFbAddr + 0x00400800);

//=// /**/
/*******************************************************************/
	Outp32(0x11c00100, 0x1558);//=// virtual screen offset 
	Outp32(0x11c00040, 0x0); //window 0 position control a register
	Outp32(0x11c00044, 0x2aaaff);//Window 0 Position Control B Register//=//
	Outp32(0x11c00020, 0x802d); //Window 0 Control Register//=//
	Outp32(0x11c00034, 0x1);//Window Shadow Control Register   bit0: enable channel 0
	Outp32(0x11c00020, 0x802d);//=//window 0 control register
	Outp32(0x11c00034, 0x1); // window shadow control register
//	Outp32(0x11c00000, 0x257);
//	Outp32(0x11c00000, 0x57); //===> MPLL should be 667 !!!!

	Outp32(0x11c00000, 0x53); //Video Main Control 0 Register
	//Outp32(0x11c00010, 0x60400); //Video Time Control 0 Register
	Outp32(0x11c00010, 0xff160b01); //Video Time Control 0 Register
	Outp32(0x11c001a4, 0x3); //I80 / RGB Trigger Control Register
/*******************************************************************/	


	Outp32(0x10010210,0x3); //syscon output path
	Outp32(0x1003c234,Inp32(0x1003c234)| 0x6); //clk_src_lcd0 = mpll
	Outp32(0x1003c534,Inp32(0x1003c534)| 0xB); //clk_src_lcd0 = mpll/12
	LCD_setprogress(0);
	//if (flag) LCD_setprogress(0);

}
void display_setting(void) //alpha setting
{
	Outp32(0x10010210,0x3); //syscon output path
	Outp32(0x1003c234,(Inp32(0x1003c234)&(~0xf))| 0x6); //clk_src_lcd0 = mpll
	Outp32(0x1003c534,(Inp32(0x1003c534)&(~0xf))| 0xB); //clk_src_lcd0 = mpll/12

	Outp32(rVIDCON0, (0<<30)|(0<<26)|(0<<23)|(0<<20)|(0<<18)|(0<<17)|(0<<16)|(0<<6)|(1<<5)|(0<<1)|(0<<0));
	Outp32(rVIDCON1, (1<<9)|(1<<7)|(0<<6)|(0<<5)|(0<<4));

	Outp32(rVIDTCON0, (7<<16)|(9<<8)|(7<<0));
	Outp32(rVIDTCON1, (50<<16)|(60<<8)|(50<<0));
	Outp32(rVIDTCON2, (799<<11)|(1279<<0));
	//Outp32(0x11c0001c, 0x0);
	Outp32(rWINCON0, Inp32(rWINCON0) |(1<<15)|(8<<2)|(1<<1));

	Outp32(rVIDOSD0A, Inp32(rVIDOSD0A) |(0<<11)|(0<<0));


	Outp32(rVIDOSD0B, Inp32(rVIDOSD0B) |(1279<<11)|(799<<0));



	Outp32(rVIDW00ADD0B0, CFG_LCD_FBUFFER);


	Outp32(rVIDW00ADD1B0, CFG_LCD_FBUFFER+0x3E8000);



	Outp32(rVIDW0ALPHA0, 0x0F0F0F);


	Outp32(rVIDW0ALPHA1, 0x0F0F0F);
	
	Outp32(rSHADOWCON, Inp32(rSHADOWCON) |(0<<5)|(1<<0));

	Outp32(rVIDW00ADD2, 0x1400);
//---5. Start Output	
	Outp32(rVIDCON0, Inp32(rVIDCON0) |(3<<0));
	Outp32(rWINCON0, Inp32(rWINCON0) |(1<<0));


}
void display_color(void	)
{
	int i =0 ,j =0 ;
for(i=0;i<200;i++)
	{
		for(j=0;j<1280;j++)
			*((u32 *)CFG_LCD_FBUFFER+1280*i+j) = 0x3FFFF;	
	   }
	for(i=200;i<400;i++)
	{
		for(j=0;j<1280;j++)
			*((u32 *)CFG_LCD_FBUFFER+1280*i+j) = 0x00FC0;	
	   }
	for(i=400;i<600;i++)
	{
		for(j=0;j<1280;j++)
			*((u32 *)CFG_LCD_FBUFFER+1280*i+j) = 0x3F000;	
	   }
	for(i=600;i<800;i++)
	{
		for(j=0;j<1280;j++)
			*((u32 *)CFG_LCD_FBUFFER+1280*i+j) = 0x0003F;	
	   }
}
#define BOOT_LOGO_BITMAP_WIDTH	320
#define BOOT_LOGO_BITMAP_HEIGHT	128

void exynos_show_logo (const unsigned char * buf)
{
	unsigned long i, j, sx, sy;
	unsigned long* pBuffer = (unsigned long*)CFG_LCD_FBUFFER;
	unsigned short* pBitmap = (unsigned short*)buf;
	unsigned long iBitmapData;
	//unsigned short* pBitmap_max= pBitmap+PART_SIZE_LOGO;

	//printf(" the map start is:%x, end is %x\n",pBitmap,pBitmap_max);
	sx = (1280-BOOT_LOGO_BITMAP_WIDTH)>>1;
	sy = (800-BOOT_LOGO_BITMAP_HEIGHT)>>1;

	for (i=sy; i<sy+BOOT_LOGO_BITMAP_HEIGHT; i++)
	{
		for (j=sx; j<sx+BOOT_LOGO_BITMAP_WIDTH; j++)
		{
			iBitmapData = 0xFFFC<<18;
			iBitmapData |= ((*pBitmap)&0xf800)<<2;
			iBitmapData |= ((*pBitmap)&0x07e0)<<1;
			iBitmapData |= ((*pBitmap)&0x1f)<<1;
			*(pBuffer+(i*1280)+j) = iBitmapData;
			pBitmap++;
		}
	}

}

unsigned  char* logo_buf;

void exynos_get_logo(int index)
{
	char run_cmd[100];
	
	//printf("%s bootlogo[%d] \n ", "reading",index);
	sprintf(run_cmd,"movi %s logo 0x%x %x","read",logo_buf,index);
	run_command(run_cmd,0);
}
void exynos_display_pic(int index)
{
	int i = 0;

	//printf("show boot logo\n");

	logo_buf = (unsigned  char*)malloc(PART_SIZE_LOGO);
	memset((void*)CFG_LCD_FBUFFER, 0x00, 1280*800*4);

	if(index == 1)
	{
		 for(i = 1;i<4;i++)
		 {
			exynos_get_logo(i);
	 		exynos_show_logo(logo_buf);
			udelay(8000000);
			udelay(8000000);
			udelay(8000000);
		 }
	}
	else
	{
			exynos_get_logo(index);
	 		exynos_show_logo(logo_buf);
	}
	free(logo_buf);

}
void LCD_Initialize_NONAME1(void)
{
	//memset((void*)CFG_LCD_FBUFFER, 0x00, 1280*800*4);
	lcd_gpio_init();
	display_setting();
	//display_color();
	memset((void*)CFG_LCD_FBUFFER, 0x00, 1280*800*4);

}
#else
// "No LCD Type is defined!"
#endif

/*
 */
void Exynos_LCD_turnon(void)
{
	//printf("[mjdbg] turn on the lcd \n");
#if defined(CFG_LCD_TL2796)
	LCD_Initialize_TL2796();
#elif defined(CFG_LCD_NONAME1)
	LCD_Initialize_NONAME1();
#endif
}

void Exynose_LCD_setfgcolor(unsigned int color)
{
	gFgColor = color;
}

void Exynose_LCD_setleftcolor(unsigned int color)
{
	gLeftColor = color;
}

void Exynose_LCD_setprogress(int percentage)
{
#if defined(CFG_LCD_TL2796) || defined(CFG_LCD_NONAME1)
	u32 i, j;
	u32* pBuffer = (u32*)CFG_LCD_FBUFFER;

	for (i=0; i < (LCD_HEIGHT/100)*percentage; i++)
	{
		for (j=0; j < LCD_WIDTH; j++)
		{
			*pBuffer++ = gFgColor;
		}
	}

	for (; i < LCD_HEIGHT; i++)
	{
		for (j=0; j < (LCD_WIDTH >> 5); j++)
		{
			*pBuffer++ = gLeftColor;
		}
		for (; j < LCD_WIDTH; j++)
		{
			*pBuffer++ = LCD_BGCOLOR;
		}
	}
#endif
}
