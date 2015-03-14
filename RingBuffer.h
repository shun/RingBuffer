#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include "mytypedef.h"

class RingBuffer
{
/*
 * definition ----------------------------------
 */
/*
 * Method ----------------------------------
 */
public :
	RingBuffer() {}
	RingBuffer(uint32 size);
	~RingBuffer();
	int8 init(uint32 size);
	int8 write(uint32 size, uint8 * data);
	int8 read(uint8 *data);
	void clear();
	uint32 getWritePoint();
	uint32 getReadPoint();
	void dumpBuffer();
	
private :

	uint32 readBuffer(uint32 size, uint32 rp, uint8 *dat);
	uint32 writeBuffer(uint32 size, uint32 wp, uint8 * dat);
	int8 checkReadData();
	int8 checkWriteData(uint32 size);
	void setReadPoint(uint32 rp) {this->m_rp = rp;}
	void setWritePoint(uint32 wp) {this->m_wp = wp;}
/*
 * Member ----------------------------------
 */
private :

	uint32 m_wp;
	uint32 m_rp;
	uint32 m_size;
	uint8 *m_buffer;
};

#endif /* _RINGBUFFER_H_ */
