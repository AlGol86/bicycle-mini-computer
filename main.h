#include "stm8s.h"

#define PORT_COLOR_LED                GPIOC

#define PIN_BLUE       4
#define BIT_BLUE                      (1<<PIN_BLUE )
#define PIN_GREEN      5
#define BIT_GREEN                     (1<<PIN_GREEN )
#define PIN_RED        7
#define BIT_RED                       (1<<PIN_RED )
#define PIN_GND        6
#define BIT_GND                       (1<<PIN_GND )
#define COLOR_LED_MASK                (BIT_GND | BIT_RED | BIT_GREEN | BIT_BLUE)

#define PORT_ZUMMER                   GPIOA

#define PIN_ZUMMER       1
#define BIT_ZUMMER                    (1<<PIN_ZUMMER )

#define GAUSS_ADC_CH          6
#define ZERO_MAGNETIC         544
#define MAGNETIC_THRESHOLD    30
#define THREE_SEC_CNT         37665
//#define WHEEL_ROUND_LENGTH    ((long)413) //206,5 cm(measurement with meterstrip), but after 2050 m 201.5 cm - need to check
//#define WHEEL_ROUND_LENGTH    ((unsigned long)394) //197 cm(measurement with meterstrip), but after 2050 m 201.5 cm - need to check
#define WHEEL_ROUND_LENGTH    ((unsigned long)401) //200.5 cm(measurement with meterstrip), but after 2050 m 201.5 cm - need to check
#define WHEEL_EKVIVAL         ((WHEEL_ROUND_LENGTH * (long)12555 * (long)18) / 1000)


unsigned int get_ADC(char ch);
char get_magnetic_state(unsigned int gaussVal);
void sound(char on);
void init_tim2 (void);