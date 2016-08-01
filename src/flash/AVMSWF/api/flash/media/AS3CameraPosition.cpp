#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CameraPosition.h"
namespace avmshell
{
	CameraPositionClass::CameraPositionClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* CameraPositionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) CameraPositionObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	CameraPositionObject::CameraPositionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}