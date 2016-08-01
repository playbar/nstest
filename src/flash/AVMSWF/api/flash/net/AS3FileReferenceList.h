#ifndef _AS3FileReferenceList_
#define _AS3FileReferenceList_

#include "../core/ArrayObject.h"
#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class FileReferenceListClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FileReferenceListObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FileReferenceListObject;

	class FileReferenceListClass : public ClassClosure
	{
	public:
		FileReferenceListObject* CreateFileReferenceList();

	public:
		FileReferenceListClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileReferenceListClassSlots m_slots_FileReferenceListClass;
};
class FileReferenceListObject : public EventDispatcherObject
{
	public:
		FileReferenceListObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		ArrayObject *m_pFileList;

	public:
		ArrayObject*	AS3_fileList_get();
		void			AS3_constructor();
		bool			AS3_browse(ArrayObject *pTypeFilter);

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileReferenceListObjectSlots m_slots_FileReferenceListObject;
};}
#endif