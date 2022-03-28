#include "OV7670.h"

volatile uint16_t Frame_Buffer[IMG_ROWS * IMG_COLUMNS];
volatile uint8_t frame_buffer[IMG_ROWS*IMG_COLUMNS];

//tao xung 24MHz noi cho camera
void OV7670_XCLK_Init(void)
{
	//F internal = F input / (CLKCR + 1) = 8 / 2 = 4;
	SCCB_write_reg(CLKRC, 0x01);
	// PLL x 6
	SCCB_write_reg(DBLV, 0x80);
}

//cau hinh chan nhan du lieu
void OV7670_Pin_Config(void)
{
	//config gpio2 output
    gpio_config_t gpio_initstructure;
    gpio_initstructure.pin_bit_mask = RET_SEL | PWDN_SEL;
    gpio_initstructure.mode = GPIO_MODE_OUTPUT;
    gpio_initstructure.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_initstructure.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_initstructure.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&gpio_initstructure);

	//config gpio15 input;
    gpio_initstructure.pin_bit_mask = D0_SEL | D1_SEL | D2_SEL | D3_SEL | D4_SEL | D5_SEL | D6_SEL | D7_SEL | VSYNC_SEL | HREF_SEL | PCLK_SEL;
    gpio_initstructure.mode = GPIO_MODE_INPUT;
    gpio_config(&gpio_initstructure);
	gpio_set_level(Pin_RET, 1);
	gpio_set_level(Pin_PWDN, 0);
}

//bat anh mau RGB565
void Capture_Image(uint16_t Cot, uint16_t Hang)
{
	uint16_t i, x, y, z;
	i = 0;
	while(!gpio_get_level(Pin_VSYNC)); // tin hieu VSYN cao ket thuc 1 frame
	while(gpio_get_level(Pin_VSYNC)); // tin hieu VSYN thap bat dau 1 frame
	y = Hang; //so line
	while(y--)
		{
			x = Cot; //so pixel tren 1 line
			while(!gpio_get_level(Pin_HREF)); // tin hieu HREF cao bat dau 1 line
			while(x--)
				{
					while(gpio_get_level(Pin_PCLK));// doi PCLK xuong thap
					z = ((gpio_get_level(D7) << 7) | (gpio_get_level(D6) << 6) | (gpio_get_level(D5) << 5) | (gpio_get_level(D4) << 4) | (gpio_get_level(D3) << 3) | (gpio_get_level(D2) << 2) | (gpio_get_level(D1) << 1) | gpio_get_level(D0)) << 8; //byte 1
					while(!gpio_get_level(Pin_PCLK));//doi PCLK len cao
					while(gpio_get_level(Pin_PCLK));//doi PLCK xuong thap
					//luu vao bo nho dem frame_buffer
					Frame_Buffer[i] = ((gpio_get_level(D7) << 7) | (gpio_get_level(D6) << 6) | (gpio_get_level(D5) << 5) | (gpio_get_level(D4) << 4) | (gpio_get_level(D3) << 3) | (gpio_get_level(D2) << 2) | (gpio_get_level(D1) << 1) | gpio_get_level(D0)) | z; //byte 2
					while(!gpio_get_level(Pin_PCLK));//doi PCLK len cao
					i++;
				}
		}
}

//bat anh xam
void CaptureVGA(uint16_t Cot, uint16_t Hang)
{
	uint16_t i = 0, x, y;
	while(!gpio_get_level(Pin_VSYNC)); // tin hieu VSYN cao ket thuc 1 frame
	while(gpio_get_level(Pin_VSYNC)); // tin hieu VSYN thap bat dau 1 frame
	y = Hang; //so line
	while(y--)
		{
			x = Cot; //so pixel tren 1 line
			while(!gpio_get_level(Pin_HREF)); // tin hieu HREF cao bat dau 1 line
			while(x--)
				{
					while(gpio_get_level(Pin_PCLK));// doi PCLK xuong thap
					frame_buffer[i] = ((gpio_get_level(D7) << 7) | (gpio_get_level(D6) << 6) | (gpio_get_level(D5) << 5) | (gpio_get_level(D4) << 4) | (gpio_get_level(D3) << 3) | (gpio_get_level(D2) << 2) | (gpio_get_level(D1) << 1) | gpio_get_level(D0)); //byte 1
					while(!gpio_get_level(Pin_PCLK));//doi PCLK len cao
					while(gpio_get_level(Pin_PCLK));//doi PLCK xuong thap
					while(!gpio_get_level(Pin_PCLK));//doi PCLK len cao
					i++;
				}
		}
}

