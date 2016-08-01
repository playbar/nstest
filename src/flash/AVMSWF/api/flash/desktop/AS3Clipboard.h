#ifndef _AS3Clipboard_
#define _AS3Clipboard_
namespace avmplus{namespace NativeID{

class DictionaryObject;

class ClipboardClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	Stringp LIFETIME_ERROR;
	Stringp READ_ACCESS_ERROR;
	Stringp WRITE_ACCESS_ERROR;

	DictionaryObject *_fromFlash;
	DictionaryObject * _toFlash;
	bool _wasFormatMapInitialized;
private:};
class ClipboardObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ClipboardObject;

	class ClipboardClass : public ClassClosure
	{
	public:
		ClipboardClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ClipboardObject* AS3_generalClipboard_get(){return NULL;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClipboardClassSlots m_slots_ClipboardClass;
};
class ClipboardObject : public ScriptObject
{
	public:
		ClipboardObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		ArrayObject*	AS3_formats_get(){return NULL;}
		void	AS3_clear(){}
		void	AS3_clearData(Stringp format){}
		bool	AS3_alive_get(){return false;}
		bool	AS3_canReadContents_get(){return false;}
		bool	AS3_canWriteContents_get(){return false;}
		void	AS3_nativeSetHandler(Stringp format, FunctionObject *pHandler){}
		ScriptObject*	AS3_getObjectReference(Stringp format){return NULL;}
		void	AS3_putObjectReference(Stringp format, ScriptObject *pData){}
		Stringp	AS3_getString(){return NULL;}
		void	AS3_putString(Stringp s){}
		Stringp	AS3_getHTML(){return NULL;}
		void	AS3_putHTML(Stringp html){}
		ByteArrayObject*	AS3_getRTF(){return NULL;}
		void	AS3_putRTF(ByteArrayObject *pRtf){}
		ByteArrayObject*	AS3_getByteArray(Stringp format){return NULL;}
		void	AS3_putByteArray(Stringp format, ByteArrayObject *pBytes){}
		int	AS3_swfVersion_get(){return 0;}
		void	AS3_setHandlerStoringData(bool status){}

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ClipboardObjectSlots m_slots_ClipboardObject;
};}
#endif