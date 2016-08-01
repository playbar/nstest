#include "StdAfxflash.h"
#include "Raster_gl.h"
#include <math.h>
//#include <list>
#include "XWindow.h"
#include "WindowView.h"
#include "XSystem.h"
#include "splayer.h"
//#include "shader/bwShader.h"
#include "shader/GLProgram.h"

//#include "png.h"
//#include "shader_vert_frag.h"
#include "SBitmap.h"
//#include "kazmath/gl/matrix.h"
#include "kazmath/mat4.h"
#include "kazmath/vec4.h"

#ifdef __ANDROID__
#define GL_BGRA GL_RGBA
#ifndef max
#define max( a, b ) (((a) > (b)) ? (a) : (b) );
#endif
#endif

#define _MULV 20
#define _CACHEONCE 0xFF
#define SQRT2F  1.4142135
#define SQRT12F 0.70710675
//#define GL_FOR_TEST

//static bool s_bVertexAttribPosition = false;
//static bool s_bVertexAttribColor = false;
//static bool s_bVertexAttribTexCoords = false;
//static bool s_bVertexAttribTexCoords1 = false;

static GLfloat _whiteColorF[]={1,1,1,1};
static GLfloat _glIDMatrix[]={1,0,0,0,
							0,1,0,0,
							0,0,1,0,
							0,0,0,1};
static GLfloat _glEmptyTransform[]={0,0,0,0,
								  0,0,0,0,
								  0,0,0,0,
								  0,0,0,0};


#define TEXTUREID(ID) (ID?ID->id:0)
#define USETEXTUREID(ID) {if(ID) ID->nLastTime=XSWFPlayer::m_pInstance->m_nRunTime;}
static inline ccVertex2F v2fsub(const ccVertex2F &v0, const ccVertex2F &v1)
{
	ccVertex2F ret = {v0.x-v1.x, v0.y-v1.y};
	return ret; 
}

static inline ccVertex2F v2fperp(const ccVertex2F &p0)
{
	ccVertex2F ret = {-p0.y, p0.x};
	return ret; 
}

static inline ccVertex2F v2fnormalize(const ccVertex2F &p)
{
	float flen = sqrtf( p.x * p.x + p.y * p.y );
	ccVertex2F ret = { p.x / flen, p.y / flen };
	return ret; 
}

static inline ccVertex2F v2fmult(const ccVertex2F &v, float s)
{
	ccVertex2F ret = {v.x * s, v.y * s};
	return ret; 
}

static inline ccVertex2F v2fadd(const ccVertex2F &v0, const ccVertex2F &v1)
{
	ccVertex2F ret = {v0.x+v1.x, v0.y+v1.y};
	return ret; 
}


static inline ccVertex2F v2fneg(const ccVertex2F &p0)
{
	ccVertex2F ret = {-p0.x, - p0.y};
	return ret; 
}

static inline ccTex2F __t(const ccVertex2F &v)
{
	return *(ccTex2F*)&v;
}

//////////////////////////////////////////////////////////////////////////


//#define	checkImageWidth 64
//#define	checkImageHeight 64
//static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
//GLuint gTextureId = 0;

//GLuint gTexId_a = 0;
//GLuint gTexId_b = 0;
//GLuint gTexGradient = 0;

//GLuint gFrameBuffer;
//GLuint gMainTexture;
//GLuint rboId; 

struct MatDataNode
{
	kmMat4 flashMat;
	kmMat4 orthoMat;
	int width;
	int heigh;
	GLint oldFbo;
	GLint oldTexId;
	GLint oldRenderId;
	//GLint oldDepSteId;
	MatDataNode *pNext;
};
XFastAlloc nodeAlloc(sizeof(MatDataNode),10);
MatDataNode *gpMatDataStack;

//void makeCheckImage(void)
//{
//	int i, j, c;
//
//	for (i = 0; i < checkImageHeight; i++) {
//		for (j = 0; j < checkImageWidth; j++) {
//			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
//			checkImage[i][j][0] = (GLubyte) c;
//			checkImage[i][j][1] = (GLubyte) c;
//			checkImage[i][j][2] = (GLubyte) c;
//			checkImage[i][j][3] = (GLubyte) 255;
//		}
//	}
//}

#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
	(unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
	((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
	((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
	((unsigned)(unsigned char)(va) << 24))


class XSortTexture:public XSort
{
public:
	XSortTexture()
	{		
	}
	~XSortTexture()
	{
		//Release();
	}
	bool IsEmpty()
	{
		return m_data.GetSize()<=0;
	}

	void SetSize(int nSize)
	{
		m_data.SetSize(nSize);
	}

	void SetValue(_TEXTUREID*id,int index)
	{
		m_data[index] = id;
	}

	void Release(XFastAlloc*alloc)
	{
		int i=0,iCount=m_data.GetSize();
		for(i=0;i<iCount;i++)
		{
			_TEXTUREID* tID = m_data[i];
			glDeleteTextures(1,&tID->id);
			alloc->Free(tID);
		}
		m_data.RemoveAll();
	}
	void Add(_TEXTUREID*tID)
	{
		if(tID == NULL) return;
#ifdef _WINEMU
		for(int i=0;i<m_data.GetSize();i++)
		{
			if(m_data[i]==tID)
				abort();
		}
#endif
		int mi=0;
		int si=Index((void*)tID->size,mi);
		m_data.InsertAt(mi,tID);
		//_MEMINFO*pInfo=NULL;
	}
	int GetBestID(XU32 size)
	{
		int nSize = m_data.GetSize();
		if(nSize<=0) return -1;
		else if(nSize==1) return 0;
		else
		{
			int mi = 0;
			int si=Index((void*)size,mi);
			if(mi>=nSize) mi--;
			else if(mi+1<nSize&&size>m_data[mi]->size)
				mi++;
			return mi;
		}
	}
	void RemoveAt(int index)
	{
		m_data.RemoveAt(index);
	}

	XU32 GetCount(){return m_data.GetSize();}
	_TEXTUREID* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareUInt((XU32)iSrt,(XU32)iDst);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID]->size;
	}
	XVector<_TEXTUREID*> m_data;
	
};
#ifdef _WINEMU
class XSortTextureID:public XSort
{
public:
	XSortTextureID()
	{		
	}
	~XSortTextureID()
	{
		//Release();
	}
	bool IsEmpty()
	{
		return m_data.GetSize()<=0;
	}

	void SetSize(int nSize)
	{
		m_data.SetSize(nSize);
	}

	void Add(_TEXTUREID*tID)
	{
		if(tID == NULL) return;
		int mi=0;
		int si=Index((void*)tID,mi);
		m_data.InsertAt(mi,tID);
		//_MEMINFO*pInfo=NULL;
	}
	void Remove(_TEXTUREID*tID)
	{
		if(tID == NULL) return;
		int mi=0;
		int si=Index((void*)tID,mi);
		if(si>=0)
			m_data.RemoveAt(si);
	}
	
	XU32 GetCount(){return m_data.GetSize();}
	_TEXTUREID* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return CompareUInt((XU32)iSrt,(XU32)iDst);
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return (void*)m_data[nID];
	}
	XVector<_TEXTUREID*> m_data;
	
};
static XSortTextureID _textures;
static void _AddTexture(_TEXTUREID*id)
{
	_textures.Add(id);
}
static void _RemoveTexture(_TEXTUREID*id)
{
	_textures.Remove(id);
}

int  RasterGL::GetTextureCount()
{
	return _textures.GetCount();
}
_TEXTUREID* RasterGL::GetTextureByID(int id)
{
	return _textures.GetAt(id);
}

#else
#define _AddTexture(ID)
#define _RemoveTexture(ID)
#endif
enum _STOCKFRAME
{
	_sf64=0,
	_sf128,
	_sf256,
	_sf512,
	_sf1024,
	_sf2048,
	_sf4096,
	_sfmax
};

const XU8 _XStockTab[]={_sf64,
						_sf128,
						_sf256,_sf256,
						_sf512,_sf512,_sf512,_sf512,
						_sf1024,_sf1024,_sf1024,_sf1024,_sf1024,_sf1024,_sf1024,_sf1024,
						_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,_sf2048,
						_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,
						_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096,_sf4096};
//const int _XStockSize[]={64,128,256,512,1024,2048,4096};

class TextureCache
{
public:
	FrameTexture* _frames[_sfmax];
	FrameTexture* GetFrame(int w,int h)
	{
		int max=w>h?w:h;
		if(max>1024) max=w;
		int id;
		if(max<64)	 id=_sf64;
		if(max>4096) id=_sf4096;//4096;
		else id=_XStockTab[(max-1)>>6];
		if(_frames[id]==XNULL)
		{
			FrameTexture*frame=new FrameTexture();
			_frames[id]=frame;
			int sw=1<<(id+6);
			int sh=sw;
			if(sw>1024&&sh>256) sh=256;
			frame->init(sw,sh,true);
		}
		return _frames[id];
	}
public:
	XFastAlloc idAlloc;
	_TEXTUREID* AllocID()
	{
		return (_TEXTUREID*)idAlloc.Alloc();
	}
	void FreeID(_TEXTUREID*id)
	{
		idAlloc.Free(id);
	}
public:
	static TextureCache* m_pInst;
	//GLuint GenTextures();
	//GLuint GenTextures( Texture2DPixelFormat format, TextureSize size );
	//void DeleteTextures( GLuint id, Texture2DPixelFormat format, TextureSize size );
	//void DeleteTextures( GLuint id );
	//GLuint GenFramebuffers();
	_TEXTUREID* GenTextures(Texture2DPixelFormat format,const void*data,int w,int h);
	void DeleteTextures( _TEXTUREID* id,bool bClear=true);
	GLuint GenFramebuffers();
	void DeleteFramebuffers( GLuint fboid );

	GLuint GenRenderbuffer();
	void DeleteRenderbuffer( GLuint id );
	static TextureCache *shareTexCache();
	static void Release();
private:
	TextureCache();
	~TextureCache();
	
	//XArray<GLuint> mFreeTexBuffers[Texture2DPixelFormat_Max][TextureSize_Max];
	//int mTexSizeStep[Texture2DPixelFormat_Max][TextureSize_Max];
	XSortTexture mFreeTexs;
	//XArray<_TEXTUREID*> mFreeTexs;
	XArray<GLuint> mFreeFrames;
	XArray<GLuint> mFreeRenderbuffer;
};
TextureCache* TextureCache::m_pInst=NULL;
//typedef struct 
//{
//	unsigned char* data;
//	int size;
//	int offset;
//}ImageSource;

//static void memReadFuncPng(png_structp png_ptr, png_bytep data, png_size_t length)
//{
//	ImageSource* isource = (ImageSource*) png_get_io_ptr(png_ptr);
//	if (isource->offset + length <= isource->size)
//	{
//		memcpy(data, isource->data + isource->offset, length);
//		isource->offset += length;
//	}
//	else
//	{
//		png_error(png_ptr, "pngReaderCallback failed");
//	}
//}

//XDrawDib gDrawDib;
//
//void GetImageData( XPCWSTR pFileName )
//{
//	XImage img;
//	img.Create( pFileName );
//	gDrawDib.Create( img, 0 );
//	return;
//}

//unsigned char* BwDecodePngDate(unsigned char* fData, long fSize, int& width, int& height)
//{
//    unsigned char* image_data = NULL;
//#ifdef _WIN32
//	png_structp png_ptr;
//	png_infop info_ptr;
//	int bit_depth, color_type;
//	png_bytep *row_pointers = NULL;
//	int rowbytes;
//
//	/* Create a png read struct */
//	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//	if (!png_ptr)
//	{
//		return NULL;
//	}
//
//	/* Create a png info struct */
//	info_ptr = png_create_info_struct (png_ptr);
//	if (!info_ptr)
//	{
//		png_destroy_read_struct (&png_ptr, NULL, NULL);
//		return NULL;
//	}
//
//	/* Initialize the setjmp for returning properly after a libpng error occured */
//	if (setjmp (png_jmpbuf (png_ptr)))
//	{
//		png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
//		if (row_pointers)
//			free (row_pointers);
//		return NULL;
//	}
//
//	ImageSource imgsource;
//	imgsource.data = fData;
//	imgsource.size = fSize;
//	imgsource.offset = 0;
//	png_set_read_fn(png_ptr, &imgsource, memReadFuncPng);
//
//	/* Read png info */
//	png_read_info (png_ptr, info_ptr);
//
//	/* Get some usefull information from header */
//	bit_depth = png_get_bit_depth (png_ptr, info_ptr);
//	color_type = png_get_color_type (png_ptr, info_ptr);
//
//	/* Convert index color images to RGB images */
//	if (color_type == PNG_COLOR_TYPE_PALETTE)
//		png_set_palette_to_rgb (png_ptr);
//
//	/* Convert RGB images to RGBA images */
//	if (color_type == PNG_COLOR_TYPE_RGB)
//		png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
//
//	/* Convert 1-2-4 bits grayscale images to 8 bits grayscale. */
//	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
//		png_set_gray_1_2_4_to_8 (png_ptr);
//
//	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
//		png_set_tRNS_to_alpha (png_ptr);
//
//	if (bit_depth == 16)
//		png_set_strip_16 (png_ptr);
//	else if (bit_depth < 8)
//		png_set_packing (png_ptr);
//
//	/* Update info structure to apply transformations */
//	png_read_update_info (png_ptr, info_ptr);
//
//	/* Retrieve updated information */
//	png_get_IHDR (png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height, &bit_depth, &color_type, NULL, NULL, NULL);
//
//	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
//	if ((image_data =(unsigned char *) malloc(height * rowbytes)) == NULL)
//	{
//		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//		return NULL;
//	}
//
//	/* Setup a pointer array.  Each one points at the begening of a row. */
//	if ((row_pointers =(png_bytepp) malloc(height * sizeof(png_bytep))) == NULL)
//	{
//		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//		free(image_data);
//		return NULL;
//	}
//	for (int i = 0; i < height; i++)
//		row_pointers[height - 1 - i] = image_data + i*rowbytes;
//
//	/* Read pixel data using row pointers */
//	png_read_image (png_ptr, row_pointers);
//
//	/* Finish decompression and release memory */
//	png_read_end (png_ptr, NULL);
//	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
//
//	/* We don't need row pointers anymore */
//	free (row_pointers);
//
//#endif
//	return image_data;
//
//}
//
//void BindOffScreen()
//{
//	glGenTextures(1, &gMainTexture );
//	glActiveTexture(GL_TEXTURE0 );
//	glBindTexture(GL_TEXTURE_2D, gMainTexture);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
//	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
//	glBindTexture( GL_TEXTURE_2D, 0 );
//
//	//glGenRenderbuffers(1, &rboId);
//	//glBindRenderbuffer(GL_RENDERBUFFER, rboId);
//	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 256, 256);
//	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//	glGenFramebuffers(1, &gFrameBuffer);
//	glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gMainTexture, 0);
//	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
//
//	//GLenum drawBuffers[] = {GL_NONE, GL_COLOR_ATTACHMENT0 };  
//	//glDrawBuffers( 2, drawBuffers);  
//
//	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
//
//	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
//	if(status != GL_FRAMEBUFFER_COMPLETE)
//		int i = 0;
//	return;
//}
//
//void SetDrawOffScreen(bool bOffScreen )
//{
//	if ( bOffScreen )
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
//	}
//	else
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	}
//}

inline static void SwapImage(XU8*pData,int w,int h)
{
	int i,lBytes = (w+3)/4*4*h;
	XU8 tmp;
	for(i=0;i<lBytes;i+=3)
	{
		tmp=pData[i];
		pData[i]=pData[i+2];
		pData[i+2]=tmp;
	}
}

_TEXTUREID* TextureCache::GenTextures(Texture2DPixelFormat format,const void*data,int w,int h)
{
	//if(w==202&&h==201)
	//	int v=0;
	int index = 0;
	int nSize = w*h;
	if(mFreeTexs.IsEmpty())//GetSize()<=0)
	{
		GLuint texIdArr[0xFF];
		glGenTextures( 0xFF, texIdArr );
		//XUINT nOldSize = mFreeTexs.GetSize();
		mFreeTexs.SetSize(0xFF);//,XFALSE);//nOldSize+0xFF,XFALSE);
		for(XUINT i=0;i<0xFF;i++)
		{
			_TEXTUREID* tID = AllocID();
			tID->w = 1;
			tID->h = 1;
			tID->tw = 1;
			tID->th = 1;
			tID->size = 1;
			tID->fmt = Texture2DPixelFormat_RGBA8888;
			tID->id = texIdArr[i];
			tID->nLastTime = 0;
			tID->nUseCount = 0;
			tID->nDisplayCount = 0;
			mFreeTexs.SetValue(tID,i);//[i] = tID;
			//mFreeTexs[nOldSize+i]=texIdArr[i];
			//glBindTexture( GL_TEXTURE_2D, texIdArr[i] );
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)800, (GLsizei)600, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, (GLsizei)800, (GLsizei)600, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL );
		}
		index = 0;
	}
	else 
	{
		index = mFreeTexs.GetBestID(nSize);
	}
	_TEXTUREID* id = mFreeTexs.GetAt(index);//[index];

	if(id->w == w&&id->h == h)
	{
		if(data)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT,4);
			glActiveTexture(GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, TEXTUREID(id) );
			USETEXTUREID(id)
			switch(format)
				{
				case Texture2DPixelFormat_RGBA8888:
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0,(GLsizei)w, (GLsizei)h,  GL_RGBA, GL_UNSIGNED_BYTE, data);
					break;
				case Texture2DPixelFormat_RGB888:
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, (GLsizei)w, (GLsizei)h,  GL_RGB, GL_UNSIGNED_BYTE, data);
					break;
				case Texture2DPixelFormat_RGB565:
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, (GLsizei)w, (GLsizei)h,  GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
					break;
				case Texture2DPixelFormat_A8:
					glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, (GLsizei)w, (GLsizei)h, GL_ALPHA, GL_UNSIGNED_BYTE, data);
					break;
				default:
					break;
				}	
			glBindTexture( GL_TEXTURE_2D,0);
		}
	}
	else
	{
		glActiveTexture(GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, TEXTUREID(id));//->id );
		USETEXTUREID(id)

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST);//GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);//GL_LINEAR );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		switch(format)
		{
		case Texture2DPixelFormat_RGBA8888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)w, (GLsizei)h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, (GLsizei)width, (GLsizei)height, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
			break;
		case Texture2DPixelFormat_RGB888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)w, (GLsizei)h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case Texture2DPixelFormat_RGB565:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)w, (GLsizei)h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
			break;
		case Texture2DPixelFormat_A8:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)w, (GLsizei)h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			break;
		}
		glBindTexture( GL_TEXTURE_2D,0);
	}

	if(id->size<nSize)
		id->size = nSize;
	id->w = w;
	id->h = h;
	id->tw = w;
	id->th = h;
	id->nLastTime = 0;
	id->nUseCount = 0;
	id->nDisplayCount = 0;
	id->fmt = format;
	mFreeTexs.RemoveAt(index);
    if(id->w == 166 && id->h == 77)
        printf("CreateTex ID:%d\n", id->id);
    RasterGL::sharedRasterGL()->LockTexture(id);
    
	return id;
	//int nSize = mFreeTexs.GetSize()-1;
	//GLuint id = mFreeTexs[nSize];
	//mFreeTexs.SetSize(nSize);
	//return id;

	//if(mnTextureIndex>=_CACHEONCE)
	//{
	//	glGenTextures(_CACHEONCE,mTextures);
	//	mnTextureIndex = 0;
	//}
	//GLuint id = mTextures[mnTextureIndex];
	//mnTextureIndex++;
	return id;
}

//int TexCacheSize[TextureSize_Max][2] =
//{
//	{ 32, 32 },
//	{ 64, 64 },
//	{ 128, 128 },
//	{ 256, 256 },
//	{ 512, 512 },
//	{ 1024, 512 },
//	{ 512, 1024 },
//	{ 1024, 1024 },
//	{ 2048, 2048 }
//};
//
//GLuint TextureCache::GenTextures( Texture2DPixelFormat format, TextureSize size )
//{
//	XArray<GLuint> &freeTexSize = mFreeTexBuffers[format][size];
//	if(freeTexSize.GetSize()<=0)
//	{
//		int &step = mTexSizeStep[format][size];
//		if (  step < 256 )
//		{
//			step *= 2;
//		}
//		GLuint *texIdArr = new GLuint[step];
//		glGenTextures( step, texIdArr );
//		freeTexSize.SetSize(step,XFALSE );
//	
//		if ( Texture2DPixelFormat_RGB888 == format )
//		{
//			for ( int i = 0; i < step; i++ )
//			{
//				freeTexSize[i]=texIdArr[i];
//				glBindTexture( GL_TEXTURE_2D, texIdArr[i] );
//				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)TexCacheSize[size][0], 
//					(GLsizei)TexCacheSize[size][1], 0, GL_BGR, GL_UNSIGNED_BYTE, NULL );
//			}
//		}
//		else
//		{
//			for ( int i = 0; i < step; i++ )
//			{
//				freeTexSize[i]=texIdArr[i];
//				glBindTexture( GL_TEXTURE_2D, texIdArr[i] );
//				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)TexCacheSize[size][0], 
//					(GLsizei)TexCacheSize[size][1], 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL );
//			}
//		}
//		delete []texIdArr;
//		
//	}
//	int nSize = freeTexSize.GetSize()-1;
//	GLuint id = freeTexSize[nSize];
//	freeTexSize.SetSize(nSize);
//	//_nTotalTextures++;
//	return id;
//}
//void TextureCache::DeleteTextures( GLuint id, Texture2DPixelFormat format, TextureSize size )
//{
//	mFreeTexBuffers[format][size].Add( id );
//
//}

