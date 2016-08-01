// XConnectFile.cpp: implementation of the XConnectFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XConnectFile.h"
#include "XClient.h"
#include "XContentType.h"
#include "XWAPText.h"
#include "XDomItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
enum 
{
	TYPE_GIF=0,
	TYPE_PNG=1,
	TYPE_JPG=2,
	TYPE_BMP=6,
	TYPE_TXT=7,
	TYPE_SWF=19
};


XConnectFile::FILETYPE XConnectFile::_fileTypes[]=
{	{"gif","image",XCNT::CIMAGE,XCNT::TGIF},
	{"png","image",XCNT::CIMAGE,XCNT::TPNG},
	{"jpg","image",XCNT::CIMAGE,XCNT::TJPEG},
	{"jpeg","image",XCNT::CIMAGE,XCNT::TJPEG},
	{"TIF","image",XCNT::CIMAGE,XCNT::TTIFF},
	{"TIFF","image",XCNT::CIMAGE,XCNT::TTIFF},
	{"bmp","image",XCNT::CIMAGE,XCNT::TVND_WAP_WBMP},
	{"txt","text",XCNT::CTEXT,XCNT::TANY},
	{"c","text",XCNT::CTEXT,XCNT::TANY},
	{"cfg","text",XCNT::CTEXT,XCNT::TXCONFIG},
    {"paysdk-pid","text",XCNT::CTEXT,XCNT::TPAYSDKPID},
	{"flt","text",XCNT::CTEXT,XCNT::TXFILTER},
	{"h","text",XCNT::CTEXT,XCNT::TANY},
	{"cpp","text",XCNT::CTEXT,XCNT::TANY},
	{"wml","wml",XCNT::CTEXT,XCNT::TVND_WAP_WML},
	{"bwml","wml",XCNT::CAPPLICATION,XCNT::TVND_WAP_WMLC},
	{"wmlc","wml",XCNT::CAPPLICATION,XCNT::TVND_WAP_WMLC},
	{"htm","wml",XCNT::CTEXT,XCNT::THTML},
	{"html","wml",XCNT::CTEXT,XCNT::THTML},
	{"doc","word",XCNT::CAPPLICATION,XCNT::TMSWORD},
	{"rtf","word",XCNT::CAPPLICATION,XCNT::TRTF},
	{"pdf","pdf",XCNT::CAPPLICATION,XCNT::TPDF},
	{"swf","flash",XCNT::CAPPLICATION,XCNT::TX_SHOCK_WAVE_FLASH},
	//////////////////////////////////////////////////////////////
	{"bdt","flash",XCNT::CAPPLICATION,XCNT::TX_SHOCK_WAVE_FLASH},
	{"mpg","media",XCNT::CVIDEO,XCNT::TMPEG},
	{"mpeg","media",XCNT::CVIDEO,XCNT::TMPEG},
	{"mpe","media",XCNT::CVIDEO,XCNT::TMPEG},
	{"mpv2","media",XCNT::CVIDEO,XCNT::TMPEG2},
	{"mp2v","media",XCNT::CVIDEO,XCNT::TMPEG2},
	{"mov","media",XCNT::CVIDEO,XCNT::TQUICKTIME},
	{"avi","media",XCNT::CVIDEO,XCNT::TX_MSVIDEO},
	{"movie","media",XCNT::CVIDEO,XCNT::TX_SGI_MOVIE},
	{"vdo","media",XCNT::CVIDEO,XCNT::TVDO},
	{"viv","media",XCNT::CVIDEO,XCNT::TVIVO},
	{"3gp","media",XCNT::CVIDEO,XCNT::T3GP},
	{"rm","media",XCNT::CVIDEO,XCNT::TANY},
	{"wmv","media",XCNT::CVIDEO,XCNT::TANY},
	{"au","media",XCNT::CAUDIO,XCNT::TAU},
	{"au","media",XCNT::CAUDIO,XCNT::TAU},
	{"au","media",XCNT::CAUDIO,XCNT::TAU},
	{"mp2","media",XCNT::CAUDIO,XCNT::TX_MPEG2},
	{"mp3","media",XCNT::CAUDIO,XCNT::TX_MPEG3},
	{"mp4","media",XCNT::CAUDIO,XCNT::TX_MPEG4},
	{"mpa2","media",XCNT::CAUDIO,XCNT::TX_MPEG2},
	{"mp2a","media",XCNT::CAUDIO,XCNT::TX_MPEG2},
	{"mpa","media",XCNT::CAUDIO,XCNT::TX_MPEG},
	{"wav","media",XCNT::CAUDIO,XCNT::TWAV},
	{"rar","rar",XCNT::CAPPLICATION,XCNT::TRAR},
	{"tar","rar",XCNT::CAPPLICATION,XCNT::TX_TAR},
	{"gtar","rar",XCNT::CAPPLICATION,XCNT::TX_GTAR},
	{"zip","rar",XCNT::CAPPLICATION,XCNT::TZIP},
	{"xml","wml",XCNT::CTEXT,XCNT::TXML},
    {"dat","flash",XCNT::CAPPLICATION,XCNT::TX_SHOCK_WAVE_FLASH},
};


