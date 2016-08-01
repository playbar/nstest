// XXSoundMix.cpp: implementation of the XXSoundMix class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafxggbase.h"
#include "XNativeSoundMix.h"
#include "XTime.h"
#ifdef _SYMBIAN
#include <mda\common\audio.h>
#include <centralrepository.h>
#include <profileenginesdkcrkeys.h>
#include "symbiandebug.h"
#elif defined(_WIN32)
#include <mmsystem.h>
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//
// adpcm decompressor class
//
#ifdef _SYMBIAN
#define SOUNDWAITTIME 1000000
// for symbian audio rate
const TInt kSBRateTable[4] = {
		TMdaAudioDataSettings::ESampleRate8000Hz,
		TMdaAudioDataSettings::ESampleRate11025Hz, 
		TMdaAudioDataSettings::ESampleRate22050Hz,
		TMdaAudioDataSettings::ESampleRate44100Hz
};
#endif

static const int indexTable2[2] = {
    -1, 2, 
};

// Is this ok?
static const int indexTable3[4] = {
    -1, -1, 2, 4,
};

static const int indexTable4[8] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
};

static const int indexTable5[16] = {
	-1, -1, -1, -1, -1, -1, -1, -1, 1, 2, 4, 6, 8, 10, 13, 16, 
};

static const int* indexTables[] = {
	indexTable2,
	indexTable3,
	indexTable4,
	indexTable5 
};

static const int stepsizeTable[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};


class XIADPCMDecomp : public XSoundDecomp
{
private:
	// Destination format - note we always decompress to 16 bit
	XBOOL stereo;
	int nBits;		// number of bits in each sample

	XS32 valpred[2];	// Current state
	int index[2];

	XS32 nSamples;	// number of samples decompressed so far

	// Parsing Info
	XU8 * src;
//	XS32 srcSize;
	XU32 bitBuf;	// this should always contain at least 24 bits of data
	int bitPos;

	void FillBuffer();
	void SkipBits(XS32 n);
	XS32 GetBits(int n) {
		if ( bitPos < n )
			FillBuffer();
		//FLASHASSERT(bitPos >= n);
		XS32 v = ((XU32)bitBuf << (32-bitPos)) >> (32-n);
		bitPos -= n;
		return v;
	}
	XS32 GetSBits(int n) {
		if ( bitPos < n )
			FillBuffer();
		//FLASHASSERT(bitPos >= n);
		XS32 v = ((XS32)bitBuf << (32-bitPos)) >> (32-n);
		bitPos -= n;
		return v;
	}

public:

	#ifdef _CELANIMATOR
	CArchive* archive;
	void Attach(CArchive* ar) { archive = ar; }
	#endif

    virtual void Setup(XSound* snd, XBOOL reset = false);
	void Decompress(XS16* dst, XS32 n);	// return number of good samples
};

void XIADPCMDecomp::Setup(XSound* snd, XBOOL reset)
{
//	FLASHASSERT(snd->CompressFormat() == sndCompressADPCM);

	stereo = snd->Stereo();
	src = (XU8 *)snd->samples;
/*
	{// Calculate the compressed size
		XS32 nblocks = snd->nSamples >> 12;
		XS32 k = nblocks * ((16 + 6) + nBits * 0xFFF);
		int nFrac = snd->nSamples & 0xFFF;
		if ( nFrac )
			k += (16 + 6) + nBits * nFrac;
		if ( stereo ) k *= 2;
		srcSize = (k+7)/8;
	}
*/
	nBits = 0;	// flag that it is not inited
	nSamples = 0;

	bitPos = 0; 
	bitBuf = 0; 

#ifdef _CELANIMATOR
	archive = 0;
#endif
}

void XIADPCMDecomp::FillBuffer()
{
#ifdef _CELANIMATOR
	if ( archive ) {
		while ( bitPos <= 24 ) {
			U8 s;
			*archive >> s;
			bitBuf = (bitBuf<<8) | s;
			bitPos += 8;
		}
	} else
#endif
	{
		while ( bitPos <= 24 /*&& srcSize > 0*/ ) {
			bitBuf = (bitBuf<<8) | *src++;
			bitPos += 8;
			//srcSize--;
		}
	}
}

void XIADPCMDecomp::SkipBits(XS32 n)
{
	if ( n <= 32 ) {
		// Skip a small chunk
		while ( n > 0 ) {
			int k = (int)XMIN(16,n);
			GetBits(k);
			n -= k;
		}
	} else {
		// Skip a larger chunk

		// Skip the current buffer
		n -= bitPos;
		bitPos = 0;

		// Skip whole bytes
		XS32 k = n/8;
		src += k;
		//srcSize -= k;

		// Skip the trailing fraction
		GetBits((int)n & 0x7);
	}
}

