/*
 * xlink_stm32f107_lwip.h
 *
 *  Created on: 2015骞�9鏈�6鏃�
 *      Author: XT800
 */

#ifndef XLINK_ALL_DEVICE_H_
#define XLINK_ALL_DEVICE_H_

#ifdef  __cplusplus
extern "C" {
#endif

#include "osapi.h"
#include "mem.h"
#include "user_interface.h"
#include "c_types.h"
#include "ip_addr.h"
#include "upgrade.h"
#include "espconn.h"

#define  XLINK_FUNC ICACHE_FLASH_ATTR

#define   xlink_strlen(x)       os_strlen((char*)(x))
#define   xlink_strcmp(x,t)     os_strcmp((char*)(x),(char*)(t))
#define   xlink_strncmp(x,t,z)  os_strncmp((char*)(x),(char*)(t),z)
#define   xlink_memset(x,d,t)   os_memset(x,d,t)
#define   xlink_memcpy(x,d,l)   os_memcpy(x,d,l)
#define   xlink_msleep(n)       xlink_delay(n);//us级
#define   xlink_sprintf         os_sprintf
#define   xlink_close(x)        xclose(x)
#define   xlink_printf          os_printf
#define   xlink_strchr          os_strchr

typedef unsigned int xsdk_time_t;

struct in_address {
	unsigned int s_addr;
};

typedef union {
	unsigned int ip;
	struct {
		unsigned char byte0 :8;
		unsigned char byte1 :8;
		unsigned char byte2 :8;
		unsigned char byte3 :8;
	}bit;
}ip_address_t;


struct sockaddr_in {
	void *data;
	unsigned short sin_port;
	struct in_address sin_addr;
};

typedef struct sockaddr_in xlink_addr;

XLINK_FUNC void xclose(int s);
extern XLINK_FUNC void xsdk_closeTCP(int flag);
#define dbg(x) airm2m_debug_printf("fun %s  line %d info %s\r\n",__FUNCTION__,__LINE__,(x))

//extern XLINK_FUNC void xclose(int s);
//extern void xlink_delay(xsdk_time_t x);

#ifdef  __cplusplus
}
#endif
#endif /* XLINK_ALL_DEVICE_H_ */
