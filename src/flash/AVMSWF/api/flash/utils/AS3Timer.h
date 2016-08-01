#ifndef _AS3Timer_
#define _AS3Timer_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class TimerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TimerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TimerClass : public ClassClosure
	{
	public:
		TimerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TimerClassSlots m_slots_TimerClass;
};
class TimerObject : public EventDispatcherObject
{
	public:
		TimerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:
		//void* pData;//Set your data!!
		int  m_nCurrentCount;
		XU32 m_nDelay;
		XU32 m_nID;
		XU32 m_nLastTick;
		int m_nRepeatCount;
		bool m_bRunning;

		int AS3_currentCount_get(){return m_nCurrentCount;}
		double AS3_delay_get(){return m_nDelay;}
		void AS3_delay_set(double v)
		{
			if(v<0)
				toplevel()->throwArgumentError(kInvalidArgumentError,core()->toErrorString(v));
			m_nDelay=v;
		}
		int AS3_repeatCount_get(){return m_nRepeatCount;}
		void AS3_repeatCount_set(int v){ m_nRepeatCount=v;}
		bool AS3_running_get(){return m_bRunning;}
		void AS3_constructor(double delay,int repeat)
		{
			if(delay<0)
				toplevel()->throwArgumentError(kInvalidArgumentError,core()->toErrorString(delay));
			m_nDelay=delay;
			m_nRepeatCount=repeat;
		}
		void AS3_reset();
		void AS3_start();
		void AS3_stop();
		void OnTimer(XU32 t);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TimerObjectSlots m_slots_TimerObject;
};}
#endif