void XIADPCMDecomp::Decompress(XS16* dst, XS32 n)
{
	if ( nBits == 0 ) {
		// Get the compression header
		nBits = (int)GetBits(2)+2;
	}

	if ( !dst ) {
		// Skip samples
		XS32 nInBlock = nSamples & ~0xFFF;
		if ( nInBlock > 0 && n > nInBlock+nSamples ) {
			// Skip all of the rest of the current block
			nSamples += nInBlock;
			n -= nInBlock;
			XS32 k = nInBlock*nBits;
			if ( stereo ) k *= 2;
			SkipBits(k);
		}

		{// Skip whole blocks
			XS32 nblocks = n >> 12;
			// The first sample is 
			XS32 k = nblocks * ((16 + 6) + nBits * 0xFFF);
			if ( stereo ) k *= 2;
			SkipBits(k);
			n = n & 0xFFF;
		}

		{// Skip a portion of the current block
			XS16 buf[1024];
			XS32 limit = stereo ? 512 : 1024;
			while ( n > 0 ) {
				XS32 k = XMIN(limit, n);
				Decompress(buf, k);
				n -= k;
			}
		}
	} else {
		// Decompress the samples
		const int* indexTable = indexTables[nBits-2];
		int k0 = 1 << (nBits-2);
		int signmask = 1 << (nBits-1);
		if ( !stereo ) {
			// Optimize for mono
			XS32 vp = valpred[0];	// maybe these can get into registers...
			int ind = index[0];
			XS32 ns = nSamples;
			while ( n-- > 0 ) {
				ns++;
				if ( (ns & 0xFFF) == 1 ) {
					// Get a new block header
					*dst++ = (XS16)(vp = GetSBits(16));
					ind = (int)GetBits(6); // The first sample in a block does not have a delta
				} else {
					// Process a delta value
					int delta = (int)GetBits(nBits);

					// Compute difference and new predicted value 
					// Computes 'vpdiff = (delta+0.5)*step/4'
					int step = stepsizeTable[ind];
					XS32 vpdiff = 0;
					int k = k0;
					do {
						if ( delta & k )
							vpdiff += step;
						step >>= 1;
						k >>= 1;
					} while ( k );
					vpdiff += step;	// add 0.5

					if ( delta & signmask ) // the sign bit
						vp -= vpdiff;
					else
						vp += vpdiff;

					// Find new index value 
					ind += indexTable[delta&(~signmask)];
					if ( ind < 0 ) ind = 0;
					else if ( ind > 88 ) ind = 88;

					// clamp output value 
					if ( vp != (XS16)vp )
						vp = vp < 0 ? -32768 : 32767;
//					FLASHASSERT(vp <=32767 && vp >= -32768);

					/* Step 7 - Output value */
					*dst++ = (XS16)vp;
				}
			}
			valpred[0] = vp;
			index[0] = ind;
			nSamples = ns;
		} else {
			int sn = stereo ? 2 : 1;
			// Stereo
			while ( n-- > 0 ) {
				nSamples++;
				if ( (nSamples & 0xFFF) == 1 ) {
					// Get a new block header
					for ( int i = 0; i < sn; i++ ) {
						*dst++ = (XS16)(valpred[i] = GetSBits(16));
						index[i] = (int)GetBits(6); // The first sample in a block does not have a delta
					}
				} else {
					// Process a delta value
					for ( int i = 0; i < sn; i++ ) {
						int delta = (int)GetBits(nBits);

						// Compute difference and new predicted value 
						// Computes 'vpdiff = (delta+0.5)*step/4'
						int step = stepsizeTable[index[i]];
						XS32 vpdiff = 0;
						int k = k0;
						do {
							if ( delta & k )
								vpdiff += step;
							step >>= 1;
							k >>= 1;
						} while ( k );
						vpdiff += step;	// add 0.5

						if ( delta & signmask ) // the sign bit
							valpred[i] -= vpdiff;
						else
							valpred[i] += vpdiff;

						// Find new index value 
						index[i] += indexTable[delta&(~signmask)];
						if ( index[i] < 0 ) index[i] = 0;
						else if ( index[i] > 88 ) index[i] = 88;

						// clamp output value 
						if ( valpred[i] != (XS16)valpred[i] )
							valpred[i] = valpred[i] < 0 ? -32768 : 32767;
//						FLASHASSERT(valpred[i] <=32767 && valpred[i] >= -32768);

						/* Step 7 - Output value */
						*dst++ = (XS16)valpred[i];
					}
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////

class XIPCMDecomp : public XSoundDecomp {
public:
	XS32     pos;
	XS32     bytesPerBlock;
	void*   samples;
    XBOOL    swapBytes;

public:
    virtual void Setup(XSound* snd, XBOOL reset = false);
    void Decompress(XS16* dst, XS32 n);	// if dst == 0, just skip the requested samples
};

class XISETDecomp : public XIPCMDecomp {
public:
	XS32     pos;
	XS32     bytesPerBlock;
	void*   samples;
    XBOOL    swapBytes;

public:
	virtual void Setup(XSound* snd, XBOOL reset = false)
	{
		samples = snd->samples;
		bytesPerBlock = 1;
		pos = 0;
		swapBytes = false;
	}
    void Decompress(XS16* dst, XS32 n)	// if dst == 0, just skip the requested samples
	{
		if(dst)
			memset(dst,0,n*bytesPerBlock);
		pos+=n;
	}
};

void XIPCMDecomp::Decompress(XS16* dst, XS32 n)
{
	if ( dst ) {
// 		hmemcpy(dst, (U8 *)samples + pos*bytesPerBlock, n*bytesPerBlock);
		memcpy(dst, (XU8 *)samples + pos*bytesPerBlock, n*bytesPerBlock);

      #ifdef _MAC
        if ( swapBytes )
            SwapBytes(dst, n*bytesPerBlock / 2);
	  #endif
	}

	pos += n;
}

void XIPCMDecomp::Setup(XSound* snd, XBOOL reset)
{
	samples = snd->samples;
	bytesPerBlock = snd->BytesPerBlock();
	pos = 0;
    #ifdef _MAC
    swapBytes = (snd->CompressFormat() == sndCompressNoneI) && !snd->Is8Bit();
	#else
	swapBytes = false;
	#endif
}
/////////////////////////////////////////////////////////////////////////////////
#include "XMp3Decomp.h"

const XS32 kRateTable[4] = {sndRate5K, sndRate11K, sndRate22K, sndRate44K};
const int kRateShiftTable[4] = {3, 2, 1, 0};

// XXSoundMix.cpp: implementation of the XXSoundMix class.
void Cvt8RateDiv2(XU8* samples, XS32 n, XBOOL stereo)
// n is the original number of samples
// Need to filter out high frequencies
{
	XU8* src = samples;
	XU8* dst = samples;
	if ( stereo ) {
		while ( n > 0 ) {
			dst[0] = (XU8)(((int)src[0]+src[2])/2);
			dst[1] = (XU8)(((int)src[1]+src[3])/2);
			src+=4;
			dst+=2;
			n-=2;
		}
	} else {
		while ( n > 0 ) {
			dst[0] = src[0];
			src+=2;
			dst++;
			n-=2;
		}
	}
}

void Cvt8RateMul2(XU8* samples, XS32 n, XBOOL stereo, XU8* prevSample)
// n is the original number of samples
{
//	FLASHASSERT(n>0);
	if ( stereo ) {
		XU8* src = samples+n*2;
		XU8* dst = samples+n*4;

		// Interpolate samples
		while ( true ) {
			dst-=4;
			src-=2;
			n--;
			dst[2] = src[0];
			dst[3] = src[1];
			if ( n == 0 ) {
				// For the first sample, interpolate with the saved previous sample
				dst[0] = (XU8)(((int)prevSample[0]+src[0])/2);
				dst[1] = (XU8)(((int)prevSample[1]+src[1])/2);
				break;
			}
			dst[0] = (XU8)(((int)src[-2]+src[0])/2);
			dst[1] = (XU8)(((int)src[-1]+src[1])/2);
		}
	} else {
		XU8* src = samples+n;
		XU8* dst = samples+n*2;

		// Interpolate samples
		while ( true ) {
			dst-=2;
			src--;
			n--;
			dst[1] = src[0];
			if ( n == 0 ) {
				// For the first sample, interpolate with the saved previous sample
				dst[0] = (XU8)(((int)prevSample[0]+src[0])/2);
				break;
			}
			dst[0] = (XU8)(((int)src[-1]+src[0])/2);
		}
	}
}

void Cvt16RateDiv2(XS16* samples, XS32 n, XBOOL stereo)
// n is the original number of samples
// Need to filter out high frequencies
{
	XS16* src = samples;
	XS16* dst = samples;
	if ( stereo ) {
		while ( n > 0 ) {
			dst[0] = (XS16)(((XS32)src[0]+src[2])/2);
			dst[1] = (XS16)(((XS32)src[1]+src[3])/2);
			src+=4;
			dst+=2;
			n-=2;
		}
	} else {
		while ( n > 0 ) {
			dst[0] = src[0];
			src+=2;
			dst++;
			n-=2;
		}
	}
}

void Cvt16RateMul2(XS16* samples, XS32 n, XBOOL stereo, XS16* prevSample)
// n is the original number of samples
{
	if ( stereo ) {
		XS16* src = samples+n*2;
		XS16* dst = samples+n*4;

		// Interpolate samples
		while ( true ) {
			dst-=4;
			src-=2;
			n--;
			dst[2] = src[0];
			dst[3] = src[1];
			if ( n == 0 ) {
				// For the first sample, interpolate with the saved previous sample
				dst[0] = (XS16)(((XS32)prevSample[0]+src[0])/2);
				dst[1] = (XS16)(((XS32)prevSample[1]+src[1])/2);
				break;
			}
			dst[0] = (XS16)(((XS32)src[-2]+src[0])/2);
			dst[1] = (XS16)(((XS32)src[-1]+src[1])/2);
		}
	} else {
		XS16* src = samples+n;
		XS16* dst = samples+n*2;

		// Interpolate samples
		while ( true ) {
			dst-=2;
			src--;
			n--;
			dst[1] = src[0];
			if ( n == 0 ) {
				// For the first sample, interpolate with the saved previous sample
				dst[0] = (XS16)(((XS32)prevSample[0]+src[0])/2);
				break;
			}
			dst[0] = (XS16)(((XS32)src[-1]+src[0])/2);
		}
	}
}

inline void Cvt8to16(XU8* samples, XS32 n, XBOOL stereo)
{
	if ( stereo ) n*=2;
	XU8* src = samples+n;
	XS16* dst = (XS16*)samples+n;
	while ( n > 0 ) {
		src--;
		dst--;
		n--;
		*dst = ((XS16)*src - 128)<<8;
	}
}

inline void Cvt16to8(XS16* samples, XS32 n, XBOOL stereo)
{
	if ( stereo ) n*=2;
	XS16* src = samples;
	XU8* dst = (XU8*)samples;
	while ( n > 0 ) {
		*dst = (XU8)((*src>>8)+128);
		src++;
		dst++;
		n--;
	}
}

inline void Cvt8StereoToMono(XU8* samples, XS32 n)
{
	XU8* src = samples;
	XU8* dst = samples;
	while ( n > 0 ) {
		*dst = (XU8)(((int)src[0] + (int)src[1])/2);
		src+=2;
		dst++;
		n--;
	}
}

inline void Cvt8MonoToStereo(XU8* samples, XS32 n)
{
	XU8* src = samples+n;
	XU8* dst = samples+2*n;
	while ( n > 0 ) {
		n--;
		src--;
		dst-=2;
		dst[0] = dst[1] = *src;
	}
}

inline void Cvt16StereoToMono(XS16* samples, XS32 n)
// n is the number of samples
{
	XS16* src = samples;
	XS16* dst = samples;
	while ( n > 0 ) {
		*dst = (XS16)(((XS32)src[0] + (XS32)src[1])/2);
		src+=2;
		dst++;
		n--;
	}
}

inline void Cvt16MonoToStereo(XS16* samples, XS32 n)
{
	XS16* src = samples+n;
	XS16* dst = samples+2*n;
	while ( n > 0 ) {
		n--;
		src--;
		dst-=2;
		dst[0] = dst[1] = *src;
	}
}

void ConvertSound(void* samples, XS32 nSamples44, int srcFormat, int dstFormat, void* prevSample)
// The memory pointed to by src->samples must be large enough to
//	hold the largest amount of data required by the conversion.
// The result will be placed in this memory
// returns the new number of samples - this is different only w/ a rate change...
{
	if ( srcFormat == dstFormat || nSamples44 == 0 ) return;
	XSound src, dst;
	src.format = srcFormat;
	dst.format = dstFormat;

	// We should always convert a multiple of 8 samples
	//FLASHASSERT((nSamples44 & 7) == 0);

	// We don't do compression
	//FLASHASSERT(!dst.Compressed()&&!src.Compressed());	

	XS32 nSamples = nSamples44>>src.RateShift();

	{// Handle a rate change
		int srcR = src.RateShift();
		int dstR = dst.RateShift();
		// Decrease the src rate
		while ( srcR < dstR ) {
			if ( src.Is8Bit() )
				Cvt8RateDiv2((XU8*)samples, nSamples, src.Stereo());
			else 
				Cvt16RateDiv2((XS16*)samples, nSamples, src.Stereo());
			nSamples /= 2;
			srcR++;
		}
		// Increase the src rate
		while ( srcR > dstR ) {
			if ( src.Is8Bit() ) 
				Cvt8RateMul2((XU8*)samples, nSamples, src.Stereo(), (XU8*)prevSample);
			else
				Cvt16RateMul2((XS16*)samples, nSamples, src.Stereo(), (XS16*)prevSample);
			nSamples *= 2;
			srcR--;
		}
	}

	// Sample Size change
	if ( !dst.Is8Bit() && src.Is8Bit() ) {
		// Map 8->16
		Cvt8to16((XU8*)samples, nSamples, src.Stereo());

	} else if ( dst.Is8Bit() && !src.Is8Bit() ) {
		// Map 16->8
		Cvt16to8((XS16*)samples, nSamples, src.Stereo());
	}

	// Stereo/Mono conversion
	if ( !dst.Stereo() && src.Stereo() ) {
		// Stereo -> Mono conversion
		if ( dst.Is8Bit() ) 
			Cvt8StereoToMono((XU8*)samples, nSamples);
		else
			Cvt16StereoToMono((XS16*)samples, nSamples);

	} else if ( dst.Stereo() && !src.Stereo() ) {
		// Mono -> Stereo conversion
		if ( dst.Is8Bit() ) 
			Cvt8MonoToStereo((XU8*)samples, nSamples);
		else
			Cvt16MonoToStereo((XS16*)samples, nSamples);
	}
}


void ApplyRamp8(XU8* s, XS32 n, int stride, XS32 level, XS32 dLevel)
{
	if ( dLevel == 0 ) {
		// A constant level
		if ( level == 0 ) {
			// Silence
			while ( n-- ) {
				*s = 128;
				s+=stride;
			}

		} else if ( level == sndMaxLevel ) {
			// Nothing to do...

		} else {
			// A constant level
			level = level >> 15;
			while ( n-- ) {
				*s = (XU8)((( ((int)*s-128) * level) >> 15) + 128);
				s+=stride;
			}
		}
	} else {
		// A ramped level
		while ( n-- ) {
			*s = (XU8)((( ((int)*s-128) * (level>>15)) >> 15) + 128);
			level += dLevel;
			s+=stride;
		}
	}
}

void ApplyRamp16(XS16* s, XS32 n, int stride, XS32 level, XS32 dLevel)
{
	if ( dLevel == 0 ) {
		// A constant level
		if ( level == 0 ) {
			// Silence
			while ( n-- ) {
				*s = 0;
				s+=stride;
			}
		} else if ( level == sndMaxLevel ) {
			// Nothing to do...

		} else {
			// A constant level
			level = level >> 15;
			while ( n-- ) {
				*s = (XS16)((*s * level)>>15);
				s+=stride;
			}
		}
	} else {
		// A ramped level
		while ( n-- ) {
			*s = (XS16)((*s * (level>>15))>>15);
			level += dLevel;
			s+=stride;
		}
	}
}

void ApplyEnvelope(int format, void* samples, XS32 nSamples44, XS32 time44, _XSoundEnvelope* envelope)
// The time should be relative to the start of the envelope
{
	if ( envelope->nPoints == 0 ) 
		return;	// the envelope is empty

	XSound snd;
	snd.format = format;
	int rateShift = snd.RateShift();
	XS32 n = nSamples44 >> rateShift;

	int bytesPerBlock = snd.BytesPerBlock();

	int ie = 0;
	while ( n > 0 ) {
		// Find the proper segment of the envelope
		while ( ie < envelope->nPoints ) {
			if ( envelope->points[ie].mark44 > time44 ) break;
			ie++;
		}

		// Set up a new envelope segment
		// Note: The level difference equation is 2.30 fixed point numbers
		XS32 nSeg = n;
		XS32 dLevel0, dLevel1, level0, level1;
		if ( ie == 0 ) {
			// Before the start-should never happen...
			dLevel0 = dLevel1 = 0;
			level0 = (XS32)envelope->points[0].level0 << 15;
			level1 = (XS32)envelope->points[0].level1 << 15;
		} else if ( ie == envelope->nPoints ) {
			// After the end
			dLevel0 = dLevel1 = 0;
			level0 = (XS32)envelope->points[ie-1].level0 << 15;
			level1 = (XS32)envelope->points[ie-1].level1 << 15;
		} else {
			// In a ramp
			_XSoundEnvelopePoint* pt = envelope->points+ie-1;
			//FLASHASSERT(pt[0].mark44 <= time44 && pt[1].mark44 > time44);

			XS32 time = time44 >> rateShift;
			XS32 mark0 = pt[0].mark44 >> rateShift;
			XS32 dmark = (pt[1].mark44 - pt[0].mark44) >> rateShift;

			dLevel0 = ((XS32)(pt[1].level0 - pt[0].level0)<<15) / dmark;
			level0  = ((XS32)pt[0].level0<<15) + dLevel0*(time-mark0);

			dLevel1 = ((XS32)(pt[1].level1 - pt[0].level1)<<15) / dmark;
			level1  = ((XS32)pt[0].level1<<15) + dLevel1*(time-mark0);

			nSeg = XMIN(n, (pt[1].mark44-time44) >> rateShift);
		}

		// Apply this segment of the envelope
		n -= nSeg;
		if ( snd.Stereo() ) {
			if ( snd.Is8Bit() ) {
				ApplyRamp8((XU8*)samples, nSeg, 2, level0, dLevel0);
				ApplyRamp8((XU8*)samples+1, nSeg, 2, level1, dLevel1);
			} else {
				ApplyRamp16((XS16*)samples, nSeg, 2, level0, dLevel0);
				ApplyRamp16((XS16*)samples+1, nSeg, 2, level1, dLevel1);
			}
		} else {
			// Merge the two envelopes for a mono envelope
			level0 = (level0 + level1)/2;
			dLevel0 = (dLevel0 + dLevel1)/2;

			if ( snd.Is8Bit() ) {
				ApplyRamp8((XU8*)samples, nSeg, 1, level0, dLevel0);
			} else {
				ApplyRamp16((XS16*)samples, nSeg, 1, level0, dLevel0);
			}
		}
		samples = (char*)samples + bytesPerBlock*nSeg;
	}
}

void ApplyTransform16(XU16*s,XS32 n,int stride,XSoundTransform*p)
{	
	//return;
	int ll=(p->m_dLeftToLeft*p->m_dVolume)>>8;
	int rl=(p->m_dRightToLeft*p->m_dVolume)>>8;
	int rr=(p->m_dRightToRight*p->m_dVolume)>>8;
	int lr=(p->m_dLeftToRight*p->m_dVolume)>>8;

	if(p->m_dPan<0)
	{
		rr=rr+(rl*(256-p->m_dPan)>>8);
		lr=lr+(ll*(256-p->m_dPan)>>8);
		ll=(ll*(256+p->m_dPan))>>8;
		rl=(ll*(256+p->m_dPan))>>8;
	}
	else
	{
		ll=ll+(lr*(256+p->m_dPan)>>8);
		rl=rl+(rr*(256+p->m_dPan)>>8);
		rr=rr*p->m_dPan>>8;
		lr=lr*p->m_dPan>>8;
	}
	/*int d=ll+rl;
	if(d>256)
	{
		ll=ll*256/d;
		rl=rl*256/d;
	}
	d=lr+rr;
	if(d>256)
	{
		lr=lr*256/d;
		rr=rr*256/d;
	}*/
	while ( n-- ) 
	{
//		if(*s)
//			int v=0;
		int left=((*s)*ll+(*(s+1))*rl)>>9;
		*(s+1)=(XU16)(((*s)*lr+(*(s+1))*rr)>>9);
		*s=(XU16)left;
		s+=stride;
	}
}
void ApplyTransform8(XU8*s,XU32 n,int stride,XSoundTransform*p)
{	
	int ll=(p->m_dLeftToLeft*p->m_dVolume)>>8;
	int rl=(p->m_dRightToLeft*p->m_dVolume)>>8;
	int rr=(p->m_dRightToRight*p->m_dVolume)>>8;
	int lr=(p->m_dLeftToRight*p->m_dVolume)>>8;

	if(p->m_dPan<0)
	{
		rr=rr+(rl*(256-p->m_dPan)>>8);
		lr=lr+(ll*(256-p->m_dPan)>>8);
		ll=(ll*(256+p->m_dPan))>>8;
		rl=(ll*(256+p->m_dPan))>>8;
	}
	else
	{
		ll=ll+(lr*(256+p->m_dPan)>>8);
		rl=rl+(rr*(256+p->m_dPan)>>8);
		rr=rr*p->m_dPan>>8;
		lr=lr*p->m_dPan>>8;
	}
	while ( n-- ) 
	{
		int left=(*s)*ll+(*(s+1))*rl;
		*(s+1)=(XU8)((*s)*lr+(*(s+1))*rr);
		*s=(XU8)left;
		s+=stride;
	}
}


void SetChannelVolume(int format, void* samples, XS32 nSamples44,int v)
// The time should be relative to the start of the envelope
{
	/*if(!v)
	{
		memset(samples,0,nSamples44*2);
		return;
	}*/
	XSound snd;
	snd.format = format;
	int rateShift = snd.RateShift();
	XS32 n = nSamples44 >> rateShift;

	int bytesPerBlock = snd.BytesPerBlock();

	int ie = 0;
	XS32 dLevel=0,level=sndMaxLevel*((v&0xff)*sndMaxLevel/100);//,rlevel=sndMaxLevel*((v>>8)*sndMaxLevel/100);//(sndMaxLevel*v/100);
	while ( n > 0 ) {
		// Find the proper segment of the envelope
		// Set up a new envelope segment
		// Note: The level difference equation is 2.30 fixed point numbers
		XS32 nSeg = n;
		
		

		// Apply this segment of the envelope
		n -= nSeg;
		if ( snd.Stereo() ) {			
			if ( snd.Is8Bit() ) 
			{
				if(!v) memset(samples,0,nSeg*2);
				else
				{
					ApplyRamp8((XU8*)samples, nSeg, 2, level, dLevel);
					ApplyRamp8((XU8*)samples+1, nSeg, 2, level, dLevel);
				}
			} else {
				{
					if(!v) memset(samples,0,nSeg*2*2);
					else
					{
						ApplyRamp16((XS16*)samples, nSeg, 2, level, dLevel);
						ApplyRamp16((XS16*)samples+1, nSeg, 2, level, dLevel);
					}
				}
			}
		} else {
			// Merge the two envelopes for a mono envelope

			if ( snd.Is8Bit() ) {
				if(!v)
					memset(samples,0,nSeg);
				else
					ApplyRamp8((XU8*)samples, nSeg, 1, level, dLevel);
			} else {
				if(!v)
					memset(samples,0,nSeg*2);
				else
					ApplyRamp16((XS16*)samples, nSeg, 1, level, dLevel);
			}
		}
		samples = (char*)samples + bytesPerBlock*nSeg;
	}
}

void ApplyTransform(int format,void*samples,XS32 nSamples44,XSoundTransform*t)
{	
	{//Just volume effect
		if(t->m_dVolume>=256) return;
		int v=t->m_dVolume*127/256;
		SetChannelVolume(format,samples,nSamples44,v);
		return;
	}
	XSound snd;
	snd.format = format;
	int rateShift = snd.RateShift();
	XS32 n = nSamples44 >> rateShift;

	int bytesPerBlock = snd.BytesPerBlock();

	int ie = 0;
	XS32 dLevel=0,level=sndMaxLevel*(t->m_dVolume*sndMaxLevel>>8);
		//((v&0xff)*sndMaxLevel/100),rleft=sndMaxLevel*((v>>8)*sndMaxLevel/100);//(sndMaxLevel*v/100);
	while ( n > 0 ) {
		// Find the proper segment of the envelope
		// Set up a new envelope segment
		// Note: The level difference equation is 2.30 fixed point numbers
		XS32 nSeg = n;		

		// Apply this segment of the envelope
		n -= nSeg;
		if ( snd.Stereo() ) {
			if ( snd.Is8Bit() ) {
				//ApplyRamp8((XU8*)samples, nSeg, 2, level, dLevel);
				//ApplyRamp8((XU8*)samples+1, nSeg, 2, level, dLevel);
				ApplyTransform8((XU8*)samples,nSeg,2,t);
			} else {
				//ApplyRamp16((XS16*)samples, nSeg, 2, level, dLevel);
				//ApplyRamp16((XS16*)samples+1, nSeg, 2, level, dLevel);
				ApplyTransform16((XU16*)samples,nSeg,2,t);
			}
		} else {
			// Merge the two envelopes for a mono envelope

			if ( snd.Is8Bit() ) {
				ApplyRamp8((XU8*)samples, nSeg, 1, level, dLevel);
			} else {
				ApplyRamp16((XS16*)samples, nSeg, 1, level, dLevel);
			}
		}
		samples = (char*)samples + bytesPerBlock*nSeg;
	}
}


/////////////////////////////////////////////////////////////////////////
XSoundChannel::XSoundChannel()
{
	Init();
}

void XSoundChannel::Init()
{
	completeProc=XNULL;
	pArgs=NULL;
	streamEnd=false;
	m_pTransform=XNULL;
	mixer = 0;
	next = 0;
	refCount = 0;
	remove = false;
	tag = 0;

	sound = 0;
	envelope.nPoints = 0;

	syncFlags = 0;
	loops = 0;
	inPoint44 = 0;
	outPoint44 = 0x7FFFFFFFL;

	streamProc = 0;
	refPtr = 0;

	samplesPlayed = 0;  //!!@xx look at all the instances of this var

	blockSamples = 0;
    seekSamples = 0;

    blockCanBeZero = false;
	fadeIn = true;
	silentBlock = false;
	streamStalled = false;
    decomp = 0;
	nVolume=-1;
	completeStop=false;
}

XSoundChannel::~XSoundChannel()
{
	if ( mixer ) {
		AddRef(); // prevent a recursive delete...
		mixer->RemoveSound(this);
		Release();
	}
	if ( decomp )
		delete decomp;
}

XBOOL XSoundChannel::CreateDecompressor()
{
	if(decomp) return XTRUE;
	switch(sound->CompressFormat())
	{
		default:
			 decomp=new XISETDecomp();break;
			 break;
		case sndCompressNone:
        case sndCompressNoneI:
			 decomp=new XIPCMDecomp();break;
		case sndCompressADPCM:
			 decomp=new XIADPCMDecomp();
			//return new XIADPCMDecomp();
			 break;
		//#ifdef FLASHMP3
		case sndCompressMP3:
			decomp=new XMp3Decomp();
			break;
			//return new CMp3Decomp();
		//#endif
	}
	//return XFALSE;
	return XTRUE;
}

void XSoundChannel::Stop() 
{
	if ( mixer ) 
		mixer->RemoveSound(this); 
}

XS32 XSoundChannel::ElapsedFrames()
{
	if ( !mixer || !sound->nSamples ) {
		return 0;
	}

	// Get the two values we want as quickly as possible so it is unlikely that we get interupted here...
	// We might need a critical section here...
	int sp = samplesPlayed;
	XS32 ct = bufferCompletionTime;

	// get an adjustment for the elapsed time into the current buffer
// 	XS32 adj = mixer->Time() - ct;	
	XS32 adj = XTime::GetTimeMSec()-ct;
		//GetTimeMSec() - ct;	
	if ( adj > 63 )
		adj = 63;
	adj *= 44;	// convert from msec to 44khz units
	//if ( adj*44 > mixer->SOUND_SAMPLES44 ) {
	//	adj = mixer->SOUND_SAMPLES44/44;
	//}

	if ( streamStalled ) adj = 0;

	// Calculate the elapsed frames
	return (sp + (adj>>sound->RateShift()) + sound->nSamples/2)/sound->nSamples;
}

///////////////////////////////////////////////////////////////////////////
void XSound::Init()
{
	format = 0;
	nSamples = 0;
	samples = 0;
	dataLen = 0;
    delay = 0;
}
//////////////////////////////////////////////////////////////////////////////
#ifdef _SYMBIAN

void XSoundMix::MaoscOpenComplete(TInt aError)
	{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::MaoscOpenComplete(TInt aError)\r\n");
#endif
		if(aError==KErrNone)
		{
			iPlayStatus=EEngineReady;
			isOpen=XTRUE;
			iWait->AsyncStop();
			createdEvent.SetEvent();
		}
		else
			CloseDevice();
	}
void XSoundMix::SetPause(XBOOL b)
	{
#ifdef _LostSound
	return;
#endif
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::SetPause(XBOOL b)\r\n");
#endif
		//
		bPause=b;
		if(!b)
			playEvent.SetEvent();
//		if(b)
//			iPlayStatus = EEngineToPause;
//		else
//		{
//			if(!IsDeviceOpen())
//				OpenDevice();
//			else
//				{
//					iPlayStatus=EEnginePlaying;
//					playEvent.SetEvent();
//					//DoPlay();
//				}
//			/*if(iPlayStatus == EEngineReady || iPlayStatus == EEnginePause)
//	        {
//				iMp3Status = EEnginePlaying;
//				if(!IsDeviceOpen())
//					OpenDevice();
//				else
//					NextBuffer();
//	        }*/
//		}
	}
void XSoundMix::MaoscBufferCopied(TInt aError, const TDesC8& /*aBuffer*/)
	{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::MaoscBufferCopied(TInt aError, const TDesC8& /*aBuffer*/)\r\n");
#endif
	iWait->AsyncStop();
	if(aError==KErrNone || aError==KErrAbort)
	 {
		switch(iPlayStatus)
			{
			case EEngineToPause:
				{
				   iPlayStatus=EEnginePause;
				   iStream->Stop();	   
				   return;
				}
				break;
			case EEnginePause:
				{
				return;
				}
				break;
			case EEnginePlaying:
			case EEngineReady:
				{
				switch(iVolumeAdjust)
					{
					case EVolumeUp:
					case EVolumeDown:
						{
						iStream->SetVolume(nVolume);
						iVolumeAdjust = EVolumeNone;
						}
						break;
					}
				   if(m_pOutput)
				   {						
					MarkBufferComplete( m_pOutput->dwUser );
					playEvent.SetEvent();
				   }
				   return;
				}
			}
/*
	  if(iPlayStatus==EEngineToPause)
	  {
	   iPlayStatus=EEnginePause;
	   iStream->Stop();	   
	   return;
	  }

	  if(iPlayStatus==EEnginePause)
	  {	   
	   return;
	  } 
	  
	   if(iPlayStatus==EEnginePlaying||iPlayStatus==EEngineReady)
	   {
		   if(m_pOutput)
		   {
			//XLock lock(&m_playLock);
			//if(!lock.Lock(XSHORTTIME))
			//	return;
			
			//soundMix->MarkBufferComplete( findIndex );
									
			MarkBufferComplete( m_pOutput->dwUser );
			playEvent.SetEvent();
			
			//lock.UnLock();
			
			   //m_pOutput->dwFlags=WHDR_DONE;
			   //MarkBufferComplete( m_pOutput->dwUser );
		   }
	   }
	   return;
	   */
	 }
	 else
	 {
		 CloseDevice();
	 }
	}
	

void XSoundMix::MaoscPlayComplete(TInt aError)
	{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::MaoscPlayComplete(TInt aError)\r\n");
#endif
//	if (aError==KErrNone || aError==KErrCancel)
//		{
//			CloseDevice();
//		}
//	else
//		iPlayStatus=EEnginePause;
//		//playEvent.SetEvent();
	}

XUINT XSoundMix::CallBack(XU32 nID)
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"XUINT XSoundMix::CallBack(XU32 nID)\r\n");
#endif
	if(NeedStop()) return nID;
	_Open();
//	createdEvent.SetEvent();
	DoPlay();
	_Close();
	return nID;
}

#elif defined(_WIN32)



void CALLBACK MixCallback(HWAVEOUT hwo,	UINT uMsg, XU32 dwInstance, XU32 dwParam1, XU32 dwParam2)
{
	if ( uMsg == WOM_DONE ) {
		// The buffer is done playing
		XSoundMix* mixer = (XSoundMix*)dwInstance;

		// Track the number of sound buffers that have completed playing for this channel...
		int i = (int)(((WAVEHDR*)dwParam1)->dwUser);
	
		mixer->MarkBufferComplete( i );

		mixer->SetEvent();//Stop();
		//SetEvent(  mixer->GetPlayEvent() );	// signal the thread to quit
	}
}
#endif

void XSoundMix::NativeConstruct()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::NativeConstruct()----2\r\n");
#endif
	isOpen	 = false;
	memset( &waveHDR, 0, sizeof(_XWAVEHDR ) * MAX_SOUND_BUFFERS );
#ifdef _SYMBIAN
	iStream=XNULL;
	m_pOutput=XNULL;
	iPlayStatus=EEngineNotReady;
	iWait = XNULL;//new(ELeave)CActiveSchedulerWait;
	playThread=XNULL;
	m_pTransformn=XNULL;
	playEvent.Create();
#elif defined(_WIN32)
	playEvent.Create();
    hWaveOut = 0;
	
	//InitializeCriticalSection((PRTL_CRITICAL_SECTION)&playLock);

	
	//playThread.Create(this,1);
	//playThread = 0;
	//playEvent.Create();
	//playEvent = CreateEvent(0, false, false, 0);

	// Set the timer interval
	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(tc));
	period = max(1, (int)tc.wPeriodMin);	// get the best possible resolution...
	timeBeginPeriod(period);
#endif
}

void XSoundMix::NativeDestruct()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::NativeDestruct()\r\n");
#endif
#ifdef _SYMBIAN
	if(iStream) delete iStream;
	iStream=XNULL;
#ifdef _DefaultThread
	if(playThread)
		{
		playThread->Stop();
		playThread->Release();
		delete playThread;
		playThread = NULL;
		}
#else
	if(playThread)
		{
		XThreadPool *pool = XThreadPool::GetInstance();
		pool->FreeThread(playThread);
		}
#endif
//	playThread.Stop();
//	playThread.Release();
	playEvent.Release();
	XSoundChannel*c=this->firstChannel;
	while(c)
	{
		XSoundChannel*next=c->next;
		if(c->completeStop)
			delete c;
		c=next;
	}
#elif defined(_WIN32)
	timeEndPeriod(period);
	playThread.Stop();
	playEvent.Release();
#endif
 	//CloseHandle(playEvent);
}

