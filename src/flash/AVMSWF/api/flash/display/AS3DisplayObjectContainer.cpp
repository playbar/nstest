#include "StdAfxflash.h"
#include "sobject.h"
#include "sdisplay.h"
#include "splayer.h"
#include "ShellCore.h"
#include "avm2.h"

using namespace avmplus;
#include "AS3DisplayObject.h"
#include "AS3DisplayObjectContainer.h"
#include "AS3TextSnapshot.h"
#include "AS3Point.h"
#include "AS3Event.h"
#include "AS3Bitmap.h"
#include "AS3BitmapData.h"
#include "AS3Loader.h"
#include "AS3Sprite.h"

//extern void* _DebugWindow;

namespace avmshell{
DisplayObjectContainerClass::DisplayObjectContainerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())DisplayObjectContainerObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DisplayObjectContainerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DisplayObjectContainerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DisplayObjectContainerObject::DisplayObjectContainerObject(VTable *vtable, ScriptObject* proto, int capacity):  InteractiveObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pAutoRemoved=NULL;	
}
//////////////////////////////////////////////////////////
//Native Method start...

void _PostEvent(int id,_XSObject*obj)
{
	//if(obj->thread&&!obj->thread->m_bInDisplay) return;
	//if(!obj->IsInDisplay()) return;
	if(!obj->parent) 
		return;
	if(obj->thread&&!obj->thread->m_bInDisplay) 
		return;
	if(obj->pASObject&&obj->character->type!=bitsChar)
	{
		((EventDispatcherObject*)obj->pASObject)->OnEvent(id,false,NULL,false);
	}
	obj=obj->bottomChild;
	while(obj)
	{
		_PostEvent(id,obj);
		obj=obj->above;
	}

}

//void DisplayObjectContainerObject::OnEventAll(int id,_XSObject*obj)
#define OnEventAll(ID,OBJ,INSTANCE)\
{\
	if((m_pObject->parent||!m_pObject->character)&&(!m_pObject->thread||m_pObject->thread->m_bInDisplay)){\
		if(INSTANCE&&INSTANCE!=OBJ->pASObject)\
			((EventDispatcherObject*)INSTANCE)->OnEvent(ID,false,NULL,false);\
		_PostEvent(ID,OBJ);}\
}

bool DisplayObjectContainerObject::AS3_mouseChildren_get()
{
	//Add your act code here...
	if(m_pObject)
		return (bool)(m_pObject->IsMouseChildren()!=0);
	return false;
}

void DisplayObjectContainerObject::Release()
{
	if(IsRunning())
	{
		/*if(m_pObject)
		{
			if(m_pObject->character->type==rootChar)
			{
				//m_pObject->character->player->AS3Unload(false);
				//EventDispatcherObject::Release();
				//delete m_pObject;
				//m_pObject=XNULL;
				//return;
				int v=0;
			}
		}*/
		if(m_pChildren!=NULL)
			m_pChildren->clear();
		//if(m_pObject)
		//	m_pObject->pASObject = NULL;
			//m_pObject->RemoveChildren();
		m_pChildren = NULL;
	}
	InteractiveObjectObject::Release();
}


void DisplayObjectContainerObject::AddChildRef(ScriptObject*obj)
{

	//if(obj==_DebugWindow)
	//	int v=0;
	ShellCore*c=(ShellCore*)core();
	if(m_pChildren==NULL)
	{
		
		ShellToplevel*top=(ShellToplevel*)toplevel();
		m_pChildren=new(c->GetGC())RCHashTable((ScriptObject*)top->m_pDumpObject.value(),4,true);
	}
	//if(m_pChildren->indexOf(obj)>=0) return;
	m_pChildren->put(obj);
	//ShellToplevel*top=(ShellToplevel*)toplevel();
	/*if(ShellCore::IsTypeClass(obj,top->getBitmapClass()))
	{
		BitmapObject*bmp=(BitmapObject*)obj;
		if(bmp->m_pLoaderInfo)
			top->PushObject((ScriptObject*)bmp->m_pLoaderInfo.value());
	}*/

}
void DisplayObjectContainerObject::RemoveChildRef(ScriptObject*obj)
{
	if(m_pChildren==NULL) return;
	m_pChildren->remove(obj,true);
	//ShellToplevel*top=(ShellToplevel*)toplevel();
	/*if(ShellCore::IsTypeClass(obj,top->getBitmapClass()))
	{
		BitmapObject*bmp=(BitmapObject*)obj;
		top->RemoveObject((ScriptObject*)bmp->m_pLoaderInfo.value());
	}*/
}

