#include "StdAfxflash.h"
#include "BwFillStyle.h"
//#include "BwGLUtility.h"
#include "SBitmap.h"
#include "math.h"
#include "XDrawDib.h"
#include "Raster_gl.h"
#include "kazmath/mat4.h"
#include "kazmath/vec4.h"

struct _GLMATRIX
{
	float a,b,c,d;
	float tx,ty;
};

inline void GLSetMatrix(MATRIX&mat,_GLMATRIX&out)
{
	out.a = mat.a/65536.0f;
	out.b = mat.b/65536.0f;
	out.c = mat.c/65536.0f;
	out.d = mat.d/65536.0f;
	out.tx = mat.tx;
	out.ty = mat.ty;
}
inline void GLMatrix9To16(_GLMATRIX&mat,float* out)
{
	memset(out,0,sizeof(float)*16);
	out[0] = mat.a;
	out[1] = mat.b;
	out[4] = mat.c;
	out[5] = mat.d;
	out[10] = 1;
	out[12] = mat.tx;
	out[13] = mat.ty;
	out[15] = 1;
}

inline void GLMatrixIdentity(_GLMATRIX*mat)
{
	mat->a = mat->d = 1;
	mat->b = mat->c = 0;
	mat->tx = mat->ty = 0;
}

void GLMatrixConcat(_GLMATRIX* m1,_GLMATRIX* m2,_GLMATRIX&result)
{
	//_GLMATRIX result;
	
  // Multiply the common terms
	result.a  = m1->a*m2->a;//FixedMul(m1->a,  m2->a);
	result.d  = m1->d*m2->d;//FixedMul(m1->d,  m2->d);
	result.b  = result.c = 0;
	result.tx = m1->tx*m2->a+m2->tx;//FixedMul(m1->tx, m2->a) + m2->tx;
	result.ty = m1->ty*m2->d+m2->ty;//FixedMul(m1->ty, m2->d) + m2->ty;
	
  // Include the less common terms
	if ( m1->b != 0 || m1->c != 0 || m2->b != 0 || m2->c != 0 ) {
		result.a  += m1->b*m2->c;//FixedMul(m1->b,  m2->c);
		result.d  += m1->c*m2->b;//FixedMul(m1->c,  m2->b);
		result.b  += m1->a*m2->b+m1->b*m2->d;//FixedMul(m1->a,  m2->b) + FixedMul(m1->b,  m2->d);
		result.c  += m1->c*m2->a+m1->d*m2->c;//FixedMul(m1->c,  m2->a) + FixedMul(m1->d,  m2->c);
		result.tx += m1->ty*m2->c;//FixedMul(m1->ty, m2->c);
		result.ty += m1->tx*m2->b;//FixedMul(m1->tx, m2->b);
	}
	//*dst = result;
}

void GLMatrixInvert(_GLMATRIX* m, _GLMATRIX* dst)
{
	if ( m->b == 0 && m->c == 0 ) {
	  // Invert a simple matrix
		dst->a  = 1/m->a;//FixedDiv(fixed_1, m->a);
		dst->d  = 1/m->d;//FixedDiv(fixed_1, m->d);
		dst->b  = dst->c = 0; 
		dst->tx = -dst->a*m->tx;//-FixedMul(dst->a, m->tx);
		dst->ty = -dst->d*m->ty;//-FixedMul(dst->d, m->ty);
		
	} else {

 		double a0 = m->a, a1 = m->b, a2 = m->c, a3 = m->d, det;
 		//SPOINT pt;
 	
 		//a0 = FixedToFloat(m->a);
 		//a1 = FixedToFloat(m->b);
 		//a2 = FixedToFloat(m->c);
 		//a3 = FixedToFloat(m->d);
 	
 		det = a0*a3 - a1*a2;
 		if ( det == 0.0 ) {
 			//FLASHOUTPUT("Singular matrix in invert.");
 			GLMatrixIdentity(dst);
 			return;
 		}
 		det = 1.0 / det;
 		
 		dst->a = (float) (a3 * det);
 		dst->b = (float)-(a1 * det);
 		dst->c = (float)-(a2 * det);
 		dst->d = (float)(a0 * det);
 		//pt.x = m->tx;
 		//pt.y = m->ty;
 		//MatrixDeltaTransformPoint(dst, &pt, &pt);
 		//dst->tx = -pt.x;
 		//dst->ty = -pt.y;
		//	SPOINT result;
		dst->tx = -dst->a*m->tx;
		if(dst->c!=0)
			dst->tx -= dst->c*m->ty;
		dst->ty = -dst->d*m->ty;
		if(dst->b!=0)
			dst->ty -= dst->b*m->tx;
		//result.x = FixedMul(m->a, p->x);
		//if ( m->c != 0 ) 
		//	result.x += FixedMul(m->c, p->y);
		//result.y = FixedMul(m->d, p->y);
		//if ( m->b != 0 ) 
		//	result.y += FixedMul(m->b, p->x);
		//*dst = result;

	}
}

