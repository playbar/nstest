#include "StdAfxflash.h"
#include "sobject.h"
#include "stags.h"
#include "sdisplay.h"
#include "ShellCore.h"

using namespace avmplus;
#include "avm2.h"
#include "splayer.h"

#include "AS3SimpleButton.h"
#include "AS3Sprite.h"
namespace avmshell{
SimpleButtonClass::SimpleButtonClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SimpleButtonObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SimpleButtonClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SimpleButtonObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SimpleButtonObject::SimpleButtonObject(VTable *vtable, ScriptObject* proto, int capacity): InteractiveObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pDownState=NULL;
	m_pUpState=NULL;
	m_pOverState=NULL;
	m_pHitState=NULL;
	m_pTransform=NULL;
}

void SimpleButtonObject::ReleaseObject()
{
	DisplayObjectObject* objs[]={m_pDownState,m_pUpState,m_pOverState,m_pHitState};
	for(int i=0;i<4;i++)
	{
		if(!objs[i]||!objs[i]->m_pObject) continue;
		_XSObject*obj=objs[i]->m_pObject;
		obj->FreeChildren(true);
		obj->FreeContext(true);
		obj->display->FreeObject(obj);
		objs[i]->ReleaseObject();
	}

	InteractiveObjectObject::ReleaseObject();
}

bool SimpleButtonObject::AS3_enabled_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsEnable();
	return false;
}

void SimpleButtonObject::AS3_enabled_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetEnable(b);
}


bool SimpleButtonObject::AS3_trackAsMenu_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
		return m_pObject->IsTrackAsMenu();
	return false;
}

void SimpleButtonObject::AS3_trackAsMenu_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetTrackAsMenu(b);
}


bool SimpleButtonObject::AS3_useHandCursor_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsHandCursor();
	return false;
}

void SimpleButtonObject::AS3_useHandCursor_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetHandCursor(b);
}

void SimpleButtonObject::AddChildRef(ScriptObject*obj)
{
	if(m_pChildren==NULL)
	{
		ShellCore*c=(ShellCore*)core();
		ShellToplevel*top=(ShellToplevel*)toplevel();
		m_pChildren=new(c->GetGC())RCHashTable((ScriptObject*)top->m_pDumpObject.value(),4,true);
	}
	//if(m_pChildren->indexOf(obj)>=0) return;
	m_pChildren->put(obj);
}
void SimpleButtonObject::RemoveChildRef(ScriptObject*obj)
{
	if(m_pChildren==NULL) return;
	m_pChildren->remove(obj,true);
}

void SimpleButtonObject::AS3_downState_set(DisplayObjectObject*p)
{
	if(m_pObject)
	{
		if(m_pObject->state==bsOverDown)
		{		
			DisplayList*display=m_pObject->display;
			XAVM2*pAVM2=display->player->m_pAVM2;
			if(m_pDownState)
			{
				DisplayObjectObject*pObj=m_pDownState;//pAVM2->GetButtonObject(this,bsOverDown,XAS3_BUT_REMOVE);
				pObj->m_bNewObject=true;
				//pAVM2->RemoveObject(pObj);
				//RemoveChildRef(pObj);
				display->AS3RemoveObject(m_pObject,pObj->m_pObject);
			}
			if(p&&p->m_pObject)
			{
				p->m_bNewObject=false;
				//pAVM2->PushObject(p);
				//AddChildRef(pObj);
				display->AS3PlaceObject(m_pObject,p->m_pObject,-1);
			}
			m_pObject->Modify();
		}
	}
	m_pDownState=p; 
}

void SimpleButtonObject::AS3_overState_set(DisplayObjectObject*p)
{
	if(m_pObject)
	{
		if(m_pObject->state==bsOverUp)
		{		
			DisplayList*display=m_pObject->display;
			XAVM2*pAVM2=display->player->m_pAVM2;
			if(m_pOverState)
			{
				DisplayObjectObject*pObj=m_pOverState;//pAVM2->GetButtonObject(this,bsOverDown,XAS3_BUT_REMOVE);
				pObj->m_bNewObject=true;
				//pAVM2->RemoveObject(pObj);
				//RemoveChildRef(pObj);
				display->AS3RemoveObject(m_pObject,pObj->m_pObject);
			}
			if(p&&p->m_pObject)
			{
				p->m_bNewObject=false;
				//pAVM2->PushObject(p);
				//AddChildRef(p);
				display->AS3PlaceObject(m_pObject,p->m_pObject,-1);
			}
			m_pObject->Modify();
		}
	}
	m_pOverState=p; 
}