void XSoundMix::GetNativeSoundCap( XS32* format )
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::GetNativeSoundCap( XS32* format )\r\n");
#endif
	// Check to capabilities of the waveform device
	*format = sndStereo | snd16Bit | snd22K;
#ifdef _SYMBIAN
#elif defined(_WIN32)
	WAVEOUTCAPS caps;
	waveOutGetDevCaps(WAVE_MAPPER, &caps, sizeof(caps));

	// the desired format
	

	// Check for sample rate support
	if ( (caps.dwFormats & WAVE_FORMAT_4M16) == 0 ) {
		*format = (*format & ~sndRateMask) | XMIN(*format & sndRateMask, snd22K);
	}

	if ( (caps.dwFormats & WAVE_FORMAT_2M16) == 0 ) {
		*format = (*format & ~sndRateMask) | XMIN(*format & sndRateMask, snd11K);
	}

	// Check to turn off stereo support
	if ( (caps.dwFormats & WAVE_FORMAT_2S16) == 0 )
		*format &= ~sndStereo;

	// Check to turn off 16 bit sound
	if ( (caps.dwFormats & WAVE_FORMAT_2M16) == 0 )
		*format &= ~snd16Bit;
#endif
}

XBOOL XSoundMix::OpenNativeDevice( XSound* format )
{
#ifdef _LostSound
	return false;
#endif
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"XBOOL XSoundMix::OpenNativeDevice( XSound* format )\r\n");
#endif
	
	if ( isOpen )
	{
		return true;
	}
	// Open a waveform device for output
