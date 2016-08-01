#include "StdAfxflash.h"
#include "splayer.h"
#include "sobject.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Stage.h"
#include "AS3StageAlign.h"
#include "AS3StageDisplayState.h"
#include "AS3InteractiveObject.h"
#include "AS3StageQuality.h"
#include "AS3StageScaleMode.h"
#include "AS3LoaderInfo.h"

namespace avmshell{
StageClass::StageClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StageObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
	m_bSupportsOrientationChange = true;
};
ScriptObject* StageClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

StageObject* StageClass::CreateStage(DisplayObjectObject*pObject)
{
	StageObject*pObj=(StageObject*)createInstance(ivtable(),prototype);
	pObj->m_pObject=&((ShellCore*)core())->GetPlayer()->display.root;//player->rootObject;//->display.root;;
	pObj->AS3_constructor();
	ShellCore*c=(ShellCore*)core();
	pObj->SetCodeContext(c->GetShellContext());
	//pObj->m_pCodeContext=c->GetShellContext();
		//c->GetPlayer()->player->m_pContext;
	return pObj;
}

DisplayObjectObject* StageObject::AS3_root_get()
{
	return this;
}

LoaderInfoObject* StageObject::AS3_loaderinfo_get()
{
	//Modify form happy farm,Mybe error for others
	ShellCodeContext*code=(ShellCodeContext*)core()->codeContext();
	//if(!code) code=m_codeContext;
	//if(code) return code->m_pRoot->m_pLoaderInfo;*/
	if(code&&code->m_pRoot)
	{
		return code->m_pRoot->m_pLoaderInfo;
	}
	ShellCore*c=(ShellCore*)core();	
	if(c->GetPlayer()->player->m_pLoaderInfo)
		return c->GetPlayer()->player->m_pLoaderInfo;
	return DisplayObjectContainerObject::AS3_loaderInfo_get();
	/*XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	if(player->m_pLoaderInfo==NULL)
	{
		ShellCore*c=(ShellCore*)core();
		ShellToplevel*top=(ShellToplevel*)toplevel();
		LoaderInfoClass*pClass=top->getLoaderInfoClass();
		ShellCodeContext*code=(ShellCodeContext*)c->codeContext();	
		LoaderInfoObject*obj=pClass->CreateLoaderInfo(NULL,code);
		ScriptObject*pObj=toplevel()->objectClass->newInstance();
		int i,nCount=player->m_argList.GetSize();
		for(i=0;i<nCount;i+=2)
		{
			XString8&strKey=player->m_argList[i];
			XString8&strValue=player->m_argList[i+1];
			Stringp name=c->internStringUTF8(strKey);
			Stringp value=c->newStringUTF8(strValue);
			pObj->setStringProperty(name,value->atom());
		}
		obj->m_params=pObj;
		top->PushObject(obj);
		player->m_pLoaderInfo=obj;
	}
	LoaderInfoObject*obj=(LoaderInfoObject*)player->m_pLoaderInfo;*/
	//return obj;
	
}

StageObject::StageObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectContainerObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_strAlign=((ShellToplevel*)toplevel())->getStageAlignClass()->getSlotTOP_LEFT();
	m_bAllowsFullScreenInteractive = false;
	m_pConstructor = NULL;
	m_numContentsScaleFactor = 1;
	m_bMouseLock = false;
	m_stage3Ds = NULL;

}
//////////////////////////////////////////////////////////
//Native Method start...

Stringp StageObject::AS3_displayState_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	//XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	return ((ShellToplevel*)toplevel())->getStageDisplayStateClass()->getSlotFULL_SCREEN();
}

void StageObject::AS3_displayState_set(Stringp)
{
	//Add your act code here...
	
}

InteractiveObjectObject* StageObject::AS3_focus_get()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	SObject*focus=player->focus?player->focus:player->display.button;
	if(focus&&focus->pASObject)
		return (InteractiveObjectObject*)focus->pASObject;
	return NULL;
	//return kAvmThunkUndefined;//Modify this please!
}

void StageObject::AS3_focus_set(InteractiveObjectObject*pObject)
{
	//Add your act code here...
	if(!pObject||!pObject->m_pObject)
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->DoButton(XNULL,false,true);
	}
	//if(pObject->m_pObject)
	else
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		//player->DoButton(pObject->m_pObject,false,true);
		player->SetMyFucos(pObject->m_pObject);
