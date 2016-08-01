// SWFEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SWFEdit.h"
#include "SWFEditDlg.h"
//#include "ShapeParser.h"
#include "XFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

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

XReplaceInfo::XReplaceInfo()
{
	ch = NULL;
	replacech = NULL;
	resWidth = 0;
	resHeight =0;
	mType = -1;
	next = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CSWFEditDlg dialog

BEGIN_MESSAGE_MAP(CSWFEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSWFEditDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EGET, OnEget)
	ON_BN_CLICKED(IDC_RGET, OnRget)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_ETREE, OnItemexpandedEtree)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_ETREE, &CSWFEditDlg::OnTvnSelchangedEtree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_RTREE, &CSWFEditDlg::OnTvnSelchangedRtree)
	ON_BN_CLICKED(IDC_CHECK1, &CSWFEditDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_REPLACE, &CSWFEditDlg::OnBnClickedReplace)
	ON_BN_CLICKED(IDC_RESTORE, &CSWFEditDlg::OnBnClickedRestore)
	ON_BN_CLICKED(IDC_PACKAGE, &CSWFEditDlg::OnBnClickedPackage)
	ON_BN_CLICKED(IDC_REPLACEIMAGE, &CSWFEditDlg::OnBnClickedReplaceimage)
	ON_BN_CLICKED(IDC_SAVE, &CSWFEditDlg::OnBnClickedSave)
END_MESSAGE_MAP()

CSWFEditDlg::CSWFEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSWFEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSWFEditDlg)
	m_strRFile = _T("");
	m_strEFile = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pReplaceSWF=NULL;
	m_pRSWF=NULL;
	m_pInfo=NULL;
	m_nMaxID=0;
}

CSWFEditDlg::~CSWFEditDlg()
{
	if(m_pReplaceSWF)
		delete m_pReplaceSWF;
	if(m_pRSWF)
		delete m_pRSWF;
	ClearInfo();
}

void CSWFEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSWFEditDlg)
	DDX_Control(pDX, IDC_RTREE, m_tTree);
	DDX_Control(pDX, IDC_ETREE, m_ReplaceTree);
	DDX_Control(pDX, IDC_RSHOW, m_rShow);
	DDX_Control(pDX, IDC_ESHOW, m_ReplaceShow);
	DDX_Text(pDX, IDC_RFILE, m_strRFile);
	DDX_Text(pDX, IDC_EFILE, m_strEFile);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSWFEditDlg message handlers

XBOOL CSWFEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	CRect rect;
	m_ReplaceShow.GetClientRect(rect);
	m_showSize=rect.Size();

	m_images.Create(IDB_TREEICON, 16, 1, RGB(255, 255, 255));
	m_status.Create(IDB_STATUS,13,1,RGB(255,255,255));
	m_ReplaceTree.SetImageList(&m_images,TVSIL_NORMAL);
	m_ReplaceTree.SetImageList(&m_status,LVSIL_STATE);
	m_tTree.SetImageList(&m_images,TVSIL_NORMAL);
	m_tTree.SetImageList(&m_status,LVSIL_STATE);
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
	
	// TODO: Add extra initialization here
	EnableItem();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSWFEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSWFEditDlg::OnPaint() 
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
HCURSOR CSWFEditDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSWFEditDlg::OnEget() 
{
	// TODO: Add your control notification handler code here
   CFileDialog dlg( TRUE, _T("swf"), m_strEFile, 
                    OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
					OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST,"Flash文件|*.swf|");
   if( dlg . DoModal(  ) != IDOK ) return;
   m_strEFile=dlg.GetPathName();
   SetDlgItemText(IDC_EFILE,m_strEFile);

   if(m_pReplaceSWF)
	   delete m_pReplaceSWF;
   m_pReplaceSWF=NULL;
   ClearInfo();
   m_pReplaceSWF=LoadSWF(m_strEFile);

   m_ReplaceShow.m_pPlayer=m_pReplaceSWF;
   m_ReplaceShow.ShowObject(NULL);
   InitTreeRoot(m_ReplaceTree,m_pReplaceSWF );
   
   EnableItem();

   if(!m_pReplaceSWF)
   {
	   AfxMessageBox(_T("载入SWF文件失败!"));
   }


}

void CSWFEditDlg::OnRget() 
{
	// TODO: Add your control notification handler code here
   CFileDialog dlg( TRUE, _T("swf"), m_strRFile, 
                    OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
					OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST,"Flash文件|*.swf|");
   if( dlg . DoModal(  ) != IDOK ) return;
   m_strRFile=dlg.GetPathName();
   SetDlgItemText(IDC_RFILE,m_strRFile);

   if(m_pRSWF)
	   delete m_pRSWF;

   ClearReplaceInfo();

   m_pRSWF=NULL;
   m_pRSWF=LoadSWF(m_strRFile);

   m_rShow.m_pPlayer=m_pRSWF;
   m_rShow.ShowObject(NULL);
   InitSrcTreeRoot(m_tTree,m_pRSWF );

   EnableItem();
   
   if(!m_pRSWF)
   {
	   AfxMessageBox(_T("载入SWF文件失败!"));
   }	

}