void GLColor::SetUp()
{
	//MATRIX divMat={fixed_1,0,0,fixed_1,0,0};
	//raster = r;
	switch(this->colorType)
	{
	case glColorSolid:
		 this->color[0] = this->rgb.red/255.0;
		 this->color[1] = this->rgb.green/255.0;
		 this->color[2] = this->rgb.blue/255.0;
		 this->color[3] = this->rgb.alpha/255.0;
		 break;
	case glColorDotFont:
		 break;
	case glFrameBuffer:
		{
		    frame.invMat = ShapeAlloc::GetInst()->AllocRamp();
			if(!frame.invMat)
			{
				colorType = glColorSolid;
				this->color[0] = this->rgb.red/255.0;
				this->color[1] = this->rgb.green/255.0;
				this->color[2] = this->rgb.blue/255.0;
				this->color[3] = this->rgb.alpha/255.0;
			}
			else
			{
				//frame.bitmap->LockBits();
				RasterGL::LockTexture(frame.frame->mTexture);
				RasterGL::LockDisplay(frame.frame->mTexture);
				_GLMATRIX mMat={frame.frame->mWidth,0,0,frame.frame->mHeight,0,0};
				//_GLMATRIX dMat={1/20.0f,0,0,1/20.0f,0,0};
				_GLMATRIX mat,out;
				GLSetMatrix(frame.savedMat,mat);
				GLMatrixConcat(&mMat,&mat,out);
				//GLMatrixConcat(&out,&dMat,mat);
				GLMatrixInvert(&out,&mat);
				GLMatrix9To16(mat,frame.invMat);
				//frame.frame->mnLastTime = XSWFPlayer::m_pInstance->m_nRunTime;
			}
		}
		break;
	case glColorDib:
		{
		    dib.invMat = ShapeAlloc::GetInst()->AllocRamp();
			if(!dib.invMat)
			{
				colorType = glColorSolid;
				this->color[0] = this->rgb.red/255.0;
				this->color[1] = this->rgb.green/255.0;
				this->color[2] = this->rgb.blue/255.0;
				this->color[3] = this->rgb.alpha/255.0;
			}
			else
			{
				//frame.bitmap->LockBits();
				_GLMATRIX mMat={dib.w,0,0,dib.h,0,0};
				//_GLMATRIX dMat={1/20.0f,0,0,1/20.0f,0,0};
				_GLMATRIX mat,out;
				GLSetMatrix(dib.savedMat,mat);
				GLMatrixConcat(&mMat,&mat,out);
				//GLMatrixConcat(&out,&dMat,mat);
				GLMatrixInvert(&out,&mat);
				GLMatrix9To16(mat,dib.invMat);
				//RasterGL::LockTexture(dib.bitmapID);
			}
		}
		break;
	case glColorBitmap:
		{
			if(!bm.bitmap->HasBits()&&!bm.bitmap->bitmapID)
			{
				colorType = glColorSolid;
				this->color[0] = this->rgb.red/255.0;
				this->color[1] = this->rgb.green/255.0;
				this->color[2] = this->rgb.blue/255.0;
				this->color[3] = this->rgb.alpha/255.0;
			}
			else
			{
				bm.invMat = ShapeAlloc::GetInst()->AllocRamp();
				if(!bm.invMat)
				{
					colorType = glColorSolid;
					this->color[0] = this->rgb.red/255.0;
					this->color[1] = this->rgb.green/255.0;
					this->color[2] = this->rgb.blue/255.0;
					this->color[3] = this->rgb.alpha/255.0;
				}
				else
				{
					bm.bitmap->LockBits();
					RasterGL::LockDisplay(bm.bitmap->bitmapID);
					_GLMATRIX mMat={bm.bitmap->width0,0,0,bm.bitmap->height0,0,0};
					//_GLMATRIX dMat={1/20.0f,0,0,1/20.0f,0,0};
					_GLMATRIX mat,out;
					/*if(bm.repeat)
					{
						_GLMATRIX sMat={10,0,0,10,-5,-5};
						GLMatrixConcat(&sMat,&mMat,mMat);
					}*/
					GLSetMatrix(bm.savedMat,mat);
					GLMatrixConcat(&mMat,&mat,out);
					//GLMatrixConcat(&out,&dMat,mat);
					GLMatrixInvert(&out,&mat);
					GLMatrix9To16(mat,bm.invMat);
					
					/*switch(bm.bitmap->bmFormat)
					{
					default:
					case bm32Bit:
						 bm.bitmapID = raster->texInitWithData(bm.bitmap->baseAddr,kCCTexture2DPixelFormat_RGBA8888,
							 bm.bitmap->width,bm.bitmap->height);
						 break;
					case bm16Bit:
						 bm.bitmapID = raster->texInitWithData(bm.bitmap->baseAddr,kCCTexture2DPixelFormat_RGB565,
							 bm.bitmap->width,bm.bitmap->height);
						 break;
					case bm8Bit:
						 bm.bitmapID = raster->texInitWithData(bm.bitmap->baseAddr,kCCTexture2DPixelFormat_A8,
							 bm.bitmap->width,bm.bitmap->height);
						 break;
					case bm2Bit:
						 bm.bitmapID = raster->texInitWithData(bm.bitmap->baseAddr,kCCTexture2DPixelFormat_PVRTC4,
							 bm.bitmap->width,bm.bitmap->height);
						 break;
					 case bm1Bit:
						 bm.bitmapID = raster->texInitWithData(bm.bitmap->baseAddr,kCCTexture2DPixelFormat_PVRTC2,
							 bm.bitmap->width,bm.bitmap->height);
						 break;
					}*/
				}
			}
		}break;
	case glColorGradient:
		 {
			 this->grad.invMat = ShapeAlloc::GetInst()->AllocRamp();
			 if(!grad.invMat) 
			 {
				colorType = glColorSolid;
				this->color[0] = this->rgb.red/255.0;
				this->color[1] = this->rgb.green/255.0;
				this->color[2] = this->rgb.blue/255.0;
				this->color[3] = this->rgb.alpha/255.0;
			 }
			 else
			 {
				 //_GLMATRIX mMat={16384*2.0f,0,0,16384*2.0f,-16384,-16384};
				 //if(grad.gradStyle==fillLinearGradient)
				 //{
				//	 _GLMATRIX mMat={16384*2.0f,0,0,16384*2.0f,-16384,-16384};
				 //_GLMATRIX dMat={1/20.0f,0,0,1/20.0f,0,0};
				 _GLMATRIX mat,out;
				 GLSetMatrix(grad.savedMat,mat);
				 if(grad.gradStyle==fillLinearGradient)
				 {
					_GLMATRIX mMat={16384*8.0f,0,0,16384*8.0f,-16384,-16384};
					GLMatrixConcat(&mMat,&mat,out);
				 }
				 else
				 {
					 _GLMATRIX mMat={16384*4.0f,0,0,16384*4.0f,0,0};
					GLMatrixConcat(&mMat,&mat,out);
				 }
				 //GLMatrixConcat(&out,&dMat,mat);
				 GLMatrixInvert(&out,&mat);
				 GLMatrix9To16(mat,grad.invMat);
				//MATRIX invMat;
				//MatrixInvert(&grad.savedMat,&invMat);
				//float* pMat = (float*)grad.pData;
				//memset(grad.invMat,0,sizeof(float)*16);
				//raster->SetGLMatrix(invMat,grad.invMat);
				// Create the color ramp
				RGB8 pixel[256*4];// = grad.colorRamp = raster->CreateRamp();
				RGB8* c = pixel;
				SRGB c1, c2;
				S32 cr1, cr2;
				S32 w1, w2, wT;

				cr1 = 0;
				cr2 = grad.ramp.colorRatio[0];
				c1 = c2 = grad.ramp.color[0];
				int ci = 1;

				for ( int i = 0; i <= 256; i++, c++ ) {
					if ( i > cr2 ) {
						cr1 = cr2;
						c1 = c2;
				 		if ( ci < grad.ramp.nColors ) {
							// Get the next color in the ramp
							cr2 = grad.ramp.colorRatio[ci];
							c2 = grad.ramp.color[ci];
							ci++;
						} else {
							// We went past the end
							cr2 = 256;
						}
					}
					w1 = cr2 - i;
					w2 = i - cr1;
					wT = w1+w2;
					if ( wT > 0 ) {
						// change div to fixed mul
						c->alpha = (U8)((w1*c1.rgb.transparency + w2*c2.rgb.transparency)/wT);
						c->blue   = (U8)((w1*c1.rgb.red   + w2*c2.rgb.red  )/wT);
						c->green = (U8)((w1*c1.rgb.green + w2*c2.rgb.green)/wT);
						c->red  = (U8)((w1*c1.rgb.blue  + w2*c2.rgb.blue )/wT);
					} else {
						c->alpha = c1.rgb.transparency;
						c->blue   = c1.rgb.red;
						c->green = c1.rgb.green;
						c->red  = c1.rgb.blue;
					}

					{// Pre-multiply the RGB value
						/*int a = c->alpha;
						if ( a < 255 ) {
							if ( a == 0 ) {
								c->red = c->green = c->blue = 0;
							} else {
								a++;
								c->red   = (c->red   * a)>>8;
								c->green = (c->green * a)>>8;
								c->blue  = (c->blue  * a)>>8;
							}
						}*/
					}
				}
				int i;
				switch(grad.ramp.spreadMode)
				{
					case _glRepeatEdge:
						 memcpy(pixel+257,pixel,256*sizeof(RGB8));
						 memcpy(pixel+512,pixel,256*sizeof(RGB8));
						 memcpy(pixel+768,pixel,256*sizeof(RGB8));
						 //for(i=1;i<4;i++)
						 //	 memcpy(pixel+256*i,pixel,256*sizeof(RGB8));
						 break;
					case _glMirrorEdge:
						 {
							 RGB8 rp[256];
							 for(i=0;i<256;i++)
								 rp[i]=pixel[256-i];
							  memcpy(pixel+257,rp,255*sizeof(RGB8));
							  memcpy(pixel+512,pixel,256*sizeof(RGB8));
							  memcpy(pixel+768,rp,256*sizeof(RGB8));
							 //for(i=2;i<4;i+=2)
							 //{
							//	 memcpy(pixel+256*i,pixel,256*sizeof(RGB8));
							//	 memcpy(pixel+256*(i-1),rp,256*sizeof(RGB8));
							 //}
						 }
						 break;
					case _glClampEdge:
						 for(i=257;i<256*4;i++)
							 pixel[i]=pixel[256];
						 break;
				}
				grad.bitmapID = RasterGL::sharedRasterGL()->texInitWithData(pixel,Texture2DPixelFormat_RGBA8888,256*4,1);

			}
		 }
		 break;
	}
}


