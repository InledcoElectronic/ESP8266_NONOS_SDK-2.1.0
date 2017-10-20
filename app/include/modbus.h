/*
 * modbus.h
 *
 *  Created on: 2017年9月11日
 *      Author: liruya
 */

#ifndef MODBUS_H_
#define MODBUS_H_

#include "c_types.h"
#include "osapi.h"
#include "mem.h"
#include "driver/uart0.h"

#define	MODBUS_BROADCAST_ADDRESS		0x00

#define	CMD_READ_COIL			0x01
#define	CMD_READ_STATUS			0x02
#define	CMD_READ_HOLD			0x03
#define	CMD_READ_INPUT			0x04
#define	CMD_WRITE_SINGLE_COIL	0x05
#define	CMD_WRITE_SINGLE_HOLD	0x06
#define	CMD_WRITE_MULTI_COIL	0x0F
#define	CMD_WRITE_MULTI_HOLD	0x10
//#define	CMD_READ_WRITE_HOLD		0x17
#define	CMD_READ_ALL			0x41
#define	CMD_ERROR_MASK			0x80

#define	ERROR_INVALID_FUNCTION	0x01
#define	ERROR_INVALID_ADDRESS	0x02
#define	ERROR_INVALID_DATA		0x03
#define	ERROR_SLAVE_FAULT		0x04

#define	COIL_ON					0xFF00
#define	COIL_OFF				0x0000

typedef struct{
	bool *coil_reg;
	bool *status_reg;
	uint16_t *hold_reg;
	uint16_t *input_reg;
	const uint16_t coil_reg_cnt;
	const uint16_t status_reg_cnt;
	const uint16_t hold_reg_t;
	const uint16_t input_reg_t;
}modbus_reg_t;

typedef void (* modbus_recv_frame_cb_t)( uint8_t *pbuf, uint16_t len );

extern void ICACHE_FLASH_ATTR modbus_init( void );
extern void modbus_crc16_init( void );
extern uint16_t modbus_crc16_once( uint8_t byte );
extern uint16_t modbus_crc16( uint8_t *buf, uint32_t len );
extern uint16_t modbus_checksum( void );
extern void modbus_send( uint8_t *pbuf, uint16_t len );
extern void modbus_receive( rx_buffer_t *pbuf );
extern void modbus_set_recv_frame_fn( modbus_recv_frame_cb_t callback );

#endif /* MODBUS_H_ */