void CSWFEditDlg::InitTreeRoot(CTreeCtrl &tree,XSWFPlayer*player )
{
	tree.DeleteAllItems();
	HTREEITEM rShape=tree.InsertItem(_T("形状"),3,3,TVI_ROOT);
	HTREEITEM rBits=tree.InsertItem(_T("图像"),3,3,TVI_ROOT);
	HTREEITEM rButton = tree.InsertItem(_T("按钮"), 3, 3, TVI_ROOT );
	HTREEITEM rFont = tree.InsertItem(_T("字体"), 3, 3, TVI_ROOT );
	HTREEITEM rText = tree.InsertItem(_T("文本"), 3, 3, TVI_ROOT );
	HTREEITEM rSound = tree.InsertItem( _T("声音"), 3, 3, TVI_ROOT );
	HTREEITEM rSprite = tree.InsertItem( _T("精灵"), 3, 3, TVI_ROOT );
	HTREEITEM rMorph = tree.InsertItem( _T("变形"), 3, 3, TVI_ROOT );
	HTREEITEM rCommand = tree.InsertItem( _T("命令"), 3, 3, TVI_ROOT );
	HTREEITEM rEditText=tree.InsertItem(_T("编辑文本"),3,3,TVI_ROOT);
	HTREEITEM rVideo = tree.InsertItem( _T("视频"), 3, 3, TVI_ROOT );
	HTREEITEM rSpriteEmpty = tree.InsertItem( _T("精灵空"), 3, 3, TVI_ROOT );
	HTREEITEM rEditTextEmpty = tree.InsertItem( _T("编辑文本空"), 3, 3, TVI_ROOT );
	HTREEITEM rButtonEmpty = tree.InsertItem( _T("按钮空"), 3, 3, TVI_ROOT );
	HTREEITEM rShapeEmpty = tree.InsertItem( _T("形状空"), 3, 3, TVI_ROOT );
	HTREEITEM rBitmapEmpty = tree.InsertItem( _T("位图空"), 3, 3, TVI_ROOT );
	HTREEITEM rBinaryData = tree.InsertItem( _T("二进制数据"), 3, 3, TVI_ROOT );
	HTREEITEM rPlayer = tree.InsertItem( _T("播放"), 3, 3, TVI_ROOT );
	HTREEITEM rVideoEmpty = tree.InsertItem( _T("视频空"), 3, 3, TVI_ROOT );
	HTREEITEM rLoader = tree.InsertItem( _T("加载器"), 3, 3, TVI_ROOT );
	HTREEITEM rLoaderImage = tree.InsertItem( _T("加载器图像"), 3, 3, TVI_ROOT );
	HTREEITEM rScroll = tree.InsertItem( _T("滚动"), 3, 3, TVI_ROOT );
	HTREEITEM rLoaderSWF = tree.InsertItem( _T("SWF加载器"), 3, 3, TVI_ROOT );
	HTREEITEM rRootClone = tree.InsertItem( _T("根克隆"), 3, 3, TVI_ROOT );
	HTREEITEM rTextLine = tree.InsertItem( _T("文本行"), 3, 3, TVI_ROOT );
	HTREEITEM rFontName = tree.InsertItem( _T("字体名"), 3, 3, TVI_ROOT );
	HTREEITEM rBitmap = tree.InsertItem( _T("位图"), 3, 3, TVI_ROOT );
	HTREEITEM rSpriteExternal = tree.InsertItem( _T("外部精灵"), 3, 3, TVI_ROOT );
	HTREEITEM rRoot = tree.InsertItem(_T("根"), 3, 3, TVI_ROOT );
	HTREEITEM rLost = tree.InsertItem( _T("丢失"), 3, 3, TVI_ROOT );
	HTREEITEM rOther = tree.InsertItem( _T("其他"), 3, 3, TVI_ROOT );

	m_nMaxID=0;
	
	if(player && player->player)
	{
		ScriptPlayer*pl=player->player;
		SCharacter** chars=player->player->charIndex;
		int i = 0;
		int nc=ScriptPlayer::charIndexSize;
		HTREEITEM item;
		CString strTxt;
		for(i=0;i<nc;i++)
		{
			SCharacter* ch = chars[i];
			
			while(ch)
			{
				if( ch->type<rootChar )
				{
					if(m_nMaxID<ch->tag)
						m_nMaxID=ch->tag;
				}
				switch(ch->type)
				{
				case shapeChar:
					strTxt.Format(_T("形状 - %d(%d,%d,%d,%d)"),ch->tag,
						ch->rbounds.xmin/20,
						ch->rbounds.ymin/20,
						ch->rbounds.xmax/20,
						ch->rbounds.ymax/20);
					item=tree.InsertItem(strTxt,0,0,rShape);
					{
						XReplaceInfo*info=new XReplaceInfo;
						info->ch=ch;
						info->replacech=NULL;
						info->next=m_pInfo;
						m_pInfo=info;
						tree.SetItemData(item,(DWORD)info);
					}
					break;
				case bitsChar:
					pl->BuildBits(ch);
					if(ch->bits.HasBits())
					{
						ch->rbounds.xmin=0;
						ch->rbounds.ymin=0;
						ch->rbounds.xmax=ch->bits.width*20;
						ch->rbounds.ymax=ch->bits.height*20;
					}
					strTxt.Format(_T("图像 - %d(%dx%d)"),ch->tag,ch->rbounds.xmax/20,ch->rbounds.ymax/20);
					item=tree.InsertItem(strTxt,1,1,rBits);
					{
						XReplaceInfo*info=new XReplaceInfo;
						info->ch=ch;
						info->replacech=NULL;
						info->next=m_pInfo;
						m_pInfo=info;
						tree.SetItemData(item,(DWORD)info);
					}
					break;
				case buttonChar:
					strTxt.Format(_T("按钮 - %d"),ch->tag);
					item = tree.InsertItem( strTxt, 2, 2, rButton);
					{
						XU8 * cxData = ch->button.cxformData;
						SParser cxParser;
						if ( cxData )
							cxParser.Attach(cxData, 0);

						SParser parser;
						parser.Attach( ch->data, 0 );
						int btnID = parser.GetWord();
						while( true )
						{
							XU8 stateFlags = parser.GetByte();
							if ( !stateFlags ) 
								break;
							PlaceInfo plInfo;
							plInfo.flags = splaceCharacter|splaceMatrix|splaceColorTransform;
							XU16 tag = parser.GetWord();
							if ( tag == 373 || tag == 378 )
							{
								int i = 0;
							}
							plInfo.depth = parser.GetWord();
							parser.GetMatrix(&plInfo.mat);
							XFilter *pFilterList=NULL;
							XFilter *pNextFilter=NULL;
							if ( ch->tagCode == stagDefineButton2 )
							{
								parser.GetColorTransform(&plInfo.cxform, XTRUE);	// new style button
								if(stateFlags&sbtnFilterList)
								{
									XU8 num=parser.GetByte();
									for(int i=0;i<num;i++)
									{
										XFilter*pFilter=XFilter::CreateFilter(&parser, player);
										if(!pFilter) 
											break;
										pFilter->m_pNext=NULL;
										if(pNextFilter==NULL)
										{
											pNextFilter=pFilter;
											pFilterList=pFilter;
										}
										else
										{
											pNextFilter->m_pNext=pFilter;
											pNextFilter=pFilter;
										}
									}
								}
								if(stateFlags&sbtnBlendMode)
								{
									plInfo.blendMode=parser.GetByte();
								}
							}
							else if ( cxData )
								cxParser.GetColorTransform(&plInfo.cxform,XFALSE);// separate cxfrom data
							else
								plInfo.cxform.Clear();							// no cxform data

							plInfo.character = pl->FindCharacter(tag);
							plInfo.name = _strEmpty;
							plInfo.pFilter=pFilterList;
							if ( plInfo.character != NULL )
							{
								XReplaceInfo *info = new XReplaceInfo;
								info->ch = plInfo.character;
								info->replacech = NULL;
								info->next = m_pInfo;
								info->mType = buttonChar;
								m_pInfo = info;
								if(stateFlags & 1 )
								{
									strTxt.Format(_T("弹起 - %d"), tag);
									HTREEITEM btnUp = tree.InsertItem( strTxt, item );
									tree.SetItemData( btnUp, (DWORD)info );
								}
								else if(stateFlags & 2)
								{
									strTxt.Format(_T("覆盖 - %d"), tag);
									HTREEITEM btnOver = tree.InsertItem(strTxt, item );
									tree.SetItemData( btnOver, (DWORD)info );
								}
								else if( stateFlags & 4 )
								{
									strTxt.Format(_T("按下 - %d"), tag);
									HTREEITEM btnDown = tree.InsertItem( strTxt, item );
									tree.SetItemData( btnDown, (DWORD)info );
								}
								else if(stateFlags & 8 )
								{
									strTxt.Format(_T("点击 - %d"), tag);
									HTREEITEM btnHit = tree.InsertItem(strTxt, item );
									tree.SetItemData( btnHit, (DWORD)info );
								}
							}
							if(pFilterList)
							{
								XFilter*p=pFilterList;
								while(p)
								{
									XFilter*pNext=p->m_pNext;
									player->FreeFilter(p);
									p=pNext;
								}
								pFilterList=XNULL;
							}
						}
					}
					break;
				case fontChar:
					strTxt.Format( _T("字体 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rFont );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD) info );
					}
					break;
				case textChar:
					strTxt.Format( _T("文本 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rText );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD) info );
					}
					break;
				case soundChar:
					strTxt.Format( _T("声音 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rSound );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case spriteChar:
					strTxt.Format( _T("精灵 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rSprite );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case morphShapeChar:
					strTxt.Format( _T("变形 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rMorph );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case commandChar:	
					strTxt.Format( _T("命令"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rCommand );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case editTextChar:
					strTxt.Format(_T("文字 - %d"),ch->tag);
					item=tree.InsertItem(strTxt,2,2,rEditText );
					{
						XReplaceInfo*info=new XReplaceInfo;
						info->ch=ch;
						info->replacech=NULL;
						info->next=m_pInfo;
						m_pInfo=info;
						tree.SetItemData(item,(DWORD)info);
					}
					break;
				case videoChar:
					strTxt.Format( _T("视频 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rVideo );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
					///////////////
				case spriteEmptyChar:
					strTxt.Format( _T("空精灵 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rSpriteEmpty );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case editTextEmptyChar:
					strTxt.Format( _T("编辑文本空 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rEditTextEmpty );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case buttonEmptyChar:
					strTxt.Format( _T("按钮空 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rButtonEmpty );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case shapeEmptyChar:
					strTxt.Format( _T("形状空 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rShapeEmpty );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case bitmapEmptyChar:
					strTxt.Format( _T("位图空 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rBitmapEmpty );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case binaryDataChar:
					strTxt.Format( _T("二进制数据 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rBinaryData );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case playerChar:
					strTxt.Format( _T("播放 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rPlayer );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case videoEmptyChar:
					strTxt.Format( _T("视频空 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rVideoEmpty );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case loaderChar:
					strTxt.Format( _T("加载器 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rLoader );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case loaderImageChar:
					strTxt.Format( _T("加载器图像 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rLoaderImage );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case scrollChar:
					strTxt.Format( _T("滚动 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rScroll );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case loaderSWFChar:
					strTxt.Format( _T("SWF加载器 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rLoaderSWF );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case rootCloneChar:
					strTxt.Format( _T("根克隆 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rRootClone );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case textLineChar:
					strTxt.Format( _T("文本行 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rTextLine );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case fontNameChar:
					strTxt.Format( _T("字体名 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rFontName );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case bitmapChar:
					strTxt.Format( _T("位图 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rBitmap );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case spriteExternalChar:
					strTxt.Format( _T("外部精灵 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rSpriteExternal );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case rootChar:
					strTxt.Format( _T("根 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rRoot );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				case lostChar:
					strTxt.Format( _T("丢失 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rLost );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				default:
					strTxt.Format( _T("其他 - %d"), ch->tag );
					item = tree.InsertItem( strTxt, 2, 2, rOther );
					{
						XReplaceInfo *info = new XReplaceInfo;
						info->ch = ch;
						info->replacech = NULL;
						info->next = m_pInfo;
						m_pInfo = info;
						tree.SetItemData( item, (DWORD)info );
					}
					break;
				
				}
				ch=ch->next;
			}
		}
	}
	//tree.InsertItem(_T("图1"),0,0,r1);
}

void CSWFEditDlg::InitSrcTreeRoot(CTreeCtrl&tree,XSWFPlayer*player )
{
	tree.DeleteAllItems();
	HTREEITEM r1=tree.InsertItem(_T("形状"),3,3,TVI_ROOT);
	HTREEITEM r2=tree.InsertItem(_T("图片"),3,3,TVI_ROOT);
	HTREEITEM r3 = tree.InsertItem(_T("按钮"), 3, 3, TVI_ROOT );
	HTREEITEM r4=tree.InsertItem(_T("文字"),3,3,TVI_ROOT);

	if(player && player->player)
	{
		ScriptPlayer*pl=player->player;
		SCharacter** chars=player->player->charIndex;
		int i = 0;
		int nc=ScriptPlayer::charIndexSize;
		HTREEITEM item;
		CString strTxt;
		for(i=0;i<nc;i++)
		{
			SCharacter* ch=chars[i];

			while(ch)
			{
				switch(ch->type)
				{
				case shapeChar:
					strTxt.Format(_T("形状 - %d(%d,%d,%d,%d)"),ch->tag,
						ch->rbounds.xmin/20,
						ch->rbounds.ymin/20,
						ch->rbounds.xmax/20,
						ch->rbounds.ymax/20);
					item=tree.InsertItem(strTxt,0,0,r1);
					tree.SetItemData(item,(DWORD)ch);
				
					break;
				case bitsChar:
					pl->BuildBits(ch);
					if(ch->bits.HasBits())
					{
						ch->rbounds.xmin=0;
						ch->rbounds.ymin=0;
						ch->rbounds.xmax=ch->bits.width*20;
						ch->rbounds.ymax=ch->bits.height*20;
					}
					strTxt.Format(_T("图像 - %d(%dx%d)"),ch->tag,ch->rbounds.xmax/20,ch->rbounds.ymax/20);
					item=tree.InsertItem(strTxt,1,1,r2);
					tree.SetItemData(item,(DWORD)ch);
					
					break;
				case buttonChar:
					strTxt.Format(_T("按钮 - %d"),ch->tag);
					item = tree.InsertItem( strTxt, 2, 2, r3);
					break;
				case editTextChar:
					strTxt.Format(_T("文字 - %d"),ch->tag);
					item=tree.InsertItem(strTxt,3,3,r4 );
					tree.SetItemData(item,(DWORD)ch);	
					break;
				default:
					if ( ch->tag == 45 )
					{
						int i = 0;
					}
					break;

				}
				ch=ch->next;
			}
		}
	}
	return;
}

void CSWFEditDlg::OnItemexpandedEtree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	HTREEITEM item=pNMTreeView->itemNew.hItem;
	if(pNMTreeView->itemNew.iImage>=3)
	{
		int iImage=0;
		switch(pNMTreeView->action)
		{
		case 1:
			iImage=3;
			break;
		case 2:
			iImage=4;
			break;
		default:
			break;
		}
		if(iImage)
			m_ReplaceTree.SetItemImage(item,iImage,iImage);
	}
	*pResult = 0;
}

XSWFPlayer* CSWFEditDlg::LoadSWF(CString&strFile)
{
	char*pData=NULL;
	CFile file;
	if(!file.Open(strFile,CFile::modeRead|CFile::typeBinary)) return NULL;
	int ilen = file.GetLength();
	if(ilen<=16)
	{
		file.Close();
		return NULL;
	}
	pData=new char[ilen];
	file.Read(pData,ilen);
	file.Close();
	XSWFPlayer*player=new XSWFPlayer(0,0);
	player->m_pDom=NULL;
	player->m_nDomSize=0;
	XString8Array list;
	XStreamData stream;
	/*typedef struct _XStreamData
{
	XXVar			url;			//!< The URL of the stream.
	ScriptPlayer*	scriptPlayer;	//!< Pointer to the ScriptPlayer associated with this download stream.
	void*		    pLoader;
	BOOL			resize;			//!< Should the window be resized to this stream dimension?
	int			    nTotalBytes;
	int				nLoadedBytes;
	_XLoadInfo*	    loadInfo;
#ifdef _WINEMU_DEBUGFILE
	XFile			m_downFile;
#endif
//	LoadInfo*		info;
}StreamData;*/
	
	XString8 strURL(strFile);
	stream.loadInfo=NULL;
	stream.nLoadedBytes=ilen;
	stream.nTotalBytes=ilen;
	stream.pLoader=NULL;
	stream.resize=XFALSE;
	stream.scriptPlayer=NULL;
	stream.url=strURL;
	player->m_strWorkPath.SetString("M:\\",3);
	player->InitCachePath();
	XSize sz(m_showSize.cx,m_showSize.cy);
	player->InitPlayer(sz,sz,NULL);
	player->m_bDataOnly=XTRUE;
	player->StreamInNew(&stream,strURL,strURL,"",NULL,list,200,NULL);
	//AfxMessageBox(_T("OK"));
	player->StreamInWrite(&stream,pData,ilen,NULL);
	//AfxMessageBox(_T("OK"));
	player->StreamInDestroy(&stream,XTRUE,"",NULL);
	//AfxMessageBox(_T("OK"));
	bool bError=player->bError!=0;
	if(bError)
	{
		delete player;
		return NULL;
	}
	else 
	{
		player->FullScreen();
		player->display.root.FreeChildren();
		player->display.camera.mat.d=player->display.camera.mat.a;
	}
	return player;
}


void CSWFEditDlg::OnTvnSelchangedEtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM item=pNMTreeView->itemNew.hItem;
	SCharacter*ch=NULL;
	XReplaceInfo*info=(XReplaceInfo*)m_ReplaceTree.GetItemData(item);
	if(info)
	{
		ch=info->ch;
		m_ReplaceShow.miType = info->mType;
	}
	m_ReplaceShow.ShowObject(ch);

	ShowPrev();

	EnableItem();

	*pResult = 0;
}

void CSWFEditDlg::OnTvnSelchangedRtree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM item=pNMTreeView->itemNew.hItem;
	SCharacter*ch=NULL;
	ch=(SCharacter*)m_tTree.GetItemData(item);
	m_rShow.ShowObject(ch);

	EnableItem();

	*pResult = 0;
}

void CSWFEditDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bFull=this->IsDlgButtonChecked(IDC_CHECK1);
	m_ReplaceShow.mbFullScreen = bFull;
	m_rShow.mbFullScreen = bFull;
	m_ReplaceShow.ShowObject(m_ReplaceShow.m_showObj);
	if(m_rShow.m_dib.IsEmpty())
		m_rShow.ShowObject(m_rShow.m_showObj);
}



void CSWFEditDlg::EnableItem()
{
	HTREEITEM item=m_ReplaceTree.GetSelectedItem();
	GetDlgItem(IDC_PACKAGE)->EnableWindow(m_pReplaceSWF!=NULL&&m_pInfo!=NULL);
	XReplaceInfo*info=item==NULL?NULL:(XReplaceInfo*)m_ReplaceTree.GetItemData(item);
	item=m_tTree.GetSelectedItem();
	SCharacter*ch=item==NULL?NULL:(SCharacter*)m_tTree.GetItemData(item);
	if(info==NULL)
	{
		GetDlgItem(IDC_REPLACE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RESTORE)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPLACEIMAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RESTORE)->EnableWindow(info->replacech!=NULL||!info->strReplaceImage.IsEmpty());
		if(ch==NULL||ch==info->replacech||ch->type!=info->ch->type)
			GetDlgItem(IDC_REPLACE)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_REPLACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_REPLACEIMAGE)->EnableWindow(TRUE);//->ch->type==bitsChar);
		GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);//info->ch->type==bitsChar);
	}
}

void CSWFEditDlg::OnBnClickedReplaceimage()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM item=m_ReplaceTree.GetSelectedItem();	
	XReplaceInfo*info=item==NULL?NULL:(XReplaceInfo*)m_ReplaceTree.GetItemData(item);
	if(info==NULL) return;
   CFileDialog dlg( TRUE, _T("png"), m_strRFile, 
                    OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
					OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST,"PNG文件|*.png|jpg文件|*.jpg|gif文件|*.gif|");
   if( dlg . DoModal(  ) != IDOK ) return;
   info->strReplaceImage=dlg.GetPathName();
   info->replacech=NULL;
   m_rShow.ShowObject(info->strReplaceImage);
   if(!m_rShow.m_dib.IsEmpty())
   {
	   int w0=(info->ch->rbounds.xmax-info->ch->rbounds.xmin)/20;
	   int h0=(info->ch->rbounds.ymax-info->ch->rbounds.ymin)/20;
	   info->resWidth = w0;
	   info->resHeight = h0;
	   if(m_rShow.m_dib.Width()!=w0 || m_rShow.m_dib.Height()!=h0)
		  AfxMessageBox(_T("替换图片的大小和原大小不一样!"));
   }
   EnableItem();
	
}

void CSWFEditDlg::ShowPrev()
{
	HTREEITEM item=m_ReplaceTree.GetSelectedItem();	
	XReplaceInfo*info=item==NULL?NULL:(XReplaceInfo*)m_ReplaceTree.GetItemData(item);
	item=m_tTree.GetSelectedItem();
	SCharacter*ch=item==NULL?NULL:(SCharacter*)m_tTree.GetItemData(item);
	if(info==NULL)
	{	m_rShow.ShowObject(ch);	}
	else if(!info->strReplaceImage.IsEmpty())
	{
		m_rShow.ShowObject(info->strReplaceImage);
	}	
	else if(info->replacech)
		m_rShow.ShowObject(info->replacech);
	else
		m_rShow.ShowObject(ch);

}

void CSWFEditDlg::OnBnClickedReplace()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM item=m_ReplaceTree.GetSelectedItem();	
	XReplaceInfo*info=item==NULL?NULL:(XReplaceInfo*)m_ReplaceTree.GetItemData(item);
	item=m_tTree.GetSelectedItem();
	SCharacter*ch=item==NULL?NULL:(SCharacter*)m_tTree.GetItemData(item);
	if(info==NULL||ch==NULL) return;
	int w0=info->ch->rbounds.xmax-info->ch->rbounds.xmin;
	int h0=info->ch->rbounds.ymax-info->ch->rbounds.ymin;
	int w1=ch->rbounds.xmax-ch->rbounds.xmin;
	int h1=ch->rbounds.ymax-ch->rbounds.ymin;
	if(w1!=w1||h0!=h1)
	{
		if(AfxMessageBox(_T("对象的大小不一致，确定要替换吗?"),MB_YESNO|MB_ICONQUESTION)!=IDYES) return;
	}
	info->replacech=ch;
	EnableItem();
}

void CSWFEditDlg::OnBnClickedRestore()
{
	// TODO: 在此添加控件通知处理程序代码
	HTREEITEM item=m_ReplaceTree.GetSelectedItem();	
	XReplaceInfo*info=item==NULL?NULL:(XReplaceInfo*)m_ReplaceTree.GetItemData(item);
	item=m_tTree.GetSelectedItem();	
	if(info==NULL) return;
	info->replacech=NULL;
	info->strReplaceImage.Empty();
	EnableItem();
	ShowPrev();
}

BYTE* CSWFEditDlg::FindTag(BYTE*pData,BYTE*pEnd,int nTag,int&nLen,int&nTotal)
{
	//pData+=21;
	while(pData+2<pEnd)
	{
		BYTE*pos=pData;
		nTotal=0;
		int tagCode = (XU16)pData[0] | ((XU16)pData[1]<<8);
		pData+=2;
		nTotal+=2;

		XS32 tagLen = tagCode & 0x3f;
		if ( tagLen == 0x3f )
		{
			if ( pData+4>=pEnd ) 
			{
				return NULL;
			}
			tagLen = (XU32)pData[0] | ((XU32)pData[1]<<8) | ((XU32)pData[2]<<16) | ((XU32)pData[3]<<24);//GetDWord();
			pData+=4;
			nTotal+=4;
		}
		int tag=(XU16)pData[0] | ((XU16)pData[1]<<8);
		pData+=tagLen;
		//tagEnd = pos + tagLen;
		if ( pData>=pEnd )
		{
			return NULL;
		}
		tagCode = tagCode >> 6;
		switch(tagCode)
		{
			case stagDefineShape:
			case stagDefineShape2:
			case stagDefineShape3:
			case stagDefineShape4:
			case stagDefineBits:
			case stagDefineBitsLossless:
			case stagDefineBitsJPEG2:
			case stagDefineBitsLossless2:
			case stagDefineBitsJPEG3:
				 if(tag==nTag)
				 {
					 nLen=tagLen;
					 nTotal+=nLen;
					 return pos;
				 }
		}
	}
	return NULL;
}

bool CSWFEditDlg::IsShape(SCharacter*ch)
{
	switch(ch->tagCode)
	{
		case stagDefineShape:
		case stagDefineShape2:
		case stagDefineShape3:
		case stagDefineShape4:
			 return TRUE;
	}
	return FALSE;
}

void CSWFEditDlg::PackageLossLess(XDrawDib&dib,XU8Array&data,int nTag)
{
	int nSize=dib.m_nDataLength*2;
	
	BYTE*pData=new BYTE[nSize];

	int len = dib.m_nDataLength;
	
	unsigned long gzlen = nSize;

	// Use zlib to compress the data
	SBitmapSwapBits(dib.GetBuffer(),dib.m_nDataLength,bm32Bit);
	compress2(pData, (unsigned long*)&gzlen,
		dib.GetBuffer(), len, 9);
	//SBitmapSwapBits(dib.GetBuffer(),dib.m_nDataLength,bm32Bit);

	// Replace the byte array with the compressed data
	int nPacksize=gzlen+7,nPackID=stagDefineBitsLossless<<6;
	if(nPacksize>=63)
	{
		nPackID|=0x3f;
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
		data.Add((nPacksize)&0xff);
		data.Add((nPacksize>>8)&0xff);
		data.Add((nPacksize>>16)&0xff);
		data.Add((nPacksize>>24)&0xff);
	}
	else
	{
		nPackID|=nPacksize;
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
	}
	data.Add((nTag)&0xff);
	data.Add((nTag>>8)&0xff);
	data.Add(5);
	data.Add((dib.Width())&0xff);
	data.Add((dib.Width()>>8)&0xff);
	data.Add((dib.Height())&0xff);
	data.Add((dib.Height()>>8)&0xff);
	data.Append(pData,gzlen);
	delete pData;
}
void CSWFEditDlg::PackageLossLess2(XDrawDib&dib,XU8Array&data,int nTag)
{
	int nSize=dib.m_nDataLength*2;
	
	SBitmapSwapBits(dib.GetBuffer(),dib.m_nDataLength,bm32Bit);

	BYTE*pData=new BYTE[nSize];

	int len = dib.m_nDataLength;
	
	unsigned long gzlen = nSize;

	// Use zlib to compress the data
	compress2(pData, (unsigned long*)&gzlen,
		dib.GetBuffer(), len, 9);

	//SBitmapSwapBits(dib.GetBuffer(),dib.m_nDataLength,bm32Bit);

	// Replace the byte array with the compressed data
	int nPacksize=gzlen+7,nPackID=stagDefineBitsLossless2<<6;
	if(nPacksize>=63)
	{
		nPackID|=0x3f;
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
		data.Add((nPacksize)&0xff);
		data.Add((nPacksize>>8)&0xff);
		data.Add((nPacksize>>16)&0xff);
		data.Add((nPacksize>>24)&0xff);
	}
	else
	{
		nPackID|=nPacksize;
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
	}
	data.Add((nTag)&0xff);
	data.Add((nTag>>8)&0xff);
	data.Add(5);
	data.Add((dib.Width())&0xff);
	data.Add((dib.Width()>>8)&0xff);
	data.Add((dib.Height())&0xff);
	data.Add((dib.Height()>>8)&0xff);
	data.Append(pData,gzlen);
	delete pData;
}

int CSWFEditDlg::MaxBits(int value)
{
	int bits=1;
	if(value<0)
	{
		//bits++;
		value=-value;
	}
	while(value>0)
	{
		bits++;
		value=value>>1;
	}
	return bits;
}

void CSWFEditDlg::MakeImageShape(XU8Array&data,XDrawDib&dib,int tagID,int imageID,int offx,int offy)
{
	//XU8Array cnt;
	XStream stream;
	stream.WriteWord((XS16)tagID);
	int xmax=dib.Width()*20+offx;
	int ymax=dib.Height()*20+offy;
	xmax /= 2;
	ymax /= 2;
	int bitsx = MaxBits(xmax);
	int bitsy = MaxBits(ymax);
	if(bitsx < bitsy) 
		bitsx=bitsy;

	bitsy = MaxBits(offx);
	if(bitsx < bitsy)
		bitsx=bitsy;
	bitsy=MaxBits(offy);
	if(bitsx<bitsy)
		bitsx=bitsy;

	{//WriteRect
		stream.WriteBits(bitsx,5);
		stream.WriteBits(offx,bitsx);
		stream.WriteBits(xmax,bitsx);
		stream.WriteBits(offy,bitsx);
		stream.WriteBits(ymax,bitsx);
		stream.FlushWriteBits();
	}
	//Fills
	{
		stream.WriteByte((XS8)1);
		stream.WriteByte((XS8)0x42);//Bitmap Fill
		stream.WriteWord((XU16)imageID);
		stream.WriteBits(1,1);//Scale
		stream.WriteBits(22,5);
		stream.WriteBits(0xA0000,22);
		stream.WriteBits(0xA0000,22);
		stream.WriteBits(0,1);//No Rotate
		stream.WriteBits(bitsx,5);
		stream.WriteBits(offx,bitsx);
		stream.WriteBits(offy,bitsx);
		stream.FlushWriteBits();
	}
	{
		stream.WriteByte((XS8)0);//Lines
	}
	{
		stream.WriteBits(2,4);
		stream.WriteBits(0,4);
		stream.FlushWriteBits();
	}
	{
		bitsx++;
		int nDataBits=bitsx-2;
		if(nDataBits<1) nDataBits=1;
		stream.WriteBits(0,1);
		int flags=eflagsMoveTo|eflagsFill0;
		stream.WriteBits(flags,5);//MoveTo(0,0)
		stream.WriteBits(nDataBits,5);
		stream.WriteBits(offx,nDataBits);
		stream.WriteBits(offy,nDataBits);
		stream.WriteBits(1,2);//Fill0
		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(0,1);//Hoz
		stream.WriteBits(xmax,bitsx); //LineTo(maxx,0)


		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(1,1);//Hoz
		stream.WriteBits(ymax,bitsx); //LineTo(maxx,maxy)

		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(0,1);//Hoz
		stream.WriteBits(-xmax,bitsx); //LineTo(0,maxy)

		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(1,1);//Hoz
		stream.WriteBits(-ymax,bitsx); //LineTo(0,0)

		stream.WriteBits(0,1);//Edge
		stream.WriteBits(0,5);//End
		stream.FlushWriteBits();
	}

		
	int nPacksize=stream.GetDataSize(),nPackID=stagDefineShape<<6;
	if(nPacksize>=63)
	{
		nPackID|=0x3f;		
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
		data.Add((nPacksize)&0xff);
		data.Add((nPacksize>>8)&0xff);
		data.Add((nPacksize>>16)&0xff);
		data.Add((nPacksize>>24)&0xff);
	}
	else
	{
		nPackID|=nPacksize;
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
	}
	data.Append(stream.GetData(),stream.GetDataSize());
}


void CSWFEditDlg::MakeImageShape(XU8Array&data,XDrawDib&dib,int imageID, XReplaceInfo *info )
{
	if ( info == NULL )
	{
		return;
	}
	XStream stream;
	stream.WriteWord((XS16)info->ch->tag);
	int offx = info->ch->rbounds.xmin;
	int offy = info->ch->rbounds.ymin;
	float scalex = (float)info->resWidth /  (float)dib.Width();
	float scaley = (float)info->resHeight / (float)dib.Height();
	int xmax= dib.Width()*20+offx;
	int ymax= dib.Height()*20+offy;
	xmax = xmax * scalex;
	ymax = ymax * scaley;
	int bitsx = MaxBits(xmax);
	int bitsy = MaxBits(ymax);
	if(bitsx < bitsy) 
		bitsx=bitsy;

	bitsy = MaxBits(offx);
	if(bitsx < bitsy)
		bitsx=bitsy;
	bitsy=MaxBits(offy);
	if(bitsx<bitsy)
		bitsx=bitsy;

	{//WriteRect
		stream.WriteBits(bitsx,5);
		stream.WriteBits(offx,bitsx);
		stream.WriteBits(xmax,bitsx);
		stream.WriteBits(offy,bitsx);
		stream.WriteBits(ymax,bitsx);
		stream.FlushWriteBits();
	}
	//Fills
	{
		int mscax = 0x140000 * scalex;
		int mscay = 0x140000 * scaley;
		stream.WriteByte((XS8)1);
		stream.WriteByte((XS8)0x42);//Bitmap Fill
		stream.WriteWord((XU16)imageID);
		stream.WriteBits(1,1);//Scale
		stream.WriteBits(22,5);
		stream.WriteBits(mscax,22);
		stream.WriteBits(mscay,22);
		stream.WriteBits(0,1);//No Rotate
		stream.WriteBits(bitsx,5);
		stream.WriteBits(offx,bitsx);
		stream.WriteBits(offy,bitsx);
		stream.FlushWriteBits();
	}
	{
		stream.WriteByte((XS8)0);//Lines
	}
	{
		stream.WriteBits(2,4);
		stream.WriteBits(0,4);
		stream.FlushWriteBits();
	}
	{
		bitsx++;
		int nDataBits=bitsx-2;
		if(nDataBits<1) nDataBits=1;
		stream.WriteBits(0,1);
		int flags=eflagsMoveTo|eflagsFill0;
		stream.WriteBits(flags,5);//MoveTo(0,0)
		stream.WriteBits(nDataBits,5);
		stream.WriteBits(offx,nDataBits);
		stream.WriteBits(offy,nDataBits);
		stream.WriteBits(1,2);//Fill0
		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(0,1);//Hoz
		stream.WriteBits(xmax,bitsx); //LineTo(maxx,0)


		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(1,1);//Hoz
		stream.WriteBits(ymax,bitsx); //LineTo(maxx,maxy)

		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(0,1);//Hoz
		stream.WriteBits(-xmax,bitsx); //LineTo(0,maxy)

		stream.WriteBits(1,1);//Edge
		stream.WriteBits(1,1);//Line
		stream.WriteBits(nDataBits,4);
		stream.WriteBits(0,1);//Hoz or Ver
		stream.WriteBits(1,1);//Hoz
		stream.WriteBits(-ymax,bitsx); //LineTo(0,0)

		stream.WriteBits(0,1);//Edge
		stream.WriteBits(0,5);//End
		stream.FlushWriteBits();
	}


	int nPacksize=stream.GetDataSize(),nPackID=stagDefineShape<<6;
	if(nPacksize>=63)
	{
		nPackID|=0x3f;		
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
		data.Add((nPacksize)&0xff);
		data.Add((nPacksize>>8)&0xff);
		data.Add((nPacksize>>16)&0xff);
		data.Add((nPacksize>>24)&0xff);
	}
	else
	{
		nPackID|=nPacksize;
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
	}
	data.Append(stream.GetData(),stream.GetDataSize());
	return;
}

void CSWFEditDlg::MakeShape(XU8Array&data,int tag,int typeID,SRECT&rect,XStream&inData)
{
	XStream shapeData;
	shapeData.WriteWord((XS16)tag);

	SRECT drect;
	XU16 nDTag=0;
	inData.ReadWord(nDTag);
	{//Read RECT
		XU32 nBits=0;
		inData.ReadBits(nBits,5);
		inData.ReadBits(drect.xmin,nBits);
		inData.ReadBits(drect.xmax,nBits);
		inData.ReadBits(drect.ymin,nBits);
		inData.ReadBits(drect.ymax,nBits);
		inData.FlushReadBits();
	}
	{//Write RECT
		int xmax=rect.xmin+(drect.xmax-drect.xmin);
		int ymax=rect.ymin+(drect.ymax-drect.ymin);
		int bits=MaxBits(xmax),bits1=MaxBits(ymax);
		int offx=rect.xmin;
		int offy=rect.ymin;
		if(bits<bits1) bits=bits1;
		bits1=MaxBits(offx);
		if(bits<bits1) bits=bits1;
		bits1=MaxBits(offy);
		if(bits<bits1) bits=bits1;
		shapeData.WriteBits(bits,5);
		shapeData.WriteBits(offx,bits);
		shapeData.WriteBits(xmax,bits);
		shapeData.WriteBits(offy,bits);
		shapeData.WriteBits(ymax,bits);
		shapeData.FlushWriteBits();
	}
	{//Write Data
		shapeData.WriteData(inData.ReadAddr(),inData.GetDataSize());
	}
	int nPacksize=shapeData.GetDataSize(),nPackID=typeID<<6;
	if(nPacksize>=63)
	{
		nPackID|=0x3f;		
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
		data.Add((nPacksize)&0xff);
		data.Add((nPacksize>>8)&0xff);
		data.Add((nPacksize>>16)&0xff);
		data.Add((nPacksize>>24)&0xff);
	}
	else
	{
		nPackID|=nPacksize;
		data.Add(nPackID&0xff);
		data.Add(nPackID>>8);
	}
	data.Append(shapeData.GetData(),shapeData.GetDataSize());
}

void CSWFEditDlg::OnBnClickedSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pReplaceSWF==NULL) return;
	HTREEITEM item=m_ReplaceTree.GetSelectedItem();
	XReplaceInfo*info=item==NULL?NULL:(XReplaceInfo*)m_ReplaceTree.GetItemData(item);
	if(info==NULL)//||info->ch->type!=bitsChar)
		return;
	ScriptPlayer*player=m_pReplaceSWF->player;
	CFileDialog dlg( FALSE, _T("bmp"), "", 
                    OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
					OFN_OVERWRITEPROMPT,_T("Window图像|*.bmp|"));
    if( dlg . DoModal(  ) != IDOK ) return;
    CString strFile=dlg.GetPathName();
	XString8 s(strFile);
	XDrawDib dib;
	SCharacter*ch=info->ch;
	if(ch->type==bitsChar)
	{
		player->BuildBits(ch);
		if(!ch->bits.HasBits()) return;	
		dib.CreateFrom(&ch->bits,true);
	}
	else
	{
		XDrawDib*pDib=m_pReplaceSWF->bits.bitmap;

		XRect rect(0,0,pDib->Width(),pDib->Height());
		dib.Create(rect.Width(),rect.Height());
		dib.Copy(*pDib,0,0,rect);

	}
	int l=dib.m_nLinePitch;
	XU8*pData=new BYTE[l];
	XU8*pUp=dib.GetBuffer();
	XU8*pDown=pUp+l*(dib.Height()-1);
	while(pUp<pDown)
	{
		if(pUp!=pDown)
		{
			memcpy(pData,pUp,l);
			memcpy(pUp,pDown,l);
			memcpy(pDown,pData,l);
		}
		pUp+=l;
		pDown-=l;
	}
	delete pData;
	dib.SaveFile(s);

}

void CSWFEditDlg::OnBnClickedPackage()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pReplaceSWF==NULL) 
		return;
	ScriptPlayer*player=m_pReplaceSWF->player;
	if(player==NULL) 
		return;
	BYTE*pData=player->script;
	int len=player->scriptLen;
	BYTE*pRData=m_pRSWF?m_pRSWF->player->script:NULL;
	int len1=m_pRSWF?m_pRSWF->player->scriptLen:0;
	BYTE*pNewData=new BYTE[len*2+ 2048*1536*4];
	BYTE*pCopy=new BYTE[len*2];
	memcpy(pNewData,pData,len);
	XReplaceInfo*info=m_pInfo;
	int nTagID=m_nMaxID+1;
	while(info)
	{
		if(!info->strReplaceImage.IsEmpty())
		{
			XDrawDib dib;
			if(XSWFShow::LoadDib(dib,info->strReplaceImage))
			{
				int nLen0=0;
				int nTotal0=0;
				BYTE*pSrt=FindTag(pNewData+player->startPos,pNewData+len,info->ch->tag,nLen0,nTotal0);
				//XShapeParser parser(info->ch->splayer, info->ch->data, 0, &m_pESWF->display.camera.mat);
				//parser.GetStyles();
				if(pSrt!=NULL)
				{
					XBOOL bAlpha=XFALSE;
					RGB8*pBuf=(RGB8*)dib.GetBuffer();
					int size=dib.Width()*dib.Height();
					{
						for(int i=0;i<size;i++)
						{
							int a=pBuf[i].alpha;
							switch(a)
							{
							case 0xff:
								break;
							case 0:
								bAlpha=XTRUE;
								pBuf[i].red=0;
								pBuf[i].green=0;
								pBuf[i].blue=0;
								break;
							default:
								bAlpha=XTRUE;
								pBuf[i].red=(pBuf[i].red*a)>>8;
								pBuf[i].green=(pBuf[i].green*a)>>8;
								pBuf[i].blue=(pBuf[i].blue*a)>>8;
								break;
							}
						}
					}
					
					XU8Array data;
					if(!IsShape(info->ch))
					{
						if(bAlpha)
							PackageLossLess2(dib,data,info->ch->tag);
						else
							PackageLossLess(dib,data,info->ch->tag);
						int nTotal1=data.GetSize();
						int nCopyLen=len-((pSrt-pNewData)+nTotal0);
						memcpy(pCopy,pSrt+nTotal0,nCopyLen);
						memcpy(pSrt+nTotal1,pCopy,nCopyLen);
						memcpy(pSrt,data.GetData(),nTotal1);
						len+=(nTotal1-nTotal0);
					}
					else
					{
						if(bAlpha)
							PackageLossLess2(dib,data,nTagID);
						else
							PackageLossLess(dib,data,nTagID);
						XU8Array shapeData;
						//MakeImageShape(shapeData,dib,info->ch->tag,nTagID,
						//	info->ch->rbounds.xmin,info->ch->rbounds.ymin);
						MakeImageShape(shapeData, dib, nTagID, info );
						int nTotal1=data.GetSize();

						int nCopyLen=len-((pSrt-pNewData)+nTotal0);
						memcpy(pCopy,pSrt+nTotal0,nCopyLen);
						memcpy(pSrt+nTotal1+shapeData.GetSize(),pCopy,nCopyLen);
						memcpy(pSrt,data.GetData(),nTotal1);
						memcpy(pSrt+nTotal1,shapeData.GetData(),shapeData.GetSize());
						len-=nTotal0;
						len+=nTotal1;
						len+=shapeData.GetSize();

					}
				}
			}
		}
		else if(info->replacech && pRData )
		{
			int nLen0=0,nLen1=0,nTotal0=0,nTotal1;
			BYTE*pSrt=FindTag(pNewData+player->startPos,pNewData+len,info->ch->tag,nLen0,nTotal0);
			BYTE*pRep=FindTag(pRData+m_pRSWF->player->startPos,pRData+len1,info->replacech->tag,nLen1,nTotal1);
			if(pSrt!=NULL&&pRep!=NULL)
			{
				int nCopyLen=len-((pSrt-pNewData)+nTotal0);
				memcpy(pCopy,pSrt+nTotal0,nCopyLen);
				memcpy(pSrt+nTotal1,pCopy,nCopyLen);
				memcpy(pSrt,pRep,nTotal1);
				pSrt+=(nTotal1-nLen1);
				pSrt[0]=info->ch->tag&0xff;
				pSrt[1]=(info->ch->tag>>8)&0xff;
				len+=(nTotal1-nTotal0);
			}
		}
		info=info->next;
	}
	delete pCopy;
	pNewData[4]=(len&0xff);
	pNewData[5]=((len>>8)&0xff);
	pNewData[6]=((len>>16)&0xff);
	pNewData[7]=((len>>24)&0xff);
	CString strFile;
	char*str=NULL;
	if(!player->m_url.IsNull())
		str=player->m_url.strTxt;
	else if(!player->m_loadUrl.IsNull())
		str=player->m_loadUrl.strTxt;
	if(str)
	{
		int ii,nc=strlen(str);
		int iDot=nc,iBegin=0;
		for(ii=nc-1;ii>=0;ii--)
		{
			if(str[ii]=='?')
				iDot=ii;
			else if(str[ii]=='/'||str[ii]=='\\')
			{
				iBegin=ii+1;
				break;
			}
		}
		strFile=CString(str+iBegin,iDot-iBegin);
	}
	CFileDialog dlg( FALSE, _T("swf"), strFile,  OFN_HIDEREADONLY | 
		OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT,_T("Flash文件|*.swf|"));
    if( dlg.DoModal( ) != IDOK ) 
		return;
    strFile=dlg.GetPathName();    
	CFile file;
	if(!file.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		AfxMessageBox(_T("不能打开文件!"));
		return;
	}
	pNewData[0]='C';
	file.Write(pNewData,player->headerLen);
	len -= player->headerLen;
	unsigned long gzlen = len * 3/2 + 32; // enough for growth plus zlib headers
	XU8 *gzdata = new XU8[gzlen];
	// Use zlib to compress the data
	compress2((XU8*)gzdata, (unsigned long*)&gzlen,
		pNewData+player->headerLen, len, 9);

	// Replace the byte array with the compressed data
	//m_byteArray.SetLength(0);
	//m_byteArray.WriteU32((U32)len);
	//m_byteArray.Write(gzdata, gzlen);
	file.Write(gzdata,gzlen);

	delete gzdata;
	delete pNewData;

	//file.Write(player->script,player->scriptLen);
	file.Close();
	return;
}