void GLColor::FreeCache()
{
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	switch(colorType)
	{
	case glColorDotFont:
		 //if(fm.invMat)
		 //	 pAlloc->FreeRamp(fm.invMat);
		 if(fm.bitmapID)
			 RasterGL::sharedRasterGL()->DeleteTexture(fm.bitmapID);
		 break;
	case glColorGradient:
		 if(grad.invMat)
		 {
			 pAlloc->FreeRamp(grad.invMat);
		 }
		 if(grad.bitmapID)
			 RasterGL::sharedRasterGL()->DeleteTexture(grad.bitmapID);
		 break;
	case glColorDib:
		 if(dib.invMat)
			 pAlloc->FreeRamp(dib.invMat);
		 RasterGL::UnLockDisplay(dib.bitmapID);
		 RasterGL::UnLockTexture(dib.bitmapID);
		 //if(dib.bitmapID)
		 //	 RasterGL::sharedRasterGL()->DeleteTexture(dib.bitmapID);
		 break;
	case glFrameBuffer:
		 if(frame.invMat)
			 pAlloc->FreeRamp(frame.invMat);
		 if(frame.frame)
		 {
			RasterGL::UnLockDisplay(frame.frame->mTexture);
			RasterGL::UnLockTexture(frame.frame->mTexture);
		 }
		 break;
	case glColorBitmap:
		 if(bm.invMat)
			pAlloc->FreeRamp(bm.invMat);
		 if(bm.bitmap)
		 {
			 bm.bitmap->UnlockBits();
			 RasterGL::UnLockDisplay(bm.bitmap->bitmapID);
		 }
		 //if(bm.bitmapID)
		 //	 raster->DeleteTexture(bm.bitmapID);
		 break;
	}
}


