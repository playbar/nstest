// XString.cpp: implementation of the XString8 class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include <math.h>
#include "XString.h"
#include "XStream.h"
#ifdef __APPLE__
#include "string_comp.h"
#include "MultiStringUtil.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void XString8::operator +=(XPCTSTR pData)
{
	if(pData==XNULL) return;
	XUINT nSize=strlen(pData);
	XUINT nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	memcpy(m_pData+nOldSize-1,pData,nSize);
}
void XString8::AddString(XPCTSTR pData,XUINT nSize)
{
	if(pData==XNULL) return;
	if(nSize==0) nSize=strlen(pData);
	XUINT nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	memcpy(m_pData+nOldSize-1,pData,nSize);
}

int XString8::Find(XTCHAR c,XUINT nBegin)
{
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
	XPCTSTR p=XSTRCHR(m_pData+nBegin,c);
	if(p==XNULL) return -1;
	return p-m_pData;
}
int XString8::Find(XPCTSTR pData,XUINT nBegin)
{
	if(pData==XNULL) return 0;
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
	XPCTSTR p=XSTRSTR(m_pData+nBegin,pData);
	if(p==XNULL) return -1;
	return p-m_pData;
}					 
void XString8::TrimLeft(char c)
{
	XUINT id=0;
	for(;id<m_nSize-1;id++)
	{
		if(c==0&&(XU8)m_pData[id]>XSPACE) break;
		else if(c!=0&&m_pData[id]!=c) break;
	}
	if(id>0) Delete(0,id);
}
void XString8::TrimRight(char c)
{
	XUINT nSize=m_nSize-2;
	for(int id=nSize;id>=0;id--)
	{
		if(c==0&&m_pData[id]>XSPACE) break;
		else if(c!=0&&m_pData[id]!=c) break;
		m_pData[id]=0;
		m_nSize--;
	}
}
void XString8::Delete(int id,int nCount)
{	RemoveAt(id,nCount); }

XU8 XString8::IsNum()
{
	if(IsEmpty()) return 0;
   	XPCTSTR pBuf=m_pData;
	int n=0;
    if(pBuf[1]=='x'||pBuf[1]=='X')
	{		
		n+=2;
		while(pBuf[n]!=0)
		{
			if(pBuf[n]>='0'&&pBuf[n]<='9') n++;
			else if(pBuf[n]>='a'&&pBuf[n]<='f')	n++;
			else if(pBuf[n]>='A'&&pBuf[n]<='F') n++;
			else return 0;			
		}
		return 1;
	}
	else
	{
		if(pBuf[0]=='+'||pBuf[0]=='-') n++;			
		int i=1;
		while(pBuf[n]!=0)
		{
			switch(pBuf[n])
			{
			case '.':n++;break;
			case 'E':
			case 'e':
			case 'D':
			case 'd':
				 n++;
				 if(pBuf[n]=='+'||
					pBuf[n]=='-') n++;
				 break;
			default:
				 if(pBuf[n]<'0'||pBuf[n]>'9') return 0;
				 n++;
			}
		}
		return i;
	}
	return 0;
//	for(XU32 i=0;i<m_nSize-1;i++)
//		if(m_pData[i]<'0'||m_pData[i]>'9') return XFALSE;
//	return XTRUE;
}
int XString8::IndexOf(XPCTSTR p[],XU32 nSize,int nDefault,XU8 nType)
{
	for(XU32 i=0;i<nSize;i++)
	{
		switch(nType)
		{
		default:
		case XLS_FULL:if(Compare(p[i],XFALSE)==0) return i;break;
		case XLS_FIRST:if(m_pData[0]==p[i][0]) return i;break;
		case XLS_NOCASE:if(Compare(p[i],XTRUE)==0) return i;break;
		}
	}
	return nDefault;
}

void XString8::FromInt(XINT val,XBOOL bHex)
{
	/*SetSize(MAXFORMATSIZE);
	if(!bHex)
		XSPRINTF(m_pData,"%d",v);
	else
		XSPRINTF(m_pData,"%x",v);*/
	char buf[16];
	
	int l=0,id=0;
	XBOOL s=0;
	if(!bHex)
	{
		int vv=val;
		if(vv<0) {s=1;vv=-vv;}
		if(!vv) {buf[0]='0';l++;}
		else
		{
			while(vv)
			{
				buf[l]='0'+(vv%10);
				vv=vv/10;
				l++;
			}
		}
		int len=l+1;
		if(s) len++;
		SetSize(len);
		if(s) {m_pData[id]='-';id++;}
	}
	else
	{
		
		XU32 vv=val;
		id=2;
		if(!vv) {buf[0]='0';l++;}
		else
		{
			while(vv)
			{
				char c=(char)(vv&0xf);
				if(c<10) buf[l]='0'+c;
				else	 buf[l]='a'+c-10;
				l++;
				vv=vv>>4;
			}
		}
		int len=l+3;
		SetSize(len);
		m_pData[0]='0';
		m_pData[1]='x';
	}

	while(l)
	{
		m_pData[id]=buf[l-1];
		l--;
		id++;
	}
	m_pData[id]=0;
	//SetSize(id+1);
	//FreeExtra();
}
//DEL void XString8::FromUInt(XU32 v,XBOOL bHex)
//DEL {
//DEL 	/*SetSize(MAXFORMATSIZE);
//DEL 	if(!bHex)
//DEL 		XSPRINTF(m_pData,"%d",v);
//DEL 	else
//DEL 		XSPRINTF(m_pData,"%x",v);
//DEL 	SetSize(strlen(m_pData)+1);
//DEL 	FreeExtra();*/
//DEL }

