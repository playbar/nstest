#include "StdAfxflash.h"
#include "splayer.h"
#include "ShellCore.h"
#include "avm2.h"
using namespace avmplus;
#include "AS3Timer.h"
#include "AS3TimerEvent.h"

namespace avmshell{
TimerClass::TimerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TimerObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TimerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TimerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
void TimerObject::Release()
{
	if(m_bRunning&&IsRunning())
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->m_pAVM2->RemoveObject(this);
		player->RemoveCaller(m_nID);
	}

	m_bRunning=false;
	EventDispatcherObject::Release();
}

TimerObject::TimerObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nCurrentCount=0;
	m_nDelay=0;
	m_nLastTick=0;
	m_nRepeatCount=0;
	m_bRunning=false;
	m_nID=0;
}
//////////////////////////////////////////////////////////
//Native Method start...

void TimerObject::AS3_reset()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	if(m_bRunning)
	{
		player->RemoveCaller(m_nID);
		player->m_pAVM2->RemoveObject(this);
		m_bRunning=false;
	}
		//AS3_stop();
	//	player->RemoveCaller(m_nID);
	//else
	//	player->m_pAVM2->PushObject(this);
	m_nCurrentCount=0;
	m_bRunning=false;
}

void TimerObject::AS3_start()
{
	//Add your act code here...	
	if(m_bRunning) return;
	m_nLastTick=XTime::GetTimeMSec();
	m_bRunning=true;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->m_pAVM2->PushObject(this);
	m_nID=player->AS3AddCaller(this,m_nDelay,NULL,3,(ShellCodeContext*)core()->codeContext());//((ShellCore*)core())->GetCodeContext());
}

void TimerObject::AS3_stop()
{
	//Add your act code here...
	if(!m_bRunning) return;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->RemoveCaller(m_nID);
	player->m_pAVM2->RemoveObject(this);
	m_bRunning=false;	
}

void TimerObject::OnTimer(XU32)
{
	if(!m_bRunning) return;
	/*XSWFPlayer*player=((ShellCore*)core())->GetPlayer();	
	TimerEventObject*pEvent=(TimerEventObject*)player->m_pAVM2->
		CreateEventObject(XAS3_EVENT_timer,XAS3EVENT_TIMER,false,0,(int*)this);
	this->AS3_dispatchEvent(pEvent);*/
	//if(this->m_nDelay==20)
	//	int v=0;


	m_nCurrentCount++;
	if(m_nRepeatCount&&m_nCurrentCount>=m_nRepeatCount)
	{
		AS3_stop();
		OnTimerEvent(XAS3_EVENT_timer);
		
		OnTimerEvent(XAS3_EVENT_timerComplete);
		//player->m_pAVM2->ResetEvent(pEvent,XAS3_EVENT_timerComplete,this);
		//this->AS3_dispatchEvent(pEvent);		
	}
	else
	{
		if(m_nDelay==40)
			int v=0;
		OnTimerEvent(XAS3_EVENT_timer);
	}
}

}
