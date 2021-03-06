#include <stdio.h>
#include <stdlib.h>
#include "RingBuffer.h"

uint32 g_wcounter = 0;
uint32 g_rcounter = 0;
RingBuffer::RingBuffer(uint32 size)
{
	init(size);
}

RingBuffer::~RingBuffer()
{
	if (NULL == this->m_buffer)
	{
		return;
	}

	free(this->m_buffer);
	this->m_buffer = NULL;
}

int8 RingBuffer::init(uint32 size)
{
	uint8 * tmp = NULL;

	tmp = (uint8 *)calloc(1, size);
	if (NULL == tmp)
	{
		// throw MemoryException;
		return -1;
	}
	
	this->m_size = size;
	this->m_buffer = tmp;
	clear();

	return 0;
}

void RingBuffer::clear()
{
	setWritePoint(0);
	setReadPoint(0);
}

uint32 RingBuffer::getWritePoint()
{
	return this->m_wp;
}

uint32 RingBuffer::getReadPoint()
{
	return this->m_rp;
}

uint32 RingBuffer::writeBuffer(uint32 size, uint32 wp, uint8 * dat)
{
	uint32 idx = 0;
	uint32 tmpwp = wp;

	for (idx = 0; idx < size; idx++)
	{
		this->m_buffer[tmpwp] = dat[idx];
		if (this->m_size < (tmpwp + 1))
		{
			tmpwp = 0;
		}
		tmpwp = (++tmpwp) % this->m_size;
	}

	return tmpwp;
}

int8 RingBuffer::write(uint32 size, uint8 *dat)
{
	uint32 tmpwp = 0;
	uint8 ret = 0;
	
	if (g_wcounter > 9700)
	{
		ret = 0;
	}
	if (g_wcounter * 120 > this->m_size)
	{
		ret = 0;
	}
	if (checkWriteData(size + sizeof(size)) < 0)
	{
		int *i = NULL;
		*i = 100;
		return -1;
	}
	tmpwp = writeBuffer(sizeof(size), this->m_wp, (uint8 *)&size);
	tmpwp = writeBuffer(size, tmpwp, dat);

	setWritePoint(tmpwp);
	g_wcounter++;
	return ret;
}

int8 RingBuffer::checkWriteData(uint32 size)
{
	uint32 freespace;

	if (m_wp == m_rp)
	{
		freespace = this->m_size;
	}
	else if (m_wp > m_rp)
	{
		freespace = this->m_size - m_wp + m_rp;
	}
	else if (m_wp < m_rp)
	{
		freespace = m_rp - m_wp;
	}
	else
	{
		return -1;
	}

	if (freespace < size)
	{
		return -1;
	}

	return 0;
}

int8 RingBuffer::checkReadData()
{
	uint32 writespace = 0;
	uint8 ret = 0;
	if (m_wp == m_rp)
	{
		ret = -1;
	}
	else if (m_wp > m_rp)
	{
		writespace = m_wp - m_rp;
	}
	else if (m_rp > m_wp)
	{
		writespace = this->m_size - (m_rp - m_wp);
	}

	if (writespace <= 0)
	{
		ret = -1;
	}
	return ret;
}

uint32 RingBuffer::readBuffer(uint32 size, uint32 rp, uint8 *dat)
{
	uint32 idx = 0;
	uint32 tmprp = rp;

	for (idx = 0; idx < size; idx++)
	{
		dat[idx] = this->m_buffer[tmprp];
		tmprp = (++tmprp) % this->m_size;
	}

	return tmprp;
}

int8 RingBuffer::read(uint8 *dat)
{
	uint32 bufsize = 0;
	uint32 tmprp = 0;

	if (g_wcounter <= (g_rcounter+1))
	{
		tmprp = 0;
	}
	if (checkReadData() < 0)
	{
		return -1;
	}
	g_rcounter++;
	tmprp = readBuffer(sizeof(bufsize), this->m_rp, (uint8 *)&bufsize);
	tmprp = readBuffer(bufsize, tmprp, dat);
	setReadPoint(tmprp);

	return 0;
}

void RingBuffer::dumpBuffer()
{
	uint32 i = 0;

	for (i = 0; i < this->m_size; i++)
	{
		printf("%02x, ", this->m_buffer[i]);
	}

	printf("\n\n");
}
