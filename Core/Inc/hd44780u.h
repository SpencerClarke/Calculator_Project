#ifndef HD44780U_H
#define HD44780U_H


#include "stm32f4xx_hal.h"

#define HD44780U_RS_PORT GPIOA
#define HD44780U_RS_PIN GPIO_PIN_5

#define HD44780U_E_PORT GPIOA
#define HD44780U_E_PIN GPIO_PIN_6

#define HD44780U_D7_PORT GPIOA
#define HD44780U_D7_PIN GPIO_PIN_7

#define HD44780U_D6_PORT GPIOB
#define HD44780U_D6_PIN GPIO_PIN_6

#define HD44780U_D5_PORT GPIOC
#define HD44780U_D5_PIN GPIO_PIN_7

#define HD44780U_D4_PORT GPIOA
#define HD44780U_D4_PIN GPIO_PIN_9

void HD44780U_Init(void);
int HD44780U_Write_At_Cursor(unsigned char c);
int HD44780U_Delete_At_Cursor(void);
int HD44780U_Write_Line(int row, const char *string);
void HD44780U_Write_Double(double f);
int HD44780U_Move_Cursor(int row, int column);
void HD44780U_Clear(void);

#endif
