#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "AS3DataFile.h"

namespace avmshell
{
	DataFile::DataFile(Toplevel *toplevel):
	DataInput(toplevel), DataOutput(toplevel), DataIOBase(toplevel)
	{
		m_filePointer = 0;
		//m_pDomain = NULL;
	}

	void DataFile::SetLength(uint32_t newLength)
	{
		if(this->GetLength()==newLength) return;

		DataMemory::SetLength(newLength);

		if (m_filePointer > newLength)
		{
			m_filePointer = newLength;
		}
	}

	uint32_t DataFile::BytesAvailable()
	{
		return (m_filePointer > m_length) ? 0 : (m_length - m_filePointer);
	}

	void DataFile::Read(void *data, uint32_t count)
	{
		CheckEOF(count);

		if (count > 0)
		{
			VMPI_memcpy(data, m_array + m_filePointer, count);
			m_filePointer += count;
		}
	}

	void DataFile::Write(const void *data, uint32_t count)
	{
		if (m_filePointer + count >= m_length)
		{
			int l = m_filePointer + count;
			Grow(l);
			m_length=l;
		}

		VMPI_memcpy(m_array + m_filePointer, data, count);
		m_filePointer += count;
	}
}

