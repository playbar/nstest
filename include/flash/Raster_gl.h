#ifndef __RASTERE_GL__H__
#define __RASTERE_GL__H__

#include "BwShapeRecord.h"
#ifdef _WIN32
#include "gl/glew.h"
#include <vector>
using namespace std;
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <vector>
using namespace std;
#elif defined __ANDROID__
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#include "XData.h"
#include "XFilter.h"
//#include <vector>
//#include <map>
//using namespace std;
#define _MINLINEW 40.0
class XCXForm {
public:
	int flags;
	enum { 
		needA=0x1,	// set if we need the multiply terms
		needB=0x2	// set if we need the constant terms
	};
	S16 aa, ab;	// a is multiply factor, b is addition factor
	S16 ra, rb;
	S16 ga, gb;
	S16 ba, bb;
	XU32 nForeColor;
	int	 nBlendMode;
	//.XU32 nBackColor;
public:
	void SwapRB()
	{
#ifdef __ANDROID__
		return;
#endif
		S16 t=ra;
		ra=ba;
		ba=t;
		t=rb;
		rb=bb;
		bb=t;
	}
	void NeedMult(){flags|=needA;}
	void NeedAdd(){flags|=needB;}
	void Clear();
	BOOL HasTransform() { return flags != 0||nForeColor; }
	BOOL HasTransparency() { return flags != 0 && (aa < 256 || ab < 0); }	// return true if this can add transparency
	BOOL HasAlpha() { return flags != 0 && (aa != 256 || ab != 0); }	// return true if this has any alpha info
	void Apply(XU32&color);
	void Apply(RGBI*, int n=1);
	void Apply(GradColorRamp*);
	
	void Concat(XCXForm*);
	void CalcFlags();
	
	SRGB Apply(SRGB);
};

typedef XCXForm ColorTransform;
BOOL Equal(ColorTransform* a, ColorTransform* b);

struct _GRIDINFO
{
	SRECT gridBy;
	SRECT devRect;
	SRECT orgRect;
//#if (__CORE_VERSION__>=0x02078000)
	int   nDepth;
//#endif
	//SRECT* orgRect;
	//bool   bNewGridBy;
	//bool   bNewDev;
};

enum 
{
	glColorSolid, 	// a solid RGB color
	glColorBitmap, 	// a bitmap 
	glColorDib,
	glFrameBuffer,
	glColorGradient, 	// a gradient ramp
	glColorClip, 	// a clip color
	glColorDotFont,
	glColorFont
};
//#include "windows.h"

struct kmMat4;
struct MatDataNode;
class WindowView;
class GLProgram;

//#define GLUCALLBACKTYPE void (*)()

enum enBlendMode 
{
	//enBlendMode_none = 0,
	enBlendMode_normal=0,
	enBlendMode_normal1=1,
	enBlendMode_layer,
	enBlendMode_multiply,
	enBlendMode_screen,
	enBlendMode_lighten,
	enBlendMode_darken,
	enBlendMode_difference,
	enBlendMode_add,
	enBlendMode_subtrace,
	enBlendMode_invert,
	enBlendMode_alpha,
	enBlendMode_erase,
	enBlendMode_overlay,
	enBlendMode_hardlight,
};

enum ENBitmapFilter
{
	en_BTM_FILTER_NONE = 0,
	en_BTM_FILTER_BEVEL,
	en_BTM_FILTER_BLUR,
	en_BTM_FILTER_COLOR_MATRIX,
	en_BTM_FILTER_CONVOLUTION,
	en_BTM_FILTER_DROP_SHADOW,
	en_BTM_FILTER_GLOW,
	en_BTM_FILTER_GRADIENT_BEVEL,
	en_BTM_FILTER_GRADIENT_GLOW
};

typedef struct _ccVertex2F
{
	GLfloat x;
	GLfloat y;
} ccVertex2F;

typedef struct _ccColor4F {
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
} ccColor4F;

typedef struct _ccTex2F {
	GLfloat u;
	GLfloat v;
} ccTex2F;

