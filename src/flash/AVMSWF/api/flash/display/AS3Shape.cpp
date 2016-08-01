#include "StdAfxflash.h"
#include "sobject.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Shape.h"
#include "AS3Graphics.h"

namespace avmshell{
ShapeClass::ShapeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ShapeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ShapeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShapeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ShapeObject* ShapeClass::CreateObject(void*pObject)
{
	ShapeObject*pObj=(ShapeObject*)createInstance(ivtable(),prototype);
	pObj->m_pObject=(_XSObject*)pObject;
	pObj->AS3_constructor();
	return pObj;
}


ShapeObject::ShapeObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pGraphics=NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
GraphicsObject* ShapeObject::AS3_graphics_get()
{
	//Add your act code here...
	if(m_pGraphics==NULL)
	{
		m_pGraphics=CreateGraphics();
	}
	//m_pGraphics->AS3_clear();
	return m_pGraphics;
}

//AS3 contructor function..
void ShapeObject::AS3_constructor()
{
	//Add your act code here...
	if(!m_pObject)
		DisplayObjectObject::CreateNewObject(shapeEmptyChar);
	//else type=m_pObject->character->tag;
}

}