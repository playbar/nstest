
#include "WindowView.h"
//#include <stdio.h>
#include "Raster_gl.h"
//#include "gl/glew.h"
//#import "OpenGLView.h"
#include "GlobalPortingFunction.h"

#define MAX_LEN  10240
#define FLT_EPSILON     1.192092896e-07F 


WindowView* WindowView::m_pInstance= NULL;

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
    //return m_pInstance!= NULL;
    return g_openGLViewInitOK();
}

WindowView::~WindowView()
{
	//if(m_hWnd)
	//	DestroyWnd(m_hWnd);
	destroyGL();
}


void WindowView::AttachGL()
{
	RasterGL::sharedRasterGL()->SetWinSize( mWidth, mHeight );
}

void WindowView::SetHW( int width, int height )
{
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
//	if (m_hDC != NULL && m_hRC != NULL)
//	{
//		// deselect rendering context and delete it
//		wglMakeCurrent(m_hDC, NULL);
//		wglDeleteContext(m_hRC);
//	}
}

bool WindowView::initGL()
{
    MakeCurrent(true);
	return true;
}

void WindowView::MakeCurrent( bool iscurrent )
{
    g_makeCurrent();

}

void WindowView::SwapBuffers()
{
    g_freshGles();
}

