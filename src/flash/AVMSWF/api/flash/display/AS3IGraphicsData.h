#ifndef _AS3IGraphicsData_
#define _AS3IGraphicsData_
namespace avmplus{namespace NativeID{
class IGraphicsDataClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IGraphicsDataObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IGraphicsDataClass : public ClassClosure
	{
	public:
		IGraphicsDataClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsDataClassSlots m_slots_IGraphicsDataClass;
};
class IGraphicsDataObject : public ScriptObject
{
	public:
		IGraphicsDataObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IGraphicsDataObjectSlots m_slots_IGraphicsDataObject;
};}
#endif