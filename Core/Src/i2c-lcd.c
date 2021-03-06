/**
 * @file i2c-lcd.c
 * @author	https://controllerstech.com/i2c-lcd-in-stm32/
 * @author	Michał Pałka
 *
 * In this file there is implementation of i2c-lcd library.
 */
#include "i2c-lcd.h"
#include <string.h>
/**
 *  i2c handle structure for hi2c conected to LCD display
 */
extern I2C_HandleTypeDef hi2c1;  // change your handler here accordingly

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

uint8_t i2c_Tx_flag=0;	/**< i2c Tx transmission availability flag*/

void lcd_send_cmd (char cmd)
{
    char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}
char data_to_sent[32*4]={0};	/**< i2c Tx buffer*/
uint8_t id;		/**< counter of sending message characters. It increments after every 4 sign added to Tx buffer, and zeros when all characters are added*/
void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	data_to_sent[0+id*4]=data_t[0];
	data_to_sent[1+id*4]=data_t[1];
	data_to_sent[2+id*4]=data_t[2];
	data_to_sent[3+id*4]=data_t[3];
	++id;
	//HAL_I2C_Master_Transmit (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_t, 4, 100);
}

void lcd_clear (void)
{
	lcd_send_cmd (0x80);
	lcd_send_cmd(1);
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }

    lcd_send_cmd (col);
}


void lcd_init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	HAL_Delay(1);  // wait for >100us
	lcd_send_cmd (0x30);
	HAL_Delay(10);
	lcd_send_cmd (0x20);  // 4bit mode
	HAL_Delay(10);

  // display initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	lcd_send_cmd (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}
void lcd_send_string (char *str)
{
	id=0;
	while (*str) lcd_send_data (*str++);
	HAL_I2C_Master_Transmit_IT (&hi2c1, SLAVE_ADDRESS_LCD,(uint8_t *) data_to_sent, strlen(data_to_sent));
	i2c_Tx_flag=1;
}

/**
 * HAL_I2C_MasterTxCpltCallback is called when i2c transmission is completed and zeros Tx buffer.
 * @param[in]	hi2c	pointer to i2c handle structure
 */
void HAL_I2C_MasterTxCpltCallback (I2C_HandleTypeDef * hi2c){
	if (hi2c==&hi2c1){
		memset(&data_to_sent,0,32*4);
		i2c_Tx_flag=0;
	}
}
