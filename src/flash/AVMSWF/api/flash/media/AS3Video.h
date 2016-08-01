#ifndef _AS3Video_
#define _AS3Video_

#include "AS3DisplayObject.h"
#include "SBitmap.h"
namespace avmplus{namespace NativeID{
class VideoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class VideoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetStreamObject;
	class CameraObject;
	class VideoClass : public ClassClosure
	{
	public:
		VideoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoClassSlots m_slots_VideoClass;
};
class VideoObject : public DisplayObjectObject
{
	public:
		VideoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void ReleaseObject();	
	public:
		//XDrawDib m_dib;
		void Update();
		//SBitmapCore m_sbitmap;		
		//void* pData;//Set your data!!
		int AS3_deblocking_get();
		void AS3_deblocking_set(int);
		bool AS3_smoothing_get();
		void AS3_smoothing_set(bool);
		int AS3_videoHeight_get();
		int AS3_videoWidth_get();
		void AS3_constructor(int,int);
		void AS3_attachCamera(CameraObject*);
		void AS3_attachNetStream(NetStreamObject*);
		void AS3_clear();
		CameraObject* GetAttachCamera(){return m_pCamera;}
	private:
		DRCWB(CameraObject*) m_pCamera;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoObjectSlots m_slots_VideoObject;
};}
#endif