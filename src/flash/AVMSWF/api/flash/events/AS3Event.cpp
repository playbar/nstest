#include "StdAfxflash.h"
#include "ShellCore.h"
#include "avm2.h"
using namespace avmplus;
#include "AS3Event.h"
#include "AS3EventDispatcher.h"
#include "splayer.h"
namespace avmshell{
EventClass::EventClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();

	//tb->findBinding(
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())EventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* EventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) EventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

Stringp EventClass::IDToStringType(int id)
{
	//Strignp strType;
	switch(id)
	{
	case XAS3_EVENT_activate:
		 return this->getSlotACTIVATE();
	case XAS3_EVENT_added:
		 return this->getSlotADDED();
	case XAS3_EVENT_addedToStage:
		 return this->getSlotADDED_TO_STAGE();
	case XAS3_EVENT_cancel:
		 return this->getSlotCANCEL();
	case XAS3_EVENT_change:
		 return this->getSlotCHANGE();
	case XAS3_EVENT_close:
		 return this->getSlotCLOSE();
	case XAS3_EVENT_complete:
		 return this->getSlotCOMPLETE();
	case XAS3_EVENT_connect:
		 return this->getSlotCONNECT();
	case XAS3_EVENT_deactivate:
		 return this->getSlotDEACTIVATE();
	case XAS3_EVENT_enterFrame:
		 return this->getSlotENTER_FRAME();
	case XAS3_EVENT_fullscreen:
		 return this->getSlotFULLSCREEN();
	case XAS3_EVENT_id3:
		 return this->getSlotID3();
	case XAS3_EVENT_init:
		 return this->getSlotINIT();
	case XAS3_EVENT_mouseLeave:
		 return this->getSlotMOUSE_LEAVE();
	case XAS3_EVENT_open:
		 return this->getSlotOPEN();
	case XAS3_EVENT_removed:
		 return this->getSlotREMOVED();
	case XAS3_EVENT_removedFromStage:
		 return this->getSlotREMOVED_FROM_STAGE();
	case XAS3_EVENT_render:
		 return this->getSlotRENDER();
	case XAS3_EVENT_resize:
		 return this->getSlotRESIZE();
	case XAS3_EVENT_scroll:
		 return this->getSlotSCROLL();
	case XAS3_EVENT_select:
		 return this->getSlotSELECT();
	case XAS3_EVENT_soundComplete:
		 return this->getSlotSOUND_COMPLETE();
	case XAS3_EVENT_tabChildrenChange:
		 return this->getSlotTAB_CHILDREN_CHANGE();
	case XAS3_EVENT_tabEnabledChange:
		 return this->getSlotTAB_ENABLED_CHANGE();
	case XAS3_EVENT_tabIndexChange:
		 return this->getSlotTAB_INDEX_CHANGE();
	case XAS3_EVENT_unload:
		 return this->getSlotUNLOAD();
		 break;
	case XAS3_EVENT_exitFrame:
		 return this->getSlotEXIT_FRAME();
		 break;
	case XAS3_EVENT_clear:
		return getSlotCLEAR();
	case XAS3_EVENT_closing:
		return getSlotCLOSING();
	case XAS3_EVENT_copy:
		return getSlotCOPY();
	case XAS3_EVENT_cut:
		return getSlotCUT();
	case XAS3_EVENT_displaying:
		return getSlotDISPLAYING();
	case XAS3_EVENT_exiting:
		return getSlotEXITING();
	case XAS3_EVENT_frameConstructed:
		return getSlotFRAME_CONSTRUCTED();
	case XAS3_EVENT_htmlBundsChange:
		return getSlotHTML_BOUNDS_CHANGE();
	case XAS3_EVENT_htmlDomInitialize:
		return getSlotHTML_DOM_INITIALIZE();
	case XAS3_EVENT_htmlRender:
		return getSlotHTML_RENDER();
	case XAS3_EVENT_locationChange:
		return getSlotLOCATION_CHANGE();
	case XAS3_EVENT_networkChange:
		return getSlotNETWORK_CHANGE();
	case XAS3_EVENT_paste:
		return getSlotPASTE();
	case XAS3_EVENT_preparing:
		return getSlotPREPARING();
	case XAS3_EVENT_selectAll:
		return getSlotSELECT_ALL();
	case XAS3_EVENT_standardErrorClose:
		return getSlotSTANDARD_ERROR_CLOSE();
	case XAS3_EVENT_standardInputClose:
		return getSlotSTANDARD_INPUT_CLOSE();
	case XAS3_EVENT_standardOutputClose:
		return getSlotSTANDARD_OUTPUT_CLOSE();
	case XAS3_EVENT_textInteractionModeChange:
		return getSlotTEXT_INTERACTION_MODE_CHANGE();
	case XAS3_EVENT_userIdle:
		return getSlotUSER_IDLE();
	case XAS3_EVENT_userPresent:
		return getSlotUSER_PRESENT();
	case XAS3_EVENT_context3DCreate:
		return getSlotCONTEXT3D_CREATE();


	case XAS3_EVENT_channelMessage:
		return getSlotCHANNEL_MESSAGE();
	case XAS3_EVENT_channelState:
		return getSlotCHANNEL_STATE();
	case XAS3_EVENT_frameLabel:
		return getSlotFRAME_LABEL();
	case XAS3_EVENT_suspend:
		return getSlotSUSPEND();
	case XAS3_EVENT_textureReady:
		return getSlotTEXTURE_READY();
	case XAS3_EVENT_videoFrame:
		return getSlotVIDEO_FRAME();
	case XAS3_EVENT_workerState:
		return getSlotWORKER_STATE();
	}
	return NULL;
}	

