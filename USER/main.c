#include "main.h"
#include "time.h"
#include "adc.h"
#include "key.h"
#include "delay.h"

_KEY key;
_ledFun ledFun;
_Battery battery;
_System system;
_TYPE_C type_c;
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

	system.Charge_For_Discharge = Discharge_State;
	qc_detection.Mode = low_speed_mode;
	battery.Battery_State = Battery_Charge;
	a1_detection.Delay_Detection_status = false;
	battery.Battery_full_time_out = false;
	battery.Battery_full_locking = false;
	battery.Delay_Detection_Battery_full_status = false;
	system.Micro_charge_enable_for_disable = false;
	a1_detection.Current_charge_state = Charge_normal;

	system.System_sleep_countdown = false;
	system.System_sleep_countdown_cnt = false;
	system.System_sleep_countdown_cnt_multiple = false;
	qc_detection.QC_Gather_finish = false;
	
	key.key_Delay_enable_cnt = false;
	key.key_Delay_enable_cnt_multiple = false;
	key.Key_Dlay_Enable = false;
	key.key_delay_switch_en = false;
	key.key_switch_protection = false;
	system.Led_current_status = false;
	system.Led_last_state = false;

	system.Lndicator_light_cnt_multiple = false;
	system.Lndicator_light_cnt = false;
	system.Adc_gather_cnt = false;
	a1_detection.Delay_enable_cnt = false;
	a1_detection.Delay_enable = false;
	battery.Battery_Full_cnt_multiple = false;
	battery.Battery_Full_cnt = false;
	a1_detection.ADC_A1_AD_Idle_current_cnt = false;
	a1_detection.ADC_A1_AD_load_current_cnt = false;
	a1_detection.A1_STATE = A1_IDLE_STATE;

	type_c.ADC_TYPE_C_Voltage = false;
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

//	CPU_CCR = 0x28;
}
/**
  * @brief  GPIO_Init
  * @param  None
  * @retval None
  */
void GPIO_Init(void)
{
  PA_DDR |= 0x06;                    //PA1 PA2输出模式
	PA_CR1 |= 0x06;                    //推挽输出
	PA_CR2 |= 0x06;                    //输出速度10Mhz

  PB_DDR |= 0x20;                    //PB5输出模式
	PB_CR1 |= 0x20;                    //推挽输出
	PB_CR2 |= 0x20;                    //输出速度10Mhz

  PC_DDR |= 0xC8;                    //PC3 PC6 PC7输出模式
	PC_CR1 |= 0xC8;                    //推挽输出
	PC_CR2 |= 0xC8;                    //输出速度10Mhz

  PD_DDR |= 0x10;                    //PD4输出模式
	PD_CR1 |= 0x10;                    //推挽输出
	PD_CR2 |= 0x10;                    //输出速度10Mhz

	PA_DDR &= ~0x08;
	PA_CR1 |= 0x08;                    //PA3 上拉输入
	PA_CR2 &= ~0x08;

	PB_CR1 |= 0x10;                    //PB4 PB5上拉输入

	PC_DDR &= ~0x30;
	PC_CR1 |= 0x30;                    //PC4 PC5上拉输入
	PC_CR2 &= ~0x30;

#if	DEBUG
  PD_DDR |= 0x02;                    //PD1输出模式
	PD_CR1 |= 0x02;                    //推挽输出
	PD_CR2 |= 0x02;                    //输出速度10Mhz
	LED = false;
#endif

	Red = 1;
	Grenn = 1;
	Blue = 1;
	
	CE = 0;
	A_DIR = 1;
	B_EN = 0;
	A_EN2 = false;
	
}
/**
  * @brief  void ClockConfig(void) => 开启所有外设时钟
  * @param  None
  * @retval None
  */