void DisplayObjectContainerObject::AS3_mouseChildren_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
	{
		//if(m_pObject->character->tag==21)
		//	int v=0;
		m_pObject->SetMouseChildren(b);
	}
}

int DisplayObjectContainerObject::AS3_numChildren_get()
{
	//Add your act code here...
	if(m_pObject)
	{
		SObject*p=m_pObject->bottomChild;
		int iCount=0;
		while(p)
		{
			//if(p->IsDisplayObject())
			iCount++;
			p=p->above;
		}
//		if(iCount==1)
//			int v=0;
		return iCount;
	}
	return 0;
}

bool DisplayObjectContainerObject::AS3_tabChildren_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
		return m_pObject->IsTabChildren();
	return false;
}

void DisplayObjectContainerObject::AS3_tabChildren_set(bool b)
{
	if(m_pObject)
		m_pObject->SetTabChildren(b);
	//Add your act code here...

}

TextSnapshotObject* DisplayObjectContainerObject::AS3_textSnapshot_get()
{
	//Add your act code here...
	if(m_pObject)
	{
		TextSnapshotClass*pClass=((ShellToplevel*)toplevel())->getTextSnapshotClass();
		return pClass->CreateObject(m_pObject);
	}
	return NULL;
}

//AS3 contructor function..
void DisplayObjectContainerObject::AS3_constructor(void)
{
	//StUTF8String s(vtable->traits->name());
	//if(strcmp(s.c_str(),"QuestTracePanel_Skin")==0)
	//		int v=0;
	InteractiveObjectObject::AS3_constructor();
	FillMembers();
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectContainerObject::FillMembers()
{
	if(m_pObject)
	{
		AvmCore*m_pCore=core();
		SObject*ps=m_pObject->bottomChild;
		while(ps)
		{
			if(ps->pASObject&&ps->AS3IsNamed()&&!ps->name.IsNull())
			{
				//if(pAVM2->SetMember(pObj,ps->name.strTxt,ps->pASObject))
				Stringp str=m_pCore->internStringUTF8(ps->name.strTxt);
				Atom value=((ScriptObject*)ps->pASObject)->atom();
		
				Traits* t=traits();
		
				ShellCore::_setProperty(this,str->atom(),value,m_pCore->findPublicNamespace(),t->getTraitsBindings());				
				//ps->SetAS3Flag();
			}
			ps=ps->above;
		}
	}
}



MethodEnv* DisplayObjectContainerObject::getRemoveMethod(ScriptObject*pObj)
{
	//Atom atom=this->getAtomProperty(name->atom());
	//if(AvmCore::isNullOrUndefined(atom)) return NULL;
	//return AvmCore::atomToScriptObject(atom);
	Multiname m;
	ShellCore*c=(ShellCore*)core();
	m.setName(c->kRemoveChild);
	m.setNamespace(c->getPublicNamespace(0));
	Binding b = avmplus::getBinding(toplevel(), pObj->vtable, &m);
	if(AvmCore::bindingKind(b)!=BKIND_METHOD) return NULL;
	return pObj->vtable->methods[AvmCore::bindingToMethodId(b)];
}
bool DisplayObjectContainerObject::CheckAddObject(DisplayObjectObject*pObj)
{

	if(m_pObject==XNULL||pObj==XNULL||pObj->m_pObject==XNULL)
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,"Object(null)");
#endif
		return false;
	}
	if(pObj->m_pObject==m_pObject)
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,"Object(self)");
#endif
		return false;
	}
	SObject*p=m_pObject->parent;
	while(p)
	{
		if(p==pObj->m_pObject)
		{
#ifdef _THROW_ERROR_
			toplevel()->throwArgumentError(kInvalidArgumentError,"Object(is parent)");
#endif
			return false;
		}
		p=p->parent;
	}
	return true;
}


