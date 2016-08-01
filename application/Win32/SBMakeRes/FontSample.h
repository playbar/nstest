#if !defined(AFX_FONTSAMPLE_H__91FC408B_5DCD_11D3_93EF_00105A763C82__INCLUDED_)
#define AFX_FONTSAMPLE_H__91FC408B_5DCD_11D3_93EF_00105A763C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontSample.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// VGFontSample window
//////////////////////////////////////////////////////
//字体示例显示控件，显示指定的中文英文字体的实际显示
//结果，显示结果不表示该字体的大小
//中文显示内容为‘汉字’，英文为‘ABCD’
//////////////////////////////////////////////////////
class VGFontSample : public CButton
{
// Construction
public:
	VGFontSample();
	void ToWide(LPCTSTR str,unsigned short *out);
// Attributes
public:
	int	m_nEnPecent;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(VGFontSample)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:					// CString
	void SetFont(LPCTSTR strFont,LPCSTR strFontEn);
	//设置要使用的中/英文字体
	//参数logFont表示要设置的字体，bChinese=TRUE表示设置中文字体
	//bChinese=FALSE表示设置英文字体
	void SetFont(LOGFONT&logFont,BOOL bChinese=TRUE);
	void SetFont(CFont*pFont,BOOL bChinese=TRUE);
	virtual ~VGFontSample();
	void SetEnPecent(int p);
	// Generated message map functions
protected:
	
	//要使用的中/英文字体
	CFont m_ch,m_en;
	CString m_strCh,m_strEn;
	//{{AFX_MSG(VGFontSample)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTSAMPLE_H__91FC408B_5DCD_11D3_93EF_00105A763C82__INCLUDED_)
