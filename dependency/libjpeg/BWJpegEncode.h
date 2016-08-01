#ifndef __BW_JPEG_ENCODE__
#define __BW_JPEG_ENCODE__

#include "XDib.h"
#include "XVector.h"

class XJpegEncode
{
public:
	XU8Array*m_pData;
	XJpegEncode();
	~XJpegEncode();
	void Release();
	bool Encode(XDib&dib,XU8Array&data);
};

#endif