#ifdef _SYMBIAN
	//�����߳�
	//playThread.Create(this,100);
	
	iStream=CMdaAudioOutputStream::NewL(*this);
	if(!iStream) return XFALSE;
	iWait = new CActiveSchedulerWait;
	if(!iWait){delete iStream;return XFALSE;}
	
	TMdaAudioDataSettings iSetting;
	iStream->Open(&iSetting);	// start
	
	iWait->Start();
	
	if(!isOpen)
		{
			delete iStream;
			if(iWait->IsStarted())
				iWait->AsyncStop();
			delete iWait;
			return XFALSE;
		}
	m_nMaxVolume = iStream->MaxVolume();
	
	// PC模拟器不支持MP3格式，所以使用PCM格式
#ifdef __WINS__		// PC�?
	if(format->Is8Bit())
		iStream->SetDataTypeL(KMMFFourCCCodePCM8);
	else
		iMixer->SetDataTypeL(KMMFFourCCCodePCM16);
#else				// 手机�?
	if(format->Compressed())
		iMixer->SetDataTypeL(KMMFFourCCCodeMP3);
	else if(format->Is8Bit())
			iMixer->SetDataTypeL(KMMFFourCCCodePCM8);
	else
		iMixer->SetDataTypeL(KMMFFourCCCodePCM16);