//! a Point with a vertex point, a tex coord point and a color 4F
typedef struct _ccV2F_C4F_T2F
{
	//! vertices (2F)
	ccVertex2F        vertices;
	//! colors (4F)
	ccColor4F        colors;
	//! tex coords (2F)
	ccTex2F            texCoords;
} ccV2F_C4F_T2F;

struct ccV2F_T2F
{
	ccVertex2F vertices;
	ccTex2F    texCoords;
};

typedef struct _ccV2F_C4F_T2F_Triangle
{
	//! Point A
	ccV2F_C4F_T2F a;
	//! Point B
	ccV2F_C4F_T2F b;
	//! Point B
	ccV2F_C4F_T2F c;
} ccV2F_C4F_T2F_Triangle;

struct LineTriangle
	{
		ccVertex2F v0;ccVertex2F n0;
		ccVertex2F v1;ccVertex2F n1;
		ccVertex2F v2;ccVertex2F n2;
	};
//typedef vector<BwPath*> PathVec;
//typedef vector<BwPath*> PathPtrVec;
//typedef map<const BwPath*, vector<ccV2F_C4F_T2F> > PathPointMap;


enum Texture2DPixelFormat
{
	Texture2DPixelFormat_RGBA8888,
    Texture2DPixelFormat_RGB888,  //! 24-bit texture: RGBA888
    Texture2DPixelFormat_RGB565,  //! 16-bit texture without Alpha channel
    Texture2DPixelFormat_A8,		//! 8-bit textures used as masks
	Texture2DPixelFormat_Max,
};

//enum TextureSize
//{
//	TextureSize_32_32 = 0,
//	TextureSize_64_64,
//	TextureSize_128_128,
//	TextureSize_256_256,
//	TextureSize_512_512,
//	TextureSize_1024_512,
//	TextureSize_512_1024,
//	TextureSize_1024_1024,
//	TextureSize_2048_2048,
//	TextureSize_Max,
//};
struct _TEXTUREID
{
	GLuint id;
	int w,h;
	int tw,th;
	int size;
	int type;
	GLuint nLastTime;
	GLuint nUseCount;
	GLuint nDisplayCount;
	Texture2DPixelFormat fmt;
	//_TEXTUREID* refID;
};

struct TextureInfo
{
	int w,h,nDiv;
	bool preAlpha;
	bool transparent;
	//GLuint id;
	_TEXTUREID* id;
};





class FrameTexture
{
public:
	static FrameTexture* GetFrameTexture(int w,int h);
	inline void FormatRect(XS32 &x, XS32 &y, XRect &rect)
	{
		if(rect.left<0)
		{	x-=rect.left;
			rect.left=0;}
		if(rect.top<0)
		{	y-=rect.top;
			rect.top=0;	}
		if(rect.right>mWidth)
		{	rect.right=mWidth;}
		if(rect.bottom>mHeight)
		{	rect.bottom=mHeight;}
	}
	bool HitTest(XRect&rr,int alpha);
	//bool HitTest(FrameTexture*pFrame,XRect&rr,int alpha,int alpha2);
	void GetPixels(int x,int y,int w,int h,XU8*pData,bool bUpdate=true);
	void GetLixelsLine(int x,int y,int w,int h,XU8*pData,int rowBytes);
	void GetDib(XDrawDib&dib,bool bUpdate=false);
	void GetDib(XDrawDib&dib,int x,int y,int w,int h,bool bUpdate);
	XU32 Threshold(FrameTexture*source,XRect&rect,XPoint&dst,XU32 nOpt,XU32 thre,XU32 mas,XU32 nColor,bool bCopy);
	void SetPixel(int x,int y,XU32 c,XBOOL);
	int Dissolve(XRect&rect,XPoint&dst,int nSeed,int nPixels,XU32 nColor);
	int Dissolve(FrameTexture*source,XRect&rect,XPoint&dst,int nSeed,int nPixels);
	void Scroll(int x,int y);
	void PaletteMap(FrameTexture*source,XRect&rect,XPoint&dst,
					XU8Array&reds,XU8Array&greens,XU8Array&blues,XU8Array&alphas);
	void Noise(int nLow,int nHigh,int nOption,int nGray);
	void Merge(TextureInfo&si,XRect&rect,XPoint&pt,int rm,int gm,int bm,int am);
	void FloodFill(int x,int y,XU32 color);
	void Draw(TextureInfo&si,MATRIX*pMatrix,XCXForm*pForm,_XRECT&rect,bool sm,bool bAlpha=true);
	void Overlay(TextureInfo&si,int dx,int dy,_XRECT&rect);
	void Copy(TextureInfo&si,int dx,int dy,_XRECT&rect,TextureInfo&ai,int adx,int ady);
	void Copy(TextureInfo&si,int dx,int dy,_XRECT&rect);
	void CopyTexture(FrameTexture*frame)
	{CopyTexture(frame,0,0,mWidth,mHeight);}
	void CopyTexture(FrameTexture*frame,int x,int y,int w,int h);
	void DrawBitmap(TextureInfo&info,int x=0,int y=0);
	void ScaleBitmap(TextureInfo&info);
	void DrawBitmap(XU8* pData,int x,int y,int w,int h);//,bool bInv);
	void DrawBitmap(XU8* pData,int w,int h,int bits);//,bool bInv);
	void DrawRect(int x,int y,int w,int h,XU32 color);
	void Transform(int x,int y,int w,int h,XCXForm&form);
	bool Compare(TextureInfo&info,TextureInfo&info2);
	void CopyChannel(TextureInfo&info,_XRECT&rect,SPOINT&pt,int sc,int dc);
public:
	FrameTexture();
	~FrameTexture();
	void SetModify(){bModify=true;}
	void ClearModify(){bModify=false;}
	bool IsModify(){return bModify;}
	void init( int width, int height, bool transparent );
	//void resize( int width, int height );
	void uninit();
	//void setOffRender(bool boff );
	void clear();
	void clear(XU32 color);

