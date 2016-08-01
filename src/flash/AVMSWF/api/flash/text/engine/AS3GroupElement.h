#ifndef _AS3GroupElement_
#define _AS3GroupElement_

#include "AS3TextElement.h"
#include "AS3ContentElement.h"

namespace avmplus{namespace NativeID{
class GroupElementClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GroupElementObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GroupElementObject;
	//class TextLineObject;
//class ContentElementClass;

class GroupElementClass : public ClassClosure//ContentElementClass
{
	public:
		GroupElementClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		GroupElementObject*CreateObject(ObjectVectorObject*,GroupElementObject*oldObj);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GroupElementClassSlots m_slots_GroupElementClass;
};
class GroupElementObject : public ContentElementObject
{
	public:
		
		GroupElementObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(ObjectVectorObject*) m_pElements;
	public:
		ContentElementObject* GetNext(ContentElementObject*obj);
		virtual void ReleaseCreationData();
		virtual bool CreateContext(TextLineObject*pLine,int nID);
		//virtual void InitCreationData();
		void MakeRowText();
		void SetItemGroup(GroupElementObject*item);
		int AS3_elementCount_get();
//		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		ContentElementObject* AS3_getElementAt(int index);
		ContentElementObject* AS3_getElementAtCharIndex(int charIndex);
//		AvmBox AS3_getElementIndex(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		GroupElementObject* AS3_groupElements(int beginIndex, int endIndex);
		TextElementObject* AS3_mergeTextElements(int beginIndex, int endIndex);
		ObjectVectorObject* AS3_replaceElements(int beginIndex, int endIndex, ObjectVectorObject *pNewElements);
		void AS3_setElements(ObjectVectorObject *pValue);
		TextElementObject* AS3_splitTextElement(int elementIndex, int splitIndex);
		void AS3_ungroupElements(int groupIndex);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GroupElementObjectSlots m_slots_GroupElementObject;
};}
#endif