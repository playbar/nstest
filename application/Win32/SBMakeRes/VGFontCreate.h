// VGFontCreate.h: interface for the VGFontCreate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VGFONTCREATE_H__C16B94F6_84E5_46E9_B7A6_EDB0ADD4C4F4__INCLUDED_)
#define AFX_VGFONTCREATE_H__C16B94F6_84E5_46E9_B7A6_EDB0ADD4C4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class VGBitmapDC : public CDC  
{
public:
	//得到位图数据
	BOOL GetDib( HGLOBAL pDib, BITMAPINFOHEADER *pHead );
	//创建系统兼容的内存句柄.W,H图象尺度
	BOOL CreateBitmapDC( int W, int H);
	//W,H图象尺度
	VGBitmapDC(int W,int H);
	VGBitmapDC(CBitmap*bmp);
	virtual ~VGBitmapDC();
	CBitmap* GetBitmap(){return &m_bitmap;}
protected:
	CBitmap m_bitmap;
	CBitmap* m_pOldBmp;
};

class VGFontCreate  
{
public:
	int DrawChar(CDC*pDC,int x,int y,unsigned long nColor,unsigned long nFrame);
	unsigned char* CreateChar(unsigned short nChar,int&nCharWidth,int&nDataSize);
	void Init(LPCTSTR strCh,LPCTSTR strEn,int nSize,int nPecent);
	VGFontCreate();
	virtual ~VGFontCreate();
	enum {SIZE_8,SIZE_12,SIZE_16};
protected:
	unsigned char m_data[64];
	VGBitmapDC* m_pDC;
	int	  m_nSize,m_nType;
	int	  m_nDataSize;
	int	  m_nCharWidth;
	CString strChinese,strEnglish;
	CFont m_ch,m_en;
};

#endif // !defined(AFX_VGFONTCREATE_H__C16B94F6_84E5_46E9_B7A6_EDB0ADD4C4F4__INCLUDED_)