	void ReadPixels( int x, int y, int width, int height, void *pdata );

	void beginPaint(bool zoom20=true,bool bFlash=true);
	void endPaint();
	bool isAvailable( ) { return mFbo != 0;};
public:
	
	//GLint mOldFbo;
	//GLint mOldTexId;
	GLuint mFbo;
	//GLuint mTexture;
	_TEXTUREID* mTexture;
	GLuint mRenderDepStenId;
	GLuint mDepthBuffer;
	GLuint mnLastTime;
	int mWidth;
	int mHeight;
	int nImgDiv;
	bool bModify;
	bool transparent;
	bool bPreAlpha;
	//MatDataNode *mpMatNode;

};

class ClipMask
{
public:
	ClipMask();
	~ClipMask();
	void beginPaint();
	void setupClip();
	void setupDraw();
	void endPaint();
	void setInverted( bool binvert );
	
	bool mbInverted;

	static GLint msMask;
	static GLint msStenciBits;

private:
	GLint mask_layer;
	GLint mask_layer_1;
	GLint mask_layer_le;

	GLboolean currentStencilEnabled;
	GLuint currentStencilWriteMask;
	GLenum currentStencilFunc;
	GLint currentStencilRef;
	GLuint currentStencilValueMask;
	GLenum currentStencilFail;
	GLenum currentStencilPassDepthFail;
	GLenum currentStencilPassDepthPass;

};

class Scale9Grid
{
	enum { triangle_cou = 16 };
public:
	Scale9Grid();
	~Scale9Grid();
	void init();
	void unint();
	void initTex(  GLuint texid, float width, float height, float left, float top, float right, float bottom  );
	void drawScale9Grid( float left, float top, float right, float bottom );
	void drawScale9Grid( kmMat4 *mat, float x, float y, float width, float height );
	void renderGrid();

private:
	bool mbDirty;
	int mTriCou;
	ccV2F_T2F mBuffer[triangle_cou];
	GLuint m_uVbo;
	GLuint mTexId;

	float mWidth;
	float mHeight;

	float mLeft;
	float mTop;
	float mRight;
	float mBottom;
	
	
	float rty;
	float lbx;
	float lby;
	float rbx;
	

};