//cau hinh che do QQVGA, RGB565
void OV7670_QQVGA_RGB565_Init(void)
{
	SCCB_write_reg(COM7, 0x80); //COM7, RESET camera
	vTaskDelay(500 / portTICK_PERIOD_MS);
	//Video format RGB565
	SCCB_write_reg(COM7, 0x04); // COM7, output format RGB
	SCCB_write_reg(COM15, 0xD0); // COM15, output format RGB565
	SCCB_write_reg(RGB444, 0x00); // disable RGB444
	SCCB_write_reg(COM1, 0x00); // COM1, disable CCIR656
	//Tan so dao dong noi camrea
	SCCB_write_reg(DBLV, 0x80); //nhan tan so voi 6
	SCCB_write_reg(CLKRC, 0x07); //chia tan so 8 => Fin = 6MHz
	// scalling windows
	SCCB_write_reg(COM3, 0x04); // COM3, DCW enable
	SCCB_write_reg(COM14, 0x1A); // COM14, chia tan so PCLK: chia 4
	SCCB_write_reg(SCALLING_XSC, 0x3A); // SCALLING_XSC
	SCCB_write_reg(SCALLING_YSC, 0x35); // SCALLING_YSC
	SCCB_write_reg(SCALLING_DCWCTR, 0x22); // downsample VGA by 4 -> QQVGA
	SCCB_write_reg(SCALLING_PCLK_DIV, 0xF2); //
	SCCB_write_reg(MVFP, 0x0); // MVFP, Mirror/Flip hinh anh
	//SCCB_write_reg(COM10, 0x32); // COM10, tat tin hieu PCLK khi HREF bank
	// Hardware window QQVGA
	SCCB_write_reg(TSLB, 0x04); // TSLB, cho phep thay doi tham so hardware windows
	SCCB_write_reg(HSTART, 0x16); // REG_HSTART
	SCCB_write_reg(HSTOP, 0x04); // REG_HSTOP
	SCCB_write_reg(REG_HREF, 0x24); // REG_HREF
	SCCB_write_reg(VSTART, 0x02); // REG_VSTART
	SCCB_write_reg(VSTOP, 0x7A); // REG_VSTOP
	SCCB_write_reg(VREF, 0x0A); // REG_VREF
	// Matrix coefficients, saturation = 0,
	SCCB_write_reg(MTX1, 0x80); // MTX1
	SCCB_write_reg(MTX2, 0x80); // MTX2
	SCCB_write_reg(MTX3, 0x00); // MTX3
	SCCB_write_reg(MTX4, 0x22); // MTX4
	SCCB_write_reg(MTX5, 0x5E); // MTX5
	SCCB_write_reg(MTX6, 0x80); // MTX6
	SCCB_write_reg(MTXS, 0x9E ); // MTXS
	// Gamma
	SCCB_write_reg(SLOP, 0x20); // SLOP
	SCCB_write_reg(GAM1, 0x10); // GAM1
	SCCB_write_reg(GAM2, 0x1E); // GAM2
	SCCB_write_reg(GAM3, 0x35); // GAM3
	SCCB_write_reg(GAM4, 0x5A); // GAM4
	SCCB_write_reg(GAM5, 0x69); // GAM5
	SCCB_write_reg(GAM6, 0x76); // GAM6
	SCCB_write_reg(GAM7, 0x80); // GAM7
	SCCB_write_reg(GAM8, 0x88); // GAM8
	SCCB_write_reg(GAM9, 0x8F); // GAM9
	SCCB_write_reg(GAM10, 0x96); // GAM10
	SCCB_write_reg(GAM11, 0xA3); // GAM11
	SCCB_write_reg(GAM12, 0xAF); // GAM12
	SCCB_write_reg(GAM13, 0xC4); // GAM13
	SCCB_write_reg(GAM14, 0xD7); // GAM14
	SCCB_write_reg(GAM15, 0xE8); // GAM15
	//While balance, can bang trang
	SCCB_write_reg(COM8, 0xA7); // COM8, AWB on | FASTAEC | AECSTEP | AGC on | AEC on
	SCCB_write_reg(AWBC1, 0x0A); // AWBC1
	SCCB_write_reg(AWBC2, 0xF0); // AWBC2
	SCCB_write_reg(AWBC3, 0x34); // AWBC3
	SCCB_write_reg(AWBC4, 0x58); // AWBC4
	SCCB_write_reg(AWBC5, 0x28); // AWBC5
	SCCB_write_reg(AWBC6, 0x3A); // AWBC6
	SCCB_write_reg(AWBC7, 0x88); // AWBC7
	SCCB_write_reg(AWBC8, 0x88); // AWBC8
	SCCB_write_reg(AWBC9, 0x44); // AWBC9
	SCCB_write_reg(AWBC10, 0x67); // AWBC10
	SCCB_write_reg(AWBC11, 0x49); // AWBC11
	SCCB_write_reg(AWBC12, 0x0E); // AWBC12
	SCCB_write_reg(AWBCTR3, 0x0A); // AWBCTR3
	SCCB_write_reg(AWBCTR2, 0x55); // AWBCTR2
	SCCB_write_reg(AWBCTR1, 0x11); // AWBCTR1
	SCCB_write_reg(AWBCTR0, 0x9F); // AWBCTR0, 9e -> advance AWB, 9f -> simple AWB
	SCCB_write_reg(0x6a, 0x40); // G chanel gain
	SCCB_write_reg(0x01, 0x40); // B chanel gain
	SCCB_write_reg(0x02, 0x40); // R chanel gain
	SCCB_write_reg(0x14, 0x68); // REG_COM9
	// Bright, do sang hinh anh
	SCCB_write_reg(0x55, 0x00); // Bright =0
	//SCCB_write_reg( 0x55, 0x18 ); // Bright =1
	//SCCB_write_reg( 0x55, 0x30 ); // Bright =2
	//SCCB_write_reg( 0x55, 0x98 ); // Bright =-1
	//SCCB_write_reg( 0x55, 0xb0 ); // Bright =-2
	// Contrast, do tuong phan hinh anh
	SCCB_write_reg(0x56, 0x40); // contrast =0
	//SCCB_write_reg( 0x56, 0x50 ); // contrast =1
	//SCCB_write_reg( 0x56, 0x60 ); // contrast =2
	//SCCB_write_reg( 0x56, 0x38 ); // contrast =-1
	//SCCB_write_reg( 0x56, 0x40 ); // contrast =-2
	// Hieu ung: normal
	SCCB_write_reg(0x67, 0xc0);
	SCCB_write_reg(0x68, 0x80);
	// Banding filter 50Hz
	SCCB_write_reg(0x3B, 0x0A); // COM11, chon bo loc 50Hz
	SCCB_write_reg(0x9d, 0x4c); // BD50ST, 50Hz banding filter value, active khi COM8[5] high, COM11[3] high
	SCCB_write_reg(0xA5, 0x05); // BD50MAX, max banding filter step
	SCCB_write_reg(0x0E, 0x61); // COM5
	SCCB_write_reg(0x0F, 0x4b); // COM6
	SCCB_write_reg(0x16, 0x02); //
	SCCB_write_reg(0x21, 0x02); // ADCCTR1
	SCCB_write_reg(0x22, 0x91); // ADCCTR2
	SCCB_write_reg(0x29, 0x07); // RSVD
	SCCB_write_reg(0x33, 0x0b); // CHLF
	SCCB_write_reg(0x35, 0x0b); //
	SCCB_write_reg(0x37, 0x1d); // ADC control
	SCCB_write_reg(0x38, 0x71); // ADC and Analog Common mode control
	SCCB_write_reg(0x39, 0x2a); // ADC Offset control
	SCCB_write_reg(0x4d, 0x40); // DM Pos, dummy row position
	SCCB_write_reg(0x4e, 0x20); //
	SCCB_write_reg(0x8d, 0x4f); //
	SCCB_write_reg(0x8e, 0x0); //
	SCCB_write_reg(0x8f, 0x0); //
	SCCB_write_reg(0x90, 0x0); //
	SCCB_write_reg(0x91, 0x0); //
	SCCB_write_reg(0x96, 0x0); //
	SCCB_write_reg(0x9a, 0x0); //
	SCCB_write_reg(0xb0, 0x84); // No document
	SCCB_write_reg(0xb1, 0x0c); // ABLC1, 1100, enable ABLC function
	SCCB_write_reg(0xb2, 0x0e); //
	SCCB_write_reg(0xb3, 0x82); // ABLC target
	SCCB_write_reg(0xb8, 0x0a); //
	SCCB_write_reg(0x3F, 0x0); // REG_EDGE Enhancement Adjustment
	SCCB_write_reg(0x74, 0x10); // REG74 0001 0000, digital gain manual control bypass.
	SCCB_write_reg(0x75, 0x05); // REG75, Edge enhancement lower limit
	SCCB_write_reg(0x76, 0xe1); // REG76, 1110 0001, [6:5]enable black/white pixel correct, [4:0]Edge enhancement higher limit
	SCCB_write_reg(0x77, 0x01); // REG77, de-noise range control
	SCCB_write_reg(0x4c, 0x0); // De-noise strength
	SCCB_write_reg(0x4b, 0x09); // UV average enable
	SCCB_write_reg(0xc9, 0x60); // Saturation control, bao hoa
	SCCB_write_reg(0x34, 0x11); // Array reference control
}
