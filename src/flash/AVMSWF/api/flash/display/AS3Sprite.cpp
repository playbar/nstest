#include "StdAfxflash.h"
#include "splayer.h"
#include "sobject.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Graphics.h"
#include "AS3Sprite.h"
#include "AS3Rectangle.h"
#include "AS3SoundTransform.h"
namespace avmshell{
SpriteClass::SpriteClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SpriteObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SpriteClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SpriteObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SpriteObject::SpriteObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectContainerObject(vtable, proto, 0)
{

	m_pGraphics=NULL;
	m_pSoundTransformObject=NULL;
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool SpriteObject::AS3_buttonMode_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsButtonMode();
	return false;
}

void SpriteObject::AS3_buttonMode_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
	{
		//if(b)
		//	m_pObject->thread->StopPlay();
		m_pObject->SetButtonMode(b);
	}
}

DisplayObjectObject* SpriteObject::AS3_dropTarget_get()
{
	//Add your act code here...
	if(m_pObject)
	{
		SObject* t=m_pObject->dropTarget;
		if(!t) return NULL;
		if(!t->pASObject)
		{
			void* pSuper=t->AS3CreateInst(NULL,XTRUE);
			t->AS3ConstructInst(pSuper);
		}
		return (DisplayObjectObject*)t->pASObject;
		
	}
	return NULL;
}

GraphicsObject* SpriteObject::AS3_graphics_get()
{
	//Add your act code here...
	if(m_pGraphics==NULL)
	{
		m_pGraphics=CreateGraphics();
	}
	return m_pGraphics;//Modify this please!
}

SpriteObject* SpriteObject::AS3_hitArea_get()
{
	//Add your act code here...
	//if(m_pObject&&m_pObject->pLink)
	//{
	//	return (SpriteObject*)m_pObject->pLink->pASObject;
	//}
	return m_hitArea;//NULL;
}

void SpriteObject::AS3_hitArea_set(SpriteObject*pObj)
{
	//Add your act code here...
	if(m_pObject&&pObj&&pObj->m_pObject)
	{
		m_pObject->LinkBy(pObj->m_pObject);
	}
	m_hitArea=pObj;
}

SoundTransformObject* SpriteObject::AS3_soundTransform_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pSoundTransformObject==NULL)
	{
		SoundTransformClass*pClass=((ShellToplevel*)toplevel())->getSoundTransformClass();
		m_pSoundTransformObject=pClass->CreateTransform();
	}
	return m_pSoundTransformObject;
}

void SpriteObject::AS3_soundTransform_set(SoundTransformObject*p)
{
	//Add your act code here...
	m_pSoundTransformObject=p;
}

bool SpriteObject::AS3_useHandCursor_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
		return m_pObject->IsHandCursor();
	return false;
}

void SpriteObject::AS3_useHandCursor_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetHandCursor(b);
}

//AS3 contructor function..
void SpriteObject::AS3_constructor()
{
	//Add your act code here...

	//StUTF8String s(vtable->traits->name());
	//if(strcmp(s.c_str(),"MoleShop")==0)
	//	int v=0;
	//StUTF8String s(vtable->traits->name());
	//if(strcmp(s.c_str(),"QuestTracePanel")==0)
	//		int v=0;

	if(m_pObject==NULL)
	{
		DisplayObjectObject::CreateNewObject(spriteEmptyChar);//CreateNewObject(
	}
	if(m_pObject)
	{

		FillMembers();

		m_pObject->thread->CMDStop();//StopPlay();
		//[Note X]
		m_pObject->thread->StopPlay();
	}
		//return kAvmThunkUndefined;//Modify this please!
}

void SpriteObject::InitObject()
{
	if(m_pObject==NULL)
	{

		DisplayObjectObject::CreateNewObject(spriteEmptyChar);//CreateNewObject(
	}

}

void SpriteObject::AS3_startDrag(bool bLockCenter,RectangleObject*r)
{
	//Add your act code here...
	//return;
	if(m_pObject)
	{
		SRECT rect;
		if(r)
		{
			AS3RECTANGLEDATA&data=r->GetData();
			rect.xmin=(XINT)((data.left<=data.left+data.width?data.left:data.left+data.width)*20);
			rect.ymin=(XINT)((data.top<=data.top+data.height?data.top:data.top+data.height)*20);
			rect.xmax=(XINT)((data.left>=data.left+data.width?data.left:data.left+data.width)*20);
			rect.ymax=(XINT)((data.top>=data.top+data.height?data.top:data.top+data.height)*20);
		}
		else
			::RectSetEmpty(&rect);
		ShellCore*c=(ShellCore*)core();
		c->GetPlayer()->StartDrag(m_pObject,rect,bLockCenter,r!=NULL);
	}
}

void SpriteObject::AS3_stopDrag()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	ShellCore*c=(ShellCore*)core();
	c->GetPlayer()->StopDrag();
}

//new adds
void SpriteObject::AS3_startTouchDrag(int touchPointID, bool lockCenter, RectangleObject *pBounds)
{
}
void SpriteObject::AS3_stopTouchDrag(int touchPointID)
{
}

//new adds end


}