double XString8::ToFloat() 
{
	if(IsEmpty()) return 0;
	if(m_pData[1]=='x'||
	   m_pData[2]=='X')
	   return (double)ToInt(16);
	return XATOF(m_pData);
}

void XString8::Mid(XString8&str,int nIndex,XU32 nSize) const
{
	//int nSize=m_nSize-nIndex-1;
	if(nIndex+nSize>=m_nSize) {str.Empty();return;}
	if(nSize<=0) nSize=m_nSize-nIndex-1;
	//if(nSize<=0) return XString8("");
	str.SetString(m_pData+nIndex,nSize);
}

void XString8::Right(XString8&str,XU32 nSize)
{
	if(nSize<=0) str.Empty();
	else 
	{
		if(nSize>m_nSize-1) nSize=m_nSize-1;
		str.SetString(m_pData+m_nSize-nSize-1,nSize);
	}
}
void XString8::MakeFirstUpper()
{
	XBOOL bFirst=XTRUE;
	for(XUINT i=0;i<m_nSize-1;i++)
	{
		if(XISALPHA(m_pData[i]))
		{
			if(bFirst)
			{
				bFirst=XFALSE;
				m_pData[i]=XTOUPPER(m_pData[i]);
			}
		}
		else bFirst=XTRUE;
	}
}


XBOOL XString8::ConvertFrom(XU8 nCharset)
{
	switch(nCharset)
	{
	 case XCHARSET_US_ASCII:
	 case XCHARSET_GBK:
	 case XCHARSET_GB2314:
	 case XCHARSET_GB2312:
		  ConvertFrom(m_pData,nCharset,m_nSize-1);
		  return XTRUE;
	}
	return XFALSE;
}

static int NextDigit(double *value)
{
  int digit;
  digit = (int) *value;
  *value -= digit;
  *value *= 10;
  return digit;
}

static int RoundInt(double x)
{
	if (x < 0) {
		return (int) (x - 0.5);
	} else {
		return (int) (x + 0.5);
	}
}

static double PowerOfTen(int exponent)
{
	double base = 10.0;

	if (exponent < 0) {
		base = 0.1;
		exponent = -exponent;
	}

	double result = 1.0;

	while (exponent) {
		if (exponent & 1) {
			result *= base;
		}
		exponent >>= 1;
		base *= base;
	}

	return result;
}

#define LOG2_10 0.301029995663981

void XString8::FromFloat(XF64 value)
{
/*	SetSize(MAXFORMATSIZE);
	XF64 v=val>0?val:-val;		
	if(v>1e10||v<1e-6&&v!=0)
	  XSPRINTF(m_pData,"%e",val);
	else 
	{ 
		XSPRINTF(m_pData,"%6.4f",val);
		int nLen=strlen(m_pData)-1;
		while(nLen>0)
		{
		  if(m_pData[nLen]=='.') 
		  {
			  m_pData[nLen]=0;
			  break;
		  }
		  else if(m_pData[nLen]=='0') m_pData[nLen]=0;
		  else break;
		  nLen--;
		}
	}
	SetSize(strlen(m_pData)+1);
	FreeExtra();*/
	int i;
	int prec = 15; // base 10 precision

	char buffer[256];
	char *s = buffer;

	// Deal with negative numbers
	if (value < 0) {
		value = -value;
		*s++ = '-';
	}

	// Determine base 2 exponent
	int exp2;
	frexp(value, &exp2);
	exp2--;

	// Convert to base 10 exponent
	int exp10 = (int) RoundInt((double)exp2 * LOG2_10);

	// Obtain base 10 mantissa
	double pow10 = PowerOfTen(exp10);
	double f = value / pow10;

	// Adjust for round-off error on exponent
	if ((int) f == 0) {
		exp10--;
		pow10 = PowerOfTen(exp10);
		f = value / pow10;
	}

	// Sentinel is used for rounding
	char *sentinel = s;

	if (exp10 < 0 && exp10 > -6) {
		// Number is of form 0.######
		if (exp10 < -prec) {
			exp10 = -prec-1;
		}
		sentinel = s;
		*s++ = '0'; // Sentinel
		*s++ = '0';
		*s++ = '.';

		// Write out leading zeros
		for (i=exp10; i<-1; i++) {
			*s++ = '0';
		}
		// Write out significand
		for (i=0; i<prec; i++) {
			*s++ = NextDigit(&f) + '0';
		}
		exp10 = 0;
	} else if (exp10 >= prec) {
		*s++ = NextDigit(&f) + '0';
		*s++ = '.';
		for (i=0; i<prec-1; i++) {
			*s++ = NextDigit(&f) + '0';
		}
	} else {
		int digits = 0;
		sentinel = s;
		*s++ = '0';
		i = NextDigit(&f);
		if (i) {
			*s++ = i + '0';
		}
		while (exp10 > 0) {
			*s++ = NextDigit(&f) + '0';
			exp10--;
			digits++;
		}
		*s++ = '.';
		for (; digits < prec-1; digits++) {
			*s++ = NextDigit(&f) + '0';
		}
    }

	// Rounding
	i = NextDigit(&f);
	if (i > 4) {
		char *ptr = s-1;
		while (ptr >= buffer) {
			if (*ptr < '0') {
				ptr--;
				continue;
			}
			(*ptr)++;
			if (*ptr != 0x3A) {
				break;
			}
			*ptr-- = '0';
		}
    }

	/* Remove trailing zeros */
	while (*(s-1) == '0') {
		s--;
	}
	if (*(s-1) == '.') {
		s--;
	}

	if(exp10) {
		*s++ = 'e';
		XString8 stre;
		stre.FromInt(exp10);
	//	char *expstr = ConvertIntegerToString(exp10);
		char *t = stre;//expstr;
		while (*t) { *s++ = *t++; }
		//FreeStr(expstr);
	}
  
	*s = '\0';

	if (sentinel && sentinel[0] == '0' && sentinel[1] != '.') {
		char *s = sentinel;
		char *t = sentinel+1;
		while ((*s++ = *t++) != 0);
	}


	*this=buffer;
	//char *result = new char[strlen(buffer) + 1];
	//strcpy(result, buffer);
	//return result;
}

