// XXObjectBitmap1.h: interface for the XXObjectBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTBITMAP1_H__35DF97C3_EEB7_4BD7_8AC5_FE23638FC9CE__INCLUDED_)
#define AFX_XXOBJECTBITMAP1_H__35DF97C3_EEB7_4BD7_8AC5_FE23638FC9CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"
#include "sbitmap.h"	// Added by ClassView
class FrameBuffer;

class XXObjectBitmap : public XXObject  
{
	XDEC_XOBJECT(XXObjectBitmap);
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);
	//void widthGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void heightGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void rectangleGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	//void transparentGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	////////////////////////////////////////////////////
	void applyFilter(XSWFCONTEXT&cnt,XXVARLIST&list);
	void clone(XSWFCONTEXT&cnt,XXVARLIST&list);
	void colorTransform(XSWFCONTEXT&cnt,XXVARLIST&list);
	void copyChannel(XSWFCONTEXT&cnt,XXVARLIST&list);
	void dispose(XSWFCONTEXT&cnt,XXVARLIST&list);
	void draw(XSWFCONTEXT&cnt,XXVARLIST&list);
	void fillRect(XSWFCONTEXT&cnt,XXVARLIST&list);
	void floodFill(XSWFCONTEXT&cnt,XXVARLIST&list);
	void generateFilterRect(XSWFCONTEXT&cnt,XXVARLIST&list);
	void getColorBoundsRect(XSWFCONTEXT&cnt,XXVARLIST&list);
	void getPixel(XSWFCONTEXT&cnt,XXVARLIST&list);
	void getPixel32(XSWFCONTEXT&cnt,XXVARLIST&list);
	void hitTest(XSWFCONTEXT&cnt,XXVARLIST&list);
	void loadBitmap(XSWFCONTEXT&cnt,XXVARLIST&list);
	void merge(XSWFCONTEXT&cnt,XXVARLIST&list);
	void noise(XSWFCONTEXT&cnt,XXVARLIST&list);
	void paletteMap(XSWFCONTEXT&cnt,XXVARLIST&list);
	void perlinNose(XSWFCONTEXT&cnt,XXVARLIST&list);
	void pixelDissolve(XSWFCONTEXT&cnt,XXVARLIST&list);
	void scroll(XSWFCONTEXT&cnt,XXVARLIST&list);
	void setPixel(XSWFCONTEXT&cnt,XXVARLIST&list);
	void setPixel32(XSWFCONTEXT&cnt,XXVARLIST&list);
	void threshold(XSWFCONTEXT&cnt,XXVARLIST&list);
public:
	//SBitmapCore m_sbitmap;
	virtual XU32 Construct(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*);
	XXObjectBitmap(ScriptPlayer*pRoot);
	virtual ~XXObjectBitmap();
	virtual XU32 Function(XSWFCONTEXT*pCnt,XXVARLIST&list,XXObject*)
	{
		return (XU32)CreateObject(pCnt,list,XFALSE);
	}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_BITMAP;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	/*XDrawDib* GetBitmap()
	{
		if(m_pDib)
		{
			m_sbitmap.PICreate(*m_pDib,m_bTransparent,NULL);
		}
		return m_pDib;
	}*/
	XBOOL IsEmptyBits()
	{
		return m_pChar==XNULL||!m_pChar->bits.HasBits();
	}
	_TEXTUREID* GetBmInfo(TextureInfo&info)
	{
		if(!m_pChar) return 0;
		//if(m_pChar->type != bitmapEmptyChar)
		{
			info.w=m_pChar->bits.width;
			info.h=m_pChar->bits.height;
			info.preAlpha = true;
			info.nDiv=m_pChar->bits.nImgDiv;
			info.transparent = m_pChar->bits.transparent!=0;
			info.id = m_pChar->bits.bitmapID;
			return m_pChar->bits.bitmapID;
		}
		/*else
		{
			info.w=m_pChar->frame->mWidth;
			info.h=m_pChar->frame->mHeight;
			info.transparent = m_pChar->frame->transparent;
			info.preAlpha = false;
			info.id = m_pChar->frame->mTexture;
			return m_pChar->frame->mTexture;
		}*/
	}
	XBOOL IsTransparent()
	{	
		if(!m_pChar) return false;
		//if(m_pChar->type!=bitmapEmptyChar)
		return m_pChar->bits.transparent;
		//else
		//	return m_pChar->frame->transparent;
		//return m_bTransparent;
	}
	void ReleaseChar(_XSCharacter*ch);
	void CreateTexture();
	void LockBitmap(bool forWrite);
	//void   LockBitmap(){m_bNeedDelete=XFALSE;}
	//XBOOL  IsCreated(){return m_bNeedDelete;}
	SCharacter*  m_pChar;
protected:	
	void CopyPixel(XXVARLIST&list);
	
	bool		 m_bAttach;
	bool		 m_bModify;
	//XBitmap* m_pBitmap;
	//XDrawDib*m_pDib;
	//XU8  m_bTransparent;	
	void InitObject( );
};

#endif // !defined(AFX_XXOBJECTBITMAP1_H__35DF97C3_EEB7_4BD7_8AC5_FE23638FC9CE__INCLUDED_)