DisplayObjectObject* DisplayObjectContainerObject::AS3_addChild(DisplayObjectObject*pObj)
{
	//Add your act code here...
#ifdef _WINEMU
	/*StUTF8String ss(pObj->vtable->traits->name());
	if(strcmp(ss.c_str(),"ToolTips")==0)
		int v=0;*/
#endif
	if(!CheckAddObject(pObj))
		return NULL;


	/*StUTF8String s(pObj->vtable->traits->name());
	if(strcmp(s.c_str(),"Upgrade")==0)
	{
		//StUTF8String ss(vtable->traits->name());
		//if(strcmp(ss.c_str(),"GridView")!=0)
			int v=0;
	}*/
	if(pObj->m_pObject&&pObj->m_pObject->thread)
	{
		pObj->m_pObject->thread->doCommand(true);
	}

	ShellToplevel*top=(ShellToplevel*)toplevel();


	if(pObj->m_pObject->parent==m_pObject)
	{
		m_pObject->display->AS3ToTop(m_pObject,pObj->m_pObject);
		return pObj;
	}
		//return pObj;
	///if(pObj->m_pObject->character->tag==28)
	//	int v=0;
	
	ShellCore*pCore=(ShellCore*)core();
	XAVM2*pAVM2=pCore->GetPlayer()->m_pAVM2;
	
	bool bPush=true;
	if(pObj->m_pObject->parent)//&&pObj->m_pObject->parent->pASObject)
	{//[Note X]
		//bPush=false;
		DisplayObjectContainerObject*obj=(DisplayObjectContainerObject*)pObj->m_pObject->parent->pASObject;
		obj->RemoveChildRef(pObj);
	}
	m_pObject->display->AS3PlaceObject(m_pObject,pObj->m_pObject,-1);
	
	//pObj->nCreateTime
	pObj->UpdateCreateTime();

	pObj->m_bNewObject=false;
	
	//ShellToplevel*top=(ShellToplevel*)toplevel();
	if(bPush)
		AddChildRef(pObj);
		//top->PushObject(pObj);
	//pObj->IncrementRef();
	//if(pObj->m_pObject->character->tag==127&&!pObj->hasEvent(XAS3_EVENT_added))
	//	int v=0;
	//	int v=0;
	//if(pObj->m_pObject->thread->player->m_bLoadedOK)// ScriptThread
	// Right or Error? don't know
	{
		//if(pObj->m_pObject->
		ScriptThread*thread=pObj->m_pObject->thread;
		if(!thread||thread->player->m_bLoadedOK)
			pObj->OnEvent(XAS3_EVENT_added,true,pObj,false);//Note:///
		OnEventAll(XAS3_EVENT_addedToStage,pObj->m_pObject,pObj);
		//pObj->OnEvent(XAS3_EVENT_addedToStage,false);
	}

	XSWFPlayer*player=pCore->GetPlayer();
	if(player->m_nLockOpt)
		player->AddOptObject(pObj->m_pObject);

	return pObj;
}


