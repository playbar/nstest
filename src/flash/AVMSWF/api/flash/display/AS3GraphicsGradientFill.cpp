#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsGradientFill.h"
namespace avmshell{
GraphicsGradientFillClass::GraphicsGradientFillClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsGradientFillClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsGradientFillObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsGradientFillObject::GraphicsGradientFillObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//ArrayObject* GraphicsGradientFillObject::AS3_alphas_get()
//{
//	return NULL;
//}
//void GraphicsGradientFillObject::AS3_alphas_set(ArrayObject *pAlphas)
//{
//
//}
//ArrayObject* GraphicsGradientFillObject::AS3_colors_get()
//{
//	return NULL;
//}
//void GraphicsGradientFillObject::AS3_colors_set(ArrayObject *pColors)
//{
//
//}
//double GraphicsGradientFillObject::AS3_focalPointRatio_get()
//{
//	return 0;
//}
//void GraphicsGradientFillObject::AS3_focalPointRatio_set(double focalPointRatio)
//{
//
//}
//Stringp GraphicsGradientFillObject::AS3_interpolationMethod_get()
//{
//	return NULL;
//}
//void GraphicsGradientFillObject::AS3_interpolationMethod_set(Stringp pInterpolationMethod)
//{
//
//}
//MatrixObject* GraphicsGradientFillObject::AS3_matrix_get()
//{
//	return NULL;
//}
//void GraphicsGradientFillObject::AS3_matrix_set(MatrixObject *pMatrix)
//{
//
//}
//ArrayObject* GraphicsGradientFillObject::AS3_ratios_get()
//{
//	return NULL;
//}
//void GraphicsGradientFillObject::AS3_ratios_set(ArrayObject *pRatios)
//{
//
//}
//Stringp GraphicsGradientFillObject::AS3_spreadMethod_get()
//{
//	return NULL;
//}
//void GraphicsGradientFillObject::AS3_spreadMethod_set(Stringp pSpreadMethod)
//{
//
//}
//Stringp GraphicsGradientFillObject::AS3_type_get()
//{
//	return NULL;
//}
//void GraphicsGradientFillObject::AS3_type_set(Stringp pType)
//{
//
//}
//void GraphicsGradientFillObject::AS3_constructor(Stringp pType, ArrayObject *pColors, ArrayObject *pAlphas, 
//					   ArrayObject *pRatios, Atom matrix, Atom spreadMethod, 
//					   Stringp pInterpolationMethod, double focalPointRatio)
//{
//
//}

}