#ifndef __AD_H
#define __AD_H
uint16_t AD_Value[5];

void AD_Init(void);
float Quantize(uint16_t adc_value);
uint8_t Quantize_0_1(uint16_t adc_value);

#endif
