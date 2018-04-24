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
static void timeIsr(void){
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
		Grenn = 0;
		Red = 0;
	 }else{
		Grenn = 1;
		Red = 1;
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

	timeIsr();
	
	TIM2_SR1 = 0x00;         																		//清除更新时间标志位
}