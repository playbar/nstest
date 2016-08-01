// XMp3Decomp.cpp: implementation of the XMp3Decomp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include <math.h>
#include <assert.h>
#include "XMp3Decomp.h"
#include "mp3decifc.h"
#include "mpegheader.h"
#include "XStream.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#pragma comment(lib, "GGBase/mp3decoder.lib")
#ifdef __MAD
# define MAD_RECOVERABLE(error)	((error) & 0xff00)

#ifdef __APPLE__

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
	m_firstBuf = true;

    //
    // open decoder instance
 
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
	if (snd->dataLen == 0)
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
	unsigned char	*ReadStart;
	int acceptCount;

	//if(snd->dataLen<srcIndex)
	//	int v=0;
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

void XMp3Decomp::Decompress(XS16* dstWord, XS32 nSamples) 
{

    long            bytesCleared;
    XS32             nBytes  = nSamples * snd->BytesPerBlock();
    XS8              *dstByte = (XS8 *) dstWord;

//    XBOOL            seeking = (!dstWord) ? (true) : false;

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
				if (!fillBufferMad())
				{
//					finishMad();
					goto exit_gracefully;
				}
			}
			else
			{
				// unrecovered error
				//finishMad();
				//printf("mad:unrecovered error\n");
				goto exit_gracefully;
			}
		}
	}

exit_gracefully:
    ;
}

#else
double MyLdExp(double x, int n)
{
    if (n == 0)
        return x;

    if (n == 1)
        return x + x;

    if (n > 0)
	    return x * (1 <<  n);
    else
        return x / (1 << n);
}

double CubeRoot(double x)
{
	const double CBRT2 = 1.2599210498948731648; /* 2^(1/3) */
	const double SQR_CBRT2 = 1.5874010519681994748; /* 2^(2/3) */

	static const double factor[5] =
	{
	  1.0 / SQR_CBRT2,
	  1.0 / CBRT2,
	  1.0,
	  CBRT2,
	  SQR_CBRT2
	};

	double xm, ym, u, t2;
	int xe;

	/* Reduce X.  XM now is an range 1.0 to 0.5.  */
	xm = frexp (fabs (x), &xe);

	/* If X is not finite or is null return it (with raising exceptions
	 if necessary.  */
	if (xe == 0 && (x == 0.0 /*|| isnan (x) || isinf (x)*/))
	return x + x;

	u = (0.354895765043919860
	  + ((1.50819193781584896
	 + ((-2.11499494167371287
		+ ((2.44693122563534430
		   + ((-1.83469277483613086
		  + (0.784932344976639262 - 0.145263899385486377 * xm) * xm)
		  * xm))
		   * xm))
		* xm))
	 * xm));

	t2 = u * u * u;

	ym = u * (t2 + 2.0 * xm) / (2.0 * t2 + xm) * factor[2 + xe % 3];

	return MyLdExp(x > 0.0 ? ym : -ym, xe / 3);
}
///////////////////////////////////////////
//
// mp3 decoder used by flash
//
XMp3Decomp::XMp3Decomp()
{
    src = 0;            // mpeg src data
    bufLength = 0;      // number of bytes last filled with
    bufIndex = 0;       // index into the out buffer
    srcIndex = 0;
    snd = 0;
    mp3Handle = 0;

    //
    // open decoder instance
    //
#ifdef WINDOWS
    if (!SSC_SUCCESS(mp3decOpen(&mp3Handle, 0, 0, 0, 1)))
#else
    if (!SSC_SUCCESS(mp3decOpen(&mp3Handle, 0, 0, 0, 0)))
#endif
        mp3Handle = 0;
}

XMp3Decomp::~XMp3Decomp()
{
    if (mp3Handle)
        mp3decClose(mp3Handle);

    mp3Handle = 0;
}

