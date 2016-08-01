// AS3AutoMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AS3AutoMap.h"
#include "AS3AutoMapDlg.h"
#include "DirBrowns.h"
#include "Register.h"
#include "TextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAS3AutoMapDlg dialog

CAS3AutoMapDlg::CAS3AutoMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAS3AutoMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAS3AutoMapDlg)
	m_strAS3Header = _T("TotalInclude.h");
	m_strOutputPath = _T("AVMSWF");
	m_strFile = _T("swfapi");
	m_strNamespace = _T("avmshell");
	m_strPreHeader = _T("stdafxflash.h");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_pClassInfo=NULL;
	m_pTail=NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAS3AutoMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAS3AutoMapDlg)
	DDX_Control(pDX, IDOK, m_okBut);
	DDX_Control(pDX, IDC_CLASSTREE, m_tree);
	DDX_Text(pDX, IDC_AS3HEADER, m_strAS3Header);
	DDX_Text(pDX, IDC_OUTPUT, m_strOutputPath);
	DDX_Text(pDX, IDC_OUTPUTFILE, m_strFile);
	DDX_Text(pDX, IDC_NAMESPACE, m_strNamespace);
	DDX_Text(pDX, IDC_PREHEADER, m_strPreHeader);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAS3AutoMapDlg, CDialog)
	//{{AFX_MSG_MAP(CAS3AutoMapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_OUTPUTFILE, OnChangeOutputfile)
	ON_BN_CLICKED(IDC_SETAS3, OnSetas3)
	ON_BN_CLICKED(IDC_SETOUT, OnSetout)
	ON_NOTIFY(NM_CLICK, IDC_CLASSTREE, OnClickClasstree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_CLASSTREE, OnSelchangedClasstree)
	ON_BN_CLICKED(IDC_ENABLE, OnEnable)
	ON_BN_CLICKED(IDC_AUTOTEXT, OnAutotext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAS3AutoMapDlg message handlers

BOOL CAS3AutoMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_images.Create(IDB_ICONS, 16, 1, RGB(255, 0, 0));
	m_tree.SetImageList(&m_images,TVSIL_NORMAL);
	//m_objs.Create(IDB_OBJS,16,1,RGB(255,255,255));

	Init(TRUE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAS3AutoMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAS3AutoMapDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAS3AutoMapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAS3AutoMapDlg::Init(BOOL bUpdate)
{
	BOOL bOK=TRUE;
	if(m_strFile.IsEmpty())
	{
		//m_okBut.EnableWindow(FALSE);
		bOK=FALSE;
		SetDlgItemText(IDC_INFO,"未指定文件名!");
	}
	else
	{
		//m_okBut.EnableWindow(TRUE);
		CString strTmp=m_strFile;
		strTmp+=".h,";
		strTmp+=m_strFile;
		strTmp+=".cpp";
		SetDlgItemText(IDC_INFO,strTmp);
		bOK=TRUE;
	}
	if(bUpdate) //return;
	{
		CString strHeader;
		if(!LoadFile(m_strAS3Header,strHeader))
		{
			bOK=FALSE;
		}
		else
		{
			if(!ParseHeader(strHeader))
				bOK=FALSE;
		}
		if(bOK)
			InitTree();
	}
	m_okBut.EnableWindow(bOK);
	GetDlgItem(IDC_ENABLE)->EnableWindow(FALSE);
}

void CAS3AutoMapDlg::OnChangeOutputfile() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_tree.GetSafeHwnd()==NULL) return;
	GetDlgItemText(IDC_OUTPUTFILE,m_strFile);
	Init(FALSE);
}

void CAS3AutoMapDlg::OnSetas3() 
{
	// TODO: Add your control notification handler code here
   CFileDialog dlg( TRUE, _T("h"), m_strAS3Header, 
                    OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
					OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST,"头文件|*.h|");
   if( dlg . DoModal(  ) != IDOK ) return;
   m_strAS3Header=dlg.GetPathName();
   SetDlgItemText(IDC_AS3HEADER,m_strAS3Header);
   Init(TRUE);
}

