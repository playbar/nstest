#if !defined(AFX_DIRTREE_H__6B219840_EEC5_4867_815F_074F95460616__INCLUDED_)
#define AFX_DIRTREE_H__6B219840_EEC5_4867_815F_074F95460616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDirTree window
/////////////////////////////////////////////////////
// 以分级方式显示指定目录结构的列表框
// 显示时在每一条的前面按目录顺序以缩进方式显示一个目录
// 图标，其后显示该目录的名称
// 使用时指定一个目录全名，控件中显示该目录的全部父目录
// 结构，该目录下的全部子目录
// 用户可以通过鼠标选择活动目录
//////////////////////////////////////////////////////
class CDirTree : public CListBox
{
// Construction
public:
	//构造函数
	CDirTree();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirTree)
	public:
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	//得到选择的目录全称(包含盘符)
	BOOL GetSelected(CString&path);
	//设置用于显示的当前目录
	BOOL SetPath(LPCSTR path);
	virtual ~CDirTree();

	// Generated message map functions
protected:
	//父目录的数目
	int m_nCount;
	//全部目录字符列表
	CStringList m_pathList;
	//{{AFX_MSG(CDirTree)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRTREE_H__6B219840_EEC5_4867_815F_074F95460616__INCLUDED_)
