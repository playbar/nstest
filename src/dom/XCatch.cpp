// XCatch.cpp: implementation of the XCatch class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XCatch.h"
#include "XClient.h"
#include "XDialogClear.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XCatch::XCatch():XConnect(XNULL)
{
	//LOGWHERE();
	int i = 0;
}

XCatch::~XCatch()
{
   Release();
}

XBOOL XCatch::Create(XPCTSTR strWorkPath)
{
   m_strWorkPath=strWorkPath;
   m_strShared=strWorkPath;

   m_strWorkPath+="/Catch";
   m_strShared +="/Shared";
   m_strWorkPath.ConvertToGB();
   m_strShared.ConvertToGB();

   if(XFileFinder::GetType(m_strWorkPath)==XFileFinder::XFOLDER)
	   return XTRUE;

   return XFile::CreateFolder(m_strWorkPath);
}

void XCatch::Release()
{
   if(m_file.IsValid())
   {
	   m_file.Close();
	   XFile::RemoveFile(m_strFile);
   }
   m_strFile.Empty();
   
}

void XCatch::DisConnect()
{
	if(m_file.IsValid())
		m_file.Close();
	XConnect::DisConnect();
}


void XCatch::EndCatch(XU32 nCode,XBOOL bSet)
{
   if(m_file.IsValid()) 
   {
	   
	   if(nCode!=XHTTP_STATUS_OK&&
		  nCode!=XHTTP_STATUS_NOT_MODIFIED)
	   {
		  m_file.Close();
		  XFile::RemoveFile(m_strFile);	   
	   }
	   else
	   {
		   m_file.Seek(0,XFile::XSEEK_SET);
		   m_file.Write("J",1);
	       m_file.Close();
		   //m_file.Seek(4,XFile::XSEEK_SET);
		   //nCode=XHTTP_STATUS_OK;
		   //m_file.Write(&nCode,4);
	   }//*/
	   
   }
   if(bSet) m_strFile.Empty();
}

void XCatch::ResetTime()
{
	XU8 s=XFile::XBINARY|XFile::XREAD|XFile::XWRITE;

	if(!m_file.Open(m_strFile,s)) return;
	XCHeader hd;
	m_file.Read(&hd,sizeof(hd));
	XTime time;
	time.SetCurrentTime();
	hd.nTime=time.GetTime();
	m_file.Seek(0,XFile::XSEEK_SET);
	m_file.Write(&hd,sizeof(hd));

	m_file.Close();
}

XBOOL XCatch::StartCatch(CONNECTINFO &req, RESPONSEINFO &info)
{
//	return XFALSE;
	if(req.URL.m_nProto>=XPROTO_FTP) return XFALSE;

	if(info.nMaxAge<=0)
	{
		if(IsStatic(info.type.m_nClass,info.type.m_nType))
		{
			int l=req.URL.m_strURL.ReverseFind('?');
			if(l<0||info.nLength>64000)
				info.nMaxAge=86400*4;
			else
				return XFALSE;
		}
		else
			return XFALSE;
	}

	MakeFile(req);
	XU8 s=XFile::XWRITE|XFile::XCREATE|XFile::XBINARY;
	if(info.nErrorCode==XHTTP_STATUS_NOT_MODIFIED)
		s=XFile::XAPPEND|XFile::XCREATE|XFile::XBINARY;


	if(!m_file.Open(m_strFile,s)) return XFALSE;

	XCHeader hd={{'F','C','T'},req.nMethod,info.nErrorCode,
				 info.nMaxAge,info.time.GetTime(),
				 info.type.m_nClass,info.type.m_nCharset,
				 info.type.m_nType};
	if(hd.nErrorCode==XHTTP_STATUS_NOT_MODIFIED)
	{
		hd.nErrorCode=XHTTP_STATUS_OK;
		m_file.Seek(0,XFile::XSEEK_SET);
	}
	else if(hd.nErrorCode!=XHTTP_STATUS_OK) 
		hd.nMaxAge=0;

	//hd.nErrorCode=XHTTP_STATUS_CREATED;

	m_file.Write(&hd,sizeof(hd));
	
	XU8 v=(XU8)info.strCookies.GetSize();
	if(v>10) v=10;
	m_file.Write(&v,sizeof(v));
	XU16 i;
	for(i=0;i<v;i++)
		m_file.WriteString(info.strCookies[i]);
	m_file.WriteString(info.strConfigURL);
	m_file.WriteString(info.strFilterURL);
	v=info.strResHeaders.GetSize();
	m_file.Write(&v,sizeof(v));
	for(i=0;i<v;i++)
		m_file.WriteString(info.strResHeaders[i]);
	//for(XU32 i=0;i<info.strCookies.GetSize();i++)
	//	m_file.WriteString(info.strCookies[i]);
	return XTRUE;
}