#define kCCUniform_MAX 30
enum GLEdgeMode
{
	_glClampEdge=0,
	_glMirrorEdge=1,
	_glRepeatEdge=2
	
};
enum GLTextureType
{
	_glTexture=0,
	_glFrameTexture=1,
	_glSimple=2,
	_glRefTexture=3,
	_glEmpty = 4,
};
class RasterGL
{
public:
	 static XBOOL IsLocked(_TEXTUREID*id)
	 {
		 if(!id) return false;
		 return id->nUseCount;
	 }
	 static void SetLastTime(_TEXTUREID*id,XU32 t)
	 {
		 if(!id) return;
		 id->nLastTime = t;
	 }
	 static XU32 GetLastTime(_TEXTUREID*id)
	 {
		 if(!id) return 0;
		 return id->nLastTime;
	 }
	 static bool IsLockDisplay(_TEXTUREID*id)
	 {
		 if(!id) return false;
		 return id->nDisplayCount!=0;
	 }
	 static void LockDisplay(_TEXTUREID*id)
	 {
		 if(!id) return;
		 id->nDisplayCount++;
	 }
	 static void UnLockDisplay(_TEXTUREID*id)
	 {
		 if(!id||!id->nDisplayCount) return;
		 id->nDisplayCount--;
	 }
	 static void LockTexture(_TEXTUREID*id)
	 {
		 if(!id) return;
//         if(id->w == 166 && id->h == 77)
//             printf("++ Id:%d, %d\n",id->id, id->nUseCount);
		 id->nUseCount++;
	 }
	 static void UnLockTexture(_TEXTUREID*id)
	 {
		 if(!id||!id->nUseCount) return;
//         if(id->w == 166 && id->h == 77)
//             printf("-- Id:%d, %d\n",id->id, id->nUseCount);
		 id->nUseCount--;
	 }
	 static RasterGL* m_pInstance;
	 static RasterGL* sharedRasterGL();
	 static void	  Release();
	 static void	  ClearCache();
	 bool mbInit,bReset;
	~RasterGL();
	 bool mbFresh;

private:
	RasterGL();
	
public:
	void Uninit();
	static void SetGLMatrix(MATRIX&mat,float*pMat);
	static void SetGLTransform(XCXForm&form,float*pMat);
public:
	//int	 HitTest(TextureInfo&info,_XRECT&r);
	//bool HitTest(TextureInfo&info,_XRECT&rr,int alpha);
	//bool HitTest(TextureInfo&info,TextureInfo&info2,_XRECT&rr,int alpha,int alpha2);
	//void GetPixel(TextureInfo&info,int x,int y,RGB8*color);
	//void GetPixels(TextureInfo&info,_XRECT&rect,XU8*pData);
	void SetCamera(MATRIX&mat);
	void BeginPaint(float* bkColor=0);
	void DrawShape(GLShapeData*pData,float*pTrans,float*matrix,XFilter*pFilter);
	void DrawShapeList(GLShapeListData*pList,MATRIX&mat,XFilter*pFilter,bool isMask=false);
#ifdef _WINEMU
	static int  GetTextureCount();
	static _TEXTUREID* GetTextureByID(int id);
	void GetTexture(_TEXTUREID*id,XDib&dib);
#endif
	//void ReadTexture(SBitmapCore*bits,int id);
	//void ReadTexture(TextureInfo&info,XDib&dib);
	void EndPaint();
	//void DrawClipEnd();

public:
	void DrawButton(float pMat[16],XDib*pDraw,XPOINT&pt,XRECT&rect,float alpha,XDib*pLight);
	void DrawBitmap(SBitmapCore*bits);//,SRECT&dr,SRECT&sr);//
	void DrawFrame(FrameTexture*pFrame,MATRIX&mat,XCXForm&form,SRECT*grid,XFilter*pFilter,MATRIX*pCameraMatrix,enBlendMode blendMode);
	void DrawRectangle(SRECT&rect,MATRIX&mat,float* color);
	void DrawRectangle(float x,float y,float w,float h,float*color,float*matrix);
	void MakeSegment(double* start,double*end,float radius,LineTriangle*output);
	void DrawFont(GLTranglesOrLines*pFL,float*matrix,float*color,float*pTrans,XFilter*pFilter);
	//void DrawFont16(GLTranglesOrLines*pFL);
	//void DrawFont12(GLTranglesOrLines*pFL);
	void TransColor(GLfloat*color,float*pTrans);
	void SetupColor(GLColor*pColor,float*pTrans,XFilter*pFilter,bool isLine=false);
	void SetupFilter(XFilter*pFilter,int width,int height,enBlendMode mode);
	void DrawTrangles(GLTranglesOrLines*pFL,float*matrix,float*pTrans,XFilter*pFilter);//GLdouble*pData,int nCount,BwFillStyle*pFillStyle);
	void DrawRectangle(GLTranglesOrLines*pFL,float*matrix,float*pTrans,XFilter*pFilter);
	void DrawSimpleRect(GLTranglesOrLines*pFL,float*matrix,float*pTrans,bool bFrame);
	void DrawLines(GLTranglesOrLines*pFL,float*matrix,float*pTrans);//GLdouble*pData,int nCount,BwLineStyle*pLineStyle);
	void DrawRLines(GLTranglesOrLines*pFL,float*matrix,float*pTrans);
public:
	void Init( );
	void InitTest(); // for test
	void SetWinSize( int width, int height );
	void drawTriangle(  const ccV2F_C4F_T2F &p1, const ccV2F_C4F_T2F &p2, const ccV2F_C4F_T2F &p3 );
	void drawSegment(const ccVertex2F &from, const ccVertex2F &to, float radius, const ccColor4F &color);
	void drawLine( );
	
