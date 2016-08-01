#ifndef _AS3ProductManager_
#define _AS3ProductManager_
namespace avmplus{namespace NativeID{
class ProductManagerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ProductManagerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ProductManagerClass : public ClassClosure
	{
	public:
		ProductManagerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ProductManagerClassSlots m_slots_ProductManagerClass;
};
class ProductManagerObject : public ScriptObject
{
	public:
		ProductManagerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void	AS3_constructor(Stringp name, bool shared);
		bool	AS3_running_get();
		bool	AS3_installed_get();
	 	bool	AS3_launch(Stringp parameters);
		Stringp	AS3_installedVersion_get();
		bool	AS3_doSelfUpgrade(Stringp os);

		bool	AS3_doDownload(Stringp caption, Stringp fileName){return false;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ProductManagerObjectSlots m_slots_ProductManagerObject;
};}
#endif