void XString8::AddWChar(XWCHAR c)//,XU8 nCharset)
{
	 if(c<0x80)
		 *this+=(XTCHAR)(c&0x7f);
	 else if(c<0x800)
	 {
		*this+=(XTCHAR)((c>>6)|0xc0);
		*this+=(XTCHAR)((c&0x3f)|0x80);
	 }
	 else
	 {
		 *this+=(XTCHAR)((c>>12)|0xe0);
		 *this+=(XTCHAR)(((c>>6)&0x3f)|0x80);
		 *this+=(XTCHAR)((c&0x3f)|0x80);
	 }
}

XBOOL XString8::ConvertFrom(XPCTSTR pStr,XU8 nCharset,int len)
{
	switch(nCharset)
	{
	default:
	case XCHARSET_UTF8:
	case XCHARSET_ISO_10646_UCS2:
		 SetString(pStr,len);
		 break;
	case XCHARSET_ISO_8859_1:
	case XCHARSET_US_ASCII:
	case XCHARSET_GBK:
	case XCHARSET_GB2314:
	case XCHARSET_GB2312:
		if(pStr==XNULL) Init();
		else
		{
			int iLen=XSTRLEN(pStr);
			if(len>0&&len<iLen) iLen=len;
			
#ifdef __APPLE__
			XU16* pData = gbkToUnicode(pStr, iLen);
#else
			XU16* pData=new XU16[iLen];
			iLen=MultiByteToWideChar(CP_ACP,XNULL,pStr,
									iLen,pData,iLen);
#endif
			Empty();
			for(int i=0;i<iLen;i++)
			{
				if(pData[i]==0) break;
				AddWChar(pData[i]);
			}
			delete pData;
			return XTRUE;
		}
		break;
	}
	return XFALSE;
}

//DEL XPCTSTR XString8::ConvertTo(XU8 nCharset)
//DEL {
//DEL 	switch(nCharset)
//DEL 	{
//DEL 	default:
//DEL 		break;
//DEL 	case XCHARSET_GB2312:
//DEL 		{
//DEL 			int nLen = m_nSize;
//DEL 			XWCHAR *lpwBuf = new XWCHAR[nLen];
//DEL 			memset(lpwBuf,0,nLen*2);
//DEL 			XTCHAR *lpDestBuf = new XTCHAR[nLen*2];
//DEL 			memset(lpDestBuf,0,nLen*2);
//DEL 
//DEL 			MultiByteToWideChar(CP_UTF8,0,m_pData,nLen,lpwBuf,nLen);
//DEL 			nLen=WideCharToMultiByte(CP_ACP,0,lpwBuf,nLen,lpDestBuf,nLen*2,XNULL,XNULL);
//DEL 
//DEL 			*this = XString8(lpDestBuf);
//DEL 
//DEL 			delete lpDestBuf;
//DEL 			delete lpwBuf;
//DEL 			break;
//DEL 		}
//DEL 	}
//DEL 	return m_pData;
//DEL }

