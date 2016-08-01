// SetDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SpiritTool.h"
#include "SetDialog.h"
#include "afxdialogex.h"
#include "FolderDialog.h"
#include "StaticUtil.h"
#include "global.h"

// CSetDialog 对话框


IMPLEMENT_DYNAMIC(CSetDialog, CDialogEx)

CSetDialog::CSetDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetDialog::IDD, pParent)
{

	m_iSrcPath = _T("");
	m_iSdkPath = _T("");
	//  m_iKeyDate = _T("");
	m_nKeyDate = 1000;
	readConfig();
}

CSetDialog::~CSetDialog()
{
}

void CSetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITSRCPATH, m_iSrcPath);
	DDV_MaxChars(pDX, m_iSrcPath, 256);
	DDX_Text(pDX, IDC_EDITSDKPATH, m_iSdkPath);
	DDV_MaxChars(pDX, m_iSdkPath, 256);
	//  DDX_Text(pDX, IDC_EDITKEYDATE, m_iKeyDate);
	//  DDV_MaxChars(pDX, m_iKeyDate, 20);
	DDX_Text(pDX, IDC_EDITKEYDATE, m_nKeyDate);
	DDV_MinMaxLong(pDX, m_nKeyDate, 0, 100000);
}


BEGIN_MESSAGE_MAP(CSetDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSetDialog::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CSetDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetDialog 消息处理程序


void CSetDialog::setSrcPath(const CString& srcpath)
{
	m_iSrcPath = srcpath;
	UpdateData(false);
}

const CString CSetDialog::getSrcPath()
{
	return m_iSrcPath;
}

void CSetDialog::setSdkPath(const CString& sdkpath)
{
	m_iSdkPath = sdkpath;
	UpdateData(false);
}

const CString CSetDialog::getSdkPath()
{
	return m_iSdkPath;
}

bool CSetDialog::makeFile()
{
	FILE *fl;
	fl = fopen(MakeKeyFile,"w");
	if(fl == NULL) return false;
	int len;
	len = ::WideCharToMultiByte(CP_ACP,0,m_iSrcPath.GetBuffer(),-1,NULL,0,NULL,NULL);
	char* tempsrcpath = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iSrcPath.GetBuffer(),-1,tempsrcpath,len,NULL,NULL);
	fprintf(fl,"@echo off\r\n");
	fprintf(fl,"keytool -genkey -keystore %s\\bin\\spirit.keystore -keyalg RSA -validity %d -storepass spirit001 -alias test -keypass spirit007\r\n",tempsrcpath,m_nKeyDate);
	fprintf(fl,"@echo on\r\n");
	delete tempsrcpath;
	fclose(fl);
	return true;
}

bool CSetDialog::makeConfig()
{
	FILE *fl;
	fl = ::fopen(ConfigFile,"w");
	if(fl == NULL) return false;
	char* temp;
	int len;
	len = ::WideCharToMultiByte(CP_ACP,0,m_iSrcPath.GetBuffer(),-1,NULL,0,NULL,NULL);
	temp = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iSrcPath.GetBuffer(),-1,temp,len,NULL,NULL);
	::fprintf(fl,"srcpath = \"%s\"\r\n",temp);
	delete temp;
	len = ::WideCharToMultiByte(CP_ACP,0,m_iSdkPath.GetBuffer(),-1,NULL,0,NULL,NULL);
	temp = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iSdkPath.GetBuffer(),-1,temp,len,NULL,NULL);
	::fprintf(fl,"sdkpath = \"%s\"\r\n",temp);
	::fprintf(fl,"keydate = \"%d\"\r\n",m_nKeyDate);
	delete temp;

	fclose(fl);
	return true;
}

void CSetDialog::readConfig()
{
	FILE *fl;
	fl = fopen(ConfigFile,"rb");
	if(fl == NULL) return;
	char *buf;		//文件数据buf
	long flen;		//文件长度
	::fseek(fl,0,SEEK_END);
	flen = ftell(fl);
	fseek(fl,0,SEEK_SET);
	buf = new char[flen];
	fread(buf,1,flen,fl);
	char* temp = buf;
	int len = CStaticUtil::findstr(temp,"srcpath = ");
	temp+=len;
	len = CStaticUtil::findstr(temp,"\"");
	temp+=len+1;
	len = CStaticUtil::findstr(temp,"\"");
	m_iSrcPath = CString(temp,len);
	temp = buf;
	len = CStaticUtil::findstr(temp,"sdkpath = ");
	temp+=len;
	len = CStaticUtil::findstr(temp,"\"");
	temp+=len+1;
	len = CStaticUtil::findstr(temp,"\"");
	m_iSdkPath = CString(temp,len);

	delete buf;
	fclose(fl);
}


void CSetDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	if(CFolderDialog::GetFolder(path))
	{
		m_iSrcPath = path;
		UpdateData(false);
	}
}


void CSetDialog::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	if(CFolderDialog::GetFolder(path))
	{
		m_iSdkPath = path;
		UpdateData(false);
	}
}


void CSetDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	makeConfig();
	bool b = makeFile();
	if(b)
	{
		char keypath[256];
		memset(keypath,0,256);
		int len = ::WideCharToMultiByte(CP_ACP,0,m_iSrcPath.GetBuffer(),-1,NULL,0,NULL,NULL);
		::WideCharToMultiByte(CP_ACP,0,m_iSrcPath.GetBuffer(),-1,keypath,len,NULL,NULL);
		memcpy(keypath+len-1,"\\bin\\spirit.keystore",strlen("\\bin\\spirit.keystore"));
		CStaticUtil::removefile(keypath);
		int stl = ::system(MakeKeyFile);
	}
	CDialogEx::OnOK();
}