void CAS3AutoMapDlg::OnSetout() 
{
	// TODO: Add your control notification handler code here
	CDirBrowns dlg;
	dlg.m_pathName=m_strOutputPath;
	if(!dlg.DoModal()) return;
	m_strOutputPath=dlg.m_pathName;
	SetDlgItemText(IDC_OUTPUT,m_strOutputPath);	
}

BOOL CAS3AutoMapDlg::LoadFile(const char *fileName, CString &txt)
{
	CFile file;
	if(!file.Open(fileName,CFile::modeRead)) return FALSE;
	int l=file.GetLength();
	file.Read(txt.GetBuffer(l+1),l);
	file.Close();
	txt.ReleaseBuffer();
	//txt+=0;
	//((char*)((const char*)txt)[l])=0;
	return TRUE;
}

BOOL CAS3AutoMapDlg::SaveFile(const char *fileName, CString &txt)
{
	CFile file;
	if(!file.Open(fileName,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate)) return FALSE;
	file.Write(txt,txt.GetLength());
	file.Close();
	return TRUE;
}



BOOL CAS3AutoMapDlg::ParseHeader(CString &strTxt)
{
	ReleaseInfo();
	CStringArray list;
	int id=0;
	m_strName.Empty();

	CString strClassList;
	CString strGetClassList;

	while(true)
	{
		int ie=strTxt.Find("\n",id);
		CString strTmp;
		if(ie>id)
			strTmp=strTxt.Mid(id,ie-id);
		else
			strTmp=strTxt.Mid(id);
		strTmp.TrimLeft();
		strTmp.TrimRight();
		int is=strTmp.Find('=');
		if(is<=0)
		{
		   if(m_strName.IsEmpty())
		   {
			   is=strTmp.ReverseFind(' ')+1;
			   int it=strTmp.Find('_',is);
			   if(is>0&&it>is)
				   m_strName=strTmp.Mid(is,it-is);
		   }
		}
		else
		{
			strTmp.Delete(is,strTmp.GetLength()-is);
			strTmp.TrimRight();
			is=strTmp.ReverseFind(' ')+1;
			if(is>0)
			{
				CString str=strTmp.Mid(is,strTmp.GetLength()-is);
				list.Add(str);
			}
				//list.Add(strTmp.Mid(is,strTmp.GetLen
		}
		if(ie<=id) break;
		id=ie+1;
	}

	int nCount=list.GetSize();
	if(nCount<=0) return FALSE;

	CRegister reg("Datas\\Classes");

	int i = 0;
	for( i=0;i<nCount;i++)
	{	//classes
		CString strTmp=list[i];
		int id=strTmp.Find('_');
		if(id<=0) continue;
		CString strKey=strTmp.Left(id);
		strKey.MakeLower();
		if(strKey=="abcclass")
		{ //class
		   ClassInfo*pInfo=new ClassInfo;
		   pInfo->pNext=NULL;
		   pInfo->strID=strTmp;
		   strTmp.Delete(0,id+1);
		   
		   if(m_pTail==NULL)
		   {
			   m_pClassInfo=pInfo;
			   m_pTail=pInfo;
		   }
		   else
		   {
			   m_pTail->pNext=pInfo;
			   m_pTail=pInfo;
		   }

		   //pInfo->pNext=m_pClassInfo;
		   pInfo->pInfo=NULL;
		   pInfo->pTail=NULL;
		   
		   int ie=strTmp.ReverseFind('_');
		   pInfo->strName=strTmp.Mid(ie+1);
		   pInfo->bInstance=reg.Read(pInfo->strName,TRUE);
		   strTmp.Delete(ie,strTmp.GetLength()-ie);
		   
		   id=0;
		   while(TRUE)
		   {
			   int ie=strTmp.Find('_',id);
			   if(ie<=0) 
			   {
				   pInfo->strPackages.Add((const char*)strTmp+id);
				   break;
			   }
			   else 
				   pInfo->strPackages.Add(strTmp.Mid(id,ie-id));
			   id=ie+1;
		   }
		}
	}

	for( i=0;i<nCount;i++)
	{	//Functions
		CString strTmp=list[i];
		//if(strTmp=="flash_events_MouseEvent_altKey_get")
		//	int v=0;
		int id=strTmp.Find('_');
		if(id<=0) continue;
		CString strKey=strTmp.Left(id);
		strKey.MakeLower();
		//CString strNameS;
		if(strKey!="abcclass")
		{ //Function
		  ClassInfo*pInfo=NULL;
		  int im=strTmp.ReverseFind('_');
		  if(im>0)
		  {
			  CString strExt=strTmp.Mid(im);
			  CString strName;
			  if(strExt=="_get"||strExt=="_set")
			  {
			     CString strs=strTmp.Left(im);
				 im=strs.ReverseFind('_');
				 strName=strs.Left(im);
			  }
			  	//strName=strTmp.Left(im);
			  else
				strName=strTmp.Left(im);
			  
			  
			  im=strName.ReverseFind('_');
			  
			  CString strNameE=im>=0?strName.Mid(im+1):strName;
			  im=im>=0?(im+strNameE.GetLength()+2):strNameE.GetLength()+1;
			  //strNameS=strNameE;
			  ClassInfo*p=m_pClassInfo;
			  while(p)
			  {
				  if(p->strName==strNameE)
				  {
					  pInfo=p;
					  break;
				  }
				  p=p->pNext;
			  }
		  }

		  /*ClassInfo*pInfo=m_pClassInfo,*pBest=NULL;
		  int im=0;
		  while(TRUE)
		  {
			  CString strs=pInfo->strName;
			  //strs+="_";
			  int imm=strTmp.Find(strs);
			  if(imm>=0)
			  {
				  BOOL bOK=TRUE;
				  if(imm>0&&strTmp[imm-1]!='_')
					  bOK=FALSE;
				  int ie=imm+strs.GetLength();
				  if(ie<strTmp.GetLength()&&strTmp[ie]!='_')
					  bOK=FALSE;
				  if(bOK&&im<=imm)
				  {
					  im=imm+strs.GetLength()+1;
					  pBest=pInfo;
				  }
				  //im+=pInfo->strName.GetLength()+1;
				  //break;
			  }
			  
			  pInfo=pInfo->pNext;
			  if(!pInfo) {pInfo=pBest;break;}
		  }*/
		  if(pInfo)
		  {
			  FunctionInfo*pFun=new FunctionInfo;
			  pFun->strID=strTmp;
			  pFun->strName=strTmp.Mid(im);
			  pFun->pNext=NULL;
			  if(pInfo->pTail==NULL)
			  {
				  pInfo->pTail=pFun;
				  pInfo->pInfo=pFun;
			  }
			  else
			  {
				  pInfo->pTail->pNext=pFun;
				  pInfo->pTail=pFun;
			  }
		  }
		  else
		  {
			  CString strMsg;
			  strMsg.Format("Error:%s",strTmp);
			  AfxMessageBox(strMsg);
		  }
		}
	}


    return !m_strName.IsEmpty();//TRUE;
}

