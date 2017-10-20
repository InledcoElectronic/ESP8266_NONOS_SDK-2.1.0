/*
 * user_smartconfig.h
 *
 *  Created on: 2017年9月8日
 *      Author: liruya
 */

#ifndef _USER_SMARTCONFIG_H_
#define _USER_SMARTCONFIG_H_

#include "smartconfig.h"
#include "osapi.h"
#include "user_interface.h"
#include "c_types.h"

extern void user_smartconfig_set_timeout( uint32_t timeout );
extern bool user_smartconfig_status( void );
extern void user_smartconfig_start( void );

#endif /* _USER_SMARTCONFIG_H_ */