void TextureCache::DeleteTextures( _TEXTUREID*id,bool bClear)//GLuint id )
{
	if(id<=0)
		return;
    if (id->w == 166 && id->h == 77)
        printf("DeleteId:%d, count:%d\n",id->id, id->nUseCount);
//    if(id->nUseCount || XSWFPlayer::m_pInstance->m_nRunTime - id->nLastTime <= MAXCACHET)
//    {
//        id->nLastTime = XSWFPlayer::m_pInstance->m_nRunTime;
//        return;
//    }
    
    /*if(bClear)
	{
		glDeleteTextures( 1, &id->id );
		FreeID(id);
		return;
	}
	else*/
	if(bClear)
	{
		glBindTexture(GL_TEXTURE_2D, id->id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
		id->w = 1;
		id->h = 1;
		id->tw = 1;
		id->th = 1;
		id->size = 1;
	}

	mFreeTexs.Add(id);
	//mFreeTexs.Add(id);

}

GLuint TextureCache::GenFramebuffers()
{

	if(mFreeFrames.GetSize()<=0)
	{
		GLuint fboArr[0xFF];
		glGenFramebuffers( 0xFF, fboArr );
		XUINT nOldSize = mFreeFrames.GetSize();
		mFreeFrames.SetSize(nOldSize+0xFF,XFALSE);
		for(XUINT i=0;i<0xFF;i++)
			mFreeFrames[nOldSize+i]=fboArr[i];
	}
	int nSize=mFreeFrames.GetSize()-1;
	GLuint id = mFreeFrames[nSize];
	mFreeFrames.SetSize(nSize);
	//return id;

	//if(mnFrameIndex>=_CACHEONCE)
	//{
	//	mnFrameIndex = 0;
	//	glGenFramebuffers(_CACHEONCE,mFrames);
	//} 
	//GLuint id = mFrames[mnFrameIndex]; 
	//mnFrameIndex++;

	return id;
}

void TextureCache::DeleteFramebuffers( GLuint fboid )
{
	if(fboid<=0)
		return;
	//glDeleteFramebuffers(1,&fboid);
	mFreeFrames.Add(fboid);
}

GLuint TextureCache::GenRenderbuffer()
{
	if(mFreeRenderbuffer.GetSize()<=0)
	{
		GLuint renderbuffArr[0xFF];
		glGenRenderbuffers( 0xFF, renderbuffArr );
		XUINT nOldSize = mFreeRenderbuffer.GetSize();
		mFreeRenderbuffer.SetSize(nOldSize+0xFF,XFALSE);
		for(XUINT i=0;i<0xFF;i++)
			mFreeRenderbuffer[nOldSize+i]=renderbuffArr[i];
	}
	int nSize = mFreeRenderbuffer.GetSize()-1;
	GLuint id = mFreeRenderbuffer[nSize];
	mFreeRenderbuffer.SetSize(nSize);
	return id;
}

void TextureCache::DeleteRenderbuffer( GLuint id )
{
	mFreeRenderbuffer.Add(id);
}

void TextureCache::Release()
{
	if(!m_pInst) 
		return;
	delete m_pInst;
	m_pInst=NULL;
}

TextureCache *TextureCache::shareTexCache()
{
	//static TextureCache *pTexCache = NULL;
	if ( m_pInst == NULL )
	{
		m_pInst = new TextureCache;
		//pTexCache->InitTex();
	}
	return m_pInst;
}

TextureCache::TextureCache():idAlloc(sizeof(_TEXTUREID),64)
{
	memset(_frames,0,sizeof(_frames));
	/*for( int i = 0; i < Texture2DPixelFormat_Max; i++ )
	{
		for( int j = 0; j < TextureSize_Max; j++ )
			mTexSizeStep[i][j] = 4;
	}*/
}

TextureCache::~TextureCache()
{
	//if(mnTextureIndex<_CACHEONCE)
	//	glDeleteTextures(_CACHEONCE-mnTextureIndex,mTextures+mnTextureIndex);
	//if(mnFrameIndex<_CACHEONCE)
	//	glDeleteFramebuffers(_CACHEONCE-mnFrameIndex,mFrames+mnFrameIndex);

	//int size = mFreeTexs.GetSize();
	int size,i = 0;
	for(i=0;i<_sfmax;i++)
	{
		if(_frames[i])
			delete _frames[i];
		_frames[i] = 0;
	}
	/*for ( i =0; i < size; i++ )
	{
		glDeleteTextures( 1, &mFreeTexs[i] );
	}
	mFreeTexs.RemoveAll();*/
	mFreeTexs.Release(&idAlloc);

	size = mFreeFrames.GetSize();
	for ( i = 0; i < size; i++ )
	{
		glDeleteFramebuffers( 1, &mFreeFrames[i]);

	}
	mFreeFrames.RemoveAll();

	size = mFreeRenderbuffer.GetSize();
	for( i = 0; i < size; i++ )
	{
		glDeleteRenderbuffers( 1, &mFreeRenderbuffer[i] );
	}
	mFreeRenderbuffer.RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	/*for ( int format = 0; format < Texture2DPixelFormat_Max; format++ )
	{
		for ( int s = 0; s < TextureSize_Max; s++ )
		{
			size = mFreeTexBuffers[format][s].GetSize();
			for ( i =0; i < size; i++ )
			{
				glDeleteTextures( 1, &mFreeTexBuffers[format][s][i] );
			}
		}
	}
	for( int i = 0; i < Texture2DPixelFormat_Max; i++ )
	{
		for( int j = 0; j < TextureSize_Max; j++ )
			mTexSizeStep[i][j] = 0;
	}*/
	return;
}

XU32 FrameTexture::Threshold(FrameTexture*source,XRect&rect,XPoint&dst,XU32 nOpt,XU32 thre,XU32 mas,XU32 nColor,bool bCopy)
{
	SWAPPIXELRB(thre);
	SWAPPIXELRB(mas);
	SWAPPIXELRB(nColor);
	XDrawDib dib;
	//FormatRect(dst.x,dst.y,rect);
	GetDib(dib);//,rect.left,rect.top,rect.Width(),rect.Height(),false);
	XU32 ret = 0;
	//dst.x-=rect.left;
	//dst.y-=rect.top;
	//XRect r1(0,0,rect.Width(),rect.Height());
	if(source==XNULL)
		ret=dib.Threshold(dib,rect,dst,nOpt,thre,mas,nColor,bCopy);
	else
	{
		XDrawDib dib1;
		source->GetDib(dib1);//,rect.left,rect.top,rect.Width(),rect.Height(),false);
		ret=dib.Threshold(dib1,rect,dst,nOpt,thre,mas,nColor,bCopy);
	}
	DrawBitmap(dib.GetBuffer(),mWidth,mHeight,32,false);
	return ret;
}
void FrameTexture::SetPixel(int x,int y,XU32 c,XBOOL)
{
	//return;
	//RasterGL* raster=RasterGL::sharedRasterGL();
	//glBindTexture(TEXTURE_2D,mTexture);
	SWAPPIXELRB(c);
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, TEXTUREID(mTexture));//mTexture->id );
	USETEXTUREID(mTexture)
	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&c);
	glBindTexture( GL_TEXTURE_2D, 0 );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	
	//this->DrawRect(x,y,1,1,c);
	/*beginPaint(false);
	glPixelZoom(1.0,1.0);
	glRasterPos2i(x,y);
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glDrawPixels(1,1,GL_RGBA,GL_UNSIGNED_BYTE,(XU8*)&c);
	//WindowView::sharedWindowView()->SwapBuffers();
	endPaint();*/
}

void FrameTexture::Scroll(int x,int y)
{
	//beginPaint(false);
	//return;
	//XString16 ss;
	//ss.FromInt(y);
	//XSWFPlayer::m_pInstance->Trace(ss);
	FrameTexture frame;
	//frame.init(mWidth,mHeight);
	int w=mWidth,h=mHeight;
	int x1=0,y1=0;
	int x2=0,y2=0;
	if(x>0)
	{
		w-=x;
		x1=0;
		x2=x;
	}
	else
	{
		w+=x;
		x1=-x;
		x2=0;
	}
	if(y>0)
	{
		h-=y;
		y1=0;
		y2=y;
	}
	else
	{
		h+=y;
		y1=-y;
		y2=0;
	}
	frame.init(w,h,transparent);
	frame.CopyTexture(this,x1,y1,w,h);
	//CopyTexture(&frame,x2,y2,w,h);
	TextureInfo info;
	info.w = w;
	info.h = h;
	info.preAlpha = true;
	info.id = frame.mTexture;
	//XRect r(0,0,w,h);
	//Draw(info,NULL,NULL,r,false);
	DrawBitmap(info,x2,y2);
	/*beginPaint(false);
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glRasterPos2i(x,y);
	glCopyPixels(0,0,mWidth,mHeight,GL_COLOR);
	endPaint();*/
	
}


void FrameTexture::CopyTexture(FrameTexture*pFrame,int x,int y,int w,int h)
{
	pFrame->beginPaint(false,false);
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, TEXTUREID(mTexture));//->id );
	USETEXTUREID(mTexture)
	glCopyTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,x,y,w,h,0);
	//glCopyTexSubImage2D( GL_TEXTURE_2D,0,100,100,x,y,w,h);
	glBindTexture( GL_TEXTURE_2D, 0 );
	pFrame->endPaint();
	/*RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false);
	
	glUniform1i((GLint)gProgram[Program_All].m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	int w = pFrame->mWidth;
	int h = pFrame->mHeight;
	glDisable(GL_BLEND);
	//w = w*20;
	//h = h*20;
	
	{
		//glUniformMatrix4fv( (GLint)m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		glActiveTexture( GL_TEXTURE0 );
		//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformPreAlpha],1);
		glBindTexture( GL_TEXTURE_2D,pFrame->mTexture );
		
		GLfloat verts[] = 
		{
			0,0,0.0f,0,0,
			w,0,0.0f,1,0,
			0,h,0.0f,0,1,
			w,h,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		glEnableVertexAttribArray( kCCVertexAttrib_Position);
		glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( kCCVertexAttrib_TexCoords );
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), verts+3 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glBindTexture( GL_TEXTURE_2D,0 );
	}

	endPaint();*/

	/*TextureInfo info;
	info.w = pFrame->mWidth;
	info.h = pFrame->mHeight;
	info.preAlpha = false;
	info.id = pFrame->mTexture;
	DrawBitmap(info,0,0);*/
}

int FrameTexture::Dissolve(XRect&rect,XPoint&dst,int nSeed,int nPixels,XU32 nColor)
{
	XDrawDib dib;
	GetDib(dib);
	int ret=dib.Dissolve(rect,dst,nSeed,nPixels,nColor);
	DrawBitmap(dib.GetBuffer(),mWidth,mHeight,32,false);
	return ret;
}
int FrameTexture::Dissolve(FrameTexture*pFrame,XRect&rect,XPoint&dst,int nSeed,int nPixels)
{
	int ret = 0;
	XDrawDib dib;
	GetDib(dib);
	if(pFrame==XNULL)
		ret=dib.Dissolve(dib,rect,dst,nSeed,nPixels);
	else
	{
		XDrawDib dib1;
		pFrame->GetDib(dib1);
		ret=dib.Dissolve(dib1,rect,dst,nSeed,nPixels);
	}
	DrawBitmap(dib.GetBuffer(),mWidth,mHeight,32,false);
	return ret;
	//return 0;
}

void FrameTexture::PaletteMap(FrameTexture*pFrame,XRect&rect,XPoint&dst,
				XU8Array&reds,XU8Array&greens,XU8Array&blues,XU8Array&alphas)
{
	XDrawDib dib;
	GetDib(dib);
	if(pFrame==XNULL)
		dib.PaletteMap(dib,rect,dst,blues,greens,reds,alphas);
	else
	{
		XDrawDib dib1;
		pFrame->GetDib(dib1);
		dib.PaletteMap(dib1,rect,dst,blues,greens,reds,alphas);
	}
	DrawBitmap(dib.GetBuffer(),mWidth,mHeight,32,false);
}
void FrameTexture::Noise(int nLow,int nHigh,int nOption,int nGray)
{
	XDrawDib dib;
	dib.Create(mWidth,mHeight,32);
	dib.Noise(nLow,nHigh,nOption,nGray);
	DrawBitmap(dib.GetBuffer(),mWidth,mHeight,32,false);
}
void FrameTexture::Merge(TextureInfo&si,XRect&rect,XPoint&pt,int rm,int gm,int bm,int am)
{

	//RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false,false);
	
	//GLProgram::selectShader(Program_BitmapFunc, 0, 0, 0);
	GLProgram::selectBitmapShader(0,0);
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_BitmapFunc);
	float data[]={0,transparent?0:1,rm/255.0,gm/255.0,bm/255.0,am/255.0};
	glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformBMPFunData]), (GLsizei)6,data);
	
	glEnable(GL_BLEND);
	glBlendEquation( GL_FUNC_ADD );
	//if(si.isInv)
	//	glBlendColor(0.9,0.9,0.9,0.9);//rm/255.0,gm/255.0,bm/255.0,am/255.0);
	//if(si.isInv)
	//	glBlendColor(0.9,0.9,0.9,0.9);//glBlendColor(bm/255.0,gm/255.0,rm/255.0,am/255.0);*
	//glBlendFunc( GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR );
	glBlendFunc( GL_ONE,GL_ZERO );
	//glDisable(GL_BLEND);

	int w = rect.right-rect.left;
	int h = rect.bottom-rect.top;
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(si.id));//->id );
		USETEXTUREID(si.id)
		//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformTexture0], si.id);
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(mTexture));//->id );
		USETEXTUREID(mTexture)
		//glUniform1i((GLint)gProgram[Program_All].m_uUniforms[kCCUniformTexture1], mTexture);
		
		GLfloat verts[] = 
		{
			pt.x,pt.y,0.0f,
				(GLfloat)rect.left/si.w,(GLfloat)rect.top/si.h,
				(GLfloat)pt.x/mWidth,(GLfloat)pt.y/mHeight,
			pt.x+w,pt.y,0.0f,
				(GLfloat)rect.right/si.w,(GLfloat)rect.top/si.h,
				(GLfloat)(pt.x+w)/mWidth,(GLfloat)pt.y/mHeight,
			pt.x,pt.y+h,0.0f,
				(GLfloat)rect.left/si.w,(GLfloat)rect.bottom/si.h,
				(GLfloat)pt.x/mWidth,(GLfloat)(pt.y+h)/mHeight,
			pt.x+w,pt.y+h,0.0f,
				(GLfloat)rect.right/si.w,(GLfloat)rect.bottom/si.h,
				(GLfloat)(pt.x+w)/mWidth,(GLfloat)(pt.y+h)/mHeight,
		};
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords | VertexAttrib_TexCoords1 );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  7 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+3 );

		glEnableVertexAttribArray( VertexAttrib_TexCoords1 );
		glVertexAttribPointer(VertexAttrib_TexCoords1, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+5 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );
		glDisableVertexAttribArray( VertexAttrib_TexCoords1 );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	glDisable(GL_BLEND);
	endPaint();
}
void FrameTexture::FloodFill(int x,int y,XU32 color)
{
	SWAPPIXELRB(color);
	XDrawDib dib;
	GetDib(dib,false);
	dib.FloodFill(x,y,color);
	//XRect r(0,0,mWidth,mHeight);
	/*TextureInfo info;
	info.w = mWidth;
	info.h = mHeight;
	info.preAlpha = false;
	RasterGL* gl=RasterGL::sharedRasterGL();
	info.id = gl->texInitWithData(dib.GetBuffer(),kCCTexture2DPixelFormat_RGBA8888,mWidth,mHeight);
	DrawBitmap(info,false);
	gl->DeleteTexture(info.id);*/
	DrawBitmap(dib.GetBuffer(),mWidth,mHeight,32,false);
}


bool FrameTexture::HitTest(XRect&r,int alpha)
{
	XDrawDib dib;
	GetDib(dib,r.left,r.top,r.Width(),r.Height(),true);
	XRect r1(0,0,r.Width(),r.Height());
	return dib.HitTest(r1,alpha)!=0;
	//return false;
}


void FrameTexture::Draw(TextureInfo&si,MATRIX*pMatrix,XCXForm*pForm,_XRECT&rect,bool sm,bool bAlpha)
{
	//return;
	//RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false,false);
	GLProgram::selectShader(Program_VerTexCoords,0,0,pForm&&pForm->flags, 0,false);
	//float color[]={1,1,0,1};
	//glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, color);
	//GLProgram::SetObjMatrix(NULL);
	//GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],si.preAlpha?0:1);
	
	/*if(pMatrix)
	{
		GLfloat mat[16];
		RasterGL::SetGLMatrix(*pMatrix,mat);
		GLProgram::SetObjMatrix(mat);
		//glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, mat);
	}
	else*/
		GLProgram::SetObjMatrix(NULL);
		//glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
	if(pForm&&pForm->flags)
	{
		GLfloat fmat[16];
		RasterGL::SetGLTransform(*pForm,fmat);
		glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,fmat);
	}

	int w = si.w;
	int h = si.h;
	int dw = rect.right - rect.left;
	int dh = rect.bottom - rect.top;
	/*if(dw==19&&dh==34&&pMatrix)
	{
		XString16 ss("--------------- Char a:"),s1;
		s1.FromInt(pMatrix->a);ss+=s1;
		ss+=XString16(",d:");
		s1.FromInt(pMatrix->d);ss+=s1;
		ss+=XString16(",tx:");
		s1.FromInt(pMatrix->tx);ss+=s1;
		ss+=XString16(",ty:");
		s1.FromInt(pMatrix->ty);ss+=s1;
		XSWFPlayer::m_pInstance->Trace(ss);
	}*/
	if(!bAlpha)
		glDisable(GL_BLEND);
	else
	{
		glEnable(GL_BLEND);
		//raster->beginBlendMode
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	}
	//w = w*20;
	//h = h*20;
	
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(si.id));//->id );
		USETEXTUREID(si.id)
		//SRECT r={rect.left,rect.right,rect.top,rect.bottom};
		SPOINT p0={rect.left,rect.top};
		SPOINT p1={rect.right,rect.top};
		SPOINT p2={rect.left,rect.bottom};
		SPOINT p3={rect.right,rect.bottom};
		if(pMatrix)
		{
			MATRIX mat;
			::MatrixInvert(pMatrix,&mat);
			::MatrixTransformPoint(&mat,&p0,&p0);
			::MatrixTransformPoint(&mat,&p1,&p1);
			::MatrixTransformPoint(&mat,&p2,&p2);
			::MatrixTransformPoint(&mat,&p3,&p3);
			//::MatrixTransformRect(&mat,&r,&r);
		}
		/*GLfloat verts[] = 
		{
			0,0,0.0f,(float)rect.left/w,(float)rect.top/h,
			dw,0,0.0f,(float)(rect.left+dw)/w,(float)rect.top/h,
			0,dh,0.0f,(float)rect.left/w,(float)(rect.top+dh)/h,
			dw,dh,0.0f,(float)(rect.left+dw)/w,(float)(rect.top+dh)/h,
		};
		*/
		GLfloat verts[] = 
		{
			rect.left,rect.top,0.0f,(float)p0.x/w,(float)p0.y/h,//(float)r.xmin/w,(float)r.ymin/h,
			rect.left+dw,rect.top,0.0f,(float)p1.x/w,(float)p1.y/h,//(float)(r.xmax)/w,(float)r.ymin/h,
			rect.left,rect.top+dh,0.0f,(float)p2.x/w,(float)p2.y/h,//(float)r.xmin/w,(float)(r.ymax)/h,
			rect.left+dw,rect.top+dh,0.0f,(float)p3.x/w,(float)p3.y/h,//(float)(r.xmax)/w,(float)(r.ymax)/h,
		};

		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), verts+3 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	glDisable(GL_BLEND);
	endPaint();
}

void FrameTexture::Copy(TextureInfo&si,int dx,int dy,_XRECT&rect,TextureInfo&ai,int adx,int ady)
{
	//RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false,false);
	//GLProgram::selectShader(Program_BitmapFunc, 0, 0, 0);
	GLProgram::selectBitmapShader(0,1);
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_BitmapFunc);
	float data[]={1,transparent?0:1,si.transparent?0:1,ai.transparent?0:1};
	glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformBMPFunData]), (GLsizei)4,data);
	
	glEnable(GL_BLEND);
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE,GL_ONE_MINUS_SRC_ALPHA );
	int w = rect.right-rect.left;
	int h = rect.bottom-rect.top;
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(si.id));//->id );
		USETEXTUREID(si.id)
		//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformTexture0], si.id);
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(ai.id));//->id );
		USETEXTUREID(ai.id)
		//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformTexture1], mTexture);
		
		GLfloat verts[] = 
		{
			dx,dy,0.0f,
				(GLfloat)rect.left/si.w,(GLfloat)rect.top/si.h,
				(GLfloat)adx/ai.w,(GLfloat)ady/ai.h,
			dx+w,dy,0.0f,
				(GLfloat)rect.right/si.w,(GLfloat)rect.top/si.h,
				(GLfloat)(adx+w)/ai.w,(GLfloat)ady/ai.h,
			dx,dy+h,0.0f,
				(GLfloat)rect.left/si.w,(GLfloat)rect.bottom/si.h,
				(GLfloat)adx/ai.w,(GLfloat)(ady+h)/ai.h,
			dx+w,dy+h,0.0f,
				(GLfloat)rect.right/si.w,(GLfloat)rect.bottom/si.h,
				(GLfloat)(adx+w)/ai.w,(GLfloat)(ady+h)/ai.h,
		};
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords | VertexAttrib_TexCoords1 );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  7 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+3 );

		glEnableVertexAttribArray( VertexAttrib_TexCoords1 );
		glVertexAttribPointer(VertexAttrib_TexCoords1, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+5 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );
		glDisableVertexAttribArray( VertexAttrib_TexCoords1 );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	glDisable(GL_BLEND);
	endPaint();
}

void FrameTexture::Overlay(TextureInfo&si,int dx,int dy,_XRECT&rect)
{
	MATRIX mat;
	::MatrixIdentity(&mat);
	//mat.tx = -rect.left;
	//mat.ty = -rect.top;
	//_XRECT dr={dx,dy,dx+rect.right-rect.left,dy+rect.bottom-rect.top};
	mat.tx = -rect.left+dx;
	mat.ty = -rect.top+dy;
	_XRECT dr={dx,dy,dx+rect.right-rect.left,dy+rect.bottom-rect.top};
	Draw(si,&mat,NULL,dr,false);
}

void FrameTexture::Copy(TextureInfo&si,int dx,int dy,_XRECT&rect)
{
	//return;
	//if(dy==2) return;
	//if(si.w==1280&&si.h==620)
	//	int v=0;
	MATRIX mat;
	::MatrixIdentity(&mat);
	//dx = 0;
	//dy = 0;
	mat.tx = -rect.left+dx;
	mat.ty = -rect.top+dy;
	_XRECT dr={dx,dy,dx+rect.right-rect.left,dy+rect.bottom-rect.top};
	Draw(si,&mat,NULL,dr,false,false);
}

void FrameTexture::ScaleBitmap(TextureInfo&info)
{
	//RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false,false);
	
	GLProgram::selectShader(Program_VerTexCoords,0, 0, 0, 0,false);
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	int w = info.w;
	int h = info.h;
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
		//raster->beginBlendMode
	//glBlendEquation( GL_ADD );
	//glBlendFunc( GL_ONE, GL_ZERO );
	//glDisable(GL_BLEND);
	//w = w*20;
	//h = h*20;
	
	{
		//glUniformMatrix4fv( (GLint)m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		glActiveTexture( GL_TEXTURE0 );
		glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],info.preAlpha?0:1);
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(info.id));//->id );
		USETEXTUREID(info.id)
		
		GLfloat verts[] = 
		{
			0,0,0.0f,0,0,
			mWidth,0,0.0f,1,0,
			0,mHeight,0.0f,0,1,
			mWidth,mHeight,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), verts+3 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	endPaint();
}

void FrameTexture::DrawBitmap(TextureInfo&info,int x,int y)
{
	//return;
	//RasterGL* raster = RasterGL::sharedRasterGL();
    beginPaint( false,false);
	GLProgram::selectShader(Program_VerTexCoords,0, 0, 0, 0,false );
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType],enShaderType_VerTexCoord);
	//float color[]={1,1,1,1};
	//glUniform4fv( (GLint)raster->mCurProgram->m_uUniforms[kCCUniformColor], 1, color);
	int w = info.w;
	int h = info.h;
	glDisable(GL_BLEND);
	//glEnable(GL_BLEND);
		//raster->beginBlendMode
	//glBlendEquation( GL_ADD );
	//glBlendFunc( GL_ONE, GL_ZERO );
	//glDisable(GL_BLEND);
	//w = w*20;
	//h = h*20;
	
	{
		//glUniformMatrix4fv( (GLint)m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		glActiveTexture( GL_TEXTURE0 );
		glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],info.preAlpha?0:1);
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(info.id));//->id );
		USETEXTUREID(info.id)
		GLfloat verts[] = 
		{
			x,y,0.0f,0,0,
			x+w,y,0.0f,1,0,
			x,y+h,0.0f,0,1,
			x+w,y+h,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), verts+3 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}

	endPaint();
}

