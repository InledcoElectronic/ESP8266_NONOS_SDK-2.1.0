/*
 * user_udp_server.h
 *
 *  Created on: 2017年9月7日
 *      Author: liruya
 */

#ifndef _USER_UDP_SERVER_H_
#define _USER_UDP_SERVER_H_

#include "user_net.h"
#include "xlink/xlink_type.h"
#include "xlink/xlink_All_Device.h"

typedef void (*user_udp_recv_pipe_cb_t)( uint8_t *pbuf, uint16_t len );

extern void user_udp_server_init( void );
extern void user_udp_pipe_cb( unsigned char *buffer, unsigned int len, xlink_addr *addr );
extern void user_udp_recv_cb( void *arg, char *pdata, unsigned short len );
extern int user_udp_send( xlink_addr *addr, unsigned char *pdata, unsigned int len );
extern void user_udp_send_pipe( uint8_t *pbuf, uint16_t len );
extern void user_udp_set_recv_pipe_fn( user_udp_recv_pipe_cb_t callback );

#endif /* _USER_UDP_SERVER_H_ */
