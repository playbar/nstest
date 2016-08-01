// XFilter.h: interface for the XFilter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFILTER_H__9A36C550_2E20_4209_ABAC_71736BD8303F__INCLUDED_)
#define AFX_XFILTER_H__9A36C550_2E20_4209_ABAC_71736BD8303F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
#include "gl/glew.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

enum
{
	XFilterDropShadow=0,
	XFilterBlur=1,
	XFilterGlow=2,
	XFilterBevel=3,
	XFilterGradientGlow=4,
	XFilterConvolution=5,
	XFilterColorMatrix=6,
	XFilterGradientBevel=7,
};
#pragma pack(1)
struct _XFilterColorMatrix
{GLfloat fData[20];};
struct _XFilterBlur
{	
	 GLfloat blurX,blurY;
	/*XU32 blurX,blurY;
	XU8 flags; */
};
struct _XFilterConvolution
{
	 GLfloat fData[9];
	 /*XU8 matrixX,matrixY;
	 float divisor,bias;
	 float *pMatrix;
	 XU32 defaultColor;
	 XU8 flag;*/
};
struct _XFilterDropShadow
{
	GLfloat color[4];
	GLfloat blurX,blurY,angle,distance;
	GLfloat strength;
	GLint konckout;
	//XU32 color;
	//XU32 blurX,blurY,angle,distance;//FIXED
	//XU16 Strength;
	//XU8	 flags;
};
struct _XFilterGlow
{
	GLfloat color[4];
	GLfloat blurX,blurY;
	GLfloat strength;
	GLint konckout;
	//XU32 color;
 //   XU32 blurX,blurY;//FIXED
 //   XU16 strength;
 //   XU8  flags;
};
struct _XFilterBevel
{
    GLfloat shadowColor[4],hiColor[4];
	GLfloat blurX,blurY,angle,distance;
	GLfloat strength;
	GLint konckout;
	//XU32 shadowColor,hiColor;
	//XU32 blurX,blurY,angle,distance;//FIXED
	//XU16 strength;
	//XU8	 flags;
};

struct _XFilterGradientGlow
{
	GLfloat color[4];
	GLfloat blurX,blurY,angle,distance;
	GLfloat strength;
	GLint konckout;
	/*XU8 colorNums;
	XU32* pGradColors;
	XU8*  pGradRatios;
	XU32  blurX,blurY,angle,distance;
	XU16  strength;
	XU8	  flags;*/
};

struct _XFilterGradientBevel
{
	GLfloat shadowColor[4],hiColor[4];
	GLfloat blurX,blurY,angle,distance;
	GLfloat strength;
	GLint konckout;
	/*XU8 colorNums;
	XU32* pGradColors;
	XU8*  pGradRatios;
	XU32  blurX,blurY,angle,distance;
	XU16  strength;
	XU8	  flags;*/
};

#pragma pack()
class SParser;
class XSWFPlayer;

typedef struct _XFilter  
{
public:
	XU32 nColor;
//#if ( __CORE_VERSION__>=0x02076000)
	XU32 nData;
//#endif
	static _XFilter* CreateFilter(SParser*pStream,XSWFPlayer*player);
	_XFilter();
	XU32 GetColor();
	XBOOL ReadFromStream(SParser*pStream);
	~_XFilter();
	_XFilter* m_pNext;
	XU8		 m_nFilterID;
	XBOOL    filter_konckout;
	union
	{
		_XFilterDropShadow pDropShadow;
		_XFilterBlur pBlur;
		_XFilterGlow pGlow;
		_XFilterBevel pBevel;
		_XFilterGradientGlow pGradientGlow;
		_XFilterConvolution pConvolution;
		_XFilterColorMatrix pColorMatrix;
		_XFilterGradientBevel pGradientBevel;
	};
//protected:
//	XU8 filterID;
}XFilter;

//typedef XVector<XFilter*> XFILTERLIST;

