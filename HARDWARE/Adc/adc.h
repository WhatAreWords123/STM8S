#ifndef __ADC_H
#define __ADC_H
#include "config.h"

void ADC_Init(void);
void Adc_Task(void);
void ADC_OFF_CMD(void);
void Battery_Volume(void);
uint16_t Read_ADC(uint8_t ch);

#endif