//#if (__CORE_VERSION__>=0x02077000)
		pObject->m_pObject->exflags|=_XSObject::XEN_FOCUSENABLE;
		player->AS3SetFocus(pObject->m_pObject);
//#endif
	}
}

double StageObject::AS3_frameRate_get()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	return player->player->frameRate/65536.0;
}

void StageObject::AS3_frameRate_set(double f)
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->player->frameRate=(int)(f*65536);
}

double StageObject::AS3_height_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	return DisplayObjectObject::AS3_height_get();
}

void StageObject::AS3_height_set(double v)
{
	//Add your act code here...
	return DisplayObjectObject::AS3_height_set(v);
}

bool StageObject::AS3_mouseChildren_get()
{
	//Add your act code here...
	//return InteractiveObjectObject::AS3_mouse
	if(m_pObject)
		return m_pObject->IsMouseChildren();
	return false;
}

void StageObject::AS3_mouseChildren_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->SetMouseChildren(b);
}

int StageObject::AS3_numChildren_get()
{
	//Add your act code here...
	return DisplayObjectContainerObject::AS3_numChildren_get();
}

Stringp StageObject::AS3_quality_get()
{
	//Add your act code here...
	StageQualityClass*pClass=((ShellToplevel*)toplevel())->getStageQualityClass();
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	return pClass->getSlotHIGH();

	//switch(player->nQuality)
	//{
	//case XConfig::LOW:
	//	 return pClass->getSlotLOW();break;
	//case XConfig::MID:
	//	 return pClass->getSlotMEDIUM();break;
	//default:
	//	 return pClass->getSlotHIGH();break;
	//}
	//return NULL;
}

void StageObject::AS3_quality_set(Stringp s)
{
	//Add your act code here...
	StageQualityClass*pClass=((ShellToplevel*)toplevel())->getStageQualityClass();
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	if(s->equals(pClass->getSlotLOW()))
		player->SetQuality(0.0F);
	else if(s->equals(pClass->getSlotHIGH())||
		    s->equals(pClass->getSlotBEST())||
			s->equals(pClass->getSlotHIGH_16X16())||
			s->equals(pClass->getSlotHIGH_16X16_LINEAR())||
			s->equals(pClass->getSlotHIGH_8X8())||
			s->equals(pClass->getSlotHIGH_8X8_LINEAR()))
		player->SetQuality(3.0F);
	else
		player->SetQuality(2.0F);
}

Stringp StageObject::AS3_scaleMode_get()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	StageScaleModeClass*pClass=((ShellToplevel*)toplevel())->getStageScaleModeClass();
	 return pClass->getSlotSHOW_ALL();

	//switch(player->scaleMode)
	//{
	//case DisplayList::showAll:
	//	 return pClass->getSlotSHOW_ALL();break;
	//case DisplayList::noBorder:
	//	 return pClass->getSlotNO_BORDER();break;
	//case DisplayList::exactFit:
	//	 return pClass->getSlotEXACT_FIT();break;
	//case DisplayList::noScale:
	//	 return pClass->getSlotNO_SCALE();break;
	//	 break;
 //   
	//default:
	//	 return pClass->getSlotEXACT_FIT();break;
	//	//return _strScaleModes[DisplayList::exactFit];
	//}
	return NULL;
}

void StageObject::AS3_scaleMode_set(Stringp value)
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	//StUTF8String s(value);
	//if(strstr(s.c_str(),"autoFit")
	//9f(
	LOGWHERE();
	//if(value->equals(core()->newConstantStringLatin1(player->GetScaleMode(DisplayList::noBorder))))
	//	player->SetAutoFit(0,0);
	player->ResetStageMode();
}

bool StageObject::AS3_showDefaultContextMenu_get()
{
	//Add your act code here...
	return false;
}

void StageObject::AS3_showDefaultContextMenu_set(bool)
{
	//Add your act code here...	
}

bool StageObject::AS3_stageFocusRect_get()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	return player->display.bFocusRect;
}

void StageObject::AS3_stageFocusRect_set(bool b)
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->display.bFocusRect=b;
	player->display.InvalidateRect(&player->display.tabFocusRect);
		//player->display.Invalidate();
}

int StageObject::AS3_stageHeight_get()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	return player->GetStageHeight();
}

void StageObject::AS3_stageHeight_set(int v)
{
	//Add your act code here...
	
}

int StageObject::AS3_stageWidth_get()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	return player->GetStageWidth();
}

void StageObject::AS3_stageWidth_set(int v)
{
	//Add your act code here...	
}