void SimpleButtonObject::AS3_upState_set(DisplayObjectObject*p)
{
	if(m_pObject)
	{
		if(m_pObject->state==bsIdle||m_pObject->state==bsOverDown)
		{		
			DisplayList*display=m_pObject->display;
			XAVM2*pAVM2=display->player->m_pAVM2;
			if(m_pUpState)
			{
				DisplayObjectObject*pObj=m_pUpState;//pAVM2->GetButtonObject(this,bsOverDown,XAS3_BUT_REMOVE);
				pObj->m_bNewObject=true;
				//pAVM2->RemoveObject(pObj);
				display->AS3RemoveObject(m_pObject,pObj->m_pObject);
			}
			if(p&&p->m_pObject)
			{
				p->m_bNewObject=false;
				//pAVM2->PushObject(p);
				display->AS3PlaceObject(m_pObject,p->m_pObject,-1);
			}
			m_pObject->Modify();
		}
	}
	m_pUpState=p;
}


static DisplayObjectObject* CheckState(DisplayObjectObject*obj,ShellToplevel*top)
{
	if(obj&&obj->m_pObject)
	{
		_XSObject*state=obj->m_pObject;
		_XSObject*child=state->bottomChild;
		if(!child->above&&child->pASObject)
		{
			obj->m_pObject->bottomChild=NULL;
			_XSObject*parent=obj->m_pObject->parent;
			child->parent=parent;//obj->m_pObject->parent;
			if(parent)
			{
				_XSObject**root=&parent->bottomChild;
				for(;;)
				{
					_XSObject*o=*root;
					if(o==state)
					{
						*root=child;
						break;
					}
					root=&o->above;
				}	
			}
			//else
			//	top->RemoveObject((ScriptObject*)child->pASObject);
			return (DisplayObjectObject*)child->pASObject;
		}
	}
	return obj;
}

void SimpleButtonObject::FormatState()
{
	//return;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	m_pUpState=CheckState(m_pUpState,top);
	m_pDownState=CheckState(m_pDownState,top);
	m_pHitState=CheckState(m_pHitState,top);
	m_pOverState=CheckState(m_pOverState,top);

	/*if(m_pUpState&&m_pUpState->m_pObject)
	{
		_XSObject*child=m_pUpState->m_pObject->bottomChild;
		if(!child->above&&child->pASObject)
			m_pUpState=(DisplayObjectObject*)child->pASObject;
	}
	if(m_pDownState&&m_pDownState->m_pObject)
	{
		_XSObject*child=m_pDownState->m_pObject->bottomChild;
		if(!child->above&&child->pASObject)
			m_pDownState=(DisplayObjectObject*)child->pASObject;
	}
	if(m_pOverState&&m_pOverState->m_pObject)
	{
		_XSObject*child=m_pOverState->m_pObject->bottomChild;
		if(!child->above&&child->pASObject)
			m_pOverState=(DisplayObjectObject*)child->pASObject;
	}
	if(m_pHitState&&m_pHitState->m_pObject)
	{
		_XSObject*child=m_pHitState->m_pObject->bottomChild;
		if(!child->above&&child->pASObject)
			m_pHitState=(DisplayObjectObject*)child->pASObject;
	}*/
	
}

DisplayObjectObject* SimpleButtonObject::CreateStateObject()
{
	SpriteClass*pClass=((ShellToplevel*)toplevel())->getSpriteClass();
	DisplayObjectObject*pObj=(DisplayObjectObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		//(DisplayObjectObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
	pObj->SetCodeContext(m_codeContext);
	//pObj->m_pCodeContext=m_pCodeContext;
	pObj->CreateNewObject(spriteEmptyChar);
	return pObj;
}

void SimpleButtonObject::AS3_constructor(DisplayObjectObject*up,DisplayObjectObject*over,DisplayObjectObject*down,DisplayObjectObject*hit)
{	


	SCharacter*ch=(SCharacter*)this->vtable->GetInitData();

	if(!m_pObject)
	{
		m_pUpState=up;
		m_pDownState=down;
		m_pOverState=over;
		m_pHitState=hit;
		CreateNewObject(buttonEmptyChar);		
	}
	else //if(!up||!down||!over||!hit)
	{
		/*int nState=0;
		if(!up) nState|=sbtnUpState;
		if(!down) nState|=sbtnDownState;
		if(!hit) nState|=sbtnHitTestState;
		if(!over) nState|=sbtnOverState;
		if(nState)
			m_pObject->display->AS3SetStateObjects(m_pObject,this,nState);*/
		//m_pObject->AS3SetStateObjects(this);
	}
	AvmAssert(m_pObject!=NULL);

	m_pObject->display->UpdateButton(m_pObject,bsIdle,true);
	
}

}
