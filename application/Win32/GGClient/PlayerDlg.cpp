// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "PlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog


CPlayerDlg::CPlayerDlg(XSWFPlayer*player,CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDlg::IDD, pParent)
{
	m_pPlayer=player;
	//{{AFX_DATA_INIT(CPlayerDlg)	
	//}}AFX_DATA_INIT
	//m_addBut.SetID(_T("IDI_ADD"));
	//m_delBut.SetID(_T("IDI_DEL"));
	m_nType=IDC_STRING;
}


void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerDlg)

	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FILELIST, m_list);
}


BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerDlg)	
	ON_EN_CHANGE(IDC_SOURCE, OnChangeSource)
	ON_BN_CLICKED(IDC_SEARCH,OnSearch)
	ON_BN_CLICKED(IDC_REPLACE,OnReplace)
	ON_BN_CLICKED(IDC_SAVE,OnSave)
	ON_BN_CLICKED(IDC_SAVEC,OnSaveCompress)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers
void CPlayerDlg::OnReplace()
{
	m_nType=GetCheckedRadioButton(IDC_DOUBLE,IDC_HEX);
	int iSelect=m_list.GetSelectionMark();
	if(iSelect<0) return;
	ScriptPlayer*player=(ScriptPlayer*)m_list.GetItemData(iSelect);
	if(player==NULL) return;
	XU8Array data;
	if(!GetTextData(IDC_SOURCE,data)||
	    data.GetSize()<=0)
	{
		AfxMessageBox(_T("请输入源数据!"));
		GetDlgItem(IDC_SOURCE)->SetFocus();
		return;
	}
	XU8Array destData;
	if(!GetTextData(IDC_DEST,destData))
	{
		AfxMessageBox(_T("请输入目标数据!"));
		GetDlgItem(IDC_DEST)->SetFocus();
		return;
	}
	if(destData.GetSize()!=data.GetSize())
	{
		AfxMessageBox(_T("要替换的长度必须和原内容相同!"));
		GetDlgItem(IDC_SOURCE)->SetFocus();
		return;
	}
	XU8*pData=player->script,*pSource=data.GetData(),*pReplace=destData.GetData();
	int len=player->scriptLen,slen=data.GetSize();
	CString strInfo;
	int i,iCount=0;
	for(i=0;i<len-slen;i++)
	{
		if(memcmp(pData+i,pSource,slen)==0)
		{//相同
			memcpy(pData+i,pReplace,slen);
			CString strInt;
			strInt.Format(_T("(%d)"),i);
			if(!strInfo.IsEmpty())
				strInfo+=_T(",");
			strInfo+=strInt;
			i+=slen-1;
			iCount++;
		}
	}
	CString strOut;
	strOut.Format(_T("一共替换%d处:"),iCount);
	strOut+=strInfo;
	SetDlgItemText(IDC_INFO,strOut);
}
void CPlayerDlg::OnSearch()
{
	m_nType=GetCheckedRadioButton(IDC_DOUBLE,IDC_HEX);
	int iSelect=m_list.GetSelectionMark();
	if(iSelect<0) return;
	ScriptPlayer*player=(ScriptPlayer*)m_list.GetItemData(iSelect);
	if(player==NULL) return;
	XU8Array data;
	if(!GetTextData(IDC_SOURCE,data)||
	    data.GetSize()<=0)
	{
		AfxMessageBox(_T("请输入源数据!"));
		GetDlgItem(IDC_SOURCE)->SetFocus();
		return;
	}
	XU8*pData=player->script,*pSource=data.GetData();
	int len=player->scriptLen,slen=data.GetSize();
	CString strInfo;
	int i,iCount=0;
	for(i=0;i<len-slen;i++)
	{
		if(memcmp(pData+i,pSource,slen)==0)
		{//相同
			CString strInt;
			strInt.Format(_T("(%d)"),i);
			if(!strInfo.IsEmpty())
				strInfo+=_T(",");
			strInfo+=strInt;
			i+=slen-1;
			iCount++;
		}
	}
	CString strOut;
	strOut.Format(_T("一共找到%d处:"),iCount);
	strOut+=strInfo;
	SetDlgItemText(IDC_INFO,strOut);
	
}
void CPlayerDlg::OnSave()
{
	m_nType=GetCheckedRadioButton(IDC_DOUBLE,IDC_HEX);
	int iSelect=m_list.GetSelectionMark();
	if(iSelect<0) return;
	ScriptPlayer*player=(ScriptPlayer*)m_list.GetItemData(iSelect);
	if(player==NULL) return;
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
			if(/*str[ii]=='.'||*/str[ii]=='?')
				iDot=ii;
			else if(str[ii]=='/'||str[ii]=='\\')
			{
				iBegin=ii+1;
				break;
			}
		}
		strFile=CString(str+iBegin,iDot-iBegin);
	}
	CFileDialog dlg( FALSE, _T("swf"), strFile, 
                    OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
					OFN_OVERWRITEPROMPT,_T("Flash文件|*.swf|"));
    if( dlg . DoModal(  ) != IDOK ) return;
    strFile=dlg.GetPathName();    
	CFile file;
	if(!file.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		AfxMessageBox(_T("不能打开文件!"));
		return;
	}
	player->script[0]='F';
	file.Write(player->script,player->scriptLen);
	file.Close();

}
void CPlayerDlg::OnSaveCompress()
{
	m_nType=GetCheckedRadioButton(IDC_DOUBLE,IDC_HEX);
	int iSelect=m_list.GetSelectionMark();
	if(iSelect<0) return;
	ScriptPlayer*player=(ScriptPlayer*)m_list.GetItemData(iSelect);
	if(player==NULL) return;
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
			if(/*str[ii]=='.'||*/str[ii]=='?')
				iDot=ii;
			else if(str[ii]=='/'||str[ii]=='\\')
			{
				iBegin=ii+1;
				break;
			}
		}
		strFile=CString(str+iBegin,iDot-iBegin);
	}
	CFileDialog dlg( FALSE, _T("swf"), strFile, 
                    OFN_HIDEREADONLY | OFN_PATHMUSTEXIST |
					OFN_OVERWRITEPROMPT,_T("Flash文件|*.swf|"));
    if( dlg . DoModal(  ) != IDOK ) return;
    strFile=dlg.GetPathName();    
	CFile file;
	if(!file.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		AfxMessageBox(_T("不能打开文件!"));
		return;
	}
	player->script[0]='C';
	file.Write(player->script,player->headerLen);

	int len = player->scriptLen-player->headerLen;
	
	unsigned long gzlen = len * 3/2 + 32; // enough for growth plus zlib headers
	
	XU8 *gzdata = new XU8[gzlen];

	// Use zlib to compress the data
	compress2((XU8*)gzdata, (unsigned long*)&gzlen,
		player->script+player->headerLen, len, 9);

	// Replace the byte array with the compressed data
	//m_byteArray.SetLength(0);
	//m_byteArray.WriteU32((U32)len);
	//m_byteArray.Write(gzdata, gzlen);
	file.Write(gzdata,gzlen);

	delete gzdata;

	//file.Write(player->script,player->scriptLen);
	file.Close();
}

