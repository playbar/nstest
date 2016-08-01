#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3H264Level.h"
namespace avmshell
{
	H264LevelClass::H264LevelClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* H264LevelClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) H264LevelObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	H264LevelObject::H264LevelObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}