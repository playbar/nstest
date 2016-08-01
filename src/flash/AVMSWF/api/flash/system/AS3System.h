#ifndef _AS3System_
#define _AS3System_
namespace avmplus{namespace NativeID{
class SystemClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SystemObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IMEObject;
	class SystemClass : public ClassClosure
	{
	public:
		DRCWB(IMEObject*) m_pIME;
		bool			  m_bUseCodePage;
	public:
		SystemClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		IMEObject* AS3_ime_get()
		{ return m_pIME; }
		uint32_t AS3_totalMemory_get();
		bool AS3_useCodePage_get(){return m_bUseCodePage;}
		void AS3_useCodePage_set(bool b){m_bUseCodePage=b;}
		void AS3_gc();
		void AS3_setClipboard(Stringp);

		//new adds
		double AS3_freeMemory_get();
		double AS3_privateMemory_get();
		double AS3_totalMemoryNumber_get();

		void AS3_disposeXML(XMLObject *pNode);
		void AS3_exit(uint32_t code);
		void AS3_pause();
		void AS3_resume();
		//new adds end

		//new adds 11
		void AS3_pauseForGCIfCollectionImminent(double imminence = 0.75);
		//new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemClassSlots m_slots_SystemClass;
};
class SystemObject : public ScriptObject
{
	public:
		SystemObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
				
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemObjectSlots m_slots_SystemObject;
};}
#endif