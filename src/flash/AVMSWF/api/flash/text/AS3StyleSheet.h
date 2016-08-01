#ifndef _AS3StyleSheet_
#define _AS3StyleSheet_

#include "XCSS.h"
#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class StyleSheetClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StyleSheetObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextFormatObject;
	class StyleSheetClass : public ClassClosure
	{
	public:
		StyleSheetClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StyleSheetClassSlots m_slots_StyleSheetClass;
};
class StyleSheetObject : public EventDispatcherObject
{
	public:
		StyleSheetObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(ArrayObject*) m_pNames;
		DRCWB(ScriptObject*) m_pSheets;
		XCSS m_css;
		//inline bool IsEmpty(){return m_list.GetSize()<=0;}
		ArrayObject* AS3_styleNames_get();
		void AS3_constructor();
		void AS3_clear();
		Atom AS3_getStyle(Stringp);
		void AS3_parseCSS(Stringp);
		void AS3_setStyle(Stringp,Atom);
		TextFormatObject* AS3_transform(Atom);
		void AddSheet(XCSS&css);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StyleSheetObjectSlots m_slots_StyleSheetObject;
};}
#endif