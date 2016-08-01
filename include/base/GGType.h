#ifndef __GGLIB_TYPE__
#define __GGLIB_TYPE__

#include <stdlib.h>

typedef unsigned char	XU8;
typedef unsigned short	XU16;
typedef unsigned long	XU32;
typedef signed char		XS8;
typedef signed short	XS16;
typedef signed long		XS32;
#ifdef __APPLE__
	typedef u_int64_t	XU64;
	typedef int64_t		XS64;
#elif defined(__ANDROID__)
	typedef u_int64_t XU64;
	typedef int64_t   XS64;
#elif defined(__SYMBIAN__ )
	typedef unsigned __int64	XU64;
	typedef signed   __int64	XS64;
#else
	typedef unsigned __int64	XU64;
	typedef signed   __int64	XS64;
#endif

typedef float			XF32;
typedef double			XF64;
typedef signed int		XSINT;
typedef unsigned int	XUINT;
typedef short			XSHORT;
typedef int				XINT;
typedef long			XLONG;
typedef void			XANY;
typedef void*		    XPANY;
typedef int				XBOOL;


#ifdef _SYMBIAN
	#define __stdcall
	#ifdef __WINS__
		#define _XW(x) L ## x
	#else
		const XU16* _convertWchar_toU16(const wchar_t* x);
		inline XU16 _convertWchar_toU16(wchar_t x) {
			return (XU16)x;
		}
		#define _XW(x) (_convertWchar_toU16((L ## x)))
	#endif
#elif defined(_WIN32)
	#define _XW(x) L ## x
#elif defined(__APPLE__)
	#define _stdcall
	const XU16* _convertWchar_toU16(const wchar_t* x);
	#define _XW(x) (_convertWchar_toU16((L ## x)))
#elif defined( __ANDROID__ )
	#define _stdcall
	#ifdef __WINS__
		#define _XW(x) L ## x
	#else
		inline XU16 _convertWchar_toU16(wchar_t x)
		{
			return (XU16)x;
		}
		#define _XW(x) (_convertWchar_toU16((L ## x)))
	#endif
#endif
//#define _XW(x) L ## x

/*#ifdef UNICODE
 #define _XT _XW
#else
 #define _XT
#endif*/

#define XINFINITE       0xFFFFFFFF
#define XNULL			0
#define XTRUE			1
#define XFALSE			0

/*#ifdef _UNICODE
typedef unsigned short XCHAR;
#else
typedef char XCHAR;
#endif
typedef XCHAR *XSTR,*XPSTR;
typedef const XCHAR *XCSTR,*XPCSTR;*/
typedef char  XTCHAR;

typedef	unsigned short 		  XWCHAR;
typedef const unsigned short *XCWSTR;
typedef const unsigned short *XPCWSTR;
typedef const char			 *XCTSTR;
typedef const char			 *XPCTSTR;
//#define XPCTSTR char *
typedef char	*XPTSTR;
typedef XWCHAR*  XPWSTR;

typedef void  XVOID,*XPVOID;
typedef const void* XPCVOID;

typedef void*	XHANDLE;
typedef XUINT	XSOCKET;


#ifndef _XEXT_CLASS
	#ifdef _USR_DLL
		#define _XEXT_CLASS __declspec(dllexport)
	#else
		#define _XEXT_CLASS
	#endif
#endif



#pragma pack(1)

typedef struct _XRECT{XS32 left,top,right,bottom;}XRECT,*XPRECT;
typedef struct _XSIZE{XS32 cx,cy;}XSIZE,*XPSIZE;
typedef struct _XPOINT{XS32 x,y;}XPOINT,*XPPOINT;
typedef struct _XLW{XINT bx,ex;}XLW;
#define LWIDTH(WW) (WW.ex-WW.bx)

typedef struct _XCOLOR
{  union
	{
		XU32 nColor;
        struct {XU8 b,g,r,a;};
	};
	}XCOLOR,*XPCOLOR;

#pragma pack()


#ifdef _SYMBIAN
#define XK_LEFT		14//EKeyLeftArrow
#define XK_RIGHT	15//EKeyRightArrow
#define XK_UP		16//EKeyUpArrow
#define XK_DOWN		17//EKeyDownArrow
#define XK_RETURN	167//EKeyEnter
#define XK_CLEAR	8 //EKeyDelete//0x08
#define XK_CLOSE	197//EKeyOff//0x2E
#define XK_MENU		EStdKeyMenu//0x12
#define XK_LEFTSOFT	164//EKeyLeftFunc//0x13
#define XK_RIGHTSOFT 165//EKeyRightFunc//0x12
#define XK_CALL		196//EKeyDial//0x70
#define XK_NUM2		 '2'
#define XK_NUM0		 '0'
#define XK_NUM1		 '1'
#define XK_NUM3		 '3'
#define XK_NUM4		 '4'
#define XK_NUM5		 '5'
#define XK_NUM6		 '6'
#define XK_NUM7		 '7'
#define XK_NUM8		 '8'
#define XK_NUM9		 '9'
#define XK_PLUS		 '+'
#define XK_WELL		 127//'#'
#define XK_STAR		 133//'*'
#define XK_SPACE	 ' '
#define XK_BACKSPACE EStdKeyBackspace//8
#else
#define XK_LEFT		0x25
#define XK_RIGHT	0x27
#define XK_UP		0x26
#define XK_DOWN		0x28
#define XK_RETURN	0x0D
#define XK_CLEAR	0x08
#define XK_CLOSE	0x2E
#define XK_MENU		0x12
#define XK_LEFTSOFT	0x13
#define XK_RIGHTSOFT 0x12
#define XK_CALL		0x70
#define XK_NUM2		'2'
#define XK_NUM0		'0'
#define XK_NUM1		'1'
#define XK_NUM3		'3'
#define XK_NUM4		'4'
#define XK_NUM5		'5'
#define XK_NUM6		'6'
#define XK_NUM7		'7'
#define XK_NUM8		'8'
#define XK_NUM9		'9'
#define XK_PLUS		'+'
#define XK_WELL		'#'
#define XK_STAR		'*'
#define XK_SPACE	' '
#define XK_BACKSPACE 8
#endif

