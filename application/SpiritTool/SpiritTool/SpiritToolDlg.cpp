
// SpiritToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SpiritTool.h"
#include "SpiritToolDlg.h"
#include "afxdialogex.h"
#include "FolderDialog.h"
#include "ApkMake.h"
#include "message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSpiritToolDlg 对话框




CSpiritToolDlg::CSpiritToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpiritToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iAppname = _T("");
	m_iOutpath = _T("");
	m_iIconpath = _T("");
	m_iApkname = _T("");
	m_iPackagename = _T("");
	m_iSpiritUA = _T("");
	m_iChannelID = _T("");
	m_iImgPath1 = _T("");
	m_iImgPath2 = _T("");
	m_iGifPath = _T("");
	m_iBWPath = _T("");
}
CSpiritToolDlg::~CSpiritToolDlg()
{
	CApkMake *apkMake = CApkMake::Singleton();
	delete apkMake;
}

void CSpiritToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_iList);
	DDX_Text(pDX, IDC_EDITAPPNAME, m_iAppname);
	DDV_MaxChars(pDX, m_iAppname, 20);
	DDX_Text(pDX, IDC_EDITOUTPATH, m_iOutpath);
	DDV_MaxChars(pDX, m_iOutpath, 256);
	DDX_Text(pDX, IDC_EDITICONPATH, m_iIconpath);
	DDV_MaxChars(pDX, m_iIconpath, 256);
	DDX_Text(pDX, IDC_EDITAPKNAME, m_iApkname);
	DDV_MaxChars(pDX, m_iApkname, 128);
	DDX_Text(pDX, IDC_EDITPACKAGENAME, m_iPackagename);
	DDV_MaxChars(pDX, m_iPackagename, 128);
	DDX_Text(pDX, IDC_EDITSPIRITUA, m_iSpiritUA);
	DDV_MaxChars(pDX, m_iSpiritUA, 20);
	DDX_Text(pDX, IDC_EDITCHANNELID, m_iChannelID);
	DDV_MaxChars(pDX, m_iChannelID, 8);
	DDX_Text(pDX, IDC_EDITIMAGEPATH1, m_iImgPath1);
	DDV_MaxChars(pDX, m_iImgPath1, 256);
	DDX_Text(pDX, IDC_EDITIMAGEPATH2, m_iImgPath2);
	DDV_MaxChars(pDX, m_iImgPath2, 256);
	DDX_Text(pDX, IDC_EDITGIFPATH, m_iGifPath);
	DDV_MaxChars(pDX, m_iGifPath, 256);
	DDX_Text(pDX, IDC_EDITBWPATH, m_iBWPath);
	DDV_MaxChars(pDX, m_iBWPath, 256);
	DDX_Check(pDX, IDC_CHECK1, m_bIsDesk);
	DDX_Control(pDX, IDC_EDIT1, m_iShow);
}

BEGIN_MESSAGE_MAP(CSpiritToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CSpiritToolDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTONSET, &CSpiritToolDlg::OnBnClickedButtonset)
	ON_BN_CLICKED(IDC_BUTTONCH1, &CSpiritToolDlg::OnBnClickedButtonch1)
	ON_BN_CLICKED(IDC_BUTTONCH2, &CSpiritToolDlg::OnBnClickedButtonch2)
	ON_BN_CLICKED(IDC_BUTTONCH3, &CSpiritToolDlg::OnBnClickedButtonch3)
	ON_BN_CLICKED(IDC_BUTTONCH4, &CSpiritToolDlg::OnBnClickedButtonch4)
	ON_BN_CLICKED(IDC_BUTTONCH5, &CSpiritToolDlg::OnBnClickedButtonch5)
	ON_BN_CLICKED(IDC_BUTTONCH6, &CSpiritToolDlg::OnBnClickedButtonch6)
	ON_BN_CLICKED(IDC_BUTTONADD, &CSpiritToolDlg::OnBnClickedButtonadd)
	ON_LBN_SELCHANGE(IDC_LIST1, &CSpiritToolDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDOK, &CSpiritToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CSpiritToolDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSpiritToolDlg 消息处理程序

BOOL CSpiritToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

//	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	CApkMake *apkMake = CApkMake::Singleton();
	apkMake->setDataArray(&m_spiritList);
	apkMake->start();
	apkMake->setListener(this);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSpiritToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSpiritToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSpiritToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSpiritToolDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

/*
void CSpiritToolDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPath;
//	CFileDialog Open(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Bitmap Files (*.bmp)|*.bmp|JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||",NULL);
	CFileDialog Open(true);
	if(Open.DoModal()==IDOK)
	{
		strPath=Open.GetPathName();//这就是你要的路径了。
	}
}
*/

void CSpiritToolDlg::OnBnClickedButtonset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iSetDialog.DoModal();			//启动设置对话框
}