void FrameTexture::DrawBitmap(XU8* pData,int x,int y,int w,int h)//,bool bInv)
{
	//return;
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, TEXTUREID(mTexture));//->id );
	USETEXTUREID(mTexture)
	glTexSubImage2D(GL_TEXTURE_2D,0,x,y,w,h,GL_RGBA,GL_UNSIGNED_BYTE,pData);
	glBindTexture( GL_TEXTURE_2D, 0 );
	/*TextureInfo info;
	info.w=w;
	info.h=h;
	info.preAlpha = false;
	RasterGL*gl=RasterGL::sharedRasterGL();
	info.id = gl->texInitWithData(pData,kCCTexture2DPixelFormat_RGBA8888,info.w,info.h);
	DrawBitmap(info,x,y);
	gl->DeleteTexture(info.id);*/
}
void FrameTexture::DrawBitmap(XU8* pData,int w,int h,int bits)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, TEXTUREID(mTexture));//->id );
	USETEXTUREID(mTexture)
	switch(bits)
	{
	default:
	case 32:
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGBA,GL_UNSIGNED_BYTE,pData);
		break;
	case 24:
		//if(bInv) SwapImage(pData,w,h);
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,pData);
		//if(bInv) SwapImage(pData,w,h);
		break;
	case 16:
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGB,GL_UNSIGNED_SHORT_5_6_5,pData);
		break;
	}
	glBindTexture( GL_TEXTURE_2D, 0 );
}
void FrameTexture::DrawRect(int x,int y,int w,int h,XU32 color)
{
	SWAPPIXELRB(color);
	//RasterGL* raster=RasterGL::sharedRasterGL();
	//raster->BeginPaint();
	beginPaint(false,false);
	glDisable(GL_BLEND);
	GLProgram::selectShader(Program_VerColor,0, 0, 0, 0,false );
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor);
	RGB8* rgb=(RGB8*)&color;
	float alpha=rgb->alpha/255.0f;
	float fc[]={rgb->blue/255.0f*alpha,rgb->green/255.0f*alpha,rgb->red/255.0*alpha,rgb->alpha/255.0f};
	//if(fc[3]<=0) fc[3]=0.00001f;
	glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, fc);
	//glDisable(GL_BLEND);
	//int w = pFrame->mWidth;
	//int h = pFrame->mHeight;
	{
		//glUniformMatrix4fv( (GLint)m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		//glActiveTexture( GL_TEXTURE0 );
		//glBindTexture( GL_TEXTURE_2D,pFrame->mTexture );
		
		GLfloat verts[] = 
		{
			x,y,0.0f,
			x+w,y,0.0f,
			x,y+h,0.0f,
			x+w,y+h,0.0f,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  0, verts );
		
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
	//MATRIX mat;
	//::MatrixIdentity(&mat);
	//SRECT r={x,x+w,y+h};
	//raster->DrawRectangle(r,mat,fc);
	endPaint();
	//raster->EndPaint();
}
void FrameTexture::Transform(int x,int y,int w,int h,XCXForm&form)
{
	//glMinmax(GL_MINMAX,GL_RGB,GL_FALSE);
	//glEnable(GL_MINMAX);
	//GLubyte values[6];
	//glGetMinmax(GL_MINMAX,GL_TRUE,GL_RGB,GL_UNSIGNED_BYTE,values);
	//RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false,false);
	GLProgram::selectShader(Program_VerTexCoords,0, 0, 1, 0,false );
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	{
		GLfloat fmat[16];
		RasterGL::SetGLTransform(form,fmat);
		glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,fmat);
	}

	glEnable(GL_BLEND);
		//raster->beginBlendMode
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ZERO );
	//w = w*20;
	//h = h*20;
	
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(mTexture));//->id );
		USETEXTUREID(mTexture)
		GLfloat verts[] = 
		{
			x,y,0.0f,(float)x/mWidth,(float)y/mHeight,
			x+w,y,0.0f,(float)(x+w)/mWidth,(float)y/mHeight,
			x,y+h,0.0f,(float)x/mWidth,(float)(y+h)/mHeight,
			x+w,y+h,0.0f,(float)(x+w)/mWidth,(float)(y+h)/mHeight,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), verts+3 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	glDisable(GL_BLEND);
	endPaint();
}
bool FrameTexture::Compare(TextureInfo&info,TextureInfo&info2)
{
	//RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false,false);
	//GLProgram::selectShader(Program_BitmapFunc, 0, 0, 0);
	GLProgram::selectBitmapShader(0,2);
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_BitmapFunc);
	float data[]={2,transparent?0:1,info.preAlpha?0:1,info2.preAlpha?0:1};
	glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformBMPFunData]), (GLsizei)4,data);
	
	glEnable(GL_BLEND);
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE,GL_ZERO );
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(info.id));//->id );
		USETEXTUREID(info.id)
		//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformTexture0], si.id);
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(info2.id));//->id );
		USETEXTUREID(info2.id)
		//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformTexture1], mTexture);
		
		GLfloat verts[] = 
		{
			0,0,0.0f,
				0,0,
				0,0,
			info.w,0,0.0f,
				1,0,
				1,0,
			0,info.h,0.0f,
				0,1,
				0,1,
			info.w,info.h,0.0f,
				1,1,
				1,1,
		};
		
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords | VertexAttrib_TexCoords1 );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  7 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+3 );

		glEnableVertexAttribArray( VertexAttrib_TexCoords1 );
		glVertexAttribPointer(VertexAttrib_TexCoords1, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+5 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );
		glDisableVertexAttribArray( VertexAttrib_TexCoords1 );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	glDisable(GL_BLEND);
	endPaint();
	XDrawDib dib;
	GetDib(dib);
	int l=dib.Width()*dib.Height();
	XU32*pColor=(XU32*)dib.GetBuffer();
	for(int i=0;i<l;i++)
	{
		if(pColor[i]) return false;
	}
	return true;
}
void FrameTexture::CopyChannel(TextureInfo&info,_XRECT&rect,SPOINT&pt,int sc,int dc)
{
	//return;
	switch(sc)
	{
		case 1:sc=0;break;
		case 2:sc=1;break;
		case 4:sc=2;break;
		case 8:sc=3;break;
		default:return;
	}
	switch(dc)
	{
		case 1:dc=0;break;
		case 2:dc=1;break;
		case 4:dc=2;break;
		case 8:dc=3;break;
		default:return;
	}
	//RasterGL* raster = RasterGL::sharedRasterGL();
	beginPaint(false,false);
	//GLProgram::selectShader(Program_BitmapFunc, 0, 0, 0);
	GLProgram::selectBitmapShader(0,3);
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i((GLint)raster->mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_BitmapFunc);
	
	float data[]={3,0,sc,dc,0,0,0,0};
	glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformBMPFunData]), (GLsizei)4,data);

	glEnable(GL_BLEND);
		//raster->beginBlendMode
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ZERO );
	int w = rect.right-rect.left;
	int h = rect.bottom-rect.top;
	
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(info.id));// );
		USETEXTUREID(info.id)
		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(mTexture));// );
		USETEXTUREID(mTexture)
		GLfloat verts[] = 
		{
			pt.x,pt.y,0.0f,(float)rect.left/info.w,(float)rect.top/info.h,
					(float)pt.x/mWidth,(float)pt.y/mHeight,
			pt.x+w,pt.y,0.0f,(float)rect.right/info.w,(float)rect.top/info.h,
					(float)(pt.x+w)/mWidth,(float)pt.y/mHeight,
			pt.x,pt.y+h,0.0f,(float)rect.left/info.w,(float)rect.bottom/info.h,
					(float)pt.x/mWidth,(float)(pt.y+h)/mHeight,
			pt.x+w,pt.y+h,0.0f,(float)rect.right/info.w,(float)rect.bottom/info.h,
					(float)(pt.x+w)/mWidth,(float)(pt.y+h)/mHeight,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords | VertexAttrib_TexCoords1 );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  7 * sizeof( GLfloat), verts );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+3 );

		glEnableVertexAttribArray( VertexAttrib_TexCoords1 );
		glVertexAttribPointer(VertexAttrib_TexCoords1, 2, GL_FLOAT, GL_FALSE,  7 * sizeof(GLfloat), verts+5 );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );
		glDisableVertexAttribArray( VertexAttrib_TexCoords1 );

		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	glDisable(GL_BLEND);
	endPaint();
}

FrameTexture::FrameTexture()
{
//	mpMatNode = 0;
	mnLastTime = 0;
	nImgDiv = 0;
	mTexture = 0;
	mFbo = 0;
	//mOldFbo = 0;
	mDepthBuffer = 0;
	mWidth = 0;
	mHeight = 0;
	transparent = true;
	bModify = true;
	bPreAlpha = false;
}

FrameTexture::~FrameTexture()
{
	uninit();
}

FrameTexture* FrameTexture::GetFrameTexture(int w,int h)
{
	return TextureCache::shareTexCache()->GetFrame(w,h);
}

void FrameTexture::GetDib(XDrawDib&dib,bool bUpdate)
{
	GetDib(dib,0,0,mWidth,mHeight,bUpdate);
}

void FrameTexture::GetDib(XDrawDib&dib,int x,int y,int w,int h,bool bUpdate)
{
	dib.Create(w,h,32);
	GetPixels(x,y,w,h,dib.GetBuffer(),bUpdate);
}

void FrameTexture::GetLixelsLine(int x,int y,int w,int h,XU8*pData,int rowBytes)
{
	beginPaint(false,false);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	for(int i=0;i<h;i++)
	{
		glReadPixels( x,y+i,w,1, GL_RGBA, GL_UNSIGNED_BYTE, pData );
		pData+=rowBytes;
	}
	endPaint();
}

void FrameTexture::GetPixels(int x,int y,int w,int h, XU8 *pdata,bool bUpdate )
{
	//glBindTexture(
	beginPaint(false,false);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels( x,y,w,h, GL_RGBA, GL_UNSIGNED_BYTE, pdata );
	endPaint();
	if(bUpdate)
	{
		RGB8* pRgb=(RGB8*)pdata;
		int l=w*h;
		int i;
		for(i=0;i<l;i++)
		{
			int t=pRgb->red;
			pRgb->red=pRgb->blue;
			pRgb->blue=t;
			pRgb++;
		}
	}
}

void FrameTexture::ReadPixels( int x, int y, int width, int height, void *pdata )
{
	//glBindTexture(
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels( x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pdata );
}


void FrameTexture::init( int width, int height,bool t )
{
	//if(mpMatNode)
	//	return;
//#ifdef _WINEMU
//	if(width>200)
//		int vv=0;
//	XString16 ss("----- Create Frame Texture W:"),s1;
//	s1.FromInt(width);
//	ss+=s1;ss+=XString16(",H:");
//	s1.FromInt(height);
//	ss+=s1;
//	XSWFPlayer::m_pInstance->Trace(ss);
//#endif
//	if (width == 166 && height == 77)
//		int a = 0;
	mFbo = TextureCache::shareTexCache()->GenFramebuffers();
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	mTexture = TextureCache::shareTexCache()->GenTextures(t?Texture2DPixelFormat_RGBA8888:Texture2DPixelFormat_RGB888,NULL,width,height);
	mTexture->type = 1;
//mTexture->pData = pData;
	transparent = t;
	mWidth = width;
	mHeight = height;
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, TEXTUREID(mTexture));//->id);
	USETEXTUREID(mTexture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	
	if(t)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

	mRenderDepStenId = TextureCache::shareTexCache()->GenRenderbuffer();
	//glGenRenderbuffers(1, &mRenderDepStenId);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderDepStenId);
    // GL_DEPTH24_STENCIL8 = 0x88F0 or GL_DEPTH24_STENCIL8_OES = 0x88F0;
	glRenderbufferStorage(GL_RENDERBUFFER, 0x88F0, mWidth, mHeight );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mRenderDepStenId );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRenderDepStenId );

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEXTUREID(mTexture),0);//->id, 0);
	USETEXTUREID(mTexture)

	int mask_layer = 0x1 << ClipMask::msMask;
	int mask_layer_1 = mask_layer - 1;
	int mask_layer_le = mask_layer | mask_layer_1;

	glClearStencil( mask_layer_le );
	glClear( GL_STENCIL_BUFFER_BIT );

	glClearColor( 0,0,0,0 );
	glClear( GL_COLOR_BUFFER_BIT );
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RasterGL::sharedRasterGL()->bReset = true;
	XSystem::_nTotalFrameMemory+=(mWidth*mHeight*4);
	XSystem::_nTotalFrames++;
	_AddTexture(mTexture);
	return;
}

//void FrameTexture::resize( int width, int height )
//{
//	mWidth = width;
//	mHeight = height;
//	glBindRenderbuffer( GL_RENDERBUFFER, mRenderDepStenId );
//	glRenderbufferStorage( GL_RENDERBUFFER, 0x88F0, mWidth, mHeight );
//
//	glActiveTexture(GL_TEXTURE0 );
//	glBindTexture(GL_TEXTURE_2D, mTexture->id);
//	if(transparent)
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
//	else
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
//	return;
//}



void FrameTexture::uninit()
{
	//if(!mpMatNode)
	//	nodeAlloc.Free(mpMatNode);
	//	return;
	//resize( 1, 1 );
	//XSystem::_nTotalFrameMemory-=(mWidth*mHeight*4);
	//XSystem::_nTotalFrames--;
	//_RemoveTexture(mTexture);
//#ifdef _WINEMU
//	XString16 ss("----- Release Frame Texture W:"),s1;
//	s1.FromInt(mWidth);
//	ss+=s1;ss+=XString16(",H:");
//	s1.FromInt(mHeight);
//	ss+=s1;
//	XSWFPlayer::m_pInstance->Trace(ss);
//#endif

	/*glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderDepStenId);
    // GL_DEPTH24_STENCIL8 = 0x88F0 or GL_DEPTH24_STENCIL8_OES = 0x88F0;
	glRenderbufferStorage(GL_RENDERBUFFER, 0x88F0, 1, 1 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0 );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0 );

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0,0);//->id, 0);

	glBindFramebuffer( GL_FRAMEBUFFER, gpMatDataStack->oldFbo );
	glBindTexture( GL_TEXTURE_2D, gpMatDataStack->oldTexId );
	glBindRenderbuffer(GL_RENDERBUFFER, gpMatDataStack->oldRenderId );

	glViewport( 0, 0, gpMatDataStack->width, gpMatDataStack->heigh );*/
	

	if ( mTexture != 0 )
	{
		_RemoveTexture(mTexture);
		XSystem::_nTotalFrames--;
		XSystem::_nTotalFrameMemory-=mTexture->size*4;
		glDeleteTextures( 1, &mTexture->id );
		TextureCache::shareTexCache()->FreeID(mTexture);
		//TextureCache::shareTexCache()->DeleteTextures( mTexture,false );
		mTexture = 0;
		//if(_textures.GetCount()<2)
		//	int v=0;
		
	}
	//glBindTexture( GL_TEXTURE_2D, 0 );

	

	if ( mRenderDepStenId != 0 )
	{
		 glDeleteRenderbuffers( 1, &mRenderDepStenId );
		//TextureCache::shareTexCache()->DeleteRenderbuffer( mRenderDepStenId );
		 mRenderDepStenId = 0;
	}
	
	if ( mFbo != 0 )
	{
		//TextureCache::shareTexCache()->DeleteFramebuffers( mFbo );
		glDeleteFramebuffers( 1, &mFbo );
		mFbo = 0;
	}

	mWidth = 0;
	mHeight = 0;
	//mOldFbo = 0;
	//delete mpMatNode;
	//mpMatNode = NULL;
}

void FrameTexture::clear()
{
	glClearColor( 0, 0, 0, 0 );
	glClear( GL_COLOR_BUFFER_BIT );
}

void FrameTexture::clear(XU32 color)
{
	RGB8* pRgb=(RGB8*)&color;
	glClear( GL_COLOR_BUFFER_BIT );
	glClearColor(pRgb->blue/255.0, pRgb->green/255.0, pRgb->red/255.0 , pRgb->alpha/255.0);

}


void FrameTexture::beginPaint(bool zoom20,bool bFlash)
{	
	ClipMask::msMask = -1;
	glDisable( GL_STENCIL_TEST );
	//ClipMask::msStenciBits = 7;
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glBindRenderbuffer(GL_RENDERBUFFER, mRenderDepStenId);
	//glStencilMask( 0xFF );

	kmMat4 mOrthoMat;
	if(zoom20)
		kmMat4OrthographicProjection(&mOrthoMat, 0, mWidth*_MULV, 0, mHeight*_MULV, -1024, 1024 );
	else
		kmMat4OrthographicProjection(&mOrthoMat, 0, mWidth, 0, mHeight, -1024, 1024 );
	//kmMat4 mat_mvp;
	//kmMat4Identity( &mat_mvp );
	kmMat4 matFlash = *RasterGL::sharedRasterGL()->mFlashMat;
	if(bFlash)
		kmMat4Multiply( &GLProgram::kMVPMatrix,&mOrthoMat,&matFlash);
	else
		GLProgram::kMVPMatrix=mOrthoMat;

	glViewport( 0, 0, mWidth, mHeight );
	//mpMatNode->orthoMat = mat_mvp;

	glEnable(GL_DITHER);
	RasterGL::sharedRasterGL()->bReset = true;
	return;
}

void FrameTexture::endPaint()
{
	//glClearColor( 0, 0, 1, 1 );
	//glClear( GL_COLOR_BUFFER_BIT );
	//glFlush();
	/*MatDataNode* gpMat = gpMatDataStack;
	gpMatDataStack = gpMatDataStack->pNext;
	nodeAlloc.Free(gpMat);
	if ( gpMatDataStack == NULL )
	{
		abort();
		return;
	}*/
	glBindFramebuffer( GL_FRAMEBUFFER, gpMatDataStack->oldFbo );
	glBindTexture( GL_TEXTURE_2D, gpMatDataStack->oldTexId );
	glBindRenderbuffer(GL_RENDERBUFFER, gpMatDataStack->oldRenderId );

	glViewport( 0, 0, gpMatDataStack->width, gpMatDataStack->heigh );
	//if(ClipMask::msMask>=0)
	//	int v=0;
	//glUniformMatrix4fv( (GLint)(RasterGL::sharedRasterGL()->m_uUniforms[kCCUniformMVPMatrix]), (GLsizei)1, GL_FALSE,gpMatDataStack->orthoMat.mat );
	//glUniformMatrix4fv( (GLint)(RasterGL::sharedRasterGL()->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
	return;
	
}


//FrameTexture gFrameTex;


GLint ClipMask::msMask = -1;
GLint ClipMask::msStenciBits = 7;

ClipMask::ClipMask()
{
	mbInverted = false;

	currentStencilEnabled = GL_FALSE;
	currentStencilWriteMask = ~0;
	currentStencilFunc = GL_ALWAYS;
	currentStencilRef = 0;
	currentStencilValueMask = ~0;
	currentStencilFail = GL_KEEP;
	currentStencilPassDepthFail = GL_KEEP;
	currentStencilPassDepthPass = GL_KEEP;
}

ClipMask::~ClipMask()
{

}

void ClipMask::beginPaint()
{
	currentStencilEnabled = glIsEnabled(GL_STENCIL_TEST);
	glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint *)&currentStencilWriteMask);
	glGetIntegerv(GL_STENCIL_FUNC, (GLint *)&currentStencilFunc);
	glGetIntegerv(GL_STENCIL_REF, &currentStencilRef);
	glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint *)&currentStencilValueMask);
	glGetIntegerv(GL_STENCIL_FAIL, (GLint *)&currentStencilFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint *)&currentStencilPassDepthFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint *)&currentStencilPassDepthPass);
	
	//if(msMask<0)
	glEnable( GL_STENCIL_TEST );
	glDepthMask(GL_FALSE);

	//glEnable( GL_ALPHA_TEST );
	//glAlphaFunc( GL_GREATER, 0 );
}

void ClipMask::setupClip()
{
	if ( msMask  > msStenciBits )
	{
		return;
	}
	msMask ++;
	//msMask = 2;
	mask_layer = 0x1 << msMask;
	mask_layer_1 = mask_layer - 1;
	mask_layer_le = mask_layer | mask_layer_1;

	//glStencilMask( mask_layer );
	//glClearStencil( mask_layer );
	//glClear( GL_STENCIL_BUFFER_BIT );
	//glStencilFunc( GL_ALWAYS, mask_layer, mask_layer );
	//glStencilOp( GL_ZERO, GL_KEEP, GL_KEEP );
	//return;

	if ( mbInverted )
	{
		glStencilMask( mask_layer );
		glClearStencil( 0 );
		glClear( GL_STENCIL_BUFFER_BIT );
		glStencilFunc( GL_NEVER, mask_layer, mask_layer );
		glStencilOp( GL_REPLACE, GL_KEEP, GL_KEEP );
	}
	else
	{
		glStencilMask( mask_layer );
		glClearStencil( mask_layer );
		glClear( GL_STENCIL_BUFFER_BIT );
		glStencilFunc( GL_NEVER, mask_layer, mask_layer );
		glStencilOp( GL_ZERO, GL_KEEP, GL_KEEP );
	}
}
void ClipMask::setupDraw()
{
	glStencilFunc( GL_EQUAL, mask_layer_le, mask_layer_le );
	//glStencilFunc( GL_EQUAL, mask_layer_le, mask_layer_le );
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
}


void ClipMask::endPaint()
{
	//glDisable( GL_ALPHA_TEST );
	glStencilFunc(currentStencilFunc, currentStencilRef, currentStencilValueMask);
	glStencilOp(currentStencilFail, currentStencilPassDepthFail, currentStencilPassDepthPass);
	glStencilMask(currentStencilWriteMask);
	
	/*glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint *)&currentStencilWriteMask);
	glGetIntegerv(GL_STENCIL_FUNC, (GLint *)&currentStencilFunc);
	glGetIntegerv(GL_STENCIL_REF, &currentStencilRef);
	glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint *)&currentStencilValueMask);
	glGetIntegerv(GL_STENCIL_FAIL, (GLint *)&currentStencilFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint *)&currentStencilPassDepthFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint *)&currentStencilPassDepthPass);*/

	msMask--;
	//if(msMask>=0)
	//	int vv=0;
	if(!currentStencilEnabled)//msMask<0)
	{
		glDisable( GL_STENCIL_TEST );
	}		
	else
	{
		glEnable(GL_STENCIL_TEST);
		//setupClip();
		//setupDraw();
		mask_layer = 0x1 << msMask;
		mask_layer_1 = mask_layer - 1;
		mask_layer_le = mask_layer | mask_layer_1;
		//glStencilMask( mask_layer );
		//glStencilFunc( GL_NEVER, mask_layer, mask_layer );
		//glStencilOp( GL_REPLACE, GL_KEEP, GL_KEEP );
		glStencilFunc( GL_EQUAL, mask_layer_le, mask_layer_le );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	}
}