XBOOL XCatch::RemoveCatch(CONNECTREQ &req, XBOOL bForce)
{
	if(req.URL.m_nProto>=XPROTO_FILE) return XFALSE;
	CONNECTINFO r;
	r.URL=req.URL;
	r.outData.Append(req.data);
	r.nMethod=req.nMethod;
	MakeFile(r);
	return RemoveFile(m_strFile,bForce);
}

void XCatch::MakeFile(CONNECTINFO&req)
{
   XU32 i;
   XU8Array data;
   data.Append((XU8*)req.URL.m_strURL.GetData(),req.URL.m_strURL.GetLength());
   data.Add(req.nMethod);
   if(req.nMethod==XEM::POST&&req.outData.GetSize()>0)
	   data.Append(req.outData);
   MD5 md5;
   char buf[16];
   md5.MakeMD5((char*)data.GetData(),data.GetSize(),buf);
   XString8Array list;
   list.SplitString(req.URL.m_strHost,'.');
   m_strFile=m_strWorkPath;
   m_strFile+='/';

   for(i=0;i<list.GetSize();i++)
	   m_strFile+=list[i];
   XFile::CreateFolder(m_strFile);
	m_strFile+='/';
   for(i=0;i<16;i+=4)
   {
	  XU32 v=*((XU32*)&buf[i]);
	  while(v)
	  {		  
		  int m=v%63;
		  if(m<10) m_strFile+=(char)(m+'0');
		  else if(m<36) m_strFile+=(char)(m+'a'-10);
		  else m_strFile+=(char)(m+'A'-36);
		  v=v/63;
	  }
	  //m_strFile+=(char)(((buf[i]&0x7f)+'0'));
	  /*if((buf[i]&0xf)<10)
		  m_strFile+=('0'+(buf[i]&0xf));
	  else
		  m_strFile+=('A'+(buf[i]&0xf)-10);*/
   }
   int id=req.URL.m_strFile.Find('?');
   if(id<0) id=req.URL.m_strFile.Find('&');
   if(id<0) m_strFile+=req.URL.m_strFile;
   else	if(id>0)
   {
	   XString8 strTmp;
	   req.URL.m_strFile.Left(strTmp,id);
	   m_strFile+=strTmp;
   }
   m_strFile+='~';
  // LOGE("%s, %s %d, %s", __FILE__, __FUNCTION__, __LINE__, m_strFile );
}



XBOOL XCatch::Catch(void *pData, int nSize)
{
	if(!m_file.IsValid()) return XFALSE;

	m_file.Write(pData,nSize);
	return XTRUE;
}

XU8 XCatch::CatchDetect(CONNECTINFO &req,XTime&time,XU32&l, XU8 bForward,bool&bStatic)
{
	if(req.URL.m_nProto>=XPROTO_FTP) return XC_NONE;
	
	MakeFile(req);
	if(XFileFinder::GetType(m_strFile)!=XFileFinder::XFILE)
		return XC_NONE;
	return DetectFile(m_strFile,l,time,bForward,bStatic);

}

void XCatch::Begin()
{
	m_inData.SetSize(MAX_PACK,XFALSE);
}

int XCatch::Connect(XBOOL bRetry)
{
	GetResponseInfo()->Reset(XCHARSET_GB2312);
	if(bRetry)
    {
		if(m_file.Open(m_strFile,XFile::XREAD|XFile::XBINARY))
			return CWAIT_OK;
		else
			return CWAIT_ERROR;
	}
	else
		return CWAIT_TIMEOUT;
}

