#ifndef _AS3BitmapData_
#define _AS3BitmapData_

class XDrawDib;
class SBitmapCore;
class FrameBuffer;
#include "AS3DisplayObject.h"
#include "SBitmap.h"
#include "SObject.h"
namespace avmplus{namespace NativeID{
class BitmapDataClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class BitmapDataObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class RectangleObject;
	class PointObject;
	class BitmapFilterObject;
	class BitmapDataObject;
	class ColorTransformObject;
	class MatrixObject;
	class ByteArrayObject;
	class IBitmapDrawableObject;
	class BitmapDataClass : public ClassClosure
	{
	public:
		BitmapDataClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		BitmapDataObject*CreateBitmapData(int w,int h,bool bTransparent);
		BitmapDataObject*CreateBitmapData(XImage*pImage);
		//BitmapDataObject*CreateBitmapData(void*s);
		BitmapDataObject*CreateDumpBitmap(void*s);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapDataClassSlots m_slots_BitmapDataClass;
};
class BitmapDataObject : public FlashObject//ScriptObject
{
	public:
		bool SaveIfNeed(XU32 t,XU32 tMax);
		bool RestoreIfNeed();
#ifdef _ANALY
		virtual void Report(XSortMemName*pInfo);
#endif
		/*void DivPoint(XPoint&pt,int nDiv)
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
		}*/
		BitmapDataObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		virtual void SetObject(void*pObject);
	public:
		void SetModify(){m_bModify=true;}
		/*bool IsFrame()
		{
			if(!m_pChar) return false;
			if(m_pChar->type != bitmapEmptyChar)
				return false;
			return true;
		}*/
		void Encode(XU8Array&output);
		virtual void Draw(BitmapDataObject*pDst,MatrixObject*pMatrix,ColorTransformObject*pTransform,Stringp blendMode,RectangleObject*clip,bool sm);
		bool HitTest(XPoint&pt,uint32_t alpha,BitmapDataObject*pBmp,PointObject*second,uint32_t alpha2);
	public:
		//bool IsEmpty();
		bool IsEmptyBits(){return !m_pChar||!m_pChar->bits.HasBits();}
		bool IsEmptyTexture(){return !m_pChar||!m_pChar->bits.bitmapID;}
		_TEXTUREID* GetBmInfo(TextureInfo&info);
		bool  IsTransparent();
		XBOOL GetDib(XDrawDib&dib);
		void CreateTexture();
		//void ExpandDummy(XBOOL bWrite);
		//void CreateDummyDib(void*ch);
		//void DivImage(int nNewDiv);
		//SBitmapCore	m_sbitmap;
		SCharacter*  m_pChar;
		//FrameBuffer* m_pBuffer;
		//bool		 m_bAttach;
		//bool		 m_bModify;
		//Modify BUG for 2.7.0
		//void*		m_pRef[2];
		//int		     m_nWidth,m_nHeight,m_nDiv;
		//bool		 m_bTrans;,m_bSaved;
		bool m_bSaved,m_bTextured;
		//2.7.0 End
		//XDrawDib*   m_pDib;

		_XSObject*  m_pObject;
		//void*	    m_pChar;
		//XU32		m_nLockTime;
		//XU32		m_nCacheTime;
		//XU32		m_nLockTimes;
		//XU32		m_nStartTime;
		//int			m_nImgDiv;
		//bool	  m_bTransparent;
		bool Create(XImage*pImage);
		//bool Create(void*);
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
		void LockBitmap(bool forWrite);	
		void CheckEmpty();
		//void FormatBitmap();
		//bool IsOption(){return m_bOpted;}
//		void SameDib(BitmapDataObject*obj);

		//new adds	
		UIntVectorObject* AS3_getVector(RectangleObject *pRect);
		ObjectVectorObject* AS3_histogram(RectangleObject *pHRect);
		void AS3_setVector(RectangleObject *pRect, UIntVectorObject *pInputVector);	
		//new adds 11
		void AS3_copyPixelsToByteArray(RectangleObject* rect, ByteArrayObject* data);
		void AS3_drawWithQuality(Atom source, MatrixObject* matrix = NULL, ColorTransformObject* colorTransform = NULL, Stringp blendMode = NULL, RectangleObject* clipRect = NULL, bool smoothing = false, Stringp quality = NULL);
 		ByteArrayObject* AS3_encode(RectangleObject* rect, ScriptObject* compressor, ByteArrayObject* byteArray = NULL);
 		//new adds 11 end

	private:
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
		bool m_bModify;
		void SameDib(BitmapDataObject*obj);
		void DivImage(int nNewDiv);
		void ReleaseChar(_XSCharacter*ch);
		//bool	m_bOpted;
		//int		m_nWidth0,m_nHeight0;
		//inline void		AddBitmap();
		//inline void		RemoveBitmap();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapDataObjectSlots m_slots_BitmapDataObject;
};}
#endif
