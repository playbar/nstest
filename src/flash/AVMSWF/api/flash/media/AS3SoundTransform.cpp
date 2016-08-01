#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SoundTransform.h"
namespace avmshell{
SoundTransformClass::SoundTransformClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SoundTransformObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SoundTransformClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	SoundTransformObject*pObj=(SoundTransformObject*)new (core()->GetGC(), ivtable->getExtraSize()) SoundTransformObject(ivtable, prototype, 0);
	m_nOffset=(char*)(&pObj->m_trans)-((char*)pObj);
	return pObj;
	//AS3 'new' opt...Add your init code here...
}

SoundTransformObject* SoundTransformClass::CreateTransform()
{
	SoundTransformObject*pObj=(SoundTransformObject*)createInstance(ivtable(),prototype);

	return pObj;
}

SoundTransformObject::SoundTransformObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_trans.m_dLeftToLeft=256;
	m_trans.m_dLeftToRight=0;
	m_trans.m_dPan=0;
	m_trans.m_dRightToLeft=0;
	m_trans.m_dRightToLeft=256;
	m_trans.m_dVolume=256;
}
//////////////////////////////////////////////////////////
//Native Method start...
/*AvmBox SoundTransformObject::AS3_leftToLeft_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_leftToLeft_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_leftToRight_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_leftToRight_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_pan_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_pan_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_rightToLeft_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_rightToLeft_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_rightToRight_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_rightToRight_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_volume_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox SoundTransformObject::AS3_volume_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
AvmBox SoundTransformObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}*/

}