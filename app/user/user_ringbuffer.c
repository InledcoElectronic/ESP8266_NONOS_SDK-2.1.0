/*
 * user_ringbuffer.c
 *
 *  Created on: 2017年10月30日
 *      Author: liruya
 */

#include "c_types.h"
#include "osapi.h"
#include "mem.h"
#include "user_ringbuffer.h"

void ICACHE_FLASH_ATTR user_rb_init( ringbuf_t *p_rb, uint8_t *pbuf, uint32_t size )
{
	if( p_rb == NULL || pbuf == NULL || size == 0 )
	{
		return;
	}
	p_rb->pbuf = pbuf;
	p_rb->size = size;
	p_rb->head = 0;
	p_rb->tail = 0;
	p_rb->lock = false;
	memset( p_rb->pbuf, 0, p_rb->size );
}

void ICACHE_FLASH_ATTR user_rb_lock( ringbuf_t *p_rb )
{
	uint32_t i = 0;
	while( p_rb->lock )
	{
		i++;
		if( i > 50000 )
		{
			break;
		}
	}
	p_rb->lock = true;
}

void ICACHE_FLASH_ATTR user_rb_unlock( ringbuf_t *p_rb )
{
	p_rb->lock = false;
}

uint32_t ICACHE_FLASH_ATTR user_rb_unread_size( ringbuf_t *p_rb )
{
	if( p_rb->head >= p_rb->tail )
	{
		return p_rb->head - p_rb->tail;
	}
	return p_rb->size + p_rb->head - p_rb->tail;
}

void ICACHE_FLASH_ATTR user_rb_reset( ringbuf_t *p_rb )
{
	p_rb->head = 0;
	p_rb->tail = 0;
	memset( p_rb->pbuf, 0, p_rb->size );
}

uint32_t ICACHE_FLASH_ATTR user_rb_put( ringbuf_t *p_rb, uint8_t *src, uint32_t len )
{
	uint32_t sz;
	uint32_t rz;
	if( p_rb->head >= p_rb->tail )
	{
		sz = p_rb->size + p_rb->tail - p_rb->head;
		rz = p_rb->size - p_rb->head;
		if( sz > len )
		{
			sz = len;
		}
		if( rz >= sz)
		{
			memcpy( p_rb->pbuf + p_rb->head, src, sz );
		}
		else
		{
			memcpy( p_rb->pbuf + p_rb->head, src, rz );
			memcpy( p_rb->pbuf, src + rz, sz - rz );
		}
	}
	else
	{
		sz = p_rb->tail - p_rb->head;
		if( sz > len )
		{
			sz = len;
		}
		memcpy( p_rb->pbuf + p_rb->head, src, sz );
	}
	p_rb->head += sz;
	if( p_rb->head >= p_rb->size )
	{
		p_rb->head -= p_rb->size;
	}
	return sz;
}

uint32_t ICACHE_FLASH_ATTR user_rb_get( ringbuf_t *p_rb, uint8_t *des, uint32_t len )
{
	uint32_t sz;
	uint32_t rz;
	if( p_rb->head >= p_rb->tail )
	{
		sz = p_rb->head - p_rb->tail;
		if( sz > len )
		{
			sz = len;
		}
		memcpy( des, p_rb->pbuf + p_rb->tail, sz );
	}
	else
	{
		sz = p_rb->size + p_rb->head - p_rb->tail;
		rz = p_rb->size - p_rb->tail;
		if( sz > len )
		{
			sz = len;
		}
		if( rz >= sz )
		{
			memcpy( des, p_rb->pbuf + p_rb->tail, sz );
		}
		else
		{
			memcpy( des, p_rb->pbuf + p_rb->tail, rz );
			memcpy( des + rz, p_rb->pbuf, sz - rz );
		}
	}
	p_rb->tail += sz;
	if( p_rb->tail >= p_rb->size )
	{
		p_rb->tail -= p_rb->size;
	}
	return sz;
}
