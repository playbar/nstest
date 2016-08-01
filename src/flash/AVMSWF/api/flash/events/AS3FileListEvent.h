#ifndef _AS3FileListEvent_
#define _AS3FileListEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class FileListEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DIRECTORY_LISTING : String = "directoryListing"
	//SELECT_MULTIPLE : String = "selectMultiple"

	Stringp DIRECTORY_LISTING;
	Stringp SELECT_MULTIPLE;

private:};
class FileListEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FileListEventClass : public ClassClosure//EventClass
	{
	public:
		FileListEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileListEventClassSlots m_slots_FileListEventClass;
};
class FileListEventObject : public EventObject
{
	public:
		FileListEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		ArrayObject* AS3_files_get();
		void AS3_files_set(ArrayObject *pFiles);

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, ArrayObject *pFiles);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileListEventObjectSlots m_slots_FileListEventObject;
};}
#endif