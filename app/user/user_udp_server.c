/*
 * user_user_udp_server.c
 *
 *  Created on: 2017年9月7日
 *      Author: liruya
 */

#include "user_udp_server.h"
#include "driver/uart0.h"

LOCAL struct espconn user_udp_server;

LOCAL user_udp_recv_pipe_cb_t user_udp_recv_pipe_cb;

void ICACHE_FLASH_ATTR user_udp_server_init( void )
{
	user_udp_server.type = ESPCONN_UDP;
	user_udp_server.state = ESPCONN_NONE;
	user_udp_server.proto.udp = (esp_udp *) os_zalloc( sizeof( esp_udp ) );
	user_udp_server.reverse = NULL;
	user_udp_server.proto.udp->local_port = UDP_LOCAL_PORT;
	user_udp_server.proto.udp->remote_port = UDP_REMOTE_PORT;
	user_udp_server.proto.udp->remote_ip[0] = 0x00;
	user_udp_server.proto.udp->remote_ip[1] = 0x00;
	user_udp_server.proto.udp->remote_ip[2] = 0x00;
	user_udp_server.proto.udp->remote_ip[3] = 0x00;
	espconn_regist_recvcb( &user_udp_server, user_udp_recv_cb );
	espconn_create( &user_udp_server );
}

void ICACHE_FLASH_ATTR user_udp_pipe_cb( unsigned char *buffer, unsigned int len, xlink_addr *addr )
{
	if( user_udp_recv_pipe_cb != NULL )
	{
		user_udp_recv_pipe_cb( buffer, len );
	}
}

void ICACHE_FLASH_ATTR user_udp_set_recv_pipe_fn( user_udp_recv_pipe_cb_t callback )
{
	user_udp_recv_pipe_cb = callback;
}

void ICACHE_FLASH_ATTR user_udp_recv_cb( void *arg, char *pdata, unsigned short len )
{
	if( pdata == NULL || len == 0 )
	{
		return;
	}
	xlink_addr addr;
	ip_address_t ip_temp;
	ip_temp.ip = 0;
	remot_info *premote = NULL;
	if( espconn_get_connection_info( &user_udp_server, &premote, 0 ) != ESPCONN_OK )
	{
		return;
	}
	os_memcpy( user_udp_server.proto.udp->remote_ip, premote->remote_ip, 4 );
	user_udp_server.proto.udp->remote_port = premote->remote_port;

	ip_temp.bit.byte0 = user_udp_server.proto.udp->remote_ip[0];
	ip_temp.bit.byte1 = user_udp_server.proto.udp->remote_ip[1];
	ip_temp.bit.byte2 = user_udp_server.proto.udp->remote_ip[2];
	ip_temp.bit.byte3 = user_udp_server.proto.udp->remote_ip[3];
	xlink_memset( &addr, 0, sizeof( xlink_addr ) );
	addr.sin_addr.s_addr = ip_temp.ip;
	addr.sin_port = user_udp_server.proto.udp->remote_port;
	if( user_config.process_udp_data != NULL )
	{
		user_config.process_udp_data( pdata, len, &addr );
	}
}

int ICACHE_FLASH_ATTR user_udp_send( xlink_addr *addr, unsigned char *pdata, unsigned int len )
{
	ip_address_t ip_addr;
	ip_addr.ip = addr->sin_addr.s_addr;
	user_udp_server.proto.udp->remote_port = addr->sin_port;
	user_udp_server.proto.udp->remote_ip[0] = ip_addr.bit.byte0;
	user_udp_server.proto.udp->remote_ip[1] = ip_addr.bit.byte1;
	user_udp_server.proto.udp->remote_ip[2] = ip_addr.bit.byte2;
	user_udp_server.proto.udp->remote_ip[3] = ip_addr.bit.byte3;
	espconn_send( &user_udp_server, pdata, len );
	return len;
}

void ICACHE_FLASH_ATTR user_udp_send_pipe( uint8_t *pbuf, uint16_t len )
{
	xlink_addr addr;
	ip_address_t ip_temp;
	ip_temp.bit.byte0 = user_udp_server.proto.udp->remote_ip[0];
	ip_temp.bit.byte1 = user_udp_server.proto.udp->remote_ip[1];
	ip_temp.bit.byte2 = user_udp_server.proto.udp->remote_ip[2];
	ip_temp.bit.byte3 = user_udp_server.proto.udp->remote_ip[3];
	xlink_memset( &addr, 0, sizeof( xlink_addr ) );
	addr.sin_addr.s_addr = ip_temp.ip;
	addr.sin_port = user_udp_server.proto.udp->remote_port;
	XlinkSendUdpPipe( pbuf, len, &addr );
}
