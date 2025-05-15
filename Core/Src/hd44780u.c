/*
 * hd44780u.c
 *
 *  Created on: Mar 17, 2025
 *      Author: spenc
 */


#include "hd44780u.h"

int current_row;
int current_column;

static void pause(uint32_t msec)
{
	uint32_t start, current;
	start = current = HAL_GetTick();
	while(current - start < msec)
	{
		current = HAL_GetTick();
	}
}

static void write_nibble(uint8_t nibble, int is_data)
{
	HAL_GPIO_WritePin(HD44780U_RS_PORT, HD44780U_RS_PIN, is_data ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HD44780U_D4_PORT, HD44780U_D4_PIN, nibble & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HD44780U_D5_PORT, HD44780U_D5_PIN, nibble & 0x02 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HD44780U_D6_PORT, HD44780U_D6_PIN, nibble & 0x04 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HD44780U_D7_PORT, HD44780U_D7_PIN, nibble & 0x08 ? GPIO_PIN_SET : GPIO_PIN_RESET);
	pause(1);
	HAL_GPIO_WritePin(HD44780U_E_PORT, HD44780U_E_PIN, GPIO_PIN_SET);
	pause(1);
	HAL_GPIO_WritePin(HD44780U_E_PORT, HD44780U_E_PIN, GPIO_PIN_RESET);
	pause(1);

}

static void write_byte(uint8_t byte, int is_data)
{
	write_nibble(byte >> 4, is_data);
	write_nibble(byte & 0x0F, is_data);
}

void HD44780U_Init(void)
{
	current_row = 0;
	current_column = 0;
	pause(40);
	write_nibble(0x03, 0); /* wake up */
	pause(5);
	write_nibble(0x03, 0); /* wake up */
	pause(1);
	write_nibble(0x03, 0); /* wake up */
	pause(1);
	write_nibble(0x02, 0); /* Function set: 4 bit interface */
	write_byte(0x28, 0); /* Function set: 4-bit interface, 2-line, 5x8 dots */
	write_byte(0x08, 0); /* Display off */
	write_byte(0x01, 0); /* Display clear */
	write_byte(0x06, 0); /* Entry mode set, move cursor right, no shift */
	write_byte(0x0F, 0); /* Display on, blink and shift */
}


int HD44780U_Write_At_Cursor(unsigned char c)
{
	write_byte(c, 1);

	current_column++;
	if(current_column == 20)
	{
		current_column = 0;
		current_row = (current_row + 1) % 4;
		HD44780U_Move_Cursor(current_row, current_column);
	}

	return 1;
}

int HD44780U_Delete_At_Cursor(void)
{
	if(current_column == 0 && current_row == 0)
	{
		return -1;
	}
	if(current_column == 0)
	{
		int row_temp, column_temp;

		current_row--;
		current_column = 19;

		row_temp = current_row;
		column_temp = current_column;

		HD44780U_Move_Cursor(current_row, current_column);
		HD44780U_Write_At_Cursor(' ');

		current_row = row_temp;
		current_column = column_temp;
		HD44780U_Move_Cursor(current_row, current_column);
	}
	else
	{
		current_column--;
		HD44780U_Move_Cursor(current_row, current_column);
		HD44780U_Write_At_Cursor(' ');
		current_column--;
		HD44780U_Move_Cursor(current_row, current_column);
	}
	return 1;
}
int HD44780U_Write_Line(int row, const char *string)
{
	if(row > 3)
	{
		return -1;
	}
	HD44780U_Move_Cursor(row, 0);

	while(*string != '\0')
	{
		HD44780U_Write_At_Cursor((unsigned char)*(string++));
	}

	return 1;
}

int HD44780U_Move_Cursor(int row, int column)
{
	uint8_t address = 0;

	if(row > 3 || column > 19)
	{
		return -1;
	}
	switch(row)
	{
		case 0:
			address = 0;
			break;
		case 1:
			address = 40;
			break;
		case 2:
			address = 20;
			break;
		case 3:
			address = 0x54;
			break;
	}
	address += column;

	write_byte(address | 0x80, 0);

	current_row = row;
	current_column = column;

	return 1;
}

void HD44780U_Clear(void)
{
	write_byte(0x01, 0);
	return;
}
void print_int(int n, int min_digits)
{
        char buffer[32];
        int i;
        int n_copy;
        int digits_buffered = 0;

        if(n >= -0.0f && n <= 0.0f)
        {
                HD44780U_Write_At_Cursor('0');
                return;
        }

        n_copy = n;
        buffer[31] = '\0';
        for(i = 30; n_copy != 0; i--)
        {
                int digit = n_copy % 10;
                if(digit < 0)
                {
                        digit = -digit;
                }
                buffer[i] = '0' + digit;
                digits_buffered++;
                n_copy /= 10;
        }

        while(digits_buffered < min_digits)
		{
			buffer[i--] = '0';
			digits_buffered++;
		}
        if(n < 0)
        {
                buffer[i--] = '-';
        }
        for(i = i + 1; buffer[i] != '\0'; i++)
        {
                HD44780U_Write_At_Cursor(buffer[i]);
        }
}

void HD44780U_Write_Double(double d)
{
        int power = 0;
        if(d == 0.0)
        {
                HD44780U_Write_At_Cursor('0');
                return;
        }
        if(d < 0)
        {
                HD44780U_Write_At_Cursor('-');
                d = -d;
        }
        if(d < 100000000 && d > 0.001)
        {
                print_int((int)d, 1);
                HD44780U_Write_At_Cursor('.');
                d = d - (int)d;
                d *= 10000;
                print_int((int)d, 4);
                return;
        }

        while(d > 10 || d < 1)
        {
                if(d > 10)
                {
                        power++;
                        d /= 10;
                }
                else if(d < 1)
                {
                        power--;
                        d *= 10;
                }
        }
        print_int((int)d, 1);
        HD44780U_Write_At_Cursor('.');
        d = d - (int)d;
        d *= 10000;
        print_int((int)d, 4);
        HD44780U_Write_At_Cursor('e');
        print_int(power, 1);
}