//CoverEntry PixCoverage[8][8];
void XCXForm::Clear()
{
	flags = 0;
	aa = ra = ga = ba = 256;
	ab = rb = gb = bb = 0;
	nForeColor=0;
	nBlendMode=0;
}

int ApplyChannel(int x, int a, int b)
{
#ifdef WIN16
	x = ((x*(a>>1))>>7)+b;     // avoid overflow
#else
	x = ((x*a)>>8)+b;
#endif
	if ( (x & 0xFF00) == 0 )
		return x;
	else if ( x > 0 )
		return 255;
	else
		return 0;
}

int ApplyChannelB(int x, int a, int b)
{
	x = x+b;
	if ( (x & 0xFF00) == 0 )
		return x;
	else if ( x > 0 )
		return 255;
	else
		return 0;
}

void XCXForm::Apply(RGBI* c, int n)
{
	if(nForeColor)
	{
		//color=nForeColor;
		//if(c.alph
		//c->alpha=c.alpha;nForeColor>>24;
#ifdef _ARGB_PIXEL_
		c->blue=(nForeColor>>16)&0xff;
		c->green=(nForeColor>>8)&0xff;
		c->red=(nForeColor)&0xff;
#else
		c->red=(nForeColor>>16)&0xff;
		c->green=(nForeColor>>8)&0xff;
		c->blue=(nForeColor)&0xff;
#endif
		return;
	}
	if ( flags == 0 ) return;
	if ( flags & needA ) {
		while ( n-- ) {
			c->alpha = ApplyChannel(c->alpha, aa, ab);
			c->red   = ApplyChannel(c->red, ra, rb);
			c->green = ApplyChannel(c->green, ga, gb);
			c->blue  = ApplyChannel(c->blue, ba, bb);
			c++;
		}
	} else {
		// We don't need the multiply
		while ( n-- ) {
			c->alpha = ApplyChannelB(c->alpha, aa, ab);
			c->red   = ApplyChannelB(c->red, ra, rb);
			c->green = ApplyChannelB(c->green, ga, gb);
			c->blue  = ApplyChannelB(c->blue, ba, bb);
			c++;
		}
	}
}

