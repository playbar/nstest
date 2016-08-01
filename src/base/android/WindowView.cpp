
#include "WindowView.h"
//#include <stdio.h>
#include "Raster_gl.h"
//#include "gl/glew.h"
//#import "OpenGLView.h"
#include <string.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include "GlesUtil.h"
#include <android/native_window.h>
#include "GlobalFun.h"
#define MAX_LEN  10240
#define FLT_EPSILON     1.192092896e-07F

EGLDisplay bwdisplay;
EGLConfig bwconfig;
EGLint bwnumConfigs;
EGLint bwformat,w,h;
EGLSurface bwsurface;
EGLContext bwcontext;
EGLSurface bwsurface_copy;
EGLContext bwcontext_Share;
EGLint bwwidth;
EGLint bwheight;
GLfloat bwratio;
ANativeWindow *bwwindow;
WindowView* WindowView::m_pInstance= NULL;

#define  LOG_TAG    "WindowView.cpp"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


WindowView::WindowView()
{
    mLeft = 0;
    mTop = 0;
    mWidth = 0;
    mHeight = 0;
}

void WindowView::Release()
{
	if(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance=NULL;
	}
}

WindowView *WindowView::sharedWindowView()
{
	//static WindowView *pView = NULL;
	if ( m_pInstance == NULL )
	{
		m_pInstance = new WindowView();
	}
	return m_pInstance;
}

bool WindowView::InitOK()
{
    return m_pInstance!= NULL;
//    return g_openGLViewInitOK();
    
}

WindowView::~WindowView()
{
	//if(m_hWnd)
	//	DestroyWnd(m_hWnd);
	destroyGL();
}


void WindowView::AttachGL()
{
	RasterGL::sharedRasterGL()->SetWinSize( gWidth, gHeight );
}

void WindowView::SetHW( int width, int height )
{
	LOGWHERE();
    mWidth = width;
    mHeight = height;
}

void WindowView::SetViewPos(int left, int top, int width, int height)
{

	mLeft = left;
	mTop = top;
    mWidth = width;
    mHeight = height;
	//resize( width, height);
	//SetWindowPos(m_hWnd, 0, left, top, width, height, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	//RasterGL::sharedRasterGL()->SetWinSize( width, height );
    
	RasterGL::sharedRasterGL()->setOrthoProjection(  width * 20, height * 20);
	RasterGL::sharedRasterGL()->setViewPort( 0, 0, width, height );
}

void WindowView::destroyGL()
{
    LOGWHERE();
    if( eglMakeCurrent( gDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE )
    {
        LOGWHEREMSG("Release MakeCurrent Error");
        windowViewInitGL_success=true;
    }else{
        windowViewInitGL_success=false;
    }
    
    RasterGL::sharedRasterGL()->mbInit =false;
//    if(bwMode==-1){
//        gWidth=firstWidth;
//        gHeight=firstHeight;
//       
//    }
}

bool WindowView::initGL()
{
    if (eglMakeCurrent( gDisplay, gSurface, gSurface, gShareContext) == EGL_FALSE)
    {
        LOGE("Unable to eglMakeCurrent");
        initGL();
    }else{
        
        LOGWHERE();
        windowViewInitGL_success=true;
    }
    glHint( GL_GENERATE_MIPMAP_HINT, GL_NICEST );
    
    glHint( GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES, GL_NICEST );

    RasterGL::sharedRasterGL()->SetWinSize( gWidth, gHeight);

	return true;
}

void WindowView::MakeCurrent( bool iscurrent )
{
    LOGWHERE();
	if (iscurrent)
	{
        if (eglMakeCurrent( gDisplay, gSurface, gSurface, gShareContext) == EGL_FALSE)
        {
            LOGE("Unable to eglMakeCurrent");
        }
	}
}

void WindowView::SwapBuffers()
{
    //LOGWHERE();
	if (eglSwapBuffers( gDisplay, gSurface)==EGL_FALSE)
	{
		LOGWHEREMSG("SwapBuffer fails");
	}
    
    
}

