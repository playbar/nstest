#ifndef		_AS3_SOCKETFILE_H_
#define		_AS3_SOCKETFILE_H_

#include "AS3DataIO.h"
#include "AS3DataMemory.h"

//#include "AS3Socket.h"

namespace avmshell
{
	class SocketObject;

	class SocketFile : public DataInput, public DataOutput
	{
	friend class SocketObject;

	public:
		SocketFile(Toplevel *pToplevel);
		virtual ~SocketFile()
		{
			m_inReadPos = m_inWritePos = 0;
			m_outReadPos = m_outWritePos = 0;
		}
		XU8* GetOutput(int&l)
		{
			l=m_outWritePos-m_outReadPos;
			//m_outWritePos+=l;
			return m_outCache.GetBuffer()+m_outReadPos;
		}
		XU8* GetInput(int l)
		{
			ClearInput();
			m_inCache.SetLength(m_inWritePos+l);
			XU8*pData=m_inCache.GetBuffer()+m_inWritePos;
			m_inWritePos+=l;
			return pData;
		}

		void		Read(void *data, uint32_t count) { ReadInCache(data, count); }
		void		Write(const void *data, uint32_t count) { WriteOutCache(data, count); }

		void		ReadInCache(void *data, uint32_t count);
		void		WriteInCache(const void *data, uint32_t length);
		void		ReadOutCache(void *data, uint32_t length);
		void		WriteOutCache(const void *data, uint32_t count);

		uint32_t	GetOutCacheReadPointer() {return m_outReadPos;}//{return m_pConnect->m_inPos;}
		void		SeekOutCacheReadPointer(uint32_t dataPointer) { m_outReadPos = dataPointer; }//{m_pConnect->m_inPos = dataPointer;}

		uint32_t	GetOutCacheWritePointer() {return m_outWritePos;}//{return m_pConnect->m_inPos;}
		void		SeekOutCacheWritePointer(uint32_t dataPointer) { m_outWritePos = dataPointer; }//{m_pConnect->m_inPos = dataPointer;}

		uint32_t	GetInCacheReadPoint()	{ return m_inReadPos; }
		void		SeekInCacheReadPoint(uint32_t dataPoint)	{ m_inReadPos = dataPoint; }

		uint32_t	GetInCacheWritePoint()	{ return m_inWritePos; }
		void		SeekInCacheWritePoint(uint32_t dataPoint)	{ m_inWritePos = dataPoint; }

		void		SetInCacheLength(uint32_t length);
		void		SetOutCacheLength(uint32_t length);

		uint32_t	InCacheBytesAvailable();
		uint32_t	OutCacheBytesAvailable();

		uint32_t	BytesAvailable(){ return InCacheBytesAvailable(); }

		DataMemory	*GetInCache() { return &m_inCache; }
		DataMemory	*GetOutCache() { return &m_outCache; }
		void		ClearData(uint32_t&read,uint32_t&write,XU8*pData)
		{
			//return;
			if(read>20480&&write-read<read)
			{				
				if(write>read)
					VMPI_memcpy(pData,pData+read,write-read);
				write-=read;
				read=0;
			}
			
		}
		inline void ClearOutput()
		{
			ClearData(m_outReadPos,m_outWritePos,m_outCache.GetBuffer());
		}
		inline void ClearInput()
		{
			ClearData(m_inReadPos,m_inWritePos,m_inCache.GetBuffer());
		}
		inline void ClearAll()
		{
			ClearInput();
			ClearOutput();
		}
		void Reset()
		{
			m_inWritePos=0;
			m_inReadPos=0;
			m_outReadPos=0;
			m_outWritePos=0;
		}
		void ResetOutput()
		{
			m_outReadPos=0;
			m_outWritePos=0;
		}
	private:
		DataMemory	m_inCache;
		DataMemory	m_outCache;
		uint32_t	m_inWritePos, m_inReadPos;
		uint32_t	m_outReadPos, m_outWritePos;
	};
}

#endif