XConnectFile::XConnectFile(XClient*pClient):XConnect(pClient)
{
   m_bRoot=XTRUE;
   m_nType=XF_NONE;
//   m_nFileType=XCNT::CAPPLICATION;
}

XConnectFile::~XConnectFile()
{
}

void XConnectFile::DisConnect()
{
	switch(m_nType)
	{
	case XF_NONE:
		 break;
	case XF_FILE:
		 m_file.Close();
		 m_inData.RemoveAll(XTRUE);
		 break;
	case XF_FOLDER:
		 m_find.Close();
		 break;
	}
	XConnect::DisConnect();
}


int XConnectFile::Connect(XBOOL bRetry)
{
	
	if(m_nType==XF_NONE) 
	{
		Handle(XClient::CONNECTED);
		Handle(XClient::STARTED);
		return CWAIT_ERROR;
	}
	RESPONSEINFO*pInfo=GetResponseInfo();
	
	if(m_nType==XF_FILE)
	{		 
		 pInfo->Reset(XCHARSET_GB2312);
		 int id=GetFileType(m_strPath);
		 if(id<0)
		 {
			 pInfo->type.SetType(XCNT::CANY,XCNT::TANY);
			 //m_nFileType=XCNT::CAPPLICATION;
		 }
		 else
		 {
			 //m_nFileType=_fileTypes[id].nClass;
			 if(_fileTypes[id].nType==XCNT::TX_SHOCK_WAVE_FLASH)
			 {
				XString8 strConfig;
				GetConfigFile(m_strPath,strConfig);
				if(XFileFinder::GetType(strConfig)==XFileFinder::FILE)
				{
#ifndef __ANDROID__
					pInfo->strConfigURL.ConvertFrom(strConfig,XCHARSET_GB2312);
#endif
				}
			 }
			 pInfo->type.SetType(_fileTypes[id].nClass,_fileTypes[id].nType,XCHARSET_GB2312);
		 }
		 
		 //FILE* file=fopen(strPath,"rb");
		 SetErrorCode(XHTTP_STATUS_OK);
		 //LOGWHEREMSG( m_strPath.GetData() );
		 if(!m_file.Open(m_strPath,XFile::XREAD|XFile::XBINARY))
		 {
			 return CWAIT_ERROR;
		 }		 
		 pInfo->nLength=m_file.GetLength();//::ftell(file);	
		 Handle(XClient::CONNECTED);
		 return CWAIT_OK;
	}
	else
	{
		pInfo->type.Reset(XCNT::CTEXT,XCNT::TVND_WAP_WML,XCHARSET_GB2312);
	
		pInfo->nLength=BLOCKSIZE; 
		SetErrorCode(XHTTP_STATUS_OK);
		m_strTxt.Create(XFALSE);
		m_strTxt.AddToken(XDOM_BODY);
		m_strTxt.AddAttrib(XEAB::STYLE,"background:url(RES@BACKGROUND)");
		CONNECTINFO*pi=GetConnectInfo();
		m_strTxt.AddToken(XDOM_P);

		if(!m_bRoot)
		{
			XPCWSTR strRoot=XResource::LoadWString(XIDS_ROOTDIR);
			XPCWSTR strUp=XResource::LoadWString(XIDS_UPDIR);
			XString8 strr(strRoot);
			XString8 stru(strUp);
			strr.ConvertToGB();
			stru.ConvertToGB();

			AddPath(m_strTxt,"/",strr,XTRUE);
			AddPath(m_strTxt,"../",stru);
		}
		//XFileFinder find;
#ifdef __APPLE__
//#elif defined( __ANDROID__ )
#else
		int l=m_strPath.GetLength();
		if(l&&m_strPath[l-1]!='/')
			m_strPath+="/";
#endif
		m_find.FindFirst(m_strPath);
	}
	
	return CWAIT_OK;
}