static void ClockConfig_ON(void)
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
static void ClockConfig_OFF(void)
{
  CLK_PCKENR1 = 0x00;
  CLK_PCKENR2 = 0x00;
}
static void Key_Interrupt_Enable(void)
{
	PA_DDR &= ~0x08;
	PA_CR2 |= 0x08;
	EXTI_CR1 |= 0x02;
	//Lower edge and low level trigger.
}
static void TYPE_C_Interrupt_Enable(void)
{
	PC_DDR &= ~0x30;
	PC_CR2 |= 0x30;
	EXTI_CR1 |= 0x30;	
}
#if 0
static void Ready_charge_arouse(void)
{
	PC_DDR &= ~0x10;
	PC_CR2 |= 0x10;
	EXTI_CR1 |= 0x80;		
}
#endif
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Charge_For_Discharge_Detection(void)
{
	if((C_DIR == false) && (STAT2 == true) && (battery.Current_Display != 6)){
//Disable Micro
		B_EN = false;
		system.Micro_charge_enable_for_disable = false;
		system.Charge_For_Discharge = Charge_State;
	}else if(Ready == false){
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
				system.Micro_charge_enable_for_disable = true;
				system.Charge_For_Discharge = Charge_State;
			}
			if(a1_detection.ADC_A1_AD_Voltage < (uint16_t)0x08){
				B_EN = false;
				a1_detection.Delay_time_out = false;
				system.Micro_charge_enable_for_disable = false;
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
			system.Micro_charge_enable_for_disable = false;
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
		if(system.Micro_charge_enable_for_disable == false){
			if((type_c.ADC_TYPE_C_Voltage <= TYPE_C_SLEEP) && (battery.Battery_voltage > (uint16_t)0x1D7)){
				if(battery.Delay_Detection_Battery_full_status == false){
					battery.Battery_full_time_out = true;
				}
				if(battery.Battery_full_locking == true){
				battery.Battery_State = Battery_Full;
					battery.Battery_full_time_out = false;
					battery.Delay_Detection_Battery_full_status = true;
				}
			}
		}else if((battery.Battery_voltage > (uint16_t)0x1D7) && (battery.Current_Display == Quantity_Electricity_100)
			&& (a1_detection.ADC_A1_AD_Voltage < (uint16_t)0x4B) && (system.Micro_charge_enable_for_disable == true)){//battery.Battery_State = Battery_Full;
			if(battery.Delay_Detection_Battery_full_status == false){
				battery.Battery_full_time_out = true;
			}
			if(battery.Battery_full_locking == true){
			battery.Battery_State = Battery_Full;
				battery.Battery_full_time_out = false;
				battery.Delay_Detection_Battery_full_status = true;
			}
		}else{//battery.Battery_State = Battery_Charge;
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
	}//&&(type_c.ADC_TYPE_C_Voltage <= TYPE_C_SLEEP)

	if((qc_detection.ADC_QC_Voltage < Overload_event)&&(qc_detection.QC_Gather_finish == true)
		&&(key.key_switch_protection == false)){
		if(++system.Overload_cnt >= 200){
			system.Overload_cnt = false;
			system.System_State = System_Sleep;
		}
	}else{
		system.Overload_cnt = false;
	}//&&(STAT2 != true)
	if((a1_detection.ADC_A1_AD_Voltage < Idle_Voltage)&&((type_c.ADC_TYPE_C_Voltage <= TYPE_C_SLEEP)||(STAT2 != true))){
		system.System_sleep_countdown = true;
	}else{
		system.System_sleep_countdown = false;
		system.System_sleep_countdown_cnt = false;
		system.System_sleep_countdown_cnt_multiple = false;
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void System_Initial(void)
{
	System_Variable_Init();
	SClK_Initial();
	ClockConfig_ON();
	GPIO_Init();
	ADC_Init();
	Time2_Init();	
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Sleep_task(void)
{
	Red = 1;
	Grenn = 1;
	CE = true;
	B_EN = false;
	A_DIR = false;
	A_EN2 = false;
	ADC_OFF_CMD();
	Tim2_DeInit();
	asm("sim");                                     //关闭全局中断
	if(battery.Batter_Low_Pressure != Batter_Low){
		Key_Interrupt_Enable();
	}
//	Ready_charge_arouse();
	TYPE_C_Interrupt_Enable();
	asm("rim");                                     //开全局中断 
	sleep:
  ClockConfig_OFF();                              //关闭所有外设时钟  
  asm("halt");                                    //进入停机模式
  ClockConfig_ON();
	if(TEST1 == false){
		delay_ms(30);
		if(TEST1 != false){
			goto sleep;
		}
	}
	System_Initial();
	qc_detection.QC_Gather_finish = false;
	system.NotifyLight_EN = true;
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
static void Usb_disable_qc_speed(void)
{
	if(key.key_delay_switch_en == true){
		key.Key_Dlay_Enable = false;
		key.key_delay_switch_en = false;
		key.key_switch_protection = false;
//		A_EN2 = !A_EN2;
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
void main(void)
{
	System_Initial();
	battery.Current_Display = 6;
	system.Flay_Adc_gather = true;
	delay_ms(150);
	battery.Battery_Level_Update = true;
	asm("rim");                                 //开全局中断 
	while(1){
	if(system.System_State == System_Run){
			Charge_For_Discharge_Detection();
			Key_event();
			Adc_Task();
			Charge_Query();
			Battery_Volume();
			Usb_disable_qc_speed();
		}else{//system.System_State == System_Sleep
			Sleep_task();
		}
	}
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
#pragma vector=EXTI0_vector//0x05
__interrupt void Exti0_OVR_IRQHandler(void){
//Don't do the action
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
#pragma vector=EXTI2_vector//0x07
__interrupt void Exti2_OVR_IRQHandler(void){
//Don't do the action
}
/**
  * @brief  None
  * @param  None
  * @retval None
  */
#pragma vector=EXTI3_vector//0x08
__interrupt void Exti3_OVR_IRQHandler(void){
//Don't do the action
}
