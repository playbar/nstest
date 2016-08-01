#ifndef _AS3BXWaveStream_
#define _AS3BXWaveStream_
#include "AS3Event.h"
#include "AS3EventDispatcher.h"
#include "AS3ByteArray.h"
namespace avmplus{namespace NativeID{
class BXWaveEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//void* instance;
	Stringp WAVE_IN;
private:};
class BXWaveEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//avmshell::ByteArrayObject* data;
//	int				 channels;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{	
	class BXWaveEventObject;
	class BXWaveEventClass : public ClassClosure
	{
	public:
		BXWaveEventClass(VTable *vtable);
		inline Stringp getSlotWAVE_IN(){return m_slots_BXWaveEventClass.WAVE_IN;}
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);		
		BXWaveEventObject* createEvent(ByteArrayObject*obj,int channels);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXWaveEventClassSlots m_slots_BXWaveEventClass;
};
class BXWaveEventObject : public EventObject//ScriptObject
{
	public:
		BXWaveEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(ByteArrayObject*) m_pData;
		int						m_nChannels;
		ByteArrayObject* AS3_getData(){return m_pData;}
		int				 AS3_getChannels(){return m_nChannels;}
		//double m_offx,m_offy;		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXWaveEventObjectSlots m_slots_BXWaveEventObject;
};}
	
/////////////////////////////////////////////////////////////////////
namespace avmplus{namespace NativeID{
class BXWaveStreamClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//void* instance;
private:};
class BXWaveStreamObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{	
	class ByteArrayObject;
	class MicrophoneObject;
	class BXWaveStreamClass : public ClassClosure
	{
	public:
		BXWaveStreamClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXWaveStreamClassSlots m_slots_BXWaveStreamClass;
};
class BXWaveStreamObject : public EventDispatcherObject//ScriptObject
{
	public:
		BXWaveStreamObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		DRCWB(MicrophoneObject*) m_pMicrophone;
	public:
		void WaveIn(void*pData,int l);
		DRCWB(ByteArrayObject*) m_pData;
		//double m_offx,m_offy;
		MicrophoneObject* AS3_getMicrophone();
		void AS3_getDCTData(ByteArrayObject*pObj);
		void AS3_constructor(MicrophoneObject*);
		bool AS3_start();
		bool AS3_stop();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXWaveStreamObjectSlots m_slots_BXWaveStreamObject;
};}
//////////////////////////////////////////////////
namespace avmplus{namespace NativeID{
class BXAccClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//void* instance;
private:};
class BXAccObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//avmshell::ByteArrayObject* data;
//	int				 channels;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{	
	class BXAccClass : public ClassClosure
	{
	public:
		BXAccClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);		
		//BXAccObject* createEvent(ByteArrayObject*obj,int channels);
		double AS3_getX();
		double AS3_getY();
		double AS3_getZ();
		Atom AS3_aObject_get();
		Atom AS3_gObject_get();
		bool AS3_init();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXAccClassSlots m_slots_BXAccClass;
	};
	class BXAccObject : public ScriptObject
	{
		public:
			BXAccObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		public:
		private:
	#ifdef _SYMBIAN
		public:
	#endif
			friend class avmplus::NativeID::SlotOffsetsAndAsserts;
			avmplus::NativeID::BXAccObjectSlots m_slots_BXAccObject;
};}

#endif