DisplayObjectObject* DisplayObjectContainerObject::AS3_addChildAt(DisplayObjectObject*pObj,int iLayer)
{
	//Add your act code here...
#ifdef _WINEMU
	//StUTF8String ss(pObj->vtable->traits->name());
	//if(strcmp(ss.c_str(),"TextLine")==0)
	//	int v=0;
#endif
//#ifdef _WINEMU
//	StUTF8String ss(pObj->vtable->traits->name());
//	if(strcmp(ss.c_str(),"NormalTips")==0)
//		int v=0;
//#endif
	if(!CheckAddObject(pObj))
		return NULL;

	if(!pObj->m_pObject)
		return pObj;
	/*StUTF8String s(pObj->vtable->traits->name());
	if(strcmp(s.c_str(),"Upgrade")==0)
	{
		//StUTF8String ss(vtable->traits->name());
		//if(strcmp(ss.c_str(),"GridView")!=0)
			int v=0;
	}*/

	ShellToplevel*top=(ShellToplevel*)toplevel();
	//if(!ShellCore::IsTypeClass(pObj,top->getDisplayObjectClass()))
	//	int v=0;
//#if (__CORE_VERSION__>=0x02073000)
	if(m_pObject==pObj->m_pObject->parent)
//#else
//	if(m_pObject->IsParentOf(pObj->m_pObject)) 
//#endif
	{
		m_pObject->display->AS3SetChildIndex(m_pObject,pObj->m_pObject,iLayer);
		m_pObject->Modify();
		return pObj;
	}
	ShellCore*pCore=(ShellCore*)core();
	XAVM2*pAVM2=pCore->GetPlayer()->m_pAVM2;
	//if(m_bNewObject)
	//	pCore->GetPlayer()->AS3Remove(pObj->m_pObject);
	//checkDouble(1.0);
	//bool bPush=true;
	bool bPush=true;
	if(pObj->m_pObject->parent)//&&pObj->m_pObject->parent->pASObject)
	{//[Note X]
		DisplayObjectContainerObject*obj=(DisplayObjectContainerObject*)pObj->m_pObject->parent->pASObject;
		obj->RemoveChildRef(pObj);
		//Stringp strCall=pCore->internString("");
		/*ScriptObject*pParent=(ScriptObject*)pObj->m_pObject->parent->pASObject;
		if(pParent->hasStringProperty(pCore->kRemoveChild))
		{
			Atom atom=pParent->getStringProperty(pCore->kRemoveChild);
			FunctionObject*pFun=NULL;
			if(AvmCore::isNullOrUndefined(atom))
				pFun=(FunctionObject*)AvmCore::atomToScriptObject(atom);
			if(pFun)
			{
				Atom atoms[]={pParent->atom(),pObj->atom()};
				pFun->call(1,atoms);
			}
		}
		else*/
		//{
		//	bPush=false;
			//this->AS3_removeChild(pObj);
		//}
		/*bPush=false;
		pObj->OnEvent(XAS3_EVENT_removed,false,NULL);//,true);
		//pObj->OnEvent(XAS3_EVENT_removedFromStage,false);
		OnEventAll(XAS3_EVENT_removedFromStage,pObj->m_pObject,pObj);
		OnEvent(XAS3_EVENT_removed,true,pObj);*/
	}
	m_pObject->display->AS3PlaceObject(m_pObject,pObj->m_pObject,iLayer);
	pObj->m_bNewObject=false;
	//pObj->IncrementRef();
	//ShellToplevel*top=(ShellToplevel*)toplevel();
	if(bPush)
		AddChildRef(pObj);
		//top->PushObject(pObj);

	/*if(ShellCore::IsTypeClass(pObj,(ClassClosure*)top->getBitmapClass()))
	{
		if(pObj->m_pObject->character->rbounds.xmax<-65536)
			int v=0;
	}*/
	//checkDouble(1.0);
	//if(pObj->m_pObject->thread->m_bLoadedOK)
	// Right or Error? don't know
	//[Note X]
	if(pObj->m_pObject->IsDisplayObject())
	{
		//if(pObj->m_pObject->thread->player->m_bLoadedOK)
		ScriptThread*thread=pObj->m_pObject->thread;
		if(!thread||thread->player->m_bLoadedOK)
			pObj->OnEvent(XAS3_EVENT_added,true,pObj,false);//Note://
		//pObj->OnEvent(XAS3_EVENT_addedToStage,false);
		OnEventAll(XAS3_EVENT_addedToStage,pObj->m_pObject,pObj);
	}
	pObj->UpdateCreateTime();

	XSWFPlayer*player=pCore->GetPlayer();
	if(player->m_nLockOpt)
		player->AddOptObject(pObj->m_pObject);
	//pCore->GetPlayer()->AddOptObject(pObj->m_pObject);
	//checkDouble(1.0);
	//EventObject*pEvent=(EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_added,XAS3EVENT_EVENT,true,0,XNULL);
	//pEvent->pTarget=pObj;
	//pObj->AS3_dispatchEvent(pEvent);
	//pAVM2->ResetEvent(pEvent,XAS3_EVENT_addedToStage,pObj);
	//pObj->AS3_dispatchEvent(pEvent);
	return pObj;
}

bool DisplayObjectContainerObject::AS3_areInaccessibleObjectsUnderPoint(PointObject*point)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	return false;
}

