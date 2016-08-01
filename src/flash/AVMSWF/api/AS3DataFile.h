#ifndef		_AS3_DATAFILE_
#define		_AS3_DATAFILE_

#include "AS3DataIO.h"
#include "AS3DataMemory.h"

namespace avmshell
{
	class DataFile : public DataMemory, public DataInput, public DataOutput
	{
	public:
		//Domain* m_pDomain;
		void Clear()
		{
			DataMemory::Clear();
			m_filePointer=0;
		}
		DataFile(Toplevel *toplevel);
		virtual ~DataFile()
		{
			m_filePointer = 0; 
		}

		uint32_t	GetFilePointer() {return m_filePointer;}
		void		Seek(uint32_t filePointer) 
		{
			//if (0 <= m_filePointer && m_filePointer <= m_length)
			//{
			//	m_filePointer = filePointer;
			//}
			m_filePointer = filePointer;
		}

		void		SetLength(uint32_t newLength);
		uint32_t	BytesAvailable();

		void		Read(void *data, uint32_t count);
		void		Write(const void *data, uint32_t count);

	private:
		uint32_t m_filePointer;
	};
}

#endif
