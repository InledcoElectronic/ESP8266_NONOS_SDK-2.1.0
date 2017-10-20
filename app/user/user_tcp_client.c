/*
 * user_tcp_client.c
 *
 *  Created on: 2017年9月7日
 *      Author: liruya
 */

#include "user_tcp_client.h"

#include "../include/user_uart.h"

LOCAL struct espconn user_tcp_client;

LOCAL uint8_t reconn_cnt;
LOCAL bool isConnectServer;
LOCAL bool flag_dns_discovery;
LOCAL bool isDnsFound;
ip_addr_t remote_ip;
os_timer_t dns_timer;

LOCAL user_tcp_recv_pipe_cb_t user_tcp_recv_pipe_cb;

void ICACHE_FLASH_ATTR user_tcp_reconnect( void );

void ICACHE_FLASH_ATTR user_tcp_status( XLINK_APP_STATUS status )
{
	switch( status )
	{
		case XLINK_WIFI_STA_APP_CONNECT:

			break;

		case XLINK_WIFI_STA_APP_DISCONNECT:

			break;

		case XLINK_WIFI_STA_APP_TIMEOUT:

			break;

		case XLINK_WIFI_STA_CONNECT_SERVER:
#ifdef	USER_DEBUG_ON
			os_printf( "connect server..." );
#endif
			break;

		case XLINK_WIFI_STA_DISCONNCT_SERVER:
			isConnectServer = false;
			user_tcp_reconnect();
#ifdef	USER_DEBUG_ON
			os_printf( "disconnect server..." );
#endif
			break;

		case XLINK_WIFI_STA_LOGIN_SUCCESS:
			isConnectServer = true;
#ifdef	USER_DEBUG_ON
			os_printf( "login server..." );
#endif
			break;

		default:

			break;
	}
}

LOCAL void ICACHE_FLASH_ATTR user_tcp_dns_found( const char *name, ip_addr_t *ip, void *arg )
{
	struct espconn *pespconn = (struct espconn *) arg;
	reconn_cnt = 0;
	isConnectServer = false;
	flag_dns_discovery = false;
	if( ip == NULL )
	{
		return;
	}
	if( ip->addr == 0 )
	{
		return;
	}
#ifdef	USER_DEBUG_ON
	os_printf( "dns found..." );
#endif
	remote_ip.addr = ip->addr;
	os_timer_disarm( &dns_timer );
	isDnsFound = true;
	os_memcpy( user_tcp_client.proto.tcp->remote_ip, &ip->addr, 4 );
	espconn_connect( &user_tcp_client );
}

LOCAL void ICACHE_FLASH_ATTR user_tcp_dns_check_cb( void *arg )
{
	struct espconn *pespconn = (struct espconn *) arg;
	espconn_gethostbyname( pespconn, XLINK_DOMAIN, &remote_ip, user_tcp_dns_found );
}

LOCAL void ICACHE_FLASH_ATTR user_tcp_start_dns( struct espconn *pespconn )
{
	remote_ip.addr = 0;

	os_timer_disarm( &dns_timer );
	os_timer_setfn( &dns_timer, user_tcp_dns_check_cb, pespconn );
	os_timer_arm( &dns_timer, 1000, 1 );
#ifdef	USER_DEBUG_ON
	os_printf( "start dns..." );
#endif
}

void ICACHE_FLASH_ATTR user_tcp_func_process( void *arg )
{
	uint32_t ctime = system_get_time()/1000000;
	uint8_t mode = wifi_get_opmode();
	if( mode == STATION_MODE )
	{
		uint8_t status = wifi_station_get_connect_status();
		struct ip_info ipinfo;
		wifi_get_ip_info( STATION_IF, &ipinfo );
		if( status == STATION_GOT_IP && ipinfo.ip.addr != 0 )
		{
			if( flag_dns_discovery == false && isDnsFound == false )
			{
				flag_dns_discovery = true;
				reconn_cnt = 0;
				user_tcp_start_dns( &user_tcp_client );
			}
			if( isConnectServer )
			{
//				XlinkGetServerTime();
			}
			XlinkSystemLoop( ctime, 0 );
			XlinkSystemTcpLoop();
		}
	}
	else
	{
		reconn_cnt = 0;
		isConnectServer = false;
		flag_dns_discovery = false;
		isDnsFound = false;
	}
}