#endif

	iMixer->SetAudioPropertiesL(format->SBRate(), format->NSBChannels());
//	iMixer->SetAudioPropertiesL(TMdaAudioDataSettings::ESampleRate22050Hz,TMdaAudioDataSettings::EChannelsStereo);
	iMixer->SetPriority(EPriorityNormal,EMdaPriorityPreferenceNone);
//	SetVolume(nVolume);
	//isOpen=XTRUE;
	return XTRUE;
#elif defined(_WIN32)
	WAVEFORMATEX pcmWaveFormat;

  	pcmWaveFormat.wFormatTag		= WAVE_FORMAT_PCM;
 	pcmWaveFormat.nSamplesPerSec	= format->Rate();
 	pcmWaveFormat.nChannels			= format->NChannels();
 	pcmWaveFormat.wBitsPerSample	= format->BitsPerSample();
 	pcmWaveFormat.nBlockAlign		= ( format->BitsPerSample() * format->NChannels() ) / 8;
 	pcmWaveFormat.nAvgBytesPerSec	= pcmWaveFormat.nBlockAlign * pcmWaveFormat.nSamplesPerSec;

	if ( waveOutOpen(	(HWAVEOUT*)&hWaveOut, 
							WAVE_MAPPER, 
							&pcmWaveFormat, 	
							(XU32)MixCallback, 
							(XU32)this, 
							CALLBACK_FUNCTION
				    ) == MMSYSERR_NOERROR )
	{
		isOpen = true;
		
		//::waveOutSetVolume((HWAVEOUT)hWaveOut,nVolume);
		SetVolume(nVolume);
		return true;
	}
	isOpen = false;
	return false;
#endif
}

void XSoundMix::CloseNativeDevice()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::CloseNativeDevice()\r\n");
#endif
#ifdef _SYMBIAN
	if(playThread == XNULL) return;
	closeEvent.Create();
	closedEvent.Create();
	closeEvent.SetEvent();
	closedEvent.ResetEvent();
	playEvent.SetEvent();
	bPause = false;
	while(!closedEvent.IsEvent(1000));
	closeEvent.Release();
	closedEvent.Release();
#ifdef _DefaultThread
	playThread->Stop();
	playThread->Release();
	delete playThread;
	playThread = NULL;
#else
	XThreadPool *pool = XThreadPool::GetInstance();
	pool->FreeThread(playThread);
#endif
	isOpen = false;
//	playThread.Stop();
#elif defined(_WIN32)
	if ( !isOpen ) return;
	playThread.Stop(XFALSE);
#endif
	//playEvent.SetEvent();
	//SetEvent(playEvent);	// signal the thread to quit
	
//	XLock lock(&m_playLock);
//	if(!lock.Lock(XSHORTTIME)) return;
	//EnterCritical(); // do this after we kill the timer proc to prevent a deadlock
//
//	isOpen = false;
	
#ifdef _SYMBIAN
//	if(iMixer) {
//			iMixer->Stop();
//			delete iMixer;
//			iMixer = NULL;
//		}
//		
//		if(iWait) {
//			if(iWait->IsStarted())
//				iWait->AsyncStop();
//			delete iWait;
//			iWait = NULL;
//		}
//	iPlayStatus=EEngineNotReady;
#elif defined(_WIN32)
	XLock lock(&m_playLock);
	if(!lock.Lock(XSHORTTIME)) return;
	//EnterCritical(); // do this after we kill the timer proc to prevent a deadlock

	if ( !isOpen ) {
		//LeaveCritical();
		lock.UnLock();
		return;	// It is possible for timeKillEvent to call CloseDevice
	}
	isOpen = false;
	
	HWAVEOUT hW = (HWAVEOUT)hWaveOut;
	hWaveOut = 0;	// flag that we are shutting down so PollSound will bail

	int res = waveOutReset(hW);
	//FLASHASSERT(res == MMSYSERR_NOERROR);

	for ( int i = 0; i < nBuffers; i++ ) {
		int limit = 50;
		while ( limit-- ) {
			int res = waveOutUnprepareHeader(hW, (WAVEHDR*)(waveHDR+i), sizeof(WAVEHDR));
			if ( res != WAVERR_STILLPLAYING ) {
				//FLASHASSERT(res == MMSYSERR_NOERROR);
				break;
			}
			Sleep(1);
		}
	}

	res = waveOutClose(hW);
	playThread.Release(XFALSE);
	lock.UnLock();
#endif
	//FLASHASSERT(res == MMSYSERR_NOERROR);
	
	//CloseHandle(playThread);
	//playThread = 0;
	//LeaveCritical();
//	lock.UnLock();
}

void XSoundMix::BuffersReady()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::BuffersReady()\r\n");
#endif
	//XU32 id;
#ifdef _SYMBIAN
	iPlayStatus=EEnginePlaying;
//	playThread.Create(this,100);
#elif defined(_WIN32)
	playThread.Create(this,0);
#endif
	//playThread = CreateThread(0, 0, SndPlayThread, this, 0, &id);

	//int p = playThread.GetThreadPriority();//playThread);
	//playThread.SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);
	//SetThreadPriority(playThread, THREAD_PRIORITY_TIME_CRITICAL);
	//p = GetThreadPriority(playThread);
	//SetThreadPriority(playThread, 31);
	//int p = playThread.GetThreadPriority();//playThread);
	//playThread.SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);
	//p = playThread.GetThreadPriority(playThread);
	//playThread.SetThreadPriority(playThread, 31);
}


XBOOL XSoundMix::NeedStop()
	{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"XBOOL XSoundMix::NeedStop()\r\n");
#endif
#ifdef _SYMBIAN
	#ifdef _DefaultThread
		if(playThread == XNULL) return XTRUE;
		return playThread->NeedStop();
	#else
		if(playThread == XNULL) return XTRUE;
		return playThread->NeedFree();
	#endif
#else
		return playThread.NeedStop();
#endif
	}

