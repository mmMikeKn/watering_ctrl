#include "global.h"
#include "core_cm3.h"
#include "stm32f10x_spi.h"

// T_CS=PA4, SPI1_SCK=PA5, SPI1_MISO=PA6, SPI1_MOSI=PA7
#define TP_CS()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define TP_DCS() GPIO_SetBits(GPIOA,GPIO_Pin_4)


void touchInit() {
	// T_CS=PA4, SPI1_SCK=PA5, SPI1_MISO=PA6, SPI1_MOSI=PA7  PENIRQ = PC13
 GPIO_InitTypeDef GPIO_InitStructure;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 GPIO_Init(GPIOA,&GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOC,&GPIO_InitStructure);

	SPI_InitTypeDef   SPI_InitStructure;

  //SPI1 Periph clock enable
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
 SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
 SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //SPI_NSS_Hard
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 SPI_InitStructure.SPI_CRCPolynomial = 7;
 SPI_Init(SPI1,&SPI_InitStructure);
 SPI_Cmd(SPI1,ENABLE);
}
//-----------------------------------------------------------------------------

static unsigned char SPI_WriteByte(unsigned char data) {
   //Wait until the transmit buffer is empty
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
  // Send the byte
 SPI_I2S_SendData(SPI1,data);
   //Wait until a data is received
 while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
  // Get the received data
 return SPI_I2S_ReceiveData(SPI1);
}

static void SpiDelay(unsigned int DelayCnt){
 unsigned int i;
 for(i=0;i<DelayCnt;i++) __NOP();
}

static uint16_t TPReadX(void) {
	uint16_t v;
 TP_CS(); SpiDelay(10);
 SPI_WriteByte(0x90); SpiDelay(10);
 v=SPI_WriteByte(0x00); v<<=8;
 v+=SPI_WriteByte(0x00); SpiDelay(10);
 TP_DCS();
 return v>>3;
}

static uint16_t TPReadY(void) {
	uint16_t v;
 TP_CS(); SpiDelay(10);
 SPI_WriteByte(0xD0); SpiDelay(10);
 v=SPI_WriteByte(0x00); v<<=8;
 v+=SPI_WriteByte(0x00); SpiDelay(10);
 TP_DCS();
 return v>>3;
}

#define cX320 170
#define cX0 3690
uint16_t touchX() {
	int32_t sx =0;
	for(int i = 0; i < 16 ; i++) {
		sx+=TPReadX();
	}
 sx = sx >> 4;
 sx = cX0-sx;
 if(sx < 0) sx = 0;
 sx = (((sx << 16)/(cX0-cX320)) * 320) >> 16;
 if(sx > 320) sx = 320;
 return sx;
}

#define cY0 310
#define cY240 3680
uint16_t touchY() {
	int32_t sy =0;
	for(int i = 0; i < 16 ; i++) {
		sy+=TPReadY();
	}
 sy = sy >> 4;
 sy -= cY0;
 if(sy < 0) sy = 0;
 sy = (((sy << 16)/(cY240-cY0)*240) >> 16);
 if(sy > 240) sy = 240;
 return sy;
}

uint8_t touchFlag() {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == Bit_RESET;
}
