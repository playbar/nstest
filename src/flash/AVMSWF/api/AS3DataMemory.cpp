#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "AS3DataMemory.h"

namespace avmshell
{
	DataMemory::DataMemory()
	{
		m_subscriberRoot = NULL;
		m_array    = NULL;
		m_capacity = 0;
		m_length   = 0;
		m_bAttach  = false;
	}

	DataMemory::DataMemory(const avmshell::DataMemory &lhs) : GlobalMemoryProvider()
	{
		m_subscriberRoot = NULL;

		//if (m_array)
		//{
		//	mmfx_delete_array(m_array);
		//	m_array = NULL;
		//}			//Additions
		m_array = mmfx_new_array(uint8_t, lhs.m_length);
		if (!m_array)
		{
//			m_pcToplevel->throwError(kOutOfMemoryError);
			ThrowMemoryError();
			return;
		}

		m_capacity	= lhs.m_capacity;
		m_length	= lhs.m_length;
		m_bAttach	= false;
		//m_pcToplevel = lhs.m_pcToplevel;
		VMPI_memcpy(m_array, lhs.m_array, m_length);
	}

	void DataMemory::Clear()
	{
		if(m_array)
		{
			if(!m_bAttach)
				mmfx_delete_array(m_array);
			m_array=NULL;
		}
		m_capacity = m_length=0;
	}

	DataMemory::~DataMemory()
	{
		m_subscriberRoot = NULL;
//		m_pcToplevel = NULL;

		if (m_array)
		{
			if(!m_bAttach)
				mmfx_delete_array(m_array);
			m_array = NULL;
		}
		
		m_capacity = m_length = 0;
	}

	bool DataMemory::Grow(uint32_t minimumCapacity)
	{
		if (minimumCapacity > m_capacity)
		{
			uint32_t newCapacity = m_capacity << 1;			
			if (newCapacity < minimumCapacity)
			{
				newCapacity = minimumCapacity;
			}
			if (newCapacity < kGrowthIncrement) 
			{
				newCapacity = kGrowthIncrement;
			}

			uint8_t *newArray = mmfx_new_array(uint8_t, newCapacity);
			if (!newArray)
			{
				return false;
			}

			if (m_array)
			{
				VMPI_memcpy(newArray, m_array, m_length);
				if(!m_bAttach)
					mmfx_delete_array(m_array);
			}
			VMPI_memset(newArray + m_length, 0, newCapacity - m_capacity);
			m_array = newArray;

			m_capacity = newCapacity;

			NotifySubscribers();
			m_bAttach=false;
		}

		return true;
	}

	uint8_t DataMemory::operator[] (uint32_t index) const
	{
		if (m_length <= index)
		{
			return 0;
			//Throw error?
		}
		return m_array[index];
	}

	uint8_t& DataMemory::operator[] (uint32_t index)
	{
		if (m_length <= index)
		{
			Grow(index + 1);
			m_length = index + 1;

			NotifySubscribers();
		}

		return m_array[index];
	}

	void DataMemory::Push(uint8_t value)
	{
		if (m_length >= m_capacity)
		{
			Grow(m_length + 1);
		}

		m_array[m_length++] = value;

		NotifySubscribers();
	}

	void DataMemory::Push(const uint8_t *data, uint32_t count)
	{
		Grow(m_length + count);
		VMPI_memcpy(m_array + m_length, data, count);

		m_length += count;

		NotifySubscribers();
	}

	void DataMemory::SetLength(uint32_t newLength)
	{
		if(m_subscriberRoot && m_length < Domain::GLOBAL_MEMORY_MIN_SIZE)		//Empty fun
			ThrowMemoryError();			//(Original)

		if (newLength > m_capacity)
		{
			if (!Grow(newLength))
			{
				ThrowMemoryError();		//(Origial)
				return;
			}
		}

		m_length = newLength;

		NotifySubscribers();
	}

	void DataMemory::NotifySubscribers()
	{
		SubscriberLink *curLink = m_subscriberRoot;
		SubscriberLink **prevNext = &m_subscriberRoot;

		while(curLink != NULL) // notify subscribers
		{
			AvmAssert(m_length >= Domain::GLOBAL_MEMORY_MIN_SIZE);

			GlobalMemorySubscriber* subscriber = (GlobalMemorySubscriber*)curLink->weakSubscriber->get();

			if (subscriber)
			{
				subscriber->notifyGlobalMemoryChanged(m_array, m_length);
				prevNext = &curLink->next;
			}
			else
			{
				// Domain went away? remove link
				MMgc::GC::WriteBarrier(prevNext, curLink->next);
			}
			curLink = curLink->next;
		}
	}

	bool DataMemory::addSubscriber(GlobalMemorySubscriber* subscriber)
	{
		if(m_length >= Domain::GLOBAL_MEMORY_MIN_SIZE)
		{
			removeSubscriber(subscriber);
			SubscriberLink *newLink = new (MMgc::GC::GetGC(subscriber)) SubscriberLink;
			newLink->weakSubscriber = subscriber->GetWeakRef();
			MMgc::GC::WriteBarrier(&newLink->next, m_subscriberRoot);
			MMgc::GC::WriteBarrier(&m_subscriberRoot, newLink);
			// notify the new "subscriber" of the current state of the world
			subscriber->notifyGlobalMemoryChanged(m_array, m_length);
			return true;
		}
		return false;
	}

	bool DataMemory::removeSubscriber(GlobalMemorySubscriber* subscriber)
	{
		SubscriberLink **prevNext = &m_subscriberRoot;
		SubscriberLink *curLink = m_subscriberRoot;

		while(curLink)
		{
			if ((GlobalMemorySubscriber*)curLink->weakSubscriber->get() == subscriber)
			{
				MMgc::GC::WriteBarrier(prevNext, curLink->next);
				return true;
			}
			prevNext = &curLink->next;
			curLink = curLink->next;
		}
		return false;
	}

	void DataMemory::ThrowMemoryError()
	{

	}
}