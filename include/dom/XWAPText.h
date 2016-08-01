// XWAPText.h: interface for the XWAPText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWAPTEXT_H__D23CBAB1_4A77_4E98_9B65_AB4815A19CEA__INCLUDED_)
#define AFX_XWAPTEXT_H__D23CBAB1_4A77_4E98_9B65_AB4815A19CEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class XWAPText : public XString8  
{
public:
	void AddBR();
	void AddLink(XPCTSTR strURL,XPCTSTR strTitle,XPCTSTR strImg);
	void AddAttrib(XU16 id,XString8Array&list);
	void AddAttrib(XU16 id,XTime&time);
	void AddImage(XPCTSTR strURL,XPCTSTR strTitle);
	XBOOL AddLink(XPCTSTR strURL,XPCTSTR strTitle,XU32 nPicks=0);
	XBOOL AddAttrib(XU16 nID,XINT nValue);
	XBOOL AddAttrib(XPCTSTR pStr,XPCTSTR strValue);
	XBOOL Finish();
	XBOOL AddAttrib(XU16 nID,XPCTSTR strValue);
	XBOOL AddText(XPCTSTR strTxt);
	XBOOL EndToken();
	XBOOL AddToken(XU16 nID);
	XBOOL Create(XBOOL bWml=XTRUE,XU8 nCharset=XCHARSET_GB2312);
	void SetMode(int nMode){m_nMode=nMode;}
	enum {XW_EMPTY,XW_TEXT,XW_TOKEN};
	void Final();
	XWAPText();
	XWAPText(XU8 nCharset)
	{
		m_nMode=XW_EMPTY;
		Create(nCharset);
	}
	virtual ~XWAPText();
protected:
	XU8 m_nMode;
	XStack<XU16> m_tokens;
};

#endif // !defined(AFX_XWAPTEXT_H__D23CBAB1_4A77_4E98_9B65_AB4815A19CEA__INCLUDED_)