void ICACHE_FLASH_ATTR user_tcp_set_recv_pipe_fn( user_tcp_recv_pipe_cb_t callback )
{
	user_tcp_recv_pipe_cb = callback;
}

void ICACHE_FLASH_ATTR user_tcp_pipe1_cb( unsigned char *buffer,
		unsigned int len, x_uint32 appid, x_uint8 *opt )
{
	if( user_tcp_recv_pipe_cb != NULL )
	{
		user_tcp_recv_pipe_cb( buffer, len );
	}
}

void ICACHE_FLASH_ATTR user_tcp_pipe2_cb( unsigned char *buffer,
		unsigned int len, x_uint8 *opt )
{

}

LOCAL void ICACHE_FLASH_ATTR user_tcp_connect_cb( void *arg )
{
#ifdef	USER_DEBUG_ON
	os_printf( "tcp connected..." );
#endif
	if ( user_config.setServerStatus != NULL )
	{
		user_config.setServerStatus( 1, 0 );
	}
	reconn_cnt = 0;
}

LOCAL void ICACHE_FLASH_ATTR user_tcp_disconnect_cb( void *arg )
{
#ifdef	USER_DEBUG_ON
	os_printf( "tcp disconnected..." );
#endif
	if ( user_config.setServerStatus != NULL )
	{
		user_config.setServerStatus( 0, 0 );
	}
	xsdk_closeTCP( 1 );
	user_tcp_reconnect();
}

LOCAL void ICACHE_FLASH_ATTR user_tcp_reconnect_cb( void *arg, sint8 errtype )
{
#ifdef	USER_DEBUG_ON
	os_printf( "tcp disconnected with error: %d", errtype );
#endif
	user_tcp_reconnect();
}

LOCAL void ICACHE_FLASH_ATTR user_tcp_recv_cb( void *arg, char *buf,
		unsigned short len )
{
	if ( buf == NULL || len == 0 )
	{
		return;
	}
	user_config.process_tcp_data( buf, len, 0 );
}

LOCAL void ICACHE_FLASH_ATTR user_tcp_sent_cb( void *arg )
{

}

int ICACHE_FLASH_ATTR user_tcp_send( unsigned char *data, unsigned int datalen )
{
	espconn_send( &user_tcp_client, data, datalen );
	return datalen;
}

void ICACHE_FLASH_ATTR user_tcp_send_pipe( uint8_t *pbuf, uint16_t len )
{
	if( isConnectServer )
	{
		XlinkSendTcpPipe2( pbuf, len );
	}
}

void ICACHE_FLASH_ATTR user_tcp_reconnect( void )
{
	if( user_smartconfig_status() )
	{
		reconn_cnt = 0;
		return;
	}
	reconn_cnt++;
	espconn_disconnect( &user_tcp_client );
	if( reconn_cnt > 5 )
	{
		reconn_cnt = 0;
		isDnsFound = false;
		isConnectServer = false;
		flag_dns_discovery = false;
	}
	else
	{
		espconn_connect( &user_tcp_client );
	}
}

void ICACHE_FLASH_ATTR user_tcp_disconnect( void )
{
	espconn_disconnect( &user_tcp_client );
	xsdk_closeTCP( 1 );
}

void ICACHE_FLASH_ATTR user_tcp_client_init( void )
{
	if ( user_tcp_client.proto.tcp == NULL )
	{
		user_tcp_client.proto.tcp = (esp_tcp *) os_zalloc( sizeof(esp_tcp) );
	}

	user_tcp_client.type = ESPCONN_TCP;
	user_tcp_client.state = ESPCONN_NONE;
	user_tcp_client.proto.tcp->local_port = TCP_LOCAL_PORT;
	user_tcp_client.proto.tcp->remote_port = TCP_REMOTE_PORT;
	espconn_regist_connectcb( &user_tcp_client, user_tcp_connect_cb );
	espconn_regist_disconcb( &user_tcp_client, user_tcp_disconnect_cb );
	espconn_regist_reconcb( &user_tcp_client, user_tcp_reconnect_cb );
	espconn_regist_recvcb( &user_tcp_client, user_tcp_recv_cb );
	espconn_regist_sentcb( &user_tcp_client, user_tcp_sent_cb );
}