void XCXForm::Apply(GradColorRamp* ramp)
{
	if ( flags == 0 ) return;
	FLASHASSERT(ramp->nColors<=fillMaxGradientColors);
	SRGB* c = ramp->color;
	for ( int i = ramp->nColors; i--; c++ ) {
		if(nForeColor)
		{
			//c->rgb.transparency=nForeColor>>24;
			//c->rgb.red=(nForeColor>>16)&0xff;
			//c->rgb.green=(nForeColor>>8)&0xff;
			//c->rgb.blue=(nForeColor)&0xff;
			c->all=nForeColor;
			continue;
		}
		c->rgb.transparency = (U8)ApplyChannel(c->rgb.transparency, aa, ab);
		c->rgb.red   = (U8)ApplyChannel(c->rgb.red  , ra, rb);
		c->rgb.green = (U8)ApplyChannel(c->rgb.green, ga, gb);
		c->rgb.blue  = (U8)ApplyChannel(c->rgb.blue , ba, bb);
	}
}

void XCXForm::CalcFlags()
{
	flags = 0;
	if ( aa != 256 || ra != 256 || ga != 256 || ba != 256 )
		flags |= needA;
	if ( ab || rb || gb || bb )
		flags |= needB;
}

void XCXForm::Concat(XCXForm* cx)
{
	if ( cx->flags == 0 )
	{
		if(cx->nForeColor)
			nForeColor=cx->nForeColor;
		return;
	}
	if ( flags == 0 ) {
		int mode=this->nBlendMode;
		*this = *cx;
		if(nBlendMode<2&&mode)
			nBlendMode = mode;
		return;
	}

	/*
		x' = a*x + b
		x'' = a1*x' + b1
		x'' = a1*(a0*x + b0) + b1 = (a1*a0)*x + (a1*b0+b1)
	*/
	nForeColor=cx->nForeColor;
	ab += ((S32)aa*cx->ab)>>8;// alpha
	aa  = ((S32)aa*cx->aa)>>8;

	rb += ((S32)ra*cx->rb)>>8;// red
	ra  = ((S32)ra*cx->ra)>>8;

	gb += ((S32)ga*cx->gb)>>8;// green
	ga  = ((S32)ga*cx->ga)>>8;

	bb += ((S32)ba*cx->bb)>>8;// blue
	ba  = ((S32)ba*cx->ba)>>8;

	if(cx->nForeColor)
		nForeColor = cx->nForeColor;

	CalcFlags();
}

