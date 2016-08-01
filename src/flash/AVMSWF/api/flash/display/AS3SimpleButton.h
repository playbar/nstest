#ifndef _AS3SimpleButton_
#define _AS3SimpleButton_

#include "AS3InteractiveObject.h"

namespace avmplus{namespace NativeID{
class SimpleButtonClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SimpleButtonObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoundTransformObject;
	class ShellCodeContext;
	class SimpleButtonClass : public ClassClosure
	{
	public:
		SimpleButtonClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SimpleButtonClassSlots m_slots_SimpleButtonClass;
};
class SimpleButtonObject : public InteractiveObjectObject
{
	public:
		void FormatState();
		SimpleButtonObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		DisplayObjectObject* CreateStateObject();
		virtual void ReleaseObject();
	public:
		DWB(RCHashTable*) m_pChildren;
		void AddChildRef(ScriptObject*);
		void RemoveChildRef(ScriptObject*);
		DRCWB(DisplayObjectObject*) m_pDownState;
		DRCWB(DisplayObjectObject*) m_pHitState;
		DRCWB(DisplayObjectObject*) m_pOverState;
		DRCWB(DisplayObjectObject*) m_pUpState;
		DRCWB(SoundTransformObject*) m_pTransform;
		//void ReplaceState(DisplayObjectObject*p,int nState);
		DisplayObjectObject* AS3_downState_get()
		{	return m_pDownState;	}
		void AS3_downState_set(DisplayObjectObject*p);		
		void AS3_overState_set(DisplayObjectObject*p);		
		void AS3_upState_set(DisplayObjectObject*p);
		bool AS3_enabled_get();
		void AS3_enabled_set(bool b);
		DisplayObjectObject* AS3_hitTestState_get()
		{ return m_pHitState;}
		void AS3_hitTestState_set(DisplayObjectObject*p)
		{ m_pHitState=p; }
		DisplayObjectObject* AS3_overState_get()
		{ return m_pOverState;}
		
		SoundTransformObject* AS3_soundTransform_get()
		{ return m_pTransform;	}
		void AS3_soundTransform_set(SoundTransformObject*p)
		{ m_pTransform=p;}
		bool AS3_trackAsMenu_get();
		void AS3_trackAsMenu_set(bool b);
		DisplayObjectObject* AS3_upState_get()
		{return m_pUpState;}
		
		bool AS3_useHandCursor_get();
		void AS3_useHandCursor_set(bool b);
		void AS3_constructor(DisplayObjectObject*up,DisplayObjectObject*over,DisplayObjectObject*down,DisplayObjectObject*hit);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SimpleButtonObjectSlots m_slots_SimpleButtonObject;
};}
#endif