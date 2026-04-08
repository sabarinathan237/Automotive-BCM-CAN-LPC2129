#include "lcd.h"

void delay_us(unsigned int us)
{
    unsigned int i, j;
    for (i = 0; i < us; i++)
    {
        for (j = 0; j < 10; j++)
        {
            ;
        }
    }
}

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 6000; j++)
        {
            ;
        }
    }
}

void lcd_command(unsigned char cmd)
{
    IO1CLR = LCD_DATA_MASK;
    IO1CLR = RS;
    IO1CLR = RW;
    IO1SET = ((unsigned int)cmd << 16);
    IO1SET = EN;
    delay_ms(2);
    IO1CLR = EN;
}

void lcd_data(unsigned char data)
{
    IO1CLR = LCD_DATA_MASK;
    IO1SET = RS;
    IO1CLR = RW;
    IO1SET = ((unsigned int)data << 16);
    IO1SET = EN;
    delay_ms(2);
    IO1CLR = EN;
}

void lcd_str(char *p)
{
    while (*p)
    {
        lcd_data(*p);
        p++;
    }
}

void lcd_config(void)
{
    IODIR1 |= LCD_DATA_MASK;
    IODIR1 |= RS | RW | EN;

    delay_ms(20);

    lcd_command(0x38);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
    delay_ms(5);
}
