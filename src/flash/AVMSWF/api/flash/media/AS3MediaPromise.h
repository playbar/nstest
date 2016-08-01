#ifndef _AS3MediaPromise_
#define _AS3MediaPromise_

#include "AS3EventDispatcher.h"

#include "AS3File.h"
#include "AS3ErrorEvent.h"

namespace avmplus{namespace NativeID{
class MediaPromiseClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MediaPromiseObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MediaPromiseClass : public ClassClosure
	{
	public:
		MediaPromiseClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MediaPromiseClassSlots m_slots_MediaPromiseClass;
};
class MediaPromiseObject : public EventDispatcherObject
{
	public:
		MediaPromiseObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		FileObject* AS3_file_get();
		bool AS3_isAsync_get();
		Stringp AS3_mediaType_get();
		Stringp AS3_relativePath_get();

		void AS3_close();
		AvmBox AS3_open(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_reportError(ErrorEventObject *pEvent);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MediaPromiseObjectSlots m_slots_MediaPromiseObject;
};}
#endif