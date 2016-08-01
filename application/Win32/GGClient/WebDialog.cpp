// WebDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "WebDialog.h"
#include "XSystem.h"


// CWebDialog 对话框

IMPLEMENT_DYNAMIC(CWebDialog, CDialog)

CWebDialog::CWebDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWebDialog::IDD, pParent)
{

}

CWebDialog::~CWebDialog()
{
}

void CWebDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEBCNT, m_webCnt);
}

BOOL CWebDialog::OnInitDialog() 
{
	return CDialog::OnInitDialog();
}
BEGIN_MESSAGE_MAP(CWebDialog, CDialog)
END_MESSAGE_MAP()

void CWebDialog::ShowCnt()
{
	//m_webCnt.Navigate(_T("http://bwtest.jqbar.com/product/AcceptData.aspx?val=9B69207BB65F9D7DBD543955481BD118E939837F602369C2CDEC257141C64A3FA12C38A330655D732F80CED05FA53F3116658F543088EAC3D543A22F1A4A25F373EB4932D6EB7E414AB80F3DF738DE5C01B841691E790D6D28F3DB5E2684268049D764933CA2CC1A8764A5FF0BC198F3329239D2D6D1F699E6479E496A1BD4B0&skipId=2"),NULL,NULL,NULL,NULL);
	//return;
	if(XSystem::m_strURL.IsEmpty()) return;
	//void Navigate(LPCTSTR URL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers)
	TRY
	{
		//XString16 url16(XSystem::m_strURL);
		VARIANT url;
		url.vt = VT_BSTR;
		url.bstrVal = (BSTR)XSystem::m_strURL.GetData();
		VARIANT cnt;
		cnt.vt = VT_BSTR;
		//XString16 post(XSystem::m_strPostData);
		cnt.bstrVal = (BSTR)XSystem::m_strPostData.GetData();//post.GetData();
		VARIANT headers;
		cnt.vt = VT_BSTR;
		XString16 head(XSystem::m_strHeaders);
		headers.bstrVal=(BSTR)XSystem::m_strHeaders.GetData();
		m_webCnt.Navigate2(&url,NULL,NULL,XSystem::m_strPostData.IsEmpty()?NULL:&cnt,
			XSystem::m_strHeaders.IsEmpty()?NULL:&headers);
		
	}
	CATCH_ALL(e)
	{
	}
	END_CATCH_ALL
}

// CWebDialog 消息处理程序
