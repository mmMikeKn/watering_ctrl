#include <string.h>
#include "global.h"

typedef struct {
	uint16_t x0, y0, dx, dy;
	const uint8_t *pic;
	void (*pf)(void);
} BOTTON;

typedef struct {
	uint16_t x, y;
	char fmt[5];
} MENU_VAL;


volatile CONFIGURE _configure;
extern const unsigned char gImage_imgBug[];
extern const unsigned char gImage_imgArrowDown[];
extern const unsigned char gImage_imgArrowUp[];
extern const unsigned char gImage_imgViewLeft[];
extern const unsigned char gImage_imgViewRight[];
extern const unsigned char gImage_imgStop[];


//--------------------------------------------------
#define BUTTON_NUM 16
static int16_t _menuId = -1, _subMenu = 0;
static int16_t _workingPumpId = -1, _workingPumpTime = 0;
static bool _refreshScr = true;


static MENU_VAL menuPump0[4] = {{8, 1, "%02d"}, {13, 1, "%02d"}, {16, 1, "%02d"}, {21, 1, "%2d"}};
static MENU_VAL menuPump1[4] = {{8, 5, "%02d"}, {13, 5, "%02d"}, {16, 5, "%02d"}, {21, 5, "%2d"}};
static MENU_VAL menuPump2[4] = {{8, 9, "%02d"}, {13, 9, "%02d"}, {16, 9, "%02d"}, {21, 9, "%2d"}};
static MENU_VAL menuTime[6] = {{2, 14, "%02d"},{5, 14, "%02d"},{8, 14, "%02d"},
		                      {11, 14, "%02d"},{14, 14, "%02d"},{30, 14, "%02d"}};
static MENU_VAL *menuList[NUMER_OF_PUMP+1] = { menuPump0, menuPump1, menuPump2, menuTime} ;

static void showBotton(BOTTON *p) {
	if(p->pic != NULL) {
		GUI_Rectangle(p->x0+2, p->y0+2, (p->x0+p->dx)-2, (p->y0+p->dy)-2, Grey, TRUE);
		GUI_Rectangle(p->x0, p->y0, p->x0+2, (p->y0+p->dy), White, TRUE);
		GUI_Rectangle(p->x0, (p->y0+p->dy)-2, (p->x0+p->dx), (p->y0+p->dy),  White, TRUE);
		GUI_Rectangle((p->x0+p->dx)-2, p->y0, (p->x0+p->dx), (p->y0+p->dy)-1, DGrey, TRUE);
		GUI_Rectangle(p->x0+1, p->y0, (p->x0+p->dx)-2, p->y0+2, DGrey, TRUE);
 	uint16_t dx = p->pic[2];
 	uint16_t dy = p->pic[4];
 	uint16_t x = p->x0 + ((p->x0+p->dx) - p->x0 - dx)/2;
 	uint16_t y = p->y0 + ((p->y0+p->dy) - p->y0 - dy)/2;
 	ili9320_DrawPicture(x,y, dx, dy, (u16*)(p->pic+8));
 }
//	else GUI_Rectangle(p->x0, p->y0, (p->x0+p->dx), (p->y0+p->dy), DGrey, FALSE);
}


static int16_t correctVal(int16_t val, int16_t min, int16_t max) {
	if(val < min) val = max;
	if(val > max) val = min;
	return val;
}

static void menuShowPos(bool isOn, int delta) {
	if(_menuId < 0) return;
	if(_menuId < NUMER_OF_PUMP) {
		if(isOn) scr_fontColor(Blue, Yellow);
		else scr_fontColor(Yellow,Blue);
	} else {
		if(isOn) scr_fontColor(Black, White);
		else scr_fontColor(White,Black);
	}
	int16_t val = 0;
	if(_menuId < NUMER_OF_PUMP) {
		if(_subMenu > 3) _subMenu = 0;
		switch(_subMenu) {
		case 0:
			val = correctVal((int16_t)_configure.pump[_menuId].dt_pump + delta, 5, 60);
			_configure.pump[_menuId].dt_pump = (uint16_t)val;
			break;
		case 1:
			val = correctVal((int16_t)_configure.pump[_menuId].pumpStarHour + delta, 0, 23);
			_configure.pump[_menuId].pumpStarHour = (uint16_t)val;
			break;
		case 2:
			val = correctVal((int16_t)_configure.pump[_menuId].pumpStartMinutes + delta, 0, 59);
			_configure.pump[_menuId].pumpStartMinutes = (uint16_t)val;
			break;
		case 3:
			val = correctVal((int16_t)_configure.pump[_menuId].dt_days + delta, 0, 90);
			_configure.pump[_menuId].dt_days = (uint16_t)val;
		}
		if(delta)	storeConfig();
	} else {
	 RTC_t rtc;
	 utime2dt(&rtc, RTC_GetCounter());
	 switch(_subMenu) {
 	 case 0:
 	 	rtc.day = (uint16_t)(val = correctVal((int16_t)rtc.day + delta, 1, 31));
 	 	break;
 	 case 1:
 	 	rtc.mon = (uint16_t)(val = correctVal((int16_t)rtc.mon + delta, 1, 12));
 	 	break;
 	 case 2:
 	 	val = correctVal((int16_t)rtc.year + delta, 00, 37);
 	 	rtc.year = (uint16_t)val;
 	 	break;
 	 case 3:
 	 	rtc.hour = (uint16_t)(val = correctVal((int16_t)rtc.hour + delta, 0, 23));
 	 	break;
 	 case 4:
 	 	rtc.min = (uint16_t)(val = correctVal((int16_t)rtc.min + delta, 0, 59));
 	 	break;
 	 case 5:
 	 	rtc.sec = (uint16_t)(val = correctVal((int16_t)rtc.sec + delta, 0, 59));
 	 	break;
	 }
	 if(delta) {
	 	rtc_set(dt2utime(&rtc));
	 }
	}
	scr_gotoxy(menuList[_menuId][_subMenu].x,menuList[_menuId][_subMenu].y);
	scr_printf(menuList[_menuId][_subMenu].fmt, val);
}

