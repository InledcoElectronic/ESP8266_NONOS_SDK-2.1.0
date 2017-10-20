/*
 * user_tcp_client.h
 *
 *  Created on: 2017年9月7日
 *      Author: liruya
 */

#ifndef _USER_TCP_CLIENT_H_
#define _USER_TCP_CLIENT_H_

#include "user_net.h"
#include "xlink/xlink_type.h"
#include "xlink/xlink_All_Device.h"

typedef void (*user_tcp_recv_pipe_cb_t)( uint8_t *pbuf, uint16_t len );

extern void user_tcp_status( XLINK_APP_STATUS status );
extern void user_tcp_pipe1_cb( unsigned char *buffer, unsigned int len, x_uint32 appid, x_uint8 *opt );
extern void user_tcp_pipe2_cb( unsigned char *buffer, unsigned int len, x_uint8 *opt );
extern int user_tcp_send( unsigned char *data, unsigned int datalen );
extern void user_tcp_send_pipe( uint8_t *pbuf, uint16_t len );
extern void user_tcp_func_process( void *arg );
extern void user_tcp_client_init( void );
extern void user_tcp_set_recv_pipe_fn( user_tcp_recv_pipe_cb_t callback );
extern void user_tcp_disconnect( void );

#endif /* _USER_TCP_CLIENT_H_ */