bool StageObject::AS3_tabChildren_get()
{
	//Add your act code here...
	_XSObject*pObj=m_pObject;
	m_pObject=&((ShellCore*)core())->GetPlayer()->display.root;
	bool b=DisplayObjectContainerObject::AS3_tabChildren_get();
	m_pObject=pObj;
	return b;
}

void StageObject::AS3_tabChildren_set(bool b)
{
	//Add your act code here...
	_XSObject*pObj=m_pObject;
	m_pObject=&((ShellCore*)core())->GetPlayer()->display.root;
	DisplayObjectContainerObject::AS3_tabChildren_set(b);
	m_pObject=pObj;
}

TextSnapshotObject* StageObject::AS3_textSnapshot_get()
{
	//Add your act code here...
	_XSObject*pObj=m_pObject;
	m_pObject=&((ShellCore*)core())->GetPlayer()->display.root;
	TextSnapshotObject*p=DisplayObjectContainerObject::AS3_textSnapshot_get();
	m_pObject=pObj;
	return p;
}



void StageObject::AS3_invalidate()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->display.m_bInvalidate=true;
	player->display.Invalidate();
}

//new adds
bool StageClass::AS3_supportsOrientationChange_get()
{
	return m_bSupportsOrientationChange;
}
//new adds end
//new adds
bool StageObject::AS3_allowsFullScreen_get()
{
	return false;
}
bool StageObject::AS3_autoOrients_get()
{
	return false;
}
void StageObject::AS3_autoOrients_set(bool autoOrients)
{

}
uint32_t StageObject::AS3_color_get()
{
	return 0;
}
void StageObject::AS3_color_set(uint32_t color)
{

}
Stringp StageObject::AS3_colorCorrection_get()
{
	return NULL;
}
void StageObject::AS3_colorCorrection_set(Stringp pColorCorrection)
{
	
}
Stringp StageObject::AS3_colorCorrectionSupport_get()
{
	return NULL;
}
Stringp StageObject::AS3_deviceOrientation_get()
{
	return NULL;
}
uint32_t StageObject::AS3_fullScreenHeight_get()
{
	return NULL;
}
RectangleObject* StageObject::AS3_fullScreenSourceRect_get()
{
	return NULL;
}
void StageObject::AS3_fullScreenSourceRect_set(RectangleObject *pFullScreenSourceRect)
{

}
uint32_t StageObject::AS3_fullScreenWidth_get()
{
	return 0;
}
NativeWindowObject* StageObject::AS3_nativeWindow_get()
{
	return NULL;
}
Stringp StageObject::AS3_orientation_get()
{
	return NULL;
}
RectangleObject* StageObject::AS3_softKeyboardRect_get()
{
	return NULL;
}
ObjectVectorObject* StageObject::AS3_stageVideos_get()
{
	return NULL;
}
ObjectVectorObject* StageObject::AS3_supportedOrientations_get()
{
	return NULL;
}
bool StageObject::AS3_wmodeGPU_get()
{
	return false;
}

void StageObject::AS3_assignFocus(InteractiveObjectObject pObjectToFocus, Stringp pDirection)
{

}
void StageObject::AS3_setAspectRatio(Stringp pNewAspectRatio)
{

}
void StageObject::AS3_setOrientation(Stringp pNewOrientation)
{

}
//new adds end

//new adds 11
 bool StageObject::AS3_allowsFullScreenInteractive_get()
 {
	//LOGWHERE();
	
	return m_bAllowsFullScreenInteractive;
 }
 
 void* StageObject::AS3_constructor_get()
 {

	 //LOGWHERE();
	 return NULL;
 }
 void StageObject::AS3_constructor_set(void* value)
 {
	//LOGWHERE();
 }
 
double StageObject::AS3_contentsScaleFactor_get()
{
	//LOGWHERE();
	return m_numContentsScaleFactor;
 }
 
bool StageObject::AS3_mouseLock_get()
{
	//LOGWHERE();
	return m_bMouseLock;
}
void StageObject::AS3_mouseLock_set(bool bLock)
{
	//LOGWHERE();
	m_bMouseLock = bLock;
}
 
ObjectVectorObject* StageObject::AS3_stage3Ds_get()
{
	//LOGWHERE();
	if(m_stage3Ds==NULL)
	{
		ObjectVectorClass* pClass= toplevel()->objectVectorClass;
		m_stage3Ds = pClass->newVector(0);
	}
	return m_stage3Ds;
}
 //new adds 11 end
}