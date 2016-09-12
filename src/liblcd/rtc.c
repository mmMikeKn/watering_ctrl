#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_bkp.h>
#include <stm32f10x_rtc.h>
#include <stm32f10x_pwr.h>
#include <string.h>

#include "rtc.h"

#define IS_LEAP(year) ( ((year)%4 == 0) )

uint32_t dt2utime(RTC_t * tm) {
	int  year, msize;
 uint32_t  tim;
 uint16_t fsize[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

 if ((year=tm->year) < 70)  year+=100;

 msize=fsize[tm->mon-1];
 if (IS_LEAP(tm->year)) msize= (tm->mon > 2) ? msize+1: msize;
   /* count number of passed days */
 tim=(year-70)*365L + (year-69)/4 + msize + tm->day-1;
   /* count number of seconds */
 tim=((tim*24 + tm->hour)*60 + tm->min)*60 + tm->sec;

 return tim;
}

void  utime2dt(RTC_t * dts, uint32_t tim) {
	int ysize;
	uint8_t msize[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	dts->sec  = (uint8_t)(tim % 60); tim /= 60;
 dts->min  = (uint8_t)(tim % 60); tim /= 60;
 dts->hour = (uint8_t)(tim % 24); tim /= 24;

    /* count number of years passed (base is 00)*/
 for(dts->year=70; tim >= (ysize=IS_LEAP(dts->year)?366:365);
                                         dts->year++, tim-=ysize );

 msize[1] = IS_LEAP(dts->year) ? 29 : 28;

 /* count number of month passed */
 for (dts->mon=0; tim >= msize[dts->mon]; tim-=msize[dts->mon++]);

 dts->mon++;
 dts->day=(uint8_t)(tim+1);
 if (dts->year >= 100)   dts->year-=100;
 return;
}

void rtc_set(uint32_t cnt) {
	PWR_BackupAccessCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_SetCounter(cnt);
	RTC_WaitForLastTask();
	PWR_BackupAccessCmd(DISABLE);
}

int rtc_init(void) {
	volatile uint16_t i;
	RTC_t dt;

	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* LSI clock stabilization time */
	for(i=0;i<5000;i++) { ; }

	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A8) {
		/* Backup data register value is not correct or not yet programmed (when
		   the first time the program is executed) */

		/* Allow access to BKP Domain */
		PWR_BackupAccessCmd(ENABLE);

		/* Reset Backup Domain */
		BKP_DeInit();

		/* Enable LSE */
		RCC_LSEConfig(RCC_LSE_ON);

		/* Wait till LSE is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) { ; }

		/* Select LSE as RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Set RTC prescaler: set RTC period to 1sec */
		RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		/* Set initial value */

		dt.year = 14;
		dt.mon = 2;
		dt.day = 14;
		dt.hour = 9;
		dt.min = 0;
		RTC_SetCounter( dt2utime(&dt) );

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A8);

		/* Lock access to BKP Domain */
		PWR_BackupAccessCmd(DISABLE);

	} else {

		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

	}
	return 0;
}