void XString8::operator=(XPCWSTR pData)
{
	if(pData==XNULL) Add(0);
	else
	{
		Empty();
		XU32 nSize=XWCSLEN(pData);
		for(XU32 i=0;i<nSize;i++)
			AddWChar(pData[i]);
		/*SetSize((nSize<<1)+1);
		nSize=WideCharToMultiByte(CP_ACP,XNULL,pData,nSize,
				m_pData,nSize*2,XNULL,XNULL);
		if(nSize<m_nSize)
		{
			m_pData[nSize]=0;
			m_nSize=nSize+1;
			FreeExtra();
		}*/
		FreeExtra();
	}
}




void XString16::SetString(XPCWSTR pData,XINT nSize)
{
   if(pData==XNULL) Init();
   else if(nSize==STRING_ATTACH)
   {
	   m_pData=(XWCHAR*)pData;
	   m_nSize=m_nMaxSize=SafeStrlen(pData)+1;
   }
   else 
   {
	   int ml=SafeStrlen(pData);
	   if(nSize>ml||nSize<=0) nSize=ml;
	   //if(m_nSize>0) SetSize(m_nSize-1);
	   SetSize(0);
	   //if(nSize>0)
	   {
		   Append(pData,nSize+1);
		   m_pData[nSize]=0;
		   //Add(0);
	   }
	   //else
		//Append(pData,ml+1);
   }
}

XINT XString16::ToInt(char nDec,XU8 bPec)
{
	 XINT nValue=0;
	 XUINT j=0;
	 XU8 bSign=XFALSE,bStart=XFALSE;
	 for(;j<m_nSize;j++)
	 {
		  XTCHAR c=XTOLOWER(m_pData[j]);
		  if(c==0) break;
		  
		  if(c>='0'&&c<='9') 
		  {
			  bStart=XTRUE;
			  nValue=nValue*nDec;
			  nValue+=(c-'0');
		  }
		  else if(c=='x') {if(bStart) break;nDec=16;}
		  else if(c>='a'&&c<='f'&&nDec==16) 
		  {
			  bStart=XTRUE;
			  nValue=nValue*nDec;

			  nValue+=(c-'a'+10);
		  }
		  else if(c=='-') {if(bStart) break;bSign=XTRUE;}
		  else if(bPec&&c=='%'){bSign=XTRUE;if(bStart) break;}
		  else if(c=='.') break;
		  else if(bStart) break;
	 }
	 if(bSign) nValue=-nValue;
	 return nValue;
}


void XString16::FromInt(XINT val,XBOOL bHex)
{

	XWCHAR buf[16];
	
	int l=0,id=0;
	XBOOL s=0;
	if(!bHex)
	{
		int vv=val;
		if(vv<0) {s=1;vv=-vv;}
		if(!vv) {buf[0]='0';l++;}
		else
		{
			while(vv)
			{
				buf[l]='0'+(vv%10);
				vv=vv/10;
				l++;
			}
		}
		int len=l+1;
		if(s) len++;
		SetSize(len);
		if(s) {m_pData[id]='-';id++;}
	}
	else
	{
		
		XU32 vv=val;
		id=2;
		if(!vv) {buf[0]='0';l++;}
		else
		{
			while(vv)
			{
				XWCHAR c=(XWCHAR)(vv&0xf);
				if(c<10) buf[l]='0'+c;
				else	 buf[l]='a'+c-10;
				l++;
				vv=vv>>4;
			}
		}
		int len=l+2;
		SetSize(len);
		m_pData[0]='0';
		m_pData[1]='x';
	}

	while(l)
	{
		m_pData[id]=buf[l-1];
		l--;
		id++;
	}
	m_pData[id]=0;
}

int XString16::GBToUCS(const char* buf, XU16* dst, int nCount)
{
#ifdef __APPLE__
    // 没测试过，有可能有问题
	int len = strlen(buf);
	XU16* ptemp = (XU16*)gbkToUnicode(buf, len);
	int iLen = gg_wcslen(ptemp);
	memmove(dst, ptemp, iLen*2);
	delete[] ptemp;
	dst[iLen]=0;
	return iLen;
#else
	//memset(dst,nCount+1);
	int iLen=MultiByteToWideChar(CP_ACP,XNULL,buf,
								 nCount,dst,nCount);
	dst[iLen]=0;
	return iLen;
#endif
}

int XString16::UTF8ToUCS(const char*buf,XU16*dst,int nCount)
{
#ifdef __APPLE__
    XU16* ptemp = (XU16*)utf8ToUnicode(buf);
	int iLen = gg_wcslen(ptemp);
	memmove(dst, ptemp, iLen*2);
	delete[] ptemp;
	dst[iLen]=0;
	return iLen;
#else
	int iLen=MultiByteToWideChar(CP_UTF8,0,buf,nCount,dst,nCount);
	dst[iLen]=0;
	return iLen;
#endif
}