#define XMIN(XX,YY) ((XX)<(YY)?(XX):(YY))
#define XMAX(XX,YY) ((XX)<(YY)?(YY):(XX))
#define XABS(XX) ((XX)<0?-(XX):(XX))

#ifdef WIN32		// add by hgl 2011-5-26
	#define STDCALL  _stdcall
#elif defined( __ANDROID__ )
	#define STDCALL __attribute__ ((visibility("default")))
#else
	#define STDCALL
#endif

#ifdef _SYMBIAN
//extern XPCWSTR wcschr(XPCWSTR,XWCHAR);
#endif

// add by hgl
#ifdef __ANDROID__
//#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
//#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
//#define LOGWHERE() LOGE("%s, %s, %d", __FILE__, __FUNCTION__, __LINE__ )
//#define LOGWHEREMSG( msg ) LOGE("%s, %s, %d, %s", __FILE__, __FUNCTION__, __LINE__, msg )
//#define LOGWHEREVAL( val ) LOGE("%s, %s, %d, %d", __FILE__, __FUNCTION__, __LINE__, val )
#define __FILENAME__ (strrchr(__FILE__,'/')+1)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, __FILENAME__,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,__FILENAME__,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,__FILENAME__,__VA_ARGS__)
#define LOGWHERE()         LOGE("L: %4d \t F: %s  ",      __LINE__, __FUNCTION__ )
#define LOGWHEREMSG( msg ) LOGE("L: %4d \t F: %s  M: %s", __LINE__, __FUNCTION__, msg )
#define LOGWHEREVAL( val ) LOGE("L: %4d \t F: %s  V: %d", __LINE__, __FUNCTION__, val )
#elif defined (WIN32 )

#include <Windows.h>
#include <stdio.h>
inline void LogWhereFun(const char *pFile, const char *pFun, int iline )
{
	//const char *pName = (strrchr(pFile,'/')+1);
	char pTmp[256] = {0};
	sprintf( pTmp, "File: %s, Fun: %s, Line: %d", pFile, pFun, iline );
	OutputDebugStringA( pTmp );
}
inline void LogWhereFunMsg(const char *pFile, const char *pFun, int iline, const char *pMsg)
{
	//const char *pName = (strrchr(pFile,'/')+1);
	char pTmp[256] = {0};
	sprintf( pTmp, "File: %s, Fun: %s, Line: %d, Msg: %s", pFile, pFun, iline, pMsg );
	OutputDebugStringA( pTmp );
}
inline void LogWhereFunVal(const char *pFile, const char *pFun, int iline,int val)
{
	//const char *pName = (strrchr(pFile,'/')+1);
	char pTmp[256] = {0};
	sprintf( pTmp, "File: %s, Fun: %s, Line: %d, Val: %d", pFile, pFun, iline, val );
	OutputDebugStringA( pTmp );
}
#define __FILENAME__ (strrchr(__FILE__,'\\')+1)
#ifdef _DEBUG
#define LOGWHERE()	LogWhereFun( __FILENAME__, __FUNCTION__, __LINE__ )
#define LOGWHEREMSG( msg ) LogWhereFunMsg( __FILENAME__, __FUNCTION__, __LINE__, msg )
#define LOGWHEREVAL( val ) LogWhereFunVal( __FILENAME__, __FUNCTION__, __LINE__, val )
#else
#define LOGWHERE()	LogWhereFun( __FILENAME__, __FUNCTION__, __LINE__ )
#define LOGWHEREMSG( msg ) LogWhereFunMsg( __FILENAME__, __FUNCTION__, __LINE__, msg )
#define LOGWHEREVAL( val ) LogWhereFunVal( __FILENAME__, __FUNCTION__, __LINE__, val )
#endif

#elif defined( __APPLE__ )
#define __FILENAME__ (strrchr(__FILE__,'/')+1)
#define LOGWHERE()	printf("File: %s, Fun: %s, Line: %d\n", __FILENAME__, __FUNCTION__, __LINE__ );
#define LOGWHEREMSG( msg ) printf( "File: %s, Fun: %s, Line: %d, Msg: %s\n", __FILENAME__, __FUNCTION__, __LINE__, msg );
#define LOGWHEREVAL( val ) printf( "File: %s, Fun: %s, Line: %d, Val: %d\n", __FILENAME__, __FUNCTION__, __LINE__, val );

#else

#define LOGWHERE()
#define LOGWHEREMSG( msg )
#define LOGWHEREVAL( val )
#endif
#endif
