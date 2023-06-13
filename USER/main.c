#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_usart.h"
#include "ESP8266.h"

#define ADC_CH_PHOTOSENSITIVE_SENSOR 0
#define ADC_RESOLUTION 1024
#define ADC_VREF 3.3

void setup() {
  // Initialize the ADC.
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous;
  ADC_InitStructure.ADC_ScanConvMode = ADC_Scan_Seq;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // Initialize the GPIO pins for the ADC.
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Initialize the USART for communication with the ESP8266.
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(USART1, &USART_InitStructure);

  // Initialize the ESP8266.
  esp8266_init();
}

void loop() {
  // Read the light level from the ADC.
  uint16_t light_level = ADC_GetConversionValue(ADC1);

  // Convert the light level to a percentage.
  float percentage = (float)light_level / (float)ADC_RESOLUTION * 100.0;

  // Send the light level to the ESP8266.
  esp8266_send_data(percentage);

  // Wait for 1 second.
  delay(1000);
}