int XCatch::SendData(XBOOL bRetry)
{
	if(!m_file.IsValid()) return CWAIT_ERROR;
	int s=m_file.GetLength();
	if(s<=sizeof(XCHeader)) 
		return CWAIT_ERROR;
	RESPONSEINFO*p=GetResponseInfo();
	{
		XCHeader hd;
		m_file.Read(&hd,sizeof(hd));
		s-=sizeof(hd);
		p->nErrorCode=hd.nErrorCode;
		//p->nLength=s-sizeof(hd);
		p->type.SetType(hd.nClass,hd.nType,hd.nCharset);
		if(p->nErrorCode!=XHTTP_STATUS_OK) return XFALSE;
		XU8 v;//=info.strCookies.GetSize();
		m_file.Read(&v,sizeof(v));
		if(v>10) return XFALSE;
		s-=sizeof(v);
		//m_file.Read(&v,sizeof(v));
		p->strCookies.RemoveAll();
		XU16 i;
		for(i=0;i<v;i++)
		{
			XString8 str;
			s-=m_file.ReadString(str);
			p->strCookies.Add(str);
		}

		s-=m_file.ReadString(p->strConfigURL);
		s-=m_file.ReadString(p->strFilterURL);
		//v=info.strHeaders.GetSize();
		s-=sizeof(v);
		m_file.Read(&v,sizeof(v));
		p->strResHeaders.RemoveAll();
		for(i=0;i<v;i++)
		{
			XString8 str;
			s-=m_file.ReadString(str);
			p->strResHeaders.Add(str);
			//m_file.WriteString(info.strHeaders[i]);
		}

		p->nLength=s;
		Handle(XClient::STARTED);
	}	
	m_nStatus=XPROC_CONTENT;
	return CWAIT_OK;
}

int XCatch::GetData()
{
	if(!m_file.IsValid()) return CWAIT_ERROR;
	RESPONSEINFO*p=GetResponseInfo();
	m_inData.SetSize(MAX_PACK,XFALSE);
	int nSize=XMIN(p->nTotalSize-p->nLength,MAX_PACK);
	int l=m_file.Read(m_inData.GetData(),nSize);
	m_inData.SetSize(l);
	return CWAIT_OK;
}

#ifdef __APPLE__
int XCatch::Clear(XU8 bAll)
{
	int nCount=0,nTotalCount=0;
	XStackC<XString8> paths;
	ClearShared();
	paths.Push(m_strWorkPath);
	while(paths.GetSize()>0)
	{
		XString8 strPath;//=paths[paths.GetSize()-1];
		paths.Pop(strPath);
		//paths.RemoveAt(paths.GetSize()-1);
		XString8 strRoot=strPath;
		XFileFinder find;
		XU8 bEmpty=XTRUE;
		if(find.FindFirst(strPath))//) continue;
		{		
			while(XTRUE)
			{
				//XString8 strFile=strRoot;
				XString8 strFile=find.cFileName;
				if(find.IsDirectory()) 
				{
					if(strFile!="."&&
					   strFile!="..")
					{
						strFile=strRoot;
						strFile+='/';
						strFile+=find.cFileName;
						paths.Push(strFile);
					}
				}
				else
				{
					strFile=strRoot;
					strFile+="/";
					strFile+=find.cFileName;
					nTotalCount++;
					if(RemoveFile(strFile,bAll))
					{
						nCount++;
					}
					else bEmpty=XFALSE;
				}
				if(!find.FindNext()) break;
			}
			find.Close();
		}
		//if(bAll||bEmpty)
		//	{
		nTotalCount++;
		find.Close();
		if((bAll||bEmpty)&&strRoot!=m_strWorkPath) 
		{
			//strRoot+="\\";
			XFile::RemoveFolder(strRoot);
			nCount++;
		}
	}
	return nCount;
}

#endif


#ifdef __ANDROID__
int XCatch::Clear(XU8 bAll)
{
	int nCount=0;
	int nTotalCount=0;
	ClearShared();
	XStackC<XString8> paths;
	paths.Push(m_strWorkPath);
	while(paths.GetSize()>0)
	{
		XString8 strPath;//=paths[paths.GetSize()-1];
		paths.Pop(strPath);
		//paths.RemoveAt(paths.GetSize()-1);
		XString8 strRoot=strPath;

		XFileFinder find;
		XU8 bEmpty=XTRUE;
		if(find.FindFirst(strPath))//) continue;
		{
			while(XTRUE)
			{
				//XString8 strFile=strRoot;
				XString8 strFile=find.cFileName;
				if(find.IsDirectory())
				{
					if(strFile!="."&&
					   strFile!="..")
					{
						strFile=strRoot;
						strFile += "/";
						strFile+=find.cFileName;
						paths.Push(strFile);
					}
				}
				else
				{
					strFile=strRoot;
					strFile += "/";
					strFile+=find.cFileName;
					nTotalCount++;
					if(RemoveFile(strFile,bAll))
					{
						nCount++;
					}
					else bEmpty=XFALSE;
				}
				if(!find.FindNext())
					break;
			}
			find.Close();
		}
		nTotalCount++;
		find.Close();
		if((bAll||bEmpty)&&strRoot!=m_strWorkPath)
		{
			XFile::RemoveFolder(strRoot);
			nCount++;
		}
	}
	return nCount;
}
#endif

