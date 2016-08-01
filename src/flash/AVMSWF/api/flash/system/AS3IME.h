#ifndef _AS3IME_
#define _AS3IME_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class IMEClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IMEObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IMEClass : public ClassClosure
	{
	public:
		DRCWB(Stringp) m_strComposition;
		IMEClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		void SetComposition(int n);
		void AS3_doConversion();
		void AS3_setCompositionString(Stringp);
		Stringp AS3_conversionMode_get();
		bool AS3_enabled_get();

		//new adds
//		bool AS3_isSupported_get();

		void AS3_compositionAbandoned();
		void AS3_compositionSelectionChanged(int start, int end);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IMEClassSlots m_slots_IMEClass;
};
class IMEObject : public EventDispatcherObject
{
	public:
		IMEObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IMEObjectSlots m_slots_IMEObject;
};}
#endif