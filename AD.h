#ifndef __AD_H
#define __AD_H
uint16_t AD_Value[5];
uint16_t AD_Value_Number;
void AD_Init(void);
float Quantize(uint16_t adc_value);
uint8_t Quantize_0_1(uint16_t adc_value);
void Quantize_AD_Value_Number(void);
int16_t AD_Value_Difference_1_3(void);
int16_t AD_Value_Difference_0_4(void);
#endif
