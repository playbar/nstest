#ifndef _AS3Camera_
#define _AS3Camera_

#include "AS3EventDispatcher.h"
#include "AS3CameraPosition.h"
#include "XCamera.h"
namespace avmplus{namespace NativeID{
class CameraClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CameraObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CameraObject;
	class VideoObject;
	class RectangleObject;
	class ByteArrayObject;
	class BitmapDataObject;
	class CameraClass : public ClassClosure
	{
	public:
		CameraClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		CameraObject* AS3_getCamera(Stringp name);
		ArrayObject* AS3_names_get();
		//new adds
//		bool AS3_isSupported_get();
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraClassSlots m_slots_CameraClass;
};
class CameraObject : public EventDispatcherObject
{
	public:
		CameraObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:
		int m_nActivityLevel;
		//int m_nWidth,m_nHeight;
		bool InitCamera(XPCWSTR name);
		XDib* GetDib();//{return m_camera.GetDib();}
		bool AttachVideo(VideoObject*obj);
		bool UnAttachVideo(VideoObject*obj);
		double AS3_activityLevel_get();
		int AS3_bandwidth_get();
		double AS3_currentFPS_get();
		double AS3_fps_get();
		int AS3_height_get();
		int AS3_index_get();
		int AS3_keyFrameInterval_get();
		bool AS3_loopback_get();
		int AS3_motionLevel_get();
		int AS3_motionTimeout_get();
		bool AS3_muted_get();
		Stringp AS3_name_get();
		int AS3_quality_get();
		int AS3_width_get();
		void AS3_setKeyFrameInterval(int);
		void AS3_setLoopback(bool);
		void AS3_setMode(int,int,double,bool);
		void AS3_setMotionLevel(int,int);
		void AS3_setQuality(int,int);
	public:
		void CopyDib();
		XDib* m_pDib;		
		XCamera m_camera;
		bool m_bFirst;

		//new adds 11
		Stringp m_strPosition;

		Stringp AS3_position_get();
		void AS3_copyToByteArray(RectangleObject* pRect, ByteArrayObject* destination);
		void AS3_copyToVector(RectangleObject* pRect, IntVectorObject* destination);
		void AS3_drawToBitmapData(BitmapDataObject* pDestination);
		//new adds 11 end
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraObjectSlots m_slots_CameraObject;
};}
#endif