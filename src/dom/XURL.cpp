// XURL.cpp: implementation of the XURL class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XURL.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
XPCTSTR XURL::_strProtocol[]={"http://","https://","WEBB|http://","webv://","ftp://","socket://","app://","ext://","win://","iapPay://","c:","res@","sys@"};
static XU16	   _uProtoPort[]={80,80,23};

XURL::XURL(XPCTSTR str)
{
  Reset();
  SetURL(str);
}

XURL::XURL(XString8& str)
{
  Reset();
  SetURL(str);
}

XURL::~XURL()
{

}



void XURL::Reset()
{
   m_nProto=XPROTO_HTTP;
   m_uPort=_uProtoPort[m_nProto];
   m_strHost.Empty();
   m_strFile.Empty();
   m_strPath=XDIV;
   m_strProtocol=_strProtocol[m_nProto];
   m_strURL.Empty();
   m_strUser.Empty();
   m_strPasswd.Empty();
   m_strCard.Empty();
   m_bJustCard=XFALSE;
}

void XURL::SetURL(XPCTSTR s)
{
	XU32 i;
   m_bJustCard=s[0]=='#';
   m_strCard.Empty();
   XString8 strURL(s);
   for( i=0;i<strURL.GetLength();i++)
   {
	   if(strURL[i]==XSP)
		   strURL[i]=XDIV;
   }

//#ifdef __ANDROID__	// zhengjl 2011.09.10
//#else
   strURL.ConvertToGB();
//#endif

   XString8 strPort;
   int id=strURL.Find(XID);
   XBOOL bMustHost=m_bJustCard?XFALSE:m_strHost.IsEmpty();
   XString8 strLeft;
   strURL.Left(strLeft,4);
   XString8 strLeftIAP;
   strURL.Left(strLeftIAP,9);
   if(strLeft.Compare(_strProtocol[XPROTO_SYSTEM],XTRUE)==0)
   {
	   m_nProto=XPROTO_SYSTEM;
	   m_strProtocol=strLeft;
	   id=strLeft.GetLength();
   }
   else if(strLeft.Compare(_strProtocol[XPROTO_RES],XTRUE)==0)
   {
	   m_nProto=XPROTO_RES;
	   m_strProtocol=strLeft;
	   id=strLeft.GetLength();
   }
   else if(strLeftIAP.Compare(_strProtocol[XPROTO_IAPPAY],XTRUE)==0)
   {
       m_nProto=XPROTO_IAPPAY;
	   m_strProtocol=strLeftIAP;
	   m_strURL = strURL;
       return;
   }
   else
   {
	   if(id>=0) //(&&id<4)//internet
	   {
		 
		 if(id>0) //ָ����Э��
		 {
			 bMustHost=XTRUE;
			 m_strPath=XDIV;
			 XString8 str;
			 strURL.Left(str,id+3);
			 m_strProtocol=str;
			 XBOOL bSet=XFALSE;
			 //���Ҷ�ӦЭ��
			 m_nProto=str.IndexOf(_strProtocol,sizeof(_strProtocol)/sizeof(XPCTSTR));

			 //δ֪Э�飬����HTTP
			 if(m_nProto<0)
			 {
				 m_nProto=XPROTO_HTTP;
				 m_strProtocol=_strProtocol[m_nProto];
			 }
			 else if(m_nProto==XPROTO_APP||
				     m_nProto==XPROTO_EXT||
					 m_nProto==XPROTO_WIN||
					 m_nProto == XPROTO_WEBB ||
                     m_nProto==XPROTO_WEBV)
			 {
				 m_strProtocol=_strProtocol[XPROTO_HTTP];
			 }
			 else if(m_nProto==XPROTO_FTP)
				 m_uPort=23;
             
             // sjj
             else if(m_nProto==XPROTO_HTTPS)
                 m_uPort=443;
			 
             else
				 m_uPort=80;
		 }
		 else if(m_nProto==XPROTO_FILE)
		 {//δָ����ʹ��ԭ��Э�飬ԭ��Ϊ�ļ�����ʹ��http
			 m_nProto=XPROTO_HTTP;
			 m_strProtocol=_strProtocol[m_nProto];
		 }
		 //m_strPath=XDIV;
		 id+=3;
	   }
	   else
	   {
		   id=strURL.Find(XCOLON);//File
		   if(id==1)
		   {
			   if(id>0)
			   {  //�����̷�
				  //m_strProtocol;
				  strURL.Left(m_strProtocol,id+1);
				  m_strPath=XDIV;

				  //m_nProto=XPROTO_FILE;
			   }
			   else if(m_nProto!=XPROTO_FILE)
			   { //û��ָ����ʹ��C:/
				 
				 m_strProtocol=_strProtocol[m_nProto];
			   }
			   id++;
			   m_nProto=XPROTO_FILE;
		   }
		   else if(m_nProto>=XPROTO_FILE) //û��Э�����η��
		   {
			   if(m_strURL.Compare("wap.",XFALSE,4)==0 ||  m_strURL.Compare("www.",XFALSE,4)==0)
			   {
				   m_nProto=XPROTO_HTTP;
				   m_strProtocol=_strProtocol[m_nProto];
				   bMustHost=XTRUE;
				   m_strPath=XDIV;
			   }
			   id=0;
		   } 
		   else
			   id=0;
		}
	   if(m_nProto<XPROTO_FILE&&id<(int)strURL.GetLength()&&bMustHost) //��������
	   {
		   int id1=strURL.Find(XDIV,id);
		   XString8 strHost;
		   if(id1>0)
		   {
			   strURL.Mid(strHost,id,id1-id);
			   id=id1;
		   }
		   else	if(bMustHost||m_strHost.IsEmpty())
		   {
			   id1=strURL.Find(XQUEST,id);
			   if(id1>0)
			   {
				 strURL.Mid(strHost,id,id1-id);
			     id=id1;
			   }
			   else
			   {
				strURL.Mid(strHost,id);
				id=strURL.GetLength();
			   }
		   }
		   if(!strHost.IsEmpty())
		   {   //�Ƿ��':'
			   id1=strHost.Find(XCOLON);
			   if(id1>0)
			   {
					 strHost.Mid(strPort,id1+1);
					 if(id1>0) strHost.Left(m_strHost,id1);
					 id1=strPort.Find(XCOLON);
					 if(id1>0)
					 {
						//m_uPort=strPort.Left(id).ToInt();
						 XString8 str;
						 strPort.Left(str,id);
						 m_uPort=str.ToInt();
						//m_strUser=
						strPort.Mid(m_strUser,id1+1);
						id1=m_strUser.Find(XCOLON);
						if(id1>0) 
						{
						  m_strUser.Mid(m_strPasswd,id1+1);
						  m_strUser.Left(m_strUser,id1);
						}
					}
					else
						m_uPort=strPort.ToInt();
				}
			   else m_strHost=strHost;
		   }
	   }
   }
   if(id<(int)strURL.GetLength())
   {
	  if(m_nProto>=XPROTO_RES)
	  {
		 ;
	  }
	  else
	  {
		  int id1=strURL.Find('?',id);
		  if(id1>0)
		  {
			  while(id1>id)
			  {
				  if(strURL[id1]=='/') break;
				  id1--;
			  }
		  }
		  else id1=strURL.ReverseFind(XDIV);
		  if(id1>0)//��Ŀ¼��Ϣ
		  {
			  if(id1<=id)
			  {
				  m_strPath=XDIV;
				  if(strURL.GetAt(id)==XDIV)
					  id++;
			  }
			  else 
			  {
				  if(strURL.GetAt(id)==XDIV)//���Ŀ¼
					strURL.Mid(m_strPath,id,id1+1-id);
				  else
				  {
					 XString8 strTmp;
					 //m_strPath+=
					 strURL.Mid(strTmp,id,id1+1-id);
					 m_strPath+=strTmp;
				  }
				  id=id1+1;
			  }
		  }
		  else if(strURL.GetAt(id)==XDIV) //��Ŀ¼
		  {
			  m_strPath=XDIV;
			  id++;
		  }
		  FormatPath();
	  }
   }

   if(id<(int)strURL.GetLength())//�����ļ���Ϣ
   {
	  if(strURL[id]=='#')
	  {
		XString8 str;
		strURL.Mid(str,id);
		m_strFile+=str;
	  }
	  else
		strURL.Mid(m_strFile,id);
   }
   else
	  m_strFile.Empty();
   id=m_strFile.Find('#');
   if(id>=0)
   {
	   m_strFile.Mid(m_strCard,id+1);
	   if(id>0) 
	   {
		   m_strFile.Delete(id,m_strFile.GetLength()-id);
		   //m_strFile.Left(m_strFile,id);
	   }
	   else m_strFile.Empty();
   }

   MakeHomeURL(m_strURL);
   if(m_nProto<XPROTO_RES)
	   m_strURL+=m_strPath;
   m_strURL+=m_strFile;

   //Encode(m_strURL);
}

