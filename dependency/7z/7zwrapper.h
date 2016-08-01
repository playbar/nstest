// 7zwrapper.h
#ifndef _7ZWRAP_H
#define _7ZWRAP_H

#include "LzmaDec.h"
#include "LzmaEnc.h"

#define LZMA_HDR_LEN		(LZMA_PROPS_SIZE + 8)
#define LZMA_IS_LEN(len)	((len) != (UInt64)(Int64)-1)

class C7ZDecoder
{
public:
	C7ZDecoder();
	~C7ZDecoder();

	void Clean();
	SRes Begin(const Byte *pHeader, SizeT lenHeader);
	SRes Begin0(const Byte *pHeader);
	SRes Decode(const Byte *src, SizeT *srcLen, Byte * dest, SizeT *destLen);
	SRes End();

private:

	enum {
		STATE_BEGIN,
		STATE_WORKING,
		STATE_END,
		STATE_ERROR
	};

	int m_nState;
	bool m_bThereIsSize;
	UInt64 m_unpackSize;
	CLzmaDec m_decState;
};

#endif // _7ZWRAP

