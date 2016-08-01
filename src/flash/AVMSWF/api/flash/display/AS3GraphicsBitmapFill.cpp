#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsBitmapFill.h"
namespace avmshell{
GraphicsBitmapFillClass::GraphicsBitmapFillClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsBitmapFillClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsBitmapFillObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsBitmapFillObject::GraphicsBitmapFillObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//BitmapDataObject* GraphicsBitmapFillObject::AS3_bitmapData_get()
//{
//	return NULL;
//}
//void GraphicsBitmapFillObject::AS3_bitmapData_set(BitmapDataObject *pBitmapData)
//{
//
//}
//MatrixObject* GraphicsBitmapFillObject::AS3_matrix_get()
//{
//	return NULL;
//}
//void GraphicsBitmapFillObject::AS3_matrix_set(MatrixObject *pMatrix)
//{
//
//}
//bool GraphicsBitmapFillObject::AS3_repeat_get()
//{
//	return false;
//}
//void GraphicsBitmapFillObject::AS3_repeat_set(bool repeat)
//{
//
//}
//
//bool GraphicsBitmapFillObject::AS3_smooth_get()
//{
//	return false;
//}
//void GraphicsBitmapFillObject::AS3_smooth_set(bool smooth)
//{
//
//}
//void GraphicsBitmapFillObject::AS3_constructor(BitmapDataObject *pBitmapData, MatrixObject *pMatrix, bool repeat, bool smooth)
//{
//
//}

}