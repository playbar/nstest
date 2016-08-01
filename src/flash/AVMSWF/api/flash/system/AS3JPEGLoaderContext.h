#ifndef _AS3JPEGLoaderContext_
#define _AS3JPEGLoaderContext_

#include "AS3LoaderContext.h"
#include "AS3ApplicationDomain.h"

namespace avmplus{namespace NativeID{
class JPEGLoaderContextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class JPEGLoaderContextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double deblockingFilter;
private:};
}}
namespace avmshell{
	class JPEGLoaderContextClass : public ClassClosure
	{
	public:
		JPEGLoaderContextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JPEGLoaderContextClassSlots m_slots_JPEGLoaderContextClass;
};
class JPEGLoaderContextObject : public LoaderContextObject
{
	public:
		JPEGLoaderContextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_deblockingFilter_get();
		//void AS3_deblockingFilter_set(double deblockingFilter);

		//void AS3_constructor(double deblockingFilter, bool checkPolicyFile, 
		//						ApplicationDomainObject *pApplicationDomain, SecurityDomainObject *pSecurityDomain);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JPEGLoaderContextObjectSlots m_slots_JPEGLoaderContextObject;
};}
#endif