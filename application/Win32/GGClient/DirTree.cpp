// DirTree.cpp : implementation file
//

#include "stdafx.h"
#include "DirTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirTree

CDirTree::CDirTree()
{
	m_nCount=1;
}

CDirTree::~CDirTree()
{
}


BEGIN_MESSAGE_MAP(CDirTree, CListBox)
	//{{AFX_MSG_MAP(CDirTree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirTree message handlers
//比较函数，显示总不排序，没有操作
int CDirTree::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
	// TODO: Add your code to determine the sorting order of the specified items
	// return -1 = item 1 sorts before item 2
	// return 0 = item 1 and item 2 sort the same
	// return 1 = item 1 sorts after item 2
	
	return 0;
}
//绘制条目
void CDirTree::DrawItem(LPDRAWITEMSTRUCT lpDS) 
{
	// TODO: Add your code to draw the specified item
	if(lpDS->itemID==LB_ERR) return;
	CDC* pDC=CDC::FromHandle(lpDS->hDC);
	//
	CRect r(lpDS->rcItem);	
	BOOL bEnable=(GetStyle()&WS_DISABLED)==0;
	
	CBrush br;
	//绘制条目背景
	if( bEnable&&(/*(lpDS->itemAction & (ODA_SELECT |ODA_SELECT|ODA_FOCUS))
		&&*/(lpDS->itemState & ODS_SELECTED))|
		((lpDS->itemState&ODS_COMBOBOXEDIT)&&(lpDS->itemState&ODS_FOCUS)))
	{  //选中并被允许
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
	//创建显示图标列表
	CImageList list;
	list.Create(_T("IDB_DIRBROWNS"),16,0,0xffffff);	
	int image=2;
	//计算正确的显示图标和缩进距离
	if(lpDS->itemData>0) //该条目为父目录中的一级
	{   //数字表示目录级别
		r.left+=(lpDS->itemData-1)*8;
		if(lpDS->itemData==(UINT)m_nCount) image++;
	}
	else //该条目为当前目录中的一个子目录 
		{r.left+=m_nCount*8;image=1;}
	list.Draw(pDC,image,r.TopLeft(),bEnable?ILD_TRANSPARENT:ILD_BLEND);	
	r.left+=20;
	//显示目录名
	CString text=m_pathList.GetAt(m_pathList.FindIndex(lpDS->itemID));
	if(lpDS->itemID==0) text+='\\';
	pDC->TextOut(r.left,r.top,text);
}

void CDirTree::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	
}

//设置当前显示目录，参数为要设置的目录的全名
//设置时同时往列表中加入目录名字符串用于列表显示
//其附加数据的值等于0时表示当前目录下的子目录，附加数字
//大于0时表示从根目录开始的级别
BOOL CDirTree::SetPath(LPCWSTR path)
{
	//清除原来列表中的内容
    ResetContent();
	m_pathList.RemoveAll();
	ASSERT(strlen(path)>2);
	ASSERT(path[2]=='\\'||path[2]=='/');
	//加入根目录
	CString temp(path,2);
	//CString fPath=temp;
	//temp+='\\';
	int index=AddString(temp);
	m_pathList.AddTail(temp);
	SetItemData(index,1);
	m_nCount=1;
	LPCWSTR p=path+3;
	//加入各级父目录
	while(p[0])
	{
		temp.Empty();
		while(p[0])
		{
			if(p[0]=='\\'||p[0]=='/') {p++;break;}
			temp+=p[0];
			p++;
		}
		if(temp.IsEmpty()) continue;
		index=AddString(temp);		
		m_pathList.AddTail(temp);
		m_nCount++;
		SetItemData(index,m_nCount);		
	}
	//利用查找函数加入当前目录的全部子目录
	WIN32_FIND_DATA data;
	CString fPath=path;fPath+=_T("\\*.*");
	HANDLE handle=::FindFirstFile(fPath,&data);
	while(handle!=INVALID_HANDLE_VALUE)
	{
		if((data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)&&
			(data.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)==0)
		{   //只加入非隐含的目录名
			//if(strcmp(data.cFileName,".")!=0&&
			//   strcmp(data.cFileName,"..")!=0)
			if(data.cFileName[0]!='.')
			{
				index=AddString(data.cFileName);
				SetItemData(index,0);
				m_pathList.AddTail(data.cFileName);
			}
		}
		if(!::FindNextFile(handle,&data)) break;
	}
	SetCurSel(m_nCount-1);
	return TRUE;
}
//得到选中的目录全名
BOOL CDirTree::GetSelected(CString &path)
{
   //得到选择的条目索引
   int index=GetCurSel();
   ASSERT(m_pathList.GetCount()>=m_nCount);
   if(index<0||index>=m_pathList.GetCount()) return FALSE;
   DWORD iData=GetItemData(index);
   if(iData>0)//所选中的对象为父目录中的某一级
   {
	   path.Empty();
	   ASSERT(iData<=(UINT)m_nCount);
	   POSITION pos=m_pathList.GetHeadPosition();
	   while(iData--)
	   {   if(!path.IsEmpty()) path+='\\';
		   path+=m_pathList.GetNext(pos);
	   }
	   if(GetItemData(index)==1) path+='\\';
	   return TRUE;
   }
   else //所选中的目录为当前目录的一个子目录
   {
	  path.Empty();
	  iData=m_nCount;
	  POSITION pos=m_pathList.GetHeadPosition();
	  while(iData--)
	  {   path+=m_pathList.GetNext(pos);
	      path+='\\';
	  }
   }
   path+=m_pathList.GetAt(m_pathList.FindIndex(index));
   return TRUE;
}
