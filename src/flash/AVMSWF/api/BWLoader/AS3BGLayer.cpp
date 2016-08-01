#include "StdAfxflash.h"
#include "XDrawDib.h"
#include "Raster_gl.h"
#include "ShellCore.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"
using namespace avmplus;
#include "AS3BGLayer.h"
#include "avm2.h"


#define MINDIVSIZE 192
#define SCALEONE 1024
namespace avmshell{
BXBGLayerClass::BXBGLayerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BXBGLayerObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BXBGLayerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BXBGLayerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

//#undef _BACKLAYER

BXBGLayerObject::BXBGLayerObject(VTable* _vtable, ScriptObject* _delegate, int capacity):SpriteObject(_vtable,_delegate,capacity)
{
//	m_offx=0;
//	m_offy=0;
	this->m_bScrollMode=false;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	m_pPlayer=player;

}

void BXBGLayerObject::AS3_scrollMode_set(bool bs)
{

}

void BXBGLayerObject::AS3_x_set(double v)
{
	if(MathUtils::isNaN(v))
		v=0;

	SpriteObject::AS3_x_set(v);

	//m_offx=v;
}
double BXBGLayerObject::AS3_x_get()
{
	
	return SpriteObject::AS3_x_get();

}
void BXBGLayerObject::AS3_y_set(double v)
{
	
	if(MathUtils::isNaN(v))
		v=0;

	SpriteObject::AS3_y_set(v);

	//m_offy=v;
}
double BXBGLayerObject::AS3_y_get()
{
	
	return SpriteObject::AS3_y_get();

}

void BXBGLayerObject::AS3_showBG()
{

	//m_pPlayer->CreateBGDisplay();
}

void BXBGLayerObject::AS3_hideBG()
{
	//return;

}

/*void BXBGLayerObject::AS3_release()
{
#ifdef _BACKLAYER
	m_pPlayer->m_bBGLayer=false;
#endif
}
void BXBGLayerObject::AS3_create()
{
#ifdef _BACKLAYER
	m_pPlayer->m_bBGLayer=true;
	//if(!m_pPlayer->m_pBGDisplay)
	if(m_pPlayer->bgBits.IsEmpty())
		m_pPlayer->CreateBGDisplay();
#endif
}*/
void BXBGLayerObject::AS3_constructor()
{
//#ifdef _BACKLAYER

//#else
	SpriteObject::AS3_constructor();

//#endif
}

}