void CAS3AutoMapDlg::ReleaseInfo()
{
	 if(m_pClassInfo==NULL) return;
	 CRegister reg("Datas\\Classes");
	 ClassInfo*pInfo=m_pClassInfo;
	 while(pInfo)
	 {
		 reg.Write(pInfo->strName,pInfo->bInstance);
		 ClassInfo*pNext=pInfo->pNext;
		 FunctionInfo*pf=pInfo->pInfo;
		 while(pf)
		 {
			 FunctionInfo*pfn=pf->pNext;
			 delete pf;
			 pf=pfn;
		 }
		 delete pInfo;
		 pInfo=pNext;
	 }
	 m_pClassInfo=NULL;
	 m_pTail=NULL;
}

CAS3AutoMapDlg::~CAS3AutoMapDlg()
{
	ReleaseInfo();
}

void CAS3AutoMapDlg::InitTree()
{
	m_tree.DeleteAllItems();
	ClassInfo*pInfo=m_pClassInfo;
	while(pInfo)
	{
		//if(pInfo->strName=="arguments")
		//	int v=0;
		HTREEITEM item=AddClass(pInfo);
		FunctionInfo*pFun=pInfo->pInfo;
		while(pFun)
		{
			m_tree.InsertItem(pFun->strName,2,2,item);
			pFun=pFun->pNext;
		}
		pInfo=pInfo->pNext;
		
	}
}

