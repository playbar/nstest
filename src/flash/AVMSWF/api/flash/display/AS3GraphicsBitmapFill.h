#ifndef _AS3GraphicsBitmapFill_
#define _AS3GraphicsBitmapFill_

#include "AS3Matrix.h"
#include "AS3BitmapData.h"

namespace avmplus{namespace NativeID{

class MatrixObject;
class BitmapDataObject;

class GraphicsBitmapFillClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsBitmapFillObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	BitmapDataObject *bitmapData;
	MatrixObject *matrix;
	XBOOL repeat;
	XBOOL smooth;
private:};
}}
namespace avmshell{
	class GraphicsBitmapFillClass : public ClassClosure
	{
	public:
		GraphicsBitmapFillClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsBitmapFillClassSlots m_slots_GraphicsBitmapFillClass;
};
class GraphicsBitmapFillObject : public ScriptObject
{
	public:
		GraphicsBitmapFillObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
//		void* pData;//Set your data!!
		//BitmapDataObject* AS3_bitmapData_get();
		//void AS3_bitmapData_set(BitmapDataObject *pBitmapData);
		//MatrixObject* AS3_matrix_get();
		//void AS3_matrix_set(MatrixObject *pMatrix);
		//bool AS3_repeat_get();
		//void AS3_repeat_set(bool repeat);
		//bool AS3_smooth_get();
		//void AS3_smooth_set(bool smooth);
		//void AS3_constructor(BitmapDataObject *pBitmapData, MatrixObject *pMatrix, bool repeat, bool smooth);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsBitmapFillObjectSlots m_slots_GraphicsBitmapFillObject;
};}
#endif