XBOOL XString16::ConvertFrom(XPCTSTR pStr,XU8 nCharset)
{
	switch(nCharset)
	{
	default:
		*this=pStr;
		break;
	case XCHARSET_ISO_8859_1:
	case XCHARSET_US_ASCII:
	case XCHARSET_GBK:
	case XCHARSET_GB2314:
	case XCHARSET_GB2312:
		if(pStr==XNULL) Init();
		else
		{
			
			
#ifdef __APPLE__
			int len = strlen(pStr);
			XU16* pData = gbkToUnicode(pStr, len);
#else
			XU32 iLen=XSTRLEN(pStr)+1;
			XU16* pData=new XU16[iLen];
			memset(pData,0,iLen*sizeof(XU16));
			iLen=MultiByteToWideChar(CP_ACP,XNULL,pStr,
									iLen,pData,iLen);
#endif
			*this=pData;
			delete pData;
			return XTRUE;
		}
		break;
	}
	return XFALSE;
}

void XString16::operator=(XPCTSTR pData)
{
	if(pData==XNULL) {Init();return;}
	int nLen=strlen(pData)+1;
	SetSize(nLen);
#ifdef __APPLE__
	XU16* tempData = utf8ToUnicode(pData);
	memmove(m_pData, tempData, (gg_wcslen(tempData) + 1) * 2);
	m_nSize=gg_wcslen(m_pData)+1;
	delete[] tempData;
#else
	MultiByteToWideChar(CP_UTF8,0,pData,nLen,m_pData,nLen);
	m_nSize=wcslen(m_pData)+1;
#endif
	
	FreeExtra();
}

int XString16::Find(XPCWSTR pData,XUINT nBegin)
{
	if(pData==XNULL) return 0;
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
#ifdef __APPLE__
	XPCWSTR p=gg_wcsstr(m_pData+nBegin,pData);
#else
	XPCWSTR p=wcsstr(m_pData+nBegin,pData);
#endif
	if(p==XNULL) return -1;
	return p-m_pData;
}

void XString8::SetString(XPCTSTR pData,int nSize)
{
   if(pData==XNULL) Init();
   else if(nSize==STRING_ATTACH)
   {
	   if(m_pData!=XNULL) delete m_pData;
	   m_pData=(XPTSTR)pData;
	   m_nSize=m_nMaxSize=XSTRLEN(pData)+1;
   }
   else 
   {
	   int ml=SafeStrlen(pData);
	   if(nSize>ml||nSize<=0) nSize=ml;
	   SetSize(nSize+1);//0);
	   //if(nSize!=0)
	   {
		   memcpy(m_pData,pData,nSize);
		   //Append(pData,nSize+1);
		   m_pData[nSize]=0;
		   //Add(0);
	   }
	   //else
		//Append(pData,SafeStrlen(pData)+1);
   }
 }

void XString8::ConvertToGB()
{
#ifdef __APPLE__
	XU16* lpwBuf = utf8ToUnicode(m_pData);	
	char* lpDestBuf = unicodeToGbk(lpwBuf);
#else
	int nLen = m_nSize;
	XWCHAR *lpwBuf = new XWCHAR[nLen];
	memset(lpwBuf,0,nLen*2);
	XTCHAR *lpDestBuf = new XTCHAR[nLen*2];
	memset(lpDestBuf,0,nLen*2);
	MultiByteToWideChar(CP_UTF8,0,m_pData,nLen,lpwBuf,nLen);
	nLen=WideCharToMultiByte(CP_ACP,0,lpwBuf,nLen,lpDestBuf,nLen*2,XNULL,XNULL);
#endif
	SetString(lpDestBuf,strlen(lpDestBuf));
	*this = XString8(lpDestBuf);
	delete[] lpDestBuf;
	delete[] lpwBuf;
}



double XString16::ToFloat() 
{
	if(IsEmpty()) return 0;
	if(m_pData[1]=='x'||
	   m_pData[2]=='X')
	   return (double)ToInt(16);
#ifdef __APPLE__
	XString8 str(*this);
	return str.ToFloat();
#else
	XWCHAR* str;
	return wcstod(m_pData,&str);
#endif
}


void XString16::Mid(XString16&str,int nIndex,XU32 nSize)
{
	//int nSize=m_nSize-nIndex-1;
	if(nIndex+nSize>=m_nSize) {str.Empty();return;}
	if(nSize<=0) nSize=m_nSize-nIndex-1;
	//if(nSize<=0) return XString8("");
	str.SetString(m_pData+nIndex,nSize);
}

void XString16::Right(XString16&str,XU32 nSize)const
{
	if(nSize<=0) str.Empty();
	else 
	{
		if(nSize>m_nSize-1) nSize=m_nSize-1;
		str.SetString(m_pData+m_nSize-nSize-1,nSize);
	}
}
int XString16::Find(XWCHAR c,XUINT nBegin)
{
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
#ifdef __APPLE__
	XPCWSTR p=gg_wcschr(m_pData+nBegin,c);
#else
	XPCWSTR p=wcschr(m_pData+nBegin,c);
#endif
	if(p==XNULL) return -1;
	return p-m_pData;
}	

