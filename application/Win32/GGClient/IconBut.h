#if !defined(AFX_ICONBUT_H__14089BB6_18DF_11D2_A267_97849FE32001__INCLUDED_)
#define AFX_ICONBUT_H__14089BB6_18DF_11D2_A267_97849FE32001__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// IconBut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIconBut window
/////////////////////////////////////////////////////////
//图标按钮/菜单按钮控件
//如控件为图标按钮类型，则需要指定图标资源，控件在按钮上
//绘制该图标，并将文字作为提示内容
//如该控件为菜单按钮，则需要将按钮类型设置为‘自画’方式
//并指定该按钮的菜单资源，绘制时在按钮的文字后绘制一黑色三角
//按下该按钮可以弹出菜单
/////////////////////////////////////////////////////////
class CIconBut : public CButton
{
// Construction
public:
	//设置按钮对应的图表资源
	void SetID(LPCTSTR  strID){m_strResID=strID;}
	//构造函数。参数resID表示图标资源名称，如为菜单按钮，该参数可以为空
	//pMenuID表示菜单资源名称，如为图标按钮，该参数可以为空。subID表示弹出菜单
	//在指定菜单中的子菜单编号
	CIconBut(BOOL bPopup=FALSE);
	void SetPopup(BOOL bPopup){m_bPopup=bPopup;}
	//设置弹出菜单参数
	//menuID菜单资源名称
	//subID子菜单编号
	// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconBut)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CWnd*pWnd,LPCTSTR pCaption,const RECT&rect,UINT nID);
	
	virtual ~CIconBut();

	// Generated message map functions
protected:	
	CString m_strResID;
	//创建控件,enableTip表示是否显示鼠标提示
	BOOL Create( BOOL enableTip = TRUE );
	//绘制菜单三角形
	void    DrawTriangle(CDC* pDC, CPoint& pt, int size );
	//显示弹出菜单
	void    DisplayMenu();
	//菜单资源名称
	//UINT    m_menuID;
	//子菜单的自然编号
	//UINT    m_subID;
	//图标资源名称
	//UINT	m_iconID;
	//是否正弹出按钮菜单
	BOOL    m_bPopup;//是否为菜单类型
	BOOL    m_bLockDown;
	//提示控件
	CToolTipCtrl m_toolTip;
	//{{AFX_MSG(CIconBut)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONBUT_H__14089BB6_18DF_11D2_A267_97849FE32001__INCLUDED_)
