#include "main.h"
#include "time.h"

_ledFun ledFun;

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
  PA_DDR |= 0x02;                    //PA1 输出模式
	PA_CR1 |= 0x02;                    //推挽输出
	PA_CR2 |= 0x02;                    //输出速度10Mhz

  PC_DDR |= 0xD8;                    //PC3 PC4 PC6 PC7输出模式
	PC_CR1 |= 0xD8;                    //推挽输出
	PC_CR2 |= 0xD8;                    //输出速度10Mhz

  PA_DDR |= 0x10;                    //PD4 输出模式
	PA_CR1 |= 0x10;                    //推挽输出
	PA_CR2 |= 0x10;                    //输出速度10Mhz

	PA_CR1 |= 0x2C;                    //PA2 PA3 PA5上拉输入

	PB_CR1 |= 0x10;                    //PB4 上拉输入

	PC_CR1 |= 0x20;                    //PC5 上拉输入
	
	PD_CR1 |= 0x02;                    //PD1 上拉输入

	
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
void main(void)
{
	SClK_Initial();
	GPIO_Init();
	ClockConfig_ON();
	Time2_Init();
	while(1);
}

