#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FrameLabel.h"
namespace avmshell{
FrameLabelClass::FrameLabelClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())FrameLabelObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* FrameLabelClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FrameLabelObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

FrameLabelObject* FrameLabelClass::CreateFrameLabel(int frame,Stringp name)
{
	FrameLabelObject*pObj=(FrameLabelObject*)createInstance(ivtable(),prototype);
	pObj->setName(name);
	pObj->setFrame(frame);
	pObj->MakeILabel();
	return pObj;
}

FrameLabelObject::FrameLabelObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_iLabel=-1;
}

void FrameLabelObject::MakeILabel()
{
	if(!m_strName) return;
	double v=MathUtils::parseInt(m_strName);
	if(v==MathUtils::kNaN) return;
	m_iLabel=(int)v;
}


}