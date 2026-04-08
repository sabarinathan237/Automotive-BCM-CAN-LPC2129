#include <lpc21xx.h>
#include "lcd.h"

#define LEFT_SW        10
#define RIGHT_SW       11
#define HAZARD_SW      12
#define WIPER_SW       13

#define SW_PRESSED(bit)   (((IOPIN0 >> (bit)) & 1) == 0)

typedef unsigned int u32;

typedef struct
{
    u32 id;
    u32 rtr;
    u32 dlc;
    u32 byteA;
    u32 byteB;
} CAN_MSG;

void can2_init(void);
void can2_send(CAN_MSG m1);
void update_lcd_status(unsigned char left, unsigned char right, unsigned char hazard, unsigned char wiper_mode);

int main(void)
{
    CAN_MSG m1;

    unsigned char left_state   = 0;
    unsigned char right_state  = 0;
    unsigned char hazard_state = 0;
    unsigned char wiper_mode   = 0;   // 0=OFF, 1=LOW, 2=HIGH

    lcd_config();
    can2_init();

    IODIR0 &= ~(1 << LEFT_SW);
    IODIR0 &= ~(1 << RIGHT_SW);
    IODIR0 &= ~(1 << HAZARD_SW);
    IODIR0 &= ~(1 << WIPER_SW);

    m1.rtr   = 0;
    m1.dlc   = 2;
    m1.byteA = 0;
    m1.byteB = 0;

    lcd_command(0x01);
    lcd_command(0x80);
    lcd_str("AUTOMOTIVE BCM");
    lcd_command(0xC0);
    lcd_str("USING CAN");

    delay_ms(1500);
    update_lcd_status(left_state, right_state, hazard_state, wiper_mode);

    while (1)
    {
        if (SW_PRESSED(LEFT_SW))
        {
            if (hazard_state == 1)
            {
                hazard_state = 0;
                m1.id = 0x24;
                m1.byteA = 0x04;
                m1.byteB = 0x00;
                can2_send(m1);
            }

            left_state = !left_state;
            right_state = 0;

            m1.id = 0x21;
            m1.byteA = 0x01;
            m1.byteB = left_state;
            can2_send(m1);

            m1.id = 0x22;
            m1.byteA = 0x02;
            m1.byteB = 0x00;
            can2_send(m1);

            update_lcd_status(left_state, right_state, hazard_state, wiper_mode);
            delay_ms(300);
            while (SW_PRESSED(LEFT_SW));
        }

        if (SW_PRESSED(RIGHT_SW))
        {
            if (hazard_state == 1)
            {
                hazard_state = 0;
                m1.id = 0x24;
                m1.byteA = 0x04;
                m1.byteB = 0x00;
                can2_send(m1);
            }

            right_state = !right_state;
            left_state = 0;

            m1.id = 0x22;
            m1.byteA = 0x02;
            m1.byteB = right_state;
            can2_send(m1);

            m1.id = 0x21;
            m1.byteA = 0x01;
            m1.byteB = 0x00;
            can2_send(m1);

            update_lcd_status(left_state, right_state, hazard_state, wiper_mode);
            delay_ms(300);
            while (SW_PRESSED(RIGHT_SW));
        }

        if (SW_PRESSED(HAZARD_SW))
        {
            hazard_state = !hazard_state;

            if (hazard_state == 1)
            {
                left_state = 0;
                right_state = 0;
            }

            m1.id = 0x24;
            m1.byteA = 0x04;
            m1.byteB = hazard_state;
            can2_send(m1);

            update_lcd_status(left_state, right_state, hazard_state, wiper_mode);
            delay_ms(300);
            while (SW_PRESSED(HAZARD_SW));
        }

        if (SW_PRESSED(WIPER_SW))
        {
            wiper_mode++;
            if (wiper_mode > 2)
                wiper_mode = 0;

            m1.id = 0x23;
            m1.byteA = 0x03;
            m1.byteB = wiper_mode;
            can2_send(m1);

            update_lcd_status(left_state, right_state, hazard_state, wiper_mode);
            delay_ms(300);
            while (SW_PRESSED(WIPER_SW));
        }
    }
}

void can2_init(void)
{
    PINSEL1 &= ~(0xF << 14);
    PINSEL1 |=  (0x5 << 14);   // P0.23=RD2, P0.24=TD2

    VPBDIV = 0x01;             // PCLK = CCLK
    C2MOD  = 0x01;             // reset mode
    AFMR   = 0x02;             // bypass acceptance filter
    C2BTR  = 0x001C001D;       // 125 kbps
    C2MOD  = 0x00;             // normal mode
}

void can2_send(CAN_MSG m1)
{
    C2TFI1 = 0x00000000;
    C2TFI1 = (m1.dlc << 16);
    C2TID1 = m1.id;

    if (m1.rtr == 0)
    {
        C2TDA1 = m1.byteA;
        C2TDB1 = m1.byteB;
    }
    else
    {
        C2TFI1 |= (1 << 30);
    }

    C2CMR = (1 << 0) | (1 << 5);

    while ((C2GSR & (1 << 3)) == 0);
}

void update_lcd_status(unsigned char left, unsigned char right, unsigned char hazard, unsigned char wiper_mode)
{
    lcd_command(0x01);
    lcd_command(0x80);

    if (hazard)
    {
        lcd_str("HZ:ON ");
    }
    else
    {
        lcd_str("HZ:OFF");
    }

    lcd_str(" ");

    if (left)
        lcd_str("L:ON");
    else
        lcd_str("L:OFF");

    lcd_command(0xC0);

    if (right)
        lcd_str("R:ON ");
    else
        lcd_str("R:OFF");

    lcd_str(" ");

    if (wiper_mode == 0)
        lcd_str("W:OFF");
    else if (wiper_mode == 1)
        lcd_str("W:LOW");
    else
        lcd_str("W:HIGH");
}
