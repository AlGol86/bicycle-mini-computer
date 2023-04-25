
#ifndef LED_H
#define LED_H

//display-1
#define PORT_led1_ODR       GPIOA->ODR
#define PORT_led1_DDR       GPIOA->DDR
#define PORT_led1_IDR       GPIOA->IDR
#define PORT_led1_CR1       GPIOA->CR1
#define PORT_led1_CR2       GPIOA->CR2
#define PIN_DC1_da   2
#define PIN_DC1_clk  1

#define BIT_DC1_da  (1<<PIN_DC1_da)
#define BIT_DC1_clk (1<<PIN_DC1_clk)

#define BIT_DC1_mask (BIT_DC1_da|BIT_DC1_clk)

//display-2
#define PORT_led2_ODR       GPIOC->ODR
#define PORT_led2_DDR       GPIOC->DDR
#define PORT_led2_IDR       GPIOC->IDR
#define PORT_led2_CR1       GPIOC->CR1
#define PORT_led2_CR2       GPIOC->CR2
#define PIN_DC2_da   3
#define PIN_DC2_clk  4

#define BIT_DC2_da  (1<<PIN_DC2_da)
#define BIT_DC2_clk (1<<PIN_DC2_clk)

#define BIT_DC2_mask (BIT_DC2_da|BIT_DC2_clk)

#define CMD_ADDR_0   0xc0
#define BRIGHTNESS   4 //0-7
#define CMD_DISP_ON  (0x88+BRIGHTNESS)
#define CMD_DISP_OFF 0x80
#define CMD_DATA_W_norm_incADDR 0x40

void sysDelay(char del);
void init_LED(void);
void sys_del_ms(int del_ms);

void startTransmittoinD1(void);
void sendByteD1(char byte);
void stopTransmittoinD1(void);
void sendCMD_D1(char byte);
void sendDataBunchD1(char* byte);

void startTransmittoinD2(void);
void sendByteD2(char byte);
void stopTransmittoinD2(void);
void sendCMD_D2(char byte);
void sendDataBunchD2(char* byte);

void printData(char* byte,  char display);
void printValue(int val, char leftAlignment,  char display);
void printHigherNumber(int val, char display);
#endif