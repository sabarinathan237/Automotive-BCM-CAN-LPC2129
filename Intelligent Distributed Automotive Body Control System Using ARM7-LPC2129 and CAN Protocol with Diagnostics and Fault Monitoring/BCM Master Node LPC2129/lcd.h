#ifndef __LCD_H__
#define __LCD_H__

#include <lpc21xx.h>

#define LCD_DATA_MASK   0x00FF0000

#define RS  (1 << 17)
#define RW  (1 << 18)
#define EN  (1 << 19)

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

void lcd_command(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_str(char *p);
void lcd_config(void);

#endif
