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
#else
typedef unsigned __int64	XU64;
typedef signed __int64		XS64;
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
const XU16* _convertWchar_toU16(const wchar_t* x);
inline XU16 _convertWchar_toU16(wchar_t x) {
	return (XU16)x;
}
#define _XW(x) (_convertWchar_toU16((L ## x)))
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

typedef	unsigned short XWCHAR;
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

//#ifdef WIN32
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
//#endif

#define XMIN(XX,YY) ((XX)<(YY)?(XX):(YY))
#define XMAX(XX,YY) ((XX)<(YY)?(YY):(XX))
#define XABS(XX) ((XX)<0?-(XX):(XX))

#ifdef __APPLE__
#define _stdcall 
#endif

#ifdef _SYMBIAN
extern XPCWSTR wcschr(XPCWSTR,XWCHAR);
#endif
#endif
