// XZLib.h: interface for the XZLib class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __XZLIB_H__
#define __XZLIB_H__


#include "GGType.h"
#include "XMutex.h"
#include "XStream.h"
#ifdef _SYMBIAN
#include <ezlib.h>
#else
#include "mozzconf.h"
#include "zlib.h"
#endif

enum
	{
		XDC_OK=0,
		XDC_NOTINIT=1,
		XDC_CONTINUE=2,
		XDC_FINISHED=3,
		XDC_NODICT=5,
		XDC_BREAK=6,
		XDC_BAD=7,
		XDC_OUTFULL=8,
		XDC_INPUTEMPTY=9,
		XDC_STOP,
	};

class _XEXT_CLASS XZLib
{
public:
	XZLib();

	virtual ~XZLib();

    XU8 Decode(XEvent*pEvent);

public:
	
	//void Release();
	XU8 Init(XStream*pIn,XStream*pOut)
	{
		if(pIn==XNULL||pOut==XNULL) return XFALSE;
		m_pInData=pIn;
		m_pOutData=pOut;

		d_stream.zalloc = (alloc_func)0;
		d_stream.zfree = (free_func)0;
		d_stream.opaque = (voidpf)0;
		return inflateInit(&d_stream)==Z_OK;
		//return XTRUE;
		//return inflateInita(DEF_BITS);
	}
	
protected:
	inline XU8 Decode0(XEvent*pStop)
	{
		 int r=XDC_OK;
		  while (1)
		  {
			  if(pStop!=XNULL&&pStop->IsEvent(0)) return XDC_STOP;

			  int err = inflate(&d_stream, Z_NO_FLUSH);
			  switch(err)
			  {
			  case Z_OK: return XDC_OK;break;
			  case Z_STREAM_END:return XDC_FINISHED;
			  case Z_NEED_DICT:return XDC_NODICT;
			  case Z_ERRNO:
			  case Z_STREAM_ERROR:
			  case Z_DATA_ERROR:
			  case Z_MEM_ERROR:
			  case Z_BUF_ERROR:
			  case Z_VERSION_ERROR:return XDC_BAD;
			  }

			}
		  return r;
	}

protected:
	//XU8 inflateInita(int w);
	XStream* m_pInData;
	XStream* m_pOutData;

	z_stream d_stream;
	//internal_state_lib state;

};

#endif // !defined(AFX_XZLIB_H__DC523C0B_BD12_4536_A400_33679EE61E9D__INCLUDED_)
