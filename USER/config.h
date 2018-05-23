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
	
	QC													PD2											input/adc/AIN3
	A1_AD												PD3											input/adc/AIN4
	C_ADC												PD5											input/adc/AIN5
	VB													PD6											input/adc/AIN6
*/

/*!< Signed integer types  */
typedef   signed char     int8_t;
typedef   signed int    	int16_t;
typedef   signed long     int32_t;

/*!< Unsigned integer types  */
typedef unsigned char     uint8_t;
typedef unsigned int    	uint16_t;
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
#define PG													PB_IDR_IDR5
#define C_DIR												PB_IDR_IDR4
#define Ready												PD_IDR_IDR1
	
#define STAT2												PC_IDR_IDR5
#define TEST1												PA_IDR_IDR3

#define ADC_Over                    0x80                  //ADC转换结束


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

#define ADC_VB                    													0x06                  //ADC2通道
#define ADC_QC																							0x03									//ADC3通道
#define ADC_A1_AD																						0x04									//ADC4通道
#define TYPE_AD																							0x05									//ADC5通道
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


#define Speed_Voltage																				(uint16_t)0x147					//1.6V
#define low_speed_Voltage																		(uint16_t)0x133					//1.5V
#define Overload_event																			(uint16_t)0x66					//0.5V
#define Idle_Voltage																				(uint16_t)0x0A					//0.05V
#define TYPE_C_VOLTAGE																			(uint16_t)0x02					//0.01V

#define Speed_mode																					true
#define low_speed_mode																			false

#define Battery_Full																				true
#define Battery_Charge																			false

#define Charge_normal																				false
#define Charge_abnormal																			true

typedef struct{
	uint8_t ledPeriod;
	uint8_t ledPlus;
	uint8_t ledModeFlag;
	uint8_t ledCnt;
	uint8_t ledFlag;
	uint16_t ledBlinktime;
	uint16_t ledExtinguish;
	uint16_t Charge_Slow_ledBlinktime;
	uint16_t Charge_quickness_ledBlinktime;
}_ledFun;

typedef struct{
	uint8_t Delay_Detection_Battery_full_status;
	uint8_t Battery_full_time_out;
	uint8_t Battery_full_locking;
	uint8_t Battery_State;
	uint8_t Battery_Full_cnt_multiple;
	uint16_t Battery_Full_cnt;
	uint8_t Battery_energy_buf;
	uint8_t Batter_Low_Pressure;
	uint8_t Current_Display;
	uint8_t Battery_Level_Update;
	uint16_t Battery_Compensate;
	uint16_t Battery_voltage;
}_Battery;

typedef struct{
	uint8_t System_State;
	uint8_t Led_last_state;
	uint8_t Led_current_status;
	uint8_t System_sleep_countdown;
	uint8_t Overload_cnt;
	uint8_t NotifyLight_EN;
	uint16_t System_sleep_countdown_cnt;
	uint8_t System_sleep_countdown_cnt_multiple;
	uint8_t Charge_For_Discharge;
	uint8_t Micro_charge_enable_for_disable;
	uint8_t Flay_Adc_gather;
	uint8_t Lndicator_light_cnt_multiple;
	uint16_t Adc_gather_cnt;
	uint16_t Lndicator_light_cnt;
}_System;

typedef struct{
	uint8_t Mode;
	uint8_t QC_Gather_finish;
	uint16_t ADC_QC_Voltage;
}_Qc_Detection;

typedef struct{
	uint8_t Delay_Detection_status;
	uint8_t Current_charge_state;
	uint8_t Delay_enable;
	uint8_t Delay_time_out;
	uint8_t Delay_enable_cnt_multiple;
	uint16_t Delay_enable_cnt;
	uint16_t ADC_A1_AD_Voltage;
}_A1_Detection;

typedef struct{
	uint8_t key_switch_protection;
	uint8_t key_delay_switch_en;
	uint8_t Key_Dlay_Enable;
	uint8_t Key_Time_cnt;
	uint8_t time_10ms_ok;
	uint8_t key;
	uint16_t key_Delay_enable_cnt;
	uint8_t key_Delay_enable_cnt_multiple;
}_KEY;

typedef struct{
	uint16_t ADC_TYPE_C_Voltage;
}_TYPE_C;

extern _KEY key;
extern _ledFun ledFun;
extern _Battery battery;
extern _System system;
extern _TYPE_C type_c;
extern _Qc_Detection qc_detection;
extern _A1_Detection a1_detection;
#endif