void pumpControl(bool isOn, int indx) {
	if(isOn) {
	 _workingPumpId = indx; _workingPumpTime = 0;
	 _configure.pump[indx].lastTime = RTC_GetCounter();
	 storeLastTime();
	 _refreshScr = TRUE;
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
		switch(indx) {
		case 0:
			GPIO_WriteBit(PUMP0_PORT, PUMP0_PIN, Bit_SET);
			break;
		case 1:
			GPIO_WriteBit(PUMP1_PORT, PUMP1_PIN, Bit_SET);
			break;
		case 2:
			GPIO_WriteBit(PUMP2_PORT, PUMP2_PIN, Bit_SET);
			break;
		}
	} else {
	 _workingPumpId = -1; _workingPumpTime = 0;
	 _refreshScr = TRUE;
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);
		GPIO_WriteBit(PUMP0_PORT, PUMP0_PIN, Bit_RESET);
		GPIO_WriteBit(PUMP1_PORT, PUMP1_PIN, Bit_RESET);
		GPIO_WriteBit(PUMP2_PORT, PUMP2_PIN, Bit_RESET);
	}
}

//----------------
static void b_Enter() {
	if(_workingPumpId < 0)	pumpControl(TRUE, _menuId);
}

static void b_Cancel() {
	pumpControl(FALSE, _workingPumpId);
}

static void b_Right() {
	menuShowPos(FALSE, 0);	_subMenu++;
	if(_menuId == 3 && _subMenu > 5) _subMenu = 0;
	if(_menuId < 3 && _subMenu > 3) _subMenu = 0;
	menuShowPos(TRUE, 0);
}

static void b_Left() {
	menuShowPos(FALSE, 0);_subMenu--;
	if(_subMenu < 0) _subMenu = _menuId == 3? 5:3;
	menuShowPos(TRUE, 0);
}

