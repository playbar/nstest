// 7zwrapper.cpp - based on LzmaUtil.c in lzma920.tar.bz2
//
#include "7zwrapper.h"
#include "Alloc.h"

static void *SzAlloc(void *p, SizeT size) {	return MyAlloc(size); }
static void SzFree(void *p, void *address) { MyFree(address); }

static ISzAlloc g_Alloc = { SzAlloc, SzFree };
static ISzAlloc g_AllocBig = { SzAlloc, SzFree };

/////////////////////////////////////////////////////////////////////////////
//
// Decoder
//
C7ZDecoder::C7ZDecoder(): m_nState(STATE_BEGIN) { Clean(); }

C7ZDecoder::~C7ZDecoder() { Clean(); }

void C7ZDecoder::Clean()
{
	if (m_nState == STATE_WORKING || m_nState == STATE_END || m_nState == STATE_ERROR)
	{
		LzmaDec_Free(&m_decState, &g_Alloc);
	}

	m_nState = STATE_BEGIN;
}


// PARAMS:
//	pHeader		[IN] the header bytes
//	lenHeader	[IN] length of the header bytes, must be LZMA_HDR_LEN
//
SRes C7ZDecoder::Begin(const Byte *pHeader, SizeT lenHeader)
{
	Clean();

	if (lenHeader != LZMA_HDR_LEN)
		return SZ_ERROR_DATA;

	LzmaDec_Construct(&m_decState);
	SRes res = LzmaDec_Allocate(&m_decState, pHeader, LZMA_PROPS_SIZE, &g_Alloc);
	if (res == SZ_OK)
	{
		m_unpackSize = 0;
		for (int i = 0; i < 8; i++)
			m_unpackSize += ((UInt64)(Byte)pHeader[LZMA_PROPS_SIZE + i]) << (i * 8);
		m_bThereIsSize = (m_unpackSize != (UInt64)(Int64)-1);
		LzmaDec_Init(&m_decState);
		m_nState = STATE_WORKING;
	}

	return res;
}

SRes C7ZDecoder::Begin0(const Byte *pHeader)
{
	Clean();

	//if (lenHeader != LZMA_HDR_LEN)
	//	return SZ_ERROR_DATA;

	LzmaDec_Construct(&m_decState);
	SRes res = LzmaDec_Allocate(&m_decState, pHeader+4, LZMA_PROPS_SIZE, &g_Alloc);
	if (res == SZ_OK)
	{
		m_unpackSize = (pHeader[0]<<24)|(pHeader[1]<<16)|(pHeader[2]<<8)|pHeader[3];
		//for (int i = 0; i < 8; i++)
		//	m_unpackSize += ((UInt64)(Byte)pHeader[LZMA_PROPS_SIZE + i]) << (i * 8);
		m_bThereIsSize = (m_unpackSize != (UInt64)(Int64)-1);
		LzmaDec_Init(&m_decState);
		m_nState = STATE_WORKING;
	}

	return res;
}


SRes C7ZDecoder::End()
{
	if (m_nState != STATE_WORKING)
	{
		return SZ_ERROR_FAIL;
	}
	else // STATE_WORKING
	{
		//LzmaDec_Free(&m_decState,&g_Alloc);
		//Clean();
		m_nState = STATE_END;
		return SZ_OK;
	}
}

// PARAMS:
//	pBufIn	input buffer
//	pLenIn	[IN] number of bytes in input buffer
//			[OUT] number of undecoded bytes left in input buffer
//	dest	output buffer
//	destLen	[IN] size of output buffer
//			[OUT] number of decoded bytes in output buffer
//
// RETURN:
//	SZ_OK	success
//	SZ_...	error
//
SRes C7ZDecoder::Decode(const Byte *src, SizeT *srcLen, Byte * dest, SizeT *destLen)
{
	int res;

	if (m_nState != STATE_WORKING)
	{
		return SZ_ERROR_PARAM;
	}
	else // STATE_WORKING
	{
		ELzmaStatus status;

		ELzmaFinishMode finishMode = LZMA_FINISH_ANY;
		if (m_bThereIsSize && *destLen > m_unpackSize)
		{
			*destLen = (SizeT)m_unpackSize;
			finishMode = LZMA_FINISH_END;
		}

		res = LzmaDec_DecodeToBuf(&m_decState, dest, destLen, src, srcLen, finishMode, &status);

		m_unpackSize -= *destLen;

		if (res != SZ_OK || m_bThereIsSize && m_unpackSize == 0)
	        return res;

		if (*srcLen == 0 && *destLen == 0)
		{
			if (m_bThereIsSize || status != LZMA_STATUS_FINISHED_WITH_MARK)
				return SZ_ERROR_DATA;
			return res;
		}

		return SZ_OK;
	}
}

