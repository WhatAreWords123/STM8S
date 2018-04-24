#ifndef __CONFIG_H
#define __CONFIG_H
#include "iostm8s003f3.h"


/**
  * @brief  GPIO Definition
  * @param  None
  * @retval None
  */
/*
	Grenn												PA1											out
	CE													PC3											out
	LED													PC4											out
	Red													PC6											out
	B_EN												PC7											out
	A_EN2												PD4											out
	
	A_DIR												PA2											input
	PG													PA5											input
	C_DIR												PB4											input
	Ready												PD1											input
	
	STAT2												PC5											input/ti
	TEST1												PA3											input/ti
	
	QC													PD2											input/adc
	A1_AD												PD3											input/adc
	C_ADC												PD5											input/adc
	VB													PD6											input/adc
*/

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

#define true					1
#define false					0

#define Grenn												PA_ODR_ODR1
#define CE													PC_ODR_ODR3
#define LED													PC_ODR_ODR4
#define Red													PC_ODR_ODR6
#define B_EN												PC_ODR_ODR7
#define A_EN2												PD_ODR_ODR4
	
#define A_DIR												PA_IDR_IDR2
#define PG													PA_IDR_IDR5
#define C_DIR												PB_IDR_IDR4
#define Ready												PD_IDR_IDR1
	
#define STAT2												PC_IDR_IDR5
#define TEST1												PA_IDR_IDR3


#define LED_PERIOD_MAX																			200


typedef struct{
	uint8_t ledPeriod;
	uint8_t ledPlus;
	uint8_t ledModeFlag;
}_ledFun;

extern _ledFun ledFun;

#endif
