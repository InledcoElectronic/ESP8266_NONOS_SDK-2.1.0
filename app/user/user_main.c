/*
 * user_main.c
 *
 *  Created on: 2017年9月2日
 *      Author: liruya
 */

#include "c_types.h"
#include "eagle_soc.h"
#include "espconn.h"
#include "ets_sys.h"
#include "gpio.h"

#include "../include/user_uart.h"
#include "mem.h"
#include "os_type.h"
#include "osapi.h"
#include "smartconfig.h"
#include "user_interface.h"
#include "driver/key.h"
#include "user_net.h"
#include "xlink/xlink_type.h"
#include "xlink/Xlink_Head_Adaptation.h"
#include "modbus.h"
#include "user_key.h"

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
 *******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set( void )
{
	enum flash_size_map size_map = system_get_flash_size_map();
	uint32 rf_cal_sec = 0;

	switch ( size_map )
	{
		case FLASH_SIZE_4M_MAP_256_256:
			rf_cal_sec = 128 - 5;
			break;

		case FLASH_SIZE_8M_MAP_512_512:
			rf_cal_sec = 256 - 5;
			break;

		case FLASH_SIZE_16M_MAP_512_512:
		case FLASH_SIZE_16M_MAP_1024_1024:
			rf_cal_sec = 512 - 5;
			break;

		case FLASH_SIZE_32M_MAP_512_512:
		case FLASH_SIZE_32M_MAP_1024_1024:
			rf_cal_sec = 1024 - 5;
			break;

		case FLASH_SIZE_64M_MAP_1024_1024:
			rf_cal_sec = 2048 - 5;
			break;
		case FLASH_SIZE_128M_MAP_1024_1024:
			rf_cal_sec = 4096 - 5;
			break;
		default :
			rf_cal_sec = 0;
			break;
	}

	return rf_cal_sec;
}

void ICACHE_FLASH_ATTR
user_rf_pre_init( void )
{
}

void ICACHE_FLASH_ATTR app_init( void )
{
	gpio_init();
	system_uart_swap();
	UART_SetPrintPort( UART1 );
	GPIO_DIS_OUTPUT( 0 );
	GPIO_DIS_OUTPUT( 13 );
	PIN_FUNC_SELECT( PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0 );
	PIN_FUNC_SELECT( PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4 );
	PIN_FUNC_SELECT( PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5 );
	PIN_FUNC_SELECT( PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12 );
	PIN_FUNC_SELECT( PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14 );
	GPIO_OUTPUT_SET( 12, 0 );
	GPIO_OUTPUT_SET( 14, 0 );
	GPIO_OUTPUT_SET( 16, 0 );
	GPIO_OUTPUT_SET( 4, 0 );

	modbus_init();
	user_key_init();
	uart0_set_rx_cb( modbus_receive );
	uart0_set_tx_empty_cb( NULL );
	uart1_set_tx_empty_cb( NULL );
	modbus_set_recv_frame_fn( user_net_send_pipe );
	user_tcp_set_recv_pipe_fn( modbus_send );
	user_udp_set_recv_pipe_fn( modbus_send );

	user_net_init();
}

void ICACHE_FLASH_ATTR user_init( void )
{
	system_init_done_cb( app_init );
}
