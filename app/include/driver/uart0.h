/*
 * uart0.h
 *
 *  Created on: 2017��9��6��
 *      Author: liruya
 */

#ifndef _UART0_H_
#define _UART0_H_

#include "c_types.h"
#include "uart.h"

typedef struct{
	uint8_t *buffer;
	uint8_t len;
}rx_buffer_t;

typedef void (* uart0_rx_cb_t)( rx_buffer_t *pbuf );
typedef void (* uart0_tx_empty_cb_t)( void );

extern void uart0_init( uint32_t rate, uint32_t frame_interval, void *para );
extern STATUS uart0_send_byte_nowait( uint8_t byte );
extern uint8_t uart0_send_byte( uint8_t byte );
extern void uart0_send_buffer( uint8_t *buf, uint32_t len );
extern void uart0_set_rx_cb( uart0_rx_cb_t cb );
extern void uart0_set_tx_empty_cb( uart0_tx_empty_cb_t cb );

#endif /* _UART0_H_ */
