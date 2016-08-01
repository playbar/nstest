#ifndef _AS3FileFilter_
#define _AS3FileFilter_
namespace avmplus{namespace NativeID{
class FileFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FileFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FileFilterObject;

	class FileFilterClass : public ClassClosure
	{
	public:
		FileFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		FileFilterObject* CreateFileFilterObject(Stringp description, Stringp extension, Stringp macType);

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileFilterClassSlots m_slots_FileFilterClass;
};
class FileFilterObject : public ScriptObject
{
	public:
		FileFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		Stringp		AS3_description_get();
		void		AS3_description_set(Stringp description);
		Stringp		AS3_extension_get();
		void		AS3_extension_set(Stringp extension);
		Stringp		AS3_macType_get();
		void		AS3_macType_set(Stringp macType);
		void		AS3_constructor(Stringp description, Stringp extension, Stringp macType);

	public:
		Stringp m_description;
		Stringp m_extension;
		Stringp m_macType;

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileFilterObjectSlots m_slots_FileFilterObject;


};}
#endif