#include <string.h>
#include <stdlib.h>
#include <stm32f10x_bkp.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_flash.h>
#include "global.h"

#define FLASH_START_ADDR       0x08000000UL
#define FLASH_PAGE_SIZE        1024
#define FLASH_PAGES            128
#define CONFIG_START_ADDR      (FLASH_START_ADDR + ((FLASH_PAGES - 1) * FLASH_PAGE_SIZE))


static void recalcConfig(void) {
 RTC_t rtc;
 for(int i = 0; i < NUMER_OF_PUMP; i++) {
 	uint32_t t = _configure.pump[i].lastTime;
 	if(t > RTC_GetCounter()) _configure.pump[i].lastTime = t = RTC_GetCounter();
 	t +=(uint32_t)_configure.pump[i].dt_days*60*60*24;
 	utime2dt(&rtc, t);
  rtc.hour =_configure.pump[i].pumpStarHour;
  rtc.min = _configure.pump[i].pumpStartMinutes;
  _configure.pump[i].nextTime = dt2utime(&rtc);
 }
}

#define MAGIC_WORD 0x783D

void restoreConfig(void) {
 RTC_t rtc;
	uint32_t *src_addr = (void *)CONFIG_START_ADDR;

 if(*src_addr != MAGIC_WORD) {
  for(int i = 0; i < NUMER_OF_PUMP; i++) {
  	_configure.pump[i].dt_days=7;
  	_configure.pump[i].dt_pump=10;
  	_configure.pump[i].pumpStarHour = 8;
  	_configure.pump[i].pumpStartMinutes = 00;
  	memset(&rtc, 0, sizeof(rtc));
  	rtc.year = 2014;
  	rtc.mon = 2;
  	rtc.day = 10;
  	_configure.pump[i].lastTime = dt2utime(&rtc);
  }
 } else {
 	src_addr++;
 	uint32_t *dest_addr = (void *)&_configure;
  for(size_t i=0; i< sizeof(CONFIGURE); i++) {
  	*dest_addr = *src_addr;	src_addr++;	dest_addr++;
  }
 }
 uint32_t v = BKP_ReadBackupRegister(BKP_DR2);
 if(v == MAGIC_WORD) {
 	v = BKP_ReadBackupRegister(BKP_DR3);
 	_configure.pump[0].lastTime = (uint32_t)v << 16 | BKP_ReadBackupRegister(BKP_DR4);
 	//-------
 	v = BKP_ReadBackupRegister(BKP_DR5);
 	_configure.pump[1].lastTime = (uint32_t)v << 16 | BKP_ReadBackupRegister(BKP_DR6);
 	//-------
 	v = BKP_ReadBackupRegister(BKP_DR7);
 	_configure.pump[2].lastTime = (uint32_t)v << 16 | BKP_ReadBackupRegister(BKP_DR8);
 }
 recalcConfig();
}

void storeLastTime(void) {
	recalcConfig();
 PWR_BackupAccessCmd(ENABLE);
 BKP_WriteBackupRegister(BKP_DR2, MAGIC_WORD);
//----
 BKP_WriteBackupRegister(BKP_DR3, (uint16_t)(_configure.pump[0].lastTime >> 16));
 BKP_WriteBackupRegister(BKP_DR4, (uint16_t)(_configure.pump[0].lastTime));
 //----
 BKP_WriteBackupRegister(BKP_DR5, (uint16_t)(_configure.pump[1].lastTime >> 16));
 BKP_WriteBackupRegister(BKP_DR6, (uint16_t)(_configure.pump[1].lastTime));
 //----
 BKP_WriteBackupRegister(BKP_DR7, (uint16_t)(_configure.pump[2].lastTime >> 16));
 BKP_WriteBackupRegister(BKP_DR8, (uint16_t)(_configure.pump[2].lastTime));
 PWR_BackupAccessCmd(DISABLE);
}


void storeConfig(void) {
 storeLastTime();
	FLASH_Unlock();
 FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR |  FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(CONFIG_START_ADDR);
	uint32_t *src_addr = (void *)&_configure;
	uint32_t *dest_addr = (void *)CONFIG_START_ADDR;
	FLASH_ProgramWord((uint32_t)dest_addr, MAGIC_WORD); dest_addr++;
 for(size_t i=0; i< sizeof(CONFIGURE); i++) {
 	FLASH_ProgramWord((uint32_t)dest_addr, *src_addr);
 	src_addr++;	dest_addr++;
 }
 FLASH_Lock();
}

