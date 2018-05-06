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
	battery.Battery_State = Battery_Charge;
	a1_detection.Delay_Detection_status = false;
	battery.Battery_full_time_out = false;
	battery.Battery_full_locking = false;
	battery.Delay_Detection_Battery_full_status = false;

	a1_detection.Current_charge_state = Charge_normal;
}
/**
  * @brief  SClK_Initial() => 初始化系统时钟，系统时钟 = 16MHZ
  * @param  None
  * @retval None 
  */
static void SClK_Initial(void)
{      
	//启用内部高速晶振且无分频16MHz
	CLK_ICKR |= 0x01;             //开启内部HSI  
	while(!(CLK_ICKR&0x02));      //HSI准备就绪
	CLK_SWR = 0xe1;               //HSI为主时钟源
	CLK_CKDIVR = 0x00;            //HSI不分频
}
/**
  * @brief  GPIO_Init
  * @param  None
  * @retval None
  */
void GPIO_Init(void)
{
  PA_DDR |= 0x06;                    //PA1 PA2输出模式
	PA_CR1 |= 0x02;                    //推挽输出
	PA_CR2 |= 0x02;                    //输出速度10Mhz

  PC_DDR |= 0xD8;                    //PC3 PC4 PC6 PC7输出模式
	PC_CR1 |= 0xD8;                    //推挽输出
	PC_CR2 |= 0xD8;                    //输出速度10Mhz

  PA_DDR |= 0x10;                    //PD4 输出模式
	PA_CR1 |= 0x10;                    //推挽输出
	PA_CR2 |= 0x10;                    //输出速度10Mhz

	PA_CR1 |= 0x27;                    //PA3 PA5上拉输入

	PB_CR1 |= 0x10;                    //PB4 上拉输入

	PC_CR1 |= 0x20;                    //PC5 上拉输入
	
	PD_CR1 |= 0x02;                    //PD1 上拉输入

	Red = 1;
	Grenn = 1;

	CE = 0;
	A_DIR = 1;
	B_EN = 0;
	
}
/**
  * @brief  void ClockConfig(void) => 开启所有外设时钟
  * @param  None
  * @retval None
  */
void ClockConfig_ON(void)
{
  CLK_PCKENR1 = 0xFF;
  CLK_PCKENR2 = 0xFF;
//  UART_CLK_UART_OFF();            //关闭UART外设时钟
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Charge_For_Discharge_Detection(void)
{
	if(Ready == false){
		CE = false;
		if(a1_detection.Delay_Detection_status == false){
			A_DIR = false;
			B_EN = true;
			a1_detection.Delay_enable = true;
			a1_detection.Delay_time_out = false;
			a1_detection.Delay_Detection_status = true;
		}
		if(a1_detection.Delay_time_out == true){
			if(a1_detection.ADC_A1_AD_Voltage > (uint16_t)0x10){
				a1_detection.Current_charge_state = Charge_normal;
				system.Charge_For_Discharge = Charge_State;
			}
			if(a1_detection.ADC_A1_AD_Voltage < (uint16_t)0x08){
				B_EN = false;
				a1_detection.Delay_time_out = false;
				a1_detection.Current_charge_state = Charge_abnormal;
			}
		}
	}else{//system.Charge_For_Discharge = Discharge_State;
		system.Charge_For_Discharge = Discharge_State;
		a1_detection.Delay_time_out = false;
		B_EN = false;
		CE = false;
		A_DIR = true;
		a1_detection.Delay_enable = false;
		battery.Battery_full_locking = false;
		battery.Delay_Detection_Battery_full_status = false;
		if(Ready == true){
			a1_detection.Delay_time_out = false;
			a1_detection.Delay_Detection_status = false;
			a1_detection.Current_charge_state = Charge_normal;
		}
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Charge_Query(void)
{
	if(system.Charge_For_Discharge == Charge_State){
		if((battery.Battery_voltage > (uint16_t)0x1D7) && (battery.Current_Display == Quantity_Electricity_100)
			&& (a1_detection.ADC_A1_AD_Voltage < (uint16_t)0x4B)){
			if(battery.Delay_Detection_Battery_full_status == false){
				battery.Battery_full_time_out = true;
				battery.Delay_Detection_Battery_full_status = true;
			}
			if(battery.Battery_full_locking == true){
				battery.Battery_State = Battery_Full;
				battery.Battery_full_locking = true;
				battery.Battery_full_time_out = false;
				battery.Delay_Detection_Battery_full_status = true;
			}
		}else{
			battery.Battery_State = Battery_Charge;
			battery.Battery_full_locking = false;
			battery.Battery_full_time_out = false;
			battery.Battery_Full_cnt = false;
			battery.Battery_Full_cnt_multiple = false;
			battery.Delay_Detection_Battery_full_status = false;
		}
	}else{//system.Charge_For_Discharge == Discharge_State
		battery.Battery_Full_cnt = false;
		battery.Battery_full_locking = false;
		battery.Battery_full_time_out = false;
		battery.Battery_State = Battery_Charge;
		battery.Battery_Full_cnt_multiple = false;
		battery.Delay_Detection_Battery_full_status = false;
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
	asm("rim");                                 //开全局中断 
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

