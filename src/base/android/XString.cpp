// XString.cpp: implementation of the XString8 class.
//
//////////////////////////////////////////////////////////////////////

#ifdef __ANDROID__
#include <wchar.h>
#endif
#include "StdAfxGGBase.h"
#include <math.h>
#include "XString.h"
#include "XStream.h"
#include "MultiStringUtil.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#if 0
#define LOGWHERESTR()  LOGWHERE()
#else
#define LOGWHERESTR()
#endif

void XString8::operator +=(XPCTSTR pData)
{
	LOGWHERESTR();
//	_ANALYENTER("String Add");
	if(pData==XNULL) return;
	XUINT nSize=strlen(pData);
	XUINT nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	memcpy(m_pData+nOldSize-1,pData,nSize);
}
void XString8::AddString(XPCTSTR pData,XUINT nSize)
{
	LOGWHERESTR();
	if(pData==XNULL) return;
	if(nSize==0) nSize=strlen(pData);
	XUINT nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	memcpy(m_pData+nOldSize-1,pData,nSize);
}

int XString8::Find(XTCHAR c,XUINT nBegin)
{
	LOGWHERESTR();
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
	XPCTSTR p=XSTRCHR(m_pData+nBegin,c);
	if(p==XNULL) return -1;
	return p-m_pData;
}
int XString8::Find(XPCTSTR pData,XUINT nBegin)
{
	LOGWHERESTR();
	if(pData==XNULL) return 0;
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
	XPCTSTR p=XSTRSTR(m_pData+nBegin,pData);
	if(p==XNULL) return -1;
	return p-m_pData;
}
void XString8::TrimLeft(char c)
{
	LOGWHERESTR();
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
	LOGWHERESTR();
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
{
	LOGWHERESTR();
	RemoveAt(id,nCount);
}

XU8 XString8::IsNum()
{
	LOGWHERESTR();
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
	LOGWHERESTR();
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

void XString8::FormatV(XPTSTR m_pData,int val,XBOOL bHex)
{
	LOGWHERESTR();
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

void XString8::FromInt(XINT val,XBOOL bHex)
{
	LOGWHERESTR();
	SetSize(20);
	FormatV(m_pData,val,bHex);
}


double XString8::ToFloat()
{
	LOGWHERESTR();
	if(IsEmpty()) return 0;
	if(m_pData[1]=='x'||
	   m_pData[2]=='X')
	   return (double)ToInt(16);
	return XATOF(m_pData);
}

void XString8::Mid(XString8&str,int nIndex,XU32 nSize) const
{
	LOGWHERESTR();
	//int nSize=m_nSize-nIndex-1;
	if(nIndex+nSize>=m_nSize) {str.Empty();return;}
	if(nSize<=0) nSize=m_nSize-nIndex-1;
	//if(nSize<=0) return XString8("");
	str.SetString(m_pData+nIndex,nSize);
}

void XString8::Right(XString8&str,XU32 nSize)
{
	LOGWHERESTR();
	if(nSize<=0) str.Empty();
	else
	{
		if(nSize>m_nSize-1) nSize=m_nSize-1;
		str.SetString(m_pData+m_nSize-nSize-1,nSize);
	}
}
void XString8::MakeFirstUpper()
{
	LOGWHERESTR();
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
	LOGWHERESTR();
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
	LOGWHERESTR();
  int digit;
  digit = (int) *value;
  *value -= digit;
  *value *= 10;
  return digit;
}

static int RoundInt(double x)
{
	LOGWHERESTR();
	if (x < 0) {
		return (int) (x - 0.5);
	} else {
		return (int) (x + 0.5);
	}
}

static double PowerOfTen(int exponent)
{
	LOGWHERESTR();
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

void XString8::FormatV(XPTSTR buffer,float value)
{
	LOGWHERESTR();
	int i;
	int prec = 8; // base 10 precision

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

void XString8::FromFloat(XF64 value)
{
	LOGWHERESTR();
	SetSize(256);
	FormatV(m_pData,value);
}

void XString8::AddWChar(XWCHAR c)//,XU8 nCharset)
{
	//LOGWHERESTR();
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
	LOGWHERESTR();
	///////////////////////////
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

			if(len>0&&len<iLen)
				iLen=len;
#ifdef __ANDROID__
			XU16* pData = (XU16*)gbkToUnicode(pStr, iLen );
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

void XString8::operator=(XPCWSTR pData)
{
	LOGWHERESTR();
	if(pData==XNULL) Add(0);
	else
	{
		Empty();
		XU32 nSize=(XU32)gg_wcslen(pData);
		for(XU32 i=0;i<nSize;i++)
			AddWChar(pData[i]);
	}
}

void XString16::SetString(XPCWSTR pData,XINT nSize)
{
	LOGWHERESTR();
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
	LOGWHERESTR();
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
	LOGWHERESTR();

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

int XString16::GBToUCS(const char*buf,XU16*dst,int nCount)
{
	LOGWHERESTR();
	XU16 *pTemp = (XU16*)gbkToUnicode( buf, nCount );
	int ilen = gg_wcslen( pTemp );
	memmove( dst, pTemp, ilen * 2);
	delete []pTemp;
	dst[ilen] = 0;
	return ilen;
}

int XString16::UTF8ToUCS(const char*buf,XU16*dst,int nCount)
{
	LOGWHERESTR();
	XU16 *pTemp = (XU16*)utf8ToUnicode( buf );
	int ilen = gg_wcslen( pTemp );
	memmove( dst, pTemp, ilen * 2);
	delete []pTemp;
	dst[ilen] = 0;
	return ilen;
}

XBOOL XString16::ConvertFrom(XPCTSTR pStr,XU8 nCharset)
{
	LOGWHERESTR();
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
			XU32 iLen=XSTRLEN(pStr);
			this->SetSize(iLen+1);
			iLen=GBToUCS(pStr,this->m_pData,iLen);
			this->m_nSize=iLen+1;
			return XTRUE;
		}
		break;
	}
	return XFALSE;
}

void XString16::operator=(XPCTSTR pData)
{
	LOGWHERESTR();
	if(pData==XNULL) {Init();return;}
	int nLen=strlen(pData)+1;
	SetSize(nLen);

#ifdef __ANDROID__
	XU16* tempData = (XU16*)utf8ToUnicode(pData);
	int ilen = gg_wcslen( tempData );
	memmove(m_pData, tempData, (ilen + 1) * 2);
	m_nSize=ilen+1;
	delete[] tempData;
#else
	MultiByteToWideChar(CP_UTF8,0,pData,nLen,m_pData,nLen);
	m_nSize=wcslen(m_pData)+1;
#endif

	//FreeExtra();

}

int XString16::Find(XPCWSTR pData,XUINT nBegin)
{
	LOGWHERESTR();
	if(pData==XNULL) return 0;
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
#ifdef __ANDROID__
	XPCWSTR p=gg_wcsstr(m_pData+nBegin,pData);
#else
	XPCWSTR p=wcsstr(m_pData+nBegin,pData);
#endif
	if(p==XNULL) return -1;
	return p-m_pData;
}

void XString8::SetString(XPCTSTR pData,int nSize)
{
	LOGWHERESTR();
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

// utf8 -> gb
void XString8::ConvertToGB()
{
	LOGWHERESTR();
#ifdef __ANDROID__
	char* lpDestBuf = utf8ToGbk(m_pData );
	SetString(lpDestBuf,strlen(lpDestBuf));
	//*this = XString8(lpDestBuf);
	delete[] lpDestBuf;
#endif
	return;
}

double XString16::ToFloat()
{
	LOGWHERESTR();
	if(IsEmpty()) return 0;
	if(m_pData[1]=='x'||
	   m_pData[2]=='X')
	   return (double)ToInt(16);
	//XWCHAR* str;
	//return wcstod((wchar_t*)m_pData,(wchar_t**)&str);
#ifdef __ANDROID__
	XString8 str(*this);
	return str.ToFloat();
#else
	XWCHAR* str;
	return wcstod(m_pData,&str);
#endif
}


void XString16::Mid(XString16&str,int nIndex,XU32 nSize)
{
	LOGWHERESTR();
	if(nIndex+nSize>=m_nSize) {str.Empty();return;}
	if(nSize<=0) nSize=m_nSize-nIndex-1;
	str.SetString(m_pData+nIndex,nSize);
}

void XString16::Right(XString16&str,XU32 nSize)const
{
	LOGWHERESTR();
	if(nSize<=0) str.Empty();
	else
	{
		if(nSize>m_nSize-1) nSize=m_nSize-1;
		str.SetString(m_pData+m_nSize-nSize-1,nSize);
	}
}
int XString16::Find(XWCHAR c,XUINT nBegin)
{
	LOGWHERESTR();
	if(nBegin>=m_nSize-1) return -1;
	if(nBegin<0) nBegin=0;
#ifdef __ANDROID__
	XPCWSTR p=gg_wcschr(m_pData+nBegin,c);
#else
	XPCWSTR p=wcschr(m_pData+nBegin,c);
#endif
	if(p==XNULL) return -1;
	return p-m_pData;
}

void XString16::MakeFirstUpper()
{
	LOGWHERESTR();
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
	LOGWHERESTR();
	if(pStr==XNULL) return 1;
#ifdef __ANDROID__
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
	LOGWHERESTR();
	if(pStr==XNULL) return 1;
#ifdef __ANDROID__
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
	LOGWHERESTR();
	if(pData==XNULL)
	{
		Init();
		return;
	}
	int nSize=SafeStrlen(pData);
	SetSize(nSize+1,XFALSE);
	memcpy(m_pData,pData,(nSize+1)*sizeof(XWCHAR));
	//XString16(pData,SafeStrlen(pData));
}

void XString16::operator +=(XPCWSTR pData)
{
	LOGWHERESTR();
	if(pData==XNULL) return;
	XUINT nSize=SafeStrlen(pData);
	XUINT nOldSize=m_nSize;
	SetSize(m_nSize+nSize);
	XCOPY(m_pData+nOldSize-1,pData,nSize*sizeof(XWCHAR));
}

XBOOL XString16::IsNum()
{
	LOGWHERESTR();
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
	LOGWHERESTR();
	FromFloat((double)val);
}

XINT XString8::ToInt(XPCTSTR buf,int nSize,int nDec,XBOOL bPec)
{
	LOGWHERESTR();
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

XINT XString8::ToInt(char nDec,XU8 bPec)
{
	LOGWHERESTR();
	return ToInt(m_pData,m_nSize,nDec,bPec);
}

void XString8::ReadFromStream(XStream *pStream, int nLen)
{
	LOGWHERESTR();
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

void XStringRef()
{
	LOGWHERESTR();
}




