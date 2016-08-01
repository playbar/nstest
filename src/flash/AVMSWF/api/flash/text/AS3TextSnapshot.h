#ifndef _AS3TextSnapshot_
#define _AS3TextSnapshot_
namespace avmplus{namespace NativeID{
class TextSnapshotClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextSnapshotObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextSnapshotObject;
	class TextSnapshotClass : public ClassClosure
	{
	public:
		TextSnapshotClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		TextSnapshotObject* CreateObject(void *);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextSnapshotClassSlots m_slots_TextSnapshotClass;
};
class TextSnapshotObject : public ScriptObject
{
	public:
		TextSnapshotObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		struct _XSObject* m_pObject;
		int AS3_charCount_get();
		int AS3_findText(int beginIndex, Stringp textToFind, bool caseSensitive);
		bool AS3_getSelected(int beginIndex, int endIndex);
		Stringp AS3_getSelectedText(bool includeLineEndings);
		Stringp AS3_getText(int beginIndex, int endIndex, bool includeLineEndings);
		ArrayObject* AS3_getTextRunInfo(int beginIndex, int endIndex);
		double AS3_hitTestTextNearPos(double x, double y, double maxDistance);
		void AS3_setSelectColor(uint32_t hexColor);
		void AS3_setSelected(int beginIndex, int endIndex, bool select);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextSnapshotObjectSlots m_slots_TextSnapshotObject;
};}
#endif