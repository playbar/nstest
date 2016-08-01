#include "WindowView.h"
#include <stdio.h>
#include "Raster_gl.h"
#include "gl/glew.h"

#define MAX_LEN  10240
#define FLT_EPSILON     1.192092896e-07F 

void CCLog(const char * pszFormat, ...)
{
	char szBuf[MAX_LEN];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[MAX_LEN] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");

	WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	printf("%s\n", szBuf);
}


static WindowView* s_pMainWindow = NULL;
static LRESULT CALLBACK _WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (s_pMainWindow && s_pMainWindow->m_hWnd == hWnd)
	{
		return s_pMainWindow->WindowProc(uMsg, wParam, lParam);
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

static void SetupPixelFormat(HDC hDC)
{

	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size
		1,                          // version
		PFD_SUPPORT_OPENGL |        // OpenGL window
		PFD_DRAW_TO_WINDOW |        // render to window
		PFD_DOUBLEBUFFER,           // support double-buffering
		PFD_TYPE_RGBA,              // color type
		32,                         // preferred color depth
		0, 0, 0, 0, 0, 0,           // color bits (ignored)
		0,                          // no alpha buffer
		0,                          // alpha bits (ignored)
		0,                          // no accumulation buffer
		0, 0, 0, 0,                 // accum bits (ignored)
		24,                         // depth buffer
		8,                          // no stencil buffer
		0,                          // no auxiliary buffers
		PFD_MAIN_PLANE,             // main layer
		0,                          // reserved
		0, 0, 0,                    // no layer, visible, damage masks
	};

	pixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelFormat, &pfd);
}


static bool glew_dynamic_binding()
{
	const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

	// If the current opengl driver doesn't have framebuffers methods, check if an extension exists
	if (glGenFramebuffers == NULL)
	{
		CCLog("OpenGL: glGenFramebuffers is NULL, try to detect an extension");
		if (strstr(gl_extensions, "ARB_framebuffer_object"))
		{
			CCLog("OpenGL: ARB_framebuffer_object is supported");

			glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbuffer");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbuffer");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffers");
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffers");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorage");
			glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameteriv");
			glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebuffer");
			glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebuffer");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffers");
			glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffers");
			glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatus");
			glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1D");
			glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2D");
			glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3D");
			glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbuffer");
			glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
			glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmap");
		}
		else
			if (strstr(gl_extensions, "EXT_framebuffer_object"))
			{
				CCLog("OpenGL: EXT_framebuffer_object is supported");
				glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) wglGetProcAddress("glIsRenderbufferEXT");
				glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) wglGetProcAddress("glBindRenderbufferEXT");
				glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
				glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) wglGetProcAddress("glGenRenderbuffersEXT");
				glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) wglGetProcAddress("glRenderbufferStorageEXT");
				glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) wglGetProcAddress("glGetRenderbufferParameterivEXT");
				glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) wglGetProcAddress("glIsFramebufferEXT");
				glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) wglGetProcAddress("glBindFramebufferEXT");
				glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
				glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) wglGetProcAddress("glGenFramebuffersEXT");
				glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
				glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) wglGetProcAddress("glFramebufferTexture1DEXT");
				glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
				glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) wglGetProcAddress("glFramebufferTexture3DEXT");
				glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
				glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
				glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) wglGetProcAddress("glGenerateMipmapEXT");
			}
			else
			{
				CCLog("OpenGL: No framebuffers extension is supported");
				CCLog("OpenGL: Any call to Fbo will crash!");
				return false;
			}
	}
	return true;
}

WindowView::WindowView()
{
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hRC = NULL;
	//mbInit = false;
	mLeft = 0;
	mTop = 0;
	mWidth = 0;
	mHeight = 0;
}
WindowView* WindowView::m_pInstance=NULL;

void WindowView::Release()
{
	if(m_pInstance)
	{
		delete m_pInstance;
		m_pInstance=NULL;
	}
}

