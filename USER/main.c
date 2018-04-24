#include "main.h"
#include "time.h"

_ledFun ledFun;

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
  PA_DDR |= 0x02;                    //PA1 ���ģʽ
	PA_CR1 |= 0x02;                    //�������
	PA_CR2 |= 0x02;                    //����ٶ�10Mhz

  PC_DDR |= 0xD8;                    //PC3 PC4 PC6 PC7���ģʽ
	PC_CR1 |= 0xD8;                    //�������
	PC_CR2 |= 0xD8;                    //����ٶ�10Mhz

  PA_DDR |= 0x10;                    //PD4 ���ģʽ
	PA_CR1 |= 0x10;                    //�������
	PA_CR2 |= 0x10;                    //����ٶ�10Mhz

	PA_CR1 |= 0x2C;                    //PA2 PA3 PA5��������

	PB_CR1 |= 0x10;                    //PB4 ��������

	PC_CR1 |= 0x20;                    //PC5 ��������
	
	PD_CR1 |= 0x02;                    //PD1 ��������

	
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
void main(void)
{
	SClK_Initial();
	GPIO_Init();
	ClockConfig_ON();
	Time2_Init();
	while(1);
}

