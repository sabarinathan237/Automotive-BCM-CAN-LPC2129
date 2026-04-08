#include <lpc21xx.h>

#define WIPER_LOW_OUT    (1 << 16)
#define WIPER_HIGH_OUT   (1 << 17)
#define FAULT_SW         18

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

void wiper_off(void);
void wiper_low(void);
void wiper_high(void);
void send_wiper_diag_response(unsigned char status_code);
void send_wiper_fault(unsigned char fault_code);

int main(void)
{
    CAN_MSG rx_msg;
    unsigned char wiper_mode = 0;
    unsigned char fault_status = 0;

    IODIR0 |= (WIPER_LOW_OUT | WIPER_HIGH_OUT);
    IODIR0 &= ~(1 << FAULT_SW);

    wiper_off();
    can2_init();

    while (1)
    {
        can2_receive(&rx_msg);

        if (rx_msg.id == 0x23)     // Wiper command
        {
            if (rx_msg.byteA == 0x03)
            {
                wiper_mode = (unsigned char)rx_msg.byteB;

                if (((IOPIN0 >> FAULT_SW) & 1) == 0)
                {
                    fault_status = 1;
                    wiper_off();
                    send_wiper_fault(0x02);   // motor fault simulated
                }
                else
                {
                    fault_status = 0;

                    if (wiper_mode == 0x00)
                        wiper_off();
                    else if (wiper_mode == 0x01)
                        wiper_low();
                    else if (wiper_mode == 0x02)
                        wiper_high();
                    else
                        wiper_off();
                }
            }
        }
        else if (rx_msg.id == 0x40)   // Diagnostic request
        {
            if (fault_status)
                send_wiper_diag_response(0x02);
            else
                send_wiper_diag_response(0x00);
        }
    }
}

void wiper_off(void)
{
    IOCLR0 = WIPER_LOW_OUT;
    IOCLR0 = WIPER_HIGH_OUT;
}

void wiper_low(void)
{
    IOSET0 = WIPER_LOW_OUT;
    IOCLR0 = WIPER_HIGH_OUT;
}

void wiper_high(void)
{
    IOCLR0 = WIPER_LOW_OUT;
    IOSET0 = WIPER_HIGH_OUT;
}

void can2_init(void)
{
    PINSEL1 &= ~(0xF << 14);
    PINSEL1 |=  (0x5 << 14);   // P0.23 = RD2, P0.24 = TD2

    VPBDIV = 0x01;
    C2MOD  = 0x01;
    AFMR   = 0x02;
    C2BTR  = 0x001C001D;       // 125 kbps
    C2MOD  = 0x00;
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

    C2CMR = (1 << 2);
}

void send_wiper_diag_response(unsigned char status_code)
{
    CAN_MSG tx_msg;

    tx_msg.id    = 0x43;   // Wiper node diagnostic response
    tx_msg.rtr   = 0;
    tx_msg.dlc   = 2;
    tx_msg.byteA = 0x03;   // Wiper node category
    tx_msg.byteB = status_code;

    can2_send(tx_msg);
}

void send_wiper_fault(unsigned char fault_code)
{
    CAN_MSG tx_msg;

    tx_msg.id    = 0x33;   // Wiper fault frame
    tx_msg.rtr   = 0;
    tx_msg.dlc   = 2;
    tx_msg.byteA = 0x03;
    tx_msg.byteB = fault_code;

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
