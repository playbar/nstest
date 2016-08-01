// DriverBox.cpp : implementation file
//

#include "stdafx.h"
#include "DriverBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDriverBox

CDriverBox::CDriverBox()
{
}

CDriverBox::~CDriverBox()
{
}


BEGIN_MESSAGE_MAP(CDriverBox, CComboBox)
	//{{AFX_MSG_MAP(CDriverBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriverBox message handlers
//创建控件时调用，该函数中查找所有可用的驱动器
//并将其名称保存到列表字符列表中和加入到显示列表中
void CDriverBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CComboBox::PreSubclassWindow();
    DWORD drives=GetLogicalDrives();
	int nums=0;	
	drives>>=2;
	char disk[]=_T("c:\\");	
	while(drives)
	{
		if(drives&0x01)
		{						
			UINT type=GetDriveType(disk);
			if(type==DRIVE_FIXED||type==DRIVE_REMOTE)
			{
				disk[2]=0;
				AddString(disk);
				m_drvList.AddTail(disk);
				disk[2]='\\';
			}
		}
		disk[0]++;
		drives>>=1;
	} 

}
//显示条目，显示在前部显示驱动其的特征图标
//在其后显示该驱动器的名称
void CDriverBox::DrawItem(LPDRAWITEMSTRUCT lpDS) 
{
	// TODO: Add your code to draw the specified item
	if(lpDS->itemID==LB_ERR) return;
	CDC* pDC=CDC::FromHandle(lpDS->hDC);
	//
	CRect r(lpDS->rcItem);	
	BOOL bEnable=(GetStyle()&WS_DISABLED)==0;
	CImageList list;
	//得到驱动器图标并显示
	list.Create("IDB_DIRBROWNS",16,0,0xffffff);
	r.left+=4;
	list.Draw(pDC,0,r.TopLeft(),bEnable?ILD_TRANSPARENT:ILD_BLEND);	
	r.left+=20;
	CBrush br;
	//绘制字符背景
	if( bEnable&&(/*(lpDS->itemAction & (ODA_SELECT |ODA_SELECT|ODA_FOCUS))
		&&*/(lpDS->itemState & ODS_SELECTED))|
		((lpDS->itemState&ODS_COMBOBOXEDIT)&&(lpDS->itemState&ODS_FOCUS)))
	{   //该条目被选中并被允许
		br.CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));		
		pDC->FillRect(&r, &br);		
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkMode(TRANSPARENT);		
		if(lpDS->itemState&ODS_COMBOBOXEDIT)
			pDC->DrawFocusRect(r);
	}		
	else 
	{	br.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		if(bEnable)pDC->FillRect(&r, &br); 		
		pDC->SetTextColor(GetSysColor(bEnable?COLOR_WINDOWTEXT:COLOR_GRAYTEXT)); 
		pDC->SetBkMode(TRANSPARENT);		
	}
	CString text=m_drvList.GetAt(m_drvList.FindIndex(lpDS->itemID));
	//this->GetItemDataPtr(0);
	//GetLBText(lpDS->itemID,text);

	pDC->TextOut(r.left,r.top,text);
}

void CDriverBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	
}
//比较函数，该控件总不排序
int CDriverBox::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	return 0;
}
//得到选择的驱动器名称，参数用于输出
BOOL CDriverBox::GetSelect(CString &driver)
{
   int index=GetCurSel();
   if(index<0||index>=m_drvList.GetCount()) return FALSE;
   driver=m_drvList.GetAt(m_drvList.FindIndex(index));
   return TRUE;
}
//选择指定的驱动器，参数表示驱动器名称
void CDriverBox::Select(LPCSTR driver)
{
   int index=0;
   //格式化参数
   CString dr(driver);
   dr.MakeLower();
   //将名称转化成索引
   POSITION pos=m_drvList.GetHeadPosition();
   while(pos)
   {
	   if(m_drvList.GetNext(pos)==dr) break;
	   index++;
   }
   if(index>=m_drvList.GetCount()) index=0;
   SetCurSel(index);
}
//检查指定的驱动器名称是否合法
BOOL CDriverBox::CheckDisk(LPCSTR drv)
{  
   //格式化驱动器名
   CString dr(drv);
   dr.MakeLower();
   //在驱动器列表中查找是否存在指定的驱动器
   POSITION pos=m_drvList.GetHeadPosition();
   while(pos)
   {
	   //指定的驱动器存在，合法
	   if(m_drvList.GetNext(pos)==dr) return TRUE;
   }
   //没有，不合法
   return FALSE;
}