void XMp3Decomp::Setup(XSound* snd, XBOOL reset)
{
    //FLASHASSERT(snd->CompressFormat() == sndCompressMP3);

	src = (XU8 *) snd->samples;
    bufLength = 0;      // number of bytes last filled with
    bufIndex = 0;       // index into the out buffer
    srcIndex = 0;
    this->snd = snd;

    if (reset)
        mp3decReset(mp3Handle);

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

void XMp3Decomp::Decompress(XS16* dstWord, XS32 nSamples) 
{
    long            bytesCleared;
    int             outBytes;
    XS32             nBytes  = nSamples * snd->BytesPerBlock();
    XS8              *dstByte = (XS8 *) dstWord;
    SSC             ssc;    // status code (returned by decoder interface functions)
    BOOL            seeking = (!dstWord) ? (true) : false;

    // check first if there is any buffered data
    if ((bytesCleared = GetBufferedData(dstByte, nBytes)) == nBytes)
        goto exit_gracefully;
    else if (!mp3Handle)
    {
        // fill with silence
        if (dstByte)
            memset(dstByte, 0x00, nBytes);

        goto exit_gracefully;
    }
    else
    {
        nBytes -= bytesCleared;

        // dst byte can be zero during seeking
        if (dstByte)
            dstByte += bytesCleared;
    }

    // Loop until error occurs or bytes are saved for next pass
	while(nBytes > 0)
	{
        outBytes = 0;
        ssc = mp3decDecode(mp3Handle, pcmBuf, kBufSize, &outBytes);
        bufLength = outBytes;
        
        switch (ssc)
		{
			case SSC_W_MPGA_SYNCEOF:
                // are we actually ever going to get here ?
                mp3decSetInputEof(mp3Handle);

                // fill with silence
                if (dstByte)
                    memset(dstByte, 0x00, nBytes - outBytes);

                goto exit_gracefully;
				break;

            // informational message
            case SSC_I_MPGA:
            case SSC_I_MPGA_CRCERROR:
            case SSC_I_MPGA_NOMAINDATA:
			case SSC_OK:
			{
				// Everythings just fine...
				// check for outputted data
				if( outBytes > 0 )
				{
                    // this should work out for us. If there is more buffered data
                    // then we need, nBytes will be <= 0 else the GetBufferedData()
                    // will clear out the bufOut
                    bytesCleared = GetBufferedData(dstByte, nBytes);
                    nBytes -= bytesCleared;
    
                    // dst byte can be zero during seeking
                    if (dstByte)
                        dstByte += bytesCleared;
                }
                else
                {
                    mp3decSetInputEof(mp3Handle);
                    goto exit_gracefully;
                }
				break;
			}

            // Fill the decoder input buffer
            case SSC_W_MPGA_SYNCSEARCHED:
			case SSC_W_MPGA_SYNCNEEDDATA:
			{
				int acceptCount; 
                int bytesRefilled;
                int samplesToSkip = nBytes / snd->BytesPerBlock();

                // see if we are seeking and data has at least four bytes left
                if (seeking && samplesToSkip > 0)
                {
                    CMpegHeader mp3Hdr;
                    
                    // skip full frames of sound
                    while (true)
                    {
                        // make sure we can read the frame header
                        if ((snd->dataLen - srcIndex) < 4)
                            break;
                      
                        // read the frame header
                        unsigned long hdr = src[srcIndex]     << 24 |
                                            src[srcIndex + 1] << 16 |
                                            src[srcIndex + 2] <<  8 |
                                            src[srcIndex + 3];

                        //  init header variables
                        mp3Hdr.FromInt(hdr);

                        int frameLen = mp3Hdr.GetFrameLen();
                        int samplesPerFrame = mp3Hdr.GetSamplesPerFrame();

                        // make sure we have a min of delay samples to skip
                        //
                        // NOTE:
                        // during seeking start decompression at least delay samples
                        // before the destination point. This allows the mp3
                        // decoder to start interpolating before we get to the
                        // actual data to be played.
                        if (snd->delay + samplesPerFrame >= samplesToSkip)
                            break;

                        // make sure we don't run out of our data
                        if ((srcIndex + frameLen / 8) > snd->dataLen)
                        {
                           // FLASHASSERT(0);
                            break;
                        }

                        // skip the frame
                        samplesToSkip -= samplesPerFrame;
                        srcIndex += (frameLen / 8);
                        nBytes -=  (samplesPerFrame * snd->BytesPerBlock()); 
                    }
                }

                mp3decGetInputFree(mp3Handle, &acceptCount);
				if (acceptCount > 0)
				{
                    /* refill input buffer */
                    acceptCount = min((long)(snd->dataLen - srcIndex), (long) acceptCount);
					mp3decFill(mp3Handle, &src[srcIndex], acceptCount, &bytesRefilled);

                    // we ran out of source data. We are probably sound streaming
                    // so we might get the data in the next stream block
                    if (bytesRefilled == 0)
                        goto exit_gracefully;

                    srcIndex += bytesRefilled;
                }
                else
                {
                    mp3decSetInputEof(mp3Handle);
                    goto exit_gracefully;
                }
			}
            break;

            case SSC_W_MPGA_SYNCLOST:
	            //FLASHASSERT(0);
                mp3decSetInputEof(mp3Handle);
                goto exit_gracefully;

            default:
	            //FLASHASSERT(0);
                mp3decClose(mp3Handle);
                mp3Handle = 0;
                goto exit_gracefully;
				break;
        }
	}

exit_gracefully:
    ;
}
#endif