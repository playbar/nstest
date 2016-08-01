// DomDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "DomDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDomDialog dialog


CDomDialog::CDomDialog(XDomItem*p,CWnd* pParent /*=NULL*/)
	: CDialog(CDomDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDomDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc=p;
}


void CDomDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDomDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDomDialog, CDialog)
	//{{AFX_MSG_MAP(CDomDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDomDialog message handlers

BOOL CDomDialog::OnInitDialog()
{
	if(m_pDoc!=NULL)
	{

	}
	return TRUE;

}