void ClipMask::setInverted( bool binvert )
{
	mbInverted = binvert;
}

//GLint g_sStenciBits;
//ClipMask gClipMask;


Scale9Grid::Scale9Grid()
{
	mbDirty = false;
	m_uVbo = 0;
	mTriCou = 0;
	memset( mBuffer, 0, sizeof( ccV2F_T2F) * triangle_cou );
	//init();
}

Scale9Grid::~Scale9Grid()
{
	unint();
}

void Scale9Grid::init()
{
	glGenBuffers( 1, &m_uVbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_uVbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( ccV2F_T2F) * 36, &mBuffer, GL_STREAM_DRAW );

	//GLProgram::selectShader(Program_VerTexCoords, 0, 0, 0);
	//GLProgram::SetObjMatrix(NULL);

	//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
	glEnableVertexAttribArray( VertexAttrib_Position );
	glVertexAttribPointer( VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, sizeof(ccV2F_T2F), (GLvoid*)offsetof(ccV2F_T2F, vertices));

	glEnableVertexAttribArray( VertexAttrib_TexCoords );
	glVertexAttribPointer( VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof( ccV2F_T2F), (GLvoid*)offsetof( ccV2F_T2F, texCoords));

	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	glDisableVertexAttribArray( VertexAttrib_TexCoords );

	return;
}

void Scale9Grid::unint()
{
	memset( mBuffer, 0, sizeof( ccV2F_T2F) * triangle_cou );
	glDeleteBuffers( 1, &m_uVbo );
	m_uVbo = 0;
}


void Scale9Grid::initTex( GLuint texid, float width, float height, float left, float top, float right, float bottom  )
{
	mTexId = texid;
	mWidth = width;
	mHeight = height;
	if(left<0) left = 0;
	if(top<0) top=0;
	if(right<0) right = 0;
	if(bottom<0) bottom = 0;
	mLeft = left;
	mTop = top;
	mRight = right;
	mBottom = bottom;

}

void Scale9Grid::drawScale9Grid( float left, float top, float right, float bottom )
{
	ccV2F_T2F temp0 = { {left,			top },	{ 0,					0} };		mBuffer[0] = temp0; 
	ccV2F_T2F temp1 = { {left + mLeft,	top },	{ mLeft / mWidth,		0} };		mBuffer[1] = temp1;
	ccV2F_T2F temp2 = { {right - mRight,top },	{ 1 - mRight / mWidth,	0} };		mBuffer[2] = temp2;
	ccV2F_T2F temp3 = { {right,			top },	{ 1,					0} };		mBuffer[3] = temp3;

	ccV2F_T2F temp4 = { {left,			top + mTop},{ 0,				mTop / mHeight}};	mBuffer[4] = temp4;
	ccV2F_T2F temp5 = { {left + mLeft,	top + mTop},{ mLeft / mWidth,	mTop / mHeight} };	mBuffer[5] = temp5;
	ccV2F_T2F temp6 = { {right - mRight,top + mTop},{ 1 - mRight/mWidth,mTop / mHeight} };	mBuffer[6] = temp6;
	ccV2F_T2F temp7 = { {right,			top + mTop},{ 1,				mTop / mHeight} };	mBuffer[7] = temp7;

	ccV2F_T2F temp8 =  { {left,			 bottom - mBottom},{ 0,					 1-mBottom / mHeight}};	mBuffer[8] = temp8;
	ccV2F_T2F temp9 =  { {left + mLeft,	 bottom - mBottom},{ mLeft / mWidth,	 1-mBottom / mHeight}};	mBuffer[9] = temp9;
	ccV2F_T2F temp10 = { {right - mRight,bottom - mBottom},{ 1 - mRight / mWidth,1-mBottom / mHeight}};	mBuffer[10] = temp10;
	ccV2F_T2F temp11 = { {right,		 bottom - mBottom},{ 1,					 1-mBottom / mHeight}};	mBuffer[11] = temp11;

	ccV2F_T2F temp12 = { {left,			 bottom},{ 0,					1} };	mBuffer[12] = temp12;
	ccV2F_T2F temp13 = { {left + mLeft,	 bottom},{ mLeft / mWidth,		1} };	mBuffer[13] = temp13;
	ccV2F_T2F temp14 = { {right - mRight,bottom},{ 1 - mRight / mWidth, 1} };	mBuffer[14] = temp14;
	ccV2F_T2F temp15 = { {right,		 bottom},{ 1,					1} };	mBuffer[15] = temp15;

	mbDirty = true;

}

void Scale9Grid::drawScale9Grid( kmMat4 *mat, float x, float y, float width, float height )
{
	kmVec4 kmout;
	kmVec4 lt = { x, y, 0, 0 };
	kmVec4Transform( &kmout, &lt, mat );
	ccV2F_T2F temp0 = { {kmout.x,kmout.y },	{ 0,					0} };	
	mBuffer[0] = temp0; 

	kmVec4 llt = { x + mLeft, y, 0, 0 };
	kmVec4Transform( &kmout, &llt, mat );
	ccV2F_T2F temp1 = { {kmout.x,kmout.y },	{ mLeft / mWidth,		0} };	
	mBuffer[1] = temp1;

	kmVec4 rrt = { x + width - mRight,  y, 0, 0 };
	kmVec4Transform( &kmout, &rrt, mat );
	ccV2F_T2F temp2 = { { kmout.x, kmout.y },	{ 1 - mRight / mWidth,	0} };		
	mBuffer[2] = temp2;

	kmVec4 rt = { x + width, y, 0, 0 };
	kmVec4Transform( &kmout, &rt, mat );
	ccV2F_T2F temp3 = { { kmout.x, kmout.y },	{ 1,					0} };		
	mBuffer[3] = temp3;

	//////////////////////////////
	kmVec4 ltt = { x, y + mTop, 0, 0 };
	kmVec4Transform( &kmout, &ltt, mat );
	ccV2F_T2F temp4 = { { kmout.x, kmout.y },{ 0,				mTop / mHeight}};	
	mBuffer[4] = temp4;

	kmVec4 lltt = { x + mLeft, y + mTop, 0, 0 };
	kmVec4Transform( &kmout, &lltt, mat );
	ccV2F_T2F temp5 = { { kmout.x, kmout.y },{ mLeft / mWidth,	mTop / mHeight} };	
	mBuffer[5] = temp5;

	kmVec4 rrtt = { x + width - mRight, y + mTop, 0, 0 };
	kmVec4Transform( &kmout, &rrtt, mat );
	ccV2F_T2F temp6 = { { kmout.x, kmout.y },{ 1 - mRight/mWidth,mTop / mHeight} };
	mBuffer[6] = temp6;

	kmVec4 rtt = { x + width, y + mTop, 0, 0 };
	kmVec4Transform( &kmout, &rtt, mat );
	ccV2F_T2F temp7 = { { kmout.x, kmout.y },{ 1,				mTop / mHeight} };	
	mBuffer[7] = temp7;

	//////////////////////////////////////////////////////////////////////////
	kmVec4 lbb = { x, y + height - mBottom, 0, 0 };
	kmVec4Transform( &kmout, &lbb, mat );
	ccV2F_T2F temp8 =  { { kmout.x, kmout.y }, { 0,					 1-mBottom / mHeight}};	
	mBuffer[8] = temp8;

	kmVec4 llbb = { x + mLeft, y + height - mBottom, 0, 0 };
	kmVec4Transform( &kmout, &llbb, mat );
	ccV2F_T2F temp9 =  { { kmout.x,	kmout.y },{ mLeft / mWidth,	 1-mBottom / mHeight}};	
	mBuffer[9] = temp9;

	kmVec4 rrbb = { x + width - mRight, y + height - mBottom, 0, 0 };
	kmVec4Transform( &kmout, &rrbb, mat );
	ccV2F_T2F temp10 = { { kmout.x, kmout.y },{ 1 - mRight / mWidth,1-mBottom / mHeight}};	
	mBuffer[10] = temp10;

	kmVec4 rbb = { x + width, y + height - mBottom, 0, 0 };
	kmVec4Transform( &kmout, &rbb, mat );
	ccV2F_T2F temp11 = { { kmout.x, kmout.y },{ 1,					 1-mBottom / mHeight}};	
	mBuffer[11] = temp11;

	//////////////////////////////////////////////////////////////////////////
	kmVec4 lb = { x, y + height, 0, 0 };
	kmVec4Transform( &kmout, &lb, mat );
	ccV2F_T2F temp12 = { { kmout.x, kmout.y },{ 0,					1} };	
	mBuffer[12] = temp12;

	kmVec4 llb = { x + mLeft, y + height, 0, 0 };
	kmVec4Transform( &kmout, &llb, mat );
	ccV2F_T2F temp13 = { { kmout.x, kmout.y },{ mLeft / mWidth,		1} };	
	mBuffer[13] = temp13;

	kmVec4 rrb = { x + width - mRight, y + height, 0, 0 };
	kmVec4Transform( &kmout, &rrb, mat );
	ccV2F_T2F temp14 = { { kmout.x, kmout.y },{ 1 - mRight / mWidth, 1} };	
	mBuffer[14] = temp14;

	kmVec4 rb = { x + width, y + height, 0, 0 };
	kmVec4Transform( &kmout, &rb, mat );
	ccV2F_T2F temp15 = { { kmout.x, kmout.y },{ 1,					1} };	
	mBuffer[15] = temp15;

	mbDirty = true;
	return;
}


void  Scale9Grid::renderGrid()
{
	//glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	//GLProgram::selectShader(Program_VerTexCoords,false,false,false);
	
	glActiveTexture( GL_TEXTURE0 );
	//glUniform1i( (GLint)m_uUniforms[kCCUniformPreAlpha],0);
	glBindTexture( GL_TEXTURE_2D, mTexId );
	glBindBuffer( GL_ARRAY_BUFFER, m_uVbo );
	if ( mbDirty )
	{
		glBufferData( GL_ARRAY_BUFFER, sizeof( ccV2F_T2F)*triangle_cou, mBuffer, GL_STREAM_DRAW );
		mbDirty = false;
	}

	//RasterGL::sharedRasterGL()->enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
	glEnableVertexAttribArray( VertexAttrib_Position );
	glVertexAttribPointer( VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof( ccV2F_T2F), ( GLvoid*)offsetof(ccV2F_T2F, vertices));

	glEnableVertexAttribArray( VertexAttrib_TexCoords );
	glVertexAttribPointer( VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof( ccV2F_T2F), (GLvoid*)offsetof( ccV2F_T2F, texCoords));

	GLuint indices [] = 
	{
		0, 1, 5,	0, 5, 4,	1, 2, 6,	1, 6, 5,	2, 3, 7,	2, 7, 6,
		4, 5, 9,	4, 9, 8,	5, 6, 10,	5, 10, 9,	6, 7, 11,	6, 11, 10,
		8, 9, 13,	8, 13, 12,	9, 10, 14,	9, 14, 13,	10, 11, 15,	10, 15, 14
	};
	glDrawElements( GL_TRIANGLES, 54, GL_UNSIGNED_INT, indices );
	glDisableVertexAttribArray( VertexAttrib_TexCoords );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	return;
}

//Scale9Grid gScale9Grid;

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//static GLfloat _dot16Rect[]=
//{
//	0,0,0,
//	320,0,0,
//	0,320,0,
//	320,320,0
//};
//static GLfloat _dot16Matrix[]=
//{
//	0.003125,0,0,0,
//	0,0.003125,0,0,
//	0,0,0.003125,0,
//	0,0,0,1
//};
//static GLfloat _dot12Rect[]=
//{
//	0,0,0,
//	240,0,0,
//	0,240,0,
//	240,240,0
//};
//static GLfloat _dot12Matrix[]=
//{
//	0.004166667f,0,0,0,
//	0,0.004166667f,0,0,
//	0,0,0.004166667f,0,
//	0,0,0,1
//};

RasterGL* RasterGL::m_pInstance=NULL;

RasterGL::RasterGL()
{
	mpMatData = mpMatData = (MatDataNode*)nodeAlloc.Alloc();//(MatDataNode*)nodeAlloc.Alloc();
	//new MatDataNode();
	::MatrixIdentity(&m_matrix);
	::MatrixIdentity(&m_camera);
	m_uVbo = 0;
	m_uBufferCapacity = 0;
	m_nBufferCount = 0;
	m_pBuffer = NULL;
	m_bDirty = true;
	mbFresh = false;
	mbInit = false;
	mBlendMode = enBlendMode_normal;
	//mCurProgram = &gProgram[Program_All];

	mFlashMat = (kmMat4*) malloc(sizeof(kmMat4));
	mOrthoMat = ( kmMat4 *)malloc( sizeof( kmMat4));
}

void RasterGL::Release()
{
	if(!m_pInstance) 
		return;
	m_pInstance->Uninit();
	delete m_pInstance;
	m_pInstance = NULL;
}

RasterGL* RasterGL::sharedRasterGL()
{
	//static RasterGL *spRaster = NULL;
	if ( m_pInstance == NULL )
	{
		m_pInstance = new RasterGL;
	}
	return m_pInstance;
}

RasterGL::~RasterGL()
{
	if(mpMatData)
		nodeAlloc.Free(mpMatData);
	free( mFlashMat );
	free( mOrthoMat );

}

void RasterGL::Uninit()
{
	if(!mbInit)
		return;
	/*free(m_pBuffer);
	m_pBuffer = NULL;
    this->m_nBufferCount = 0;

	glDeleteBuffers(1, &m_uVbo);
	m_uVbo = 0;*/
	mbInit = false;
	//delete mpMatData;
	nodeAlloc.Free(mpMatData);
	mpMatData = NULL;
	GLProgram::releaseShaders();
	ClearCache();
}

void RasterGL::Init( )
{
	if(!mpMatData)
		mpMatData = (MatDataNode*)nodeAlloc.Alloc();//new MatDataNode();
	//mBlendMode = 0;
	mpMatData->pNext = NULL;
	gpMatDataStack = mpMatData;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mpMatData->oldFbo);
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &mpMatData->oldRenderId);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &mpMatData->oldTexId);// = 0;

	mbInit = true;
	glGetIntegerv( GL_STENCIL_BITS, &ClipMask::msStenciBits );
	//beginBlendMode( enBlendMode_none );
	//setHint();
	//setDepthTest( true );
	//m_startSize = m_WinSize;
	float max=(m_WinSize.cx>m_WinSize.cy?m_WinSize.cx:m_WinSize.cy);
	m_startSize.cx = max;
	m_startSize.cy = max;
	setOrthoProjection( max*20,max*20 );//m_WinSize.cx *_MULV, m_WinSize.cy *_MULV );
	setViewPort( 0, 0, max,max);//m_WinSize.cx, m_WinSize.cy );
	m_fOrX0=mOrthoMat->mat[12];
	m_fOrY0=mOrthoMat->mat[13];
	SetWinSize(m_WinSize.cx,m_WinSize.cy);
	InitShader();
//	InitRender();

//	InitTest();
//////////////////////////////////////////////////////////////////////////
#ifdef GL_FOR_TEST
//	InitTest();  // for test
#endif
}

void RasterGL::InitTest()
{
//    //ccColor4F color = { 1, 0.8, 0, 0.8 };
//    //ccV2F_C4F_T2F p1 = { {50 * 20, 20 * 20}, color, {0, 0 } };
//    //ccV2F_C4F_T2F p2 = { {250 * 20, 50 * 20}, color, {0, 0}};
//    //ccV2F_C4F_T2F p3 = { {250 * 20, 100 * 20}, color, {0, 0}};
//    //drawTriangle( p1, p2, p3 );
//    //
//    //return;
//
////    ccVertex2F from = { 100, 100 };
////    ccVertex2F to = { 200, 200 };
////    ccColor4F color1 = { 1, 0, 0, 1 };
////    drawSegment( from, to, 20, color1 );
//	//ccVertex2F from1 = { 10, 200 };
//	//ccVertex2F to1 = { 200, 10 };
//	//ccColor4F color1 = { 0, 1, 0, 1 };
//	//drawSegment( from1, to1, 10, color1 );
//
//	//{50.0f,  20.0f, 0.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f},
//	//{250.0f, 50.0f, 0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f},
//
////	ccColor4F color = { 1, 0.8, 0, 0.8 };
////	ccV2F_C4F_T2F p1 = { {50, 20}, color, {0, 0 } };
////	ccV2F_C4F_T2F p2 = { {250, 50}, color, {0, 0}};
////	ccV2F_C4F_T2F p3 = { {250, 100}, color, {0, 0}};
////	drawTriangle( p1, p2, p3 );
//
////	ccV2F_C4F_T2F p4 = { {50, 20},color, {0, 0 } };
////	ccV2F_C4F_T2F p5 = { {250, 100}, color, {0, 0}};
////	ccV2F_C4F_T2F p6 = { {20, 150}, color, {0, 0}};
////	drawTriangle( p4, p5, p6 );
//
//	//for test
//
//	unsigned char *pImgData = NULL;
//
//
//	FILE *pFile = fopen( "d://a.png", "rb" );
//	fseek( pFile, 0, SEEK_END );
//	int ilen = ftell( pFile );
//	fseek( pFile, 0, SEEK_SET );
//	unsigned char *pData = new unsigned char[ilen];
//	fread( pData, ilen, 1, pFile );
//	fclose(pFile );
//	int width = 0;
//	int height = 0;
//	pImgData = BwDecodePngDate( pData, ilen, width, height );
//	gTexId_a = texInitWithData( pImgData, kCCTexture2DPixelFormat_RGBA8888, width, height,_glRepeatEdge );
//	delete []pData;
//	free( pImgData );
//
//
//	//gScale9Grid.init();
//	//gScale9Grid.initTex( gTexId_a, width, height, 5, 5, 5, 5 );
//	//gScale9Grid.drawScale9Grid( 100, 100, 300, 300 );
//
////	pFile = fopen( "d://b.png", "rb" );
////	fseek( pFile, 0, SEEK_END );
////	ilen = ftell( pFile );
////	fseek( pFile, 0, SEEK_SET );
////	pData = new unsigned char[ilen];
////	fread( pData, ilen, 1, pFile );
////	fclose( pFile );
////	pImgData = BwDecodePngDate( pData, ilen, width, height );
////	gTexId_b = texInitWithData( pImgData, kCCTexture2DPixelFormat_RGBA8888, width, height );
////	delete []pData;
////	free( pImgData );
//
//
//	pFile = fopen( "d://gradient.png", "rb" );
//	fseek( pFile, 0, SEEK_END );
//	ilen = ftell( pFile );
//	fseek( pFile, 0, SEEK_SET );
//	pData = new unsigned char[ilen ];
//	fread( pData, ilen, 1, pFile );
//	fclose( pFile );
//	pImgData = BwDecodePngDate( pData, ilen, width, height );
//	gTexGradient = texInitWithData( pImgData, kCCTexture2DPixelFormat_RGBA8888, width, height,_glRepeatEdge );
//	delete []pData;
//	free( pImgData );
//
//
//	/////////////////////////////////////////////////////////////////////////
//
//
//	gFrameTex.init( 256, 256, true );
//	ccV2F_C4F_T2F p1 = { {10*_MULV,  10*_MULV},{ 0.8, 0, 0, 1 }, {0, 0 } };
//	ccV2F_C4F_T2F p2 = { {200*_MULV, 200*_MULV}, {0, 0.5, 0, 1}, {0, 0}};
//	ccV2F_C4F_T2F p3 = { {250*_MULV, 100*_MULV}, {0, 1, 0, 1}, {0, 0}};
//	drawTriangle( p1, p2, p3 );
//	gFrameTex.beginPaint();
//
//	gFrameTex.clear( 0xff00f0ff );
//	SRECT rr={0, 20 * _MULV, 0, 20 * _MULV };
//	MATRIX mat;
//	::MatrixIdentity(&mat);
//	float color[]={0,1,1,1};
//	DrawRectangle(rr,mat,color);
//	Render();
//	//gFrameTex.clear();
//	//drawLine();
//	gFrameTex.endPaint();
//	//////m_nBufferCount = 0;
//
//
//
//
//	//////////////////////////////////////////////////////////////////////////
//
//	//BindOffScreen();
//	//ccV2F_C4F_T2F p1 = { {100, 100},{ 0.8, 0, 0, 1 }, {0, 0 } };
//	//ccV2F_C4F_T2F p2 = { {200, 200}, {0, 0.5, 0, 1}, {0, 0}};
//	//ccV2F_C4F_T2F p3 = { {250, 100}, {0, 1, 0, 1}, {0, 0}};
//	//drawTriangle( p1, p2, p3 );
//	//testDrawoffScreen();
//	//setViewProjection();
//
//
//	//makeCheckImage();
//	//gTextureId = texInitWithData( checkImage, kCCTexture2DPixelFormat_RGBA8888, checkImageWidth, checkImageHeight );
}







GLuint RasterGL::GenBuffer()
{
	XSystem::_nTotalBuffers++;
	GLuint vbo = 0;
	glGenBuffers( 1, &vbo );
	return vbo;
}

void RasterGL::BindBuffer( GLuint vbo)
{
	
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
}

void RasterGL::BufferData( GLsizeiptr size, const GLvoid *data )
{
	XSystem::_nTotalBufferMemory+=size;
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
}

void RasterGL::BufferSubData( GLuint offset,GLsizeiptr size, const GLvoid *data )
{
	glBufferSubData(GL_ARRAY_BUFFER,offset,size, data);
}

void RasterGL::DeleteBuffer( GLuint vbo )
{
	XSystem::_nTotalBuffers--;
	glDeleteBuffers(1,  &vbo );
}

void RasterGL::InitRender()
{
	ensureCapacity( 512 );

	glGenBuffers(1, &m_uVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4F_T2F)* m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);

	int colpos= offsetof(ccV2F_C4F_T2F, colors);
	int texpos = offsetof(ccV2F_C4F_T2F, texCoords);

	//enableVertexAttribs( VertexAttrib_Position | VertexAttrib_Color | VertexAttrib_TexCoords );
	glEnableVertexAttribArray(VertexAttrib_Position);
	glVertexAttribPointer(VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4F_T2F), (GLvoid *)offsetof(ccV2F_C4F_T2F, vertices));

	glEnableVertexAttribArray(VertexAttrib_Color);
	glVertexAttribPointer(VertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, sizeof(ccV2F_C4F_T2F), (GLvoid *)offsetof(ccV2F_C4F_T2F, colors));

	glEnableVertexAttribArray(VertexAttrib_TexCoords);
	glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4F_T2F), (GLvoid *)offsetof(ccV2F_C4F_T2F, texCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray( VertexAttrib_Color );
	glDisableVertexAttribArray( VertexAttrib_TexCoords );

	m_bDirty = true;

}

