#ifndef __BwFILLSTYLE_H__
#define __BwFILLSTYLE_H__

#include "BwMatrix.h"
//#include <vector>
//#include "gl3.h"
#include "XDrawDib.h"
#include "GGType.h"
#include "Shape.h"
#include "sbitmap.h"

//#include <Windows.h>
//#include <gl\GL.h>
#ifdef _WIN32
#include "gl/glew.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
//#include "CCGL.h"

class RasterGL;
struct GLColorTransform {
public:
	int flags;
	S16 aa, ab;	// a is multiply factor, b is addition factor
	S16 ra, rb;
	S16 ga, gb;
	S16 ba, bb;
};
class FrameTexture;
typedef struct _GLFrameInfo
{
	U16    bitsStyle;
	U8     smooth;
	int	   repeat;
	MATRIX savedMat;
	float* invMat;
	FrameTexture* frame;
}GLFrameInfo;
typedef struct _GLColorBMInfo {
	U16 bitsStyle;
	U8 smooth;
	int	repeat;
	SBitmapCore* bitmap;
	MATRIX	savedMat;
	float*  invMat;
	//int bitmapID;
	//MATRIX invMat;
	//MATRIX scaleMat;
	//MATRIX gridMat;
	//MATRIX orgMat;
	//MATRIX devMat;
	//SRECT  gridBy;
	//SRECT  gridRect;
	//SRECT  orgRect;
	//int    offX,offY;
	//GLColorTransform cxform;
	//_XRColorMap* colorMap;
	//XU16*		 lineAlpha;
	
	//BltProc bltProc;
	//BltProc compositeProc;
	//BltProcI bltProcI;
	//BltProc fastBltProc;
	//_XBltInfo bi;		// cached blting info
}GLColorDIBInfo;

typedef struct _GLColorDIBInfo {
	int w,h;
	bool preAlpha;
	struct _TEXTUREID* bitmapID;
	//int bitmapID;
	int repeat;
	MATRIX	savedMat;
	float*  invMat;
}GLColorBMInfo;

typedef struct _GLColorGradInfo {
	int gradStyle;
  	MATRIX savedMat;
	//GLfloat invMat[16];
	//MATRIX invMat;
	float* invMat;
	GradColorRamp ramp;
	struct _TEXTUREID* bitmapID;
	//int bitmapID;
	//RGB8* colorRamp;
}GLColorGradInfo;

struct _GLFontInfo
{
	unsigned char*	    fontData;
	struct _TEXTUREID*  bitmapID;
	//int bitmapID;
	float*	  invMat;
	int		  fontW,fontH;
};

typedef struct _GLColor {
	U8	 colorType;			// solid, bitmap, gradient
	RGBI rgb;				// the expanded color components
	//RasterGL* raster;
	//XU16 nWidth;
	//XU16 nFlag;
	//int pattern;			// The pattern of the object
	//U32 rgbPoint;			// the x,y location that rgb currently represents in device pixels, rgbPoint = (y<<16) | x
	
	union {
		//XU32  pat;
		GLfloat color[4];
		_GLFontInfo fm;
		_GLFrameInfo frame;
		//U32 pat[16];		// for solid colors, the expanded pattern
		_GLColorBMInfo bm;	// for bitmaps, the source bitmap info
		_GLColorDIBInfo dib;
		_GLColorGradInfo grad;// for gradients, the gradient info
	};
	
	void SetUp();
	
	//void BuildCache();
	void FreeCache();
}GLColor;
typedef GLColor BwFillStyle;
struct BwLineStyle
{
	enum ENJoinStyle
	{
		en_JOIN_ROUND = 0,
		en_JOIN_BEVEL = 1,
		en_JOIN_MITER = 2
	};

	enum ENCapStyle
	{
		en_CAP_ROUND = 0,
		en_CAP_NONE = 1,
		en_CAP_SQUARE = 2
	};
	GLColor* color;
	
	XU16 mWidth;
	RGBI mColor;

	ENCapStyle menStartCapStyle;
	ENJoinStyle menJoinStyle;

	bool mbHasFillFlag;
	bool mbNoHScaleFlag;
	bool mbNoVScaleFlag;
	bool mbPixelHintingFlag;
	bool mbNoClose;
	ENCapStyle menEndCatStyl;
	float mfMiterLimitFactor;
};


#endif
