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

#ifdef __APPLE__

static inline int min(int x, int y)
{
	return x > y ? y : x;
}

#endif

#ifdef _SYMBIAN
static inline int min(int x, int y)
{
	return x > y ? y : x;
}
#endif

#ifdef __ANDROID__
static inline int min(int x, int y)
{
	return x > y ? y : x;
}
#endif

static inline
signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
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
	blockSamples = 0;
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
	assert(snd->dataLen >= srcIndex);
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
		assert(remaining < kBufSize);
		acceptCount = min(numDataLeft + remaining, kBufSize);
		ReadStart = tempBuf;
		int numFromData = acceptCount - remaining;
		assert(numFromData > 0);
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
////	ReadStart = src + srcIndex;
//
//	int acceptCount = min(acceptCount, kBufSize);
//	assert(acceptCount >= 0);
////	assert(acceptCount >= Stream.bufend - Stream.next_frame);
//
//	if (acceptCount == 0)
//		return false;
////	mad_stream_buffer(&Stream, src, 
//	mad_stream_buffer(&Stream,ReadStart,acceptCount);
////	srcIndex += acceptCount;
	return true;
}

#include "XFile.h"

void XMp3Decomp::Decompress(XS16* dstWord, XS32 nSamples) 
{
	if( snd == NULL )
		return;
    long            bytesCleared;
    XS32             nBytes  = nSamples * snd->BytesPerBlock();
	XS32			 nAdjust = snd->format>>24;
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

        // dst byte can be zero during seeking
        if (dstByte)
            dstByte += bytesCleared;
	}

	if (m_firstBuf)	// first enter
	{
		// read head. and give data
		bool b = fillBufferMad();
		if(!b) return;
		assert(b);
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
			/*if(!blockSamples)
			{
				blockSamples=(Frame.header.samplerate*26)/1000;
			}*/
			mad_synth_frame(&Synth,&Frame);

			struct mad_pcm *pcm = &(Synth.pcm);

			int nchannels = pcm->channels;
//			int nsamples  = pcm->length;

			assert(Synth.pcm.length * nchannels * 2 <= kBufSize);
			assert(Synth.pcm.length > 0);

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
			// Adjust for padding
			//bufLength = (Synth.pcm.length-2 )* nchannels * 2;
			///if(Frame.header.flags&MAD_FLAG_PADDING)
			//	bufLength = (Synth.pcm.length-1)* nchannels * 2;
			//else
			//if(Synth.phase)
				bufLength = (Synth.pcm.length-nAdjust)* nchannels * 2;

				//if(bufLength<=0)
				//	int v=0;
			//else
			//	bufLength = (Synth.pcm.length)* nchannels * 2;

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
//#if (__CORE_VERSION__>=0x02071000)
				if(dstByte)
					memset(dstByte, 0x00, nBytes);
//#endif
				if (madError == MAD_ERROR_LOSTSYNC)
				{
					//finishMad();
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
				// need data.
//#if (__CORE_VERSION__>=0x02071000)
				if(dstByte)
					memset(dstByte, 0x00, nBytes);
//#endif
				if (!fillBufferMad())
				{
//					finishMad();
					goto exit_gracefully;
				}
			}
			else
			{
//#if (__CORE_VERSION__>=0x02071000)
				if(dstByte)
					memset(dstByte, 0x00, nBytes);
//#endif
				// unrecovered error
//				finishMad();
//				printf("mad:unrecovered error\n");
				goto exit_gracefully;
			}
		}
	}

	/*static bool bFirst=true;
	if(dstWord)
	{
		XS32             nBytes  = nSamples * snd->BytesPerBlock();
		XFile file;
		{
			if(bFirst)
			{
				file.Open("E:\\test.dat",XFile::XBINARY|XFile::XWRITE|XFile::XCREATE);
				struct WAVHEADER
				{
					char RIFF[4];
					XU32 size;
					char WAVE[4];
					char FMT[4];
					char rev[4];
					short nFmt;
					short nChannel;
					XU32  freq;
					XU32  bitrate;
					short adg;
					short bits;
					char data[4];
					XU32 waveLen;
				}HEADER=
				{
					{'R','I','F','F'},
					1024*512+sizeof(WAVHEADER),
					{'W','A','V','E'},
					{'f','m','t',' '},
					{0,0,0,0},
					0x1,
					2,
					44100,
					2*44100*2,
					2*2,
					16,
					{'d','a','t','a'},
					1024*512
				};
				file.Write(&HEADER,sizeof(HEADER));
				bFirst=false;
			}
			else
				file.Open("E:\\test.dat",XFile::XBINARY|XFile::XWRITE|XFile::XAPPEND);
			file.Write(dstWord,nBytes);
			file.Close();
		}
	}*/
	
exit_gracefully:
    ;
}




