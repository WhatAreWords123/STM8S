#include "adc.h"

/**
  * @brief  None
  * @param  None
  * @retval None
  */
void ADC_Init(void)
{
  ADC_CR1 = 0x40;                       //8分频系数   单次转换模式
  ADC_CR2 = 0x30;                       
  ADC_TDRH = 0x01;                      //禁止高位施密特触发功能
  ADC_TDRL = 0x01;                      //禁止低位施密特触发功能
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void ADC_OFF_CMD(void)
{
  ADC_CR1 &= ~0x01;                    //失能ADC
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
uint16_t Read_ADC(uint8_t ch)
{

	uint8_t m,n,i;
  uint8_t  ADC_Num_H = false;
  uint8_t  ADC_Num_L = false;
	uint16_t AD_Date;
	uint16_t AD_Date_Array[6];
	uint16_t AD_Date1, AD_Date2;

  ADC_CSR = 0x00;                       //清除一下通道
  ADC_CSR = ch&0x0F;                    //ADC转换通道
//  ADC_CSR = ch&0x80;                    //清除转换结束标志位

		
	for(m=0; m<=5; m++)
	{
	  ADC_CR1 |= 0x01;                      									//使能ADC并开始转换
	  for(i=0; i<200; i++);                 									//延时等待
	  ADC_CR1 |= 0x01;                      									//使能ADC并开始转换
		while((ADC_CSR & ADC_Over) != ADC_Over);    						//等待ADC转换结束
		ADC_CSR &= ~ADC_Over;                       						//清除转换结束标志位
	  ADC_Num_H = ADC_DRH;
	  ADC_Num_L = ADC_DRL;		
		AD_Date_Array[m] = ((uint16_t)ADC_Num_H << 2) + (uint16_t)ADC_Num_L;
		ADC_CR1 = 0x00;
	}

	for(m=0; m<=4; m++)
	{
		for(n=m+1; n<=5; n++)
		{
			AD_Date1 = AD_Date_Array[m];
			AD_Date2 = AD_Date_Array[n];
			if(AD_Date1 > AD_Date2)
			{
				AD_Date_Array[m] = AD_Date2;
				AD_Date_Array[n] = AD_Date1;
			}
		}
	}
	AD_Date = false;

	for(m=2; m<=3; m++)
	{
		AD_Date += AD_Date_Array[m];
	}
  AD_Date = AD_Date >> 1;
	
  return AD_Date;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Fast_slow_charge_discharge_judge(void)
{
	qc_detection.QC_Gather_finish = true;
	
	if(qc_detection.ADC_QC_Voltage > Speed_Voltage){
		qc_detection.Mode = Speed_mode;
	}
	else if(qc_detection.ADC_QC_Voltage < low_speed_Voltage){
		qc_detection.Mode = low_speed_mode;
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void Adc_Task(void)
{
	static uint8_t Adc_Query = false;
	
  if(system.Flay_Adc_gather == true)
	{
    switch(Adc_Query)
		{
      case 0: battery.Battery_voltage = Read_ADC(ADC_VB); Adc_Query++; break;
			case 1: qc_detection.ADC_QC_Voltage = Read_ADC(ADC_QC); Fast_slow_charge_discharge_judge(); Adc_Query++; break;
			case 2: a1_detection.ADC_A1_AD_Voltage = Read_ADC(ADC_A1_AD); Adc_Query++; break;
			case 3: type_c.ADC_TYPE_C_Voltage = Read_ADC(TYPE_AD); Adc_Query=0; break;
			default:break;
    }
		system.Flay_Adc_gather = false;
  }
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void Battery_Volume(void)
{
	if(system.Charge_For_Discharge == Discharge_State){
		battery.Battery_Compensate = false;
	}
	else{//system.Charge_For_Discharge == Charge_State
		if(qc_detection.Mode == Speed_mode){
			battery.Battery_Compensate = (uint16_t)0x14;//Charge Compensate	0.1V
		}else{//qc_detection.Mode == low_speed_mode
			battery.Battery_Compensate = (uint16_t)0x0A;//Charge Compensate	0.05V
		}
	}
	if((battery.Battery_Level_Update == true)&&(key.key_switch_protection == false)){
		if(battery.Battery_voltage >= (Battery_Level_3 + battery.Battery_Compensate))
			battery.Battery_energy_buf = Quantity_Electricity_100;
		else{
			if(battery.Battery_voltage >= (Battery_Level_2 + battery.Battery_Compensate))
				battery.Battery_energy_buf = Quantity_Electricity_70;
			else{
				if(battery.Battery_voltage >= (Battery_Level_1 + battery.Battery_Compensate))
					battery.Battery_energy_buf = Quantity_Electricity_40;
				else{
					if(battery.Battery_voltage >= (Battery_Level_0 + battery.Battery_Compensate))
						battery.Battery_energy_buf = Quantity_Electricity_5;
					else{
						if(battery.Battery_voltage < Battery_Level_0){
							if(system.Charge_For_Discharge == Discharge_State)
								battery.Batter_Low_Pressure = Batter_Low;
								system.System_State = System_Sleep;
						}
					}
				}
			}
		}
		if(system.Charge_For_Discharge == Charge_State){
			if(battery.Battery_energy_buf >= battery.Current_Display){
				battery.Current_Display = battery.Battery_energy_buf;
				system.NotifyLight_EN = true;
			}
		}
		else{//system.Charge_For_Discharge == Discharge_State
			if(battery.Battery_energy_buf	< battery.Current_Display){
				battery.Current_Display = battery.Battery_energy_buf;
				system.NotifyLight_EN = true;
			}
		}
		battery.Battery_Level_Update = false;
	}
}
