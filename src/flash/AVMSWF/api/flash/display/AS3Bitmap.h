#ifndef _AS3Bitmap_
#define _AS3Bitmap_

#include "AS3BitmapData.h"
#include "AS3DisplayObject.h"
class SBitmapCore;

namespace avmplus{namespace NativeID{
class BitmapClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class BitmapObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BitmapDataObject;
	class MatrixObject;
	class ColorTransformObject;
	class RectangleObject;
	class BitmapObject;
	class BitmapClass : public ClassClosure
	{
	public:
		BitmapClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		BitmapObject* CreateBitmap(int w,int h,bool bTransparent);
		BitmapObject* CreateBitmap(XImage*pImage,ShellCodeContext*pCode,void*obj);
		BitmapObject* CreateSBitmap(void*ch,ShellCodeContext*pCode,void*obj);
		BitmapObject* CreateBitmap(BitmapDataObject*pData,ShellCodeContext*pCnt);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapClassSlots m_slots_BitmapClass;
};
class BitmapObject : public DisplayObjectObject
{
	public:
		BitmapObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:
		DRCWB(BitmapDataObject*) m_pBitmapData;//Set your data!!
		DRCWB(Stringp)			 m_strPixelSnapping;
		bool					 m_bSmoothing;
		//SBitmapCore				 m_sbitmap;
	public:
		virtual void Draw(BitmapDataObject*pDst,MatrixObject*pMatrix,ColorTransformObject*pTransform,Stringp blendMode,RectangleObject*clip,bool sm)
		{
			if(m_pBitmapData&&!m_pBitmapData->IsEmptyBits())
				m_pBitmapData->Draw(pDst,pMatrix,pTransform,blendMode,clip,sm);
			else
				return DisplayObjectObject::Draw(pDst,pMatrix,pTransform,blendMode,clip,sm);
		}
		BitmapDataObject* AS3_bitmapData_get();
		void AS3_bitmapData_set(BitmapDataObject*);
		Stringp AS3_pixelSnapping_get();
		void AS3_pixelSnapping_set(Stringp);
		bool AS3_smoothing_get();
		void AS3_smoothing_set(bool);
		void AS3_constructor(BitmapDataObject*,Stringp,bool);
		void Create(int w,int h,bool bTransparent);
		void CreateS(void*s);
		void Create(XImage*pImage);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapObjectSlots m_slots_BitmapObject;
};}
#endif