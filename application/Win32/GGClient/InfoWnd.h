#pragma once


// CInfoWnd

class CInfoWnd : public CButton
{
	DECLARE_DYNAMIC(CInfoWnd)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetMemory(int nMem)
	{
		m_nMemory=nMem;
		if(m_nMaxMemory<nMem)
			m_nMaxMemory=nMem;
	}
	void SetFrames(int nFrames)
	{
		m_nFrames=nFrames;
		if(m_nMinFrames>nFrames)
			m_nMinFrames=nFrames;
	}
	void SetRate(int nRate)
	{
		m_nNetRate=nRate;
		if(m_nMaxNetRate<nRate)
			m_nMaxNetRate=nRate;
	}
	void UpdateInfo();
	
public:
	int		GetMemoryPecent(int v)
	{
		v=v*120/256;
		if(v<0) v=0;
		if(v>120) v=120;
		return v;
	}
	int		GetFramePecent(int v)
	{
		v=v*120/100;
		if(v<0) v=0;
		if(v>120) v=120;
		return v;
	}
	int		GetRatePecent(int v)
	{
		v=v*120/128;
		if(v<0) v=0;
		if(v>120) v=120;
		return v;
	}
	void AddNetData(XU32 nBytes)
	{
		m_nNetBytes+=nBytes;
	}
	int		m_nMemory,m_nMaxMemory;
	int		m_nNetRate,m_nMaxNetRate;
	int		m_nFrames,m_nMinFrames;
	CInfoWnd();
	virtual ~CInfoWnd();
	XImage m_frame,m_frames,m_netrate,m_memory;
protected:
	XU32   m_nNetBytes,m_nLastNetBytes;
	XU32   m_nTime;
	DECLARE_MESSAGE_MAP()
};