void XConnectFile::Begin()
{
 	XURL*p=&GetConnectInfo()->URL;
#ifdef __APPLE__
	m_strPath.Empty();
#elif defined( __ANDROID__ )
	m_strPath.Empty();
#else
	m_strPath=p->m_strProtocol;
#endif
#if defined (__APPLE__) && defined (__HDLOCALNOCOPY)
    m_strPath+=p->m_strURL;
#else
	m_strPath+=p->m_strPath;
	m_bRoot=p->m_strPath=="/";
	m_strPath+=p->m_strFile;
	int id=m_strPath.Find('?');
	if(id>0)
	{
		m_strPath[id]=0;
	}
#endif
	//LOGWHEREMSG( m_strPath.GetData() );
	///strPath.ConvertToGB();
	XU8 nType=GetFileAttrib(m_strPath);
	m_strSubPath.Empty();
	m_nStatus=XPROC_CONTENT;
	switch(nType)
	{
	case XNONE:
		 m_nType=XF_NONE;
		 break;
		 //m_bRoot=XFALSE;
		 //LoadPath(strPath,XEnumHttpStatus::GetErrorString(XERR_NOT_EXIST));
		 //break;
	case XFILE:	
		 //m_nStatus=XPROC_HEADER;
	case XFOLDER:
		 if(!p->m_strFile.IsEmpty())
		 {
		   if(p->m_strFile.Find(XQUEST)<0&&
			  p->m_strFile.Find(XMULTI)<0)
		   {
			   if(nType==XFILE)
				   m_nType=XF_FILE;
			   else
			   {
				   m_strSubPath=p->m_strFile;
				   m_strSubPath+=XDIV;
				   m_bRoot=XFALSE;				   
				   m_nType=XF_FOLDER;

			   }
		   }
		   else
			   m_nType=XF_FOLDER;
			 //LoadPath(strPath); 
		 }
		 else
		 {
			 m_nType=XF_FOLDER;
			 //strPath+="*.*";
			 //LoadPath(strPath);
		 }
		 break;
	}
	m_strTxt.Empty();
	m_inData.SetSize(MAX_PACK,XFALSE);
}

//# MAX_PACK

int XConnectFile::GetData()
{
	switch(m_nType)
	{
	case XF_NONE:return CWAIT_ERROR;
	case XF_FILE:
		if(m_file.IsValid())
		{
			RESPONSEINFO*pInfo=GetResponseInfo();
			int l=pInfo->nLength-pInfo->nTotalSize;
			if(l<=0) return CWAIT_FINISHED;
			int nSize=l>MAX_PACK?MAX_PACK:l;
			m_inData.SetSize(nSize);
			m_file.Read(m_inData.GetData(),nSize);//==nSize) return CWAIT_ERROR;
			return l<(MAX_PACK)?CWAIT_FINISHED:CWAIT_OK;
		}
		else
		  return CWAIT_ERROR;
		break;
	case XF_FOLDER:
		//if(m_find.IsValid())
		{
			RESPONSEINFO*pInfo=GetResponseInfo();
			int nc=0,bFinished=!m_find.IsValid();
			while(m_find.IsValid())
			{
				if(m_find.IsDirectory())
				{
					if(strcmp(m_find.cFileName,".")!=0&&
					   strcmp(m_find.cFileName,"..")!=0)
						AddPath(m_strTxt,m_find.cFileName,m_find.cFileName);
				}
				else 
					AddFile(m_strTxt,m_find.cFileName,m_find.cFileName);
				
				//if(pInfo->nTotalSize+m_strTxt.GetLength()>pInfo->nLength)
				//	pInfo->nLength+=m_strTxt.GetLength()<<2;

				if(!m_find.FindNext()) {bFinished=XTRUE;break;}
				nc++;
				if(nc>50||m_strTxt.GetLength()>4096) break;
			}
			if(m_strTxt.GetLength())
			{
				if(bFinished)
				{
					m_strTxt.EndToken();
					m_strTxt.Final();
				}
				m_inData.SetSize(0);
				m_inData.Append((XU8*)m_strTxt.GetData(),m_strTxt.GetLength());
				pInfo->nTotalSize+=m_strTxt.GetLength();
				m_strTxt.Empty();
			}
			return bFinished?CWAIT_FINISHED:CWAIT_OK;
		}
		//else
		//return CWAIT_OK;
	}
	return CWAIT_TIMEOUT;
}

int XConnectFile::SendData(XBOOL bRetry)
{
	switch(m_nType)
	{
	case XF_NONE:
		 return CWAIT_ERROR;
	case XF_FOLDER:		 
	default:
		Handle(XClient::STARTED);
		return CWAIT_OK;
	}
	return CWAIT_TIMEOUT;
}