void XString16::MakeFirstUpper()
{
	XBOOL bFirst=XTRUE;
	for(XUINT i=0;i<m_nSize-1;i++)
	{
		if(XISALPHA(m_pData[i]))
		{
			if(bFirst)
			{
				bFirst=XFALSE;
				m_pData[i]=XTOUPPER(m_pData[i]);
			}
		}
		else bFirst=XTRUE;
	}
}

int XString16::Compare(XPCWSTR pSrt,XPCWSTR pStr,XBOOL bLowcase,int nLen)
{
	if(pStr==XNULL) return 1;
#ifdef __APPLE__
	if(nLen>0)
		return bLowcase?wcsnicmp(pSrt,pStr,nLen):gg_wcsncmp(pSrt,pStr,nLen);
	else return bLowcase?wcsicmp(pSrt,pStr):gg_wcscmp(pSrt,pStr);
#else
	if(nLen>0)
		return bLowcase?wcsnicmp(pSrt,pStr,nLen):wcsncmp(pSrt,pStr,nLen);
	else return bLowcase?wcsicmp(pSrt,pStr):wcscmp(pSrt,pStr);
#endif
}
int XString16::Compare(XPCWSTR pStr,XBOOL bLowcase,int nLen)const
{
	if(pStr==XNULL) return 1;
#ifdef __APPLE__
	if(nLen>0)
		return bLowcase?wcsnicmp(m_pData,pStr,nLen):gg_wcsncmp(m_pData,pStr,nLen);
	else return bLowcase?wcsicmp(m_pData,pStr):gg_wcscmp(m_pData,pStr);
#else
	if(nLen>0)
		return bLowcase?wcsnicmp(m_pData,pStr,nLen):wcsncmp(m_pData,pStr,nLen);
	else return bLowcase?wcsicmp(m_pData,pStr):wcscmp(m_pData,pStr);
#endif
}

void XString16::operator=(XPCWSTR pData)
{
	if(pData==XNULL) {Init();return;}
	int nSize=SafeStrlen(pData);
	SetSize(nSize+1,XFALSE);
	memcpy(m_pData,pData,(nSize+1)*sizeof(XWCHAR));
	//XString16(pData,SafeStrlen(pData));
}

void XString16::operator +=(XPCWSTR pData)
{
	if(pData==XNULL) return;
	XUINT nSize=SafeStrlen(pData);
	XUINT nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	XCOPY(m_pData+nOldSize-1,pData,nSize*sizeof(XWCHAR));
}

//void XString16::Init(){SetSize(1);}


XBOOL XString16::IsNum()
{
	if(IsEmpty()) return 0;
   	XPCWSTR pBuf=m_pData;
	int n=0;
    if(pBuf[1]=='x'||pBuf[1]=='X')
	{		
		n+=2;
		while(pBuf[n]!=0)
		{
			if(pBuf[n]>='0'&&pBuf[n]<='9') n++;
			else if(pBuf[n]>='a'&&pBuf[n]<='f')	n++;
			else if(pBuf[n]>='A'&&pBuf[n]<='F') n++;
			else return 0;			
		}
		return 1;
	}
	else
	{
		if(pBuf[0]=='+'||pBuf[0]=='-') n++;			
		int i=1;
		while(pBuf[n]!=0)
		{
			switch(pBuf[n])
			{
			case '.':n++;break;
			case 'E':
			case 'e':
			case 'D':
			case 'd':
				 n++;
				 if(pBuf[n]=='+'||
					pBuf[n]=='-') n++;
				 break;
			default:
				 if(pBuf[n]<'0'||pBuf[n]>'9') return 0;
				 n++;
			}
		}
		return i;
	}
	return 0;
}

void XString8::FromFloat(float val)
{
	FromFloat((double)val);
//	XU32 vi=*(XU32*)&v;
/*	SetSize(MAXFORMATSIZE);
	XF64 v=val>0?val:-val;		
	if(v>1e10||v<1e-6&&v!=0)
	  XSPRINTF(m_pData,"%e",val);
	else 
	{ 
		XSPRINTF(m_pData,"%5.4f",val);
		int nLen=strlen(m_pData)-1;
		while(nLen>0)
		{
		  if(m_pData[nLen]=='.') 
		  {
			  m_pData[nLen]=0;
			  break;
		  }
		  else if(m_pData[nLen]=='0') m_pData[nLen]=0;
		  else break;
		  nLen--;
		}
	}
	SetSize(strlen(m_pData)+1);
	FreeExtra();*/
}