EventObject* EventClass::CreateEventObject(Stringp strType)
{
	if(!strType) return NULL;
	EventObject*pObj=(EventObject*)createInstance(ivtable(),prototype);
	pObj->AS3_constructor(strType,false,false);
	pObj->nEventID=-1;	
	//pObj->
	//construct(
	return pObj;
}

EventObject* EventClass::CreateEventObject(int id)
{
	Stringp strType=IDToStringType(id);
	if(!strType) return NULL;
	EventObject*pObj=(EventObject*)createInstance(ivtable(),prototype);
	pObj->AS3_constructor(strType,false,false);
	pObj->nEventID=id;	
	//pObj->
	//construct(
	return pObj;
}

EventObject::EventObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	bBubbles=false;//[read-only] ָʾ�¼��Ƿ�Ϊð���¼���  Event 
	bCancelable=false;//[read-only] ָʾ�Ƿ������ֹ���¼����������Ϊ��  Event 
	pCurrentTarget=NULL;//[read-only] ��ǰ����ʹ��ĳ���¼����������� Event ����Ķ���?Event 
	pStopObj=NULL;
	uEventPhase=0;//[read-only] �¼����еĵ�ǰ�׶Ρ�  Event 
	pTarget=NULL;//[read-only] �¼�Ŀ�ꡣ  Event 
	strType=NULL;//[read-only] �¼������͡� 
	bStoped=XFALSE;
	bStopedLater=false;
	bDefaultPrevented=XFALSE;
	bWithTarget=false;
	nCallStack=0;
	nEventID=0;
	nCreateTime=0xffffffff;//

	/*StUTF8String ss(vtable->traits->name());
	if(strcmp(ss.c_str(),"MvcEvent")==0)
	{
		//if(_DebugWindow==NULL)
		//	_DebugWindow=this;
		int v=0;
	}*/
	//Add your construct code here...
}



