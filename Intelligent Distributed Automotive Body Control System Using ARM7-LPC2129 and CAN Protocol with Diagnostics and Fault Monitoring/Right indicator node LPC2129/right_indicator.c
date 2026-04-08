#include <lpc21xx.h>

#define RLED1   (1 << 20)
#define RLED2   (1 << 21)
#define RLED3   (1 << 22)

typedef unsigned int u32;

typedef struct
{
    u32 id;
    u32 rtr;
    u32 dlc;
    u32 byteA;
    u32 byteB;
} CAN_MSG;

void delay_ms(unsigned int ms);
void can2_init(void);
void can2_send(CAN_MSG m1);
void can2_receive(CAN_MSG *m1);

void right_leds_on(void);
void right_leds_off(void);
void send_right_diag_response(void);

int main(void)
{
    CAN_MSG rx_msg;
    unsigned char right_cmd = 0;
    unsigned char hazard_cmd = 0;

    IODIR0 |= (RLED1 | RLED2 | RLED3);
    right_leds_off();

    can2_init();

    while (1)
    {
        can2_receive(&rx_msg);

        if (rx_msg.id == 0x22)            // Right indicator command
        {
            if (rx_msg.byteA == 0x02)
            {
                if (rx_msg.byteB == 0x01)
                    right_cmd = 1;
                else
                    right_cmd = 0;
            }
        }
        else if (rx_msg.id == 0x24)       // Hazard command
        {
            if (rx_msg.byteA == 0x04)
            {
                if (rx_msg.byteB == 0x01)
                    hazard_cmd = 1;
                else
                    hazard_cmd = 0;
            }
        }
        else if (rx_msg.id == 0x40)       // Diagnostic request
        {
            send_right_diag_response();
        }

        while (right_cmd || hazard_cmd)
        {
            right_leds_on();
            delay_ms(400);

            right_leds_off();
            delay_ms(400);

            can2_receive(&rx_msg);

            if (rx_msg.id == 0x22)
            {
                if (rx_msg.byteA == 0x02)
                {
                    if (rx_msg.byteB == 0x01)
                        right_cmd = 1;
                    else
                        right_cmd = 0;
                }
            }
            else if (rx_msg.id == 0x24)
            {
                if (rx_msg.byteA == 0x04)
                {
                    if (rx_msg.byteB == 0x01)
                        hazard_cmd = 1;
                    else
                        hazard_cmd = 0;
                }
            }
            else if (rx_msg.id == 0x40)
            {
                send_right_diag_response();
            }

            if ((right_cmd == 0) && (hazard_cmd == 0))
            {
                right_leds_off();
                break;
            }
        }

        right_leds_off();
    }
}

void right_leds_on(void)
{
    IOSET0 = RLED1 | RLED2 | RLED3;
}

void right_leds_off(void)
{
    IOCLR0 = RLED1 | RLED2 | RLED3;
}

void can2_init(void)
{
    PINSEL1 &= ~(0xF << 14);
    PINSEL1 |=  (0x5 << 14);   // P0.23 = RD2, P0.24 = TD2

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

void can2_receive(CAN_MSG *m1)
{
    while ((C2GSR & 0x01) == 0);

    m1->id  = C2RID;
    m1->dlc = (C2RFS >> 16) & 0x0F;
    m1->rtr = (C2RFS >> 30) & 0x01;

    if (m1->rtr == 0)
    {
        m1->byteA = C2RDA;
        m1->byteB = C2RDB;
    }

    C2CMR = (1 << 2);   // release receive buffer
}

void send_right_diag_response(void)
{
    CAN_MSG tx_msg;

    tx_msg.id    = 0x42;   // Right node diagnostic response
    tx_msg.rtr   = 0;
    tx_msg.dlc   = 2;
    tx_msg.byteA = 0x02;   // Right node ID/category
    tx_msg.byteB = 0x00;   // 0x00 = healthy

    can2_send(tx_msg);
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