XINT XString8::ToInt(char nDec,XU8 bPec)
{
	 XINT nValue=0;
	 XUINT j=0;
	 XU8 bSign=XFALSE,bStart=XFALSE;
	 for(;j<m_nSize;j++)
	 {
		  XTCHAR c=XTOLOWER(m_pData[j]);
		  if(c==0) break;
		  
		  if(c>='0'&&c<='9') 
		  {
			  bStart=j;//XTRUE;
			  nValue=nValue*nDec;
			  nValue+=(c-'0');
		  }
		  else if(c=='x') {if(bStart) break;nDec=16;}
		  else if(c>='a'&&c<='f'&&nDec==16) 
		  {
			  bStart=XTRUE;
			  nValue=nValue*nDec;

			  nValue+=(c-'a'+10);
		  }
		  else if(c=='-') {if(bStart) break;bSign=XTRUE;}
		  else if(bPec&&c=='%'){bSign=XTRUE;if(bStart) break;}
		  else if(c=='.') break;
		  else if(bStart) break;
	 }
	 if(bSign) nValue=-nValue;
	 return nValue;
}

void XString8::ReadFromStream(XStream *pStream, int nLen)
{
	int i=0;
	XTCHAR nChar;
	if(nLen==0)
	{
	   while(pStream->ReadByte(nChar))
	   {
		   if(nChar==0) break;
		   *this+=(XTCHAR)nChar;
		   //text+=nChar;
	   }
	}
    for ( i=0; i < nLen; i++ )
	{
		if(!pStream->ReadByte(nChar)) break;
		*this+=(XTCHAR)nChar;
	}
}

XINT XString8::ToInt(XPCTSTR buf,int nSize,int nDec,XBOOL bPec)
{
	XINT nValue=0;
	int j=0;
	XU8 bSign=XFALSE,bStart=XFALSE;
	if(nSize<=0) nSize=SafeStrlen(buf);
	for(;j<nSize;j++)
	{
		XTCHAR c=XTOLOWER(buf[j]);
		if(c==0) break;
		
		if(c>='0'&&c<='9') 
		{
			bStart=j;//XTRUE;
			nValue=nValue*nDec;
			nValue+=(c-'0');
		}
		else if(c=='x') {if(bStart) break;nDec=16;}
		else if(c>='a'&&c<='f'&&nDec==16) 
		{
			bStart=XTRUE;
			nValue=nValue*nDec;
			
			nValue+=(c-'a'+10);
		}
		else if(c=='-') {if(bStart) break;bSign=XTRUE;}
		else if(bPec&&c=='%'){bSign=XTRUE;if(bStart) break;}
		else if(c=='.') break;
		else if(bStart) break;
	}
	if(bSign) nValue=-nValue;
	return nValue;
}

void XString8::FormatV(XPTSTR m_pData,int val,XBOOL bHex)
{
	char buf[16];
	
	int l=0,id=0;
	XBOOL s=0;
	if(!bHex)
	{
		int vv=val;
		if(vv<0) {s=1;vv=-vv;}
		if(!vv) {buf[0]='0';l++;}
		else
		{
			while(vv)
			{
				buf[l]='0'+(vv%10);
				vv=vv/10;
				l++;
			}
		}
		int len=l+1;
		if(s) len++;
		//		SetSize(len);
		if(s) {m_pData[id]='-';id++;}
	}
	else
	{
		
		XU32 vv=val;
		id=2;
		if(!vv) {buf[0]='0';l++;}
		else
		{
			while(vv)
			{
				char c=(char)(vv&0xf);
				if(c<10) buf[l]='0'+c;
				else	 buf[l]='a'+c-10;
				l++;
				vv=vv>>4;
			}
		}
		int len=l+3;
		//		SetSize(len);
		m_pData[0]='0';
		m_pData[1]='x';
	}
	
	while(l)
	{
		m_pData[id]=buf[l-1];
		l--;
		id++;
	}
	m_pData[id]=0;
}

#define LOG2_10 0.301029995663981

