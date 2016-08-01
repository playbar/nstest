#ifndef _AS3ExtensionContext_
#define _AS3ExtensionContext_

#include "AS3EventDispatcher.h"

#include "AS3File.h"

namespace avmplus{namespace NativeID{
class ExtensionContextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ExtensionContextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ExtensionContextObject;

	class ExtensionContextClass : public ClassClosure
	{
	public:
		ExtensionContextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		ExtensionContextObject* AS3_createExtensionContext(Stringp pExtensionID, Stringp pContextType);
		FileObject* AS3_getExtensionDirectory(Stringp pExtensionID);

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ExtensionContextClassSlots m_slots_ExtensionContextClass;
};
class ExtensionContextObject : public EventDispatcherObject
{
	public:
		ExtensionContextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		Atom AS3_actionScriptData_get();
		void AS3_actionScriptData_set(Atom pActionScriptData);

		Atom AS3_call(Stringp pFunctionName, ArrayObject *pArgs);
		void AS3_dispose();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ExtensionContextObjectSlots m_slots_ExtensionContextObject;
};}
#endif