XBOOL DisplayObjectContainerObject::AS3_contains(DisplayObjectObject*pObj)
{
	//Add your act code here...
//#if (__CORE_VERSION__>=0x02081000)
	if( this == pObj )
	{
		return true;
	}
	if(!pObj || !m_pObject) 
//#else
//	if(!pObj||pObj->m_bNewObject||!m_pObject) 
//#endif
		return XFALSE;
	_XSObject*obj=pObj->m_pObject;
	while(obj)
	{
		if(obj==m_pObject) 
			return XTRUE;
		obj=obj->parent;
	}
	/*if(m_pObject&&pObj->m_pObject)
	{
		SObject*p=m_pObject->bottomChild;
		while(p)
		{
			if(p==pObj->m_pObject) return XTRUE;
			p=p->above;
		}
		//return iCount;
	}*/
	return XFALSE;//Modify this please!
}


DisplayObjectObject* DisplayObjectContainerObject::AS3_getChildAt(int i)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	//if(pObj->m_bNewObject) return false;
	int iCount=0;
	if(m_pObject)
	{
		SObject*p=m_pObject->bottomChild;
		while(p)
		{	
			if(!i)//&&p->IsDisplayObject())
			{
				if(!p->pASObject&&!p->IsRootObject())
				{
					//if(p->thread->player->rootObject==p)
					//	return (DisplayObjectObject*)p->thread->player->m_pContext->m_pLoader;
					p->AS3CreateAndConstruct(NULL,XTRUE);
				}

				if(p->pASObject)
				{
					//return NULL;
					ShellToplevel*top=(ShellToplevel*)toplevel();
					ScriptObject*obj=(ScriptObject*)p->pASObject;
					if(!ShellCore::IsTypeClass(obj,top->getDisplayObjectClass()))
					{
						if(ShellCore::IsTypeClass(obj,top->getBitmapDataClass()))
						{
							DisplayObjectClass*pClass=top->getDisplayObjectClass();
							VTable* ivtable = pClass->ivtable();
		
							ScriptObject*prototype = pClass->prototype;
							DisplayObjectObject*pRet=(DisplayObjectObject*)pClass->createInstance(ivtable,prototype);
							pRet->SetRefBitmap((BitmapDataObject*)p->pASObject);
							pRet->m_pObject=p;
							pRet->m_bNewObject = false;
							return pRet;
						}
						return NULL;
						//return this;
					}
					//	return NULL;
				}
				return (DisplayObjectObject*)p->pASObject;
			}
			//if(p->IsDisplayObject())
			{
				i--;
				iCount++;
			}
			p=p->above;
		}
	}
//#if (__CORE_VERSION__>=0x02070200)
	toplevel()->throwRangeError(kInvalidRangeError,INTString(i),INTString(iCount));
//#else
//
//#ifdef _THROW_ERROR_
//	toplevel()->throwRangeError(kInvalidRangeError,INTString(i),INTString(iCount));
//#endif
//#endif
	return NULL;
}

DisplayObjectObject* DisplayObjectContainerObject::AS3_getChildByName(Stringp name)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
	{
		StUTF8String s(name);
		//if(strcmp(s.c_str(),"txtAmount")==0)
		//	int v=0;

		SObject*p=m_pObject->GetChildOf(s.c_str(),false);
		if(p)
		{
			//if(p->character&&p->character->type==bitsChar)
			//	return NULL;
			if(!p->pASObject)
				p->AS3CreateAndConstruct(NULL,XTRUE);
			if(p->pASObject)
				{
					ShellToplevel*top=(ShellToplevel*)toplevel();
					ScriptObject*obj=(ScriptObject*)p->pASObject;
					if(!ShellCore::IsTypeClass(obj,top->getDisplayObjectClass()))
						return NULL;
				}
				//p->AS3CreateInstance(NULL,XTRUE);
			return (DisplayObjectObject*)p->pASObject;
		}
	}
	return NULL;
}

int DisplayObjectContainerObject::AS3_getChildIndex(DisplayObjectObject*pObj)
{
	//Add your act code here...	
	//if(!pObj||!pObj->m
	//StUTF8String ss(vtable->traits->name());
	//if(strcmp(ss.c_str(),"Combat")==0)
	//	int v=0;
	if(m_pObject&&pObj&&pObj->m_pObject)
	{
		if(pObj->m_pObject->parent==m_pObject)
		{
			SObject*p=m_pObject->bottomChild;
			int i=0;
			while(p)
			{	
				if(pObj->m_pObject==p)
					return i;
				//if(p->IsDisplayObject())
					i++;
				p=p->above;
			}
		}
	}
#ifdef _THROW_ERROR_
	toplevel()->throwArgumentError(kInvalidArgumentError,"child");
#endif
	return -1;
}

