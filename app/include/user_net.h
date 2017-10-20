/*
 * user_net.h
 *
 *  Created on: 2017年9月7日
 *      Author: liruya
 */

#ifndef _USER_NET_H_
#define _USER_NET_H_

#include "user_udp_server.h"
#include "user_tcp_client.h"
#include "xlink/xlink_type.h"
#include "xlink/xlink_All_Device.h"
#include "xlink/Xlink_Head_Adaptation.h"
#include "xlink/xlink_system.h"

#define XLINK_DOMAIN    	"cm2.xlink.cn"
#define	UDP_LOCAL_PORT		5987
#define	UDP_REMOTE_PORT		6000
#define	TCP_LOCAL_PORT		5001
#define	TCP_REMOTE_PORT		23778

#define SPI_FLASH_SECTOR_SIZE       4096
/*need  to define for the User Data section*/
#define PRIV_PARAM_START_SECTOR     0x7C  //need to change when compile in STEP4

#define	NET_BUFFER_SIZE	512

extern XLINK_USER_CONFIG user_config;

extern void user_net_init( void );
extern void user_net_send_pipe( uint8_t *pbuf, uint16_t len );

#endif /* APP_USER_USER_NET_H_ */
