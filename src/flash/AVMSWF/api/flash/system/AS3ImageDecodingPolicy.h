#ifndef _AS3ImageDecodingPolicy_
#define _AS3ImageDecodingPolicy_
namespace avmplus{namespace NativeID{
class ImageDecodingPolicyClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//ON_DEMAND : String = "onDemand"
	//ON_LOAD : String = "onLoad"

	Stringp ON_DEMAND;
	Stringp ON_LOAD;

private:};
class ImageDecodingPolicyObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ImageDecodingPolicyClass : public ClassClosure
	{
	public:
		ImageDecodingPolicyClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ImageDecodingPolicyClassSlots m_slots_ImageDecodingPolicyClass;
};
class ImageDecodingPolicyObject : public ScriptObject
{
	public:
		ImageDecodingPolicyObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ImageDecodingPolicyObjectSlots m_slots_ImageDecodingPolicyObject;
};}
#endif