void XSoundMix::DoPlay()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::DoPlay()\r\n");
#endif
	while ( XTRUE ) {
		// Wait for a buffer done event
		//if(playEvent.IsEvent(10*1000)) break;
#ifdef _SYMBIAN
	#ifdef _DefaultThread
		if(playThread->NeedStop() || closeEvent.IsEvent(0))
			break;
	#else
		if(!playThread || playThread->NeedFree() || closeEvent.IsEvent(0)) break;
	#endif
#else
		if(playThread.NeedStop()) 
			break;
#endif
		if(!playEvent.IsEvent(1000)) continue;
		playThread->Sleep(200);

		if(bPause)
		{
			playThread->Sleep(XSHORTTIME);
			continue;
		}

		//WaitForSingleObject(playEvent, 10*1000);
//		XLock lock(&m_playLock);
//		if(!lock.Lock(SOUNDWAITTIME)) continue;
		//EnterCritical();
		
		/*if ( nSilent > nBuffers )
		{
			lock.UnLock();
			continue;
		}*/

		// If a buffer is done, fill it...
		int nSent = 0;
		for ( int i = 0; i < nBuffers; i++ ) {
			XLock lock(&m_playLock);
			if(!lock.Lock(XSHORTTIME)) continue;
			_XWAVEHDR* hdr = (_XWAVEHDR*)(waveHDR+i);
			if ( (hdr->dwFlags & WHDR_DONE) || !(hdr->dwFlags & WHDR_PREPARED) ) {
				if ( buffer[i] )
				{
	 				BuildAndWrite( &waveHdr[i], false );
				}
// 				SendBuffer(i);
				nSent++;
				lock.UnLock();
			}
		}
		if ( nSent > nBuffers-2 ) 
			EnlargeBuffers();

		if ( nSilent > nBuffers ) 
		{
			//CloseNativeDevice();
			m_nSignal--;
			if(m_nSignal<=0)
				playEvent.ResetEvent();
//			lock.UnLock();
			playThread->Sleep(20);
			break;
		}
//		lock.UnLock();
		//LeaveCritical();
	}

}

void XSoundMix::BuildAndWrite( _XWaveHeader* wave,XBOOL silence )
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::BuildAndWrite( _XWaveHeader* wave,XBOOL silence )\r\n");
#endif
#ifdef _SYMBIAN
	int i = wave->bufferId;
	_XWAVEHDR* hdr = (_XWAVEHDR*)&waveHDR[i];
	if ( silence ) {
			memset(buffer[i], mixFormat.Is8Bit() ? 0x80 : 0, bufferBytes);
		} else {
			BuildBuffer( i );
		}	
	m_pOutput=hdr;
	//hdr->dwFlags |= WHDR_PREPARED;
	//hdr->dwFlags |= WHDR_INQUEUE;
	iPtrC8.Set((const TUint8*)hdr->lpData, hdr->dwBufferLength);
	int l = iPtrC8.Length();
	hdr->dwFlags=WHDR_PREPARED|WHDR_INQUEUE;
	iMixer->WriteL(iPtrC8);
	iWait->Start();
#elif defined(_WIN32)
	int i = wave->bufferId;
	_XWAVEHDR* hdr = (_XWAVEHDR*)&waveHDR[i];

	// Clean up the header
	//int res = waveOutUnprepareHeader((HWAVEOUT)hWaveOut, hdr, sizeof(WAVEHDR));
//	FLASHASSERT(res == MMSYSERR_NOERROR);

	// Generate the sound info
	if ( silence ) {
		memset(buffer[i], mixFormat.Is8Bit() ? 0x80 : 0, bufferBytes);
	} else {
		BuildBuffer( i );
	}

 	int res = waveOutPrepareHeader((HWAVEOUT)hWaveOut, hdr, sizeof(WAVEHDR));
	//FLASHASSERT(res == MMSYSERR_NOERROR);
	res = waveOutWrite((HWAVEOUT)hWaveOut, hdr, sizeof(WAVEHDR));
#endif
	//FLASHASSERT(res == MMSYSERR_NOERROR);
}




void XSoundMix::BuffersUpdated()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::BuffersUpdated()\r\n");
#endif
	for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
	{
		waveHDR[i].lpData	= (char*) buffer[i];
		waveHDR[i].dwUser	= i;			// Put in the buffer id in the user space
		waveHDR[i].dwBufferLength = bufferBytes;
	}
}

/*void XSoundMix::EnterCritical()	
{ EnterCriticalSection((PRTL_CRITICAL_SECTION)&playLock); }
void XSoundMix::LeaveCritical()	
{ LeaveCriticalSection((PRTL_CRITICAL_SECTION)&playLock); }*/



void XSoundMix::MarkBufferComplete(int i)
{
#ifdef _LostSound
	return;
#endif
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::MarkBufferComplete(int i)\r\n");
#endif
	XS32 bufferCompletionTime = XTime::GetTimeMSec();

	for ( XSoundChannel* c = firstChannel; c; c = c->next ) {
		c->samplesPlayed += c->bufferCounts[i];
		c->bufferCounts[i] = 0;
		c->bufferCompletionTime = bufferCompletionTime;
	}
	waveHDR[i].dwFlags = WHDR_DONE;
}

void XSoundMix::AddSound(XSoundChannel *c)
{
#ifdef _LostSound
	return;
#endif
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::AddSound(XSoundChannel *c)\r\n");
#endif
	//FLASHASSERT(!c->mixer);

	// Create the decompression object for the sound data
	//FLASHASSERT(!c->decomp);	// sound channels are never reused...
	if(!c->CreateDecompressor())//;
	//c->decomp = CreateDecompressor(c->sound->CompressFormat());
	//if ( !c->decomp ) 
		return;		// we failed to create a decompressor, don't even start the sound channel

	if ( c->streamProc ) {
		// A stream channel
		for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
			c->bufferCounts[i] = 0;

		c->bufferCompletionTime = XTime::GetTimeMSec();

	} else {
		// An event channel
		// Calculate the start and stop times
		XSound* s = c->sound;

		if ( c->syncFlags & syncStop ) {
			// Stop the sound
			FreeSound(s);
			return;
		} else if ( (c->syncFlags & syncNoMultiple) && SoundPlaying(s) ) {
			// Don't start the sound since it is already playing
			return;
		}

		int rs = s->RateShift();
		c->pos = c->inPoint44>>rs;
		c->outPos = XMIN(c->outPoint44>>rs, s->nSamples);
		c->envelopePos44 = 0;
		if ( c->pos >= c->outPos ) // there is no sound to play, ignore the channel
			return;

		c->decomp->Setup(c->sound, true);
		c->decomp->Decompress(0, c->pos);	// Skip to the start point
	}

	// Init prevSample in case we need to upsample...
	c->prevSampleInited = false;

	//EnterCritical();
	XLock lock(&m_playLock);
	if(lock.Lock(SOUNDWAITTIME)) //return;
	{

		PurgeList();	// Remove any old channels...

		c->AddRef();

		// Add to the mixer's channel list
		c->mixer = this;
		// Add at the end...
		XSoundChannel** link = &firstChannel;
		while ( *link ) 
			link = &(*link)->next;
		*link = c;
		//c->next = firstChannel;
		//firstChannel = c;

		// Open the device when we first need it...
		if ( !IsDeviceOpen() ) {
			OpenDevice();
			if ( !IsDeviceOpen() )
				// The device failed to open
				RemoveAll();
		}
		playEvent.SetEvent();
		m_nSignal++;
		lock.UnLock();
	}
	else
		{
		SYMBIANDEB(DefaultLog,"sound \r\n");
		}
	//LeaveCritical();
}

void XSoundMix::RemoveSound(XSoundChannel *c)
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::RemoveSound(XSoundChannel *c)\r\n");
#endif
	c->remove = XTRUE;
	PurgeList();
}

void XSoundMix::EnlargeBuffers()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::EnlargeBuffers()\r\n");
#endif
	if ( nBuffers < MAX_SOUND_BUFFERS ) {
		if ( buffer[nBuffers] == 0 ) {
			// Allocate the buffer if not already allocated
			buffer[nBuffers] = new XU8[bufferBytes];
			waveHdr[nBuffers].data		= (XU8*)buffer[nBuffers];
			waveHdr[nBuffers].bufferId	= nBuffers;
		}

		nBuffers++;
	}
	BuffersUpdated();	
}

void XSoundMix::StopAll()
{
	XLock lock(&m_playLock);
	if(!lock.Lock(XSHORTTIME)) return;

	XSoundChannel* c = firstChannel;
	while(c)
	{
		XSoundChannel*next=c->next;
		if(!c->remove) //continue;		
		{
			if(c->completeProc)
				c->completeProc(c->pArgs,false);
			//c->Stop();
			c->remove=XTRUE;
		}
		//c->mixer=XNULL;
		//delete c;
		c=next;
	}
	//firstChannel=XNULL;
	lock.UnLock();
}

