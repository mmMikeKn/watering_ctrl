/*
 * global.h
 *
 *  Created on: 14.04.2011
 *      Author: mm
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_rtc.h"
#include "hw_config.h"
#include "ili9320.h"
#include "scr_io.h"
#include "rtc.h"

#define NUMER_OF_PUMP 3

typedef struct {
	uint32_t pumpStarHour, pumpStartMinutes; // starting time
	int32_t dt_days, dt_pump;
 uint32_t nextTime, lastTime;
} PUMP_CONFIG; // unit16_t*n.. size

typedef struct {
	PUMP_CONFIG pump[NUMER_OF_PUMP];
} CONFIGURE;

extern volatile CONFIGURE _configure;

void delayMs(uint32_t msec);
extern uint32_t _sysTickCounter;

void setTime(void);
void restoreConfig(void);
void storeConfig(void);
void storeLastTime(void);
void pumpControl(bool isOn, int idx);

void touchInit();
uint16_t touchX();
uint16_t touchY();
uint8_t touchFlag();


// free PA8, PA11, PB1
#define PUMP0_PIN GPIO_Pin_1
#define PUMP0_PORT GPIOB

#define PUMP1_PIN GPIO_Pin_8
#define PUMP1_PORT GPIOA

#define PUMP2_PIN GPIO_Pin_11
#define PUMP2_PORT GPIOA


#endif /* GLOBAL_H_ */