int XCatch::ClearShared()
{
	XString8 strPath=m_strShared;
#ifdef _WIN32
	strPath+="/";
#endif
	XFileFinder find;
	if(find.FindFirst(strPath))
	{
		while(XTRUE)
		{
			XString8 strFile=m_strShared;
			strFile+="/";
			strFile+=find.cFileName;
			XFile::RemoveFile(strFile);
			//RemoveFile(strFile,bAll);
			if(!find.FindNext())
                break;
		}
		find.Close();
	}
	return 0;
}

#ifdef __ANDROID__
#else
int XCatch::Clear(XDialogClear *pDlg, XU8 bAll)
{
	int nCount=0,nTotalCount=0;
	ClearShared();
	XStackC<XString8> paths;
	//XString8Array paths;
	paths.Push(m_strWorkPath);
	//paths.Add(m_strWorkPath);
	while(paths.GetSize()>0)
	{
		if(pDlg!=XNULL&&pDlg->NeedStop()) break;
		XString8 strPath;//=paths[paths.GetSize()-1];
		paths.Pop(strPath);
		//paths.RemoveAt(paths.GetSize()-1);
		XString8 strRoot=strPath;
		//strRoot+="\\";
#ifdef _WIN32
		strPath+="/";
#endif
		XFileFinder find;
		XU8 bEmpty=XTRUE;
		if(find.FindFirst(strPath))//) continue;
		{		
			while(XTRUE)
			{
				if(pDlg!=XNULL&&pDlg->NeedStop()) break;
				//XString8 strFile=strRoot;
				XString8 strFile=find.cFileName;
				if(find.IsDirectory()) 
				{
					if(strFile!="."&&
					   strFile!="..")
					{
						strFile=strRoot;
						strFile+='/';
						strFile+=find.cFileName;
						paths.Push(strFile);
					}
				}
				else
				{
					strFile=strRoot;
					strFile+="/";
					strFile+=find.cFileName;
					nTotalCount++;
					if(pDlg!=XNULL)
						pDlg->SetProcCount(nTotalCount);
					if(RemoveFile(strFile,bAll))
					{
						nCount++;
						if(pDlg!=XNULL) pDlg->SetCount(nCount);
					}
					else bEmpty=XFALSE;
				}
				if(!find.FindNext()) break;
			}
			find.Close();
		}
		//if(bAll||bEmpty)
		//	{
		nTotalCount++;
		if(pDlg!=XNULL)
			pDlg->SetProcCount(nTotalCount);
		find.Close();
		if((bAll||bEmpty)&&strRoot!=m_strWorkPath) 
		{
			//strRoot+="\\";
			XFile::RemoveFolder(strRoot);
			nCount++;
			if(pDlg!=XNULL) pDlg->SetCount(nCount);
		}
	}
	if(pDlg!=XNULL) pDlg->EndDialog(XIDCANCEL);
	return nCount;
}
#endif

XBOOL XCatch::IsStatic(int nClass,int nType)
{
	XBOOL bStatic=false;
	switch(nClass)
		{
		case XCNT::CIMAGE:
		case XCNT::CVIDEO:
		case XCNT::CAUDIO:
			 bStatic=true;
			 break;
		case XCNT::CTEXT:
			switch(nType)
			{
			case XCNT::TPLAIN:
				 bStatic=true;
				 break;
			}break;
		case XCNT::CAPPLICATION:
			switch(nType)
			{
			case XCNT::TGIF:
			case XCNT::TJPEG:
			case XCNT::TPNG:
			case XCNT::TSWF:
			case XCNT::T3GP:
			case XCNT::TTIFF:
			case XCNT::TX_SHOCK_WAVE_FLASH:
			case XCNT::TOCTET_STREAM:
				 bStatic=true;
			}break;
		}
	return bStatic;
}

