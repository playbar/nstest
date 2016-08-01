#ifndef __BW_PNG_ENCODE__
#define __BW_PNG_ENCODE__

#include "XDib.h"
#include "XVector.h"

class XPngEncode
{
public:
	XU8Array*m_pData;
	XPngEncode();
	~XPngEncode();
	void Release();
	bool Encode(XDib&dib,XU8Array&data);
};

#endif