static void b_Up() {	menuShowPos(TRUE, 1); }
static void b_Down() {	menuShowPos(TRUE, -1); }
static void b_pump0_1() {	menuShowPos(FALSE, 0); _menuId = 0; _subMenu = 0; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump1_1() {	menuShowPos(FALSE, 0); _menuId = 1; _subMenu = 0; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump2_1() {	menuShowPos(FALSE, 0); _menuId = 2; _subMenu = 0; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump0_2() {	menuShowPos(FALSE, 0); _menuId = 0; _subMenu = 1; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump1_2() {	menuShowPos(FALSE, 0); _menuId = 1; _subMenu = 1; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump2_2() {	menuShowPos(FALSE, 0); _menuId = 2; _subMenu = 1; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump0_3() {	menuShowPos(FALSE, 0); _menuId = 0; _subMenu = 3; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump1_3() {	menuShowPos(FALSE, 0); _menuId = 1; _subMenu = 3; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_pump2_3() {	menuShowPos(FALSE, 0); _menuId = 2; _subMenu = 3; _refreshScr = TRUE; menuShowPos(TRUE, 0); }
static void b_time() {	menuShowPos(FALSE, 0); _menuId = 3; _subMenu = 0; menuShowPos(TRUE, 0); }

static BOTTON mainScrBottons[BUTTON_NUM] = {
		{0, 5, 90, 60, NULL, b_pump0_1},
		{95, 5, 55, 60, NULL, b_pump0_2},
		{155, 5, 50, 60, NULL, b_pump0_3},

		{0, 70, 90, 60, NULL, b_pump1_1},
		{95, 70, 55, 60, NULL, b_pump1_2},
		{155, 70, 50, 60, NULL, b_pump1_3},

		{0, 135, 90, 60, NULL, b_pump2_1},
		{95, 135, 55, 60, NULL, b_pump2_2},
		{155, 135, 50, 60, NULL, b_pump2_3},

		{0, 210, 200, 29, NULL, b_time},

		{210, 15, 50, 50, gImage_imgArrowUp, b_Up},
		{265, 15, 50, 50, gImage_imgArrowDown,  b_Down},

		{210, 90, 50, 50, gImage_imgViewLeft, b_Left},
		{265, 90, 50, 50, gImage_imgViewRight, b_Right},

		{210, 165, 50, 50, gImage_imgBug, b_Enter},
		{265, 165, 50, 50, gImage_imgStop, b_Cancel},
};
//----------------

static void showScreen() {
 static uint8_t sec = 0, min = 0;
 RTC_t rtcCur;
 utime2dt(&rtcCur, RTC_GetCounter());

 if(rtcCur.min != min || _refreshScr) {
  min = rtcCur.min;
  win_showMenu(16-5, 3, 22, TEXT_Y_MAX-3);
  scr_setfullTextWindow();
  for(int i = 0; i < NUMER_OF_PUMP; i++) {
   RTC_t rtc;
   if(_configure.pump[i].dt_days == 0) scr_fontColor(Black,Blue);
   else scr_fontColor(Yellow,Blue);
   utime2dt(&rtc,_configure.pump[i].lastTime);

   scr_gotoxy(3,i*4+1); scr_printf("pump:%02d   %02d:%02d  +%2d",
   		_configure.pump[i].dt_pump, _configure.pump[i].pumpStarHour,
   		_configure.pump[i].pumpStartMinutes, _configure.pump[i].dt_days);
   scr_gotoxy(3,i*4+2); scr_printf("Last: %02d.%02d.%02d %02d:%02d", rtc.day, rtc.mon, rtc.year, rtc.hour, rtc.min);
   utime2dt(&rtc, _configure.pump[i].nextTime);
   if(rtcCur.year == rtc.year && rtcCur.mon == rtc.mon && rtcCur.day == rtc.day) {
   	if(rtcCur.hour == rtc.hour)	scr_fontColor(White,Blue);
   	else scr_fontColor(Magenta,Blue);
   }
   scr_gotoxy(3,i*4+3); scr_printf("Next: %02d.%02d.%02d %02d:%02d",	rtc.day, rtc.mon, rtc.year, rtc.hour, rtc.min);
  }
  menuShowPos(TRUE, 0);
 }
 //--------
 if(rtcCur.sec != sec || _refreshScr) {
  sec = rtcCur.sec; scr_setfullTextWindow();
  scr_fontColor(White,Black); scr_gotoxy(2,TEXT_Y_MAX-1);
  scr_printf("%02d.%02d.%02d %02d:%02d:%02d ", rtcCur.day, rtcCur.mon, rtcCur.year, rtcCur.hour, rtcCur.min, rtcCur.sec);
  menuShowPos(TRUE, 0);
  if(_workingPumpId >= 0) {
   if(++_workingPumpTime > _configure.pump[_workingPumpId].dt_pump) {
   	_workingPumpId = -1; _workingPumpTime = 0;
   	pumpControl(FALSE, _workingPumpId);
   	scr_fontColor(Yellow,Blue);
    scr_gotoxy(10,_workingPumpId*4+1); scr_printf("  ");
   } else {
   	scr_fontColor(White,Red);
    scr_gotoxy(10,_workingPumpId*4+1); scr_printf("%2d",	_workingPumpTime);
   }
  } else {
 		uint32_t t = RTC_GetCounter();
  	for(int i = 0; i < NUMER_OF_PUMP; i++)
  		if(t > _configure.pump[i].nextTime &&
  				_configure.pump[i].dt_days != 0 && _configure.pump[i].dt_pump != 0) {
  			pumpControl(TRUE, i);
  			break;
  		}
  }
 }
 //--------
 if(_refreshScr) {
	 for(uint32_t i = 0; i < BUTTON_NUM; i++) {
		 showBotton(&mainScrBottons[i]);
	 }
 }
}

//======================================================================
void delayMs(uint32_t msec)  {
 uint32_t tmp = 7000 * msec  ;
 while( tmp-- ) __NOP();
}


int main() {
	uint8_t flag;
	uint32_t nextTouchTime = 0, timeLcdLightOff = RTC_GetCounter()+50;
	int i;
	SystemStartup();
 restoreConfig();
 _sysTickCounter = 0;
 Lcd_Light_ON;

 while(TRUE) {
 	if(timeLcdLightOff < RTC_GetCounter()) Lcd_Light_OFF;
 	showScreen();
 	_refreshScr = false;

 	if(_sysTickCounter > 0x6FFFFFFF) {
 		nextTouchTime = _sysTickCounter = 0;
 	}
 	if(_sysTickCounter > nextTouchTime) {
 		flag = TRUE;
 		nextTouchTime = _sysTickCounter+100;
  	for(i = 0; i < 200 && flag; i++) {
  		if(!touchFlag()) flag = FALSE;
  	}
  	if(flag) {
  	 Lcd_Light_ON;
  	 timeLcdLightOff = RTC_GetCounter()+30;
  	 int x = touchX(), y = touchY();
	  	for(i = 0; i < BUTTON_NUM; i++) {
	  		BOTTON *p = &mainScrBottons[i];
	  		if(x > p->x0 && x < (p->x0+p->dx) && y > p->y0 && y < (p->y0+p->dy)) {
	  			p->pf(); break;
	  		}
  	 }
  	}
 	}
 }
}


