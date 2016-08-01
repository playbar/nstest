// XTime.h: interface for the XTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTIME_H__4C0E387A_381D_42C1_A320_F4C3AF63502A__INCLUDED_)
#define AFX_XTIME_H__4C0E387A_381D_42C1_A320_F4C3AF63502A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GGType.h"
#include "XString.h"

typedef struct _XFILETIME
{
	XU32 dwLowDateTime;
    XU32 dwHighDateTime;
}XFILETIME;

class _XEXT_CLASS XTimeSpan
{
public:
	XTimeSpan() {m_nTime=0;}
	XTimeSpan(long t){m_nTime=t;}
	XTimeSpan(int day,int hour,int min,int sec)
	{
		m_nTime=sec+60*(min+60*(hour+24*day));
	}
	operator long&(){return m_nTime;}
public:
	long m_nTime;
};

class _XEXT_CLASS XTime  
{
public:
	long GetTime(){return m_nTime;}
//	XU16 GetMTime(){return m_nTickCount;}
//	void SetTime(XU32 nTime)
//	{
//		m_nTime=nTime/1000;
//		m_nTickCount=nTime%1000;
//	}
	XBOOL IsValid(){return m_nTime!=0&&m_nTime!=-1;}
	XTime(long nTime){m_nTime=nTime;}
	void operator=(long nTime){m_nTime=nTime;}
	void operator=(const XTime&t){m_nTime=t.m_nTime;}
	XTime(){m_nTime=0;}
	virtual ~XTime();
	long operator-(const XTime&t){return m_nTime-t.m_nTime;}
	XBOOL operator==(const XTime&t) {return m_nTime==t.m_nTime;}
	XBOOL operator!=(const XTime&t) {return m_nTime!=t.m_nTime;}
	XBOOL operator>=(const XTime&t) {return m_nTime>=t.m_nTime;}
	XBOOL operator<=(const XTime&t) {return m_nTime<=t.m_nTime;}
	XBOOL operator<(const XTime&t) {return m_nTime<t.m_nTime;}
	XBOOL operator>(const XTime&t) {return m_nTime>t.m_nTime;}
	void operator-=(const XTimeSpan&t) {m_nTime-=t.m_nTime;}
	void operator+=(const XTimeSpan&t) {m_nTime+=t.m_nTime;}
	//operator XU32&(){return m_nTime;}
	
public:
	static XBOOL GetFrequency(long& high,XU32& low);
	void SetDayOfWeek(int day);
	static XU32 GetTimeMSec();
	
	static XU32 GetTickCount();
	void SetTime(const XFILETIME&t);
	void GetTime(XString8&strTime);
	XBOOL SetTime(XString8&strTime);
	void SetSecond(int sec);
	void SetMinute(int min);
	void SetHour(int hour);
	void SetDay(int day);
	void SetMonth(int mon);
	void SetYear(int year);
	void SetTime(int year,int month,int day,int hour,int min,int sec);
	int GetDayOfWeek();
	int GetSecond();
	int GetMinute();
	int GetHour();
	int GetDay();
	int GetMonth();
	int GetYear();
	void SetCurrentTime();
	static XINT Month(XString8&str);
	static XINT WeekDay(XString8&str);
	static XPCTSTR Month(int id);
	static XPCTSTR WeekDay(int id);
protected:
	unsigned long m_nTime;
	//XU16 m_nTickCount;
	static XPCTSTR strWeekDay[];
	static XPCTSTR strMonth[];
};

#endif // !defined(AFX_XTIME_H__4C0E387A_381D_42C1_A320_F4C3AF63502A__INCLUDED_)
