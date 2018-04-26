#include "adc.h"

/**
  * @brief  None
  * @param  None
  * @retval None
  */
void ADC_Init(void)
{
  ADC_CR1 = 0x40;                       //8��Ƶϵ��   ����ת��ģʽ
  ADC_CR2 = 0x30;                       
  ADC_TDRH = 0x01;                      //��ֹ��λʩ���ش�������
  ADC_TDRL = 0x01;                      //��ֹ��λʩ���ش�������
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void ADC_OFF_CMD(void)
{
  ADC_CR1 &= ~0x01;                    //ʧ��ADC
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

  ADC_CSR = 0x00;                       //���һ��ͨ��
  ADC_CSR = ch&0x0F;                    //ADCת��ͨ��
//  ADC_CSR = ch&0x80;                    //���ת��������־λ

		
	for(m=0; m<=5; m++)
	{
	  ADC_CR1 |= 0x01;                      									//ʹ��ADC����ʼת��
	  for(i=0; i<200; i++);                 									//��ʱ�ȴ�
	  ADC_CR1 |= 0x01;                      									//ʹ��ADC����ʼת��
		while((ADC_CSR & ADC_Over) != ADC_Over);    						//�ȴ�ADCת������
		ADC_CSR &= ~ADC_Over;                       						//���ת��������־λ
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
void Adc_Task(void)
{
	static uint8_t Adc_Query = false;
	
  if(system.Flay_Adc_gather == true)
	{
    switch(Adc_Query)
		{
      case 0: battery.Battery_voltage = Read_ADC(ADC_VB); Adc_Query=0; break;
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
	if(system.Charge_For_Discharge == Discharge_State)
		battery.Battery_Compensate = false;
	else//system.Charge_For_Discharge == Charge_State
		battery.Battery_Compensate = false;
	
	if(battery.Battery_Level_Update == true){
		if(battery.Battery_voltage >= (Battery_Level_3 + battery.Battery_Compensate))
			battery.Battery_energy_buf = Quantity_Electricity_100;
		else
		{
			if(battery.Battery_voltage >= (Battery_Level_2 + battery.Battery_Compensate))
				battery.Battery_energy_buf = Quantity_Electricity_70;
			else
			{
				if(battery.Battery_voltage >= (Battery_Level_1 + battery.Battery_Compensate))
					battery.Battery_energy_buf = Quantity_Electricity_40;
				else
				{
					if(battery.Battery_voltage >= (Battery_Level_0 + battery.Battery_Compensate))
						battery.Battery_energy_buf = Quantity_Electricity_5;
					else
					{
						if(battery.Battery_voltage < Battery_Level_0)
						{
							if(system.Charge_For_Discharge == Discharge_State)
								battery.Batter_Low_Pressure = Batter_Low;
						}
					}
				}
			}
		}
		
		if(system.Charge_For_Discharge == Charge_State)
		{
			if(battery.Battery_energy_buf >= battery.Current_Display)
				battery.Current_Display = battery.Battery_energy_buf;
		}
		else//system.Charge_For_Discharge == Discharge_State
		{
			if(battery.Battery_energy_buf	< battery.Current_Display)
			{
				battery.Current_Display = battery.Battery_energy_buf;
				system.NotifyLight_EN = true;
			}
		}
		battery.Battery_Level_Update = false;
	}
}