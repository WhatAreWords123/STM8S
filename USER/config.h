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

#define Grenn												PC_ODR_ODR6
#define CE													PC_ODR_ODR3
#define LED													PC_ODR_ODR4
#define Red													PA_ODR_ODR1
#define B_EN												PC_ODR_ODR7
#define A_EN2												PD_ODR_ODR4
	
#define A_DIR												PA_ODR_ODR2
#define PG													PA_IDR_IDR5
#define C_DIR												PB_IDR_IDR4
#define Ready												PD_IDR_IDR1
	
#define STAT2												PC_IDR_IDR5
#define TEST1												PA_IDR_IDR3

#define ADC_Over                    0x80                  //ADCת������


#define LED_PERIOD_MAX																			250
#define ADC_GATHER_TIME																			1000								//1000 * 50us = 50ms
#define LNDICATOR_LIGHT_CNT																	20000								//20000 * 50us	= 1S
#define MULTIPLE																						5										// 5 * LNDICATOR_LIGHT_CNT = 5S

#define Discharge_State																			true
#define Charge_State																				false

#define Batter_Low																					true
#define Batter_Normal																				false

#define System_Sleep                                        false
#define System_Run                                          true

#define ADC_VB                    													0x06                  //ADC2ͨ��

#if 0
#define Battery_Level_0                                     (uint16_t)0x15F          //1.714V
#define Battery_Level_1																			(uint16_t)0x163          //1.734V
#define Battery_Level_2                                     (uint16_t)0x16A          //1.771V
#define Battery_Level_3                                     (uint16_t)0x182          //1.885V
#define Battery_Level_4                                     (uint16_t)0x1A8          //2.070V
#define Battery_Level_5                                     (uint16_t)0x1E8          //2.400V
#else
#define Battery_Level_0                                     (uint16_t)0x155          //1.666V
#define Battery_Level_1																			(uint16_t)0x189          //1.920V
#define Battery_Level_2                                     (uint16_t)0x1A7          //2.070V
#define Battery_Level_3                                     (uint16_t)0x1B8          //2.15V
#endif
#define Quantity_Electricity_100														5
#define Quantity_Electricity_70															4
#define Quantity_Electricity_40															3
#define Quantity_Electricity_10															2
#define Quantity_Electricity_5															1




typedef struct{
	uint8_t ledPeriod;
	uint8_t ledPlus;
	uint8_t ledModeFlag;
	uint8_t ledCnt;
	uint16_t ledBlinktime;
	uint16_t ledExtinguish;
	uint8_t ledFlag;
}_ledFun;

typedef struct{
	uint8_t Battery_energy_buf;
	uint8_t Batter_Low_Pressure;
	uint8_t Current_Display;
	uint8_t Battery_Level_Update;
	uint16_t Battery_Compensate;
	uint16_t Battery_voltage;
}_Battery;

typedef struct{
	uint8_t System_State;
	uint8_t NotifyLight_EN;
	uint8_t Charge_For_Discharge;
	uint8_t Flay_Adc_gather;
	uint8_t Lndicator_light_cnt_multiple;
	uint16_t Adc_gather_cnt;
	uint16_t Lndicator_light_cnt;
}_System;

extern _ledFun ledFun;
extern _Battery battery;
extern _System system;

#endif