void CSpiritToolDlg::OnBnClickedButtonch1()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	CFileDialog Open(true);
	if(Open.DoModal()==IDOK)
	{
		m_iOutpath=Open.GetPathName();//这就是你要的路径了。
		UpdateData(false);
	}
	*/
	/*
	CString path;
	CFolderDialog open(&path,this);
	if(open.DoModal() == IDOK)
	{
		m_iOutpath=open.GetPathName();//这就是你要的路径了。
		UpdateData(false);
	}
	*/
	UpdateData();
	CString path;
	if(CFolderDialog::GetFolder(path))
	{
		m_iOutpath = path;
		UpdateData(false);
	}
}

void CSpiritToolDlg::OnBnClickedButtonch2()
{
	// TODO: 在此添加控件通知处理程序代码
//	CFileDialog Open(true);
	UpdateData();
	CFileDialog Open(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"PNG File (*.png)|*.png||",
		NULL,
		0UL,
		1);
	if(Open.DoModal()==IDOK)
	{
		m_iIconpath=Open.GetPathName();
		UpdateData(false);
	}
}


void CSpiritToolDlg::OnBnClickedButtonch3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CFileDialog Open(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"PNG File (*.png)|*.png||",
		NULL,
		0UL,
		1);
	if(Open.DoModal()==IDOK)
	{
		m_iImgPath1=Open.GetPathName();
		UpdateData(false);
	}
}


void CSpiritToolDlg::OnBnClickedButtonch4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CFileDialog Open(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"PNG File (*.png)|*.png||",
		NULL,
		0UL,
		1);
	if(Open.DoModal()==IDOK)
	{
		m_iImgPath2=Open.GetPathName();
		UpdateData(false);
	}
}


void CSpiritToolDlg::OnBnClickedButtonch5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CFileDialog Open(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"GIF File (*.gif)|*.gif||",
		NULL,
		0UL,
		1);
	if(Open.DoModal()==IDOK)
	{
		m_iGifPath=Open.GetPathName();
		UpdateData(false);
	}
}


void CSpiritToolDlg::OnBnClickedButtonch6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CFileDialog Open(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"APK File (*.apk)|*.apk||",
		NULL,
		0UL,
		1);
	if(Open.DoModal()==IDOK)
	{
		m_iBWPath=Open.GetPathName();
		UpdateData(false);
	}
}


