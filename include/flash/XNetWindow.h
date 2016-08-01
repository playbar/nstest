#ifndef _X_NET_WINDOW_H_
#define _X_NET_WINDOW_H_

#include "../dom/XClient.h"

class XSWFPlayer;
#define MAX_CMDS 64*3

class XNetWindow : public XWindow
{
public:
	XSWFPlayer*m_pPlayer;
	XNetWindow(XSWFPlayer*player);
	~XNetWindow();
	XU32 m_nCmds[MAX_CMDS];
	int  m_nCmdSet,m_nCmdGet;
	void CallBack();
	XBOOL AddCommand(XU32 nCmd,XU32 nType,XU32 nData)
	{
		int vi=m_nCmdSet+3;
		if(vi>=MAX_CMDS)
			vi-=MAX_CMDS;
		if(vi==m_nCmdGet) return XFALSE;
		m_nCmds[m_nCmdSet]=nCmd;
		m_nCmds[m_nCmdSet+1]=nType;
		m_nCmds[m_nCmdSet+2]=nData;
		m_nCmdSet=vi;
		//if(m_nCmdSet==m_nCmdGet) return XFALSE;
		return XTRUE;
	}
private:
	void	OnConnect(XClient *pClient);
	void	OnStarted(XClient *pClient);
	void	OnRecive(XClient *pClient);		 
	void	OnFinished(XClient *pClient,XBOOL finished = XTRUE);

public:
	virtual XBOOL Handle(XU32 nOperator,XU32 nParam1,XU32 nParam2);
	XBOOL	HandleSocketMsg(XU32 nCmd, XClient *pClient);
protected:
	//XMutex m_ptLock;
};

#endif