//#if (__CORE_VERSION__>=0x02079000)

static bool _getObjectsUnderPoint(ArrayObject*pArray,SObject*pObj,SPOINT&point,STransform x)
{
	bool bHit=false;
	MatrixConcat(&pObj->xform.mat,&x.mat,&x.mat);
	SObject*pObject=pObj;
//#if (__CORE_VERSION__>=0x02081000)
	if(pObj->HitTestOne(&x,&point, true ))
//#else
//	if(pObj->HitTestOne(&x,&point,0xff))
//#endif
	{
		/*if(!pObj->pASObject)
			pObj->AS3CreateAndConstruct(NULL,XTRUE);
			//pObj->AS3CreateInstance(XNULL,XTRUE);
		//[Note X]
		if(!pObj->pASObject||!pObj->IsDisplayObject()) return;
		ScriptObject*object=(ScriptObject*)pObj->pASObject;
		Atom atom=object->atom();
		pArray->push(&atom,1);
		return;*/
		bHit=true;
	}
	//SObject*p=pObject->bottomChild;
	pObj=pObj->bottomChild;
	while(pObj)
	{	
		if(_getObjectsUnderPoint(pArray,pObj,point,x))
			bHit=true;
		pObj=pObj->above;		
	}
	if(bHit)
	{
		if(!pObject->pASObject)
			pObject->AS3CreateAndConstruct(NULL,XTRUE);
		if(pObject->pASObject&&pObject->IsDisplayObject())
		{
			ScriptObject*object=(ScriptObject*)pObject->pASObject;
			Atom atom=object->atom();
			pArray->push(&atom,1);
		}
	}
	return bHit;
}
//#else
//static void _getObjectsUnderPoint(ArrayObject*pArray,SObject*pObj,SPOINT&point,STransform x)
//{
//	MatrixConcat(&pObj->xform.mat,&x.mat,&x.mat);
//	if(pObj->HitTestOne(&x,&point,0xff))
//	{
//		if(!pObj->pASObject)
//			pObj->AS3CreateAndConstruct(NULL,XTRUE);
//			//pObj->AS3CreateInstance(XNULL,XTRUE);
//		//[Note X]
//		if(!pObj->pASObject||!pObj->IsDisplayObject()) return;
//		ScriptObject*object=(ScriptObject*)pObj->pASObject;
//		Atom atom=object->atom();
//		pArray->push(&atom,1);
//		return;
//	}
//	//SObject*p=pObject->bottomChild;
//	pObj=pObj->bottomChild;
//	while(pObj)
//	{	
//		_getObjectsUnderPoint(pArray,pObj,point,x);
//		pObj=pObj->above;		
//	}
//}
//#endif

ArrayObject* DisplayObjectContainerObject::AS3_getObjectsUnderPoint(PointObject*pt)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	ArrayObject*pArray=toplevel()->arrayClass->newArray(0);
	if(m_pObject)
	{
		AS3POINTDATA&data=pt->GetData();
		SPOINT point={(int)data.x*20,(int)data.y*20};
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		::MatrixTransformPoint(&player->display.camera.mat,&point,&point);
		STransform x;
		x.Clear();
		m_pObject->GetTotalMatrix(x.mat,XTRUE);
		SObject*p=m_pObject->bottomChild;
		while(p)
		{
			_getObjectsUnderPoint(pArray,p,point,x);
			p=p->above;
		}
	}
	return pArray;
}

