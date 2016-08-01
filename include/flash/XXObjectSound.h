// XXObjectSound.h: interface for the XXObjectSound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTSOUND_H__361061F7_893F_48F8_95AC_64AC2C61985B__INCLUDED_)
#define AFX_XXOBJECTSOUND_H__361061F7_893F_48F8_95AC_64AC2C61985B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"
#include "sobject.h"

class XXObjectSound : public XXObject  
{
	XDEC_XOBJECT(XXObjectSound)
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*);
	XXObjectSound(ScriptPlayer*pRoot);
	virtual ~XXObjectSound();
	/*virtual void Release()
	{
		XXObject::Release();
		FREEOBJECT(m_pClip);
		//StopSound();
	}*/
	virtual void Destruct()
	{
		StopSound();
		FREEOBJECT(m_pClip);
		FREEOBJECT(m_pSuper);
	}
	/*virtual void Destroy()
	{
		XXObject::Destroy();
		FREEOBJECT(m_pClip);
		Destruct();
		//StopSound();
	}*/
	virtual XU32 GetObjectType()
	{
		return XXOBJ_SOUND;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
protected:
	void StopSound();
	XSoundChannel* m_pChannel;
	XSound* m_pSound;
	XXObject* m_pClip;
	XBOOL m_bStream;
	void InitObject();
};

#endif // !defined(AFX_XXOBJECTSOUND_H__361061F7_893F_48F8_95AC_64AC2C61985B__INCLUDED_)