/*class XFilterColorMatrix:public XFilter
{
protected:
	XBOOL ReadFromStream(SParser*pStream)
	{
		return pStream->ReadData(fData,sizeof(fData));
	}
	float fData[20];
};

class XFilterBlur:public XFilter
{
protected:
    XBOOL ReadFromStream(SParser*pStream)
	{
		if(!pStream->ReadDWord(blurX)) return XFALSE;
		if(!pStream->ReadDWord(blurY)) return XFALSE;
		return pStream->ReadByte(flags);
	}
	XU32 blurX,blurY;
	XU8 flags;
};

class XFilterBevel:public XFilter
{
protected:
	XBOOL ReadFromStream(SParser*pStream)
	{
		if(!pStream->ReadDWord(shadowColor)) return XFALSE;
		if(!pStream->ReadDWord(hiColor)) return XFALSE;
		if(!pStream->ReadDWord(blurX)) return XFALSE;
		if(!pStream->ReadDWord(blurY)) return XFALSE;
		if(!pStream->ReadDWord(angle)) return XFALSE;
		if(!pStream->ReadDWord(distance)) return XFALSE;
		if(!pStream->ReadWord(strength)) return XFALSE;
		return pStream->ReadByte(flags);
	}
	XU32 shadowColor,hiColor;
	XU32 blurX,blurY,angle,distance;
	XU16 strength;
	XU8	 flags;
};

class XFilterGradient:public XFilter
{
public:
	XFilterGradient()
	{
		pGradColors=XNULL;
		pGradRatios=XNULL;
	}
	~XFilterGradient()
	{
		if(pGradColors) delete pGradColors;
		if(pGradRatios) delete pGradRatios;
	}
protected:
	XBOOL ReadFromStream(SParser*pStream)
	{
		if(!pStream->ReadByte(colorNums)) return XFALSE;
		if(colorNums)
		{
			pGradColors=new XU32[colorNums];
			pGradRatios=new XU8[colorNums];
			if(!pStream->ReadData(pGradColors,colorNums*sizeof(XU32))) return XFALSE;
			if(!pStream->ReadData(pGradRatios,colorNums)) return XFALSE;
		}
		if(!pStream->ReadDWord(blurX)) return XFALSE;
		if(!pStream->ReadDWord(blurY)) return XFALSE;
		if(!pStream->ReadDWord(angle)) return XFALSE;
		if(!pStream->ReadDWord(distance)) return XFALSE;
		if(!pStream->ReadWord(strength)) return XFALSE;
		return pStream->ReadByte(flags);
	}
	XU8 colorNums;
	XU32* pGradColors;
	XU8*  pGradRatios;
	XU32  blurX,blurY,angle,distance;
	XU16  strength;
	XU8	  flags;
};

class XFilterGrow:public XFilter
{
protected:
	  XBOOL ReadFromStream(SParser*pStream)
	  {
		if(!pStream->ReadDWord(color)) return XFALSE;
		if(!pStream->ReadDWord(blurX)) return XFALSE;
		if(!pStream->ReadDWord(blurY)) return XFALSE;
		if(!pStream->ReadWord(strength)) return XFALSE;
		return pStream->ReadByte(flags);
 
	  }
	  XU32 color;
	  XU32 blurX,blurY;
	  XU16 strength;
	  XU8  flags;
};

class XFilterDropShadow:public XFilter
{
protected:
	XBOOL ReadFromStream(SParser*pStream)
	{
		if(!pStream->ReadDWord(color)) return XFALSE;
		if(!pStream->ReadDWord(blurX)) return XFALSE;
		if(!pStream->ReadDWord(blurY)) return XFALSE;
		if(!pStream->ReadDWord(angle)) return XFALSE;
		if(!pStream->ReadDWord(distance)) return XFALSE;
		if(!pStream->ReadWord(Strength)) return XFALSE;
		return pStream->ReadByte(flags);

	}
	XU32 color;
	XU32 blurX,blurY,angle,distance;
	XU16 Strength;
	XU8	 flags;
};

class XFilterConvolution:public XFilter
{
public:
	 XFilterConvolution()
	 {
		 pMatrix=XNULL;
	 }
	 ~XFilterConvolution()
	 {
		 if(pMatrix) delete pMatrix;
	 }
protected:
	 XBOOL ReadFromStream(SParser*pStream)
	 {
		 if(!pStream->ReadByte(matrixX)) return XFALSE;
		 if(!pStream->ReadByte(matrixY)) return XFALSE;
		 if(!pStream->ReadFloat(divisor)) return XFALSE;
		 if(!pStream->ReadFloat(bias)) return XFALSE;
		 int s=matrixX*matrixY;
		 if(s)
		 {
			 pMatrix=new float[s];
			 if(!pStream->ReadData(pMatrix,s*sizeof(float))) return XFALSE;
		 }
		 if(!pStream->ReadDWord(defaultColor)) return XFALSE;
		 if(!pStream->ReadByte(flag)) return XFALSE;
		 return XTRUE;
	 }
	 XU8 matrixX,matrixY;
	 float divisor,bias;
	 float *pMatrix;
	 XU32 defaultColor;
	 XU8 flag;
};*/

#endif // !defined(AFX_XFILTER_H__9A36C550_2E20_4209_ABAC_71736BD8303F__INCLUDED_)