DisplayObjectObject* DisplayObjectContainerObject::AS3_removeChild(DisplayObjectObject*pObj)
{
	//Add your act code here...
	//if(pObj)
	//{
	//	StUTF8String ss(pObj->vtable->traits->name());
	//	if(strcmp(ss.c_str(),"RoleScene")==0)
	//		int v=0;
	//}
	//return pObj;
	if(m_pAutoRemoved==pObj)
		return pObj;
	if(!m_pObject) return NULL;
	if(pObj->GetRefBitmap())//m_pDummyObject)
	{
		BitmapDataObject*bmp=pObj->GetRefBitmap();
		BitmapDataClass*pClass=((ShellToplevel*)toplevel())->getBitmapDataClass();
		if(ShellCore::IsTypeClass(bmp,pClass))
		{
			//BitmapDataObject*pBmp=(BitmapDataObject*)pObj->m_pDummyObject.value();
			if(bmp->m_pObject)
			{
				m_pObject->display->AS3RemoveObject(m_pObject,bmp->m_pObject);
			}
		}

		this->RemoveChildRef(bmp);//m_pDummyObject);

		return pObj;
	}
	if(!pObj||!pObj->m_pObject||pObj->m_bNewObject)
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,"child(null)");
#endif
		return pObj;
	}

	//StUTF8String ss(pObj->vtable->traits->name());


	//if(strcmp(ss.c_str(),"Bitmap")==0)
	//	int v=0;
	//if(pObj->m_bNewObject)
	//	return pObj;
	
	ShellCore*pCore=(ShellCore*)core();
	XAVM2*pAVM2=pCore->GetPlayer()->m_pAVM2;


	

	SObject*parent=pObj->m_pObject->parent;

	//[Note X]
	if(parent&&pObj->m_pObject->IsDisplayObject())
	{
		pObj->OnEvent(XAS3_EVENT_removed,false,NULL);//,true);
		//pObj->OnEvent(XAS3_EVENT_removedFromStage,false);
		OnEventAll(XAS3_EVENT_removedFromStage,pObj->m_pObject,pObj);
		OnEvent(XAS3_EVENT_removed,true,pObj);
	}

	if(m_pObject->display->AS3RemoveObject(m_pObject,pObj->m_pObject))
	{
		//pCore->GetPlayer()->AS3Push(pObj->m_pObject);
		pObj->m_bNewObject=true;
		//pObj->DecrementRef();
		RemoveChildRef(pObj);
		//((ShellToplevel*)toplevel())->RemoveObject(pObj);
		//pObj->OnEvent(XAS3_EVENT_removed,true);
		//OnEvent(XAS3_EVENT_removed,false,pObj);
		//pObj->OnEvent(XAS3_EVENT_removedFromStage,true);
		
		//OnEvent(XAS3_EVENT_removedFromStage,true,pObj);
		//OnEvent(XAS3_EVENT_removedFromStage,false,pObj);
		//EventObject*pEvent=(EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_removed,XAS3EVENT_EVENT,true,0,XNULL);
		//pEvent->pTarget=pObj;
		//pObj->AS3_dispatchEvent(pEvent);
		//this->AS3_dispatchEvent(pEvent);
		//pAVM2->ResetEvent(pEvent,XAS3_EVENT_removedFromStage,pObj);
		//pObj->AS3_dispatchEvent(pEvent);
		//this->AS3_dispatchEvent(pEvent);
	}
	//else
	//	toplevel()->throwArgumentError(kInvalidArgumentError);

	//void*pEvent=pAVM2->CreateEventObject(XAS3_EVENT_added,XAS3EVENT_EVENT,true,0,XNULL);
	//pAVM2->DispatchEvent(pEvent,pObj);
	return pObj;
}



