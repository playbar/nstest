#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsSolidFill.h"
namespace avmshell{
GraphicsSolidFillClass::GraphicsSolidFillClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsSolidFillClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsSolidFillObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsSolidFillObject::GraphicsSolidFillObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double GraphicsSolidFillObject::AS3_alpha_get()
//{
//	return 0;
//}
//void GraphicsSolidFillObject::AS3_alpha_set(double alpha)
//{
//
//}
//uint32_t GraphicsSolidFillObject::AS3_color_get()
//{
//	return 0;
//}
//void GraphicsSolidFillObject::AS3_color_set(uint32_t color)
//{
//
//}
//void GraphicsSolidFillObject::AS3_constructor(uint32_t color, double alpha)
//{
//
//}

}