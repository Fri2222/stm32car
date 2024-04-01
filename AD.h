#ifndef __AD_H
#define __AD_H
uint16_t AD_Value[5];

void AD_Init(void);
float Quantize(uint16_t adc_value);
void adjustMotorSpeed(void);

#endif