void CSpiritToolDlg::OnBnClickedButtonadd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	CSpiritData *Item = new CSpiritData();

	char *tempI;
	int len;
	//app名字
	if(m_iAppname.GetLength()<=0)
	{
		CString str(L"请填写app名称");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iAppname.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iAppname.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hAppname = tempI;

	//输出路径
	if(m_iOutpath.GetLength()<=0)
	{
		CString str(L"请填写输出路径");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iOutpath.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iOutpath.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hOutpath = tempI;

	//图标路径
	if(m_iIconpath.GetLength()<=0)
	{
		CString str(L"请填写图标路径");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iIconpath.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iIconpath.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hIconpath = tempI;

	//程序包名
	if(m_iApkname.GetLength()<=0)
	{
		CString str(L"请填写程序包名");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iApkname.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iApkname.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hApkname = tempI;

	//标识名
	if(m_iPackagename.GetLength()<=0)
	{
		CString str(L"请填写程序标识名");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iPackagename.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iPackagename.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hPackagename = tempI;

	//精灵UA
	if(m_iSpiritUA.GetLength()<=0)
	{
		CString str(L"请填写精灵UA");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iSpiritUA.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iSpiritUA.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hSpiritUA = tempI;

	//精灵渠道号
	if(m_iChannelID.GetLength()<=0)
	{
		CString str(L"请填写精灵渠道号");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iChannelID.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iChannelID.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hChannelId = tempI;

	//图片1路径
	if(m_iImgPath1.GetLength()<=0)
	{
		CString str(L"请添加图片1");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iImgPath1.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iImgPath1.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hImagepath1 = tempI;

	//图片2路径
	if(m_iImgPath2.GetLength()<=0)
	{
		CString str(L"请添加图片2");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iImgPath2.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iImgPath2.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hImagepath2 = tempI;

	//gif图片路径
	if(m_iGifPath.GetLength()<=0)
	{
		CString str(L"请添加gif图片");
		m_iShow.SetWindowTextW(str);
		delete Item;
		return;
	}
	len = ::WideCharToMultiByte(CP_ACP,0,m_iGifPath.GetBuffer(),-1,NULL,0,NULL,NULL);
	tempI = new char[len];
	::WideCharToMultiByte(CP_ACP,0,m_iGifPath.GetBuffer(),-1,tempI,len,NULL,NULL);
	Item->m_hGifpath = tempI;

	//百玩包路径
	if(m_iBWPath.GetLength()>0)
	{
		len = ::WideCharToMultiByte(CP_ACP,0,m_iBWPath.GetBuffer(),-1,NULL,0,NULL,NULL);
		tempI = new char[len];
		::WideCharToMultiByte(CP_ACP,0,m_iBWPath.GetBuffer(),-1,tempI,len,NULL,NULL);
		Item->m_hBWpath = tempI;
	}

	m_spiritList.addData(Item);
	m_iList.AddString(m_iAppname);
	CString text = m_iAppname;
	text+=L"-添加成功";
	m_iShow.SetWindowTextW(text);
}


void CSpiritToolDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_iList.GetCurSel();
	if(index == LB_ERR) return;
	CSpiritData *item = m_spiritList.getData(index);
	m_iAppname = CString(item->m_hAppname);
	m_iOutpath = CString(item->m_hOutpath);
	m_iIconpath = CString(item->m_hIconpath);
	m_iApkname = CString(item->m_hApkname);
	m_iPackagename = CString(item->m_hPackagename);
	m_iSpiritUA = CString(item->m_hSpiritUA);
	m_iChannelID = CString(item->m_hChannelId);
	m_iImgPath1 = CString(item->m_hImagepath1);
	m_iImgPath2 = CString(item->m_hImagepath2);
	m_iGifPath = CString(item->m_hGifpath);
	m_iBWPath = CString(item->m_hBWpath);
	UpdateData(false);
	CString text;
	text+=L"App名：";
	text+=m_iAppname;
	text+=L"\r\n";
	text+=L"UA：";
	text+=m_iSpiritUA;
	text+=L"\r\n";
	text+=L"渠道号：";
	text+=m_iChannelID;
	m_iShow.SetWindowTextW(text);
}


void CSpiritToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_iSetDialog.m_iSdkPath.GetLength()<=0)
	{
		m_iShow.SetWindowTextW(L"请设置androidSdk路径.");
		return;
	}
	if(m_iSetDialog.m_iSrcPath.GetLength()<=0)
	{
		m_iShow.SetWindowTextW(L"请设置源码路径.");
		return;
	}
	CApkMake *apkMake = CApkMake::Singleton();
	char src[256];
	char sdk[256];
	int len = ::WideCharToMultiByte(CP_ACP,0,m_iSetDialog.m_iSrcPath.GetBuffer(),-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,m_iSetDialog.m_iSrcPath.GetBuffer(),-1,src,len,NULL,NULL);
	src[len] = 0;
	len = ::WideCharToMultiByte(CP_ACP,0,m_iSetDialog.m_iSdkPath.GetBuffer(),-1,NULL,0,NULL,NULL);
	::WideCharToMultiByte(CP_ACP,0,m_iSetDialog.m_iSdkPath.GetBuffer(),-1,sdk,len,NULL,NULL);
	sdk[len] = 0;
	apkMake->setPath(src,sdk);
	apkMake->startMake();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTONADD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTONSET)->EnableWindow(false);
	return;
//	CDialogEx::OnOK();
}


void CSpiritToolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_iList.GetCurSel();
	if(index == LB_ERR) return;
	m_iList.DeleteString(index);
//	delete m_iList.GetItemDataPtr(index);
	delete m_spiritList.removeData(index);
	 m_iAppname = L"";				//app名称
	 m_iOutpath = L"";				//输出路径
	 m_iIconpath = L"";			//图标路径
	 m_iApkname = L"";				//程序包名
	 m_iPackagename = L"";			//程序标识，主类包名
	 m_iSpiritUA = L"";			//精灵UA
	 m_iChannelID = L"";			//精灵渠道号
	 m_iImgPath1 = L"";			//图片1路径
	 m_iImgPath2 = L"";			//图片2路径
	 m_iGifPath = L"";				//gif图片路径
	 m_iBWPath = L"";				//百玩包图片路径
	UpdateData(false);
}

void CSpiritToolDlg::onListen(int type,unsigned long data)
{
	switch(type)
	{
	case Kcomplete:
		{
			int index = data-1;
			int len = m_iShow.GetWindowTextLengthW();
			wchar_t *str = new wchar_t[len+1];
			m_iShow.GetWindowTextW(str,len);
			CString buf(str);
			buf+=L"\r\n";
			buf+=m_spiritList.getData(index)->m_hAppname;
			buf+=L"---已完成";
			m_iShow.SetWindowTextW(buf);
			UpdateData(false);
			delete str;
		}
		break;
	case Kover:
		{
			int index = data-1;
			int len = m_iShow.GetWindowTextLengthW();
			wchar_t *str = new wchar_t[len+1];
			m_iShow.GetWindowTextW(str,len);
			CString buf(str);
			buf+=L"\r\n";
			buf+=L"全部完成";
			m_iShow.SetWindowTextW(buf);
			UpdateData(false);
			delete str;
			GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
			GetDlgItem(IDC_BUTTONADD)->EnableWindow(true);
			GetDlgItem(IDC_BUTTONSET)->EnableWindow(true);
		}
		break;
	}
}