void XSoundMix::BuildBuffer(int iBuf)
{
#ifdef _SYMBIANDEBUG
	static int un=0;
	SYMBIANDEB(DefaultLog,"void XSoundMix::BuildBuffer(int iBuf)\r\n");
#endif
	int mixN = 0;
	void* mixBuf[MAX_SOUND_CHANNELS];

	XBOOL silent = true;
	for ( XSoundChannel* c = firstChannel; c; c = c->next ) {
		if ( c->remove )
		{
			if(c->completeStop)
				//c->Release();
				delete c;
			continue;	// This channel is finished and waiting to be removed
		}

		silent = false;

		// Set up the buffer
		void* buf = 0;
		if ( mixN < MAX_SOUND_CHANNELS ) {
			buf = GetScratch();
			if ( buf ) 
				mixBuf[mixN++] = buf;
		}

		// Get the sound from the source into the mixBuf
		XSound* s = c->sound;
		int rs = s->RateShift();
		int bytesPerBlock = s->BytesPerBlock();

		XS32 n = SOUND_SAMPLES44>>rs;
		XU8* b = (XU8*)buf;

		//if(!c->nVolume||(c->m_pTransform&&c->m_pTransform)
		//	memset(b,0,scratchBytes);
		//else
		{
		if ( c->streamProc ) {
			// Get samples from a synchronized sound stream
			
			c->bufferCounts[iBuf] = 0;
			//FLASHASSERT(!s->Is8Bit());	// I think this is ok for uncompressed sounds...
			while ( n > 0 ) {
				if(playThread->NeedStop()) 
					break;
				XS32 nc = XMIN(n, c->blockSamples);
				if ( nc > 0) {

					if ( c->silentBlock||c->nVolume==0||(c->m_pTransform&&c->m_pTransform->m_dVolume==0) ) {
						// Note we are not advancing in the stream...
						if ( b )
							memset(b, 0x00, nc*bytesPerBlock);
					} else {
						c->decomp->Decompress((XS16*)b, nc);
					}
					if ( !c->streamStalled )
						c->bufferCounts[iBuf] += nc;	// we're playing these samples from the source stream...

					if ( b )
						b += nc*bytesPerBlock;
					n -= nc;
					c->blockSamples -= nc;
				} else {
					// We are out of samples, get a new block
					c->streamStalled = !c->streamProc(c);

					if(!c->streamProc)//stream end
					{
						if ( --c->loops <= 0 ) 
						{
							if(c->completeProc)
								c->completeProc(c->pArgs,false);
							//if(c->completeStop)
								//c->Release();
							//	delete c;
							//else
							c->remove = true;
							break;
						}
					}

					if ( c->blockSamples <= 0) {
						// We did not get any data, create a silent block
                        if (c->blockCanBeZero)
                        {
                            c->blockCanBeZero = false;
                            if (c->seekSamples < 0)
                            {
                                // the steaming code is telling us
                                // to play some silence to sync to the
                                // futere sound frames
						        c->blockSamples = -c->seekSamples;
						        c->seekSamples = 0;
                            }
                        }
                        else
						    c->blockSamples = s->nSamples;

						c->silentBlock = true;
					} else {
						// Play back this block of data
						c->silentBlock = false;
					}
				}
			}

			// Fade in on the first block
			
			if ( c->fadeIn ) {
				c->fadeIn = false;
				_XSoundEnvelope fader;
				fader.nPoints = 2;
				fader.points[0].mark44 = 0;
				fader.points[0].level0 = fader.points[0].level1 = 0;
				fader.points[1].mark44 = XMIN(SOUND_SAMPLES44, sndRate44K/4);
				fader.points[1].level0 = fader.points[1].level1 = sndMaxLevel;
				if ( buf )
					ApplyEnvelope(c->sound->format, buf, SOUND_SAMPLES44, 0, &fader);
			}

		} else {
			// Get samples from an event sound
			while ( n > 0 ) {
				XS32 nc = XMIN(n, c->outPos - c->pos);
				if ( nc <= 0 ) {
					// We are out of samples
					if ( --c->loops > 0 ) {
						// Loop the sound
						c->pos = c->inPoint44>>rs;
				
						// Setup should be the same as a rewind operation for the decompressor - Warning we cannot do memory allocations here on Mac...
						c->decomp->Setup(s, true);
						c->decomp->Decompress(0, c->pos);	// skip

					} else {
						// Remove the channel and pad w/ silence
						if ( b )
							memset(b, s->Is8Bit() ? 0x80 : 0x00, n*bytesPerBlock);
						
						if(c->completeProc)
							c->completeProc(c->pArgs,false);
						//if(c->completeStop)
							//c->Release();
						//	delete c;
						//else
						c->remove = true;
						break;
					}
				}

				// Decompress the sound
				c->decomp->Decompress((XS16*)b, nc);
					
				if ( b )
					b += nc*bytesPerBlock;
				n -= nc;
				c->pos += nc;
			}
			}
			
			
		}
		
		
		
		//SetVolume(c->nVolume);
		// Convert the sound to the mixer format
		if ( buf ) 
		{
			
			
			// Save the last sample from this buffer for interpolating with the next buffer
			XS32 savedSample;
			memcpy(&savedSample, (XU8*)buf+bytesPerBlock*((SOUND_SAMPLES44>>rs)-1), bytesPerBlock);

			if ( !c->prevSampleInited ) {
				// For the first buffer, just use the first sample of this buffer
				memcpy(&c->prevSample, buf, bytesPerBlock);
				c->prevSampleInited = true;
			}

			ConvertSound(buf, SOUND_SAMPLES44, s->format&~sndCompressMask, mixFormat.format, &c->prevSample);
			c->prevSample = savedSample;
		}

		// Apply a volume envelope
		if ( c->envelope.nPoints > 0 ) {
			if ( buf )
				ApplyEnvelope(mixFormat.format, buf, SOUND_SAMPLES44, c->envelopePos44, &c->envelope);
			c->envelopePos44 += SOUND_SAMPLES44;
		}
		if(buf)
		{
			if(c->nVolume>=0)
				SetChannelVolume(c->sound->format, buf, SOUND_SAMPLES44, c->nVolume);
			if(c->m_pTransform)
				ApplyTransform(c->sound->format,buf,SOUND_SAMPLES44,c->m_pTransform);
		}
	}
	//}

	if ( silent )
		nSilent++;
	else
		nSilent = 0;

	// Add the sound channels together
	if ( mixFormat.Is8Bit() ) {
		// Combine the sounds into our buffer
		// Note this changes the mixBuf pointers
		XU8* buf = (XU8*)buffer[iBuf];
		if ( mixN == 0 ) {
			// Set the buffer to silence
			memset(buf, 0x80, bufferBytes);
			//memset(buf, 0x10, bufferBytes/2); // a test square wave...

		} else {
			if ( mixN == 1 ) {
				// Copy a single sound
				memcpy(buf, mixBuf[0], bufferBytes);
				
			} else if ( mixN == 2 ) {
				// Mix 2 sounds
				XU8* src1 = (XU8*)mixBuf[0];
				XU8* src2 = (XU8*)mixBuf[1];
				XU8* dst = buf;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- ) {
					int s = (int)*src1++ + *src2++ - 0x80;
					if ( s & 0xFFFF00 ) {
						// Overflow or underflow
						*dst++ = s < 0 ? 0 : 0xff;
					} else {
						*dst++ = (XU8)s;
					}
				}

			} else {
				// Mix multiple sounds
				XU8* src[MAX_SOUND_CHANNELS];
				memcpy(src, mixBuf, sizeof(void*)*mixN);

				XU8* dst = buf;
				int offset = (mixN-1) * -0x80;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- ) {
					int s = offset;
					for ( int j = mixN; j--; ) {
						s += *(src[j]++);
					}
					if ( s & 0xFFFF00 ) {
						// Overflow or underflow
						*dst++ = s < 0 ? 0 : 0xff;
					} else {
						*dst++ = (XU8)s;
					}
				}
			}
		}
	} else {
		// Combine the sounds into our buffer
		// Note this changes the mixBuf pointers
		XS16* buf = (XS16*)buffer[iBuf];
		if ( mixN == 0 ) {
			// Set the buffer to silence
			memset(buf, 0, bufferBytes);
			//memset(buf, 0x10, bufferBytes/2); // a test square wave...
		} else {
			if ( mixN == 1 ) {
				// Copy a single sound
				memcpy(buf, mixBuf[0], bufferBytes);

			} else if ( mixN == 2 ) {
				// Mix 2 sounds
				XS16* src1 = (XS16*)mixBuf[0];
				XS16* src2 = (XS16*)mixBuf[1];
				XS16* dst = buf;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- ) {
					XS32 s = (XS32)*src1++ + *src2++;
					if ( (XS16)s != s ) {
						// Overflow or underflow - if the high 16 bits are not all 0 or all 1
						*dst++ = s < 0 ? (int)-32768 : (int)32767;
					} else {
						*dst++ = (XS16)s;
					}
				}

			} else {
				// Mix multiple sounds
				XS16* src[MAX_SOUND_CHANNELS];
				memcpy(src, mixBuf, sizeof(void*)*mixN);

				XS16* dst = buf;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- ) {
					XS32 s = 0;
					for ( int j = mixN; j--; ) {
						s += *(src[j]++);
					}
					if ( (XS16)s != s ) {
						// Overflow or underflow - if the high 16 bits are not all 0 or all 1
						*dst++ = s < 0 ? (int)-32768 : (int)32767;
					} else {
						*dst++ = (XS16)s;
					}
				}
			}
		}
	}
	if(m_pTransformn)
		ApplyTransform(mixFormat.format,buffer[iBuf],SOUND_SAMPLES44,m_pTransformn);

	ReleaseScratch();	// mark the scratch buffers as unused
#ifdef _SYMBIANDEBUG
	SYMBIANDEB1(DefaultLog,"void XSoundMix::BuildBuffer(int iBuf)----%dend\r\n",un);
	un++;
#endif
}

void XSoundMix::FreeSound(XSound *s)
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::FreeSound(XSound *s)\r\n");
#endif
	for ( XSoundChannel* c = firstChannel; c; c = c->next ) {
		if ( c->sound == s )
			c->remove = true;
	}

	PurgeList();
}

XBOOL XSoundMix::SoundPlaying(XSound *s)
{
#ifdef _LostSound
	return false;
#endif
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"XBOOL XSoundMix::SoundPlaying(XSound *s)\r\n");
#endif
	XBOOL playing = false;
	//EnterCritical();
	XLock lock(&m_playLock);
	if(!lock.Lock(XSHORTTIME)) return true;
	for ( XSoundChannel* c = firstChannel; c; c = c->next ) {
		if ( c->sound == s && !c->remove ) {
			playing = true;
			break;
		}
	}
	lock.UnLock();
	//LeaveCritical();
	return playing;
}

void XSoundMix::PurgeList()
{
#ifdef _SYMBIANDEBUG
	SYMBIANDEB(DefaultLog,"void XSoundMix::PurgeList()\r\n");
#endif
	//EnterCritical();
	XLock lock(&m_playLock);
	if(!lock.Lock(XSHORTTIME)) return;
	XSoundChannel** l = &firstChannel;
	while ( *l ) {
		XSoundChannel* c = *l;
		if ( c->remove ) {
			// Remove the channel
			*l = c->next;
			c->remove = false;
			c->next = 0;
			c->mixer = 0;
			if(c->completeStop)
				delete c;
			else
				c->Release();
		} else {
			// Advance to the next channel
			l = &c->next;
		}
	}
	m_nSignal=0;
	lock.UnLock();
	//LeaveCritical();
}

