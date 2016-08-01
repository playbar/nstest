#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "AS3SocketFile.h"

namespace avmshell
{

SocketFile::SocketFile(Toplevel *pToplevel):
DataInput(pToplevel), DataOutput(pToplevel), DataIOBase(pToplevel)
{
	m_inReadPos = m_inWritePos = 0;
}

uint32_t SocketFile::InCacheBytesAvailable()
{
	//if(m_inWritePos>=m_inCache.GetLength())
	//	int v=0;
	AvmAssert(m_inWritePos<=m_inCache.GetLength());
	return (m_inWritePos <= m_inReadPos) ? 0 : (m_inWritePos - m_inReadPos);
}

uint32_t SocketFile::OutCacheBytesAvailable()
{
	return (m_outWritePos <= m_outReadPos) ? 0 : (m_outWritePos - m_outReadPos);
}

void SocketFile::ReadInCache(void *data, uint32_t count)
{
	CheckEOF(count);

	if (count > 0)
	{
		AvmAssert(m_inReadPos+count<=m_inCache.GetLength());
		VMPI_memcpy(data, m_inCache.GetBuffer() + m_inReadPos, count);
		m_inReadPos += count;
	}

	//if (0 == InCacheBytesAvailable())
	//{
	//	m_inReadPos = m_inWritePos = 0;
	//}
}

void SocketFile::WriteInCache(const void *data, uint32_t length)
{
	if (m_inWritePos + length >= m_inCache.GetLength())
	{
		m_inCache.SetLength(m_inWritePos + length);
		//		m_outCache.Grow(m_outCache.GetLength());
	}

	VMPI_memcpy(m_inCache.GetBuffer() + m_inWritePos, data, length);
	m_inWritePos += length;
}

void SocketFile::ReadOutCache(void *data, uint32_t length)
{
	CheckEOF(length);

	if (length > 0)
	{
		VMPI_memcpy(data, m_outCache.GetBuffer() + m_outReadPos, length);
		m_outReadPos += length;
	}

	//if (0 == OutCacheBytesAvailable())
	//{
	//	m_outReadPos = m_outWritePos = 0;
	//}
}

void SocketFile::WriteOutCache(const void *data, uint32_t count)
{
	if (m_outWritePos + count >= m_outCache.GetLength())
	{
		m_outCache.SetLength(m_outWritePos + count);
//		m_outCache.Grow(m_outCache.GetLength());
	}

	VMPI_memcpy(m_outCache.GetBuffer() + m_outWritePos, data, count);
	m_outWritePos += count;
}

void SocketFile::SetInCacheLength(uint32_t newLength)
{
	m_inCache.SetLength(newLength);

	if (m_inReadPos > newLength)
	{
		m_inReadPos = newLength;
	}
	if (m_inWritePos > newLength)
	{
		m_inWritePos = newLength;
	}
}

void SocketFile::SetOutCacheLength(uint32_t newLength)
{
	m_outCache.SetLength(newLength);

	if (m_outWritePos > newLength)
	{
		m_outWritePos = newLength;
	}
	if (m_outReadPos > newLength)
	{
		m_outReadPos = newLength;
	}
}

}

