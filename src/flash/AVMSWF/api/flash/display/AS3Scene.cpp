#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Scene.h"
namespace avmshell{
SceneClass::SceneClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SceneObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SceneClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SceneObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

SceneObject* SceneClass::CreateScene(Stringp name,int nFrames,int offset)
{
	SceneObject*pObj=(SceneObject*)createInstance(ivtable(),prototype);
	pObj->m_strName=name;
	pObj->m_nFrames=nFrames;
	pObj->m_nOffset=offset;
	return pObj;
}

SceneObject::SceneObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	m_nFrames=0;
	m_pLabels=toplevel()->arrayClass->newArray(0);
	m_strName=core()->kEmptyString;
	//Add your construct code here...
}

}