void RasterGL::drawTriangle( const ccV2F_C4F_T2F &p1, const ccV2F_C4F_T2F &p2, const ccV2F_C4F_T2F &p3 )
{
	unsigned int vertex_count = 3;
	ensureCapacity( vertex_count );
	ccV2F_C4F_T2F_Triangle triangle = { p1, p2, p3 };
	ccV2F_C4F_T2F_Triangle *triangles = (ccV2F_C4F_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
	triangles[0] = triangle;
	m_nBufferCount += vertex_count;
	m_bDirty = true;
	return;
}
void RasterGL::MakeSegment(double* start,double*end,float radius,LineTriangle*triangles)
{
	ccVertex2F a={start[0],start[1]};
	ccVertex2F b={end[0],end[1]};
	ccVertex2F n = v2fnormalize(v2fperp(v2fsub(b, a)));
	ccVertex2F t = v2fperp(n);

	ccVertex2F nw = v2fmult(n, radius);
	ccVertex2F tw = v2fmult(t, radius);
	ccVertex2F v0 = v2fsub(b, v2fadd(nw, tw));
	ccVertex2F v1 = v2fadd(b, v2fsub(nw, tw));
	ccVertex2F v2 = v2fsub(b, nw);
	ccVertex2F v3 = v2fadd(b, nw);
	ccVertex2F v4 = v2fsub(a, nw);
	ccVertex2F v5 = v2fadd(a, nw);
	ccVertex2F v6 = v2fsub(a, v2fsub(nw, tw));
	ccVertex2F v7 = v2fadd(a, v2fadd(nw, tw));


	//LineTriangle* triangles = (LineTriangle*)output;
	//pOutput[0] = v0;
	//pOutput[1] = __t(v2fneg(v2fadd(n, t)));
	//ccV2F_C4F_T2F_Triangle *triangles = (ccV2F_C4F_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
	
	{
		LineTriangle triangles0 =
		{
			v0,(v2fneg(v2fadd(n, t))),
			v1,(v2fsub(n, t)),
			v2,(v2fneg(n)),
		};
		triangles[0] = triangles0;
	}
	{
		LineTriangle triangles1 = {
			v3,(n),
			v1,(v2fsub(n, t)),
			v2,(v2fneg(n)),
		};
		triangles[1] = triangles1;
	}
	{
		LineTriangle triangles2 = {
			v3,(n),
			v4,(v2fneg(n)),
			v2,(v2fneg(n)),
		};
		triangles[2] = triangles2;
	}
	{
		LineTriangle triangles3 = {
			v3,(n),
			v4,(v2fneg(n)),
			v5,(n),
		};
		triangles[3] = triangles3;
	}
	{
		LineTriangle triangles4 = {
			v6,(v2fsub(t, n)),
			v4,(v2fneg(n)),
			v5,(n),
		};
		triangles[4] = triangles4;
	}
	{
		LineTriangle triangles5 = {
			v6,(v2fsub(t, n)),
			v7,(v2fadd(n, t)),
			v5,(n),
		};
		triangles[5] = triangles5;
	}
}
void RasterGL::drawSegment(const ccVertex2F &from, const ccVertex2F &to, float radius, const ccColor4F &color)
{

	unsigned int vertex_count = 6*3;
	ensureCapacity(vertex_count);

	ccVertex2F a = from;
	ccVertex2F b = to;

	ccVertex2F n = v2fnormalize(v2fperp(v2fsub(b, a)));
	ccVertex2F t = v2fperp(n);

	ccVertex2F nw = v2fmult(n, radius);
	ccVertex2F tw = v2fmult(t, radius);
	ccVertex2F v0 = v2fsub(b, v2fadd(nw, tw));
	ccVertex2F v1 = v2fadd(b, v2fsub(nw, tw));
	ccVertex2F v2 = v2fsub(b, nw);
	ccVertex2F v3 = v2fadd(b, nw);
	ccVertex2F v4 = v2fsub(a, nw);
	ccVertex2F v5 = v2fadd(a, nw);
	ccVertex2F v6 = v2fsub(a, v2fsub(nw, tw));
	ccVertex2F v7 = v2fadd(a, v2fadd(nw, tw));


	ccV2F_C4F_T2F_Triangle *triangles = (ccV2F_C4F_T2F_Triangle *)(m_pBuffer + m_nBufferCount);

	ccV2F_C4F_T2F_Triangle triangles0 =
	{
		{v0, color, __t(v2fneg(v2fadd(n, t)))},
		{v1, color, __t(v2fsub(n, t))},
		{v2, color, __t(v2fneg(n))},
	};
	triangles[0] = triangles0;

	ccV2F_C4F_T2F_Triangle triangles1 = {
		{v3, color, __t(n)},
		{v1, color, __t(v2fsub(n, t))},
		{v2, color, __t(v2fneg(n))},
	};
	triangles[1] = triangles1;

	ccV2F_C4F_T2F_Triangle triangles2 = {
		{v3, color, __t(n)},
		{v4, color, __t(v2fneg(n))},
		{v2, color, __t(v2fneg(n))},
	};
	triangles[2] = triangles2;

	ccV2F_C4F_T2F_Triangle triangles3 = {
		{v3, color, __t(n)},
		{v4, color, __t(v2fneg(n))},
		{v5, color, __t(n) },
	};
	triangles[3] = triangles3;

	ccV2F_C4F_T2F_Triangle triangles4 = {
		{v6, color, __t(v2fsub(t, n))},
		{v4, color, __t(v2fneg(n)) },
		{v5, color, __t(n)},
	};
	triangles[4] = triangles4;

	ccV2F_C4F_T2F_Triangle triangles5 = {
		{v6, color, __t(v2fsub(t, n))},
		{v7, color, __t(v2fadd(n, t))},
		{v5, color, __t(n)},
	};
	triangles[5] = triangles5;

	m_nBufferCount += vertex_count;

	m_bDirty = true;
}


void RasterGL::drawLine()
{
	//SelectProgram( Program_VerColor );
	GLProgram::selectShader(Program_VerColor,0, 0, 0, 0,false );
	GLProgram::SetObjMatrix(NULL);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag], 2 );
	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformShaderType], enShaderType_VerColor );
	//GLfloat color[4] = { 1, 0, 0, 0.8 };
	//glUniform4fv( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformColor], 1, color );
	//glUniform1f( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformPointSize], 20.0f);
	glLineWidth( 1 );
	//glPointSize( 20 );

	//GLfloat verts[4][9] = 
	//{
	//	{50.0f,  50.0f, 1024.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f},
	//	{250.0f, 50.0f, -1024.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f},
	//	{50.0f,  250.0f,1024.0f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f},
	//	{250.0f, 250.0f,-1024.0f,	1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f},
	//};
	GLfloat verts[4][9] = 
	{
		{50.0f,  20.0f, 0.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f},
		{250.0f, 50.0f, 0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f},
		{250.0f, 100.0f,0.0f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f},
		{20.0f,  150.0f,0.0f,	1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f},
	};
	
	//enableVertexAttribs( VertexAttrib_Position );
	glEnableVertexAttribArray( VertexAttrib_Position);
	glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  9 * sizeof( GLfloat), &verts[0][0] );

	glDrawArrays( GL_LINE_LOOP, 0, 4 );
	//glDrawArrays( GL_POINTS, 0, 4 );

	//glDisableVertexAttribArray( VertexAttrib_Position );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag], 1 );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag], 0 );

	return;
}

void RasterGL::drawStyleLine()
{
//    kmMat4 mat;
//    kmMat4Identity( &mat );
//    glUniformMatrix4fv( (GLint)(gProgram[Program_All].m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE,  mat.mat );
//    drawStencil();
//    Render();
//    return;
//	//
//	//return;
//	//glEnable( GL_LINE_SMOOTH );
//	//beginBlendMode( enBlendMode_add ); 
//
//	GLfloat verta[36] = 
//	{
//		10.0f,  600.0f,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
//		10.0f,  10.0f, 0.0f,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
//		300.0f, 600.0f,0.0f,	1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,
//		300.0f, 10.0f, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f
//	};
//	drawTextureWithCoord( gTexGradient, verta);
//
//	glEnable(GL_BLEND);
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
//
//	Render();
//	//glEnable( GL_ALPHA_TEST );
//	//glAlphaFunc( GL_LEQUAL, 0.8 );
//#ifdef _WIN32
//	//glBlendFunc( GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA );
//	glEnable (GL_MULTISAMPLE_ARB);
//	glSampleCoverage( 1, true );
//	//glEnable( GL_MULTISAMPLE );
//	//glEnable( GL_SAMPLE_COVERAGE );
//	glEnable(GL_LINE_SMOOTH);
//	glHint(GL_LINE_SMOOTH_HINT , GL_NICEST);  // Antialias the lines
//
//	GLfloat color[4] = { 1, 0.8, 0, 0.8 };
//	glUniform4fv( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformColor], 1, color );
//
//	GLfloat aliColor[4] = { 0.8, 0.64, 0, 0.8 };
//	glUniform4fv( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformAliasingColor], 1, aliColor );
//	drawLine();
//
//	glDisable( GL_LINE_SMOOTH );
//#endif
//
//	//glDisable( GL_ALPHA_TEST );
//	
//	//endBlendMode();
//    glDisable( GL_BLEND );
//	//glDisable( GL_LINE_SMOOTH );

}


void RasterGL::UpdateTexture(int id,const void*data,Texture2DPixelFormat pixFormat,int x,int y,int width,int height)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, id );

	switch(pixFormat)
		{
		case Texture2DPixelFormat_RGBA8888:
			glTexSubImage2D(GL_TEXTURE_2D, 0, x,y,(GLsizei)width, (GLsizei)height,  GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case Texture2DPixelFormat_RGB888:
			glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, (GLsizei)width, (GLsizei)height,  GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case Texture2DPixelFormat_RGB565:
			glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, (GLsizei)width, (GLsizei)height,  GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
			break;
		case Texture2DPixelFormat_A8:
			glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, (GLsizei)width, (GLsizei)height, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			break;
		}	
	glBindTexture( GL_TEXTURE_2D,0);
}

/*static int iTexCount = 0;
static int _texs[2048][2];
static void _Add(int id,int w,int h)
{
	if(iTexCount<=0)
		memset(_texs,0,sizeof(_texs));
	if(id<2048)
	{
		_texs[id][0] = w;
		_texs[id][1] = h;
	}
}

static void _Remove(int id)
{
	if(id<2048)
	{
		_texs[id][0] = 0;
		_texs[id][1] = 0;
	}
}*/

_TEXTUREID* RasterGL::texInitWithData(const void *data, Texture2DPixelFormat pixFormat, int width, int height)//,int lBytes,int pBytes )
{
//#ifdef _WINEMU
//	//if(width==1280&&height==620)
//	//	int v=0;
//	XString16 ss("----- Create Texture W:"),s1;
//	s1.FromInt(width);
//	ss+=s1;ss+=XString16(",H:");
//	s1.FromInt(height);
//	ss+=s1;
//	XSWFPlayer::m_pInstance->Trace(ss);
//#endif
	//if(width==359&&height==252)
	//	int v=0;
	//GLuint texId = 0;
	_TEXTUREID* texId = 0;
	//return texId;
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	//glGenTextures(1, &texId);
//	if(width == 166 && height == 77)
//		int a = 0;
	texId = TextureCache::shareTexCache()->GenTextures(pixFormat,data,width,height);
	texId->type = 0;
//texId->pData = pData;
	//glActiveTexture(GL_TEXTURE0 );
	//glBindTexture( GL_TEXTURE_2D, TEXTUREID(texId));//->id );

	////glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	////glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	////switch(edgeMode)
	////{
	////default:
	////case _glClampEdge:
	////	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	////	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	////	break;
	////
	////case _glRepeatEdge:
	////	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	////	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	////	break;
	////case _glMirrorEdge:
	////	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	////	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
	////	break;
	////}
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST);//GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);//GL_LINEAR );
	////glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	////glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	//switch(pixFormat)
	//{
	//case Texture2DPixelFormat_RGBA8888:
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
	//	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, (GLsizei)width, (GLsizei)height, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
	//	break;
	//case Texture2DPixelFormat_RGB888:
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	break;
	//case Texture2DPixelFormat_RGB565:
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
	//	break;
	//case Texture2DPixelFormat_A8:
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)width, (GLsizei)height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
	//	break;
	//default:
	//	break;
	//}
	//if(bResize)
	//	delete (XU8*)data;
	//_Add(texId,width,height);
	//iTexCount++;
	XSystem::_nTotalTextures++;
	XSystem::_nTotalTextureMemory+=texId->size*4;
	_AddTexture(texId);
	return texId;
}

void RasterGL::DeleteTexture( _TEXTUREID*textureId)//GLuint textureId)
{
	//_Remove(textureId);
	//glDeleteTextures(1, &textureId);
	//iTexCount--;
	if(!textureId) return;

	//if(textureId->w==359&&textureId->h==252)
	//	int v=0;

	XSystem::_nTotalTextures--;
	XSystem::_nTotalTextureMemory-=textureId->size*4;
	_RemoveTexture(textureId);
	
	TextureCache::shareTexCache()->DeleteTextures( textureId );
	return;
}

void RasterGL::drawTextureWithMat( GLuint textureId )
{
	//textureId = gTextureId;
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformShaderType], 4);//enShaderType_VerTexMat );
	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformLineFlag],0);
	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformPreAlpha],0);
	//glUniformMatrix4fv( (GLint)(gProgram[Program_All].m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
	//kmMat4 texMat = 
	//{
	//	206, 0, 0, 0,
	//	0, 138, 0, 0,
	//	0, 0, 1, 0,
	//	0, 0, 0, 1
	//};
	//kmMat4 texMatIn;
	//kmMat4 rotaMat;
	//kmMat4 tranMat;
	//kmMat4Identity( &rotaMat );
	//kmMat4RotationZ( &rotaMat, -60 );
	//kmMat4Identity( &texMatIn );
	//kmMat4Inverse(&texMatIn, &texMat );
	////kmVec4 vetcoord = { 0.0f, 138.0f, 0, 0 };
	////kmMat4Translation( &tranMat, 100, 0.4, 0 );
	////kmMat4Multiply( &texMatIn, &texMatIn, &tranMat );
	////kmMat4 inversMat;
	////kmMat4Inverse( &inversMat, &texMatIn );
	////kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );
	//glUniformMatrix4fv( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, texMatIn.mat );

	//GLfloat verts[4][9] = 
	//{
	//	{0.0f,  138.0f*20,0.0f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f},
	//	{0.0f,  0.0f, 0.0f,	    0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f},
	//	{206.0f*20, 138.0f*20,0.0f,	1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f},
	//	{206.0f*20, 00.0f, 0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f},
	//};

	//glActiveTexture( GL_TEXTURE0 );
	//glBindTexture( GL_TEXTURE_2D, textureId );
	////int ilen = sizeof(ccV2F_C4F_T2F);
	//glEnableVertexAttribArray( VertexAttrib_Position);
	//glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  9 * sizeof( GLfloat), &verts[0][0] );

	////color
	//glEnableVertexAttribArray( VertexAttrib_Color );
	//glVertexAttribPointer(VertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), &verts[0][5]);
	//
	//glEnableVertexAttribArray( VertexAttrib_TexCoords );
	//glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  9 * sizeof(GLfloat), &verts[0][3] );

	////glEnableVertexAttribArray( kCCVertexAttrib_TexCoords );
	////glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  0, tex );

	//
	//glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	//
	//return;

}

void RasterGL::drawTextureWithCoord( GLuint texId, GLfloat *data )
{
	//mCurProgram = SelectProgram( Program_VerTexCoords );
	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
	//glActiveTexture( GL_TEXTURE0 );
	//glBindTexture( GL_TEXTURE_2D, texId );
	////int ilen = sizeof(ccV2F_C4F_T2F);

	//// position
	//glEnableVertexAttribArray( VertexAttrib_Position);
	//glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  9 * sizeof( GLfloat), &data[0] );

	//// texcoord
	//glEnableVertexAttribArray( VertexAttrib_TexCoords );
	//glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  9 * sizeof(GLfloat), &data[3] );

	////color
	//glEnableVertexAttribArray( VertexAttrib_Color );
	//glVertexAttribPointer(VertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), &data[5]);

	////glEnableVertexAttribArray( kCCVertexAttrib_TexCoords );
	////glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  0, tex );


	//glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	//return;
}

void RasterGL::drawRadialGradient( GLuint textureId )
{
	//textureId = gTextureId;
	////glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformShaderType], enShaderType_RadialGradient );
	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformLineFlag],0);
	//glUniform1i( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformPreAlpha],0);
	////glUniform1f( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformDistance], 206.0 );

	//kmMat4 texMat = 
	//{
	//	206, 0, 0, 0,
	//	0, 138, 0, 0,
	//	0, 0, 1, 0,
	//	-100, -50, 0, 1
	//};
	//kmMat4 texMatIn;
	//kmMat4 rotaMat;
	//kmMat4 tranMat;
	//kmMat4Identity( &rotaMat );
	//kmMat4RotationZ( &rotaMat, -60 );
	//kmMat4Identity( &texMatIn );
	//kmMat4Inverse(&texMatIn, &texMat );
	////kmVec4 vetcoord = { 0.0f, 138.0f, 0, 0 };
	////kmMat4Translation( &tranMat, 100, 0.4, 0 );
	////kmMat4Multiply( &texMatIn, &texMatIn, &tranMat );
	////kmMat4 inversMat;
	////kmMat4Inverse( &inversMat, &texMatIn );
	////kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );
	//glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, texMatIn.mat );

	//GLfloat verts[4][9] = 
	//{
	//	{0.0f,  138.0f,0.0f,	-1.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f},
	//	{0.0f,  0.0f, 0.0f,	    -1.0f, -1.0f,		1.0f, 0.0f, 0.0f, 1.0f},
	//	{206.0f, 138.0f,0.0f,	1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f},
	//	{206.0f, 00.0f, 0.0f,	1.0f, -1.0f,		0.0f, 1.0f, 0.0f, 1.0f},
	//};

	//glActiveTexture( GL_TEXTURE0 );
	//glBindTexture( GL_TEXTURE_2D, textureId );
	////int ilen = sizeof(ccV2F_C4F_T2F);
	//glEnableVertexAttribArray( VertexAttrib_Position);
	//glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  9 * sizeof( GLfloat), &verts[0][0] );

	////color
	//glEnableVertexAttribArray( VertexAttrib_Color );
	//glVertexAttribPointer(VertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), &verts[0][5]);

	//glEnableVertexAttribArray( VertexAttrib_TexCoords );
	//glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  9 * sizeof(GLfloat), &verts[0][3] );

	////glEnableVertexAttribArray( kCCVertexAttrib_TexCoords );

	////glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  0, tex );

	//glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	//return;
}

void RasterGL::drawBlendMode()
{

	//glBlendEquation( GL_MAX );
	//glBlendFunc( GL_ONE, GL_ONE );
	
	//glClearColor( 1, 1, 1, 1 );
	//glClear( GL_COLOR_BUFFER_BIT );
	
	GLfloat verta[36] = 
	{
		25.0f,  225.0f,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		25.0f,  25.0f, 0.0f,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		225.0f, 225.0f,0.0f,	1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,
		225.0f, 25.0f, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f
	};
	//drawTextureWithCoord( gTexId_a, verta);



	beginBlendMode(enBlendMode_subtrace);

	GLfloat vertb[36] = 
	{
		100.0f, 300.0f,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		100.0f, 100.0f,  0.0f,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		300.0f, 300.0f,0.0f,	1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,
		300.0f, 100.0f, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f
	};
	//drawTextureWithCoord( gTexId_b, vertb);

	endBlendMode();
	

	return;
}

void RasterGL::SetCamera(MATRIX&mat)
{
	this->SetGLMatrix(mat,mFlashMat->mat);
	MATRIX mat0={fixed_1/20,0,0,fixed_1/20,0,0};
	::MatrixConcat(&mat,&mat0,&m_camera);
	this->mbFresh = true;
	mpMatData->flashMat = *mFlashMat;

	//m_camera=mat;
}

void RasterGL::BeginPaint(float* bkColor)
{
	ClipMask::msMask = -1;
	glDisable( GL_STENCIL_TEST );
	//ClipMask::msStenciBits = 0;
	if(bReset)
	{
		bReset = false;
		glBindFramebuffer( GL_FRAMEBUFFER, gpMatDataStack->oldFbo );
		glBindTexture( GL_TEXTURE_2D, gpMatDataStack->oldTexId );
		glBindRenderbuffer(GL_RENDERBUFFER, gpMatDataStack->oldRenderId );
		glViewport( 0, 0, gpMatDataStack->width, gpMatDataStack->heigh );
		GLProgram::kMVPMatrix = gpMatDataStack->orthoMat;
		//glUniformMatrix4fv( (GLint)(gProgram[Program_All].m_uUniforms[kCCUniformMVPMatrix]), (GLsizei)1, GL_FALSE,gpMatDataStack->orthoMat.mat );
		//glUniformMatrix4fv( (GLint)(RasterGL::sharedRasterGL()->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
		
	}
#ifndef __ANROID__
	if(bkColor)
		glClearColor( bkColor[0],bkColor[1],bkColor[2],bkColor[3]);//1, 1, 1, 1 );
	else
#endif
		glClearColor( 1, 1, 1, 1 );

	glClear( GL_COLOR_BUFFER_BIT );
	kmMat4 mat_mvp;
	kmMat4Identity( &mat_mvp );
	kmMat4Multiply( &mat_mvp,mOrthoMat,mFlashMat);
	//glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformMVPMatrix], (GLsizei)1, GL_FALSE, mat_mvp.mat );
	GLProgram::kMVPMatrix = mat_mvp;
	mpMatData->orthoMat = mat_mvp;
	beginBlendMode(enBlendMode_add);
	glEnable(GL_DITHER);
	return;
}

void RasterGL::EndPaint()
{
	WindowView::sharedWindowView()->SwapBuffers();
	//if(ClipMask::msMask>=0)
	//	int v=0;
}

void RasterGL::TestDrawArrays()
{
    //
    //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], 100 );
    //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag], 0);
    //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
    //
    //GLfloat vertices[] =
    //{
    //    20 * 20.0f,  20*20.0f, 0.0f,
    //    100 * 20.0f, 20*20.0f, 0.0f,
    //    100 * 20.0f,  100 * 20.0f, 0.0f };
    //
    //// Load the vertex data
    ////
    //glEnableVertexAttribArray(VertexAttrib_Position);
    //glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, vertices );
    //
    //
    //// Draw triangle
    ////
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //return;
}

void RasterGL::Render()
{
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], 100 );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag], 0);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
 //   
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
 //   
 //   
 //   TestDrawArrays();
 //   
 //   return;

	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], 100 );

 //   glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
	//if (m_bDirty)
	//{
	//	
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4F_T2F)*m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
	//	m_bDirty = false;
	//}

	////glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
	//// vertex
	//glEnableVertexAttribArray( VertexAttrib_Position );
	//glVertexAttribPointer(VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4F_T2F), (GLvoid *)offsetof(ccV2F_C4F_T2F, vertices));

	//// color
	//glEnableVertexAttribArray( VertexAttrib_Color );
	//glVertexAttribPointer(VertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4F_T2F), (GLvoid *)offsetof(ccV2F_C4F_T2F, colors));

	//// texcood
	//glEnableVertexAttribArray( VertexAttrib_TexCoords );
	//glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4F_T2F), (GLvoid *)offsetof(ccV2F_C4F_T2F, texCoords));

	//glDrawArrays(GL_TRIANGLES, 0, m_nBufferCount );
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
 //   return;
}

