#ifndef BUTTON_MATRIX_H
#define BUTTON_MATRIX_H

#include "stm32f4xx_hal.h"

#define COL0_PORT GPIOC
#define COL0_PIN GPIO_PIN_0

#define ROW0_PORT GPIOC
#define ROW0_PIN GPIO_PIN_1


extern GPIO_TypeDef *button_matrix_row_ports[];
extern uint16_t button_matrix_row_pins[];

extern GPIO_TypeDef *button_matrix_col_ports[];
extern uint16_t button_matrix_col_pins[];

extern char key_dict[];

extern uint32_t latest_press;

void button_matrix_init(void);
int button_matrix_is_pressed(int row, int col);
void button_matrix_wait_for_unpress(void);
void button_matrix_get_press(int *row, int *col);
char button_matrix_get_key(void);
#endif