SRGB XCXForm::Apply(SRGB c)
{
	if(nForeColor)
	{
	   SRGB res;
	   res.all=nForeColor;
	   return res;
	}
	if ( flags == 0 ) return c;
	SRGB res;
	res.rgb.transparency = (U8)ApplyChannel(c.rgb.transparency, aa, ab);
	res.rgb.red   = (U8)ApplyChannel(c.rgb.red  , ra, rb);
	res.rgb.green = (U8)ApplyChannel(c.rgb.green, ga, gb);
	res.rgb.blue  = (U8)ApplyChannel(c.rgb.blue , ba, bb);
	return res;
}

void XCXForm::Apply(XU32&color)
{
	if(nForeColor)
	{
		color=nForeColor;
		return;
	}
	XU8*p=(XU8*)&color;
	p[3] = (U8)ApplyChannel(p[3], aa, ab);
	p[2]   = (U8)ApplyChannel(p[2]  , ra, rb);
	p[1] = (U8)ApplyChannel(p[1], ga, gb);
	p[0]  = (U8)ApplyChannel(p[0] , ba, bb);
	//LOGWHEREVAL(p[3]);
	//LOGWHEREVAL(p[2]);
}


BOOL Equal(XCXForm* a, XCXForm* b)
{
	return	a->flags == b->flags &&
			a->aa == b->aa && a->ab == b->ab &&
			a->ra == b->ra && a->rb == b->rb &&
			a->ga == b->ga && a->gb == b->gb &&
			a->ba == b->ba && a->bb == b->bb;
}
//////////////////////////////////////////////////////////////////////////