void RasterGL::testDrawoffScreen()
{
	//BindOffScreen();
	//glViewport( 0, 0, m_WinSize.cx,m_WinSize.cy );
	//SetDrawOffScreen( true );
	//glViewport( 0, 0, m_WinSize.cx,m_WinSize.cy );

	////kmMat4Identity( mOrthoMat );
	////kmMat4 orthoMatrix;
	//kmMat4OrthographicProjection(mOrthoMat, 0, m_WinSize.cx, 0, m_WinSize.cy, -1024, 1024 );
	////kmMat4Multiply(mOrthoMat, mOrthoMat, &orthoMatrix );
	//kmMat4Identity( mFlashMat );
	////setProjection();
	//BeginPaint();

	////ccV2F_C4F_T2F p1 = { {100, 100},{ 0.8, 0, 0, 1 }, {0, 0 } };
	////ccV2F_C4F_T2F p2 = { {200, 200}, {0, 0.5, 0, 1}, {0, 0}};
	////ccV2F_C4F_T2F p3 = { {300, 100}, {0, 1, 0, 1}, {0, 0}};
	////drawTriangle( p1, p2, p3 );
	//Render();
	//drawLine();
	//SetDrawOffScreen( false );
	//glBindTexture(GL_TEXTURE_2D, gMainTexture);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
}

void RasterGL::drawOffScreen( bool bOff )
{
	//glClearColor( 0, 1, 0, 1 );
	//glClear( GL_COLOR_BUFFER_BIT );
	
	//BindOffScreen();
	//SetDrawOffScreen( true );
	//ccV2F_C4F_T2F p1 = { {100*_MULV, 100*_MULV},{ 0.8, 0, 0, 1 }, {0, 0 } };
	//ccV2F_C4F_T2F p2 = { {200*_MULV, 200*_MULV}, {0, 0.5, 0, 1}, {0, 0}};
	//ccV2F_C4F_T2F p3 = { {300*_MULV, 100*_MULV}, {0, 1, 0, 1}, {0, 0}};
	//drawTriangle( p1, p2, p3 );
	//drawLine();
	//SetDrawOffScreen( false );
	//glBindTexture(GL_TEXTURE_2D, gMainTexture);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//setViewProjection();
	
	//BeginPaint();
	//kmMat4 mat;
	//kmMat4Identity( &mat );
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE,  mat.mat );
	//Render();
	////texDraw( gMainTexture );

	//GLfloat verta[36] = 
	//{
	//	25.0f*_MULV,  150.0f*_MULV,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	//	25.0f*_MULV,  25.0f*_MULV, 0.0f,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	//	150.0f*_MULV, 150.0f*_MULV,0.0f,	1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,
	//	150.0f*_MULV, 25.0f*_MULV, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f
	//};
	//drawTextureWithCoord( gFrameTex.mTexture, verta);

	//drawTextureWithMat( gFrameTex.mTexture );
}

void RasterGL::drawStencil()
{
	//mCurProgram = SelectProgram( Program_All );
	//enableVertexAttribs( VertexAttrib_Position );
	////glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor );
	////glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag], 0);
	////glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
	////glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);

	////gFrameTex.init( 512, 512, true );
	////gFrameTex.beginPaint();

	////gClipMask.beginPaint();
	////gClipMask.setInverted( true );
	////gClipMask.setupClip();

	//GLfloat verts11[12] = 
	//{
	//	50.0f * 20, 50.0f * 20, 0.0f,
	//	100.0f * 20, 50.0f * 20, 0.0f,
	//	50.0f * 20, 100.0f * 20, 0.0f,
	//	100.0f * 20, 100.0f * 20, 0.0f
	//};
	//GLfloat color[4] = { 1, 1, 0, 1 };
	//drawRect( verts11, color );

	////gClipMask.setupDraw();

	////GLfloat verts12[12] = 
	////{
	////	10.0f * 20,  10.0f * 20, 0.0f,
	////	300.0f * 20, 10.0f * 20, 0.0f,
	////	10.0f * 20,  300.0f * 20, 0.0f,
	////	300.0f * 20, 300.0f * 20, 0.0f
	////};
	////GLfloat color1[4] = { 1, 0, 0, 1 };
	////drawRect( verts12, color1 );

	////gClipMask.endPaint();
	////gFrameTex.endPaint();

	////GLfloat verta[36] = 
	////{
	////	25.0f*_MULV,  450.0f*_MULV,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
	////	25.0f*_MULV,  25.0f*_MULV, 0.0f,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
	////	450.0f*_MULV, 450.0f*_MULV,0.0f,	1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,
	////	450.0f*_MULV, 25.0f*_MULV, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f
	////};
	////drawTextureWithCoord( gFrameTex.mTexture, verta);

	//return;


	//static GLint layer = 1;
	////if ( layer + 1 == g_sStenciBits )
	////{
	////	return;
	////}
	//layer ++;
	//GLint mask_layer = 0x1 << layer;
	//GLint mask_layer_1 = mask_layer - 1;
	//GLint mask_layer_le = mask_layer | mask_layer_1;

	//// manually save the stencil state
	//GLboolean currentStencilEnabled = GL_FALSE;
	//GLuint currentStencilWriteMask = ~0;
	//GLenum currentStencilFunc = GL_ALWAYS;
	//GLint currentStencilRef = 0;
	//GLuint currentStencilValueMask = ~0;
	//GLenum currentStencilFail = GL_KEEP;
	//GLenum currentStencilPassDepthFail = GL_KEEP;
	//GLenum currentStencilPassDepthPass = GL_KEEP;
	//currentStencilEnabled = glIsEnabled(GL_STENCIL_TEST);
	//glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint *)&currentStencilWriteMask);
	//glGetIntegerv(GL_STENCIL_FUNC, (GLint *)&currentStencilFunc);
	//glGetIntegerv(GL_STENCIL_REF, &currentStencilRef);
	//glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint *)&currentStencilValueMask);
	//glGetIntegerv(GL_STENCIL_FAIL, (GLint *)&currentStencilFail);
	//glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint *)&currentStencilPassDepthFail);
	//glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint *)&currentStencilPassDepthPass);

	//glEnable( GL_STENCIL_TEST );
	////glStencilMask( 3 );
	////glClearStencil( 0 );
	////glClear( GL_STENCIL_BUFFER_BIT );

	////glStencilFunc( GL_NEVER, 7, 4 );
	////glStencilOp( GL_REPLACE, GL_KEEP, GL_KEEP );

	////GLfloat verts4[12] = 
	////{
	////	50.0f, 50.0f, 0.0f,
	////	60.0f, 50.0f, 0.0f,
	////	50.0f, 60.0f, 0.0f,
	////	60.0f, 60.0f, 0.0f
	////};
	////drawRect( verts4 );

	////////////////////////////////////////////////////////////////////////////

	//glStencilMask( 7 );
	//glClearStencil( 3 );
	//glClear( GL_STENCIL_BUFFER_BIT );
	//GLboolean currentDepthWriteMask = GL_TRUE;
	//glGetBooleanv( GL_DEPTH_WRITEMASK, &currentDepthWriteMask );
	//
	//glStencilFunc( GL_NEVER, mask_layer_1, mask_layer );
	//glStencilOp( GL_ZERO, GL_KEEP, GL_KEEP );

	//GLfloat verts[12] = 
	//{
	//	50.0f, 50.0f, 0.0f,
	//	60.0f, 50.0f, 0.0f,
	//	50.0f, 60.0f, 0.0f,
	//	60.0f, 60.0f, 0.0f
	//};
	//drawRect( verts );
	////Render();

	//glStencilFunc( GL_EQUAL, mask_layer_1, mask_layer_1 );
	//glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	//GLfloat verts1[12] = 
	//{
	//	10.0f,  10.0f, 0.0f,
	//	600.0f, 10.0f, 0.0f,
	//	10.0f,  600.0f, 0.0f,
	//	600.0f, 600.0f, 0.0f
	//};
	//drawRect( verts1 );
	//

	//glStencilFunc(currentStencilFunc, currentStencilRef, currentStencilValueMask);
	//glStencilOp(currentStencilFail, currentStencilPassDepthFail, currentStencilPassDepthPass);
	//glStencilMask(currentStencilWriteMask);

	//glDisable( GL_STENCIL_TEST );
	//layer--;
}

/*void RasterGL::drawStencilTex()
{
	static GLint layer = -1;
	//if ( layer + 1 == g_sStenciBits )
	//{
	//	return;
	//}
	layer ++;
	GLint mask_layer = 0x1 << layer;
	GLint mask_layer_1 = mask_layer - 1;
	GLint mask_layer_le = mask_layer | mask_layer_1;

	// manually save the stencil state
	GLboolean currentStencilEnabled = GL_FALSE;
	GLuint currentStencilWriteMask = ~0;
	GLenum currentStencilFunc = GL_ALWAYS;
	GLint currentStencilRef = 0;
	GLuint currentStencilValueMask = ~0;
	GLenum currentStencilFail = GL_KEEP;
	GLenum currentStencilPassDepthFail = GL_KEEP;
	GLenum currentStencilPassDepthPass = GL_KEEP;
	currentStencilEnabled = glIsEnabled(GL_STENCIL_TEST);
	glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint *)&currentStencilWriteMask);
	glGetIntegerv(GL_STENCIL_FUNC, (GLint *)&currentStencilFunc);
	glGetIntegerv(GL_STENCIL_REF, &currentStencilRef);
	glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint *)&currentStencilValueMask);
	glGetIntegerv(GL_STENCIL_FAIL, (GLint *)&currentStencilFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, (GLint *)&currentStencilPassDepthFail);
	glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, (GLint *)&currentStencilPassDepthPass);

	glEnable( GL_STENCIL_TEST );

	//////////////////////////////////////////////////////////////////////////

	//glStencilMask( mask_layer );
	//glClearStencil( mask_layer );
	//glClear( GL_STENCIL_BUFFER_BIT );
	//glStencilFunc( GL_NEVER, mask_layer, mask_layer );
	//glStencilOp( GL_ZERO, GL_KEEP, GL_KEEP );

	glStencilMask( mask_layer );
	glClearStencil( 0 );
	glClear( GL_STENCIL_BUFFER_BIT );
	glStencilFunc( GL_NEVER, mask_layer, mask_layer );
	glStencilOp( GL_REPLACE, GL_KEEP, GL_KEEP );

	GLfloat verts[12] = 
	{
		100.0f, 100.0f, 0.0f,
		300.0f, 120.0f, 0.0f,
		70.0f, 400.0f, 0.0f,
		450.0f, 460.0f, 0.0f
	};
	drawRect( verts );
	//Render();

	glStencilFunc( GL_EQUAL, mask_layer_le, mask_layer_le );
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

	GLfloat verta[36] = 
	{
		25.0f,  500.0f,0.0f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, 1.0f,
		25.0f,  25.0f, 0.0f,	0.0f, 1.0f,		1.0f, 0.0f, 0.0f, 1.0f,
		500.0f, 500.0f,0.0f,	1.0f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,
		500.0f, 25.0f, 0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f, 1.0f
	};

	drawTextureWithCoord( gTexId_a, verta);

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT , GL_NICEST);  // Antialias the lines
	//glEnable( GL_POLYGON_SMOOTH );
	//glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	 //glEnable(GL_MULTISAMPLE);

	glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineType], enLineType_None );
	glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor );
	//GLfloat color[4] = { 0.1, 0.1, 0.1, 0.8 };
	GLfloat color[4] = { 1, 1, 1, 1 };
	glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, color );
	glLineWidth( 1 );
	GLfloat verts1[4][9] = 
	{
		{100.0f, 100.0f, 0.0f,	0.0f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f},
		{300.0f, 120.0f, 0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f},
		{ 450.0f, 460.0f,0.0f,	1.0f, 1.0f,		1.0f, 1.0f, 0.0f, 1.0f},
		{ 70.0f, 400.0f,0.0f,	0.0f, 1.0f,		0.0f, 0.0f, 1.0f, 1.0f},
	};

	glEnableVertexAttribArray( kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  9 * sizeof( GLfloat), &verts1[0][0] );

	glDrawArrays( GL_LINE_LOOP, 0, 4 );

	

	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_BLEND );


	glStencilFunc(currentStencilFunc, currentStencilRef, currentStencilValueMask);
	glStencilOp(currentStencilFail, currentStencilPassDepthFail, currentStencilPassDepthPass);
	glStencilMask(currentStencilWriteMask);

	glDisable( GL_STENCIL_TEST );
	layer--;
}*/

void RasterGL::drawScale9Grid()
{
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],1);
	////glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformSpreadMode],4);
	//gScale9Grid.renderGrid();
}

void RasterGL::drawRect(GLfloat *data, GLfloat *color )
{
	//SelectProgram( Program_VerColor );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	//GLfloat color[4] = { 1, 1, 0, 1 };
	//glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, color );
	////glUniform1f( (GLint)mCurProgram->m_uUniforms[kCCUniformPointSize], 20.0f);

	////glEnableVertexAttribArray( VertexAttrib_Position);
	//glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  3 * sizeof( GLfloat), data );

	////glDrawArrays( GL_LINE_STRIP, 0, 4 );
	////glDrawArrays( GL_POINTS, 0, 4 );
	//glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

	//return;
}

void RasterGL::ensureCapacity(unsigned int count)
{
	if(m_nBufferCount + count > m_uBufferCapacity)
	{
		m_uBufferCapacity += max(m_uBufferCapacity, count);
		m_pBuffer = (ccV2F_C4F_T2F*)realloc(m_pBuffer, m_uBufferCapacity*sizeof(ccV2F_C4F_T2F));
	}
}


bool RasterGL::InitShader()
{
	GLProgram::initShaders();
	//mCurProgram = GLProgram::selectShader(Program_VerColor,false,false,false);
	//////////////////////////////////////////////////////////////////////////
	/*gProgram[Program_VerColor].initWithVertexShaderByteArray( VerColor_vert, VerColor_frag );
	gProgram[Program_VerColor].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_VerColor].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_VerColor].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_VerColor].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_VerColor].link();
	gProgram[Program_VerColor].updateUniforms();

	//////////////////////////////////////////////////////////////////////////
	gProgram[Program_VerTexMat].initWithVertexShaderByteArray( VerTexMat_vert, VerTexMat_frag );
	gProgram[Program_VerTexMat].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_VerTexMat].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_VerTexMat].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_VerTexMat].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_VerTexMat].link();
	gProgram[Program_VerTexMat].updateUniforms();

	//////////////////////////////////////////////////////////////////////////
	gProgram[Program_RadialGradient].initWithVertexShaderByteArray( RadialGradient_vert, RadialGradient_frag );
	gProgram[Program_RadialGradient].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_RadialGradient].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_RadialGradient].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_RadialGradient].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_RadialGradient].link();
	gProgram[Program_RadialGradient].updateUniforms();

	//////////////////////////////////////////////////////////////////////////
	gProgram[Program_VerTexCoords].initWithVertexShaderByteArray( VerTexCoords_vert, VerTexCoords_frag );
	gProgram[Program_VerTexCoords].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_VerTexCoords].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_VerTexCoords].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_VerTexCoords].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_VerTexCoords].link();
	gProgram[Program_VerTexCoords].updateUniforms();

	//////////////////////////////////////////////////////////////////////////
	gProgram[Program_Font].initWithVertexShaderByteArray( Font_vert, Font_frag );
	gProgram[Program_Font].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_Font].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_Font].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_Font].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_Font].link();
	gProgram[Program_Font].updateUniforms();

	//////////////////////////////////////////////////////////////////////////
	gProgram[Program_BitmapFunc].initWithVertexShaderByteArray( BitmapFunc_vert, BitmapFunc_frag );
	gProgram[Program_BitmapFunc].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_BitmapFunc].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_BitmapFunc].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_BitmapFunc].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_BitmapFunc].link();
	gProgram[Program_BitmapFunc].updateUniforms();

	//////////////////////////////////////////////////////////////////////////
	gProgram[Program_Filter].initWithVertexShaderByteArray( Filter_vert, Filter_frag );
	gProgram[Program_Filter].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_Filter].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_Filter].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_Filter].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_Filter].link();
	gProgram[Program_Filter].updateUniforms();
  
	//////////////////////////////////////////////////////////////////////////
	gProgram[Program_Test].initWithVertexShaderByteArray( Test_vert, Test_frag );
	gProgram[Program_Test].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_Test].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_Test].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_Test].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_Test].link();
	gProgram[Program_Test].updateUniforms();

	////////////////////////////////////////////////////////////////////////////
	gProgram[Program_All].initWithVertexShaderByteArray( ccPositionTextureColor_vert, ccPositionTextureColor_frag );
	gProgram[Program_All].BindAttribLocation( VertexAttrib_Position, kCCAttributeNamePosition );
	gProgram[Program_All].BindAttribLocation( VertexAttrib_Color, kCCAttributeNameColor);
	gProgram[Program_All].BindAttribLocation( VertexAttrib_TexCoords, kCCAttributeNameTexCoord );
	gProgram[Program_All].BindAttribLocation( VertexAttrib_TexCoords1, kCCAttributeNameTexCoord1 );
	gProgram[Program_All].link();
	gProgram[Program_All].updateUniforms();

	mCurProgram = &gProgram[ Program_Test ];
	mCurProgram->use();*/
	return true;

}

void RasterGL::setOrthoProjection( int width, int height )
{
	kmMat4Identity( mOrthoMat );
#ifdef GL_FOR_TEST
	kmMat4OrthographicProjection(mOrthoMat, 0, m_WinSize.cx, m_WinSize.cy, 0, -1024, 1024 );
	//kmMat4OrthographicProjection(mOrthoMat, 0, m_WinSize.cx*_MULV, m_WinSize.cy*_MULV, 0, -1024, 1024 );
#else
	kmMat4OrthographicProjection(mOrthoMat, 0, width, height, 0, -1024, 1024 );
#endif
	mpMatData->orthoMat = *mOrthoMat;
	kmMat4Identity( mFlashMat );
}

void RasterGL::setDepthTest(bool bOn)
{
	if (bOn)
	{
		glClearDepthf( 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc( GL_LEQUAL );
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void RasterGL::enableVertexAttribs( unsigned int flags )
{
	bool bPosition = flags & VertexAttrib_Position;
	//if ( bPosition != s_bVertexAttribPosition )
	{
		if ( bPosition )
			glEnableVertexAttribArray( VertexAttrib_Position );
		else
			glDisableVertexAttribArray( VertexAttrib_Position );
		//s_bVertexAttribPosition = bPosition;
	}

	bool bColor = flags & VertexAttrib_Color;
	//if ( bColor != s_bVertexAttribColor )
	{
		if ( bColor )
		{
			glEnableVertexAttribArray( VertexAttrib_Color );
		}
		else
			glDisableVertexAttribArray( VertexAttrib_Color );
		//s_bVertexAttribColor = bColor;
	}
	bool bTexCoords = flags & VertexAttrib_TexCoords;
	//if ( bTexCoords != s_bVertexAttribTexCoords )
	{
		if ( bTexCoords )
		{
			glEnableVertexAttribArray( VertexAttrib_TexCoords );
		}
		else
			glDisableVertexAttribArray( VertexAttrib_TexCoords );
		//s_bVertexAttribTexCoords = bTexCoords;
	}

	bool bTexCoords1 = flags & VertexAttrib_TexCoords1;
	//if ( bTexCoords1 != s_bVertexAttribTexCoords1 )
	{
		if ( bTexCoords1 )
		{
			glEnableVertexAttribArray( VertexAttrib_TexCoords1 );
		}
		else
			glDisableVertexAttribArray( VertexAttrib_TexCoords1 );
		//s_bVertexAttribTexCoords1 = bTexCoords1;
	}
	return;
}

void RasterGL::beginBlendMode(enBlendMode blendmode)
{
	//glBlendEquation( GL_FUNC_ADD );
	//glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//return;
	//blendmode = enBlendMode_invert;
	mBlendMode = blendmode;
    glEnable(GL_BLEND);
	
	switch( blendmode )
	{
	//case enBlendMode_add:			// ok
	//	glBlendEquation( GL_FUNC_ADD );
	//	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//	break;
	//case enBlendMode_normal:		// ok
	//	glBlendEquation( GL_FUNC_ADD );
	//	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//	break;
	//case enBlendMode_darken:  // ok
	//	glEnable( GL_COLOR_LOGIC_OP );
	//	glLogicOp( GL_AND_REVERSE );
	//	glBlendFunc( GL_ONE, GL_ONE );
	//	glBlendEquation( GL_MIN );
	//	break;
	//case enBlendMode_difference: // darken
	//	glEnable( GL_COLOR_LOGIC_OP );
	//	glLogicOp( GL_AND_REVERSE );
	//	glBlendFunc( GL_ONE, GL_ONE );
	//	glBlendEquation( GL_MIN );
	//	break;
	case enBlendMode_overlay: // ok
		//glBlendEquation( GL_FUNC_SUBTRACT );
		//glBlendFunc( GL_SRC_COLOR, GL_DST_COLOR );
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc( GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR );
		break;
	case enBlendMode_invert:
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc( GL_ZERO, GL_ONE_MINUS_DST_COLOR );
		break;
	//case enBlendMode_lighten: // ok
	//	glBlendEquation( GL_MAX );
	//	glBlendFunc( GL_ONE, GL_ONE );
	//	break;
	//case enBlendMode_erase:  // ok
	//	glEnable( GL_COLOR_LOGIC_OP );
	//	glLogicOp( GL_CLEAR );
	//	break;
	default:
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
		break;
	}
	//glEnable( GL_ALPHA_TEST );
	//glAlphaFunc( GL_GREATER, 0 );
}

void RasterGL::endBlendMode()
{
	/*switch( mBlendMode )
	{
	case enBlendMode_erase:
		break;
	}*/
	//glDisable( GL_COLOR_LOGIC_OP );
	glBlendFunc( GL_ONE, GL_ZERO );
	//glDisable( GL_ALPHA_TEST );
}

void RasterGL::setViewPort( int x, int y, int width, int height )
{
	//glViewport( 0, 0, m_WinSize.cx, m_WinSize.cy );
	//glViewport( 0, 0, 200, 2000 );
	//glViewport( 0, 100, m_WinSize.cx, m_WinSize.cy - 100 );
	//float max=width>height?width:height;
	glViewport( x, y, width, height );
	mpMatData->width = width;
	mpMatData->heigh = height;
}

void RasterGL::SetWinSize( int width, int height )
{
	m_WinSize.cx = width;
	m_WinSize.cy = height;
	this->mbFresh = true;
	//if(width>height)
	//setOrthoProjection(width*20,height*20);
	if(this->mbInit)
	{
		float wi=width-m_startSize.cx;
		float hi=height-m_startSize.cy;
		//int v=0;
		if(hi<0)
		{
			hi=hi*mOrthoMat->mat[5]*20;
			mOrthoMat->mat[13]=m_fOrY0-hi;
		}
		else
			mOrthoMat->mat[13]=m_fOrY0;
		if(wi<0)
		{
			wi=wi*mOrthoMat->mat[0]*20;
			mOrthoMat->mat[12]=m_fOrX0+hi;
		}
		else
			mOrthoMat->mat[12]=m_fOrX0;
			//m_fOrY0=0;
	}
	//this->setViewPort(0,0,width,height);
	
}

void RasterGL::DrawRLines(GLTranglesOrLines*pFL,float*matrix,float* pTrans)
{
	//return;
	if(!pFL->lines.nBufID||
		!pFL->lines.pLineStyle) return;
	BwLineStyle*pStyle = pFL->lines.pLineStyle;
	if(pStyle->color)
		SetupColor(pStyle->color,pTrans,NULL,true);
	else
	{
		GLfloat color[4] = { (float)pStyle->mColor.red / 255, (float)pStyle->mColor.green / 255, 
							 (float)pStyle->mColor.blue / 255, (float)pStyle->mColor.alpha / 255 };
		GLProgram::selectShader(Program_VerColor,0, 1, 0, 0,false);
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType],enShaderType_VerColor);
		TransColor(color,pTrans);
		//glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, color );
	}
	beginBlendMode(mBlendMode);
	GLProgram::SetObjMatrix(matrix);
//	glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],1);
	
	glBindBuffer(GL_ARRAY_BUFFER,pFL->lines.nBufID);

	//enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
	glEnableVertexAttribArray( VertexAttrib_Position );
	glVertexAttribPointer(VertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

	glEnableVertexAttribArray( VertexAttrib_TexCoords );
	glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
    
    int icou = (pFL->nCount - 1) * 6 * 3;
	
	glDrawArrays( GL_TRIANGLES, 0, icou);

	glDisableVertexAttribArray( VertexAttrib_TexCoords );
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 3 );

	glBindBuffer(GL_ARRAY_BUFFER,0);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	return;
}

void RasterGL::DrawLines(GLTranglesOrLines*pFL,float*pMatrix,float* pTrans)//GLdouble*pData,int nCount,BwLineStyle*pLineStyle)
{
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerLine); 

	//GLfloat color[4] = { 1, 0, 0, 1 };
	BwLineStyle* pStyle = pFL->lines.pLineStyle;
	if(pStyle->color)
		SetupColor(pStyle->color,pTrans,NULL);
	else
	{
		GLfloat color[4] = { (float)pStyle->mColor.red / 255, (float)pStyle->mColor.green / 255, 
							 (float)pStyle->mColor.blue / 255, (float)pStyle->mColor.alpha / 255 };
		GLProgram::selectShader(Program_VerColor,0, 0, 0, 0,false);
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType],enShaderType_VerColor);
		//gProgram[Program_VerColor].use();
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
		TransColor(color,pTrans);
		//glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, color );
	}
	beginBlendMode(mBlendMode);
	GLProgram::SetObjMatrix(pMatrix);
	//glEnable(GL_LINE_SMOOTH);

	//MATRIX mat;
	//XSWFPlayer::m_pInstance->display.GetCameraMatrix(mat);
	float lWidth=1;
	if(::MatrixIsIdentity(&m_matrix))
	{
		lWidth = pStyle->mWidth/20.0;
	}
	else
	{
		//::MatrixTransformThickness(&m_matrix,pStyle->mWidth);
		//SPOINT pt;
		//pt.x = pt.y = pStyle->mWidth;
		//MatrixDeltaTransformPoint(&m_matrix, &pt, &pt);
		/*SPOINT result;
		result.x = FixedMul(m->a, p->x);
		if ( m->c != 0 ) 
			result.x += FixedMul(m->c, p->y);
		result.y = FixedMul(m->d, p->y);
		if ( m->b != 0 ) 
			result.y += FixedMul(m->b, p->x);
		*dst = result;*/
		float x = (float)pStyle->mWidth*m_matrix.a/fixed_1;
		if(m_matrix.c)
			x+= (float)pStyle->mWidth*m_matrix.c/fixed_1;
		float y = (float)pStyle->mWidth*m_matrix.d/fixed_1;
		if(m_matrix.b)
			y+= (float)pStyle->mWidth*m_matrix.b/fixed_1;
		//float sq2=sqrtf(2);
		lWidth=sqrtf(x*x+y*y)*SQRT12F;
		if(lWidth<1) lWidth=1;
		//glUniform1f( (GLint)mCurProgram->m_uUniforms[kCCUniformLineWidth],thick);
		//SCOORD thick = FixedMul(fixedSqrt2/2, PointLength(&pt));
		//if ( t > 0 ) thick = Max(1, thick);
		//MATRIX mat;
		//::MatrixConcat(&m_matrix,&m_
	}

	glLineWidth( lWidth );
	glBindBuffer(GL_ARRAY_BUFFER,pFL->lines.nBufID);
	//enableVertexAttribs( VertexAttrib_Position );
	glEnableVertexAttribArray( VertexAttrib_Position);
	glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,0, 0 );
    int icou = pFL->nCount;
	glDrawArrays( GL_LINE_STRIP, 0, pFL->nCount );
	glBindBuffer(GL_ARRAY_BUFFER,0);
	//glDrawArrays( GL_POINTS, 0, nCount );
	
	return;
}

