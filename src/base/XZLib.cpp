// XZLib.cpp: implementation of the XZLib class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XZLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


XZLib::XZLib()
{
  //state.blocks=XNULL;
}

XZLib::~XZLib()
{
   //Release();
	m_pInData=XNULL;
	m_pOutData=XNULL;
	inflateEnd(&d_stream);
	//inflateInit
	//d_stream.state.
}



XU8 XZLib::Decode(XEvent*pStop)
{
	if(m_pInData==XNULL) return XDC_NOTINIT;
	//next_in=m_pInData->ReadAddr();
	//avail_in=m_pInData->GetDataSize();
	//total_in=0;

	d_stream.next_in=m_pInData->ReadAddr();
	d_stream.avail_in=m_pInData->GetDataSize();
	d_stream.total_in=0;

	//if(m_pOutData->GetMenSize()
	XU32 fs=m_pOutData->GetFreeSize();
	/*if(bReset&&fs<d_stream.avail_in)
	{
		fs=d_stream.avail_in;
		m_pOutData->SetSize(m_pOutData->GetDataSize()+fs);
	}*/

	d_stream.next_out  = m_pOutData->WriteAddr();
	XU8*p=d_stream.next_out;
	d_stream.avail_out = fs;
	d_stream.total_out = 0;

	XU8 s=Decode0(pStop);

	m_pInData->Skip(d_stream.total_in);
	m_pOutData->WriteData(NULL,d_stream.total_out);//readPos+=d_stream.total_out;

	return s;
}

//inline XU8 XZLib::Decode0(XEvent*pStop)
//{
//
//  int r=XDC_OK;
////  uInt b;
//  //next_in=m_pInData->ReadAddr();
//
//  while (1)
//  {
//	  if(pStop!=XNULL&&pStop->IsEvent(0)) return XDC_STOP;
//
//	  int err = inflate(&d_stream, Z_NO_FLUSH);
//	  switch(err)
//	  {
//	  case Z_OK: return XDC_OK;break;
//	  case Z_STREAM_END:return XDC_FINISHED;
//	  case Z_NEED_DICT:return XDC_NODICT;
//	  case Z_ERRNO:
//	  case Z_STREAM_ERROR:
//	  case Z_DATA_ERROR:
//	  case Z_MEM_ERROR:
//	  case Z_BUF_ERROR:
//	  case Z_VERSION_ERROR:return XDC_BAD;
//	  }
//
//	}
//  return r;
//}

