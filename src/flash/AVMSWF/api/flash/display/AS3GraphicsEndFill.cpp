#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsEndFill.h"
namespace avmshell{
GraphicsEndFillClass::GraphicsEndFillClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsEndFillClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsEndFillObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsEndFillObject::GraphicsEndFillObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
//void GraphicsEndFillObject::AS3_constructor()
//{
//	//Add your act code here...
//}

}