HTREEITEM CAS3AutoMapDlg::AddClass(ClassInfo *pInfo)
{
	HTREEITEM item=TVI_ROOT;
	for(int i=1;i<pInfo->strPackages.GetSize();i++)
	{
		CString& strPackage=pInfo->strPackages[i];

		HTREEITEM is=item;

		item=m_tree.GetNextItem(item,TVGN_CHILD);

		//if(item==TVI_ROOT)
		//	item=m_tree.GetFirstVisibleItem();
		//else
		//	item=m_tree.GetChildItem(item);
		while(item)
		{
			CString strName=m_tree.GetItemText(item);
			if(strName==strPackage) break;
			item=m_tree.GetNextItem(item,TVGN_NEXT);
		}
		if(item==NULL)
			item=m_tree.InsertItem(strPackage,0,0,item);
		//CTreeCtrl
		//m_tree.GetChildItem(
	}
	item=m_tree.InsertItem(pInfo->strName,1,1,item);
	m_tree.SetItemData(item,(DWORD)pInfo);
	return item;
}

void CAS3AutoMapDlg::OnClickClasstree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	/*HTREEITEM item=m_tree.GetSelectedItem();
	if(item)
	{
	   ClassInfo*pInfo=(ClassInfo*)m_tree.GetItemData(item);
	   if(pInfo)
	   {
		   GetDlgItem(IDC_ENABLE)->EnableWindow(TRUE);
		   //GetDlgItem(IDC_ENABLE)::CWnd::CheckDlgButton
		   CheckDlgButton(IDC_ENABLE,pInfo->bInstance);
	   }
	   else
		   GetDlgItem(IDC_ENABLE)->EnableWindow(FALSE);
	}*/
	*pResult = 0;
}

void CAS3AutoMapDlg::OnSelchangedClasstree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	TVITEMA p=pNMTreeView->itemNew;//m_tree.GetSelectedItem();
	if(p.hItem)
	{
	   ClassInfo*pInfo=(ClassInfo*)m_tree.GetItemData(p.hItem);
	   if(pInfo)
	   {
		   GetDlgItem(IDC_ENABLE)->EnableWindow(TRUE);
		   //GetDlgItem(IDC_ENABLE)::CWnd::CheckDlgButton
		   CheckDlgButton(IDC_ENABLE,pInfo->bInstance);
	   }
	   else
		   GetDlgItem(IDC_ENABLE)->EnableWindow(FALSE);
	}
	*pResult = 0;
}

void CAS3AutoMapDlg::OnEnable() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM item=m_tree.GetSelectedItem();
	if(!item) return;
	ClassInfo*pInfo=(ClassInfo*)m_tree.GetItemData(item);
	if(!pInfo) return;
	pInfo->bInstance=IsDlgButtonChecked(IDC_ENABLE);
}