XU8 XCatch::DetectFile(XPCTSTR strFile,XU32&l,XTime&time,XU8 bForward,bool&bStatic)
{
	//XTime time;
	{
		l=0;
		if(!m_file.Open(strFile,XFile::XREAD|XFile::XBINARY))
		{
			XFile::RemoveFile(strFile);
			return XC_REMOVE;
		}
		XCHeader hd;
		if(m_file.Read(&hd,sizeof(hd))!=sizeof(hd)||
		   hd.strMark[0]!='J'||
		   hd.strMark[1]!='C'||
		   hd.strMark[2]!='T')
		{
			m_file.Close();
			XFile::RemoveFile(strFile);
			return XC_REMOVE;
		}
		l=m_file.GetLength();
		time=hd.nTime;
		m_file.Close();
		
		bStatic=IsStatic(hd.nClass,hd.nType);

		if(bForward==XCT_CONNECT)
		{
			if(hd.nErrorCode!=XHTTP_STATUS_OK)
			{
				XFile::RemoveFile(strFile);
				return XC_REMOVE;
			}
			XTime t(hd.nTime);
			//XString8 str;
			//t.GetTime(str);
			t+=hd.nMaxAge;
			XTime t1;
			t1.SetCurrentTime();
			
			//t.GetTime(str);
			//t1.GetTime(str);
			if(t<t1) return XC_OUTOFAGE;
		}
	}
	return XC_OK;
}

XBOOL XCatch::RemoveFile(XPCTSTR strFile, XBOOL bForce)
{
	if(bForce) {XFile::RemoveFile(strFile);return XTRUE;}
	XU32 l=0;
	XTime time;
	bool bStatic=false;
	XU8 nType=DetectFile(strFile,l,time,XCT_CONNECT,bStatic);
	switch(nType)
	{
	case XC_REMOVE:
		 return XTRUE;
	case XC_OK:
	case XC_OUTOFAGE:
		 if(bForce||nType==XC_OUTOFAGE)
		 {
			 XFile::RemoveFile(strFile);
			 return XTRUE;
		 }break;
	}
	return XFALSE;
}

void XCatch::RemoveCatch(REQARRAY &list,XBOOL bForce)
{
	for(XU32 i=0;i<list.GetSize();i++)
		RemoveCatch(*list[i],bForce);
}

XBOOL XCatch::GetCatchData()
{
	RESPONSEINFO*p=GetResponseInfo();
	//GetResponseInfo()->Reset(XCHARSET_GB2312);
	p->Reset(XCHARSET_GB2312);
    if(!m_file.Open(m_strFile,XFile::XREAD|XFile::XBINARY)) return XFALSE;
	int s=m_file.GetLength();
	if(s>MAX_PACK) return XFALSE;
	
	{
		XCHeader hd;
		m_file.Read(&hd,sizeof(hd));		
		//p->nErrorCode=hd.nErrorCode;
		//p->nLength=s-sizeof(hd);
		s-=sizeof(hd);
		p->type.SetType(hd.nClass,hd.nType,hd.nCharset);
		if(hd.nErrorCode!=XHTTP_STATUS_OK)
		{
			m_file.Close();
			return XFALSE;
		}
		XU8 v;//=info.strCookies.GetSize();
		m_file.Read(&v,sizeof(v));
		if(v>10)
		{
			m_file.Close();
			return XFALSE;
		}
		//m_file.Read(&v,sizeof(v));
		XString8 str;
		for(XU16 i=0;i<v;i++)
		{
			s-=m_file.ReadString(str);
			p->strCookies.Add(str);
		}
		s-=m_file.ReadString(str);
		p->strConfigURL=str;
		s-=m_file.ReadString(str);
		p->strFilterURL=str;
		m_file.Read(&v,sizeof(v));
		s-=2;
		for(XU16 i=0;i<v;i++)
		{
			s-=m_file.ReadString(str);
			p->strResHeaders.Add(str);
		}
		p->nLength=s;
		//Handle(XClient::STARTED);
	}
	//int nSize=XMIN(p->nTotalSize-p->nLength,MAX_PACK);
	s=m_file.Read(p->recBuf.GetAddres(),s);
	if (s<=0)
	{
		m_file.Close();
		return XFALSE;
	}
	p->recBuf.Write(XNULL,s);
	p->nTotalSize=s;
	if(s>=p->nLength)
	{
		p->nErrorCode=XHTTP_STATUS_OK;
		m_file.Close();
	}
	//XThread::Sleep(XWAIT_MONMENT);
	//Handle(XClient::FINISHED);

	return XTRUE;

}







