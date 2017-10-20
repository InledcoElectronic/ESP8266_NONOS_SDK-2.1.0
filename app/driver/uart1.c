/*
 * uart1.c
 *
 *  Created on: 2017年10月20日
 *      Author: liruya
 */

#include "uart1.h"
#include "ets_sys.h"

LOCAL uart1_tx_empty_cb_t uart1_tx_empty_cb;

void ICACHE_FLASH_ATTR uart1_init ( uint32_t rate )
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);
	uart_div_modify( UART1, rate );
	WRITE_PERI_REG( UART_CONF0( UART1 ), ( STOP_BITS_ONE << UART_STOP_BIT_NUM_S ) | ( DATA_BITS_EIGHT << UART_BIT_NUM_S ) );
	//clear rx and tx fifo,not ready
	SET_PERI_REG_MASK( UART_CONF0( UART1 ), UART_RXFIFO_RST | UART_TXFIFO_RST );    //RESET FIFO
	CLEAR_PERI_REG_MASK( UART_CONF0( UART1 ), UART_RXFIFO_RST | UART_TXFIFO_RST );
	WRITE_PERI_REG( UART_CONF1( UART1 ), ( ( (UART_RXFIFO_FULL_THRHD & 1) << UART_RXFIFO_FULL_THRHD_S ) ) |
										   ( (UART_TXFIFO_EMPTY_THRHD & 1) << UART_TXFIFO_EMPTY_THRHD_S ) );
	//clear all interrupt
	WRITE_PERI_REG( UART_INT_CLR( UART1 ), 0xFFFF );
}

void ICACHE_FLASH_ATTR uart1_set_tx_empty_cb( uart1_tx_empty_cb_t cb )
{
	uart1_tx_empty_cb = cb;
}
