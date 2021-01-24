/**
 * @file i2c-lcd.h
 * @author	https://controllerstech.com/i2c-lcd-in-stm32/
 * @author	Michał Pałka
 *
 * <pre>
 * This library I copied from \ref https://controllerstech.com/i2c-lcd-in-stm32.
 * Unfortunately there were few unacceptable techniques in it so I decided to improve it.
 * </pre>
 */
#include "stm32l4xx_hal.h"

void lcd_init (void);   // initialize lcd
/**
 * lcd_send_cmd function sends character to 1602 LCS as a command.
 * @param[in]	cmd	command char
 *
 * Unfortunately it works in blocking mode. However there is up to 8 signs (2 command characters) to sent via i2c in one time, so I decided not to change it.
 */
void lcd_send_cmd (char cmd);  // send command to the lcd
/**
 * lcd_send_data in original implementation was sending single character (4 sign) via i2c in blocking mode.
 * It was called in the loop for every character in the lcd_send_string, so it could take even 10 ms to send whole line to the display.
 * It was unacceptable for me so now it adds data to i2c Tx buffer which is sending in non-blocking mode.
 * @param	data	data char which is adding to Tx buffer
 */
void lcd_send_data (char data);  // send data to the lcd
/**
 * lcd_send_string sends data to LCD in non blocking mode (in my implementation).
 * @param	str	pointer to sending data
 */
void lcd_send_string (char *str);  // send string to the lcd
/**
 * lcd_put_cur sends command to set cursor in given position on LCD. It works in blocking mode but it sends only one command character (4 sign) so I decided not to change it.
 * @param[in]	row		row cursor position, range [0-1]
 * @param[in]	col		column cursor position, range [0-15]
 */
void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);
/**
 * lcd_clear sends commands to clear LCD display. It works in blocking mode but it sends only two command character (8 sign) so I decided not to change it.
 */
void lcd_clear (void);