void XURL::MakeURL()
{
	MakeHomeURL(m_strURL);
	if(m_nProto<XPROTO_RES)
	   m_strURL+=m_strPath;
    m_strURL+=m_strFile;
    if(m_nProto==XPROTO_FILE)
    {
	   for(int i=0;i<m_strURL.GetLength();i++)
		   if(m_strURL[i]==XDIV) m_strURL[i]=XSP;
    }
}

void XURL::MakeHomeURL(XString8 &strURL)
{
   strURL=m_strProtocol;
   if(m_nProto<XPROTO_FILE)
	   strURL+=m_strHost;
   if(m_nProto<=XPROTO_FTP&&m_uPort!=_uProtoPort[m_nProto])
   {
	 strURL+=XCOLON;
	 strURL+=m_uPort;
   }
}

void XURL::Encode(XString8 &str)
{
	XString8 str1(str.DataTo(),STRING_ATTACH);
	str.Empty();
	for(XU32 i=0;i<str1.GetLength();i++)
	{
		XTCHAR c=str1[i];
/*		switch(c)
		{
		case QSTAR:
			break;
		default:*/
			//if(c<'0'&&c!='.')//XSPACE)//Modify Mybe error!!!!
		switch(c)
		{
			case '.':
			case '_':
			case '!':
			case '*':
			case '(':
			case ')':
			case '\'':
			case '@':
				str+=c;
				break;
			default:
				if((c>='0'&&c<='9')||(c>='a'&&c<='z')||(c>='A'&&c<='Z'))
					str+=c;
				else //if()
				{
				   str+=XPEC;
				   XString8 strTmp;
				   strTmp.FromInt((XU8)c,XTRUE);
				   str+=strTmp.GetData()+2;
				}
				//str+=c;
				break;
		}
			//else str+=c;
			//break;
	}
}

