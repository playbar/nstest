#ifndef _AS3URLLoaderDataFormat_
#define _AS3URLLoaderDataFormat_
namespace avmplus{namespace NativeID{
class URLLoaderDataFormatClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp BINARY = "binary";
	//Stringp TEXT = "text";
	//Stringp VARIABLES = "variables";

	Stringp BINARY;
	Stringp TEXT;
	Stringp VARIABLES;
private:};
class URLLoaderDataFormatObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class URLLoaderDataFormatObject;

	class URLLoaderDataFormatClass : public ClassClosure
	{
	public:
		URLLoaderDataFormatObject *CreateURLLoaderDataFormat();

	public:
		URLLoaderDataFormatClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotBINARY(){return m_slots_URLLoaderDataFormatClass.BINARY;}
		inline Stringp getSlotTEXT(){return m_slots_URLLoaderDataFormatClass.TEXT;}
		inline Stringp getSlotVARIABLES(){return m_slots_URLLoaderDataFormatClass.VARIABLES;}
	public:
		Stringp IDToString(int id){return NULL;}

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLLoaderDataFormatClassSlots m_slots_URLLoaderDataFormatClass;
};
class URLLoaderDataFormatObject : public ScriptObject
{
	public:
		URLLoaderDataFormatObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLLoaderDataFormatObjectSlots m_slots_URLLoaderDataFormatObject;
};}
#endif