// XHTMLForm.h: interface for the XHTMLForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XHTMLFORM_H__1B37E6B4_97B6_4BFD_93FA_146D39324D1C__INCLUDED_)
#define AFX_XHTMLFORM_H__1B37E6B4_97B6_4BFD_93FA_146D39324D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XDomItem.h"

class XSHA1
{
private:
	int i,g;
public:
	XSHA1();
	virtual ~XSHA1();
	void Final(const char*pData,int len,void*md);
	int a(int k,int j,int m,int l);
	void b(int*A,int length,int r,int*output);
	//void h(int*A,char*output);
	//void f(char*input,int len,int*output);
	int d(unsigned int j,int k)
	{
		return(j<<k)|(j>>(32-k));
	}
	int e(int j,int m)
	{
		int l=(j&65535)+(m&65535);
		int k=(j>>16)+(m>>16)+(l>>16);
		return(k<<16)|(l&65535);
	}
	int c(int j)
	{
		return(j<20)?1518500249:(j<40)?1859775393:(j<60)?-1894007588:-899497514;
	}

	void f(char*m,int length,int*output)
	{
		int j=(1<<g)-1;
		for(int k=0;k<length*g;k+=g)
		{
			output[k>>5]|=(m[k/g]&j)<<(24-k%32);
		}
	}
	void h(int*l,int length,char*output)
	{
		const char* k=i?"0123456789ABCDEF":"0123456789abcdef";
		//var m="";
		for(int j=0;j<length*4;j++)
		{
			output[j*2]=k[(l[j>>2]>>((3-j%4)*8+4))&15];
			output[j*2+1]=k[((l[j>>2]>>((3-j%4)*8))&15)];
				//k[(l[j>>2]>>((3-j%4)*8+4))&15]+k[((l[j>>2]>>((3-j%4)*8))&15)];
		}
		output[length*8]=0;
		//return m
	}
};

class XHTMLForm : public XDomNode
{
	XDEC_CREATE(XHTMLForm)
public:
	XHTMLForm(XU16 nID):XDomNode(nID){}
protected:
	void MakeNonce(XString8&strNonce,int len);
	XU32 DoSubmit(XEVENTDATA*pData2);
	void MakeWeiboInterface(XEVENTDATA*pData);
	XU32 Handle(XU32 nOpera,XU32 pData1,XU32 pData2);
};

#endif // !defined(AFX_XHTMLFORM_H__1B37E6B4_97B6_4BFD_93FA_146D39324D1C__INCLUDED_)