//////////////////////////////////////////////////////////
//Native Method start...
bool EventObject::AS3_bubbles_get()
{
	return bBubbles;
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

bool EventObject::AS3_cancelable_get()
{
	return bCancelable;
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

AvmBox EventObject::AS3_currentTarget_get()
{

	return pCurrentTarget?pCurrentTarget->atom():kAvmThunkNull;
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

uint32 EventObject::AS3_eventPhase_get()
{
	//Add your act code here...
	return uEventPhase;
	//return kAvmThunkUndefined;//Modify this please!
}

AvmBox EventObject::AS3_target_get()
{
	//Add your act code here...
	
	return pTarget?pTarget->atom():kAvmThunkNull;
	//return kAvmThunkUndefined;//Modify this please!
}

Stringp EventObject::AS3_type_get()
{
	//Add your act code here...
	return strType?(Stringp)strType:(Stringp)core()->kEmptyString; // modify by hgl
}

//AS3 contructor function..
void EventObject::AS3_constructor(Stringp type,bool bB,bool bC)//AvmMethodEnv, uint32_t argc, AvmBox* argv)
{
	//Add your act code here...
	/*enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
        , argoff2 = argoff1 + AvmThunkArgSize_AvmString
		, argoff3 = argoff2 + AvmThunkArgSize_AvmBool32
    };
	switch(argc)
	{
	case 3:
		bCancelable=AvmThunkUnbox_AvmBool32(argv[argoff3]);
	case 2:
		bBubbles=AvmThunkUnbox_AvmBool32(argv[argoff2]);
	case 1:
		strType=AvmThunkUnbox_AvmString(argv[argoff1]);
		break;
	}*/
	//StUTF8String s(type);
	//if(strcmp(s.c_str(),"AvatarEvent_click")==0)
	//	int v=0;
	//if(type==NULL)
	//	int v=0;

	strType=type;
	bBubbles =bB;
	bCancelable=bC;
	//return kAvmThunkUndefined;//Modify this please!
}

EventObject* EventObject::Clone(EventObject*pObj)
{	
	//Add your act code here...
	if(pObj==NULL)
	{
		Toplevel*t=toplevel();
		ClassClosure *pClass=t->getBuiltinExtensionClass(abcclass_flash_events_Event);
		//Atom argv=pClass->atom();
		//Atom atoms[]={pClass->atom(),strType->atom(),bBubbles,bCancelable};
		pObj=(EventObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		//Atom atom=pClass->
		//construct(3,atoms);
		//pObj=(EventObject*)AvmCore::atomToScriptObject(atom);	
	}
	//pObj->IncrementRef();
	pObj->bBubbles=bBubbles;
	pObj->bCancelable=bCancelable;
	pObj->uEventPhase=uEventPhase;
	pObj->pTarget=pTarget;
	pObj->pCurrentTarget=pTarget;
	pObj->strType=strType;
	pObj->bStoped=bStoped;
	pObj->bDefaultPrevented=bDefaultPrevented;
	pObj->nEventID=nEventID;
	return pObj;
	//return pObj->atom();
	//SciptObject
	//this->clone
}

EventObject* EventObject::AS3_clone()
{
	return Clone(NULL);
	//Add your act code here...
	//Toplevel*t=toplevel();
	//ClassClosure *pClass=t->getBuiltinExtensionClass(abcclass_flash_events_Event);
	//Atom atom=pClass->construct();
	//this->clone
	//return kAvmThunkUndefined;//Modify this please!
}

Stringp EventObject::AS3_formatToString(Stringp strName,ArrayObject*pArgs)//AvmMethodEnv, uint32_t argc, AvmBox* argv)
{

	ShellCore*core=(ShellCore*)this->core();
	Stringp str=core->kLeftQ;
	str=str->concatStrings(str,formatToString(core->kSpaceString,core->kEqualString,core->kQuotString,strName,pArgs));
	str=str->concatStrings(str,core->kRightQ);
	return str;
}

bool EventObject::AS3_isDefaultPrevented()
{
	
	
	//Add your act code here...
	return bDefaultPrevented;//kAvmThunkUndefined;//Modify this please!
}

void EventObject::AS3_preventDefault(bool b)
{	
	bDefaultPrevented=b;
	//Add your act code here...	
}

void EventObject::AS3_stopImmediatePropagation(void)
{
	//Add your act code here...
	bStoped=XTRUE;
	//return kAvmThunkUndefined;//Modify this please!
}

void EventObject::AS3_stopPropagation(void)
{
	//Add your act code here...
	bStopedLater=XTRUE;
	
}


Stringp EventObject::AS3_toString()
{
	//return (AvmBox)MakeVariableString();
	ShellCore*core=(ShellCore*)this->core();
	Stringp str=core->kLeftQ;
	str=str->concatStrings(str,formatString(core->kSpaceString,core->kEqualString,core->kQuotString,true));
	str=str->concatStrings(str,core->kRightQ);
	return str;
	//Add your act code here...
	//Stringp str=core()->newStringLatin1(_strLeftQ);
	//Stringp strName=this->vtable->traits->name();
	//return strName->atom();
	/*XString8 strResult=_strLeftQ;
	strResult.AddString((char*)strName->GetBuffer()->p8,strName->length());
	strResult+=(char)' ';
	strResult+=_strType;
	strResult+="=\"";
	strResult.AddString((char*)strType->GetBuffer()->p8,strType->length());
	strResult+="\" ";
	strResult+=_strBubbles;
	strResult+=(char)'=';
	strName=bBubbles?core()->ktrue:core()->kfalse;
	strResult.AddString((char*)strName->GetBuffer()->p8,strName->length());
	strResult+=(char)' ';

	strResult+=_strCancelable;
	strResult+=(char)'=';
	strName=bCancelable?core()->ktrue:core()->kfalse;
	strResult.AddString((char*)strName->GetBuffer()->p8,strName->length());
	strResult+=(char)' ';

	strResult+=_strEventPhase;
	strResult+=(char)'=';
	strResult+=(XINT)uEventPhase;
	strResult+=_strRightQ;

	strName=core()->newStringUTF8(strResult);

	return strName->atom();*/
	//strResult.AddString(String::Pointers(strName).p8);
	//FormatMessageV(
	//String::
	//return kAvmThunkUndefined;//Modify this please!
}

}
