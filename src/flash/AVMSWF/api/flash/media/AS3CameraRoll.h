#ifndef _AS3CameraRoll_
#define _AS3CameraRoll_

#include "AS3EventDispatcher.h"

#include "AS3BitmapData.h"

namespace avmplus{namespace NativeID{
class CameraRollClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CameraRollObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CameraRollClass : public ClassClosure
	{
	public:
		CameraRollClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraRollClassSlots m_slots_CameraRollClass;
};
class CameraRollObject : public EventDispatcherObject
{
	public:
		CameraRollObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_supportsAddBitmapData_get();
		bool AS3_supportsBrowseForImage_get();

		void AS3_constructor();
		void AS3_addBitmapData(BitmapDataObject *pBitmapData);
		void AS3_browseForImage();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraRollObjectSlots m_slots_CameraRollObject;
};}
#endif