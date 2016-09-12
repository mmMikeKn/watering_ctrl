#include "stm32f10x_conf.h"
#include "ili9320.h"
#include "ili9320_font.h"

//#define ili9320_WriteData(x)\
//{GPIOB->ODR=((GPIOB->ODR&0x00ff)|(x<<8));\
//GPIOC->ODR=((GPIOC->ODR&0xff00)|(x>>8));}

u16 DeviceCode;

void Lcd_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	/*开启相应时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);  
	/*所有Lcd引脚配置为推挽输出*/
	/*16位数据低8位*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/*16位数据高8位*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*控制脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/*背光控制*/
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	Lcd_Light_ON;
} 

/****************************************************************************
* 名    称：void ili9320_Initializtion()
* 功    能：初始化 ILI9320 控制器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Initializtion();
****************************************************************************/
void ili9320_Initializtion()
{
  /*****************************
  **    硬件连接说明          **
  ** STM32         ili9320    **
  ** PE0~15 <----> DB0~15     **
  ** PD15   <----> nRD        **
  ** PD14   <----> RS         **
  ** PD13   <----> nWR        **
  ** PD12   <----> nCS        **
  ** PD11   <----> nReset     **
  ** PC0    <----> BK_LED     **
  ******************************/
 	u16 i;

  	ili9320_WriteData(0xffff);
//  	Set_Rst;
	Set_nWr;
	Set_Cs;
	Set_Rs;
	Set_nRd;
//	Set_Rst;
//  ili9320_Reset();                                        // 复位 ili9320_Reset
  for(i=50000;i>0;i--);
  for(i=50000;i>0;i--);
  for(i=50000;i>0;i--);
  ili9320_WriteRegister(0x0000,0x0001);ili9320_Delay(10000);
  for(i=50000;i>0;i--);
  for(i=50000;i>0;i--);
  DeviceCode = ili9320_ReadRegister(0x0000);
	if(DeviceCode==0x9325||DeviceCode==0x9328)
	{
  		ili9320_WriteRegister(0x00e7,0x0010);      
        ili9320_WriteRegister(0x0000,0x0001);  			//start internal osc
        ili9320_WriteRegister(0x0001,0x0100);     
        ili9320_WriteRegister(0x0002,0x0700); 				//power on sequence                     
        ili9320_WriteRegister(0x0003,(1<<12)|(1<<5)|(1<<4) ); 	//65K 
        ili9320_WriteRegister(0x0004,0x0000);                                   
        ili9320_WriteRegister(0x0008,0x0207);	           
        ili9320_WriteRegister(0x0009,0x0000);         
        ili9320_WriteRegister(0x000a,0x0000); 				//display setting         
        ili9320_WriteRegister(0x000c,0x0001);				//display setting          
        ili9320_WriteRegister(0x000d,0x0000); 				//0f3c          
        ili9320_WriteRegister(0x000f,0x0000);
//Power On sequence //
        ili9320_WriteRegister(0x0010,0x0000);   
        ili9320_WriteRegister(0x0011,0x0007);
        ili9320_WriteRegister(0x0012,0x0000);                                                                 
        ili9320_WriteRegister(0x0013,0x0000);                 
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        ili9320_WriteRegister(0x0010,0x1590);   
        ili9320_WriteRegister(0x0011,0x0227);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        ili9320_WriteRegister(0x0012,0x009c);                  
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        ili9320_WriteRegister(0x0013,0x1900);   
        ili9320_WriteRegister(0x0029,0x0023);
        ili9320_WriteRegister(0x002b,0x000e);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        ili9320_WriteRegister(0x0020,0x0000);                                                            
        ili9320_WriteRegister(0x0021,0x0000);           
///////////////////////////////////////////////////////      
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        ili9320_WriteRegister(0x0030,0x0007); 
        ili9320_WriteRegister(0x0031,0x0707);   
        ili9320_WriteRegister(0x0032,0x0006);
        ili9320_WriteRegister(0x0035,0x0704);
        ili9320_WriteRegister(0x0036,0x1f04); 
        ili9320_WriteRegister(0x0037,0x0004);
        ili9320_WriteRegister(0x0038,0x0000);        
        ili9320_WriteRegister(0x0039,0x0706);     
        ili9320_WriteRegister(0x003c,0x0701);
        ili9320_WriteRegister(0x003d,0x000f);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        ili9320_WriteRegister(0x0050,0x0000);        
        ili9320_WriteRegister(0x0051,0x00ef);   
        ili9320_WriteRegister(0x0052,0x0000);     
        ili9320_WriteRegister(0x0053,0x013f);
        ili9320_WriteRegister(0x0060,0xa700);        
        ili9320_WriteRegister(0x0061,0x0001); 
        ili9320_WriteRegister(0x006a,0x0000);
        ili9320_WriteRegister(0x0080,0x0000);
        ili9320_WriteRegister(0x0081,0x0000);
        ili9320_WriteRegister(0x0082,0x0000);
        ili9320_WriteRegister(0x0083,0x0000);
        ili9320_WriteRegister(0x0084,0x0000);
        ili9320_WriteRegister(0x0085,0x0000);
      
        ili9320_WriteRegister(0x0090,0x0010);     
        ili9320_WriteRegister(0x0092,0x0000);  
        ili9320_WriteRegister(0x0093,0x0003);
        ili9320_WriteRegister(0x0095,0x0110);
        ili9320_WriteRegister(0x0097,0x0000);        
        ili9320_WriteRegister(0x0098,0x0000);  
         //display on sequence     
        ili9320_WriteRegister(0x0007,0x0133);
    
        ili9320_WriteRegister(0x0020,0x0000);                                                            
        ili9320_WriteRegister(0x0021,0x0000);
	}
	else if(DeviceCode==0x9320)
	{
		ili9320_WriteRegister(0x00,0x0000);
		ili9320_WriteRegister(0x01,0x0100);	//Driver Output Contral.
		ili9320_WriteRegister(0x02,0x0700);	//LCD Driver Waveform Contral.
//		ili9320_WriteRegister(0x03,0x1030);	//Entry Mode Set.
		ili9320_WriteRegister(0x03,0x1018);	//Entry Mode Set.
	
		ili9320_WriteRegister(0x04,0x0000);	//Scalling Contral.
		ili9320_WriteRegister(0x08,0x0202);	//Display Contral 2.(0x0207)
		ili9320_WriteRegister(0x09,0x0000);	//Display Contral 3.(0x0000)
		ili9320_WriteRegister(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		ili9320_WriteRegister(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		ili9320_WriteRegister(0x0d,0x0000);	//Frame Maker Position.
		ili9320_WriteRegister(0x0f,0x0000);	//Extern Display Interface Contral 2.
	
		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
		ili9320_WriteRegister(0x07,0x0101);	//Display Contral.
		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
	
		ili9320_WriteRegister(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		ili9320_WriteRegister(0x11,0x0007);								//Power Control 2.(0x0001)
		ili9320_WriteRegister(0x12,(1<<8)|(1<<4)|(0<<0));					//Power Control 3.(0x0138)
		ili9320_WriteRegister(0x13,0x0b00);								//Power Control 4.
		ili9320_WriteRegister(0x29,0x0000);								//Power Control 7.
	
		ili9320_WriteRegister(0x2b,(1<<14)|(1<<4));
		
		ili9320_WriteRegister(0x50,0);		//Set X Start.
		ili9320_WriteRegister(0x51,239);	//Set X End.
		ili9320_WriteRegister(0x52,0);		//Set Y Start.
		ili9320_WriteRegister(0x53,319);	//Set Y End.
	
		ili9320_WriteRegister(0x60,0x2700);	//Driver Output Control.
		ili9320_WriteRegister(0x61,0x0001);	//Driver Output Control.
		ili9320_WriteRegister(0x6a,0x0000);	//Vertical Srcoll Control.
	
		ili9320_WriteRegister(0x80,0x0000);	//Display Position? Partial Display 1.
		ili9320_WriteRegister(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		ili9320_WriteRegister(0x82,0x0000);	//RAM Address End-Partial Display 1.
		ili9320_WriteRegister(0x83,0x0000);	//Displsy Position? Partial Display 2.
		ili9320_WriteRegister(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		ili9320_WriteRegister(0x85,0x0000);	//RAM Address End? Partial Display 2.
	
		ili9320_WriteRegister(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		ili9320_WriteRegister(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		ili9320_WriteRegister(0x93,0x0001);	//Panel Interface Contral 3.
		ili9320_WriteRegister(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		ili9320_WriteRegister(0x97,(0<<8));	//
		ili9320_WriteRegister(0x98,0x0000);	//Frame Cycle Contral.

	
		ili9320_WriteRegister(0x07,0x0173);	//(0x0173)
	}
	else if(DeviceCode==0x9919)
	{
		//------POWER ON &RESET DISPLAY OFF
		 ili9320_WriteRegister(0x28,0x0006);
		
		 ili9320_WriteRegister(0x00,0x0001);
		
		 ili9320_WriteRegister(0x10,0x0000);
		
		 ili9320_WriteRegister(0x01,0x72ef);

		 ili9320_WriteRegister(0x02,0x0600);

		 ili9320_WriteRegister(0x03,0x6a38);
		
		 ili9320_WriteRegister(0x11,0x6874);//70
		
		 
		     //  RAM WRITE DATA MASK
		 ili9320_WriteRegister(0x0f,0x0000); 
		    //  RAM WRITE DATA MASK
		 ili9320_WriteRegister(0x0b,0x5308); 
		
		 ili9320_WriteRegister(0x0c,0x0003);
		
		 ili9320_WriteRegister(0x0d,0x000a);
		
		 ili9320_WriteRegister(0x0e,0x2e00);  //0030
		
		 ili9320_WriteRegister(0x1e,0x00be);
		
		 ili9320_WriteRegister(0x25,0x8000);
		
		 ili9320_WriteRegister(0x26,0x7800);
		
		 ili9320_WriteRegister(0x27,0x0078);
		
		 ili9320_WriteRegister(0x4e,0x0000);
		
		 ili9320_WriteRegister(0x4f,0x0000);
		
		 ili9320_WriteRegister(0x12,0x08d9);
		
		 // -----------------Adjust the Gamma Curve----//
		 ili9320_WriteRegister(0x30,0x0000);	 //0007
		
		 ili9320_WriteRegister(0x31,0x0104);	   //0203
		
		 ili9320_WriteRegister(0x32,0x0100);		//0001

		 ili9320_WriteRegister(0x33,0x0305);	  //0007

		 ili9320_WriteRegister(0x34,0x0505);	  //0007
		
		 ili9320_WriteRegister(0x35,0x0305);		 //0407
		
		 ili9320_WriteRegister(0x36,0x0707);		 //0407
		
		 ili9320_WriteRegister(0x37,0x0300);		  //0607
		
		 ili9320_WriteRegister(0x3a,0x1200);		 //0106
		
		 ili9320_WriteRegister(0x3b,0x0800);		 

		 ili9320_WriteRegister(0x07,0x0033);
	} 
/*	else if(DeviceCode==0x9331)
	{
	 	//POWER ON &RESET DISPLAY OFF///////////
		// Start Initial Sequence //////////
		ili9320_WriteRegister(0x00E7, 0x1014);
		ili9320_WriteRegister(0x0001, 0x0100); // set SS and SM bit   0x0100
		ili9320_WriteRegister(0x0002, 0x0200); // set 1 line inversion
		ili9320_WriteRegister(0x0003, 0x1030); // set GRAM write direction and BGR=1.     0x1030
		ili9320_WriteRegister(0x0008, 0x0202); // set the back porch and front porch
		ili9320_WriteRegister(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		ili9320_WriteRegister(0x000A, 0x0000); // FMARK function
		ili9320_WriteRegister(0x000C, 0x0000); // RGB interface setting
		ili9320_WriteRegister(0x000D, 0x0000); // Frame marker Position
		ili9320_WriteRegister(0x000F, 0x0000); // RGB interface polarity
		//Power On sequence 
		ili9320_WriteRegister(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		ili9320_WriteRegister(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		ili9320_WriteRegister(0x0012, 0x0000); // VREG1OUT voltage
		ili9320_WriteRegister(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		ili9320_Delay(200); // Dis-charge capacitor power voltage
		ili9320_WriteRegister(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		ili9320_WriteRegister(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		ili9320_Delay(50); // Delay 50ms
		ili9320_WriteRegister(0x0012, 0x000C); // Internal reference voltage= Vci;
		ili9320_Delay(50); // Delay 50ms
		ili9320_WriteRegister(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		ili9320_WriteRegister(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		ili9320_WriteRegister(0x002B, 0x000B); // Set Frame Rate
		ili9320_Delay(50); // Delay 50ms
		ili9320_WriteRegister(0x0020, 0x0000); // GRAM horizontal Address
		ili9320_WriteRegister(0x0021, 0x0000); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		ili9320_WriteRegister(0x0030, 0x0000);
		ili9320_WriteRegister(0x0031, 0x0106);
		ili9320_WriteRegister(0x0032, 0x0000);
		ili9320_WriteRegister(0x0035, 0x0204);
		ili9320_WriteRegister(0x0036, 0x160A);
		ili9320_WriteRegister(0x0037, 0x0707);
		ili9320_WriteRegister(0x0038, 0x0106);
		ili9320_WriteRegister(0x0039, 0x0707);
		ili9320_WriteRegister(0x003C, 0x0402);
		ili9320_WriteRegister(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		ili9320_WriteRegister(0x0050, 0x0000); // Horizontal GRAM Start Address
		ili9320_WriteRegister(0x0051, 0x00EF); // Horizontal GRAM End Address
		ili9320_WriteRegister(0x0052, 0x0000); // Vertical GRAM Start Address
		ili9320_WriteRegister(0x0053, 0x013F); // Vertical GRAM Start Address
		ili9320_WriteRegister(0x0060, 0x2700); // Gate Scan Line
		ili9320_WriteRegister(0x0061, 0x0001); // NDL,VLE, REV
		ili9320_WriteRegister(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		ili9320_WriteRegister(0x0080, 0x0000);
		ili9320_WriteRegister(0x0081, 0x0000);
		ili9320_WriteRegister(0x0082, 0x0000);
		ili9320_WriteRegister(0x0083, 0x0000);
		ili9320_WriteRegister(0x0084, 0x0000);
		ili9320_WriteRegister(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		ili9320_WriteRegister(0x0090, 0x0010);
		ili9320_WriteRegister(0x0092, 0x0600);
		ili9320_WriteRegister(0x0007,0x0021);		
		ili9320_Delay(50);
		ili9320_WriteRegister(0x0007,0x0061);
		ili9320_Delay(50);
		ili9320_WriteRegister(0x0007,0x0133);  // 262K color and display ON
		ili9320_Delay(50);
	} 								   */
	else if(DeviceCode==0x4531)
	{		
		// Setup display
		ili9320_WriteRegister(0x00,0x0001);
	    ili9320_WriteRegister(0x10,0x0628);
	    ili9320_WriteRegister(0x12,0x0006);
	    ili9320_WriteRegister(0x13,0x0A32);
	    ili9320_WriteRegister(0x11,0x0040);
	    ili9320_WriteRegister(0x15,0x0050);
	    ili9320_WriteRegister(0x12,0x0016);
	    ili9320_Delay(15);
	    ili9320_WriteRegister(0x10,0x5660);
	    ili9320_Delay(15);
	    ili9320_WriteRegister(0x13,0x2A4E);
	    ili9320_WriteRegister(0x01,0x0100);
	    ili9320_WriteRegister(0x02,0x0300);
	
	    ili9320_WriteRegister(0x03,0x1030);
//	    ili9320_WriteRegister(0x03,0x1038);
	
	    ili9320_WriteRegister(0x08,0x0202);
	    ili9320_WriteRegister(0x0A,0x0000);
	    ili9320_WriteRegister(0x30,0x0000);
	    ili9320_WriteRegister(0x31,0x0402);
	    ili9320_WriteRegister(0x32,0x0106);
	    ili9320_WriteRegister(0x33,0x0700);
	    ili9320_WriteRegister(0x34,0x0104);
	    ili9320_WriteRegister(0x35,0x0301);
	    ili9320_WriteRegister(0x36,0x0707);
	    ili9320_WriteRegister(0x37,0x0305);
	    ili9320_WriteRegister(0x38,0x0208);
	    ili9320_WriteRegister(0x39,0x0F0B);
	    ili9320_Delay(15);
	    ili9320_WriteRegister(0x41,0x0002);
	    ili9320_WriteRegister(0x60,0x2700);
	    ili9320_WriteRegister(0x61,0x0001);
	    ili9320_WriteRegister(0x90,0x0119);
	    ili9320_WriteRegister(0x92,0x010A);
	    ili9320_WriteRegister(0x93,0x0004);
	    ili9320_WriteRegister(0xA0,0x0100);
//	    ili9320_WriteRegister(0x07,0x0001);
	    ili9320_Delay(15);
//	    ili9320_WriteRegister(0x07,0x0021); 
	    ili9320_Delay(15);
//	    ili9320_WriteRegister(0x07,0x0023);
	    ili9320_Delay(15);
//	    ili9320_WriteRegister(0x07,0x0033);
	    ili9320_Delay(15);
	    ili9320_WriteRegister(0x07,0x0133);
	    ili9320_Delay(15);
	    ili9320_WriteRegister(0xA0,0x0000);
	    ili9320_Delay(20);
	} 									   
	else if(DeviceCode==0x7783)
	{
		// Start Initial Sequence
		ili9320_WriteRegister(0x00FF,0x0001);
		ili9320_WriteRegister(0x00F3,0x0008);
		ili9320_WriteRegister(0x0001,0x0100);
		ili9320_WriteRegister(0x0002,0x0700);
		ili9320_WriteRegister(0x0003,0x1030);  //0x1030
		ili9320_WriteRegister(0x0008,0x0302);
		ili9320_WriteRegister(0x0008,0x0207);
		ili9320_WriteRegister(0x0009,0x0000);
		ili9320_WriteRegister(0x000A,0x0000);
		ili9320_WriteRegister(0x0010,0x0000);  //0x0790
		ili9320_WriteRegister(0x0011,0x0005);
		ili9320_WriteRegister(0x0012,0x0000);
		ili9320_WriteRegister(0x0013,0x0000);
		ili9320_Delay(50);
		ili9320_WriteRegister(0x0010,0x12B0);
		ili9320_Delay(50);
		ili9320_WriteRegister(0x0011,0x0007);
		ili9320_Delay(50);
		ili9320_WriteRegister(0x0012,0x008B);
		ili9320_Delay(50);
		ili9320_WriteRegister(0x0013,0x1700);
		ili9320_Delay(50);
		ili9320_WriteRegister(0x0029,0x0022);
		
		//################# void Gamma_Set(void) ####################//
		ili9320_WriteRegister(0x0030,0x0000);
		ili9320_WriteRegister(0x0031,0x0707);
		ili9320_WriteRegister(0x0032,0x0505);
		ili9320_WriteRegister(0x0035,0x0107);
		ili9320_WriteRegister(0x0036,0x0008);
		ili9320_WriteRegister(0x0037,0x0000);
		ili9320_WriteRegister(0x0038,0x0202);
		ili9320_WriteRegister(0x0039,0x0106);
		ili9320_WriteRegister(0x003C,0x0202);
		ili9320_WriteRegister(0x003D,0x0408);
		ili9320_Delay(50);
		
		
		ili9320_WriteRegister(0x0050,0x0000);		
		ili9320_WriteRegister(0x0051,0x00EF);		
		ili9320_WriteRegister(0x0052,0x0000);		
		ili9320_WriteRegister(0x0053,0x013F);		
		ili9320_WriteRegister(0x0060,0xA700);		
		ili9320_WriteRegister(0x0061,0x0001);
		ili9320_WriteRegister(0x0090,0x0033);				
		ili9320_WriteRegister(0x002B,0x000B);		
		ili9320_WriteRegister(0x0007,0x0133);
		ili9320_Delay(50);
	}  
	/*
	else if(DeviceCode==0x1505)
	{
// second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        ili9320_WriteRegister(0x0007,0x0000);
        ili9320_Delay(5);
        ili9320_WriteRegister(0x0012,0x011C);//0x011A   why need to set several times?
        ili9320_WriteRegister(0x00A4,0x0001);//NVM
    //
        ili9320_WriteRegister(0x0008,0x000F);
        ili9320_WriteRegister(0x000A,0x0008);
        ili9320_WriteRegister(0x000D,0x0008);
       
  //GAMMA CONTROL/
        ili9320_WriteRegister(0x0030,0x0707);
        ili9320_WriteRegister(0x0031,0x0007); //0x0707
        ili9320_WriteRegister(0x0032,0x0603); 
        ili9320_WriteRegister(0x0033,0x0700); 
        ili9320_WriteRegister(0x0034,0x0202); 
        ili9320_WriteRegister(0x0035,0x0002); //?0x0606
        ili9320_WriteRegister(0x0036,0x1F0F);
        ili9320_WriteRegister(0x0037,0x0707); //0x0f0f  0x0105
        ili9320_WriteRegister(0x0038,0x0000); 
        ili9320_WriteRegister(0x0039,0x0000); 
        ili9320_WriteRegister(0x003A,0x0707); 
        ili9320_WriteRegister(0x003B,0x0000); //0x0303
        ili9320_WriteRegister(0x003C,0x0007); //?0x0707
        ili9320_WriteRegister(0x003D,0x0000); //0x1313//0x1f08
        ili9320_Delay(5);
        ili9320_WriteRegister(0x0007,0x0001);
        ili9320_WriteRegister(0x0017,0x0001);   //Power supply startup enable
        ili9320_Delay(5);
  
  //power control//
        ili9320_WriteRegister(0x0010,0x17A0); 
        ili9320_WriteRegister(0x0011,0x0217); //reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        ili9320_WriteRegister(0x0012,0x011E);//0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        ili9320_WriteRegister(0x0013,0x0F00); //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        ili9320_WriteRegister(0x002A,0x0000);  
        ili9320_WriteRegister(0x0029,0x000A); //0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        ili9320_WriteRegister(0x0012,0x013E); // 0x013C  power supply on
           //Coordinates Control//
        ili9320_WriteRegister(0x0050,0x0000);//0x0e00
        ili9320_WriteRegister(0x0051,0x00EF); 
        ili9320_WriteRegister(0x0052,0x0000); 
        ili9320_WriteRegister(0x0053,0x013F); 
    //Pannel Image Control//
        ili9320_WriteRegister(0x0060,0x2700); 
        ili9320_WriteRegister(0x0061,0x0001); 
        ili9320_WriteRegister(0x006A,0x0000); 
        ili9320_WriteRegister(0x0080,0x0000); 
    //Partial Image Control//
        ili9320_WriteRegister(0x0081,0x0000); 
        ili9320_WriteRegister(0x0082,0x0000); 
        ili9320_WriteRegister(0x0083,0x0000); 
        ili9320_WriteRegister(0x0084,0x0000); 
        ili9320_WriteRegister(0x0085,0x0000); 
  //Panel Interface Control//
        ili9320_WriteRegister(0x0090,0x0013); //0x0010 frenqucy
        ili9320_WriteRegister(0x0092,0x0300); 
        ili9320_WriteRegister(0x0093,0x0005); 
        ili9320_WriteRegister(0x0095,0x0000); 
        ili9320_WriteRegister(0x0097,0x0000); 
        ili9320_WriteRegister(0x0098,0x0000); 
  
        ili9320_WriteRegister(0x0001,0x0100); 
        ili9320_WriteRegister(0x0002,0x0700); 
        ili9320_WriteRegister(0x0003,0x1030); 
        ili9320_WriteRegister(0x0004,0x0000); 
        ili9320_WriteRegister(0x000C,0x0000); 
        ili9320_WriteRegister(0x000F,0x0000); 
        ili9320_WriteRegister(0x0020,0x0000); 
        ili9320_WriteRegister(0x0021,0x0000); 
        ili9320_WriteRegister(0x0007,0x0021); 
        ili9320_Delay(20);
        ili9320_WriteRegister(0x0007,0x0061); 
        ili9320_Delay(20);
        ili9320_WriteRegister(0x0007,0x0173); 
        ili9320_Delay(20);
	}							 			  
	else if(DeviceCode==0x8989)
	{
		ili9320_WriteRegister(0x0000,0x0001);    ili9320_Delay(50000);  //打开晶振
    	ili9320_WriteRegister(0x0003,0xA8A4);    ili9320_Delay(50000);   //0xA8A4
    	ili9320_WriteRegister(0x000C,0x0000);    ili9320_Delay(50000);   
    	ili9320_WriteRegister(0x000D,0x080C);    ili9320_Delay(50000);   
    	ili9320_WriteRegister(0x000E,0x2B00);    ili9320_Delay(50000);   
    	ili9320_WriteRegister(0x001E,0x00B0);    ili9320_Delay(50000);   
    	ili9320_WriteRegister(0x0001,0x2B3F);    ili9320_Delay(50000);   //驱动输出控制320*240  0x6B3F
    	ili9320_WriteRegister(0x0002,0x0600);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0010,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0011,0x6070);    ili9320_Delay(50000);        //0x4030           //定义数据格式  16位色 		横屏 0x6058
    	ili9320_WriteRegister(0x0005,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0006,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0016,0xEF1C);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0017,0x0003);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0007,0x0233);    ili9320_Delay(50000);        //0x0233       
    	ili9320_WriteRegister(0x000B,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x000F,0x0000);    ili9320_Delay(50000);        //扫描开始地址
    	ili9320_WriteRegister(0x0041,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0042,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0048,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0049,0x013F);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x004A,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x004B,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0044,0xEF00);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0045,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0046,0x013F);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0030,0x0707);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0031,0x0204);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0032,0x0204);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0033,0x0502);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0034,0x0507);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0035,0x0204);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0036,0x0204);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0037,0x0502);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x003A,0x0302);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x003B,0x0302);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0023,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0024,0x0000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x0025,0x8000);    ili9320_Delay(50000);
    	ili9320_WriteRegister(0x004f,0);        //行首址0
    	ili9320_WriteRegister(0x004e,0);        //列首址0
	}  									*/
  for(i=50000;i>0;i--);
  ili9320_Clear(0x00f8);
  ili9320_Clear(0xf800);
}

/****************************************************************************
* 名    称：void ili9320_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetCursor(10,10);
****************************************************************************/
__inline void ili9320_SetCursor(u16 x,u16 y)
{
	if(DeviceCode==0x8989)
	{
	 	ili9320_WriteRegister(0x004e,y);        //行
    	ili9320_WriteRegister(0x004f,0x13f-x);  //列
	}
	else if(DeviceCode==0x9919)
	{
		ili9320_WriteRegister(0x004e,x); // 行
  		ili9320_WriteRegister(0x004f,y); // 列	
	}
	else
	{
  		ili9320_WriteRegister(0x0020,y); // 行
  		ili9320_WriteRegister(0x0021,0x13f-x); // 列
	}
}

/****************************************************************************
* 名    称：void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetWindows(0,0,100,100)；
****************************************************************************/
__inline void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_SetCursor(StartX,StartY);
  ili9320_WriteRegister(0x0050, StartX);
  ili9320_WriteRegister(0x0052, StartY);
  ili9320_WriteRegister(0x0051, EndX);
  ili9320_WriteRegister(0x0053, EndY);
}

/****************************************************************************
* 名    称：void ili9320_Clear(u16 dat)
* 功    能：将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* 入口参数：dat      填充值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Clear(0xffff);
****************************************************************************/
void ili9320_Clear(u16 dat)
{
  u32  i;

  ili9320_SetCursor(0x0000, 0x0000);  
  Clr_Cs; 
  ili9320_WriteIndex(0x0022); 
  Set_Rs;   
  for(i=0;i<76800;i++)
  {
    ili9320_WriteData(dat);
	Clr_nWr;
	Set_nWr;
  }

  Set_Cs;
}

/****************************************************************************
* 名    称：u16 ili9320_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=ili9320_GetPoint(10,10);
****************************************************************************/
//u16 ili9320_GetPoint(u16 x,u16 y)
//{
//  ili9320_SetCursor(x,y);
//  return (ili9320_BGR2RGB(ili9320_ReadRegister(0x0022)));
//}


u16 ili9320_GetPoint(u16 x,u16 y)
{
  u16 temp;
  ili9320_SetCursor(x,y);
  Clr_Cs;
  ili9320_WriteIndex(0x0022);
   Clr_nRd;  
	temp = ili9320_ReadData(); //dummy
   Set_nRd;   
   Clr_nRd;  
	temp = ili9320_ReadData(); 
   Set_nRd;	
   Set_Cs;
   if(DeviceCode!=0x7783&&DeviceCode!=0x4531)
   	  temp=ili9320_BGR2RGB(temp);

   return (temp);
}

/****************************************************************************
* 名    称：void ili9320_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  if ( (x>=320)||(y>=240) ) return;
  ili9320_SetCursor(x,y);

  Clr_Cs;
  ili9320_WriteIndex(0x0022);
  Set_Rs;
  ili9320_WriteData(point);
  Clr_nWr;Set_nWr;
  Set_Cs;
}

/****************************************************************************
* 名    称：void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic)
* 功    能：在指定座标范围显示一副图片
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
            pic        图片头指针
* 出口参数：无
* 说    明：图片取模格式为水平扫描，16位颜色模式
* 调用方法：ili9320_DrawPicture(0,0,100,100,(u16*)demo);
****************************************************************************/
void ili9320_DrawPicture(u16 x0, u16 y0,u16 dX,u16 dY,u16 *pic) {
	for(int i = 0, n = 0; i < dX; i++) {
		ili9320_SetCursor(x0+i,y0);
	 Clr_Cs;
	 ili9320_WriteIndex(0x0022);
	 Set_Rs;
	 for(int j = 0; j < dY; j++) {
	  ili9320_WriteData(pic[n++]);
	  Clr_nWr;Set_nWr;
	 }
	 Set_Cs;
	}
}


void ili9320_VLine(u16 x0, u16 y0, u16 h,u16 color) {
 int i;
	ili9320_SetCursor(x0,y0);
 Clr_Cs;
 ili9320_WriteIndex(0x0022);
 Set_Rs;
 for(i = 0; i<h;i++) {
  ili9320_WriteData(color);
  Clr_nWr;Set_nWr;
 }

 Set_Cs;

}

/****************************************************************************
* 名    称：void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( ((tmp_char >> (7-j)) & 0x01) == 0x01)
        {
          ili9320_SetPoint(x+j,y+i,charColor); // 字符颜色
        }
        else
        {
          ili9320_SetPoint(x+j,y+i,bkColor); // 背景颜色
        }
    }
  }
}

/****************************************************************************
* 名    称：void ili9320_Test()
* 功    能：测试液晶屏
* 入口参数：无
* 出口参数：无
* 说    明：显示彩条，测试液晶屏是否正常工作
* 调用方法：ili9320_Test();
****************************************************************************/
void ili9320_Test()
{
  u8  R_data,G_data,B_data,i,j;

	ili9320_SetCursor(0x00, 0x0000);
	ili9320_WriteRegister(0x0050,0x00);//水平 GRAM起始位置
	ili9320_WriteRegister(0x0051,239);//水平GRAM终止位置
	ili9320_WriteRegister(0x0052,0);//垂直GRAM起始位置
	ili9320_WriteRegister(0x0053,319);//垂直GRAM终止位置  
	Clr_Cs; 
	ili9320_WriteIndex(0x0022);
	Set_Rs;
    R_data=0;G_data=0;B_data=0;     
    for(j=0;j<50;j++)//红色渐强条
    {
        for(i=0;i<240;i++)
            {R_data=i/8;ili9320_WriteData(R_data<<11|G_data<<5|B_data);
			Clr_nWr;Set_nWr;}
    }
    R_data=0x1f;G_data=0x3f;B_data=0x1f;
    for(j=0;j<50;j++)
    {
        for(i=0;i<240;i++)
            {
            G_data=0x3f-(i/4);
            B_data=0x1f-(i/8);
            ili9320_WriteData(R_data<<11|G_data<<5|B_data);
			Clr_nWr;
			Set_nWr;
			}
    }
//----------------------------------
    R_data=0;G_data=0;B_data=0;
    for(j=0;j<50;j++)//绿色渐强条
    {
        for(i=0;i<240;i++)
            {G_data=i/4;
			ili9320_WriteData(R_data<<11|G_data<<5|B_data);
			Clr_nWr;
			Set_nWr;}
    }

    R_data=0x1f;G_data=0x3f;B_data=0x1f;
    for(j=0;j<50;j++)
    {
        for(i=0;i<240;i++)
            {
            R_data=0x1f-(i/8);
            B_data=0x1f-(i/8);
            ili9320_WriteData(R_data<<11|G_data<<5|B_data);
			Clr_nWr;
			Set_nWr;
		}
    }
//----------------------------------
 
    R_data=0;G_data=0;B_data=0;
    for(j=0;j<60;j++)//蓝色渐强条
    {
        for(i=0;i<240;i++)
            {B_data=i/8;ili9320_WriteData(R_data<<11|G_data<<5|B_data);
			Clr_nWr;
			Set_nWr;}
    } 

    B_data=0; 
    R_data=0x1f;G_data=0x3f;B_data=0x1f;

    for(j=0;j<60;j++)
    {
        for(i=0;i<240;i++)
            {
            G_data=0x3f-(i/4);
            R_data=0x1f-(i/8);
            ili9320_WriteData(R_data<<11|G_data<<5|B_data);
			Clr_nWr;
			Set_nWr;
		}
    }	  
	Set_Cs;
}

/****************************************************************************
* 名    称：u16 ili9320_BGR2RGB(u16 c)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：c      BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
* 调用方法：
****************************************************************************/
u16 ili9320_BGR2RGB(u16 c)
{
  u16  r, g, b, rgb;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;
  
  rgb =  (b<<11) + (g<<5) + (r<<0);

  return( rgb );
}

/****************************************************************************
* 名    称：void ili9320_WriteIndex(u16 idx)
* 功    能：写 ili9320 控制器寄存器地址
* 入口参数：idx   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
* 调用方法：ili9320_WriteIndex(0x0000);
****************************************************************************/
__inline void ili9320_WriteIndex(u16 idx)
{
    Clr_Rs;
	Set_nRd;
	ili9320_WriteData(idx);
	Clr_nWr;
	Set_nWr;
	Set_Rs;
}

/****************************************************************************
* 名    称：void ili9320_WriteData(u16 dat)
* 功    能：写 ili9320 寄存器数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，调用前需先写寄存器地址，内部函数
* 调用方法：ili9320_WriteData(0x1030)
****************************************************************************/
void ili9320_WriteData(u16 data)
{
	GPIOC->ODR = (GPIOC->ODR&0xff00)|(data&0x00ff);
	GPIOB->ODR = (GPIOB->ODR&0x00ff)|(data&0xff00);
}

/****************************************************************************
* 名    称：u16 ili9320_ReadData(void)
* 功    能：读取控制器数据
* 入口参数：无
* 出口参数：返回读取到的数据
* 说    明：内部函数
* 调用方法：i=ili9320_ReadData();
****************************************************************************/
__inline u16 ili9320_ReadData(void)
{
//========================================================================
// **                                                                    **
// ** nCS       ----\__________________________________________/-------  **
// ** RS        ------\____________/-----------------------------------  **
// ** nRD       -------------------------\_____/---------------------  **
// ** nWR       --------\_______/--------------------------------------  **
// ** DB[0:15]  ---------[index]----------[data]-----------------------  **
// **                                                                    **
//========================================================================
	u16 tmp;
	GPIOB->CRH = (GPIOB->CRH & 0x00000000) | 0x44444444;
	GPIOC->CRL = (GPIOC->CRL & 0x00000000) | 0x44444444;
	tmp = ((GPIOB->IDR&0xff00)|(GPIOC->IDR&0x00ff));
	GPIOB->CRH = (GPIOB->CRH & 0x00000000) | 0x33333333;
	GPIOC->CRL = (GPIOC->CRL & 0x00000000) | 0x33333333;
	return tmp;
}

/****************************************************************************
* 名    称：u16 ili9320_ReadRegister(u16 index)
* 功    能：读取指定地址寄存器的值
* 入口参数：index    寄存器地址
* 出口参数：寄存器值
* 说    明：内部函数
* 调用方法：i=ili9320_ReadRegister(0x0022);
****************************************************************************/
__inline u16 ili9320_ReadRegister(u16 index)
{ 
  	Clr_Cs;
	ili9320_WriteIndex(index);
	Clr_nRd;     
	index = ili9320_ReadData(); 
	Set_nRd;      	
	Set_Cs;
	return index;
}

/****************************************************************************
* 名    称：void ili9320_WriteRegister(u16 index,u16 dat)
* 功    能：写指定地址寄存器的值
* 入口参数：index    寄存器地址
*         ：dat      寄存器值
* 出口参数：无
* 说    明：内部函数
* 调用方法：ili9320_WriteRegister(0x0000,0x0001);
****************************************************************************/
__inline void ili9320_WriteRegister(u16 index,u16 dat)
{
 /************************************************************************
  **                                                                    **
  ** nCS       ----\__________________________________________/-------  **
  ** RS        ------\____________/-----------------------------------  **
  ** nRD       -------------------------------------------------------  **
  ** nWR       --------\_______/--------\_____/-----------------------  **
  ** DB[0:15]  ---------[index]----------[data]-----------------------  **
  **                                                                    **
  ************************************************************************/
//   u16 temp;
    Clr_Cs;
	Clr_Rs;
	Set_nRd;
	ili9320_WriteData(index);
//	temp=(*((volatile unsigned long *) 0x40010C0C));
//	(*((volatile unsigned long *) 0x40010C0C))=(index<<8)|(temp&0x00ff);
//	temp=(*((volatile unsigned long *) 0x4001100C));
//	(*((volatile unsigned long *) 0x4001100C))=(index>>8)|(temp&0xff00);
	Clr_nWr;
	Set_nWr;
	Set_Rs;       
	ili9320_WriteData(dat);  
//	temp=(*((volatile unsigned long *) 0x40010C0C));
//	(*((volatile unsigned long *) 0x40010C0C))=(dat<<8)|(temp&0x00ff);
//	temp=(*((volatile unsigned long *) 0x4001100C));
//	(*((volatile unsigned long *) 0x4001100C))=(dat>>8)|(temp&0xff00);     
	Clr_nWr;
	Set_nWr;
	Set_Cs;
}

/****************************************************************************
* 名    称：void ili9320_Reset()
* 功    能：复位 ili9320 控制器
* 入口参数：无
* 出口参数：无
* 说    明：复位控制器，内部函数
* 调用方法：ili9320_Reset()
****************************************************************************/
void ili9320_Reset()
{
  /***************************************
   **                                   **
   **  -------\______________/-------   **
   **         | <---Tres---> |          **
   **                                   **
   **  Tres: Min.1ms                    **
   ***************************************/
    
//  	Set_Rst;;
//    ili9320_Delay(50000);
//  	Clr_Rst;
//    ili9320_Delay(50000);
//  	Set_Rst;
//    ili9320_Delay(50000);
}

/****************************************************************************
* 名    称：void ili9320_BackLight(u8 status)
* 功    能：开、关液晶背光
* 入口参数：status     1:背光开  0:背光关
* 出口参数：无
* 说    明：
* 调用方法：ili9320_BackLight(1);
****************************************************************************/
void ili9320_BackLight(u8 status)
{
  if ( status >= 1 )
  {
    Lcd_Light_ON;
  }
  else
  {
    Lcd_Light_OFF;
  }
}

/****************************************************************************
* 名    称：void ili9320_Delay(vu32 nCount)
* 功    能：延时
* 入口参数：nCount   延时值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Delay(10000);
****************************************************************************/
void ili9320_Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
