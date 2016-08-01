// XMp3Decomp.cpp: implementation of the XMp3Decomp class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include <math.h>
#include <assert.h>
#include "XMp3Decomp.h"
#include "XStream.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

# define MAD_RECOVERABLE(error)	((error) & 0xff00)

static inline int min(int x, int y)
{
	return x > y ? y : x;
}

static inline
signed int scale(mad_fixed_t sample)
{
  sample += (1L << (MAD_F_FRACBITS - 16));

  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

XMp3Decomp::XMp3Decomp()
{
	m_isInitMad = false;
    src = 0;            // mpeg src data
    bufLength = 0;      // number of bytes last filled with
    bufIndex = 0;       // index into the out buffer
    srcIndex = 0;
    snd = 0;
	m_firstBuf = true;

}

void XMp3Decomp::finishMad()
{
	if (m_isInitMad)
	{
		mad_synth_finish(&Synth);
		mad_frame_finish(&Frame);
		mad_stream_finish(&Stream);
		m_isInitMad = false;
	}
}

void XMp3Decomp::initMad()
{
	finishMad();
	m_isInitMad = true;
	m_firstBuf = true;
	mad_stream_init(&Stream);
	mad_frame_init(&Frame);
	mad_synth_init(&Synth);
	mad_timer_reset(&Timer);
}

XMp3Decomp::~XMp3Decomp()
{
	finishMad();
}

void XMp3Decomp::Setup(XSound* snd, XBOOL reset)
{
	if (snd->dataLen <= 0)
		return;
	if (!isInitMad())
		initMad();

	src = (XU8 *) snd->samples;
    bufLength = 0;      // number of bytes last filled with
    bufIndex = 0;       // index into the out buffer
    srcIndex = 0;
    this->snd = snd;

    // skip past the mp3 compression delay
    Decompress(0, snd->delay);
}

//////////////////////////////////////////////////
//
// Get buffered data. If return value == input n
// then we had enough data in the buffer
//
long XMp3Decomp::GetBufferedData(XS8 *dst, XS32 n)
{
    int bytesToCopy = n < (bufLength - bufIndex) ? n : (bufLength - bufIndex);

    // dst can bee 0 during seeking
    if (dst)
        memcpy(dst, &pcmBuf[bufIndex], bytesToCopy);

    // adjust the callers index and the current buff pointer
    bufIndex += bytesToCopy;

    if (bufIndex >= bufLength)
        bufIndex = bufLength = 0;

    return bytesToCopy;
}

bool XMp3Decomp::fillBufferMad()
{
	if (snd->dataLen <= 0)
		return false;
	unsigned char	*ReadStart;
	int acceptCount;
	//assert(snd->dataLen >= srcIndex);
	int numDataLeft = snd->dataLen - srcIndex;
	if (numDataLeft == 0)
	{
		return false;
	}

	if (Stream.next_frame != XNULL)
	{
//		srcIndex -= Stream.bufend-Stream.next_frame;
//		assert(srcIndex >= 0);
		int remaining = Stream.bufend - Stream.next_frame;
		memmove(tempBuf, Stream.next_frame, remaining);
		//assert(remaining < kBufSize);
		acceptCount = min(numDataLeft + remaining, kBufSize);
		ReadStart = tempBuf;
		int numFromData = acceptCount - remaining;
		//assert(numFromData > 0);
		memcpy(tempBuf + remaining, src + srcIndex, numFromData);
		srcIndex += numFromData;
	}
	else
	{
		acceptCount = min(numDataLeft, kBufSize);
		ReadStart = src + srcIndex;
		srcIndex += acceptCount;
	}
	mad_stream_buffer(&Stream, ReadStart, acceptCount);
	return true;
}

void XMp3Decomp::Decompress(XS16* dstWord, XS32 nSamples)
{

    long            bytesCleared;
    XS32             nBytes  = nSamples * snd->BytesPerBlock();
    XS8              *dstByte = (XS8 *) dstWord;

	bytesCleared = GetBufferedData(dstByte, nBytes);

	if (!isInitMad())
	{
		if (dstByte)
		{
			memset(dstByte, 0x00, nBytes);
		}
		goto exit_gracefully;
	}
	else
	{
		nBytes -= bytesCleared;

        if (dstByte)
            dstByte += bytesCleared;
	}

	if (m_firstBuf)	// first enter
	{
		// read head. and give data
		bool b = fillBufferMad();
		//assert(b);
		m_firstBuf = false;
	}

    // check first if there is any buffered data
    if (bytesCleared == nBytes)
        goto exit_gracefully;

	mad_error madError;

	while(nBytes > 0)
	{
		int success = mad_frame_decode(&Frame, &Stream);
		madError = Stream.error;
		if (success == 0)
		{
			// do success things.
			mad_synth_frame(&Synth,&Frame);

			struct mad_pcm *pcm = &(Synth.pcm);

			int nchannels = pcm->channels;

			XU8* p = pcmBuf;

			for (int i = 0; i < Synth.pcm.length; i++)
			{
				signed int Sample = scale(Synth.pcm.samples[0][i]);
				*p++ = Sample & 0xff;
				*p++ = (Sample >> 8) & 0xff;
				if (nchannels == 2)
				{
					Sample = scale(Synth.pcm.samples[0][i]);
					*p++ = Sample & 0xff;
					*p++ = (Sample >> 8) & 0xff;
				}
			}

			bufLength = Synth.pcm.length * nchannels * 2;

			bytesCleared = GetBufferedData(dstByte, nBytes);
			nBytes -= bytesCleared;
			if (dstByte)
			{
				dstByte += bytesCleared;
            }
		}
		else
		{
			if(MAD_RECOVERABLE(madError))
			{
				if (madError == MAD_ERROR_LOSTSYNC)
				{
					nBytes = 0;

					goto exit_gracefully;
				}
				else
				{
					nBytes = 0;
					//printf("mad:recoverd error\n");
					goto exit_gracefully;
				}
			}
			else if (madError == MAD_ERROR_BUFLEN)
			{
				if (!fillBufferMad())
				{
					goto exit_gracefully;
				}
			}
			else
			{
				goto exit_gracefully;
			}
		}
	}

exit_gracefully:
    return;
}