XU8 XConnectFile::GetFileAttrib(XString8&strPath)
{
	XFileFinder find;
#ifdef __APPLE__	
	if(find.FindPath(strPath)) 
	{	
		bool isDirectory = find.IsDirectory();
		find.Close();
		if (isDirectory)
			return XFOLDER;
		else 
			return XFILE;
		
#else
	if (find.FindFirst(strPath))
	{
	#ifdef __ANDROID__
		bool isDirectory = find.IsDirectory();
		find.Close();
		if (isDirectory)
			return XFOLDER;
		else
			return XFILE;
	#else
		find.Close();
		XString8 str=strPath;
		str+="/";
		if(!find.FindFirst(str))
			return XFILE;
		else
			return XFOLDER;
	#endif
#endif
	}
	else
	{
#ifdef __APPLE__
		return XNONE;
#else
		XString8 str=strPath;
		str+="/";
		if(find.FindFirst(str))
			return XFOLDER;
		return XNONE;
#endif
	}
}

void XConnectFile::AddPath(XWAPText &m_strTxt,XPCTSTR strPath,XPCTSTR strPathName,XBOOL bRoot)
{
	m_strTxt.AddToken(XDOM_DIV);
	m_strTxt.AddAttrib(XEAB::STYLE,"padding:1px");
	m_strTxt.AddToken(XDOM_IMG);
	m_strTxt.AddAttrib(XEAB::SRC,"res@folder");
	m_strTxt.EndToken();
	if(!bRoot)
	{
		XString8 str;
		str=m_strSubPath;
		str+=strPath;
		str+="/";
		m_strTxt.AddLink(str,strPathName);//,"res@folder");
	}
	else
		m_strTxt.AddLink(strPath,strPathName);//,"res@folder");
	m_strTxt.EndToken();
	//m_strTxt.AddBR();
}

void XConnectFile::AddFile(XWAPText &m_strTxt, XPCTSTR strFile, XPCTSTR strFileName)
{
	int id=GetFileType(XString8(strFile));
	if(_fileTypes[id].nType!=XCNT::TX_SHOCK_WAVE_FLASH) return;
	XString8 strIcon="res@";
	strIcon+=id>=0?_fileTypes[id].strIcon:"file";
	XString8 str=m_strSubPath;
	str+=strFile;
	m_strTxt.AddToken(XDOM_DIV);
	m_strTxt.AddAttrib(XEAB::STYLE,"padding:1px");
	m_strTxt.AddToken(XDOM_IMG);
	m_strTxt.AddAttrib(XEAB::SRC,strIcon);
	m_strTxt.EndToken();
	m_strTxt.AddLink(str,strFileName);//,strIcon);
	m_strTxt.EndToken();
	//m_strTxt.AddBR();

	/*m_strTxt+="<a href=\"";
	m_strTxt+=m_strSubPath;
	m_strTxt+=strFile;
	m_strTxt+="\"><img align=center src=\"res@";
	if(id>=0) m_strTxt+=_fileTypes[id].strIcon;
	else m_strTxt+="file";
	m_strTxt+="\"/>";
	m_strTxt+=strFileName;
	m_strTxt+="</a><br/>";*/


}

XINT XConnectFile::GetFileType(const XString8 &strFile)
{
	int id=strFile.ReverseFind('.');
	if(id<=0) return -1;
	XString8 strExtern;
	strFile.Mid(strExtern,id+1);
	for(int i=0;i<sizeof(_fileTypes)/sizeof(FILETYPE);i++)
		if(strExtern.Compare(_fileTypes[i].strExtern,XTRUE)==0) return i;
	return -1;
}

void XConnectFile::GetConfigFile(XString8 &strFile, XString8 &strConfig)
{
	strConfig=strFile;
	int id=strConfig.ReverseFind('.');
	if(id>=0) 
	{
		strConfig.SetSize(id+2);
		strConfig[id+1]=0;
	}
	strConfig+="cfg";
}

/*int XConnectFile::ProcHeader(XU8 *pData, XU32 nSize)
{
	m_nStatus=XPROC_CONTENT;
	if(m_nFileType==XCNT::CTEXT)
	{
		XU32 v=(((XU8)pData[0])<<16)|(((XU8)pData[1])<<8)|((XU8)pData[2]);
		switch(v)
		{
		case 0xefbbbf:
			 nSize-=3;
			 pData+=3;
			 //m_pClient->m_nCharset=XCHARSET_UTF8;
			 GetResponseInfo()->type.m_nCharset=XCHARSET_UTF8;
			 //m_pDom->m_nCharset=XCHARSET_UTF8;
			 break;
		}
		return 3;
	}
	return 0;
}*/

