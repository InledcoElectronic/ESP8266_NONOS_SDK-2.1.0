/*
 * user_smartconfig.c
 *
 *  Created on: 2017年9月8日
 *      Author: liruya
 */

#include "user_smartconfig.h"

LOCAL bool smartconfig_status;
LOCAL uint32_t sc_timeout = 60000;
LOCAL os_timer_t sc_timer;

LOCAL void ICACHE_FLASH_ATTR user_smartconfig_done( sc_status status, void *pdata )
{
	switch ( status )
	{
		case SC_STATUS_WAIT:
			os_printf( "SC_STATUS_WAIT\n" );
			break;
		case SC_STATUS_FIND_CHANNEL:
			os_printf( "SC_STATUS_FIND_CHANNEL\n" );
			break;
		case SC_STATUS_GETTING_SSID_PSWD:
			os_printf( "SC_STATUS_GETTING_SSID_PSWD\n" );
			sc_type *type = pdata;
			if ( *type == SC_TYPE_ESPTOUCH )
			{
				os_printf( "SC_TYPE:SC_TYPE_ESPTOUCH\n" );
			}
			else
			{
				os_printf( "SC_TYPE:SC_TYPE_AIRKISS\n" );
			}
			break;
		case SC_STATUS_LINK:
			os_printf( "SC_STATUS_LINK\n" );
			struct station_config *sta_conf = pdata;

			wifi_station_set_config( sta_conf );
			wifi_station_disconnect();
			wifi_station_connect();
			break;
		case SC_STATUS_LINK_OVER:
			os_printf( "SC_STATUS_LINK_OVER\n" );
			if ( pdata != NULL )
			{
				//SC_TYPE_ESPTOUCH
				uint8 phone_ip[4] = { 0 };
				os_memcpy( phone_ip, (uint8*) pdata, 4 );
				os_printf( "Phone ip: %d.%d.%d.%d\n", phone_ip[0], phone_ip[1],
						phone_ip[2], phone_ip[3] );
			}
			else
			{
				//SC_TYPE_AIRKISS - support airkiss v2.0
			}
			os_timer_disarm( &sc_timer );
			smartconfig_stop();
			smartconfig_status = false;
			break;
	}
}

void ICACHE_FLASH_ATTR user_smartconfig_timeout_cb( void *arg )
{
	os_timer_disarm( &sc_timer );
	smartconfig_stop();
	smartconfig_status = false;
}

void ICACHE_FLASH_ATTR user_smartconfig_start( void )
{
	smartconfig_status = true;
	os_timer_disarm( &sc_timer );
	smartconfig_stop();
	wifi_set_opmode( STATION_MODE );
	smartconfig_set_type( SC_TYPE_ESPTOUCH );
	os_timer_setfn( &sc_timer, user_smartconfig_timeout_cb, NULL );
	os_timer_arm( &sc_timer, sc_timeout, 0 );
	smartconfig_start( user_smartconfig_done );
}

void ICACHE_FLASH_ATTR user_smartconfig_set_timeout( uint32_t timeout )
{
	sc_timeout = timeout;
}

bool ICACHE_FLASH_ATTR user_smartconfig_status( void )
{
	return smartconfig_status;
}
