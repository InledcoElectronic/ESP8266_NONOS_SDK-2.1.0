/*
 * uart1.h
 *
 *  Created on: 2017年10月20日
 *      Author: liruya
 */

#ifndef UART1_H_
#define UART1_H_

#include "uart.h"

typedef void (* uart1_tx_empty_cb_t)( void );

extern void uart1_init( uint32_t rate );
extern STATUS uart1_send_byte_nowait( uint8_t byte );
extern uint8_t uart1_send_byte( uint8_t byte );
extern void uart1_send_buffer( uint8_t *buf, uint32_t len );
extern void uart1_set_tx_empty_cb( uart1_tx_empty_cb_t cb );

#endif /* UART1_H_ */