void CPlayerDlg::OnChangeSource()
{
	m_nType=GetTextType();
	CheckRadioButton(IDC_DOUBLE,IDC_HEX,m_nType);
	XU8Array data;
	if(GetTextData(IDC_SOURCE,data))
	{
		CString str;
		int i,len=data.GetSize();
		for(i=0;i<len;i++)
		{
			XU8 c=data[i];
			int hi=(c>>4)&0xf;
			if(hi<10)
				str+=(char)('0'+hi);
			else
				str+=(char)('A'+hi-10);
			hi=(c&0xf);
			if(hi<10)
				str+=(char)('0'+hi);
			else
				str+=(char)('A'+hi-10);
		}
		SetDlgItemText(IDC_SRCBIN,str);
	}
}


BOOL CPlayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_list.InsertColumn(0,_T("文件名"),LVCFMT_LEFT,80);
	m_list.InsertColumn(1,_T("大小"),LVCFMT_LEFT,100);
	m_list.InsertColumn(2,_T("URL"),LVCFMT_LEFT,240);
	CheckRadioButton(IDC_DOUBLE,IDC_HEX,m_nType);
	if(m_pPlayer!=NULL)
	{
		XU32Array list;
		m_pPlayer->GetPlayerList(list);
		int i,nc=list.GetSize();
		for(i=0;i<nc;i++)
		{
			ScriptPlayer*player=(ScriptPlayer*)list[i];
			if(!player->m_url.IsNull())
			{
				/*CString s(player->m_url.strTxt);
				int iDot=s.ReverseFind('.');
				if(iDot<=0)
					iDot=s.ReverseFind('?');
				if(iDot<=0)
					iDot=*/
				char*str=player->m_url.strTxt;
				int ii,nc=strlen(str);
				int iDot=nc,iBegin=0;
				for(ii=nc-1;ii>=0;ii--)
				{
					if(/*str[ii]=='.'||*/str[ii]=='?')
						iDot=ii;
					else if(str[ii]=='/'||str[ii]=='\\')
					{
						iBegin=ii+1;
						break;
					}
				}
				CString ss(str+iBegin,iDot-iBegin);
				m_list.InsertItem(i,ss);
				CString url(str);
				m_list.SetItemText(i,2,url);
			}
			else
			{
				m_list.InsertItem(i,_T(" "));
				if(!player->m_loadUrl.IsNull())
				{
					CString url(player->m_loadUrl.strTxt);
					m_list.SetItemText(i,2,url);
				}
			}
			CString strLen;
			strLen.Format(_T("%d"),player->scriptLen);
			m_list.SetItemText(i,1,strLen);
			m_list.SetItemData(i,(XU32)player);
			//m_list.
		}
	}
	m_list.SetSelectionMark(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




int CPlayerDlg::GetTextType(void)
{
	CString strInput;
	GetDlgItemText(IDC_SOURCE,strInput);
	if(strInput.IsEmpty())
		return IDC_STRING;
	int nType=0;//IDC_STRING;
	const unsigned long _nType[]={IDC_INT,IDC_DOUBLE,IDC_HEX,IDC_STRING};
	int i,nc=strInput.GetLength();
	for(i=0;i<nc;i++)
	{
		int c=strInput[i];
		if(c=='+'||c=='-')
			continue;
		else if(c=='.')
		{
			switch(nType)
			{
			case 0:
			case 1:
				nType=1;break;
			case 2:
			case 3:
				nType=3;break;
			}
		}
		else if((c>='a'&&c<='z')||c>='A'&&c<='Z')
		{
			if(nType<2)
				nType=2;
		}
		else if(c<'0'||c>'9')
		{
			nType=3;
		}
	}
	//m_nType=_nType[nType];
	return _nType[nType];
}

bool CPlayerDlg::GetTextData(int id,XU8Array&data)
{
	bool bOK=true;
	switch(m_nType)
	{
	case IDC_STRING:
		{
			CString strTxt;
			GetDlgItemText(id,strTxt);
			if(strTxt.IsEmpty())
				return false;
			XString8 s(strTxt);
			data.Append((XU8*)s.GetData(),s.GetLength());
		}break;
	case IDC_INT:
		{
			int v=GetDlgItemInt(id);
			XU8*pData=(XU8*)&v;
			data.Append(pData,4);
		}break;
	case IDC_DOUBLE:
		{
			CString strTxt;
			GetDlgItemText(id,strTxt);
			if(strTxt.IsEmpty())
				return false;
			XString8 s(strTxt);
			double v=atof((const char*)s.GetData());
			XU8*pData=(XU8*)&v;
			data.Append(pData,8);
		}break;
	case IDC_HEX:
		{
			CString strTxt;
			GetDlgItemText(id,strTxt);
			if(strTxt.IsEmpty())
				return false;
			XString8 s(strTxt);
			char*pData=s.GetData();
			while(*pData)
			{
				XU8 v=0;
				char c=pData[0];
				if(c>='0'&&c<='9')
					v=c-'0';
				else if(c>='a'&&c<='f')
					v=c-'a'+10;
				else if(c>='A'&&c<='F')
					v=c-'A'+10;
				else return false;
				c=pData[1];
				if(c)
				{
					v<<=4;
					if(c>='0'&&c<='9')
						v|=c-'0';
					else if(c>='a'&&c<='f')
						v|=c-'a'+10;
					else if(c>='A'&&c<='F')
						v|=c-'A'+10;
				}
				data.Add(v);
				pData+=2;
			}
		}break;
	}
	return bOK;
}
