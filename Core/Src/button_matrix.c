#include "button_matrix.h"


GPIO_TypeDef *button_matrix_row_ports[] = {GPIOC,      GPIOC,      GPIOA,      GPIOB,      GPIOB};
uint16_t button_matrix_row_pins[] = {      GPIO_PIN_3, GPIO_PIN_2, GPIO_PIN_4, GPIO_PIN_7, GPIO_PIN_1};

GPIO_TypeDef *button_matrix_col_ports[] = {GPIOC,      GPIOC,      GPIOB,      GPIOA,      GPIOB,      GPIOB};
uint16_t button_matrix_col_pins[] = {      GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_0, GPIO_PIN_8, GPIO_PIN_3, GPIO_PIN_5};

uint32_t latest_press = 0;
char key_dict[] = {
		'1', '2', '3', '+', '^',
		'4', '5', '\0', '-', 's',
		'7', '8', '9', '*', 'c',
		'.', '0', 'l', '/', 'p',
		'(', ')', 'a', '\0', 'e',
		'6', '\0', '\0', 'd', 'n'
};

void button_matrix_init(void)
{
	for(int i = 0; i < sizeof(button_matrix_row_ports) / sizeof(button_matrix_row_ports[0]); i++)
	{
		HAL_GPIO_WritePin(button_matrix_row_ports[i], button_matrix_row_pins[i], GPIO_PIN_SET);
	}
}
int button_matrix_is_pressed(int row, int col)
{
	int debounce_checks = 5;
	int debounce_ms = 2;

	HAL_GPIO_WritePin(button_matrix_row_ports[row], button_matrix_row_pins[row], GPIO_PIN_RESET);

	for(int i = 0; i < debounce_checks; i++)
	{
		if(HAL_GPIO_ReadPin(button_matrix_col_ports[col], button_matrix_col_pins[col]) == GPIO_PIN_RESET)
		{
			HAL_GPIO_WritePin(button_matrix_row_ports[row], button_matrix_row_pins[row], GPIO_PIN_SET);
			return 1;
		}
		HAL_Delay(debounce_ms);
	}
	HAL_GPIO_WritePin(button_matrix_row_ports[row], button_matrix_row_pins[row], GPIO_PIN_SET);
	return 0;
}

void button_matrix_wait_for_unpress(void)
{
	int is_pressed = 1;
	while(is_pressed)
	{
		is_pressed = 0;
		for(int i = 0; i < sizeof(button_matrix_row_pins)/sizeof(button_matrix_row_pins[0]); i++)
		{
			for(int j = 0; j < sizeof(button_matrix_col_pins)/sizeof(button_matrix_col_pins[0]); j++)
			{
				if(button_matrix_is_pressed(i, j))
				{
					is_pressed = 1;
				}
			}
		}
		HAL_Delay(2);
	}
}

void button_matrix_get_press(int *row, int *col)
{
	for(;;)
	{
		for(int i = 0; i < sizeof(button_matrix_row_pins)/sizeof(button_matrix_row_pins[0]); i++)
		{
			for(int j = 0; j < sizeof(button_matrix_col_pins)/sizeof(button_matrix_col_pins[0]); j++)
			{
				if(button_matrix_is_pressed(i, j))
				{
					*row = i;
					*col = j;
					goto get_press_end;
				}
			}
		}
	}
	get_press_end:
	button_matrix_wait_for_unpress();
	HAL_Delay(2);
}

char button_matrix_get_key(void)
{
	int row, col;
	button_matrix_get_press(&row, &col);
	return key_dict[(5-col) * 5 + row];
}
