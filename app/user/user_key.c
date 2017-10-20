/*
 * user_key.c
 *
 *  Created on: 2017年9月11日
 *      Author: liruya
 */

#include "user_key.h"
#include "user_config.h"
#include "user_smartconfig.h"
#include "user_tcp_client.h"
#include "osapi.h"

struct single_key_param *key[USER_KEY_NUM];
struct keys_param key_list;
LOCAL os_timer_t tmr;

LOCAL void ICACHE_FLASH_ATTR user_restart( void *arg )
{
	system_restart();
}

LOCAL void ICACHE_FLASH_ATTR user_key_short_press( void )
{
	user_tcp_disconnect();
	user_smartconfig_start();
}

LOCAL void ICACHE_FLASH_ATTR user_key_long_press( void )
{
	wifi_station_disconnect();
	XlinkReSetSDK();
	system_restore();
	wifi_set_opmode( SOFTAP_MODE );
	os_timer_disarm( &tmr );
	os_timer_setfn( &tmr, user_restart, NULL );
	os_timer_arm( &tmr, 100, 0 );
//	system_restart();
}

void ICACHE_FLASH_ATTR user_key_init( void )
{
	key[0] = key_init_single( USER_KEY_SMARTCONFIG_IO_NUM,
							  USER_KEY_SMARTCONFIG_IO_MUX,
							  USER_KEY_SMARTCONFIG_FUNC,
							  user_key_long_press,
							  user_key_short_press );
	key_list.key_num = USER_KEY_NUM;
	key_list.single_key = key;
	key_init( &key_list );
}