void XURL::DecodeNum(XString8&str)
{
	int i,l=str.GetLength();
	XU8*pData=(XU8*)str.DataTo();
	str.Empty();
	for(i=0;i<l;i+=2)
	{
		XU8 c=pData[i];
		char iChar=0;
		if(c>='0'&&c<='9')
			iChar=(c-'0')<<4;
		else if(c>='a'&&c<='f')
			iChar=(10+(c-'a'))<<4;
		else if(c>='A'&&c<='F')
			iChar=(10+(c-'A'))<<4;
		c=pData[i+1];
		if(c>='0'&&c<='9')
			iChar|=(c-'0');
		else if(c>='a'&&c<='f')
			iChar|=(10+(c-'a'));
		else if(c>='A'&&c<='F')
			iChar|=(10+(c-'A'));
		str+=iChar;
	}
}

void XURL::Encode0(XString8 &str)
{
	int id=str.ReverseFind('?');
	if(id<=0) return;
	XString8 str1(str.DataTo(),STRING_ATTACH);
	str.Empty();
	for(XU32 i=0;i<str1.GetLength();i++)
	{
		XTCHAR c=str1[i];
		if(i<=id)
		{
			str+=c;
			continue;
		}
		switch(c)
		{
			case '&':
			case '=':
			case '.':
			case '_':
			case '!':
			case '*':
			case '(':
			case ')':
			case '\'':
			case '@':
				str+=c;
				break;
			default:
				if((c>='0'&&c<='9')||(c>='a'&&c<='z')||(c>='A'&&c<='Z'))
					str+=c;
				else //if()
				{
				   str+=XPEC;
				   XString8 strTmp;
				   strTmp.FromInt((XU8)c,XTRUE);
				   str+=strTmp.GetData()+2;
				}
				//str+=c;
				break;
		}
			//else str+=c;
			//break;
	}
}

void XURL::FormatPath()
{
	XString8Array dirs;
	dirs.SplitString(m_strPath,XDIV,XFALSE);
	if(dirs.GetSize()<=0) return;
	for(;;)
	{
		bool bSet=false;
		for(XINT nIndex=0;nIndex<dirs.GetSize();nIndex++)
		{
			if(dirs[nIndex]==XPTPT)
			{
				
				if(nIndex>0)
					dirs.RemoveAt(nIndex-1,2);
				else
					dirs.RemoveAt(nIndex,1);
				bSet=true;
				break;
			}
			else if(dirs[nIndex]==XSPT)
			{
				dirs.RemoveAt(nIndex);
				break;
			}
		}
		if(!bSet) break;
	}
	/*XINT nIndex=dirs.GetSize()-1;
	while(nIndex>=0)
	{
	   if(dirs[nIndex]==XPTPT)
	   {
		   if(nIndex>0)
			dirs.RemoveAt(nIndex-1,2);
		   else
			dirs.RemoveAt(nIndex);
		   nIndex--;
	   }
	   else if(dirs[nIndex]==XSPT)
		   dirs.RemoveAt(nIndex);
	   nIndex--;
	}*/
	m_strPath=XDIV;
	for(int nIndex=0;nIndex<(XINT)dirs.GetSize();nIndex++)
	{
		m_strPath+=dirs[nIndex];
		m_strPath+=XDIV;
	}
	//m_strPath+=XDIV;
}

static int NumOf(int hi)
{
	if(hi>='0'&&hi<='9')
				hi=hi-'0';
	else if(hi>='A'&&hi<='F')
	{
		hi=hi-'A'+10;
	}
	else if(hi>='a'&&hi<='f')
	{
		hi=hi-'a'+10;
	}
	else hi=0;
	return hi;
}

void XURL::Decode(XString8 &str)
{
	XString8 strTmp(str.DataTo(),STRING_ATTACH);	
	int i,nc=strTmp.GetLength(),id=0;
	str.SetSize(nc+1);
	for(i=0;i<nc;i++)
	{
		char c=strTmp[i];
		if(c=='%'&&i+2<nc)
		{
			int hi=strTmp[i+1];
			int lo=strTmp[i+2];
			hi=NumOf(hi);
			lo=NumOf(lo);
			str[id]=(hi<<4)+lo;
			i+=2;
		}
		else
		{
			str[id]=c;
		}
		id++;
	}
}
