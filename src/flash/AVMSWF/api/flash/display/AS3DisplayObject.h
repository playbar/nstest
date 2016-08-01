#ifndef _AS3DisplayObject_
#define _AS3DisplayObject_

#include "AS3EventDispatcher.h"
#include "AS3AccessibilityProperties.h"
#include "AS3Shader.h"
#include "AS3Matrix.h"
#include "AS3Vector3D.h"
#include "AS3Point.h"

#include "flashgeom.h"
#include "Raster_gl.h"

namespace avmplus{namespace NativeID{
class DisplayObjectClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DisplayObjectObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LoaderInfoObject;
	class PointObject;
	class RectangleObject;
	class TransformObject;
	class BitmapDataObject;
	class ColorTransformObject;
	class RectangleObject;
	class MatrixObject;
	class GraphicsObject;
	class StageObject;
	class BitmapDataObject;
	class BXDeviceBitmapDataObject;
	class DisplayObjectClass : public ClassClosure
	{
	public:
		DisplayObjectClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplayObjectClassSlots m_slots_DisplayObjectClass;
};
class DisplayObjectObject : public EventDispatcherObject
{
	public:
		void SetRefBitmap(BitmapDataObject*bmp)
		{
			m_pRefBitmap=bmp;
		}
		BitmapDataObject* GetRefBitmap(){return m_pRefBitmap;}
	private:
		
		DRCWB(BitmapDataObject*) m_pRefBitmap;
		DRCWB(ScriptObject*) m_pMetaData;
		double m_rotationX,
			   m_rotationY,
			   m_rotationZ,
			   m_scaleZ;
	//private:
		//DRCWB(BitmapDataObject*) m_pRefBitmap;
		//void*	m_pDummyObject;
	public:
		DisplayObjectObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		//virtual ~DisplayObjectObject();
		virtual void Release();		
		virtual void ReleaseObject();
		virtual ArrayObject* GetFilters(){return m_pFilters;}
		virtual void SetObject(void*pObject);
		//virtual bool CheckEventMask(int nCmd);
		virtual bool CallFrame(int iFrame){return false;}
		virtual GraphicsObject* GetGraphics(){return NULL;}
		virtual void Draw(BitmapDataObject*pDst,MatrixObject*pMatrix,ColorTransformObject*pTransform,Stringp blendMode,RectangleObject*clip,bool sm);		
		virtual void DrawDevice(BXDeviceBitmapDataObject*pDst,MATRIX&nat,ColorTransformObject*pTransform,Stringp blendMode,XRect&rcc,bool sm,XRect&rc);
		//int v;
		DRCWB(AccessibilityPropertiesObject*) m_pAccessibilityProp;
		DRCWB(ArrayObject*)					  m_pFilters;
		DRCWB(RectangleObject*)				  m_scale9grid;
		DRCWB(DisplayObjectObject*)				  m_pMask;
		DRCWB(LoaderInfoObject*)			  m_pLoaderInfo;
		ScriptPlayer*						  m_pFreePlayer;
		//BitmapDataObject*					  m_pDummyObject;
		//virtual int MapSystemEvent(Stringp strName,int&nMax);
	public:		
		//int type;
		GraphicsObject* CreateGraphics();
		void GetMatrix(MATRIX&m);
		void SetMatrix(MATRIX&m);
		void GetCXForm(XCXForm&m);
		void SetCXForm(XCXForm&m);
	public:
		bool   m_bNewObject;
		struct _XSObject* m_pObject;//Set your data!!
		bool	CreateNewObject(int type);
		virtual EventDispatcherObject* GetParentObject(bool bSeek);
		ScriptObject* AS3_accessibilityProperties_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_accessibilityProperties_set(ScriptObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_alpha_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_alpha_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_blendMode_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_blendMode_set(Stringp);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_cacheAsBitmap_get();
		void AS3_cacheAsBitmap_set(bool);
		ArrayObject* AS3_filters_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_filters_set(ArrayObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_height_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_height_set(double v);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		LoaderInfoObject* AS3_loaderInfo_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		ScriptObject* AS3_mask_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_mask_set(DisplayObjectObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_mouseX_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_mouseY_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_name_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_name_set(Stringp);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_opaqueBackground_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_opaqueBackground_set(Atom pObj);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		DisplayObjectObject* AS3_parent_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		DisplayObjectObject* AS3_root_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_rotation_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_rotation_set(double v);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		RectangleObject* AS3_scale9Grid_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_scale9Grid_set(RectangleObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_scaleX_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_scaleX_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_scaleY_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_scaleY_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		RectangleObject* AS3_scrollRect_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_scrollRect_set(RectangleObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		StageObject* AS3_stage_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		TransformObject* AS3_transform_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_transform_set(TransformObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_visible_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_visible_set(bool);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_width_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_width_set(double w);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_x_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_x_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_y_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_y_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_z_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_z_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		RectangleObject* AS3_getBounds(DisplayObjectObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		RectangleObject* AS3_getRect(DisplayObjectObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		PointObject* AS3_globalToLocal(PointObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_hitTestObject(DisplayObjectObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_hitTestPoint(double x,double y,bool b);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		PointObject* AS3_localToGlobal(PointObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_constructor();
		void AS3_drawNow();

		//new adds
		void AS3_blendShader_set(ShaderObject* pShader);
		MatrixObject* AS3_cacheAsBitmapMatrix_get();
		void AS3_cacheAsBitmapMatrix_set(MatrixObject *pMatrix);
		double AS3_rotationX_get();
		void AS3_rotationX_set(double rotationX);
		double AS3_rotationY_get();
		void AS3_rotationY_set(double rotationY);
		double AS3_rotationZ_get();
		void AS3_rotationZ_set(double rotationZ);
		double AS3_scaleZ_get();
		void AS3_scaleZ_set(double scaleZ);
		//double AS3_z_get();
		//void AS3_z_set(double z);
		
		//new adds
		Vector3DObject* AS3_globalToLocal3D(PointObject *pPoint);
		PointObject* AS3_local3DToGlobal(Vector3DObject *pPoint3D);
		//new adds end
		 
		//new adds 11
		ScriptObject* AS3_metaData_get();
		void AS3_metaData_set(ScriptObject* metaData);
		//new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplayObjectObjectSlots m_slots_DisplayObjectObject;
};}
#endif
