// XXObjectDate.cpp: implementation of the XXObjectDate class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectDate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XXObjectDate::XXObjectDate(ScriptPlayer*pRoot):XXObject(pRoot)
{
	m_bNoCase=XTRUE;
	m_nTickCount=0;
}

XXObjectDate::~XXObjectDate()
{

}


XBOOL XXObjectDate::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	switch(id)
	{
		case _SYSID(getDate):
		case _SYSID(getDay):
		case _SYSID(getFullYear):
		case _SYSID(getHours):
		case _SYSID(getMinutes):
		case _SYSID(getMilliseconds):
		case _SYSID(getMonth):
		case _SYSID(getSeconds):
		case _SYSID(getTime):
		case _SYSID(getTimezoneOffset):
		case _SYSID(getUTCDate):
		case _SYSID(getUTCDay):
		case _SYSID(getUTCFullYear):
		case _SYSID(getUTCHours):
		case _SYSID(getUTCMilliseconds):
		case _SYSID(getUTCMinutes):
		case _SYSID(getUTCMonth):
		case _SYSID(getUTCSeconds):
		case _SYSID(getUTCYear):
		case _SYSID(getYear):
		
		case _SYSID(setDate):
		//case _SYSID(setDay):
		case _SYSID(setFullYear):
		case _SYSID(setHours):
		case _SYSID(setMilliseconds):
		case _SYSID(setMinutes):
		case _SYSID(setMonth):
		case _SYSID(setSeconds):
		case _SYSID(setTime):
		case _SYSID(setUTCDate):
		//case _SYSID(setUTCDay):
		case _SYSID(setUTCFullYear):
		case _SYSID(setUTCHours):
		case _SYSID(setUTCMilliseconds):
		case _SYSID(setUTCMinutes):
		case _SYSID(setUTCMonth):
		case _SYSID(setUTCSeconds):
		//case _SYSID(setUTCYear):
		case _SYSID(setYear):
		case _SYSID(UTC):
			var=this;
			var.ToFunction(id);
			return XTRUE;
	}
	return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectDate::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	int nSize=list.GetSize();
	switch(id)
	{
		case _SYSID(getUTCDate):
		case _SYSID(getDate):
			 pCnt->pStack->Push(m_time.GetDay());return XTRUE;
		case _SYSID(getUTCDay):
		case _SYSID(getDay):
			 pCnt->pStack->Push((m_time.GetDayOfWeek()-1));return XTRUE;
		case _SYSID(getUTCFullYear):
		case _SYSID(getFullYear):
			 pCnt->pStack->Push((m_time.GetYear()));return XTRUE;
		case _SYSID(getUTCHours):
		case _SYSID(getHours):
			 pCnt->pStack->Push((m_time.GetHour()));return XTRUE;
		case _SYSID(getUTCMinutes):
		case _SYSID(getMinutes):
			 pCnt->pStack->Push((m_time.GetMinute()));return XTRUE;
		case _SYSID(getUTCMilliseconds):
		case _SYSID(getMilliseconds):
			{
				//int v=m_time.GetTime();
				//v=v%1000;
				pCnt->pStack->Push(m_nTickCount);
			}return XTRUE;
		case _SYSID(getUTCMonth):
		case _SYSID(getMonth):
			pCnt->pStack->Push((m_time.GetMonth()));return XTRUE;
		case _SYSID(getUTCSeconds):
		case _SYSID(getSeconds):
			pCnt->pStack->Push((m_time.GetSecond()));return XTRUE;
		case _SYSID(getTime):
			pCnt->pStack->Push((m_time.GetTime()*1000+m_nTickCount));return XTRUE;
		case _SYSID(getTimezoneOffset):
			pCnt->pStack->Push((8));return XTRUE;				
		case _SYSID(getUTCYear):
		case _SYSID(getYear):
		case _SYSID(setYear):
			pCnt->pStack->Push((m_time.GetYear()));return XTRUE;
		case _SYSID(setDate):
		case _SYSID(setUTCDate):
			m_time.SetDay(list[0].ToInt());
			return XTRUE;
		//case _SYSID(setDay):
		case _SYSID(setUTCFullYear):
		case _SYSID(setFullYear):
			if(nSize)
			{
					int v=list[0].ToInt();
					if(v<100) v+=1900;
					m_time.SetYear(v);
			}return XTRUE;
		case _SYSID(setUTCHours):
		case _SYSID(setHours):
			if(nSize)
				m_time.SetHour(list[0].ToInt());return XTRUE;
		case _SYSID(setUTCMilliseconds):
		case _SYSID(setMilliseconds):
			if(nSize)
			{
				int v=list[0].ToInt();
				if(v<0) v=0;
				else if(v>999) v=999;
				m_nTickCount=v;
				//m_time=v+(m_time.GetTime()/1000*1000);
			}return XTRUE;
		case _SYSID(setUTCMinutes):
		case _SYSID(setMinutes):
			if(nSize)
				m_time.SetMinute(list[0].ToInt());
			return XTRUE;
		case _SYSID(setUTCMonth):
		case _SYSID(setMonth):
			if(nSize)
				m_time.SetMonth(list[0].ToInt());
			return XTRUE;
		case _SYSID(setUTCSeconds):
		case _SYSID(setSeconds):
			if(nSize)
				m_time.SetSecond(list[0].ToInt());
			return XTRUE;
		case _SYSID(setTime):
			if(nSize)
				m_time=list[0].ToInt();
			return XTRUE;				
		//case _SYSID(setUTCYear):
		
		case _SYSID(UTC):
			XXObjectDate*pObj=(XXObjectDate*)CreateObject(pCnt,list,XFALSE);
			if(pObj)
			{
				int v=pObj->m_time-m_time;
				pCnt->pStack->FreePop();
				pCnt->pStack->Push((v));
			}return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

void XXObjectDate::InitObject( )
{
	_SYSCALL(getDate);
	_SYSCALL(getDay);
	_SYSCALL(getFullYear);
	_SYSCALL(getHours);
	_SYSCALL(getMinutes);
	_SYSCALL(getMilliseconds);
	_SYSCALL(getMonth);
	_SYSCALL(getSeconds);
	_SYSCALL(getTime);
	_SYSCALL(getTimezoneOffset);
	_SYSCALL(getUTCDate);
	_SYSCALL(getUTCDay);
	_SYSCALL(getUTCFullYear);
	_SYSCALL(getUTCHours);
	_SYSCALL(getUTCMilliseconds);
	_SYSCALL(getUTCMinutes);
	_SYSCALL(getUTCMonth);
	_SYSCALL(getUTCSeconds);
	_SYSCALL(getUTCYear);
	_SYSCALL(getYear);
	
	_SYSCALL(setDate);
	//_SYSCALL(setDay);
	_SYSCALL(setFullYear);
	_SYSCALL(setHours);
	_SYSCALL(setMilliseconds);
	_SYSCALL(setMinutes);
	_SYSCALL(setMonth);
	_SYSCALL(setSeconds);
	_SYSCALL(setTime);
	_SYSCALL(setUTCDate);
	//_SYSCALL(setUTCDay);
	_SYSCALL(setUTCFullYear);
	_SYSCALL(setUTCHours);
	_SYSCALL(setUTCMilliseconds);
	_SYSCALL(setUTCMinutes);
	_SYSCALL(setUTCMonth);
	_SYSCALL(setUTCSeconds);
//	_SYSCALL(setUTCYear);
	_SYSCALL(setYear);
	_SYSCALL(UTC);
	
}

XXObject* XXObjectDate::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	XXObjectDate*pObj=CreateClass(this,XFALSE,m_pRoot);
	//if(list.GetSize())
	if(pObj)
	{
		pObj->m_pSuperObj=GetSuperObject();
		pObj->Construct(pCnt,list,this);
	}
	else pCnt->pStack->Push((pObj));
	return pObj;
}

