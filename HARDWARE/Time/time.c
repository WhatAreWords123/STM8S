#include "time.h"

/**
  * @brief  TIM2定时器    	时钟源16M   16分频    1M时钟   1us
  * @brief  定时器初值    	65536 - 50  = 0xFFCD
  * @brief  定时时间      	1MS
  * @brief  自动重装载值   	0xFFCD
  */

#define TIME_CNTRH														0xFF
#define TIME_CNTRL														0xCD
void Time2_Init(void)
{
  TIM2_IER |= 0x01;         																//使能更新中断
  TIM2_EGR |= 0x01;         																//产生更新事件
  TIM2_CNTRH = TIME_CNTRH;        													//计数器值   
  TIM2_CNTRL = TIME_CNTRL;        													//计数器值 
  TIM2_PSCR = 0x04;         																//分频值   2^4 = 16分频  
  TIM2_CR1 |= 0x01;         																//使能计数器
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void Tim2_DeInit(void)
{
  TIM2_CR1 &= 0xFE;                                          //失能Tim2计数器
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void PWM_indicator(void)
{
	if(++ledFun.ledPeriod >= LED_PERIOD_MAX){	//整个周期结束
		ledFun.ledPeriod = 0;
		if(ledFun.ledModeFlag == 1){	//渐灭
			if(ledFun.ledPlus){	//占空比越来越小
				--ledFun.ledPlus;
				if(!ledFun.ledPlus){
					ledFun.ledModeFlag = 0;
				}
			}
		}else{//渐亮
			if(++ledFun.ledPlus == LED_PERIOD_MAX){	//占空比越来越大
				ledFun.ledModeFlag = 1;
			}
		}
	}
	if(ledFun.ledPeriod < ledFun.ledPlus){	//1输出占空比
		switch(battery.Current_Display){
			case Quantity_Electricity_100: Grenn = 0; Red = 1; break;
			case Quantity_Electricity_70:  Grenn = 0; Red = 0; break;
			case Quantity_Electricity_40:  Grenn = 1; Red = 0; break;
			default: break;
		}
	}else{
		Grenn = 1;
		Red = 1;
	}
}
static void LED_indicator(void)
{
	ledFun.ledPeriod = false;
	ledFun.ledPlus = false;
	ledFun.ledModeFlag = false;
	switch(battery.Current_Display){
		case Quantity_Electricity_100: Grenn = 0; Red = 1; break;
		case Quantity_Electricity_70:  Grenn = 0; Red = 0; break;
		case Quantity_Electricity_40:  Grenn = 1; Red = 0; break;
		default: break;
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Low_electrical_warning(void)
{
	ledFun.ledPeriod = false;
	ledFun.ledPlus = false;
	ledFun.ledModeFlag = false;
	Grenn = 1;

	if(!ledFun.ledFlag){
		if(ledFun.ledCnt < 4){
			if(++ledFun.ledBlinktime >= 2000){
				ledFun.ledBlinktime = false;
				ledFun.ledCnt++;
				Red = !Red;
				}
		}
		else{
			ledFun.ledCnt = false;
			ledFun.ledFlag = true;
		}
	}else{
		if(++ledFun.ledExtinguish >= 8000){
			ledFun.ledExtinguish = false;
			ledFun.ledFlag = false;
		}else{
			Red = 1;
		}
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Charge_indicator_quickness(void)
{
	ledFun.Charge_Slow_ledBlinktime = false;
	if(++ledFun.Charge_quickness_ledBlinktime >= 5000){
		ledFun.Charge_quickness_ledBlinktime = false;
		switch(battery.Current_Display){
			case Quantity_Electricity_100: Grenn = !Grenn; Red = 1; break;
			case Quantity_Electricity_70:  Grenn = !Grenn; Red = !Red; break;
			case Quantity_Electricity_10:
			case Quantity_Electricity_40:
				Grenn = 1;
				Red = !Red;
				break;
			default: break;
		}		
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Charge_indicator_Slow(void)
{
	ledFun.Charge_quickness_ledBlinktime = false;
	if(++ledFun.Charge_Slow_ledBlinktime >= 10000){
		ledFun.Charge_Slow_ledBlinktime = false;
		switch(battery.Current_Display){
			case Quantity_Electricity_100: Grenn = !Grenn; Red = 1; break;
			case Quantity_Electricity_70:  Grenn = !Grenn; Red = !Red; break;
			case Quantity_Electricity_10:
			case Quantity_Electricity_40:
				Grenn = 1;
				Red = !Red;
				break;
			default: break;
		}		
	}	
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void timeIsr(void){
	if(system.NotifyLight_EN == true){
		if(system.Charge_For_Discharge == Discharge_State){
			if(battery.Current_Display > Quantity_Electricity_10){
				if(qc_detection.Mode == Speed_mode){
					PWM_indicator();
				}else{//qc_detection.Mode == low_speed_mode
					LED_indicator();
				}
			}else{//battery.Current_Display <= Quantity_Electricity_10
				Low_electrical_warning();
			}
		}else{//system.Charge_For_Discharge == Charge_State
			ledFun.ledPeriod = false;
			ledFun.ledPlus = false;
			ledFun.ledModeFlag = false;
			if(battery.Battery_State == Battery_Charge){
				if(qc_detection.Mode == Speed_mode){
					Charge_indicator_quickness();
				}else{//qc_detection.Mode == low_speed_mode
					Charge_indicator_Slow();
				}
			}else{//battery.Battery_State == Battery_Full
				Grenn = 0;
				Red = 1;
			}
		}
	}
}
/**
  * @brief  TIM2更新时间中断    中断向量  0x0F
  * @param  None
  * @retval None	
  */
#pragma vector=TIM2_OVR_UIF_vector//0x0F
__interrupt void Time2_OVR_IRQHandler(void)
{
  
  TIM2_CNTRH = TIME_CNTRH;       															//计数器值   
  TIM2_CNTRL = TIME_CNTRL;       															//计数器值
  if(system.System_State == System_Run){
	  if(!system.Flay_Adc_gather){
		  if(++system.Adc_gather_cnt >= ADC_GATHER_TIME){
				system.Adc_gather_cnt = false;
				system.Flay_Adc_gather = true;
		  }
	  }

		if(!battery.Battery_Level_Update){
			if(++system.Lndicator_light_cnt >= LNDICATOR_LIGHT_CNT){
				system.Lndicator_light_cnt = false;
				if(++system.Lndicator_light_cnt_multiple >= MULTIPLE){
					system.Lndicator_light_cnt_multiple = false;
					battery.Battery_Level_Update = true;
				}
			}
		}

		if(a1_detection.Delay_enable == true){
			if(++a1_detection.Delay_enable_cnt >= LNDICATOR_LIGHT_CNT){
				a1_detection.Delay_enable_cnt = false;
				if(++a1_detection.Delay_enable_cnt_multiple >= 2){
					a1_detection.Delay_enable_cnt_multiple = false;
					a1_detection.Delay_enable = false;
					a1_detection.Delay_time_out = true;
				}
			}
		}

		if(battery.Battery_full_time_out == true){
			if(++battery.Battery_Full_cnt >= LNDICATOR_LIGHT_CNT){
				battery.Battery_Full_cnt = false;
				if(++battery.Battery_Full_cnt_multiple >= 12){
					battery.Battery_Full_cnt_multiple = false;
					battery.Battery_full_locking = true;
					battery.Battery_full_time_out = false;
				}
			}			
		}

		if(system.System_sleep_countdown == true){
			if(++system.System_sleep_countdown_cnt >= LNDICATOR_LIGHT_CNT){
				system.System_sleep_countdown_cnt = false;
				if(++system.System_sleep_countdown_cnt_multiple >= 15){
					system.System_sleep_countdown_cnt_multiple = false;
					system.System_State = System_Sleep;
				}
			}			
		}
#if test
		if(++key.Key_Time_cnt >= 200){
			key.Key_Time_cnt = false;
			key.time_10ms_ok = true;
		}
#endif
		timeIsr();
  }else{//system.System_State == System_Sleep
  }
	TIM2_SR1 = 0x00;         																		//清除更新时间标志位
}