bool WindowView::InitOK()
{
	return m_pInstance != NULL;
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

WindowView::~WindowView()
{
	//if(m_hWnd)
	//	DestroyWnd(m_hWnd);
	destroyGL();
}

void WindowView::CreateView(HWND hwnd, int left, int top, int width, int height )
{
	HINSTANCE hInstance = GetModuleHandle( NULL );
	WNDCLASS  wc;        // Windows Class Structure

	// Redraw On Size, And Own DC For Window.
	wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc    = _WindowProc;                    // WndProc Handles Messages
	wc.cbClsExtra     = 0;                              // No Extra Window Data
	wc.cbWndExtra     = 0;                                // No Extra Window Data
	wc.hInstance      = hInstance;                        // Set The Instance
	wc.hIcon          = LoadIcon( NULL, IDI_WINLOGO );    // Load The Default Icon
	wc.hCursor        = LoadCursor( NULL, IDC_ARROW );    // Load The Arrow Pointer
	wc.hbrBackground  = NULL;                           // No Background Required For GL
	wc.lpszMenuName   = NULL;                         //
	wc.lpszClassName  = L"Raster";               // Set The Class Name

	RegisterClass(&wc);

	// center window position
	RECT rcDesktop;
	GetWindowRect(GetDesktopWindow(), &rcDesktop);


	// create window
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,    // Extended Style For The Window
		L"Raster",                                    // Class Name
		L"Raster",                                                // Window Title
		//WS_CAPTION | WS_POPUPWINDOW | WS_MINIMIZEBOX,        // Defined Window Style
		WS_CHILD,        // Defined Window Style
		0, 0,                                                // Window Position
		//TODO: Initializing width with a large value to avoid getting a wrong client area by 'GetClientRect' function.
		width,                                               // Window Width
		height,                                               // Window Height
		hwnd,                                                // No Parent Window
		NULL,                                                // No Menu
		hInstance,                                            // Instance
		NULL );
	//::SetTimer(m_hWnd, 2, 50,NULL);
	//bool bRet = initGL();
	//if(!bRet)
	//	destroyGL();

	//ShowWindow( m_hWnd, true );

	s_pMainWindow = this;
	//resize( width, height);
	mLeft = left;
	mTop = top;
	mWidth = width;
	mHeight = height;
	SetWindowPos(m_hWnd, 0, left, top, width, height, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	//RasterGL::sharedRasterGL()->SetWinSize( width, height );
	return;
}

void WindowView::AttachGL()
{
	RasterGL::sharedRasterGL()->SetWinSize( mWidth, mHeight );
}


void WindowView::destroyGL()
{
	if (m_hDC != NULL && m_hRC != NULL)
	{
		// deselect rendering context and delete it
		wglMakeCurrent(m_hDC, NULL);
		wglDeleteContext(m_hRC);
	}
}

bool WindowView::initGL()
{
	
	m_hDC = GetDC(m_hWnd);
	//m_hDC = GetDC( m_hParentWnd );
	SetupPixelFormat(m_hDC);
	//SetupPalette();
	m_hRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hRC);

	// check OpenGL version at first
	const GLubyte* glVersion = glGetString(GL_VERSION);

	if ( atof((const char*)glVersion) < 1.5 )
	{
		return false;
	}

	GLenum GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult)
	{
		return false;
	}

	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
	{
		CCLog("Ready for GLSL");
	}
	else
	{
		CCLog("Not totally ready :(");
	}

	if (glewIsSupported("GL_VERSION_2_0"))
	{
		CCLog("Ready for OpenGL 2.0");
	}
	else
	{
		CCLog("OpenGL 2.0 not supported");
	}

	if(glew_dynamic_binding() == false)
	{
		return false;
	}

	// Enable point size by default on windows.
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	AttachGL();
	//wglMakeCurrent(m_hDC, NULL);
	//MakeCurrent( false );

	return true;
}

void WindowView::ShowView( bool bshow )
{
	ShowWindow(m_hWnd, bshow );
}

void WindowView::SetViewPos(int left, int top, int width, int height)
{
	mLeft = left;
	mTop = top;
	mWidth = width;
	mHeight = height;
	//resize( width, height);
	SetWindowPos(m_hWnd, 0, left, top, width, height, SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER);
	//width*=20;
	//height*=20;
	RasterGL::sharedRasterGL()->SetWinSize( width, height );

	//RasterGL::sharedRasterGL()->setOrthoProjection(  width * 20, height * 20);
	//RasterGL::sharedRasterGL()->setViewPort( 0, 0, width, height );
}


void WindowView::resize(int width, int height)
{
	if (! m_hWnd)
	{
		return;
	}
	mWidth = width;
	mHeight = height;

	//this->destroyGL();

	RECT rcWindow;
	GetWindowRect(m_hWnd, &rcWindow);

	RECT rcClient;
	GetClientRect(m_hWnd, &rcClient);

	// calculate new window width and height
	POINT ptDiff;
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	rcClient.right = rcClient.left + width;
	rcClient.bottom = rcClient.top + height;

	AdjustWindowRectEx(&rcClient, GetWindowLong(m_hWnd, GWL_STYLE), FALSE, GetWindowLong(m_hWnd, GWL_EXSTYLE));

	// change width and height
	SetWindowPos(m_hWnd, 0, 0, 0, width + ptDiff.x, height + ptDiff.y,
		SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	//width*=20;
	//height*=20;
	//initGL();
	RasterGL::sharedRasterGL()->SetWinSize( width, height );
	
	 ::InvalidateRect(m_hWnd, NULL, TRUE);
	//RasterGL::sharedRasterGL()->setOrthoProjection(  width , height );
	//RasterGL::sharedRasterGL()->setViewPort( 0, 0, width, height );
}

LRESULT WindowView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(m_hWnd, &ps);
		RasterGL::sharedRasterGL()->mbFresh = true;
		EndPaint(m_hWnd, &ps);
		//Sleep( 1 );
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		{
			int x = LOWORD( lParam );
			int y = HIWORD( lParam );
			x += mLeft;
			y += mTop;
			LPARAM lpar = MAKELONG( x, y );
			HWND hwnd = ::GetParent(m_hWnd);
			SendMessage( hwnd, message, wParam, lpar );
		}
		break;
	case WM_TIMER:
		//SendMessage( GetParent(m_hWnd), message, wParam, lParam );
		break;
	default:
		return DefWindowProc(m_hWnd, message, wParam, lParam);
	}
	return 1;
}

void WindowView::MakeCurrent( bool iscurrent )
{
	if ( iscurrent )
	{ 
		bool re = wglMakeCurrent(m_hDC, m_hRC );
	}
	else
	{
		wglMakeCurrent( m_hDC, NULL );
	}
}

void WindowView::SwapBuffers()
{
	if ( m_hDC != NULL )
	{
		::SwapBuffers(m_hDC);
	}
}

