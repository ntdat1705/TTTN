#ifndef __OV7670_H
#define __OV7670_H


#include "main.h"
/*----------------------------------------------------------------------------*/
// Image settings
#define IMG_ROWS   			120
#define IMG_COLUMNS   		160

// Image buffer
extern volatile uint16_t Frame_Buffer[IMG_ROWS*IMG_COLUMNS];
extern volatile uint8_t frame_buffer[IMG_ROWS*IMG_COLUMNS];


#define OV7670_Addr		    0x21
#define XCLK_SEL 				GPIO_SEL_5		//GPIOA
#define D0_SEL					GPIO_SEL_22		//GPIOA
#define D1_SEL					GPIO_SEL_33		//GPIOA
#define D2_SEL					GPIO_SEL_21		//GPIOA
#define D3_SEL					GPIO_SEL_25		//GPIOA
#define D4_SEL					GPIO_SEL_19		//GPIOA
#define D5_SEL					GPIO_SEL_26		//GPIOA
#define D6_SEL					GPIO_SEL_18		//GPIOA
#define D7_SEL					GPIO_SEL_27		//GPIOA

#define VSYNC_SEL			GPIO_SEL_12		//GPIOB
#define HREF_SEL			GPIO_SEL_2		//GPIOB
#define PCLK_SEL			GPIO_SEL_14		//GPIOB
#define RET_SEL             GPIO_SEL_32
#define PWDN_SEL            GPIO_SEL_23

#define XCLK 				GPIO_NUM_5		//GPIOA
#define D0					GPIO_NUM_22		//GPIOA
#define D1					GPIO_NUM_33		//GPIOA
#define D2					GPIO_NUM_21		//GPIOA
#define D3					GPIO_NUM_25		//GPIOA
#define D4					GPIO_NUM_19		//GPIOA
#define D5					GPIO_NUM_26		//GPIOA
#define D6					GPIO_NUM_18		//GPIOA
#define D7					GPIO_NUM_27		//GPIOA

#define Pin_VSYNC			GPIO_NUM_12		//GPIOB
#define Pin_HREF			GPIO_NUM_2		//GPIOB
#define Pin_PCLK			GPIO_NUM_14		//GPIOB
#define Pin_RET             GPIO_NUM_32
#define Pin_PWDN            GPIO_NUM_23

//ov7670 reg
#define DBLV				0x6B
#define CLKRC				0x11
#define RGB444				0x8C
#define COM1				0x04
#define COM3				0x0C
#define COM7				0x12
#define COM8				0x13
#define COM10				0x15
#define COM14				0x3E
#define COM15				0x40
#define SCALLING_XSC		0x70
#define SCALLING_YSC		0x71
#define SCALLING_DCWCTR		0x72
#define SCALLING_PCLK_DIV	0x73
#define MVFP				0x1E
#define TSLB				0x3A
#define HSTART				0x17
#define HSTOP				0x18
#define REG_HREF			0x32
#define VSTART				0x19
#define VSTOP				0x1A
#define	VREF				0x03
#define MTX1				0x4f
#define MTX2				0x50
#define MTX3				0x51
#define MTX4				0x52
#define MTX5				0x53
#define MTX6				0x54
#define MTXS				0x58
#define SLOP				0x7A
#define GAM1				0x7B
#define GAM2				0x7C
#define GAM3				0x7D
#define GAM4				0x7E
#define GAM5				0x7F
#define GAM6				0x80
#define GAM7				0x81
#define GAM8				0x82
#define GAM9				0x83
#define GAM10				0x84
#define GAM11				0x85
#define GAM12				0x86
#define GAM13				0x87
#define GAM14				0x88
#define GAM15				0x89
#define AWBC1				0x43
#define AWBC2				0x44
#define AWBC3				0x45
#define AWBC4				0x46
#define AWBC5				0x47
#define AWBC6				0x48
#define AWBC7				0x59
#define AWBC8				0x5A
#define AWBC9				0x5B
#define AWBC10				0x5C
#define AWBC11				0x5D
#define AWBC12				0x5E
#define AWBCTR3				0x6C
#define AWBCTR2				0x6D
#define AWBCTR1				0x6E
#define AWBCTR0				0x6F

void OV7670_XCLK_Init(void);
void OV7670_Pin_Config(void);
void Capture_Image(uint16_t Cot, uint16_t Hang);
void CaptureVGA(uint16_t Cot, uint16_t Hang);
void OV7670_QQVGA_RGB565_Init(void);

#endif
