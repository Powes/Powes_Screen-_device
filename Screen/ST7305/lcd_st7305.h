#ifndef __TFTLCD_ST7305_H
#define __TFTLCD_ST7305_H	 
//#include "sys.h"
//#include "stdlib.h"	

 
#define TFTLCD_SCL_GPIO_PORT				GPIOB
#define TFTLCD_SCL_GPIO_PIN				GPIO_PIN_9
				
#define TFTLCD_SDA_GPIO_PORT				GPIOB
#define TFTLCD_SDA_GPIO_PIN				GPIO_PIN_8
				
#define TFTLCD_RST_GPIO_PORT				GPIOB
#define TFTLCD_RST_GPIO_PIN				GPIO_PIN_7
				
#define TFTLCD_DC_GPIO_PORT				GPIOB
#define TFTLCD_DC_GPIO_PIN				GPIO_PIN_6
				
#define TFTLCD_CS_GPIO_PORT				GPIOB
#define TFTLCD_CS_GPIO_PIN				GPIO_PIN_5
 
#define TFTLCD_BLK_GPIO_PORT				GPIOB
#define TFTLCD_BLK_GPIO_PIN				GPIO_PIN_3

#define TFTLCD_TE_GPIO_PORT				GPIOB
#define TFTLCD_TE_GPIO_PIN				GPIO_PIN_4



#define TFTLCD_SDA_SET    HAL_GPIO_WritePin(TFTLCD_SDA_GPIO_PORT,TFTLCD_SDA_GPIO_PIN,GPIO_PIN_SET) //BSRR--Bit Set Reset Register  设置 GPIOx  GPIOx->BSRR= GPIO_Pin11;
#define TFTLCD_SDA_RESET  HAL_GPIO_WritePin(TFTLCD_SDA_GPIO_PORT,TFTLCD_SDA_GPIO_PIN,GPIO_PIN_RESET)//BRR--Bit Reset Register  清空 GPIOx   GPIOx->BRR= GPIO_Pin11;

#define TFTLCD_SCL_SET    HAL_GPIO_WritePin(TFTLCD_SCL_GPIO_PORT,TFTLCD_SCL_GPIO_PIN,GPIO_PIN_SET)
#define TFTLCD_SCL_RESET  HAL_GPIO_WritePin(TFTLCD_SCL_GPIO_PORT,TFTLCD_SCL_GPIO_PIN,GPIO_PIN_RESET)

#define TFTLCD_CS_SET     HAL_GPIO_WritePin(TFTLCD_CS_GPIO_PORT,TFTLCD_CS_GPIO_PIN,GPIO_PIN_SET)//GPIO_SetBits
#define TFTLCD_CS_RESET   HAL_GPIO_WritePin(TFTLCD_CS_GPIO_PORT,TFTLCD_CS_GPIO_PIN,GPIO_PIN_RESET) //GPIO_ResetBits

#define TFTLCD_DC_SET     HAL_GPIO_WritePin(TFTLCD_DC_GPIO_PORT,TFTLCD_DC_GPIO_PIN,GPIO_PIN_SET)
#define TFTLCD_DC_RESET   HAL_GPIO_WritePin(TFTLCD_DC_GPIO_PORT,TFTLCD_DC_GPIO_PIN,GPIO_PIN_RESET)

#define TFTLCD_RST_SET    HAL_GPIO_WritePin(TFTLCD_RST_GPIO_PORT,TFTLCD_RST_GPIO_PIN,GPIO_PIN_SET)
#define TFTLCD_RST_RESET  HAL_GPIO_WritePin(TFTLCD_RST_GPIO_PORT,TFTLCD_RST_GPIO_PIN,GPIO_PIN_RESET)

#define TFTLCD_BLK_SET    HAL_GPIO_WritePin(TFTLCD_BLK_GPIO_PORT,TFTLCD_BLK_GPIO_PIN,GPIO_PIN_SET)
#define TFTLCD_BLK_RESET  HAL_GPIO_WritePin(TFTLCD_BLK_GPIO_PORT,TFTLCD_BLK_GPIO_PIN,GPIO_PIN_RESET)

#define USE_HORIZONTAL 0

#define TFT_COLUMN_OFFSET 0	
#define TFT_LINE_OFFSET   0


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
	#define TFTLCD_W 300
	#define TFTLCD_H 400
#else
	#define TFTLCD_W 400
	#define TFTLCD_H 300
#endif


// ============================================
// 字体配置
// ============================================
typedef enum {
    FONT_6X8 = 0,   // 6x8 小字体
    FONT_8X16       // 8x16 大字体
} Font_Size_t;


void TFTLCD_Init(void);//LCD
void LCD_DrawPoint(uint16_t x, uint16_t y, uint8_t color);
uint8_t LCD_ReadPoint(uint16_t x, uint16_t y);
void refreshLcd(void);

uint16_t LCD_DrawString(uint16_t x, uint16_t y, const char *str, Font_Size_t font, uint8_t color);


#endif