DisplayObjectObject* DisplayObjectContainerObject::AS3_removeChildAt(int iRemove)
{
	//return NULL;
	//Add your act code here...
	//StUTF8String ss(vtable->traits->name());
	//if(strcmp(ss.c_str(),"Combat")==0)
	//	int v=0;
	if(!m_pObject)
		return NULL;
		//toplevel()->throwArgumentError(kInvalidArgumentError);
	ShellCore*pCore=(ShellCore*)core();
	XAVM2*pAVM2=pCore->GetPlayer()->m_pAVM2;

	{
		int i=iRemove;
		SObject** link = &m_pObject->bottomChild;
		SObject* obj=*link;
		while(i) {
			
			if ( !obj ) 
			{
#ifdef _THROW_ERROR_
				toplevel()->throwArgumentError(kInvalidArgumentError,INTString(iRemove));
#endif
				return XNULL;	 	
			}
			//if(obj->IsDisplayObject())
				i--;
			link = &obj->above;
			obj = *link;
			
		}
		if(obj&&obj->pASObject&&obj->IsDisplayObject())
		{
			((EventDispatcherObject*)obj->pASObject)->OnEvent(XAS3_EVENT_removed,true,NULL);//,true);		
			OnEventAll(XAS3_EVENT_removedFromStage,obj,NULL);
			//OnEvent(XAS3_EVENT_removed,true,pObject);
			//OnEvent(XAS3_EVENT_removedFromStage,true,pObject);
		}
	}

	SObject*pObj=m_pObject->display->AS3RemoveObject(m_pObject,iRemove);
	if(pObj)
	{
		FlashObject*pObject=(FlashObject*)pObj->pASObject;
		//pObj->pASObject = (void*)1;
		if(pObject)
		{
			//pCore->GetPlayer()->AS3Push(pObj);
			//pObject->DecrementRef();
			//((ShellToplevel*)toplevel())->RemoveObject(pObject);
			RemoveChildRef(pObject);
			//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxx
			//pObject->m_bNewObject=true;
		}
		else
		{
			//return NULL;
			pObj->AS3CreateAndConstruct(NULL,XTRUE);//AS3CreateInstance(XNULL,XTRUE);
			pObject=(FlashObject*)pObj->pASObject;
			if(!pObject)
			{
				pObj->FreeChildren();
				pObj->FreeContext();
				m_pObject->display->FreeObject(pObj);
				return NULL;
			}
		}
		//pObject->OnEvent(XAS3_EVENT_removed,false);		
		//OnEventAll(XAS3_EVENT_removedFromStage,pObject->m_pObject);
		//OnEvent(XAS3_EVENT_removed,true,pObject);
		//OnEvent(XAS3_EVENT_removedFromStage,true,pObject);
		
		/*EventObject*pEvent=(EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_removed,XAS3EVENT_EVENT,true,0,XNULL);
		pEvent->pTarget=pObject;
		pObject->AS3_dispatchEvent(pEvent);
		this->AS3_dispatchEvent(pEvent);
		pAVM2->ResetEvent(pEvent,XAS3_EVENT_removedFromStage,pObject);
		pObject->AS3_dispatchEvent(pEvent);
		this->AS3_dispatchEvent(pEvent);*/
		if(!pObj->IsDisplayObject()) return NULL;
		return (DisplayObjectObject*)pObject;
	}
	else
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,INTString(iRemove));
#endif
	}

	//void*pEvent=pAVM2->CreateEventObject(XAS3_EVENT_added,XAS3EVENT_EVENT,true,0,XNULL);
	//pAVM2->DispatchEvent(pEvent,pObj);
	return NULL;
}

void DisplayObjectContainerObject::AS3_setChildIndex(DisplayObjectObject*pObj,int i)
{
	//Add your act code here...
	//StUTF8String ss(vtable->traits->name());
	//if(strcmp(ss.c_str(),"Combat")==0)
	//	int v=0;
	if(!m_pObject||!pObj||!pObj->m_pObject)
	{
#ifdef _THROW_ERROR_	
		toplevel()->throwArgumentError(kInvalidArgumentError,"child(null)");
#endif
		return;
	}
	if(!m_pObject->display->AS3SetChildIndex(m_pObject,pObj->m_pObject,i))
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,INTString(i));
#endif
	}
}

void DisplayObjectContainerObject::AS3_swapChildren(DisplayObjectObject*p1,DisplayObjectObject*p2)
{
	//Add your act code here...
	if(!m_pObject||!p1->m_pObject||!p2->m_pObject)
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,"child(null)");
		return;
#endif
	}
	if(!m_pObject->display->AS3SwapChild(m_pObject,p1->m_pObject,p2->m_pObject))
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,"child(not child)");
#endif
	}

}

void DisplayObjectContainerObject::AS3_swapChildrenAt(int i1,int i2)
{
	//Add your act code here...
	if(!m_pObject)
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,"object(null)");
#endif
		return;
	}
	if(!m_pObject->display->AS3SwapChild(m_pObject,i1,i2))
	{
#ifdef _THROW_ERROR_
		toplevel()->throwArgumentError(kInvalidArgumentError,INTString(i1),INTString(i2));
#endif
	}
}

//new adds 11
 void DisplayObjectContainerObject::AS3_removeChildren(int beginIndex, int endIndex)
 {
	//LOGWHERE();
	if(!m_pObject)
		return;
	int b = beginIndex, e = endIndex;
	for ( ; e >= b; e-- )
		AS3_removeChildAt(e);

 }
 void DisplayObjectContainerObject::AS3_stopAllMovieClips()
 {
	LOGWHERE();
 }
//new adds 11 end
}