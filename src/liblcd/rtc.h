#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t year;	/* 1..4095 */
	uint8_t  mon;	/* 1..12 */
	uint8_t  day;	/* 1..31 */
	uint8_t  hour;	/* 0..23 */
	uint8_t  min;	/* 0..59 */
	uint8_t  sec;	/* 0..59 */
} RTC_t;

int rtc_init(void);
uint32_t dt2utime(RTC_t * tm);
void  utime2dt(RTC_t * dts, uint32_t tim);
void rtc_set(uint32_t cnt);

#endif
