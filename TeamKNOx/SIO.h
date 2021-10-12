// SIO.h

#ifndef SIO_H
#define SIO_H

/*--- Sirial IO registers ---*/
#define REG_SIOCNT   *(vu16*)0x4000128
#define REG_SIODATA8 *(vu16*)0x400012A
#define REG_RCNT     *(vu16*)0x4000134

/*--- REG_SIOCNT defines ---*/
// UART Mode
#define BAUD_RATE_9600   0x0000
#define BAUD_RATE_38400  0x0001
#define BAUD_RATE_57600  0x0002
#define BAUD_RATE_115200 0x0003
#define CTS_ENABLE       0x0004
#define SEND_DATA_FULL   0x0010
#define RECV_DATA_EMPTY  0x0020
#define ERROR            0x0040
#define DATA_LENGTH_7    0x0000
#define DATA_LENGTH_8    0x0080
#define FIFO_ENABLE      0x0100
#define PARITY_NONE      0x0000
#define PARITY_EVEN      0x0200
#define PARITY_ODD       0x0208
#define SEND_ENABLE      0x0400
#define RECV_ENABLE      0x0800
#define UART_MODE        0x3000
#define SIO_IRQ_ENABLE   0x4000


#endif
