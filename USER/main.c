#include "main.h"
#include "time.h"
#include "adc.h"

_ledFun ledFun;
_Battery battery;
_System system;
_Qc_Detection qc_detection;
_A1_Detection a1_detection;

/**
  * @brief  Configure System_Variable_Init
  * @param  None
  * @retval None
  */
static void System_Variable_Init(void)
{
	system.System_State = System_Run;
	system.Charge_For_Discharge = Discharge_State;
	battery.Batter_Low_Pressure = Batter_Normal;
	battery.Current_Display = 6;

	system.Charge_For_Discharge = Discharge_State;
	qc_detection.Mode = low_speed_mode;
	
}
/**
  * @brief  SClK_Initial() => ��ʼ��ϵͳʱ�ӣ�ϵͳʱ�� = 16MHZ
  * @param  None
  * @retval None 
  */
static void SClK_Initial(void)
{      
	//�����ڲ����پ������޷�Ƶ16MHz
	CLK_ICKR |= 0x01;             //�����ڲ�HSI  
	while(!(CLK_ICKR&0x02));      //HSI׼������
	CLK_SWR = 0xe1;               //HSIΪ��ʱ��Դ
	CLK_CKDIVR = 0x00;            //HSI����Ƶ
}
/**
  * @brief  GPIO_Init
  * @param  None
  * @retval None
  */
void GPIO_Init(void)
{
  PA_DDR |= 0x06;                    //PA1 PA2���ģʽ
	PA_CR1 |= 0x02;                    //�������
	PA_CR2 |= 0x02;                    //����ٶ�10Mhz

  PC_DDR |= 0xD8;                    //PC3 PC4 PC6 PC7���ģʽ
	PC_CR1 |= 0xD8;                    //�������
	PC_CR2 |= 0xD8;                    //����ٶ�10Mhz

  PA_DDR |= 0x10;                    //PD4 ���ģʽ
	PA_CR1 |= 0x10;                    //�������
	PA_CR2 |= 0x10;                    //����ٶ�10Mhz

	PA_CR1 |= 0x27;                    //PA3 PA5��������

	PB_CR1 |= 0x10;                    //PB4 ��������

	PC_CR1 |= 0x20;                    //PC5 ��������
	
	PD_CR1 |= 0x02;                    //PD1 ��������

	Red = 1;
	Grenn = 1;

	CE = 0;
	A_DIR = 1;
	B_EN = 0;
	
}
/**
  * @brief  void ClockConfig(void) => ������������ʱ��
  * @param  None
  * @retval None
  */
void ClockConfig_ON(void)
{
  CLK_PCKENR1 = 0xFF;
  CLK_PCKENR2 = 0xFF;
//  UART_CLK_UART_OFF();            //�ر�UART����ʱ��
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static uint8_t Charge_For_Discharge_Detection(void)
{
	if(a1_detection.Current_charge_state == Charge_normal){
		if(Ready == false){
			system.Charge_For_Discharge = Charge_State;
		}else{
			system.Charge_For_Discharge = Discharge_State;
		}
	}else{//a1_detection.Current_charge_state == Charge_abnormal
		//Forced switch to discharge state.
		system.Charge_For_Discharge = Discharge_State;
	}
	
	if(system.Last_state == system.Charge_For_Discharge){
		return 0;
	}
	system.Last_state = system.Charge_For_Discharge;

	if(system.Charge_For_Discharge == Charge_State){
		A_DIR = false;
		B_EN = true;
		a1_detection.Delay_enable = true;
	}else{//system.Charge_For_Discharge == Charge_State
		CE = false;
		A_DIR = true;
		B_EN = false;
	}
	return 0;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Charge_Query(void)
{
	if(a1_detection.Delay_time_out == true){
		a1_detection.Delay_time_out = false;
		if(a1_detection.ADC_A1_AD_Voltage > (uint16_t)0x41){
			B_EN = true;
			a1_detection.Current_charge_state = Charge_normal;
		}else{
			B_EN = false;
			a1_detection.Current_charge_state = Charge_abnormal;
		}
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Sleep_task(void)
{

}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void main(void)
{
	System_Variable_Init();
	SClK_Initial();
	ClockConfig_ON();
	GPIO_Init();
	ADC_Init();
	Time2_Init();
	asm("rim");                                 //��ȫ���ж� 
	while(1){
	if(system.System_State == System_Run){
			Charge_For_Discharge_Detection();
			Adc_Task();
			Charge_Query();
			Battery_Volume();
		}else{//system.System_State == System_Sleep
			Sleep_task();
		}
	}
}

