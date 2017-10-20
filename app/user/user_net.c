/*
 * user_net.c
 *
 *  Created on: 2017年9月7日
 *      Author: liruya
 */

#include "user_net.h"
#include "espconn.h"

LOCAL uint8_t auth_pwd_name_buf[128];
XLINK_USER_CONFIG user_config;
LOCAL os_timer_t xlink_loop_timer;

LOCAL int ICACHE_FLASH_ATTR user_write_config( char *data,unsigned int datalen )
{
	xlink_memcpy( auth_pwd_name_buf, data, datalen );
	spi_flash_erase_sector( PRIV_PARAM_START_SECTOR );
	spi_flash_write( PRIV_PARAM_START_SECTOR*4096, (uint32 *) auth_pwd_name_buf, sizeof( auth_pwd_name_buf ) );
	return datalen;
}

LOCAL int ICACHE_FLASH_ATTR user_read_config( char *Buffer,unsigned int bufferlen )
{
	spi_flash_read( PRIV_PARAM_START_SECTOR*4096, (uint32 *)auth_pwd_name_buf, sizeof( auth_pwd_name_buf ) );
	os_memcpy( Buffer, auth_pwd_name_buf, bufferlen );
	return bufferlen;
}

void ICACHE_FLASH_ATTR user_net_send_pipe( uint8_t *pbuf, uint16_t len )
{
	user_tcp_send_pipe( pbuf, len );
	user_udp_send_pipe( pbuf, len );
}

void ICACHE_FLASH_ATTR user_net_init( void )
{
	user_udp_server_init();
	user_tcp_client_init();
	user_config.maclen = 6;
	user_config.in_internet = 1;
	user_config.wifisoftVersion = 1;
	user_config.wifi_type = STATION_MODE;
	user_config.pipetype = 1;
	user_config.devicetype = 0;

	user_config.send_tcp = user_tcp_send;
	user_config.send_udp = user_udp_send;
	user_config.tcp_pipe = user_tcp_pipe1_cb;
	user_config.tcp_pipe2 = NULL;
	user_config.udp_pipe = user_udp_pipe_cb;
	user_config.status = user_tcp_status;
	user_config.writeConfig = user_write_config;
	user_config.readConfig = user_read_config;
	user_config.DebugPrintf = NULL;
	user_config.server_time = NULL;
	wifi_get_macaddr( STATION_IF, user_config.mac );
	if( XlinkSystemInit( PRODUCT_ID, PRODUCT_KEY, &user_config ) )
	{

	}
	if( XlinkGetACK() == 0 )
	{
		XlinkSetACK( 88888888 );
	}
	XlinkSystemSetDeviceName( "Test" );
	XlinkPorcess_UDP_Enable();
	if( user_config.setServerStatus != NULL )
	{
		user_config.setServerStatus( 0, 0 );
	}
	XlinkSystemSetWifiStatus( 1 );
	os_timer_disarm( &xlink_loop_timer );
	os_timer_setfn( &xlink_loop_timer, user_tcp_func_process, NULL );
	os_timer_arm( &xlink_loop_timer, 100, 1 );
}