void CAS3AutoMapDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CString strPath=m_strOutputPath,strTmp;
	CString strInclude=strPath+"\\Inc";
	CString strSrc=strPath+"\\src";
	{		
		::CreateDirectory(strSrc,NULL);
		::CreateDirectory(strInclude,NULL);
	}

	CString strTxtInc,strTxtCpp;
	strTxtInc.Format(
		"#ifndef _%s_\r\n#define _%s_\r\nnamespace avmplus { namespace NativeID {\r\n#include \"AS3/%s.h\"\r\n\r\nAVMTHUNK_DECLARE_NATIVE_INITIALIZER(%s)\r\n}}\r\n#endif",m_strFile,m_strFile,m_strName,m_strName);
	if(!m_strPreHeader.IsEmpty())
	{
		strTxtCpp.Format("#include \"%s\"\r\n",m_strPreHeader);		
	}
	strTxtCpp+="#include \"avmplus.h\"\r\n#include \"shellcore.h\"\r\n";

	{
		CString strInc,
				strSlot="class SlotOffsetsAndAsserts\r\n{\r\nprivate:\r\n\tstatic uint32_t getSlotOffset(Traits* t, int nameId);\r\npublic:\r\n\tstatic const uint16_t s_slotsOffsetScriptObject = 0;\r\n",
				strClassGlu="//Map Class(ProtoType) Table\r\n",
				strMethodMap,
				strClassMap;
		strMethodMap.Format("\t//Map Method Table\r\n\tAVMTHUNK_BEGIN_NATIVE_METHODS(%s)\r\n\r\n",m_strName);
		strClassMap.Format("\t//Map Class(Instance) Table\r\n\tAVMTHUNK_BEGIN_NATIVE_CLASSES(%s)\r\n\r\n",m_strName);


		ClassInfo*pInfo=m_pClassInfo;
		while(pInfo)
		{
			
			//Glu
			strTmp.Format("AVMTHUNK_NATIVE_CLASS_GLUE(%sClass,"
						  "::%s::%sClass, SlotOffsetsAndAsserts::do%sClassAsserts)\r\n",
						  pInfo->strName,m_strNamespace,pInfo->strName,pInfo->strName);
			strClassGlu+=strTmp;
			//////////////////////Inclues
			strTmp.Format("#include \"Inc/AS3%s.h\"\r\n",pInfo->strName);
			strInc+=strTmp;
			////////////////////Slots
			strTmp.Format("\tstatic const uint16_t s_slotsOffset%sClass = offsetof(::%s::%sClass, m_slots_%sClass);\r\n", 						  
						  pInfo->strName,m_strNamespace,pInfo->strName,pInfo->strName);
			//strTmp.Format("\tstatic const uint16_t s_slotsOffset%sClass = 0;\r\n", 						  
			//			  pInfo->strName,m_strNamespace);
			strSlot+=strTmp;
			if(pInfo->bInstance)
			{
				strTmp.Format("\tstatic const uint16_t s_slotsOffset%sObject = offsetof(::%s::%sObject,m_slots_%sObject);\r\n",							  
							  pInfo->strName,m_strNamespace,pInfo->strName,pInfo->strName);
			}
			else
				strTmp.Format("\tstatic const uint16_t s_slotsOffset%sObject=0;\r\n",pInfo->strName);
			strSlot+=strTmp;
			strTmp.Format("\tstatic void do%sClassAsserts(Traits* cTraits, Traits* iTraits)\r\n",pInfo->strName);
			strSlot+=strTmp;
			strSlot+="\t{\r\n\t\t(void)cTraits; (void)iTraits;\r\n";
			/*strTmp.Format("\t\tMMGC_STATIC_ASSERT(offsetof(::%s::%sClass, m_slots_%sClass) == s_slotsOffset%sClass);\r\n",
						  m_strNamespace,
						  pInfo->strName,pInfo->strName,pInfo->strName);
			strSlot+=strTmp;
			if(pInfo->bInstance)
			{
				strTmp.Format("\t\tMMGC_STATIC_ASSERT(offsetof(::%s::%sObject, m_slots_%sObject) == s_slotsOffset%sObject);\r\n",
						  m_strNamespace,
						  pInfo->strName,pInfo->strName,pInfo->strName);
				strSlot+=strTmp;
			}*/
			strSlot+="\t}\r\n";

			/////////////////////////////////////////////////////////
			// Make .h and .cpp
			CString strClassHeader,strClassCpp;
			//////////////////////////////////////////////////////////cpp
			if(!m_strPreHeader.IsEmpty())
			{
				strClassCpp.Format("#include \"%s\"\r\n",m_strPreHeader);		
			}

			strTmp.Format("#include \"avmplus.h\"\r\nusing namespace avmplus;\r\n#include \"../Inc/AS3%s.h\"\r\n",pInfo->strName);
			strClassCpp+=strTmp;

			strTmp.Format("namespace %s{\r\n",m_strNamespace);
			strClassCpp+=strTmp;
			strTmp.Format("%sClass::%sClass(VTable* cvtable):ClassClosure(cvtable)\r\n{\r\n",
						  pInfo->strName,pInfo->strName);
			strClassCpp+=strTmp;
			if(pInfo->bInstance)
			{
				//strTmp.Format("\tprototype = new (core()->GetGC(), cvtable->getExtraSize())"
							  //"%sObject(cvtable, toplevel()->objectClass->prototype, 0);\r\n",pInfo->strName);
				strClassCpp+="\tcreateVanillaPrototype();\r\n";//strTmp;
			}
			else
			{
				strClassCpp+="\tcreateVanillaPrototype();\r\n";
			}
			strClassCpp+="\t//Add your construct code here...\r\n};\r\n";

			if(pInfo->bInstance)
			{
				strTmp.Format("ScriptObject* %sClass::createInstance(VTable *ivtable, ScriptObject* prototype)\r\n"
							 "{\r\n\treturn new (core()->GetGC(), ivtable->getExtraSize()) %sObject(ivtable, prototype, 0);\r\n"
						     "\t//AS3 'new' opt...Add your init code here...\r\n}\r\n",
							 pInfo->strName,pInfo->strName);
				strClassCpp+=strTmp;
				strTmp.Format("%sObject::%sObject(VTable *vtable, ScriptObject* proto, int capacity)"
							  ": ScriptObject(vtable, proto, 0)\r\n{\r\n"
							  "\t//Add your construct code here...\r\n"
							  "}\r\n",pInfo->strName,pInfo->strName);
				strClassCpp+=strTmp;
			}
			strClassCpp+="//////////////////////////////////////////////////////////\r\n//Native Method start...\r\n";

			//////////////////////////////////////////////////////////////h
			strClassHeader.Format("#ifndef _AS3%s_\r\n#define _AS3%s_\r\n",
								  pInfo->strName,pInfo->strName);
			strClassHeader+="namespace avmplus{namespace NativeID{\r\n";
			strTmp.Format("class %sClassSlots{\r\nfriend class SlotOffsetsAndAsserts;\r\npublic:"
						  "//Declare your STATIC AS3 slots here!!!\r\n"
						  "\r\nprivate:};\r\n",pInfo->strName);
			strClassHeader+=strTmp;
			if(pInfo->bInstance)
			{
				strTmp.Format("class %sObjectSlots{\r\nfriend class SlotOffsetsAndAsserts;\r\npublic:\r\n"
							  "//Declare your MEMBER AS3 slots here!!!\r\n"
							  "private:};\r\n",pInfo->strName);
				strClassHeader+=strTmp;
			}
			strClassHeader+="}}\r\n";
			
			strTmp.Format("namespace %s{\r\n",m_strNamespace);
			strClassHeader+=strTmp;
			strTmp.Format("\tclass %sClass : public ClassClosure\r\n\t{\r\n\tpublic:\r\n",pInfo->strName);
			
			strClassHeader+=strTmp;
			strTmp.Format("\t\t%sClass(VTable *vtable);\r\n",pInfo->strName);
			strClassHeader+=strTmp;
			if(pInfo->bInstance)
			{
				strClassHeader+="\t\tScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);\r\n";
				
				strClassHeader+="\tprivate:\r\n\t\tfriend class avmplus::NativeID::SlotOffsetsAndAsserts;\r\n";
				strTmp.Format("\t\tavmplus::NativeID::%sClassSlots m_slots_%sClass;\r\n",pInfo->strName,pInfo->strName);
				strClassHeader+=strTmp;				

				strTmp.Format("};\r\nclass %sObject : public ScriptObject\r\n{\r\n\tpublic:\r\n",pInfo->strName);
				strClassHeader+=strTmp;
				strTmp.Format("\t\t%sObject(VTable* _vtable, ScriptObject* _delegate, int capacity);\r\n",pInfo->strName);
				strClassHeader+=strTmp;
				strClassHeader+="\tpublic:\r\n";
				strClassHeader+="\t\tvoid* pData;//Set your data!!\r\n";

			}
			//strTmp.Format("\t\tScriptObject* %sClass(VTable *vtable);",pInfo->strName);
			//strClassHeader+=strTmp;

			////////////////////////////Methods
			{
				FunctionInfo*pFun=pInfo->pInfo;
				while(pFun)
				{
					//CString strFun="AS3"+pFun->strName;
					CString strFun;
					BOOL bConstructor=FALSE;
					if(pFun->strName==pInfo->strName)
					{
						bConstructor=TRUE;
						strFun="AS3_constructor";
					}
					else
						strFun="AS3_"+pFun->strName;
					if(pInfo->bInstance)
					{
						strTmp.Format("%sAvmBox %sObject::%s(AvmMethodEnv, uint32_t, AvmBox* argv)\r\n"
									  "{\r\n"
									  "\t//Add your act code here...\r\n"
									  "\treturn kAvmThunkUndefined;//Modify this please!\r\n"
									  "}\r\n\r\n",
									  bConstructor?"//AS3 contructor function..\r\n":"",
									  pInfo->strName,strFun);
						strClassCpp+=strTmp;

						strTmp.Format("\t\tAVMTHUNK_NATIVE_METHOD_EX(%s, ::%s::%sObject::%s)\r\n",
									  pFun->strID,m_strNamespace,pInfo->strName,strFun);
						
					}
					else
					{
						strTmp.Format("%sAvmBox %sClass::%s(AvmMethodEnv, uint32_t, AvmBox* argv)\r\n"
									  "{\r\n"
									  "\t//Add your act code here...\r\n"
									  "\treturn kAvmThunkUndefined;//Modify this please!\r\n"
									  "}\r\n\r\n",
									  bConstructor?"//AS3 contructor function..\r\n":"",
									  pInfo->strName,strFun);
						strClassCpp+=strTmp;
						strTmp.Format("\t\tAVMTHUNK_NATIVE_METHOD_EX(%s, ::%s::%sClass::%s)\r\n",
									   pFun->strID,m_strNamespace,pInfo->strName,strFun);
					}
					strMethodMap+=strTmp;
					pFun=pFun->pNext;
					/////////////////////////////////////////////////////////
					// Method Header
					strTmp.Format("\t\tAvmBox %s(AvmMethodEnv env, uint32_t argc, AvmBox* argv);\r\n",strFun);
					strClassHeader+=strTmp;
				}
			}

			if(!pInfo->bInstance)
			{				
				strClassHeader+="\tprivate:\r\n\t\tfriend class avmplus::NativeID::SlotOffsetsAndAsserts;\r\n";
				strTmp.Format("\t\tavmplus::NativeID::%sClassSlots m_slots_%sClass;\r\n",pInfo->strName,pInfo->strName);
				strClassHeader+=strTmp;
			}
			else
			{
				strClassHeader+="\tprivate:\r\n\t\tfriend class avmplus::NativeID::SlotOffsetsAndAsserts;\r\n";
				strTmp.Format("\t\tavmplus::NativeID::%sObjectSlots m_slots_%sObject;\r\n",pInfo->strName,pInfo->strName);
				strClassHeader+=strTmp;
			}

			strClassHeader+="};}\r\n#endif";   
			strClassCpp+="}";

			CString strFile=strInclude;
			strFile+="\\AS3";
			strFile+=pInfo->strName;
			strFile+=".h";
			SaveFile(strFile,strClassHeader);
			strFile=strSrc;
			strFile+="\\AS3";
			strFile+=pInfo->strName;
			strFile+=".cpp";
			SaveFile(strFile,strClassCpp);


			//AVMTHUNK_NATIVE_METHOD(Object_private__hasOwnProperty, ObjectClass::_hasOwnProperty)
			///////////////////////////Classes
			if(pInfo->bInstance)
				strTmp.Format("\t\tAVMTHUNK_NATIVE_CLASS(%s, %sClass, ::%s::%sClass, SlotOffsetsAndAsserts::s_slotsOffset%sClass, "
				"::%s::%sObject, SlotOffsetsAndAsserts::s_slotsOffset%sObject)\r\n",
				pInfo->strID,pInfo->strName,
				m_strNamespace,
				pInfo->strName,pInfo->strName,m_strNamespace,pInfo->strName,pInfo->strName);
			else
				strTmp.Format("\t\tAVMTHUNK_NATIVE_CLASS(%s, %sClass, ::%s::%sClass, SlotOffsetsAndAsserts::s_slotsOffset%sClass, "
				"ScriptObject, SlotOffsetsAndAsserts::s_slotsOffset%sObject)\r\n",
				pInfo->strID,pInfo->strName,
				m_strNamespace,
				pInfo->strName,pInfo->strName,pInfo->strName);
			strClassMap+=strTmp;

			pInfo=pInfo->pNext;
		}

		strSlot+="};\r\n";
		strMethodMap+="\tAVMTHUNK_END_NATIVE_METHODS()\r\n";
		strClassMap+="\tAVMTHUNK_END_NATIVE_CLASSES()\r\n";
		strTxtCpp+=strInc;//strTxtCpp+="\r\n";
		
		strTmp.Format("namespace avmplus { namespace NativeID {\r\n#include \"AS3/%s.h\"\r\n#include \"AS3/%s.cpp\"\r\n",
					  m_strName,m_strName);
		strTxtCpp+=strTmp;

		strTxtCpp+=strSlot;strTxtCpp+="\r\n";
		strTxtCpp+="AvmBox shell_toplevel_normal_thunk(AvmMethodEnv env, uint32_t argc, AvmBox* argv)\r\n";
		strTxtCpp+="{\r\n\tenum {argoff0 = 0, argoff1 = argoff0 + AvmThunkArgSize_AvmObject};\r\n";
		strTxtCpp+="\t(void)argc;\r\n\ttypedef AvmBox (AvmObjectT::*FuncType)(AvmMethodEnv,uint32_t,AvmBox*);\r\n";
		strTxtCpp+="\tconst FuncType func = reinterpret_cast<FuncType>(AVMTHUNK_GET_METHOD_HANDLER(env));\r\n";
		strTxtCpp+="\treturn (*(AvmThunkUnbox_AvmReceiver(AvmObject, argv[argoff0])).*(func))(env,argc,argv);\r\n}\r\n";
		strTxtCpp+="#define _AVMTHUNK_NATIVE_METHOD_EX(CLS, METHID, IMPL) \\\r\n";
		strTxtCpp+="{ { _NATIVE_METHOD_CAST_PTR(CLS, &IMPL) }, (AvmThunkNativeThunker)shell_toplevel_normal_thunk, avmplus::NativeID::METHID },\r\n\r\n";
		strTxtCpp+="#define AVMTHUNK_NATIVE_METHOD_EX(METHID, IMPL) \\\r\n";
		strTxtCpp+="_AVMTHUNK_NATIVE_METHOD_EX(ScriptObject, METHID, IMPL)\r\n\r\n";

		strTxtCpp+=strClassGlu;strTxtCpp+="\r\n";
		strTmp.Format("AVMTHUNK_BEGIN_NATIVE_TABLES(%s)\r\n",m_strName);
		strTxtCpp+=strTmp;
		strTxtCpp+=strMethodMap;strTxtCpp+="\r\n";
		strTxtCpp+=strClassMap;strTxtCpp+="\r\n";
		strTxtCpp+="AVMTHUNK_END_NATIVE_TABLES()\r\n";
		strTmp.Format("AVMTHUNK_DEFINE_NATIVE_INITIALIZER(%s);\r\n}}",m_strName);
		strTxtCpp+=strTmp;
	}

	

	//"#ifndef _%s_\r\n#define _%s_\r\n",m_strFile,m_strFile);

	//strTxtInc+="#endif";

	CString strFile=strPath;
	strFile+="\\";
	strFile+=m_strFile;
	strFile+=".h";
	SaveFile(strFile,strTxtInc);

	strFile=strPath;
	strFile+="\\";
	strFile+=m_strFile;
	strFile+=".cpp";
	SaveFile(strFile,strTxtCpp);

	//CDialog::OnOK();
}

void CAS3AutoMapDlg::OnAutotext() 
{
	// TODO: Add your control notification handler code here
	CString strClass,strFunction,strTmp;
	ClassInfo*pInfo=m_pClassInfo;

	while(pInfo)
	{
		strTmp.Format("class %sClass;\r\n",pInfo->strName);
		strClass+=strTmp;
		strTmp.Format("%sClass* get%sClass(){return (%sClass*)getBuiltinExtensionClass(%s);}\r\n",
					  pInfo->strName,pInfo->strName,pInfo->strName,pInfo->strID);
		strFunction+=strTmp;
		pInfo=pInfo->pNext;

	}
	strClass+="\r\n";
	strClass+=strFunction;
	CTextDlg dlg(strClass);
	dlg.DoModal();
}
