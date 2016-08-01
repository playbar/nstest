#if !defined(AFX_DRIVERBOX_H__81C3AA5B_A119_415C_8445_34B80E95EFAA__INCLUDED_)
#define AFX_DRIVERBOX_H__81C3AA5B_A119_415C_8445_34B80E95EFAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DriverBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDriverBox window
/////////////////////////////////////////////////////////
// 显示可读写驱动器(分区)列表的下拉式组合框
// 显示内容不包含可移动设备
/////////////////////////////////////////////////////////
class CDriverBox : public CComboBox
{
// Construction
public:
	//构造函数
	CDriverBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDriverBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	//检查指定的驱动器符号是否合法
	//参数表示驱动器名称格式为 ‘盘符:\’如‘c:\’
	BOOL CheckDisk(LPCSTR drv);
	//选择指定的驱动器，参数表示驱动器名称，格式同上
	void Select(LPCSTR driver);
	//得到选择的驱动器名称
	BOOL GetSelect(CString&driver);
	virtual ~CDriverBox();

	// Generated message map functions
protected:
	//驱动器名称列表
	CStringList m_drvList;
	//{{AFX_MSG(CDriverBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRIVERBOX_H__81C3AA5B_A119_415C_8445_34B80E95EFAA__INCLUDED_)
