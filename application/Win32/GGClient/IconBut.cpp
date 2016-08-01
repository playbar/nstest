// IconBut.cpp : implementation file
//

#include "stdafx.h"
#include "IconBut.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconBut
//构造图标/菜单按钮
//resID:图标资源名称，如为菜单按钮，该参数可以为空
//pMenuID:菜单资源名称，如为图标按钮，该参数可以为空
//subID:子菜单编号，如为图标按钮，该参数无效
CIconBut::CIconBut(BOOL bPopup)
{
	m_bLockDown=FALSE;
	m_bPopup=bPopup;
	m_strResID="IDI_ADD";
}

CIconBut::~CIconBut()
{
}


BEGIN_MESSAGE_MAP(CIconBut, CButton)
	//{{AFX_MSG_MAP(CIconBut)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//消息处理函数，当提示控件启动时为提示控件提供正确的消息入口
LRESULT CIconBut::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( m_toolTip.GetSafeHwnd() != NULL )
	{ switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		{
			MSG msg;
			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;

			m_toolTip.RelayEvent(&msg);
		}
		}
	}
	return CButton::WindowProc(message, wParam, lParam);
}
//创建控件时调用
//enableTip表示是否创建提示控件
BOOL CIconBut::Create(BOOL enableTip )
{	
	//UINT nID=m_nID;
	if(!m_bPopup)
	{   //图标按钮，设置按钮图标
		HICON icon=AfxGetApp( ) -> LoadIcon( m_strResID );
		if(icon) SetIcon( icon ); 
	}
	if( !enableTip ) return TRUE;
	//需要提示控件，创建提示控件。只有图标按钮才需要提示文字
	if( !m_toolTip.Create( this ) ) return FALSE;
	CString m_str;
	GetWindowText(m_str);
	//提示内容为按钮标题
	m_toolTip.AddTool( this, m_str );
	m_toolTip.Activate(TRUE);	
	return TRUE;
}

//绘制按钮，只有菜单按钮才需要该函数
//图标按钮不是自画方式，使用系统函数进行绘制
void CIconBut::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	//CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
	if( ( GetStyle( ) & BS_OWNERDRAW ) == BS_OWNERDRAW )
	{  //该控件为自画方式
		//if(m_bStayDown&&m_bLockDown) return;	  
	   CRect rect(lpDrawItemStruct->rcItem);
	   CDC* pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	   UINT state=lpDrawItemStruct->itemState;
	   UINT action=lpDrawItemStruct->itemAction;
	   //得到系统的按钮各部分颜色
	   HBRUSH hbrText=GetSysColorBrush(COLOR_BTNTEXT);
	   HBRUSH hbrFace=GetSysColorBrush(COLOR_3DFACE);
	   HBRUSH hOldBrush=(HBRUSH)SelectObject(pDC->m_hDC,hbrFace);
       //绘制边界
	   CRect edgeRect(rect);
	   pDC->Rectangle(rect);
	   if(state&ODS_FOCUS)
		   edgeRect.InflateRect(-1,-1);
	   //绘制按钮体
	   /*if((state&ODS_SELECTED)&&(action&ODA_SELECT)&&(state&ODS_FOCUS))
	   { //按钮被选中
		 CPen pen(PS_SOLID,1,GetSysColor(COLOR_3DSHADOW));		 
		 CPen* pOldPen=pDC->SelectObject(&pen);		 		 
		 pDC->Rectangle(edgeRect);
		 pDC->SelectObject(pOldPen);
	   }
	   else*/
	   	 pDC->DrawEdge(edgeRect,EDGE_RAISED,BF_RECT|BF_SOFT);
	     //m_bStayDown=FALSE; }
	   //绘制按钮文字标题
	   CSize szTri=pDC->GetOutputTextExtent(_T("M"),1);
	   CString text;
	   GetWindowText(text);
	   CSize szText=pDC->GetOutputTextExtent(text,text.GetLength());
	   CSize szTotal(szText);
	   szTotal.cx+=szTri.cx;
	   CRect textRect=rect;
	   textRect.InflateRect(-((rect.Width()-szTotal.cx)/2),
		   -((rect.Height()-szTotal.cy)/2));
	   textRect.top--;
	   CRect triRect=textRect;
	   triRect.left=textRect.right-szTri.cx;
	   triRect.InflateRect(0,-((textRect.Height()-szTri.cy)/2));
	   pDC->SetBkMode(TRANSPARENT);
	   if(state&ODS_SELECTED)
		   textRect.OffsetRect(1,1);
	   pDC->DrawText(text,textRect,DT_SINGLELINE|DT_LEFT);
	   SelectObject(pDC->m_hDC,hbrText);
	   CPoint ptTri(textRect.left+szText.cx,textRect.top+3);
	   DrawTriangle(pDC,ptTri,10);
	   if(state&ODS_FOCUS)
	   {
		   CRect fRect(rect);
		   fRect.InflateRect(-4,-4);
		   pDC->DrawFocusRect(fRect);
	   }
	   SelectObject(pDC->m_hDC,hOldBrush);
	   if((action&ODA_SELECT)&&
		 (state&ODS_FOCUS)&&(state&ODS_SELECTED))
	        DisplayMenu();
	}
	
}
//显示弹出菜单
void CIconBut::DisplayMenu()
{   
   CMenu menu;
   //UINT nID=m_nID;
   if( !menu.LoadMenu(m_strResID) ) return;
   CMenu* m_pMenu=menu.GetSubMenu(0);
   if(m_pMenu==NULL) return;   
   CRect rect;
   GetWindowRect(&rect);
   m_bLockDown=TRUE;
   m_pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,
	   rect.left,rect.bottom,GetParent(),rect);
   //SendMessage(WM_LBUTTONUP,0,MAKELPARAM(1,1));
   m_bLockDown=FALSE;
   //if( m_bStayDown ) Invalidate(FALSE);
}

//绘制菜单三角形
void CIconBut::DrawTriangle(CDC *pDC, CPoint &pt, int size)
{  POINT tri[3]={pt.x,pt.y,pt.x+size,pt.y,
	             pt.x+size/2,pt.y+size/2};
   pDC->Polygon(tri,3);

}


//控件被创建时自动调用，创建内部数据
void CIconBut::PreSubclassWindow()
 {
 	Create(( GetStyle( ) & BS_OWNERDRAW ) != BS_OWNERDRAW);
	CButton::PreSubclassWindow(); 	
 }

BOOL CIconBut::Create(CWnd *pWnd, LPCTSTR pCaption,const RECT &rect, UINT nID)
{
	DWORD nFlag=WS_CHILD|WS_VISIBLE;
	if(m_bPopup) nFlag|=BS_OWNERDRAW;
	else         nFlag|=BS_ICON;
	if(!CButton::Create(pCaption,nFlag,rect,pWnd,nID)) return FALSE;
	return Create(TRUE);
}



//DEL void CIconBut::OnLButtonDown(UINT nFlags, CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	
//DEL 	CButton::OnLButtonDown(nFlags, point);
//DEL 	DisplayMenu();
//DEL }

void CIconBut::OnDestroy() 
{
	CButton::OnDestroy();
	
	// TODO: Add your message handler code here
	m_toolTip.DestroyWindow();
}
