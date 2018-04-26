#include "time.h"

/**
  * @brief  TIM2��ʱ��    	ʱ��Դ16M   16��Ƶ    1Mʱ��   1us
  * @brief  ��ʱ����ֵ    	65536 - 50  = 0xFFCD
  * @brief  ��ʱʱ��      	1MS
  * @brief  �Զ���װ��ֵ   	0xFFCD
  */

#define TIME_CNTRH														0xFF
#define TIME_CNTRL														0xCD
void Time2_Init(void)
{
  TIM2_IER |= 0x01;         																//ʹ�ܸ����ж�
  TIM2_EGR |= 0x01;         																//���������¼�
  TIM2_CNTRH = TIME_CNTRH;        													//������ֵ   
  TIM2_CNTRL = TIME_CNTRL;        													//������ֵ 
  TIM2_PSCR = 0x04;         																//��Ƶֵ   2^4 = 16��Ƶ  
  TIM2_CR1 |= 0x01;         																//ʹ�ܼ�����
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void timeIsr(void){
	if(system.NotifyLight_EN == true){
		if(battery.Current_Display > Quantity_Electricity_10){
			if(++ledFun.ledPeriod >= LED_PERIOD_MAX){	//�������ڽ���
				ledFun.ledPeriod = 0;
				if(ledFun.ledModeFlag == 1){	//����
					if(ledFun.ledPlus){	//ռ�ձ�Խ��ԽС
						--ledFun.ledPlus;
						if(!ledFun.ledPlus){
							ledFun.ledModeFlag = 0;
						}
					}
				}else{//����
					if(++ledFun.ledPlus == LED_PERIOD_MAX){	//ռ�ձ�Խ��Խ��
						ledFun.ledModeFlag = 1;
					}
				}
			}
			 if(ledFun.ledPeriod < ledFun.ledPlus){	//1���ռ�ձ�
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
		else{		//battery.Current_Display <= Quantity_Electricity_10
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
	}
}
/**
  * @brief  TIM2����ʱ���ж�    �ж�����  0x0F
  * @param  None
  * @retval None	
  */
#pragma vector=TIM2_OVR_UIF_vector//0x0F
__interrupt void Time2_OVR_IRQHandler(void)
{
  
  TIM2_CNTRH = TIME_CNTRH;       															//������ֵ   
  TIM2_CNTRL = TIME_CNTRL;       															//������ֵ
  
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
	timeIsr();
	
	TIM2_SR1 = 0x00;         																		//�������ʱ���־λ
}
