/*
 * user_key.h
 *
 *  Created on: 2017年9月11日
 *      Author: liruya
 */

#ifndef USER_KEY_H_
#define USER_KEY_H_

#include "driver/key.h"
#include "gpio.h"
#include "os_type.h"
#include "osapi.h"

#define	USER_KEY_NUM					1

#define	USER_KEY_SMARTCONFIG_IO_MUX		PERIPHS_IO_MUX_GPIO0_U
#define	USER_KEY_SMARTCONFIG_IO_NUM		0
#define	USER_KEY_SMARTCONFIG_FUNC		FUNC_GPIO0

extern void user_key_init( void );

#endif /* USER_KEY_H_ */