void XSoundMix::OpenDevice()
{
#ifdef _LostSound
	return;
#endif
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::OpenDevice()\r\n");
#endif
	if ( IsDeviceOpen() ) return;

	createEvent.Create();
	createdEvent.Create();
	createdEvent.ResetEvent();
	createEvent.SetEvent();
#ifdef _SYMBIAN
//	if(playThread == XNULL)
//		{
//		XThreadPool *pool = XThreadPool::GetInstance();
//		playThread = pool->GetThread(this);
//		}
//	if(playThread)
//		playThread->SetName(_L("soundThread"));
	if(playThread == NULL)
		playThread = new XThread();
	playThread->Create(this,100);
#else
	playThread.Create(this,100);
#endif
	while(!createdEvent.IsEvent(1000))
	{
		if(NeedStop()) return;
	}
	createEvent.Release();
	createdEvent.Release();
	//EnterCritical();
//	XLock lock(&m_playLock);
//	if(!lock.Lock(XSHORTTIME)) return;
//	{
//		GetNativeSoundCap( &mixFormat.format );
//		int okay = OpenNativeDevice( &mixFormat );
//
//		if ( !okay ) 
//			goto Fail; // something is wrong...
//
//		// We are ready to play sound...
//// 		deviceOpen = true;
//		nSilent = 0;
//
//		AllocBuffers();
//		if ( !buffer[0] || !buffer[1] ) {
//			CloseNativeDevice();
//			goto Fail;
//		}
//
//		// Prime the stream, send first two buffers as silence to prevent burps
//		int nSilence = firstPlay ? 4 : 2;
//		firstPlay = false;
//		for ( int i = 0; i < nBuffers; i++ ) {
//// 			SendBuffer(i, i < nSilence);	
//			if ( buffer[i] )
//			{
//	 			BuildAndWrite( &waveHdr[i], i < nSilence );
//			}
//		}
//
//		BuffersReady();
//	}
//
//Fail:
//	lock.UnLock();
//	//TRACE("open\n");
//	//LeaveCritical();
}
#ifdef _SYMBIAN
void XSoundMix::CloseDevice()
{
#ifdef _LostSound
	return;
#endif
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::CloseDevice()\r\n");
#endif
//	if(!IsDeviceOpen()) return;
//	playThread.Stop();
	CloseNativeDevice();
}
#endif

void XSoundMix::RemoveAll()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::RemoveAll()\r\n");
#endif
	for ( XSoundChannel* c = firstChannel; c; c = c->next )
		c->remove = true;
		
	PurgeList();
}

void* XSoundMix::GetScratch()
{
#ifdef _SoundLog
	SYMBIANDEB1(DefaultLog,"void* XSoundMix::GetScratch()---%d\r\n",scratchBytes);
#endif
	// The mac can't allocate these at interupt time, they better already be available
//	XU8 *test = new XU8[scratchBytes];
	if ( !scratchBuf[nextScratch] ) {
		scratchBuf[nextScratch] = new XU8[scratchBytes];
	}
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void* XSoundMix::GetScratch()---end\r\n");
#endif
	return scratchBuf[nextScratch++];
}


void XSoundMix::AllocBuffers()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::AllocBuffers()\r\n");
#endif
	XS32 nSamples = SOUND_SAMPLES44>>mixFormat.RateShift();
	if ( buffer[0] && nSamples == bufferSamples ) 
		return;
	FreeBuffers();

	bufferDuration = (SOUND_SAMPLES44*10)/441;
		//FixedDiv(SOUND_SAMPLES44, FloatToFixed(44.1));

	// Allocate the buffers
	bufferSamples = nSamples;
	bufferBytes = mixFormat.BytesPerBlock()*nSamples;

	{
		for ( int i = 0; i < nBuffers; i++ )
			buffer[i] = new XU8[bufferBytes];
	}

	// We allocate scratch buffers large enough to hold 44kHz 16 bit stereo sound
	//	so that we have space for any format conversion
	scratchBytes = SOUND_SAMPLES44*4;

	// Set up the wave headers
	for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ ) {
		memset(waveHdr+i, 0, sizeof(_XWaveHeader));
		waveHdr[i].data			= (XU8*)buffer[i];
		waveHdr[i].bufferId		= i;//(U32)this;
	}
	BuffersUpdated();
}

void XSoundMix::FreeBuffers()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::FreeBuffers()\r\n");
#endif
	{
		for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ ) {
			delete buffer[i];
			buffer[i] = 0;
		}
	}
	{
		for ( int i = 0; i < MAX_SOUND_CHANNELS; i++ ) {
			delete scratchBuf[i];
			scratchBuf[i] = 0;
		}
	}
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::FreeBuffers()------------------------end\r\n");
#endif
}


void XSoundMix::Construct()
{
#ifdef _LostSound
	return;
#endif
//	isCreate = false;
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::Construct()---1\r\n");
#endif
	mixFormat.Init();
// 	mixFormat.format = BEST_SOUND_FORMAT;

	firstPlay = true;

//	nVolume=0xafff;//0xafffafff;

	firstChannel = 0;
	nBuffers = INITIAL_SOUND_BUFFERS;

	bufferDuration = SOUND_SAMPLES44*10/441;
		//FixedDiv(SOUND_SAMPLES44, FloatToFixed(44.1));

	nSilent = 0;

	bPause = XFALSE;

	bufferSamples = 0;
	{
		for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
			buffer[i] = 0;
	}
	
	nextScratch = 0;
	{
		for ( int i = 0; i < MAX_SOUND_CHANNELS; i++ )
			scratchBuf[i] = 0;
	}

	NativeConstruct();
}

void XSoundMix::Destruct()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::Destruct()\r\n");
#endif
	RemoveAll();
	CloseNativeDevice();
	FreeBuffers();
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::Destruct()--------------------end\r\n");
#endif
}

XBOOL XSoundMix::SetVolume(int v)
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"XBOOL XSoundMix::SetVolume(int v)\r\n");
#endif
	if(v>0xffff) v=0xffff;
	else if(v<0) v=0;
	//v=v*0xff/100;
	
#ifdef _SYMBIAN
//	if(!IsDeviceOpen())
//		{
//		nVolume=v;
//		return XFALSE;
//		}
//	int iMax=iStream->MaxVolume();
//	v=(v*iMax)>>16;
//	v=v|v<<16;
//	iStream->SetVolume(v);
	float f = m_nMaxVolume /10.0;
	if(v>10)
		v=10;
	v = v*f;
//	iStream->SetVolume(v);
	nVolume=v;
	iVolumeAdjust = EVolumeUp;
	m_bVolumeChange=true;
#elif defined(_WIN32)
	if(!IsDeviceOpen()) return XFALSE;
	//nVolume=(v<<16)|v;
	nVolume=v;
	::waveOutSetVolume((HWAVEOUT)hWaveOut,v);
#endif
	return XTRUE;
}

int XSoundMix::GetVolume()
{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"int XSoundMix::GetVolume()\r\n");
#endif
	if(!IsDeviceOpen()) return nVolume;
	XU32 v=nVolume;
#ifdef _SYMBIAN
//	v=iMixer->Volume();
//	int iMax=iStream->MaxVolume();
//	v=(v*0xffff)/iMax;
#elif defined(_WIN32)
	::waveOutGetVolume((HWAVEOUT)hWaveOut,&v);
	v=//(v&0xff)*100/0xff;
		((v>>16)+(v&0xffff))>>1;
#endif
	return v;
	//return XTRUE;
}
#ifdef _SYMBIAN
int XSoundMix::GetMaxVolume()
	{
	if(iStream) return m_nMaxVolume;
	return 0;
	}
#endif

#ifdef _SYMBIAN
void XSoundMix::_Open()
	{
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::_Open()\r\n");
#endif
		if ( IsDeviceOpen() ) return;
		while(!createEvent.IsEvent(1000))
		{
			if(NeedStop()) return;
		}

		//EnterCritical();
		XLock lock(&m_playLock);
		if(!lock.Lock(XSHORTTIME)) return;
		{
			GetNativeSoundCap( &mixFormat.format );
			int okay = OpenNativeDevice( &mixFormat );

			if ( !okay ) 
				goto Fail; // something is wrong...

			// We are ready to play sound...
	// 		deviceOpen = true;
			nSilent = 0;

			AllocBuffers();
			if ( !buffer[0] || !buffer[1] ) {
				CloseNativeDevice();
				goto Fail;
			}

			// Prime the stream, send first two buffers as silence to prevent burps
			int nSilence = firstPlay ? 4 : 2;
			firstPlay = false;
			if(!m_bVolumeChange)
				{
				CRepository* cr = CRepository::NewL( KCRUidProfileEngine );
				TInt value;
				User::LeaveIfError( cr->Get( KProEngActiveRingingVolume, value ) );
				nVolume = value;
				delete cr;
				}
			iStream->SetVolume(nVolume);
			for ( int i = 0; i < nBuffers; i++ ) {
	// 			SendBuffer(i, i < nSilence);	
				if ( buffer[i] )
				{
		 			BuildAndWrite( &waveHdr[i], i < nSilence );
				}
			}

			BuffersReady();
			
		}

	Fail:
		lock.UnLock();
	}

void XSoundMix::_Close()
	{
//	if(!NeedStop()) return;
#ifdef _SoundLog
	SYMBIANDEB(DefaultLog,"void XSoundMix::_Close()\r\n");
#endif
	if(!closeEvent.IsEvent(0)) return;
	XLock lock(&m_playLock);
	if(!lock.Lock(XSHORTTIME)) return;
	//EnterCritical(); // do this after we kill the timer proc to prevent a deadlock

	isOpen = false;
	if(iMixer) {
			iMixer->Stop();
			delete iMixer;
			iMixer = NULL;
		}
		
		if(iWait) {
			if(iWait->IsStarted())
				iWait->AsyncStop();
			delete iWait;
			iWait = NULL;
		}
	iPlayStatus=EEngineNotReady;
	lock.UnLock();
	closedEvent.SetEvent();
	}
#endif


