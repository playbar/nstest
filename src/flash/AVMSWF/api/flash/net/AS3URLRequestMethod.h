#ifndef _AS3URLRequestMethod_
#define _AS3URLRequestMethod_
namespace avmplus{namespace NativeID{
class URLRequestMethodClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp GET = "GET";
	//Stringp POST = "POST";

	//DELETE : String = "DELETE"
	//HEAD : String = "HEAD"
	//OPTIONS : String = "OPTIONS"
	//PUT : String = "PUT"


	Stringp GET;
	Stringp POST;

	//new adds
	Stringp DELETE_C;
	Stringp HEAD;
	Stringp OPTIONS;
	Stringp PUT;
	//new adds end

private:};
class URLRequestMethodObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class URLRequestMethodClass : public ClassClosure
	{
	public:
		enum {GET=0, POST=1, DELETE_C=2, HEAD=3, OPTIONS=4, PUT=5, UNDEF};
	public:
		URLRequestMethodClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		inline Stringp getSlotGET(){return m_slots_URLRequestMethodClass.GET;}
		inline Stringp getSlotPOST(){return m_slots_URLRequestMethodClass.POST;}

		//new adds
		inline Stringp getSlotDELETE(){return m_slots_URLRequestMethodClass.DELETE_C;}
		inline Stringp getSlotHEAD(){return m_slots_URLRequestMethodClass.HEAD;}
		inline Stringp getSlotOPTIONS(){return m_slots_URLRequestMethodClass.OPTIONS;}
		inline Stringp getSlotPUT(){return m_slots_URLRequestMethodClass.PUT;}
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestMethodClassSlots m_slots_URLRequestMethodClass;
};
class URLRequestMethodObject : public ScriptObject
{
	public:
		URLRequestMethodObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestMethodObjectSlots m_slots_URLRequestMethodObject;
};}
#endif