void RasterGL::SetupColor(GLColor* pColor,float*pTrans,XFilter*pFilter,bool isLine)
{
	//if(isLine)
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],isLine?1:0);
	/*if(isMask)
	{
		float color[]={1,1,1,1};
		glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor);//enShaderType_VerTexture );
		
		glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, color);//pColor->color );
		return;
	}*/
	if(!pColor) return;
	switch(pColor->colorType)
	{
	case glColorSolid:
		GLProgram::selectShader(Program_VerColor,0,isLine,false,false,isMask);
		 //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor);//enShaderType_VerTexture );
		 /*if(pTrans==NULL)
			 glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, pColor->color );
		 else*/
		 {
			 TransColor(pColor->color,pTrans);
		 }
		 break;
	case glFrameBuffer:
		 glEnable(GL_DITHER);
		 GLProgram::selectShader(Program_VerTexMat,pColor->frame.repeat,isLine,pTrans!=NULL,GLProgram::selectfilterid(pFilter,Program_VerTexMat,mBlendMode),isMask);
		 //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformSpreadMode],4);
		 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		 //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexMat);
		 glUniformMatrix4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, pColor->frame.invMat );
		 glActiveTexture( GL_TEXTURE0 );
		 glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],pColor->frame.frame->bPreAlpha?0:1);
		 glBindTexture( GL_TEXTURE_2D, TEXTUREID(pColor->frame.frame->mTexture));//->id );
		 USETEXTUREID(pColor->frame.frame->mTexture)
		 if(pTrans)
			 glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,pTrans);
		 //if(pFilter)
		 SetupFilter(pFilter,pColor->frame.frame->mWidth,pColor->frame.frame->mHeight,mBlendMode);
		 break;
 	case glColorDib:
		 glEnable(GL_DITHER);
		 GLProgram::selectShader(Program_VerTexMat,pColor->dib.repeat,isLine,pTrans!=NULL,GLProgram::selectfilterid(pFilter,Program_VerTexMat,mBlendMode),isMask);
		 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		 //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformSpreadMode],4);
		 //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexMat);
		 glUniformMatrix4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, pColor->dib.invMat );
		 glActiveTexture( GL_TEXTURE0 );
		 glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],isMask?1:0);
		 glBindTexture( GL_TEXTURE_2D, TEXTUREID(pColor->dib.bitmapID));//->id );
		 USETEXTUREID(pColor->dib.bitmapID)
		 if(pTrans)
			 glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,pTrans);
		 //if(pFilter)
		 SetupFilter(pFilter,pColor->dib.w,pColor->dib.h,mBlendMode);
		 break;
	case glColorBitmap:
		 //if(!pColor->bm.bitmap->bitmapID)
		//	 int v=0;
		 glEnable(GL_DITHER);
		 GLProgram::selectShader(Program_VerTexMat,pColor->bm.repeat,isLine,pTrans!=NULL,GLProgram::selectfilterid(pFilter,Program_VerTexMat,mBlendMode),isMask);
		 //if(pColor->bm.bitsStyle&1)
		 {
			 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		 }
		// glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexMat);
		 glUniformMatrix4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, pColor->bm.invMat );
		 glActiveTexture( GL_TEXTURE0 );
		 glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],pColor->bm.bitmap->preAlpha?0:1);
		 glBindTexture( GL_TEXTURE_2D, TEXTUREID(pColor->bm.bitmap->bitmapID));//->id );
		 USETEXTUREID(pColor->bm.bitmap->bitmapID)
		 if(pTrans)
			 glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,pTrans);
		// glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
		 //if(pFilter)
		 SetupFilter(pFilter,pColor->bm.bitmap->width,pColor->bm.bitmap->height,mBlendMode);
		 break;
	case glColorGradient:
		 //glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformSpreadMode],pColor->grad.ramp.spreadMode);
		 /*switch(pColor->grad.ramp.spreadMode)
		 {
		 
		 case _glRepeatEdge:
			  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			  break;
		 case _glMirrorEdge:
			  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
			  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
			  break;
		 default:
		 case _glClampEdge:
			  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
			  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
			  break;
		 }*/
		 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
         if(pColor->grad.bitmapID->w == 640 && pColor->grad.bitmapID->h == 960)
             int a = 0;
		 if(pColor->grad.gradStyle == fillLinearGradient)
		 {
			 GLProgram::selectShader(Program_VerTexMat,pColor->grad.ramp.spreadMode==_glClampEdge?0:1,
				 isLine,pTrans!=NULL,false,false);
			//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexMat);
			 glUniformMatrix4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, pColor->grad.invMat );
			glActiveTexture( GL_TEXTURE0 );
			glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],1);
			glBindTexture( GL_TEXTURE_2D, TEXTUREID(pColor->grad.bitmapID));//->id );
			USETEXTUREID(pColor->grad.bitmapID)
		 }
		 else
		 {
			 GLProgram::selectShader(Program_RadialGradient,pColor->grad.ramp.spreadMode==_glClampEdge?0:1,
				 isLine,pTrans!=NULL,false,false);
			//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType],enShaderType_RadialGradient );
			 glUniformMatrix4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, pColor->grad.invMat );
			glActiveTexture( GL_TEXTURE0 );
			glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],1);
			glBindTexture( GL_TEXTURE_2D, TEXTUREID(pColor->grad.bitmapID));//->id );
			USETEXTUREID(pColor->grad.bitmapID)
		 }
		 if(pTrans)
			 glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,pTrans);
		 break;
	}
}

void RasterGL::TransColor(GLfloat*color,float*pTrans)
{
    if (!GLProgram::mCurProgram) {
        return;
    }
	if(!pTrans)
	{
		float cls[]={color[0]*color[3],color[1]*color[3],color[2]*color[3],color[3]};
		glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, cls );
	}
	else if(pTrans[2]!=0)
	{
		GLfloat cls[]={pTrans[12]*pTrans[15],pTrans[13]*pTrans[15],
					   pTrans[14]*pTrans[15],pTrans[15]};	
		cls[0]=cls[0]*cls[3];
		cls[1]=cls[1]*cls[3];
		cls[2]=cls[2]*cls[3];
		glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, cls );
	}
	else if(pTrans[0]!=0)
	{
		GLfloat cls[]={color[0]*pTrans[4]+pTrans[8],
						 color[1]*pTrans[5]+pTrans[9],
						 color[2]*pTrans[6]+pTrans[10],
						 color[3]*pTrans[7]+pTrans[11]};
		cls[0]=cls[0]*cls[3];
		cls[1]=cls[1]*cls[3];
		cls[2]=cls[2]*cls[3];
		if(cls[3]>1) cls[3]=1;
		/*cls[0] = cls[0]*cls[3];
		cls[1] = cls[1]*cls[3];
		cls[2] = cls[2]*cls[3];*/
		glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, cls);
	}
	else 
	{
		float cls[]={color[0]*color[3],color[1]*color[3],color[2]*color[3],color[3]};
		glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1,cls );
	}
}

void RasterGL::DrawSimpleRect(GLTranglesOrLines*pFL,float*matrix,float*pTrans,bool bFrame)
{
	GLSimpleRect*pRect = pFL->rect;
	if(!pRect) 
		return;
	GLProgram::selectShader(Program_VerColor,0,false,false,false,false);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor);//enShaderType_VerTexture );
	//gProgram[Program_VerColor].use();
	/*if(!pTrans)
		glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, pRect->color );
	else*/
	{
		TransColor(pRect->color,pTrans);
	}
	beginBlendMode(mBlendMode);
	GLProgram::SetObjMatrix(matrix);
	if(bFrame)
	{
		GLfloat verts[12] = 
		{
			pRect->pos[0],pRect->pos[1],0.0f,
			pRect->pos[0],pRect->pos[3],0.0f,
			pRect->pos[2],pRect->pos[3],0.0f,
			pRect->pos[2],pRect->pos[1],0.0f,
		};
		//enableVertexAttribs( VertexAttrib_Position );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );

		glDrawArrays( GL_LINE_LOOP, 0, 4 );
	}
	else
	{
		GLfloat verts[12] = 
		{
			pRect->pos[0],pRect->pos[1],0.0f,
			pRect->pos[0],pRect->pos[3],0.0f,
			pRect->pos[2],pRect->pos[1],0.0f,
			pRect->pos[2],pRect->pos[3],0.0f,
			
		};
		//enableVertexAttribs( VertexAttrib_Position );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );


		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	}
	//mCurProgram->use();
}



void RasterGL::DrawRectangle(GLTranglesOrLines*pFL,float*matrix,float*pTrans,XFilter*pFilter)
{
	GLRectangle* pRect = pFL->rectangle;
	if(!pRect||!pRect->pFillStyle) return;
	SetupColor(pRect->pFillStyle,pTrans,pFilter);
	beginBlendMode(mBlendMode);
	GLProgram::SetObjMatrix(matrix);
	GLfloat verts[12] = 
	{
		pRect->pos[0],pRect->pos[1],0.0f,
		pRect->pos[2],pRect->pos[1],0.0f,
		pRect->pos[0],pRect->pos[3],0.0f,
		pRect->pos[2],pRect->pos[3],0.0f,
	};
	
	//enableVertexAttribs( VertexAttrib_Position );
	glEnableVertexAttribArray( VertexAttrib_Position);
	glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );

	//glDrawArrays( GL_LINE_STRIP, 0, 4 );
	//glDrawArrays( GL_POINTS, 0, 4 );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	/*glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor );
	GLfloat color[4] = { 1, 0, 0, 1 };
	glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, color );
	glUniform1f( (GLint)mCurProgram->m_uUniforms[kCCUniformPointSize], 20.0f);

	GLfloat verts[4][3] = 
	{
		{50.0f,  50.0f, 0.0f },
		{150.0f, 50.0f, 0.0f },
		{50.0f,  150.0f,0.0f },
		{150.0f, 150.0f,0.0f },
	};

	glEnableVertexAttribArray( kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );

	//glDrawArrays( GL_LINE_STRIP, 0, 4 );
	//glDrawArrays( GL_POINTS, 0, 4 );
	glDrawArrays( GL_TRIANGLES, 0, 3 );*/
}

/*void RasterGL::DrawFont16(GLTranglesOrLines*pFL)
{
	glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_DotText);

	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexMat);
	glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, _dot16Matrix );
	glActiveTexture( GL_TEXTURE0 );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
	glBindTexture( GL_TEXTURE_2D, pFL->fontID);

	glEnableVertexAttribArray( kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, _dot16Rect );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

}
void RasterGL::DrawFont12(GLTranglesOrLines*pFL)
{

	glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_DotText);

	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexMat);
	glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, _dot12Matrix );
	glActiveTexture( GL_TEXTURE0 );
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
	glBindTexture( GL_TEXTURE_2D, pFL->fontID);

	glEnableVertexAttribArray( kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, _dot12Rect );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}*/

void RasterGL::DrawTrangles(GLTranglesOrLines*pFL,float*matrix,float*pTrans,XFilter*pFilter)
{
	if(!pFL->trangles.nBufID || !pFL->trangles.pFillStyle )
        return;
	SetupColor(pFL->trangles.pFillStyle,pTrans,pFilter);
	beginBlendMode(mBlendMode);
	GLProgram::SetObjMatrix(matrix);
	glBindBuffer(GL_ARRAY_BUFFER,pFL->trangles.nBufID);

	//enableVertexAttribs( VertexAttrib_Position );
	glEnableVertexAttribArray( VertexAttrib_Position );
	glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

    int icou = pFL->nCount;
	glDrawArrays( GL_TRIANGLES, 0, pFL->nCount );
	glBindBuffer(GL_ARRAY_BUFFER,0);
	return;
}

void RasterGL::DrawRectangle(float x,float y,float w,float h,float* color,float*matrix)
{
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	GLProgram::selectShader(Program_VerColor,0,false,false,false,false);
	GLProgram::SetObjMatrix(matrix);
	beginBlendMode(mBlendMode);
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
	
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor);//enShaderType_VerTexture );
	//glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, color);//pColor->color );
	TransColor(color,NULL);
	
	GLfloat verts[12] = 
	{
		x,y,0.0f,
		x+w,y,0.0f,
		x,y+h,0.0f,
		x+w,y+h,0.0f,
	};
	
	//enableVertexAttribs( VertexAttrib_Position );
	glEnableVertexAttribArray( VertexAttrib_Position);
	glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

void RasterGL::DrawRectangle(SRECT&rect,MATRIX&mat,float* color)
{
	GLProgram::selectShader(Program_VerColor,0,false,false,false,false);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
	if(!::MatrixIsIdentity(&mat))
	{
		float fmat[16];
		this->SetGLMatrix(mat,fmat);
		GLProgram::SetObjMatrix(fmat);
		//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, fmat);
	}
	else
		GLProgram::SetObjMatrix(NULL);
		//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor);//enShaderType_VerTexture );
	//glUniform4fv( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformColor], 1, color);//pColor->color );
	TransColor(color,NULL);
	beginBlendMode(mBlendMode);
	
	GLfloat verts[12] = 
	{
		rect.xmin,rect.ymin,0.0f,
		rect.xmax,rect.ymin,0.0f,
		rect.xmin,rect.ymax,0.0f,
		rect.xmax,rect.ymax,0.0f,
	};
	
	//enableVertexAttribs( VertexAttrib_Position );
	glEnableVertexAttribArray( VertexAttrib_Position);
	glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

void RasterGL::DrawShapeList(GLShapeListData*pList,MATRIX&mat,XFilter*pFilter,bool mask)
{
	_ANALYENTER( "RasterGL::DrawShapeList" );
	if(pFilter!=NULL)
	{
		if(pFilter->m_nFilterID==XFilterDropShadow||pFilter->m_nFilterID==XFilterGlow||
			pFilter->m_nFilterID==XFilterBevel||pFilter->m_nFilterID==XFilterGradientBevel||
			pFilter->m_nFilterID==XFilterGradientGlow)
		{
			if(pFilter->filter_konckout)
				return;

		}
	}
	//m_matrix=mat;
	isMask = mask;
	::MatrixConcat(&mat,&m_camera,&m_matrix);
	m_matrix.tx=m_matrix.ty=0;
	//float*matrix = pList->matrix;
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformClipDraw], isMask?1:0);
	while(pList)
	{
		//if(pList->blendMode)
		//beginBlendMode((enBlendMode)pList->blendMode);
		mBlendMode = (enBlendMode)pList->blendMode;
		/*if(pList->matrix)
		{
			glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, pList->matrix);
		}
		else
			glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
		if(pList->colorTransform)
		{
			glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE, pList->colorTransform);
		}
		else
			glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);*/
		DrawShape(pList->pData,pList->colorTransform,pList->matrix,pFilter);
		//if(pList->blendMode)
		//	beginBlendMode(enBlendMode_add);
		pList = pList->pNext;
	}
}


/*#define TOPIXEL(VV) VV=(VV+10)/20
//	{if(VV>0) VV=(VV+10)/20;\
//	 else	  VV=(VV-10)/20;}
#if (__CORE_VERSION__>=0x02070200)
inline _GRIDINFO* SetGGRect(_XSObject*obj,_GRIDINFO*info,MATRIX*mat,bool&bSet)
#else
inline _GRIDINFO* SetGGRect(_XSObject*obj,_GRIDINFO*info,MATRIX*mat)
#endif
{
	//if(obj->character&&obj->character->tag==32)
	//	int v=0;
	//return NULL;
#if (__CORE_VERSION__>=0x02070200)
	bSet=false;
#endif
	if(!obj->gridRect) return NULL;
	int nOne=(fixed_1<<obj->display->antialias_b)/20;
	if(mat->c!=0||mat->b!=0) return NULL;
#if (__CORE_VERSION__>=0x02074000)
	nOne=nOne>>2;
	if(mat->a<=nOne&&mat->a>=-nOne&&mat->d<=nOne&&mat->d>=-nOne) 
	{
		bSet=true;
		return NULL;
	}
#elif (__CORE_VERSION__>=0x02070200)
	if(mat->a<=nOne&&mat->a>=-nOne&&mat->d<=nOne&&mat->d>=-nOne) 
	{
		bSet=true;
		return NULL;
	}
#else
	if(mat->a<=nOne&&mat->a>=-nOne&&mat->d<=nOne&&mat->d>=-nOne) return NULL;
#endif
	SRECT*grid=obj->gridRect;
	if(grid->xmin>grid->xmax||
       grid->ymin>grid->ymax) return NULL;
	//info->bNewGridBy=false;
	//info->bNewDev=XTRUE;
	DisplayList*display=obj->display;
	if(!RectIsEmpty(&obj->devBounds))
	{
		SRECT*rr=&info->devRect;
		*rr=obj->devBounds;
		//info->devRect=rr;
		ToPixel(&info->devRect,display->antialias_b);
		//info->devRect=&obj->devBounds;
	}
	else if(!obj->bottomChild) return NULL;
	else
	{
		_XSObject*child=obj->bottomChild;
		if(child->above==NULL&&!RectIsEmpty(&child->devBounds))
		{
			SRECT*rr=&info->devRect;//display->CreateRect();
			*rr=child->devBounds;
			//info->devRect=rr;
			ToPixel(&info->devRect,display->antialias_b);
		}
		else
		{			
			SRECT*rr=&info->devRect;//display->CreateRect();
			//GetBoundingBox(obj,mat,rr);
			obj->GetDevBound(rr);
			//info->bNewDev=XTRUE;
			//info->devRect=rr;
			ToPixel(&info->devRect,display->antialias_b);
		}
	}
	//info->gridBy=obj->gridRect;
	
	
	
	{
		//SRECT rb;
		::GetBoundingRect(obj,&info->orgRect);//rb);
		TOPIXEL(info->orgRect.xmin);
		TOPIXEL(info->orgRect.xmax);
		TOPIXEL(info->orgRect.ymin);
		TOPIXEL(info->orgRect.ymax);
		//rb.xmin=(rb.xmin+20
		//info->orgWidth=rb.xmax-rb.xmin;
		//info->orgHeight=rb.ymax-rb.ymin;
		//int cx=info->orgWidth/2;
		//int cy=info->orgHeight/2;
		SRECT*rr=&info->gridBy;//display->CreateRect();
		//info->orgX=(rb.xmax+rb.xmin)/2;
		//info->orgY=(rb.ymax+rb.ymin)/2;

		{
			//rr->xmin=grid->xmin+cx;
			//rr->ymin=grid->ymin+cy;
			
			//rr->xmax=grid->xmax+cx;			
			//rr->ymax=grid->ymax+cy;		
			rr->xmin=grid->xmin-info->orgRect.xmin;
			rr->ymin=grid->ymin-info->orgRect.ymin;
			rr->xmax=info->orgRect.xmax-grid->xmax;
			rr->ymax=info->orgRect.ymax-grid->ymax;
		}

		int dl=RectWidth(&info->devRect);
		if(rr->xmin+rr->xmax>dl)
		{
			//int dd=rr->xmin+rr->xmax;
			rr->xmin=0;//rr->xmin*dl/dd;
			rr->xmax=0;//rr->xmax*dl/dd;
		}
		dl=RectHeight(&info->devRect);
		if(rr->ymin+rr->ymax>dl)
		{
			//int dd=rr->ymin+rr->ymax;
			rr->ymin=0;//rr->ymin*dl/dd;
			rr->ymax=0;//rr->ymax*dl/dd;
		}
		//int nOne=(fixed_1<<obj->display->antialias_b)/20;


		//info->gridBy=rr;
#if (__CORE_VERSION__>=0x02076000)
		//if(rr->xmin<0) rr->xmin=0;
		//if(rr->xmax<0) rr->xmax=0;
		//if(rr->ymin<0) rr->ymin=0;
		//if(rr->ymax<0) rr->ymax=0;
		if(rr->xmin<0||rr->ymin<0||
		   rr->xmax<0||rr->ymax<0)
		{
			bSet=true;
			return NULL;
		}
#else
		if(rr->xmin<0||rr->ymin<0||
		   rr->xmax<0||rr->ymax<0) return NULL;
#endif
		//info->bNewGridBy=true;
	}
	
	return info;
	//return grid;
}*/

