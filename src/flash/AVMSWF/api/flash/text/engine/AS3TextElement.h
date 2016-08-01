#ifndef _AS3TextElement_
#define _AS3TextElement_

#include "AS3ContentElement.h"

namespace avmplus{namespace NativeID{
class TextElementClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextElementObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextElementObject;
	class TextElementClass : public ClassClosure//ContentElementClass
	{
	public:
		TextElementClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		TextElementObject* CreateObject(Stringp,TextElementObject*);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextElementClassSlots m_slots_TextElementClass;
};
class TextElementObject : public ContentElementObject
{
	public:
		TextElementObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual bool CreateContext(TextLineObject*pLine,int nID);
		void CombineText(TextElementObject*pTxt);
		TextElementObject* Split(int charIndex);
//		AvmBox AS3_text_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_text_set(Stringp text);
//		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_replaceText(int beginIndex, int endIndex, Stringp newText);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextElementObjectSlots m_slots_TextElementObject;
};}
#endif