#ifndef __WINDOWVIEW_H__
#define __WINDOWVIEW_H__

#ifdef _WIN32
#include "windows.h"
#endif

class WindowView
{
public:
	static WindowView* m_pInstance;
	static WindowView *sharedWindowView();
	static void Release();
    bool InitOK();
public:

#ifdef _WIN32
	void CreateView(HWND hwnd,int left, int top, int width, int height );
    void ShowView( bool bshow );
    LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif
    ~WindowView();
    void SetHW( int width, int height );
	void SetViewPos(int left, int top, int width, int height);
	void AttachGL();

	void destroyGL();
	void resize(int width, int height);
	
    bool initGL();
	void MakeCurrent( bool iscurrent );
	void SwapBuffers();

private:
	WindowView();

public:
#ifdef _WIN32
	HWND m_hWnd;
	HDC  m_hDC;
	HGLRC m_hRC;
#endif
    
	//bool mbInit;

	int mLeft;
	int mTop;
	int mWidth;
	int mHeight;

};

#endif


