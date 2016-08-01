#ifndef _AS3XMLNodeType_
#define _AS3XMLNodeType_
namespace avmplus{namespace NativeID{
class XMLNodeTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//uint32_t ELEMENT_NODE = 1;
	//uint32_t TEXT_NODE = 3;

	uint32_t ELEMENT_NODE;
	uint32_t COPY_NODE;
	uint32_t TEXT_NODE;
	uint32_t XML_NODE;
private:};
class XMLNodeTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:

//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	enum
	{
		XELEMENT_NODE=1,
		XCOPY_NODE=2,
		XTEXT_NODE=3,
		XXML_NODE=4
	};
	class XMLNodeTypeClass : public ClassClosure
	{
	public:
		XMLNodeTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLNodeTypeClassSlots m_slots_XMLNodeTypeClass;
	public:
		inline uint32_t getSlotELEMENT_NODE(){return m_slots_XMLNodeTypeClass.ELEMENT_NODE;}
		inline uint32_t getSlotTEXT_NODE(){return m_slots_XMLNodeTypeClass.TEXT_NODE;}
};
class XMLNodeTypeObject : public ScriptObject
{
	public:
		XMLNodeTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLNodeTypeObjectSlots m_slots_XMLNodeTypeObject;
};}
#endif