void XString8::FormatV(XPTSTR buffer,float value)
{
	int i;
	int prec = 15; // base 10 precision
	
	char *s = buffer;
	
	// Deal with negative numbers
	if (value < 0) {
		value = -value;
		*s++ = '-';
	}
	
	// Determine base 2 exponent
	int exp2;
	frexp(value, &exp2);
	exp2--;
	
	// Convert to base 10 exponent
	int exp10 = (int) RoundInt((double)exp2 * LOG2_10);
	
	// Obtain base 10 mantissa
	double pow10 = PowerOfTen(exp10);
	double f = value / pow10;
	
	// Adjust for round-off error on exponent
	if ((int) f == 0) {
		exp10--;
		pow10 = PowerOfTen(exp10);
		f = value / pow10;
	}
	
	// Sentinel is used for rounding
	char *sentinel = s;
	
	if (exp10 < 0 && exp10 > -6) {
		// Number is of form 0.######
		if (exp10 < -prec) {
			exp10 = -prec-1;
		}
		sentinel = s;
		*s++ = '0'; // Sentinel
		*s++ = '0';
		*s++ = '.';
		
		// Write out leading zeros
		for (i=exp10; i<-1; i++) {
			*s++ = '0';
		}
		// Write out significand
		for (i=0; i<prec; i++) {
			*s++ = NextDigit(&f) + '0';
		}
		exp10 = 0;
	} else if (exp10 >= prec) {
		*s++ = NextDigit(&f) + '0';
		*s++ = '.';
		for (i=0; i<prec-1; i++) {
			*s++ = NextDigit(&f) + '0';
		}
	} else {
		int digits = 0;
		sentinel = s;
		*s++ = '0';
		i = NextDigit(&f);
		if (i) {
			*s++ = i + '0';
		}
		while (exp10 > 0) {
			*s++ = NextDigit(&f) + '0';
			exp10--;
			digits++;
		}
		*s++ = '.';
		for (; digits < prec-1; digits++) {
			*s++ = NextDigit(&f) + '0';
		}
    }
	
	// Rounding
	i = NextDigit(&f);
	if (i > 4) {
		char *ptr = s-1;
		while (ptr >= buffer) {
			if (*ptr < '0') {
				ptr--;
				continue;
			}
			(*ptr)++;
			if (*ptr != 0x3A) {
				break;
			}
			*ptr-- = '0';
		}
    }
	
	/* Remove trailing zeros */
	while (*(s-1) == '0') {
		s--;
	}
	if (*(s-1) == '.') {
		s--;
	}
	
	if(exp10) {
		*s++ = 'e';
		XString8 stre;
		stre.FromInt(exp10);
		//	char *expstr = ConvertIntegerToString(exp10);
		char *t = stre;//expstr;
		while (*t) { *s++ = *t++; }
		//FreeStr(expstr);
	}
	
	*s = '\0';
	
	if (sentinel && sentinel[0] == '0' && sentinel[1] != '.') {
		char *s = sentinel;
		char *t = sentinel+1;
		while ((*s++ = *t++) != 0);
	}
}

//void XString8::FormatV(XPTSTR buffer,XF64 value)
//{
//	int i;
//	int prec = 15; // base 10 precision
//	
//	char *s = buffer;
//	
//	// Deal with negative numbers
//	if (value < 0) {
//		value = -value;
//		*s++ = '-';
//	}
//	
//	// Determine base 2 exponent
//	int exp2;
//	frexp(value, &exp2);
//	exp2--;
//	
//	// Convert to base 10 exponent
//	int exp10 = (int) RoundInt((double)exp2 * LOG2_10);
//	
//	// Obtain base 10 mantissa
//	double pow10 = PowerOfTen(exp10);
//	double f = value / pow10;
//	
//	// Adjust for round-off error on exponent
//	if ((int) f == 0) {
//		exp10--;
//		pow10 = PowerOfTen(exp10);
//		f = value / pow10;
//	}
//	
//	// Sentinel is used for rounding
//	char *sentinel = s;
//	
//	if (exp10 < 0 && exp10 > -6) {
//		// Number is of form 0.######
//		if (exp10 < -prec) {
//			exp10 = -prec-1;
//		}
//		sentinel = s;
//		*s++ = '0'; // Sentinel
//		*s++ = '0';
//		*s++ = '.';
//		
//		// Write out leading zeros
//		for (i=exp10; i<-1; i++) {
//			*s++ = '0';
//		}
//		// Write out significand
//		for (i=0; i<prec; i++) {
//			*s++ = NextDigit(&f) + '0';
//		}
//		exp10 = 0;
//	} else if (exp10 >= prec) {
//		*s++ = NextDigit(&f) + '0';
//		*s++ = '.';
//		for (i=0; i<prec-1; i++) {
//			*s++ = NextDigit(&f) + '0';
//		}
//	} else {
//		int digits = 0;
//		sentinel = s;
//		*s++ = '0';
//		i = NextDigit(&f);
//		if (i) {
//			*s++ = i + '0';
//		}
//		while (exp10 > 0) {
//			*s++ = NextDigit(&f) + '0';
//			exp10--;
//			digits++;
//		}
//		*s++ = '.';
//		for (; digits < prec-1; digits++) {
//			*s++ = NextDigit(&f) + '0';
//		}
//    }
//	
//	// Rounding
//	i = NextDigit(&f);
//	if (i > 4) {
//		char *ptr = s-1;
//		while (ptr >= buffer) {
//			if (*ptr < '0') {
//				ptr--;
//				continue;
//			}
//			(*ptr)++;
//			if (*ptr != 0x3A) {
//				break;
//			}
//			*ptr-- = '0';
//		}
//    }
//	
//	/* Remove trailing zeros */
//	while (*(s-1) == '0') {
//		s--;
//	}
//	if (*(s-1) == '.') {
//		s--;
//	}
//	
//	if(exp10) {
//		*s++ = 'e';
//		XString8 stre;
//		stre.FromInt(exp10);
//		//	char *expstr = ConvertIntegerToString(exp10);
//		char *t = stre;//expstr;
//		while (*t) { *s++ = *t++; }
//		//FreeStr(expstr);
//	}
//	
//	*s = '\0';
//	
//	if (sentinel && sentinel[0] == '0' && sentinel[1] != '.') {
//		char *s = sentinel;
//		char *t = sentinel+1;
//		while ((*s++ = *t++) != 0);
//	}
//}