	void Render();
	
	void drawOffScreen( bool bOff = false );
	void drawRect( GLfloat *data, GLfloat *color = NULL );
	
	_TEXTUREID* texInitWithData(const void *data, Texture2DPixelFormat pixFormat, int width, int height);//,int lBytes,int pBytes); 
	void   UpdateTexture(int id,const void*data,Texture2DPixelFormat pixFormat,int x,int y,int w,int h);
	void   drawTextureWithMat( GLuint textureId );
	void   drawTextureWithCoord( GLuint texId, GLfloat *data );
	void   DeleteTexture( _TEXTUREID* textureId);//GLuint textureId );

	GLuint GenBuffer();
	void BindBuffer( GLuint vbo);
	void BufferData( GLsizeiptr size, const GLvoid *data );
	void BufferSubData( GLuint offset,GLsizeiptr size, const GLvoid *data );
	void DeleteBuffer( GLuint vbo );

public:  // for test
	void drawRadialGradient( GLuint textureId );
	void drawBlendMode();
	void drawStyleLine();
	void testDrawoffScreen();
	void drawStencil();
	void drawScale9Grid();
	void testFilter();
    void TestDrawArrays();

public:
	void SetObjMatrix(MATRIX&mat)
	{
		m_matrix=mat;
	}
	XSize  m_WinSize,m_startSize;
	float  m_fOrX0,m_fOrY0;
	//GLint  m_uUniforms[kCCUniform_MAX];

	GLuint muProgramArr[100];
    void disbleVertexAttribs( unsigned int flags );
	void enableVertexAttribs( unsigned int flags );

	void setOrthoProjection( int width, int height );
	void setViewPort( int x, int y, int width, int height );
	
private:
	void ensureCapacity(unsigned int count);
	bool InitShader();
	void InitRender();
	GLProgram * SelectProgram( int shaderTyep );
	//bool initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);
	//bool compileShader(GLuint * shader, GLenum type, const GLchar* source);
	void setDepthTest(bool bOn );
	void beginBlendMode(enBlendMode blendmode );
	void endBlendMode();
private:
	unsigned int    m_uBufferCapacity;
	GLsizei         m_nBufferCount;
	ccV2F_C4F_T2F   *m_pBuffer;
	bool m_bDirty;
	enBlendMode mBlendMode;
	MATRIX		    m_matrix,m_camera;
public:
	//GLuint m_uProgram;
	//GLuint m_uVertShader;
	//GLuint m_uFragShader;
	//GLProgram *mCurProgram;
	kmMat4 *mFlashMat;
	kmMat4 *mOrthoMat;

	MatDataNode *mpMatData;
	
	GLuint m_uVbo;
	bool isMask;
	WindowView *mpWinView;

};

#endif
