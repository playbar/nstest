#ifndef _AS3BXDeviceBitmapData_
#define _AS3BXDeviceBitmapData_

class XDrawDib;
class SBitmapCore;
#include "AS3DisplayObject.h"
namespace avmplus{namespace NativeID{
class BXDeviceBitmapDataClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class BXDeviceBitmapDataObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class RectangleObject;
	class PointObject;
	class BitmapFilterObject;
	class BXDeviceBitmapDataObject;
	class ColorTransformObject;
	class MatrixObject;
	class ByteArrayObject;
	class DisplayObjectObject;
	class BXDeviceBitmapDataClass : public ClassClosure
	{
	public:
		BXDeviceBitmapDataClass(VTable *vtable);
		virtual ScriptObject* createInstance(VTable* ivtable, ScriptObject* prototype);
		bool AS3_isMobile(){return true;}
		BXDeviceBitmapDataObject* AS3_GetDeviceBitmap(int,int,int,int);
		BXDeviceBitmapDataObject* AS3_GetDeviceBitmapObj(DisplayObjectObject*obj);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXDeviceBitmapDataClassSlots m_slots_BXDeviceBitmapDataClass;
};
class BXDeviceBitmapDataObject : public FlashObject//ScriptObject
{
	public:
		//MATRIX m_totalMatrix;
		bool m_bIsLocked;
		int  m_nScaleX,m_nScaleY;
		int  m_nOffsetX,m_nOffsetY;
		int  m_nNoScaleX,m_nNoScaleY;
		void TransformSize(int&x,int&y);
		void Transform(int&x,int&y);
		void Transform(SPOINT&pt);
		void Transform(int&x,int&y,int&w,int&h);
		void Transform(XRect&rc,bool offset);
		void DivPoint(XPoint&pt,int nDiv)
		{
			pt.x>>=nDiv;
			pt.y>>=nDiv;
		}
		void DivRect(XRect&rect,int nDiv)
		{
			rect.left>>=nDiv;
			rect.top>>=nDiv;
			rect.right>>=nDiv;
			rect.bottom>>=nDiv;
		}
		void InitDevice(int x,int y,int w,int h);
		void LimitRect(XRect&rc);
		BXDeviceBitmapDataObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		virtual void SetObject(void*pObject);
	public:
		virtual void Draw(BitmapDataObject*pDst,MATRIX&mat,ColorTransformObject*pTransform,Stringp blendMode,XRect&rc,bool sm);
		bool HitTest(XPoint&pt,uint32_t alpha,BitmapDataObject*pBmp,PointObject*second,uint32_t alpha2);
	public:
		//void ExpandDummy(XBOOL bWrite);
		//void CreateDummyDib(void*ch);
		//void DivImage(int nNewDiv);
		//SBitmapCore	m_sbitmap;
		//Modify BUG for 2.7.0
		//void*		m_pRef[2];
		//2.7.0 End
		//XDrawDib*   m_pDib;
		//_XSObject*  m_pObject;
		//void*	    m_pChar;
		//XU32		m_nLockTime;
		//XU32		m_nCacheTime;
		//XU32		m_nLockTimes;
		//XU32		m_nStartTime;
		//int			m_nImgDiv;
		//bool	  m_bTransparent;
		//bool Create(XImage*pImage);
		//bool Create(void*);
		
		void Clear();
		int AS3_height_get();
		RectangleObject* AS3_rect_get();
		bool AS3_transparent_get();
		int AS3_width_get();
		void AS3_constructor(int w,int h,bool transparent,uint32_t fillColor);
		void AS3_applyFilter(BitmapDataObject*,RectangleObject*,PointObject*,BitmapFilterObject*);
		BitmapDataObject* AS3_clone();
		void AS3_colorTransform(RectangleObject*,ColorTransformObject*);
		Atom AS3_compare(BitmapDataObject*);
		void AS3_copyChannel(BitmapDataObject*,RectangleObject*,PointObject*,uint32_t,uint32_t);
		void AS3_copyPixels(BitmapDataObject*,RectangleObject*,PointObject*,BitmapDataObject*,PointObject*,bool);
		void AS3_dispose();
		void AS3_draw(Atom s,MatrixObject*,ColorTransformObject*,Stringp,RectangleObject*,bool);
		void AS3_fillRect(RectangleObject*,uint32_t);
		void AS3_floodFill(int,int,uint32_t);
		RectangleObject* AS3_generateFilterRect(RectangleObject*,BitmapFilterObject*);
		RectangleObject* AS3_getColorBoundsRect(uint32_t,uint32_t,bool);
		uint32_t AS3_getPixel(int,int);
		uint32_t AS3_getPixel32(int,int);
		ByteArrayObject* AS3_getPixels(RectangleObject*);
		bool AS3_hitTest(PointObject*,uint32_t,Atom,PointObject*,uint32_t);
		void AS3_lock();
		void AS3_merge(BitmapDataObject*,RectangleObject*,PointObject*,uint32_t,uint32_t,uint32_t,uint32_t);
		void AS3_noise(int,uint32_t,uint32_t,uint32_t,bool);
		void AS3_paletteMap(BitmapDataObject*,RectangleObject*,PointObject*,ArrayObject*,ArrayObject*,ArrayObject*,ArrayObject*);
		void AS3_perlinNoise(double,double,uint32_t,int,bool,bool,uint32_t,bool,ArrayObject*);
		int AS3_pixelDissolve(BitmapDataObject*,RectangleObject*,PointObject*,int,int,uint32_t);
		void AS3_scroll(int,int);
		void AS3_setPixel(int,int,uint32_t);
		void AS3_setPixel32(int,int,uint32_t);
		void AS3_setPixels(RectangleObject*,ByteArrayObject*);
		uint32_t AS3_threshold(BitmapDataObject*,RectangleObject*,PointObject*,Stringp,uint32_t,uint32_t,uint32_t,bool);
		void AS3_unlock(RectangleObject*);

		//new adds	
		UIntVectorObject* AS3_getVector(RectangleObject *pRect);
		ObjectVectorObject* AS3_histogram(RectangleObject *pHRect);
		void AS3_setVector(RectangleObject *pRect, UIntVectorObject *pInputVector);
		//new adds end


	public:
		int		m_nOffX,m_nOffY;
		int		m_nAntiX,m_nAntiY;
		int		m_nWidth,m_nHeight;
		XRaster* m_pRaster;
		char*	m_pAddr;
		int	    m_nScaleLine;
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BXDeviceBitmapDataObjectSlots m_slots_BXDeviceBitmapDataObject;
};}
#endif
