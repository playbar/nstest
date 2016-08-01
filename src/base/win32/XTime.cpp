// XTime.cpp: implementation of the XTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XTime.h"
#include "XSort.h"
#include <time.h>
#include <mmsystem.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XPCTSTR XTime::strWeekDay[]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
XPCTSTR XTime::strMonth[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};


XTime::~XTime()
{

}


XPCTSTR XTime::Month(int id)
{
	if(id<1||id>12) return XNULL;
	return strMonth[id-1];
}
XPCTSTR XTime::WeekDay(int id)
{
	if(id<0||id>7) return XNULL;
	return strWeekDay[id];
}

XINT XTime::WeekDay(XString8&str)
{
	return str.IndexOf(strWeekDay,sizeof(strWeekDay)/sizeof(XPCTSTR),-1,XString8::XLS_NOCASE);
}

XINT XTime::Month(XString8&str)
{
	return str.IndexOf(strMonth,sizeof(strMonth)/sizeof(XPCTSTR),-1,XString8::XLS_NOCASE)+1;
}

void XTime::SetCurrentTime()
{
	m_nTime=::time(XNULL);
	//m_nTickCount=(GetTimeMSec()%1000);
}

int XTime::GetYear()
{
	time_t timep = m_nTime;
	return localtime(&timep)->tm_year+1900;
//	return localtime((time_t*)&m_nTime)->tm_year+1900;
}

int XTime::GetMonth()
{
	time_t timep = m_nTime;
	return localtime(&timep)->tm_mon+1;
//	return localtime((time_t*)&m_nTime)->tm_mon+1;
}

int XTime::GetDay()
{
	time_t timep = m_nTime;
	return localtime(&timep)->tm_mday;
//	return localtime((time_t*)&m_nTime)->tm_mday;
}

int XTime::GetHour()
{
	time_t timep = m_nTime;
	return localtime(&timep)->tm_hour;
//	return localtime((time_t*)&m_nTime)->tm_hour;
}

int XTime::GetMinute()
{
	time_t timep = m_nTime;
	return localtime(&timep)->tm_min;
//	return localtime((time_t*)&m_nTime)->tm_min;
}

int XTime::GetSecond()
{
	time_t timep = m_nTime;
	return localtime(&timep)->tm_sec;
//   return localtime((time_t*)&m_nTime)->tm_sec;
}

int XTime::GetDayOfWeek()
{
	time_t timep = m_nTime;
	return localtime(&timep)->tm_wday+1;
//   return localtime((time_t*)&m_nTime)->tm_wday+1;
}

void XTime::SetTime(int year, int month, int day, int hour, int min, int sec)
{
	struct tm atm;
	atm.tm_sec = sec;
	atm.tm_min = min;
	atm.tm_hour = hour;
	atm.tm_mday = day;
	atm.tm_mon = month - 1;        // tm_mon is 0 based
	atm.tm_year = year - 1900;     // tm_year is 1900 based
	atm.tm_isdst = -1;
	m_nTime = mktime(&atm);
}

void XTime::SetYear(int year)
{
	struct tm *t=localtime((time_t*)&m_nTime);
	t->tm_year=year-1900;
	m_nTime=mktime(t);
}

void XTime::SetMonth(int mon)
{
	struct tm *t=localtime((time_t*)&m_nTime);
	t->tm_mon=mon-1;
	m_nTime=mktime(t);
}

void XTime::SetDay(int day)
{
   	struct tm *t=localtime((time_t*)&m_nTime);
	t->tm_mday=day;
	m_nTime=mktime(t);
}

void XTime::SetHour(int hour)
{
	struct tm *t=localtime((time_t*)&m_nTime);
	t->tm_hour=hour;
	m_nTime=mktime(t);
}

void XTime::SetMinute(int min)
{
	struct tm *t=localtime((time_t*)&m_nTime);
	t->tm_min=min;
	m_nTime=mktime(t);
}

void XTime::SetSecond(int sec)
{
	struct tm *t=localtime((time_t*)&m_nTime);
	t->tm_sec=sec;
	m_nTime=mktime(t);
}

XBOOL XTime::SetTime(XString8 &strTime)
{
	//int id=strTime.Find(',');
	//if(id<3) return XFALSE;
	XU32 is=0,id=0;
	XU16 nYear=0,nMon=0,nDay=0,nHour=0,nMin=0,nSec=0;
	while(id<=strTime.GetLength())
	{
		if(strTime[id]==' '||strTime[id]==','||strTime[id]=='-'||strTime[id]==0)
		{
			if(id>is+1)
			{
				XString8 str;
				strTime.Mid(str,is,id-is);
				XString8Array list;
				list.SplitString(str,':');
				if(list.GetSize()>1)
				{
					nHour=list[0].ToInt();
					nMin=list[1].ToInt();
					if(list.GetSize()>2)
						nSec=list[2].ToInt();
				}
				else 
				{
					int id=Month(str);
					if(id>0&&id<=12&&nMin<1) nMon=id;
					else 
					{
						id=str.ToInt();
						if(id>0&&id<32&&nDay<1) nDay=id;
						else if(id>=1900) nYear=id;
						else if(!nHour)
							nHour=id;
					}
				}
			}
			id++;
			is=id;
		}
		id++;
	}
	if(nYear<1900||nMon<1||nDay<1) return XFALSE;
	SetTime(nYear,nMon,nDay,nHour,nMin,nSec);
	return XTRUE;
}

void XTime::GetTime(XString8 &strTime)
{
	int v=GetDayOfWeek();
	strTime=WeekDay(v);
	v=GetDay();
	if(v<10) strTime+=",0";
	else	 strTime+=",";
	strTime+=v;
	strTime+=' ';
	//v=GetMonth();
	strTime+=Month(GetMonth());
	strTime+=' ';
	strTime+=GetYear();
	strTime+=' ';
	if(this->GetHour()<10)
		strTime+='0';
	strTime+=GetHour();
	strTime+=':';
	if(this->GetMinute()<10)
		strTime+='0';
	strTime+=GetMinute();
	strTime+=':';
	if(this->GetSecond()<10)
		strTime+='0';
	strTime+=GetSecond();
	strTime+=" GMT";
}

void XTime::SetTime(const XFILETIME &t)
{
	FILETIME localTime;
	if (!FileTimeToLocalFileTime((FILETIME*)&t, &localTime))
	{
		m_nTime = 0;
		return;
	}

	// then convert that time to system time
	SYSTEMTIME sysTime;
	if (!FileTimeToSystemTime(&localTime, &sysTime))
	{
		m_nTime = 0;
		return;
	}
	SetTime(sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		    sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	// then convert the system time to a time_t (C-runtime local time)
	//CTime timeT(sysTime, nDST);
	//*this = timeT;
}

XU32 XTime::GetTickCount()
{
	return ::GetTickCount();
}

XU32 XTime::GetTimeMSec()
{
	return //GetTickCount();
			timeGetTime() & 0x7FFFFFFF;
}

void XTime::SetDayOfWeek(int day)
{
	struct tm *t=localtime((time_t*)&m_nTime);
	t->tm_wday=day-1;
	m_nTime=mktime(t);
}

XBOOL XTime::GetFrequency(long& high, XU32& low)
{
	LARGE_INTEGER time;
	if(!QueryPerformanceCounter(&time)) return XFALSE;
	high=time.HighPart;
	low=time.LowPart;
	return XTRUE;
}