void XXObjectDate::GetString(XXVar&var)//XString8 &str)
{
	XString8 str;
	m_time.GetTime(str);
	//var.SetString(str,str.GetLength());
	var.AttachString(str.DataTo());
}



XU32 XXObjectDate::Construct(XSWFCONTEXT *pCnt, XXVARLIST &list, XXObject*)
{
		int nSize=list.GetSize();
		switch(nSize)
		{
		case 0:
			m_time.SetCurrentTime();
			m_nTickCount=XTime::GetTickCount()%1000;
			//list[0].ToString();
			//m_strMessage=list[0].strData;
			break;
		case 1:
			m_time=list[0].ToInt();
			break;
		//case 3:
		case 7:
			{
				int m=list[6].ToInt();
				if(m<0) m=0;
				else if(m>999) m=999;
				m_nTickCount=m;
				//m_time=m_time.GetTime()/1000*1000+m;
			//m_time=(m_time.GetTime()/1000*1000+(list[6].ToInt()));
			}
		case 6:
			m_time.SetSecond(list[5].ToInt());
		case 5:
			m_time.SetMinute(list[4].ToInt());
		case 4:
			{
				int h=list[3].ToInt();
				m_time.SetHour(h);
			}
		case 3:
			{
				int year=list[0].ToInt();
				if(year<100)
					year+=1900;
				m_time.SetYear(year);
				int mon=list[1].ToInt()+1;
				if(mon<1) mon=1;
				else if(mon>12) mon=12;
				m_time.SetMonth(mon);
				int day=list[2].ToInt();
				m_time.SetDay(day);
			}break;
		}
		pCnt->pStack->Push(this);
		return (XU32)this;
}
