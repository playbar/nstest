#ifndef _AS3_DATAMEMORY_
#define _AS3_DATAMEMORY_

#ifdef WIN32
	#ifdef _SYMBIAN
	#include "symbian/symbian-platform.h"
	#else
	#include "../../platform/win32/win32-platform.h"
	#endif
#elif defined(__APPLE__)
#include "../../platform/mac/mac-platform.h"
// begin hgl 添加文件头文件定义         
#elif defined __ANDROID__
#include "../../platform/android/android-platform.h"
#elif defined __SYMBIAN32__
#include "symbian/symbian-platform.h"
// end 
#endif
#include "../../core/Domain.h"
#include "../../core/Toplevel.h"

namespace avmshell
{
	class DataMemory : public GlobalMemoryProvider
	{
	public:
		void Clear();
	public:
		void ThrowMemoryError();
		DataMemory();
		DataMemory(const DataMemory &lhs);

		~DataMemory();

		uint8_t		operator[] (uint32_t index) const;
		uint8_t&	operator[](uint32_t index);

		void		Push(uint8_t data);
		void		Push(const uint8_t *pData, uint32_t length);

		uint32_t	GetLength()const{return m_length;}
		void		SetLength(uint32_t newLength);

		bool		Grow(uint32_t minimumCapacity);
		uint8_t*	GetBuffer() const {return m_array;}	
		inline bool		IsAttach(){return m_bAttach;}
		void		Attach(uint8_t* buffer,int length)
		{
			if(!m_bAttach&&m_array)
				mmfx_delete_array(m_array);
			m_bAttach=true;
			m_array=buffer;
			m_length=length;
			m_capacity=0;
		}
		// from GlobalMemoryProvider
		/*virtual*/ bool addSubscriber(GlobalMemorySubscriber* subscriber);
		/*virtual*/ bool removeSubscriber(GlobalMemorySubscriber* subscriber);

		//deflate start -dsx
		bool GetMemory(DataMemory &dataMemory);
		//deflate end

	protected:
		enum {kGrowthIncrement = 4096};
		bool			m_bAttach;
		uint32_t		m_capacity;
		uint32_t		m_length;
		uint8_t*		m_array;
//		Toplevel* const m_pcToplevel;

		// singly linked list of all subscribers to this ByteArray...
		// in practice, there isn't much liklihood of too many
		// subscribers at a time and notifications should be rare
		// hence the slower, simpler, smaller structures and algorithms

		// links are removed explicitly via GlobalMemoryUnsubscribe
		// or implicitly when the backing store changes
		struct SubscriberLink : public MMgc::GCObject
		{
			// weak reference to the subscriber DomainEnv so
			// multiple DomainEnvs subscribing to the same
			// ByteArray don't result in a DomainEnv not being
			// collectable because a ByteArray refers to it and
			// is referenced by another live DomainEnv
			MMgc::GCWeakRef *weakSubscriber;
			// next link
			SubscriberLink *next;
		};
		SubscriberLink* m_subscriberRoot;

		void NotifySubscribers();
//		void ThrowMemoryError();
	};
}

#endif