void RasterGL::DrawBitmap(SBitmapCore*bits)
{
	if(!bits->bitmapID&&!bits->HasBits()) return;
	if(!bits->bitmapID)
		bits->CreateTexture();
	GLProgram::selectShader(Program_VerTexCoords,0,false,false,false,isMask);
	GLProgram::SetObjMatrix(NULL);
	beginBlendMode(enBlendMode_add);
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
	int w = bits->width;
	int h = bits->height;
	w = w*20;
	h = h*20;
	{
		//glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		glActiveTexture( GL_TEXTURE0 );
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],bits->preAlpha?1:0);
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(bits->bitmapID));//->id );
		USETEXTUREID(bits->bitmapID)
		GLfloat verts[] = 
		{
			0,0,0.0f,0,0,
			w,0,0.0f,1,0,
			0,h,0.0f,0,1,
			w,h,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), &verts[0] );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), &verts[3] );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );
	}	
}

void RasterGL::DrawButton(float* pMat,XDib*pDraw,XPOINT&pt,XRECT&rect,float alpha,XDib*pLight)
{
	glEnable(GL_DITHER);
	GLProgram::selectShader(Program_VerTexCoords,0,false,alpha<1,false,false);
	GLProgram::SetObjMatrix(pMat);
	beginBlendMode(enBlendMode_add);
	//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, pMat);
	if(alpha<1)
	{
		float trans[16];
		memset(trans,0,sizeof(trans));
		memset(trans,0,16*sizeof(float));
		trans[0]=1;
		trans[4]=1;
		trans[5]=1;
		trans[6]=1;
		trans[7]=alpha;
		glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,trans);
	}
	//else
	//	glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],1);
	
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],1);
	
	int w = (rect.right-rect.left);
	int h = (rect.bottom-rect.top);
	{
		//glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		glActiveTexture( GL_TEXTURE0 );
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
		_TEXTUREID* tID=this->texInitWithData(pDraw->GetBuffer(),Texture2DPixelFormat_RGBA8888,pDraw->Width(),pDraw->Height());
		glBindTexture( GL_TEXTURE_2D,tID->id );
		
		GLfloat verts[] = 
		{
			pt.x,pt.y,0.0f,0,0,
			pt.x+w,pt.y,0.0f,1,0,
			pt.x,pt.y+h,0.0f,0,1,
			pt.x+w,pt.y+h,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), &verts[0] );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), &verts[3] );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		this->DeleteTexture(tID);
		glDisableVertexAttribArray( VertexAttrib_TexCoords );
	}
	if(pLight)
	{

		glActiveTexture( GL_TEXTURE0 );
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
		_TEXTUREID* tID=this->texInitWithData(pLight->GetBuffer(),Texture2DPixelFormat_RGBA8888,pLight->Width(),pLight->Height());
		glBindTexture( GL_TEXTURE_2D,tID->id );
		w=pLight->Width();
		h=pLight->Height();
		GLfloat verts[] = 
		{
			pt.x,pt.y,0.0f,0,0,
			pt.x+w,pt.y,0.0f,1,0,
			pt.x,pt.y+h,0.0f,0,1,
			pt.x+w,pt.y+h,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), &verts[0] );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), &verts[3] );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
		this->DeleteTexture(tID);
		glDisableVertexAttribArray( VertexAttrib_TexCoords );
	}
}

static float _blendDataScreen[20]=
	{1,		0,		0,		0,		0,
	 0,		1,		0,		0,		0,
	 0,		0,		1,		0,		0,
	 1.0f/3,1.0f/3, 1.0f/3, 0,		0
	};
static float _blendDataLighten[20]=
	{1,		0,		0,		0,		0,
	 0,		1,		0,		0,		0,
	 0,		0,		1,		0,		0,
	 1.0f/3,1.0f/3, 1.0f/3, 0,		0
	};
static float _blendDataAdd[20]=
	{2.5f,	0,		0,		0,		0,
	 0,		2.5f,	0,		0,		0,
	 0,		0,		2.5f,	0,		0,
	 1.0f/3,1.0f/3, 1.0f/3, 0,		0
	};
static float _blendDataOverlay[20]=
	{1,		0,		0,		0,		0,
	 0,		1,		0,		0,		0,
	 0,		0,		1,		0,		0,
	 1.0f/3,1.0f/3, 1.0f/3, 0,	0
	};
static float _blendDataSubtrace[20]=
	{-1+1.0f/3,		+1.0f/3,		+1.0f/3,		0,		0,
	 +1.0f/3,		-1+1.0f/3,		0+1.0f/3,		0,		0,
	 +1.0f/3,		+1.0f/3,		-1+1.0f/3,		0,		0,
	 1.0f/3,1.0f/3, 1.0f/3, 0,		0
	};
void RasterGL::SetupFilter(XFilter*pFilter,int w,int h,enBlendMode mode)
{
/*if(pFilter!=XNULL)
	{
		glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_Filter);

		float offset_x=0.0;
		float offset_y=0.0;

		int pFilterid=pFilter->m_nFilterID;

		switch(pFilter->m_nFilterID)
		{
			case XFilterBlur:
				 offset_x=pFilter->pBlur.blurX/2;
				 offset_y=pFilter->pBlur.blurY/2;
				 break;
			case XFilterGlow:
				 offset_x=pFilter->pGlow.blurX/2;
				 offset_y=pFilter->pGlow.blurY/2;
				 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, pFilter->pGlow.color );
				 glUniform1f( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterstrength], (pFilter->pGlow.strength)/4.0>1.0?(pFilter->pGlow.strength)/4.0:1.0 );
				 break;
			case XFilterDropShadow:
				 if(pFilter->pDropShadow.distance!=0.0)
				 {
					 offset_x=(pFilter->pDropShadow.distance);
					 offset_y=(pFilter->pDropShadow.distance);
					 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, pFilter->pDropShadow.color );
				 }
				 else
				 {
					 pFilterid=2;
					 offset_x=(pFilter->pDropShadow.blurX)/2;
					 offset_y=(pFilter->pDropShadow.blurY)/2;
					 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, pFilter->pDropShadow.color );
					 glUniform1f( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterstrength], (pFilter->pDropShadow.strength)/4.0>1.0?(pFilter->pDropShadow.strength)/4.0:1.0 );
				 }
				 break;
			case XFilterBevel:
				 offset_x=(pFilter->pBevel.blurX/2+pFilter->pBevel.distance);
				 offset_y=(pFilter->pBevel.blurY/2+pFilter->pBevel.distance);
				 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, pFilter->pBevel.hiColor );
				 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterBcolors], 1, pFilter->pBevel.shadowColor );
				 break;
			case XFilterGradientGlow:
				 offset_x=(pFilter->pGradientGlow.blurX/2);
				 offset_y=(pFilter->pGradientGlow.blurY/2);
				 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, pFilter->pGradientGlow.color );
				 //glUniform1f( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterstrength], pFilter->pGradientGlow.strength );
				 break;
			case XFilterGradientBevel:
				 offset_x=(pFilter->pGradientBevel.blurX+pFilter->pGradientBevel.distance);
				 offset_y=(pFilter->pGradientBevel.blurY+pFilter->pGradientBevel.distance);
				 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformColor], 1, pFilter->pGradientBevel.hiColor );
				 glUniform4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterBcolors], 1, pFilter->pGradientBevel.shadowColor );
				 break;
			case XFilterConvolution:
				 glUniform1fv( (GLint)mCurProgram->m_uUniforms[kCCUniformFiltercon], 9, pFilter->pConvolution.fData );
				 break;
			case XFilterColorMatrix:
				 glUniform1fv( (GLint)mCurProgram->m_uUniforms[kCCUniformFiltercmatrix], 20, pFilter->pColorMatrix.fData );
				 break;
			default:break;
		}

		glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformFiltertype], pFilterid);

		glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterkonckout], pFilter->filter_konckout );

		GLfloat Filteroffset[2] = {offset_x>5.0?5.0:offset_x, offset_y>5.0?5.0:offset_y };
		glUniform2fv( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterOffset], 1, Filteroffset );

		GLfloat Filterbounds[2] ={(GLfloat)pFrame->mWidth,(GLfloat)pFrame->mHeight};
		glUniform2fv( (GLint)mCurProgram->m_uUniforms[kCCUniformFilterBounds], 1, Filterbounds );

		

	}*/
	switch(mode)
	{
	case enBlendMode_subtrace:
	case enBlendMode_difference:
		 glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformFiltercmatrix]), 
					   (GLsizei)20,_blendDataSubtrace);
		 break;
	case enBlendMode_screen:
		 glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformFiltercmatrix]), 
					   (GLsizei)20,_blendDataScreen);
		 return;
	case enBlendMode_lighten:
		 glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformFiltercmatrix]), 
					   (GLsizei)20,_blendDataLighten);
		 return;
	case enBlendMode_add:
		 glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformFiltercmatrix]), 
					   (GLsizei)20,_blendDataAdd);
		 return;
	/*case enBlendMode_overlay:
		 glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformFiltercmatrix]), 
					   (GLsizei)20,_blendDataOverlay);
		 return;*/
	}
	if(pFilter==NULL)
		return;
	GLfloat filter_data[7];
	filter_data[0] = w/20.0;
	filter_data[1] = h/20.0;
	switch(pFilter->m_nFilterID)
	{
		case XFilterGlow:
		filter_data[2] = pFilter->pGlow.color[0];
		filter_data[3] = pFilter->pGlow.color[1];
		filter_data[4] = pFilter->pGlow.color[2];
		filter_data[5] = pFilter->pGlow.color[3];
		filter_data[6] = pFilter->pGlow.strength;
		break;
		case XFilterColorMatrix:
		glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformFiltercmatrix]), (GLsizei)20,pFilter->pColorMatrix.fData);
		break;
		case XFilterDropShadow:
		filter_data[2] = pFilter->pDropShadow.color[0];
		filter_data[3] = pFilter->pDropShadow.color[1];
		filter_data[4] = pFilter->pDropShadow.color[2];
		filter_data[5] = pFilter->pDropShadow.color[3];
		filter_data[6] = pFilter->pDropShadow.strength;
		break;
	}
	glUniform1fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformFilterData]), (GLsizei)7,filter_data);
}

void RasterGL::DrawFrame(FrameTexture*pFrame,MATRIX&mat,XCXForm&form,SRECT*grid,XFilter*pFilter,MATRIX*pCamreaMatrix,enBlendMode blendMode)
{
	//for Test
	//pFilter = NULL;
	//grid = NULL;
	glEnable(GL_DITHER);
	//this->beginBlendMode(blendMode);
	GLProgram::selectShader(Program_VerTexCoords,0,false,form.flags!=0,GLProgram::selectfilterid(pFilter,Program_VerTexCoords,blendMode),isMask);
	beginBlendMode(blendMode);
	SetupFilter(pFilter,0,0,blendMode);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformClipDraw],isMask?1:0);
	int w = pFrame->mWidth;
	int h = pFrame->mHeight;
	w = w*20;
	h = h*20;
	SRECT r={0,w,0,h};
	if(grid)
	{
		::MatrixTransformRect(&mat,&r,&r);
		int w1=r.xmax-r.xmin;
		int h1=r.ymax-r.ymin;
		if(w1<grid->xmin+w-grid->xmax||
		   h1<grid->ymin+h-grid->ymax)
		   grid = NULL;
	}

	if(grid&&pCamreaMatrix)
	{
		//MATRIX invMat;
		//XSWFPlayer::m_pInstance->display.GetCameraMatrix(cmat);
		//::MatrixConcat(&mat,pCamreaMatrix,&mat);
		//::MatrixInvert(pCamreaMatrix,&invMat);
		//GLfloat fmat[16];
		//SetGLMatrix(invMat,fmat);
		GLProgram::SetObjMatrix(NULL);//fmat);
		//glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
		//glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
	}
	else if(::MatrixIsIdentity(&mat))
	{
		GLProgram::SetObjMatrix(NULL);
		//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
	}
	else
	{
		GLfloat fmat[16];
		SetGLMatrix(mat,fmat);
		GLProgram::SetObjMatrix(fmat);
		//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, fmat);
	}
	if(form.flags)
		//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,_glEmptyTransform);
	//else
	{
		GLfloat fmat[16];
		SetGLTransform(form,fmat);
		glUniformMatrix4fv( (GLint)(GLProgram::mCurProgram->m_uUniforms[kCCUniformColorTransform]), (GLsizei)1, GL_FALSE,fmat);
	}
	//glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformLineFlag],0);

	//glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerTexCoord);
	
	glUniform1i( (GLint)GLProgram::mCurProgram->m_uUniforms[kCCUniformPreAlpha],pFrame->bPreAlpha?0:1);
	
	if(pFrame->nImgDiv)
	{
		w<<=pFrame->nImgDiv;
		h<<=pFrame->nImgDiv;
	}
	if(grid)
	{
		//glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
		Scale9Grid grid9;
		grid9.init();
		grid9.initTex(TEXTUREID(pFrame->mTexture),w,h,grid->xmin,
										   grid->ymin,
										   w-grid->xmax,
										   h-grid->ymax);
		USETEXTUREID(pFrame->mTexture)
		//SRECT r={0,w,0,h};
		//::MatrixTransformRect(&mat,&r,&r);
		grid9.drawScale9Grid(r.xmin,r.ymin,r.xmax,r.ymax);
		grid9.renderGrid();
	}
	else
	{
		//glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		glActiveTexture( GL_TEXTURE0 );
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(pFrame->mTexture));//->id );
		USETEXTUREID(pFrame->mTexture)
		GLfloat verts[] = 
		{
			0,0,0.0f,0,0,
			w,0,0.0f,1,0,
			0,h,0.0f,0,1,
			w,h,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), &verts[0] );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), &verts[3] );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );
	}
	//drawOffScreen(true);
}

void RasterGL::DrawFont(GLTranglesOrLines*pFL,float*matrix,float*color,float*pTrans,XFilter*pFilter)//DrawFont(GLTranglesOrLines*pFL,XFilter*pFilter)
{
	glEnable(GL_DITHER);
	GLProgram::selectShader(Program_Font,0,false,false,GLProgram::selectfilterid(pFilter,Program_Font,mBlendMode),isMask);
	TransColor(color,pTrans);
	beginBlendMode(mBlendMode);
	GLProgram::SetObjMatrix(matrix);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformLineFlag],0);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_Font);
	//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
	{
		//glUniformMatrix4fv( (GLint)mCurProgram->m_uUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, matrix );
		int w=pFL->pChar->w;
		int h=pFL->pChar->h;
		SetupFilter(pFilter,w,h,mBlendMode);
		glActiveTexture( GL_TEXTURE0 );
		//glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformPreAlpha],0);
		glBindTexture( GL_TEXTURE_2D,TEXTUREID(pFL->pChar->id));//->id );
		USETEXTUREID(pFL->pChar->id)
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		GLfloat verts[] = 
		{
			0,0,0.0f,0,0,
			w,0,0.0f,1,0,
			0,h,0.0f,0,1,
			w,h,0.0f,1,1,
		};
		
		//glEnableVertexAttribArray( kCCVertexAttrib_Position);
		//glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, 0, verts );
		//enableVertexAttribs( VertexAttrib_Position | VertexAttrib_TexCoords );
		glEnableVertexAttribArray( VertexAttrib_Position);
		glVertexAttribPointer(VertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  5 * sizeof( GLfloat), &verts[0] );
		
		glEnableVertexAttribArray( VertexAttrib_TexCoords );
		glVertexAttribPointer(VertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  5 * sizeof(GLfloat), &verts[3] );

		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

		glDisableVertexAttribArray( VertexAttrib_TexCoords );
	}
	//drawOffScreen(true);
}


void RasterGL::DrawShape(GLShapeData*pData,float*pTrans,float*matrix,XFilter*pFilter)
{
	//Render();
	//return;
	while(pData)
	{
		if(pData->matrix)
			matrix = pData->matrix;
		//{
		//	glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, pData->matrix);
		//}
		//else
		//	glUniformMatrix4fv( (GLint)(mCurProgram->m_uUniforms[kCCUniformOBJMatrix]), (GLsizei)1, GL_FALSE, _glIDMatrix);
		float* color = _whiteColorF;//{1.0f,1.0f,1.0f,1.0f};
		if(pData->color[3]!=0)
			color=pData->color;
		/*{
			glUniform1i( (GLint)mCurProgram->m_uUniforms[kCCUniformShaderType], enShaderType_VerColor);//enShaderType_VerTexture );
			if(pTrans==XNULL)
				glUniform4fv( (GLint)gProgram[Program_All].m_uUniforms[kCCUniformColor], 1, pData->color);//pColor->color );
			else
				TransColor(pData->color,pTrans);
		}*/
		GLTranglesOrLines*pFL = pData->pData;
		while(pFL)
		{
			bool bBreak = false;
			switch(pFL->type)
			{
			case GLT_LINES:
				 if(pFL->lines.pLineStyle->mWidth<=_MINLINEW)
					DrawLines(pFL,matrix,pTrans);//pFL->lines.pPoints,pFL->nCount,pFL->lines.pLineStyle);
				 else
			  		DrawRLines(pFL,matrix,pTrans);//pFL->lines.pPoints,pFL->nCount,pFL->lines.pLineStyle);
				 break;
			case GLT_TRANGLES:
				//WindowView::sharedWindowView()->MakeCurrent( true );
				 DrawTrangles(pFL,matrix,pTrans,pFilter);//->trangles,pFL->nCount,pFL->trangles.pFillStyle);
				//WindowView::sharedWindowView()->MakeCurrent( false );
				 break;
			/*case GLT_FONT16:
				 DrawFont16(pFL);
				 break;
			case GLT_FONT12:
				 DrawFont12(pFL);
				 break;*/
			case GLT_FONT:
				 DrawFont(pFL,matrix,color,pTrans,pFilter);
				 bBreak = true;
				 break;
			case GLT_RECTANGLE:
				 DrawRectangle(pFL,matrix,pTrans,pFilter);
				 bBreak = true;
				 break;
			case GLT_SIMPLERECT:
				 DrawSimpleRect(pFL,matrix,pTrans,false);
				 bBreak = true;
				 break;
			case GLT_SIMPLEFRAME:
				 DrawSimpleRect(pFL,matrix,pTrans,true);
				 bBreak = true;
				 break;
			}
			if(bBreak)
			{
				//if(!pData->matrix)
				//	int v=0;
				break;
			}
			pFL = pFL->pNext;
		}
		pData = pData->pNext;
	}
	//glEnable( GL_SCISSOR_TEST );
	//glScissor( 10, 10, 50, 50 );
	//texDraw(0);
}

void RasterGL::SetGLMatrix(MATRIX &m, float *mat)
{
	memset(mat,0,16*sizeof(float));
	mat[0] = m.a / 65536.0f;
    mat[1] = m.b / 65536.0f;
    mat[4] = m.c / 65536.0f;
    mat[5] = m.d / 65536.0f;
    mat[10] = 1;
    mat[12] = m.tx;
    mat[13] = m.ty;
    mat[15] = 1;
}

void RasterGL::SetGLTransform(XCXForm&form,float*mat)
{
	memset(mat,0,16*sizeof(float));
	mat[0] = (form.flags)?1:0;
	mat[1] = 0;
	mat[2] = form.nForeColor?1:0;
	mat[3] = form.nBlendMode;
	mat[4] = form.ra/255.0;
	mat[5] = form.ga/255.0;
	mat[6] = form.ba/255.0;
	mat[7] = form.aa/255.0;
	mat[8] = form.rb/255.0;
	mat[9] = form.gb/255.0;
	mat[10] = form.bb/255.0;
	mat[11] = form.ab/255.0;
	if(form.nForeColor)
	{
		RGB8* rgb=(RGB8*)&form.nForeColor;
		mat[12] = rgb->red/255.0;
		mat[13] = rgb->green/255.0;
		mat[14] = rgb->blue/255.0;
		mat[15] = rgb->alpha/255.0;
	}
}

//void RasterGL::ReadTexture(SBitmapCore *bits, int id)
//{
//	glBindTexture(GL_TEXTURE_2D,id);
//	glPixelStorei(GL_PACK_ALIGNMENT, 1);
//	//glReadPixels( 0, 0, bits->width, bits->height, GL_RGBA, GL_UNSIGNED_BYTE, bits->baseAddr );
//	//glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,bits->baseAddr);
//	glBindTexture(GL_TEXTURE_2D,0);
//}

#ifdef _WINEMU
void RasterGL::GetTexture(_TEXTUREID*id,XDib&dib)
{
	glBindTexture(GL_TEXTURE_2D,id->id);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	//glReadPixels( 0, 0, bits->width, bits->height, GL_RGBA, GL_UNSIGNED_BYTE, bits->baseAddr );
	dib.Create(id->w,id->h,32);
	glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,dib.GetBuffer());
	//glReadPixels( 0, 0, id->w,id->h, GL_RGBA, GL_UNSIGNED_BYTE,dib.GetBuffer() );
	glBindTexture(GL_TEXTURE_2D,0);
	int iCount=dib.Width()*dib.Height();
	RGB8*pData=(RGB8*)dib.GetBuffer();
	for(int i=0;i<iCount;i++)
	{
		int t=pData->red;
		pData->red=pData->blue;
		pData->blue=t;
		pData++;
	}
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);
}
#endif
void RasterGL::ClearCache()
{
	TextureCache::Release();	
}
