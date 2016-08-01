// XXSoundMix.cpp: implementation of the XXSoundMix class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XTime.h"
#include "XMp3Decomp.h"
#ifdef __ANDROID__
	#include "media/AudioTrack.h"
	using namespace android;
	#include "Define.h"
#include <dlfcn.h>
#include "GlobalFun.h"

#endif
#include "XNativeSoundMix.h"

#if 0
#define LOGWHERESOUND()  LOGWHERE()
#else
#define LOGWHERESOUND()
#endif

const XS32 kRateTable[4] = {sndRate5K, sndRate11K, sndRate22K, sndRate44K};
const int kRateShiftTable[4] = {3, 2, 1, 0};

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

	XU32 bitBuf;	// this should always contain at least 24 bits of data
	int bitPos;

	void FillBuffer();
	void SkipBits(XS32 n);
	XS32 GetBits(int n)
	{
		if ( bitPos < n )
			FillBuffer();

		XS32 v = ((XU32)bitBuf << (32-bitPos)) >> (32-n);
		bitPos -= n;
		return v;
	}
	XS32 GetSBits(int n)
	{
		if ( bitPos < n )
			FillBuffer();

		XS32 v = ((XS32)bitBuf << (32-bitPos)) >> (32-n);
		bitPos -= n;
		return v;
	}

public:

    virtual void Setup(XSound* snd, XBOOL reset = false);
	void Decompress(XS16* dst, XS32 n);

};

void XIADPCMDecomp::Setup(XSound* snd, XBOOL reset)
{
	stereo = snd->Stereo();
	src = (XU8 *)snd->samples;
	nBits = 0;	// flag that it is not inited
	nSamples = 0;
	bitPos = 0;
	bitBuf = 0;
}

void XIADPCMDecomp::FillBuffer()
{
	while ( bitPos <= 24 )
	{
		bitBuf = (bitBuf<<8) | *src++;
		bitPos += 8;
	}
}

void XIADPCMDecomp::SkipBits(XS32 n)
{
	if ( n <= 32 )
	{
		while ( n > 0 )
		{
			int k = (int)XMIN(16,n);
			GetBits(k);
			n -= k;
		}
	}
	else
	{
		n -= bitPos;
		bitPos = 0;

		XS32 k = n/8;
		src += k;

		GetBits((int)n & 0x7);
	}
}

void XIADPCMDecomp::Decompress(XS16* dst, XS32 n)
{
	if ( nBits == 0 )
	{
		nBits = (int)GetBits(2)+2;
	}

	if ( !dst )
	{
		XS32 nInBlock = nSamples & ~0xFFF;
		if ( nInBlock > 0 && n > nInBlock+nSamples )
		{
			nSamples += nInBlock;
			n -= nInBlock;
			XS32 k = nInBlock*nBits;
			if ( stereo ) k *= 2;
			SkipBits(k);
		}

		// Skip whole blocks
		XS32 nblocks = n >> 12;
		// The first sample is
		XS32 k = nblocks * ((16 + 6) + nBits * 0xFFF);
		if ( stereo ) k *= 2;
		SkipBits(k);
		n = n & 0xFFF;

		// Skip a portion of the current block
		XS16 buf[1024];
		XS32 limit = stereo ? 512 : 1024;
		while ( n > 0 )
		{
			XS32 k = XMIN(limit, n);
			Decompress(buf, k);
			n -= k;
		}
	}
	else
	{
		// Decompress the samples
		const int* indexTable = indexTables[nBits-2];
		int k0 = 1 << (nBits-2);
		int signmask = 1 << (nBits-1);
		if ( !stereo )
		{
			// Optimize for mono
			XS32 vp = valpred[0];	// maybe these can get into registers...
			int ind = index[0];
			XS32 ns = nSamples;
			while ( n-- > 0 )
			{
				ns++;
				if ( (ns & 0xFFF) == 1 )
				{
					// Get a new block header
					*dst++ = (XS16)(vp = GetSBits(16));
					ind = (int)GetBits(6); // The first sample in a block does not have a delta
				}
				else
				{
					// Process a delta value
					int delta = (int)GetBits(nBits);

					// Compute difference and new predicted value
					// Computes 'vpdiff = (delta+0.5)*step/4'
					int step = stepsizeTable[ind];
					XS32 vpdiff = 0;
					int k = k0;
					do
					{
						if ( delta & k )
						{
							vpdiff += step;
						}
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
					if ( ind < 0 )
						ind = 0;
					else if ( ind > 88 )
						ind = 88;

					// clamp output value
					if ( vp != (XS16)vp )
						vp = vp < 0 ? -32768 : 32767;

					*dst++ = (XS16)vp;
				}
			}
			valpred[0] = vp;
			index[0] = ind;
			nSamples = ns;
		}
		else
		{
			int sn = stereo ? 2 : 1;
			// Stereo
			while ( n-- > 0 )
			{
				nSamples++;
				if ( (nSamples & 0xFFF) == 1 )
				{
					// Get a new block header
					for ( int i = 0; i < sn; i++ )
					{
						*dst++ = (XS16)(valpred[i] = GetSBits(16));
						index[i] = (int)GetBits(6); // The first sample in a block does not have a delta
					}
				}
				else
				{
					// Process a delta value
					for ( int i = 0; i < sn; i++ )
					{
						int delta = (int)GetBits(nBits);

						int step = stepsizeTable[index[i]];
						XS32 vpdiff = 0;
						int k = k0;
						do
						{
							if ( delta & k )
							{
								vpdiff += step;
							}
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
						if ( index[i] < 0 )
							index[i] = 0;
						else if ( index[i] > 88 )
							index[i] = 88;

						// clamp output value
						if ( valpred[i] != (XS16)valpred[i] )
							valpred[i] = valpred[i] < 0 ? -32768 : 32767;

						*dst++ = (XS16)valpred[i];
					}
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////

class XIPCMDecomp : public XSoundDecomp
{
public:
	XS32     pos;
	XS32     bytesPerBlock;
	void*   samples;
    XBOOL    swapBytes;

public:
    virtual void Setup(XSound* snd, XBOOL reset = false);
    void Decompress(XS16* dst, XS32 n);	// if dst == 0, just skip the requested samples
};

void XIPCMDecomp::Decompress(XS16* dst, XS32 n)
{
	if ( dst )
	{
		memcpy(dst, (XU8 *)samples + pos*bytesPerBlock, n*bytesPerBlock);
	}
	pos += n;
}

void XIPCMDecomp::Setup(XSound* snd, XBOOL reset)
{
	samples = snd->samples;
	bytesPerBlock = snd->BytesPerBlock();
	pos = 0;
	swapBytes = false;

}

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

void Cvt8RateDiv2(XU8* samples, XS32 n, XBOOL stereo)
{
	XU8* src = samples;
	XU8* dst = samples;
	if ( stereo )
	{
		while ( n > 0 )
		{
			dst[0] = (XU8)(((int)src[0]+src[2])/2);
			dst[1] = (XU8)(((int)src[1]+src[3])/2);
			src+=4;
			dst+=2;
			n-=2;
		}
	}
	else
	{
		while ( n > 0 )
		{
			dst[0] = src[0];
			src+=2;
			dst++;
			n-=2;
		}
	}
}

void Cvt8RateMul2(XU8* samples, XS32 n, XBOOL stereo, XU8* prevSample)
{
	if ( stereo )
	{
		XU8* src = samples+n*2;
		XU8* dst = samples+n*4;

		while ( true )
		{
			dst-=4;
			src-=2;
			n--;
			dst[2] = src[0];
			dst[3] = src[1];
			if ( n == 0 )
			{
				dst[0] = (XU8)(((int)prevSample[0]+src[0])/2);
				dst[1] = (XU8)(((int)prevSample[1]+src[1])/2);
				break;
			}
			dst[0] = (XU8)(((int)src[-2]+src[0])/2);
			dst[1] = (XU8)(((int)src[-1]+src[1])/2);
		}
	}
	else
	{
		XU8* src = samples+n;
		XU8* dst = samples+n*2;
		while ( true )
		{
			dst-=2;
			src--;
			n--;
			dst[1] = src[0];
			if ( n == 0 )
			{
				dst[0] = (XU8)(((int)prevSample[0]+src[0])/2);
				break;
			}
			dst[0] = (XU8)(((int)src[-1]+src[0])/2);
		}
	}
}

void Cvt16RateDiv2(XS16* samples, XS32 n, XBOOL stereo)
{
	XS16* src = samples;
	XS16* dst = samples;
	if ( stereo )
	{
		while ( n > 0 )
		{
			dst[0] = (XS16)(((XS32)src[0]+src[2])/2);
			dst[1] = (XS16)(((XS32)src[1]+src[3])/2);
			src+=4;
			dst+=2;
			n-=2;
		}
	}
	else
	{
		while ( n > 0 )
		{
			dst[0] = src[0];
			src+=2;
			dst++;
			n-=2;
		}
	}
}

void Cvt16RateMul2(XS16* samples, XS32 n, XBOOL stereo, XS16* prevSample)
{
	if ( stereo )
	{
		XS16* src = samples+n*2;
		XS16* dst = samples+n*4;

		while ( true )
		{
			dst-=4;
			src-=2;
			n--;
			dst[2] = src[0];
			dst[3] = src[1];
			if ( n == 0 )
			{
				dst[0] = (XS16)(((XS32)prevSample[0]+src[0])/2);
				dst[1] = (XS16)(((XS32)prevSample[1]+src[1])/2);
				break;
			}
			dst[0] = (XS16)(((XS32)src[-2]+src[0])/2);
			dst[1] = (XS16)(((XS32)src[-1]+src[1])/2);
		}
	}
	else
	{
		XS16* src = samples+n;
		XS16* dst = samples+n*2;

		while ( true )
		{
			dst-=2;
			src--;
			n--;
			dst[1] = src[0];
			if ( n == 0 )
			{
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
	while ( n > 0 )
	{
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
	while ( n > 0 )
	{
		n--;
		src--;
		dst-=2;
		dst[0] = dst[1] = *src;
	}
}

inline void Cvt16StereoToMono(XS16* samples, XS32 n)
{
	XS16* src = samples;
	XS16* dst = samples;
	while ( n > 0 )
	{
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
	while ( n > 0 )
	{
		n--;
		src--;
		dst-=2;
		dst[0] = dst[1] = *src;
	}
}

// The memory pointed to by src->samples must be large enough to
//	hold the largest amount of data required by the conversion.
// The result will be placed in this memory
// returns the new number of samples - this is different only w/ a rate change...
void ConvertSound(void* samples, XS32 nSamples44, int srcFormat, int dstFormat, void* prevSample)
{
	if ( srcFormat == dstFormat || nSamples44 == 0 ) return;
	XSound src, dst;
	src.format = srcFormat;
	dst.format = dstFormat;

	XS32 nSamples = nSamples44>>src.RateShift();


	int srcR = src.RateShift();
	int dstR = dst.RateShift();
	// Decrease the src rate
	while ( srcR < dstR )
	{
		if ( src.Is8Bit() )
			Cvt8RateDiv2((XU8*)samples, nSamples, src.Stereo());
		else
			Cvt16RateDiv2((XS16*)samples, nSamples, src.Stereo());
		nSamples /= 2;
		srcR++;
	}
	// Increase the src rate
	while ( srcR > dstR )
	{
		if ( src.Is8Bit() )
			Cvt8RateMul2((XU8*)samples, nSamples, src.Stereo(), (XU8*)prevSample);
		else
			Cvt16RateMul2((XS16*)samples, nSamples, src.Stereo(), (XS16*)prevSample);
		nSamples *= 2;
		srcR--;
	}

	// Sample Size change
	if ( !dst.Is8Bit() && src.Is8Bit() )
	{
		// Map 8->16
		Cvt8to16((XU8*)samples, nSamples, src.Stereo());

	}
	else if ( dst.Is8Bit() && !src.Is8Bit() )
	{
		// Map 16->8
		Cvt16to8((XS16*)samples, nSamples, src.Stereo());
	}

	// Stereo/Mono conversion
	if ( !dst.Stereo() && src.Stereo() )
	{
		// Stereo -> Mono conversion
		if ( dst.Is8Bit() )
			Cvt8StereoToMono((XU8*)samples, nSamples);
		else
			Cvt16StereoToMono((XS16*)samples, nSamples);

	}
	else if ( dst.Stereo() && !src.Stereo() )
	{
		// Mono -> Stereo conversion
		if ( dst.Is8Bit() )
			Cvt8MonoToStereo((XU8*)samples, nSamples);
		else
			Cvt16MonoToStereo((XS16*)samples, nSamples);
	}
}


void ApplyRamp8(XU8* s, XS32 n, int stride, XS32 level, XS32 dLevel)
{
	if ( dLevel == 0 )
	{
		// A constant level
		if ( level == 0 )
		{
			// Silence
			while ( n-- )
			{
				*s = 128;
				s+=stride;
			}
		}
		else if ( level == sndMaxLevel )
		{
			// Nothing to do...
		}
		else
		{
			// A constant level
			level = level >> 15;
			while ( n-- )
			{
				*s = (XU8)((( ((int)*s-128) * level) >> 15) + 128);
				s+=stride;
			}
		}
	}
	else
	{
		// A ramped level
		while ( n-- )
		{
			*s = (XU8)((( ((int)*s-128) * (level>>15)) >> 15) + 128);
			level += dLevel;
			s+=stride;
		}
	}
}

void ApplyRamp16(XS16* s, XS32 n, int stride, XS32 level, XS32 dLevel)
{
	if ( dLevel == 0 )
	{
		// A constant level
		if ( level == 0 )
		{
			// Silence
			while ( n-- )
			{
				*s = 0;
				s+=stride;
			}
		}
		else if ( level == sndMaxLevel )
		{
			;
		}
		else
		{
			// A constant level
			level = level >> 15;
			while ( n-- )
			{
				*s = (XS16)((*s * level)>>15);
				s+=stride;
			}
		}
	}
	else
	{
		// A ramped level
		while ( n-- )
		{
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
	while ( n > 0 )
	{
		// Find the proper segment of the envelope
		while ( ie < envelope->nPoints )
		{
			if ( envelope->points[ie].mark44 > time44 )
				break;
			ie++;
		}

		// Set up a new envelope segment
		// Note: The level difference equation is 2.30 fixed point numbers
		XS32 nSeg = n;
		XS32 dLevel0, dLevel1, level0, level1;
		if ( ie == 0 )
		{
			// Before the start-should never happen...
			dLevel0 = dLevel1 = 0;
			level0 = (XS32)envelope->points[0].level0 << 15;
			level1 = (XS32)envelope->points[0].level1 << 15;
		}
		else if ( ie == envelope->nPoints )
		{
			// After the end
			dLevel0 = dLevel1 = 0;
			level0 = (XS32)envelope->points[ie-1].level0 << 15;
			level1 = (XS32)envelope->points[ie-1].level1 << 15;
		}
		else
		{
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
		if ( snd.Stereo() )
		{
			if ( snd.Is8Bit() )
			{
				ApplyRamp8((XU8*)samples, nSeg, 2, level0, dLevel0);
				ApplyRamp8((XU8*)samples+1, nSeg, 2, level1, dLevel1);
			}
			else
			{
				ApplyRamp16((XS16*)samples, nSeg, 2, level0, dLevel0);
				ApplyRamp16((XS16*)samples+1, nSeg, 2, level1, dLevel1);
			}
		}
		else
		{
			// Merge the two envelopes for a mono envelope
			level0 = (level0 + level1)/2;
			dLevel0 = (dLevel0 + dLevel1)/2;

			if ( snd.Is8Bit() )
			{
				ApplyRamp8((XU8*)samples, nSeg, 1, level0, dLevel0);
			}
			else
			{
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
	XSound snd;
	snd.format = format;
	int rateShift = snd.RateShift();
	XS32 n = nSamples44 >> rateShift;

	int bytesPerBlock = snd.BytesPerBlock();

	int ie = 0;
	XS32 dLevel=0,level=sndMaxLevel*((v&0xff)*sndMaxLevel/100);  //XS32 dLevel=0,level=sndMaxLevel*v;//(sndMaxLevel*v/100);
	while ( n > 0 )
	{
		// Find the proper segment of the envelope
		// Set up a new envelope segment
		// Note: The level difference equation is 2.30 fixed point numbers
		XS32 nSeg = n;

		// Apply this segment of the envelope
		n -= nSeg;
		if ( snd.Stereo() )
		{
			if ( snd.Is8Bit() )
			{
				if(!v)
				{
					memset(samples,0,nSeg*2);
				}
				else
				{
					ApplyRamp8((XU8*)samples, nSeg, 2, level, dLevel);
					ApplyRamp8((XU8*)samples+1, nSeg, 2, level, dLevel);
				}
			}
			else
			{
				if(!v)
				{
					memset(samples,0,nSeg*2*2);
				}
				else
				{
					ApplyRamp16((XS16*)samples, nSeg, 2, level, dLevel);
					ApplyRamp16((XS16*)samples+1, nSeg, 2, level, dLevel);
				}
			}
		}
		else
		{
			// Merge the two envelopes for a mono envelope

			if ( snd.Is8Bit() )
			{
				if(!v)
					memset(samples,0,nSeg);
				else
					ApplyRamp8((XU8*)samples, nSeg, 1, level, dLevel);
			}
			else
			{
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
	//LOGWHERE();
	Init();
}

void XSoundChannel::Init()
{
	//LOGWHERE();
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
	//LOGWHERE();
	if ( mixer )
	{
		AddRef(); // prevent a recursive delete...
		mixer->RemoveSound(this);
		Release();
	}
	if ( decomp )
	{
		delete decomp;
		decomp = NULL;
	}
}

XBOOL XSoundChannel::CreateDecompressor()
{
	//LOGWHERE();
	if(decomp)
		return XTRUE;
	switch(sound->CompressFormat())
	{
		default:
			 decomp=new XISETDecomp();
			 break;
		case sndCompressNone:
        case sndCompressNoneI:
			 decomp=new XIPCMDecomp();
			 break;
		case sndCompressADPCM:
			 decomp=new XIADPCMDecomp();
			 break;
		case sndCompressMP3:
			decomp=new XMp3Decomp();
			break;
	}
	return XTRUE;
}

void XSoundChannel::Stop()
{
	//LOGWHERE();
	if ( mixer )
		mixer->RemoveSound(this);
}

XS32 XSoundChannel::ElapsedFrames()
{
	//LOGWHERE();
	if ( !mixer || !sound->nSamples )
	{
		return 0;
	}

	// Get the two values we want as quickly as possible so it is unlikely that we get interupted here...
	// We might need a critical section here...
	int sp = samplesPlayed;
	XS32 ct = bufferCompletionTime;

	// get an adjustment for the elapsed time into the current buffer
	XS32 adj = XTime::GetTimeMSec()-ct;

	if ( adj > 63 )
		adj = 63;
	adj *= 44;

	// convert from msec to 44khz units
	//if ( adj*44 > mixer->SOUND_SAMPLES44 ) {
	//	adj = mixer->SOUND_SAMPLES44/44;
	//}

	if ( streamStalled )
		adj = 0;

	// Calculate the elapsed frames
	return (sp + (adj>>sound->RateShift()) + sound->nSamples/2)/sound->nSamples;
}


void XSoundChannel::AddRef()
{
	//LOGWHERE();
	refCount++;
}

void XSoundChannel::Release()
{
	//LOGWHERE();
	if ( --refCount == 0 )
		delete this;
}

///////////////////////////////////////////////////////////////////////////
//XSound::XSound()
//{
//	LOGWHERE();
//	format = 0;
//	nSamples = 0;
//	samples = 0;
//	dataLen = 0;
//    delay = 0;
//}
//XSound::~XSound()
//{
//	LOGWHERE();
//	format = 0;
//	nSamples = 0;
//	samples = 0;
//	dataLen = 0;
//    delay = 0;
//}
void XSound::Init()
{
	format = 0;
	nSamples = 0;
	samples = 0;
	dataLen = 0;
    delay = 0;
}

XS32 XSound::Rate()
{
	return kRateTable[(format >> 2)&0x3];
}

int XSound::RateShift()
{
	return kRateShiftTable[(format >> 2)&0x3];
}

XBOOL XSound::Stereo()
{
	return (format & sndStereo) != 0;
}

int XSound::NChannels()
{
	return (format & sndStereo) ? 2 : 1;
}
XBOOL XSound::Is8Bit()
{
	return (format & snd16Bit) == 0;
}

int XSound::BitsPerSample()
{
	return (format & snd16Bit) ? 16 : 8;
}

int XSound::BytesPerSample()
{
	if( this == NULL )
		return 1;
	//LOGWHEREVAL( (int)format);
	//return 1;
	return (format & snd16Bit) ? 2 : 1;
}

int XSound::CompressFormat()
{
	return format & sndCompressMask;
}

XBOOL XSound::Compressed()
{
	return (format & sndCompressMask) != 0;
}

// Manage the duration in 44kHz units
XS32 XSound::GetDuration44()
{
	return nSamples << RateShift();
}

void XSound::SetDuration44(XS32 d)
{
	nSamples = d >> RateShift();
}

int XSound::BytesPerBlock()
{
	if( this == NULL )
		return 0;
	int bytes = (format & snd16Bit) ? 2 : 1;
	int channel = (format & sndStereo) ? 2 : 1;
	return bytes * channel;
}

XS32 XSound::SizeBytes()
{
	return nSamples * BytesPerBlock();
}
////////////////////////////////////////////////////////////////////////////

XUINT XSoundMix::CallBack(XU32 nID)
{
	LOGWHERESOUND();
	int i = 0;
	this->DoPlay();
	return nID;
}

void XSoundMix::SetSilence( bool bsilence )
{
	mbSilence = bsilence;
}

void XSoundMix::Stop()
{
	playThread.Stop( true );
}

void XSoundMix::SetEvent()
{
	playEvent.SetEvent();
}

void XSoundMix::NativeConstruct()
{
	LOGWHERESOUND();
	playEvent.Create();
	isOpen = false;
	mbSilence = true;
	memset(&waveHDR, 0, sizeof(_XWAVEHDR) * MAX_SOUND_BUFFERS);
	ReSetAudio();
}

void XSoundMix::ReSetAudio()
{
	LOGWHERESOUND();
	m_pAudioTrack = XNULL;
	mpdlopen = NULL;

	mFnAudioCreate = NULL;
	mFnAudioCreateAT = NULL;
	mFnAudioStart = NULL;
	mFnAudioStop = NULL;
	mFnAudioDestroy = NULL;
	mFnAudioWrite = NULL;
	mFnAudioSetVolume = NULL;
	mFnAudioGetVolume = NULL;
}

void XSoundMix::NativeDestruct()
{
	LOGWHERESOUND();
	playThread.Stop( true );
	playEvent.Release();
	XSoundChannel*c=this->firstChannel;
	while(c)
	{
		XSoundChannel*next=c->next;
		if(c->completeStop)
			delete c;
		c=next;
	}
//	if( m_pAudioTrack != XNULL )
//	{
//		delete (AudioTrack*)m_pAudioTrack;
//		m_pAudioTrack = XNULL;
//	}
	if( mpdlopen != NULL )
	{
		isOpen = false;	// zhengjl 2012.05.25
		mFnAudioStop();
		mFnAudioDestroy();
		dlclose(mpdlopen);
		ReSetAudio();
	}

}

void XSoundMix::GetNativeSoundCap( XS32* format )
{
	LOGWHERESOUND();
	*format = sndStereo | snd16Bit | snd22K;
}


void XSoundMix::SetPause(XBOOL b)
{
	LOGWHERESOUND();
	bPause = b;
//	if( bPause )
//	{
//		((AudioTrack*)m_pAudioTrack)->stop();
//	}
//	else
//	{
//		((AudioTrack*)m_pAudioTrack)->start();
//	}

	if( bPause )
	{
		if( mFnAudioStop != NULL )
			mFnAudioStop();
	}
	else
	{
		if( mFnAudioStart != NULL )
			mFnAudioStart();
	}

	if(!b)
	{
		playEvent.SetEvent();
	}
}

XBOOL XSoundMix::NeedStop()
{
	LOGWHERESOUND();
	return playThread.NeedStop(0);
}

static void MixCallbackAT(int event, void* user, void *info)
{
	LOGWHERESOUND();
	if( event == AudioTrack::EVENT_MORE_DATA )
	{
		XSoundMix* mixer = (XSoundMix*)user;
		AudioTrack::Buffer* buff = (AudioTrack::Buffer*)info;
		mixer->MakeDataAT( buff );
	}
	return;
}

//
void  XSoundMix::MakeDataAT(void *bufdata)
{
	LOGWHERESOUND();
	AudioTrack::Buffer* buff = (AudioTrack::Buffer*)bufdata;
	static int buffIndex = 0;
	static int buffWrite = 0;

	if( bPause )
	{
		buff->size = 0;
		XThread::Sleep( 1000 );
		return;
	}

	if (buffIndex >= nBuffers)
	{
		buffIndex = 0;
	}
	while  (waveHDR[buffIndex].dwFlags != WHDR_PREPARED)
	{
		buffIndex++;
		if (buffIndex >= nBuffers)
		{
			buffIndex = 0;
		}
	}

	char * tmp = (char*)buffer[buffIndex];
	int len = bufferBytes - buffWrite;
	if (len > buff->size)
	{
		memcpy(buff->raw, tmp + buffWrite , buff->size);
		buffWrite += buff->size;
	}
	else
	{
		memcpy(buff->raw, tmp + buffWrite, len);
		buff->size = len;
		buffWrite = 0;
		MarkBufferComplete(buffIndex++);
	}
	//SetEvent();
	return;

}

static void MixCallbackSL(void* bq, void *soundMix)
{
	LOGWHERESOUND();
	XSoundMix* mixer = (XSoundMix*)soundMix;
	mixer->MakeDataSL( );

	return;
}

void XSoundMix::MakeDataSL()
{
	static int buffIndex = 0;

	if (buffIndex >= nBuffers)
	{
		buffIndex = 0;
	}
	while  (waveHDR[buffIndex].dwFlags != WHDR_PREPARED)
	{
		//LOGWHERE();
		buffIndex++;
		if (buffIndex >= nBuffers)
		{
			buffIndex = 0;
		}
	}
	if (isOpen)	// zhengjl 2012.05.25
	{
		mFnAudioWrite( buffer[buffIndex], bufferBytes );
	}

	// zhengjl 2012.06
	MarkBufferComplete(buffIndex++);
	//SetEvent();

}

XBOOL XSoundMix::OpenNativeDevice( XSound* format )
{
	LOGWHERESOUND();

	if ( isOpen )
	{
		return true;
	}

	if( mpdlopen != NULL )
	{
		dlclose(mpdlopen);
		ReSetAudio();
	}

	if (GFunGetClientVersion() <= 8)
	{
		mpdlopen=dlopen( BWLIBLOW,RTLD_NOW);
	}
	else
	{
		mpdlopen=dlopen( BWLIBHEIGHT,RTLD_NOW);
	}
	mFnAudioCreate =	(fnAudioCreate)dlsym	(mpdlopen,"BwAudioCreate");
	mFnAudioCreateAT =	(fnAudioCreateAT)dlsym	(mpdlopen,"BwAudioCreateAT");
	mFnAudioCreateSL =	(fnAudioCreateSL)dlsym	(mpdlopen,"BwAudioCreateSL");
	mFnAudioStart = 	(fnAudioStart) dlsym	(mpdlopen,"BwAudioStart");
	mFnAudioStop = 		(fnAudioStop)dlsym		(mpdlopen,"BwAudioStop");
	mFnAudioDestroy=	(fnAudioDestroy)dlsym	(mpdlopen,"BwAudioDestroy");
	mFnAudioWrite=		(fnAudioWrite)dlsym		(mpdlopen,"BwAudioWrite");
	mFnAudioSetVolume=	(fnAudioSetVolume)dlsym	(mpdlopen,"BwAudioSetVolume");
	mFnAudioGetVolume=	(fnAudioGetVolume)dlsym	(mpdlopen,"BwAudioGetVolume");

	LOGWHERE();

	if( mFnAudioCreateSL != NULL )
	{
		if (GFunGetClientVersion() <= 8)
			mFnAudioCreateAT(MixCallbackAT, this, format->Rate() );
		else
			mFnAudioCreateSL(MixCallbackSL, this, format->Rate(),format->NChannels(), format->BitsPerSample() );
		mFnAudioStart();
		mbSilence = false;
		isOpen = true;
		LOGWHERE();

	}
	else
	{
		LOGWHERE();
		mbSilence = true;
		isOpen = false;
	}
	return isOpen;



//	m_pAudioTrack = new AudioTrack(
//			AudioSystem::MUSIC,// stream type
//			format->Rate(),
//			AudioSystem::PCM_16_BIT,// word length, PCM
//			AudioSystem::CHANNEL_OUT_STEREO,
//			0, 0, MixCallbackAT, this, 0 );
//
//	if( m_pAudioTrack != XNULL )
//	{
//		//((AudioTrack*)m_pAudioTrack)->initCheck();
//		//((AudioTrack*)m_pAudioTrack)->setVolume(1.0, 1.0);
//		((AudioTrack*)m_pAudioTrack)->start();
//		isOpen = true;
//	}
//	else
//	{
//		isOpen = false;
//	}
//	return isOpen;
}

void XSoundMix::CloseNativeDevice()
{
	LOGWHERESOUND();

	if ( !isOpen )
		return;

	playThread.Stop( true );

	if ( ! m_playLock.Lock(XSHORTTIME))
		return;
	if( mpdlopen != NULL )
	{
		isOpen = false;	// zhengjl 2012.05.25
		mFnAudioStop();
		mFnAudioDestroy();
		dlclose(mpdlopen);
		ReSetAudio();
	}
//	((AudioTrack*)m_pAudioTrack)->stop();
//	delete (AudioTrack*)m_pAudioTrack;
	isOpen = false;
	mbSilence = true;
	playThread.Release(XFALSE);
	m_playLock.UnLock();
	LOGWHERESOUND();
}

void XSoundMix::BuffersReady()
{
	LOGWHERESOUND();
	//playThread.Create(this,0);
	mFnAudioWrite( buffer[0], bufferBytes );
}

void XSoundMix::DoPlay()
{
	LOGWHERESOUND();
	mFnAudioWrite( buffer[0], bufferBytes );
	while (XTRUE)
	{
		LOGWHERE();
		if(playThread.NeedStop())
			break;
		if(!playEvent.IsEvent(1000 ))
			continue;
		playEvent.ResetEvent();

		if(bPause)
		{
			XThread::Sleep(XSHORTTIME);
			continue;
		}

		if ( !IsDeviceOpen() )
		{
			return;
		}

//		m_playLock.Lock(XSHORTTIME);
//		int nSent = 0;
//		for ( int i = 0; i < nBuffers; i++ )
//		{
//			LOGWHERE();
//			_XWAVEHDR *hdr = (_XWAVEHDR*)(waveHDR+i);
//			if ( (hdr->dwFlags & WHDR_DONE) || !(hdr->dwFlags & WHDR_PREPARED) )
//			{
//				if ( buffer[i] )
//				{
//					LOGWHERE();
//					BuildAndWrite( &waveHdr[i], false );
//					LOGWHERE();
//				}
//				nSent++;
//			}
//			LOGWHERE();
//		}
//
//		if ( nSent > nBuffers-2 )
//			EnlargeBuffers();
//		m_playLock.UnLock();
//
//		LOGWHERE();

		if ( nSilent > nBuffers )
		{
			XThread::Sleep(20);
			break;
		}
		LOGWHERE();
	} // end of while

}

void XSoundMix::BuildAndWrite( _XWaveHeader* wave,XBOOL silence )
{
	LOGWHERESOUND();
	int i = wave->bufferId;
	_XWAVEHDR* hdr = (_XWAVEHDR*)&waveHDR[i];
	hdr->dwFlags = 0;
	if ( silence )
	{
		memset(buffer[i], mixFormat.Is8Bit() ? 0x80 : 0, bufferBytes);
	}
	else
	{
		BuildBuffer( i );
	}

//	int size = 0;
//	while (size < bufferBytes)
//	{
//		size += mFnAudioWrite(m_pAudioTrack,buffer[i],bufferBytes);
//	}
//
//	SetEvent();
//	return;

	hdr->dwFlags = WHDR_PREPARED;
	return;
}

void XSoundMix::BuffersUpdated()
{
	LOGWHERESOUND();
	for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
	{
		waveHDR[i].lpData	= (char*) buffer[i];
		waveHDR[i].dwUser	= i;			// Put in the buffer id in the user space
		waveHDR[i].dwBufferLength = bufferBytes;
	}
}

void XSoundMix::MarkBufferComplete(int i)
{
	LOGWHERESOUND();
	XS32 bufferCompletionTime = XTime::GetTimeMSec();

	for ( XSoundChannel* c = firstChannel; c; c = c->next )
	{
		c->samplesPlayed += c->bufferCounts[i];
		c->bufferCounts[i] = 0;
		c->bufferCompletionTime = bufferCompletionTime;
	}
	//_XWAVEHDR* hdr = (_XWAVEHDR*)(waveHDR+i);
	//hdr->dwFlags = WHDR_DONE;
	//waveHDR[i].dwFlags = WHDR_DONE;
	BuildAndWrite( &waveHdr[i], false );
	return;
}

void XSoundMix::AddSound(XSoundChannel *c)
{
	LOGWHERESOUND();
	if(!c->CreateDecompressor())
		return;
	if ( c->streamProc )
	{
		for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
			c->bufferCounts[i] = 0;
		c->bufferCompletionTime = XTime::GetTimeMSec();

	}
	else
	{
		XSound* s = c->sound;
		if ( c->syncFlags & syncStop )
		{
			// Stop the sound
			FreeSound(s);
			return;
		}
		else if ( (c->syncFlags & syncNoMultiple) && SoundPlaying(s) )
		{
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


	if(m_playLock.Lock(XSHORTTIME)) //return;
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

		// Open the device when we first need it...
		if ( !IsDeviceOpen() )
		{
			OpenDevice();
			if ( !IsDeviceOpen() )
				RemoveAll();
		}
		m_playLock.UnLock();
	}
	return;
}

void XSoundMix::RemoveSound(XSoundChannel *c)
{
	LOGWHERESOUND();
	c->remove = XTRUE;
	PurgeList();
}

void XSoundMix::EnlargeBuffers()
{
	LOGWHERESOUND();
	if ( nBuffers < MAX_SOUND_BUFFERS )
	{
		if ( buffer[nBuffers] == 0 )
		{
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
	LOGWHERESOUND();
	if(!m_playLock.Lock(XSHORTTIME))
		return;

	XSoundChannel* c = firstChannel;
	while(c)
	{
		XSoundChannel*next=c->next;
		if(!c->remove) //continue;
		{
			if(c->completeProc)
				c->completeProc(c->pArgs,false);
			c->remove=XTRUE;
		}
		c=next;
	}
	m_playLock.UnLock();
}

void XSoundMix::BuildBuffer(int iBuf)
{
	int mixN = 0;
	void* mixBuf[MAX_SOUND_CHANNELS];

	bool silent = true;
	for ( XSoundChannel* c = firstChannel; c; c = c->next )
	{
		if ( c->remove )
		{
			continue;	// This channel is finished and waiting to be removed
		}
		silent = false;
		// Set up the buffer
		void* buf = 0;
		if ( mixN < MAX_SOUND_CHANNELS )
		{
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

		if ( c->streamProc )
		{
			c->bufferCounts[iBuf] = 0;
			while ( n > 0 )
			{
//				if(playThread.NeedStop())
//					break;
				XS32 nc = XMIN(n, c->blockSamples);
				if ( nc > 0)
				{

					if ( c->silentBlock )
					{
						if ( b )
							memset(b, 0x00, nc*bytesPerBlock);
					}
					else
					{
						if( c != NULL && c->decomp != NULL )
						{
							c->decomp->Decompress((XS16*)b, nc);
							if( mbSilence ||c->nVolume==0||c->m_pTransform&&c->m_pTransform->m_dVolume==0)
								memset(b,0x00,nc*bytesPerBlock);
						}
					}
					if ( !c->streamStalled )
						c->bufferCounts[iBuf] += nc;	// we're playing these samples from the source stream...

					if ( b )
						b += nc*bytesPerBlock;
					n -= nc;
					c->blockSamples -= nc;
				}
				else
				{
					// We are out of samples, get a new block
					c->streamStalled = !c->streamProc(c);
					if(c->streamEnd)
					{
						if ( --c->loops <= 0 )
						{
							if(c->completeProc)
								c->completeProc(c->pArgs,false);
							c->remove = true;
							break;
						}
						else
							c->streamEnd=false;
					}

					if ( c->blockSamples <= 0)
					{
						// We did not get any data, create a silent block
						if (c->blockCanBeZero)
						{
							c->blockCanBeZero = false;
							if (c->seekSamples < 0)
							{
								c->blockSamples = -c->seekSamples;
								c->seekSamples = 0;
							}
						}
						else
						{
							c->blockSamples = s->nSamples;
						}
						c->silentBlock = true;
					}
					else
					{
						// Play back this block of data
						c->silentBlock = false;
					}
				}
			}

			if ( c->fadeIn )
			{
				c->fadeIn = false;
				_XSoundEnvelope fader;
				fader.nPoints = 2;
				fader.points[0].mark44 = 0;
				fader.points[0].level0 = fader.points[0].level1 = 0;
				fader.points[1].mark44 = XMIN(SOUND_SAMPLES44, sndRate44K/4);
				fader.points[1].level0 = fader.points[1].level1 = sndMaxLevel;
				if ( buf )
				{
					ApplyEnvelope(c->sound->format, buf, SOUND_SAMPLES44, 0, &fader);
				}
			}

		}
		else
		{
			// Get samples from an event sound
			while ( n > 0 )
			{
				XS32 nc = XMIN(n, c->outPos - c->pos);
				if ( nc <= 0 )
				{
					// We are out of samples
					if ( --c->loops > 0 )
					{
						// Loop the sound
						c->pos = c->inPoint44>>rs;

						// Setup should be the same as a rewind operation for the decompressor - Warning we cannot do memory allocations here on Mac...
						c->decomp->Setup(s, true);
						c->decomp->Decompress(0, c->pos);	// skip

					}
					else
					{
						// Remove the channel and pad w/ silence
						if ( b )
							memset(b, s->Is8Bit() ? 0x80 : 0x00, n*bytesPerBlock);

						if(c->completeProc)
							c->completeProc(c->pArgs,false);
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

		//SetVolume(c->nVolume);
		// Convert the sound to the mixer format
		if ( buf )
		{
			// Save the last sample from this buffer for interpolating with the next buffer
			XS32 savedSample;
			memcpy(&savedSample, (XU8*)buf+bytesPerBlock*((SOUND_SAMPLES44>>rs)-1), bytesPerBlock);

			if ( !c->prevSampleInited )
			{
				// For the first buffer, just use the first sample of this buffer
				memcpy(&c->prevSample, buf, bytesPerBlock);
				c->prevSampleInited = true;
			}

			ConvertSound(buf, SOUND_SAMPLES44, s->format&~sndCompressMask, mixFormat.format, &c->prevSample);
			c->prevSample = savedSample;
		}

		// Apply a volume envelope
		if ( c->envelope.nPoints > 0 )
		{
			if ( buf )
				ApplyEnvelope(mixFormat.format, buf, SOUND_SAMPLES44, c->envelopePos44, &c->envelope);
			c->envelopePos44 += SOUND_SAMPLES44;
		}
		if(buf)
		{
			if(c->nVolume>=0)
				SetChannelVolume(mixFormat.format, buf, SOUND_SAMPLES44, c->nVolume);
			if(c->m_pTransform)
				ApplyTransform(mixFormat.format,buf,SOUND_SAMPLES44,c->m_pTransform);
		}
	}

	if ( silent )
		nSilent++;
	else
		nSilent = 0;

	// Add the sound channels together
	if ( mixFormat.Is8Bit() )
	{
		// Combine the sounds into our buffer
		// Note this changes the mixBuf pointers
		XU8* buf = (XU8*)buffer[iBuf];
		if ( mixN == 0 )
		{
			// Set the buffer to silence
			memset(buf, 0x80, bufferBytes);
			//memset(buf, 0x10, bufferBytes/2); // a test square wave...

		}
		else
		{
			if ( mixN == 1 )
			{
				// Copy a single sound
				memcpy(buf, mixBuf[0], bufferBytes);

			}
			else if ( mixN == 2 )
			{
				// Mix 2 sounds
				XU8* src1 = (XU8*)mixBuf[0];
				XU8* src2 = (XU8*)mixBuf[1];
				XU8* dst = buf;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- )
				{
					int s = (int)*src1++ + *src2++ - 0x80;
					if ( s & 0xFFFF00 )
					{
						// Overflow or underflow
						*dst++ = s < 0 ? 0 : 0xff;
					}
					else
					{
						*dst++ = (XU8)s;
					}
				}

			}
			else
			{
				// Mix multiple sounds
				XU8* src[MAX_SOUND_CHANNELS];
				memcpy(src, mixBuf, sizeof(void*)*mixN);

				XU8* dst = buf;
				int offset = (mixN-1) * -0x80;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- )
				{
					int s = offset;
					for ( int j = mixN; j--; )
					{
						s += *(src[j]++);
					}
					if ( s & 0xFFFF00 )
					{
						// Overflow or underflow
						*dst++ = s < 0 ? 0 : 0xff;
					}
					else
					{
						*dst++ = (XU8)s;
					}
				}
			}
		}
	}
	else
	{
		// Combine the sounds into our buffer
		// Note this changes the mixBuf pointers
		XS16* buf = (XS16*)buffer[iBuf];
		if ( mixN == 0 )
		{
			// Set the buffer to silence
			memset(buf, 0, bufferBytes);
		}
		else
		{
			if ( mixN == 1 )
			{
				// Copy a single sound
				memcpy(buf, mixBuf[0], bufferBytes);

			}
			else if ( mixN == 2 )
			{
				// Mix 2 sounds
				XS16* src1 = (XS16*)mixBuf[0];
				XS16* src2 = (XS16*)mixBuf[1];
				XS16* dst = buf;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- )
				{
					XS32 s = (XS32)*src1++ + *src2++;
					if ( (XS16)s != s )
					{
						// Overflow or underflow - if the high 16 bits are not all 0 or all 1
						*dst++ = s < 0 ? (int)-32768 : (int)32767;
					}
					else
					{
						*dst++ = (XS16)s;
					}
				}

			}
			else
			{
				// Mix multiple sounds
				XS16* src[MAX_SOUND_CHANNELS];
				memcpy(src, mixBuf, sizeof(void*)*mixN);

				XS16* dst = buf;
				XS32 i = bufferSamples;
				if ( mixFormat.Stereo() ) i*=2;
				while ( i-- )
				{
					XS32 s = 0;
					for ( int j = mixN; j--; )
					{
						s += *(src[j]++);
					}
					if ( (XS16)s != s )
					{
						// Overflow or underflow - if the high 16 bits are not all 0 or all 1
						*dst++ = s < 0 ? (int)-32768 : (int)32767;
					}
					else
					{
						*dst++ = (XS16)s;
					}
				}
			}
		}
	}
//  三国魂退出重新进入会造成程序崩溃
//	if(m_pTransformn)
//	{
//		ApplyTransform(mixFormat.format,buffer[iBuf],SOUND_SAMPLES44,m_pTransformn);
//	}

	ReleaseScratch();	// mark the scratch buffers as unused
}

void XSoundMix::FreeSound(XSound *s)
{
	LOGWHERESOUND();
	for ( XSoundChannel* c = firstChannel; c; c = c->next )
	{
		if ( c->sound == s )
			c->remove = true;
	}
	PurgeList();
}

XBOOL XSoundMix::SoundPlaying(XSound *s)
{
	LOGWHERESOUND();
	XBOOL playing = false;
	if(!m_playLock.Lock(XSHORTTIME))
		return true;
	for ( XSoundChannel* c = firstChannel; c; c = c->next )
	{
		if ( c->sound == s && !c->remove )
		{
			playing = true;
			break;
		}
	}
	m_playLock.UnLock();
	return playing;
}

void XSoundMix::PurgeList()
{
	LOGWHERESOUND();

	if(!m_playLock.Lock(XSHORTTIME))
		return;
	XSoundChannel** l = &firstChannel;
	while ( *l )
	{
		XSoundChannel* c = *l;
		if ( c->remove )
		{
			// Remove the channel
			*l = c->next;
			c->remove = false;
			c->next = 0;
			c->mixer = 0;
			if(c->completeStop)
				delete c;
			else
				c->Release();
		}
		else
		{
			// Advance to the next channel
			l = &c->next;
		}
	}
	m_playLock.UnLock();

}

void XSoundMix::OpenDevice()
{
	LOGWHERESOUND();
	if ( IsDeviceOpen() )
		return;

	if(!m_playLock.Lock(XSHORTTIME))
	{
		return;
	}
	{
		GetNativeSoundCap( &mixFormat.format );
		int okay = OpenNativeDevice( &mixFormat );
		if ( !okay )
			goto Fail; // something is wrong...
		nSilent = 0;

		AllocBuffers();
		if ( !buffer[0] || !buffer[1] )
		{
			CloseNativeDevice();
			goto Fail;
		}
		// Prime the stream, send first two buffers as silence to prevent burps
		int nSilence = firstPlay ? 4 : 2;
		firstPlay = false;
		for ( int i = 0; i < nBuffers; i++ )
		{
			if ( buffer[i] )
			{
				BuildAndWrite( &waveHdr[i], i < nSilence );
			}
		}

		BuffersReady();
	}

Fail:
	m_playLock.UnLock();
	return;

}

void XSoundMix::RemoveAll()
{
	LOGWHERESOUND();
	for ( XSoundChannel* c = firstChannel; c; c = c->next )
		c->remove = true;

	PurgeList();
}

void* XSoundMix::GetScratch()
{
	LOGWHERESOUND();
	// The mac can't allocate these at interupt time, they better already be available
	if ( !scratchBuf[nextScratch] ) {
		scratchBuf[nextScratch] = new XU8[scratchBytes];
	}
	return scratchBuf[nextScratch++];
}

void XSoundMix::AllocBuffers()
{
	LOGWHERESOUND();
	XS32 nSamples = SOUND_SAMPLES44>>mixFormat.RateShift();
	if ( buffer[0] && nSamples == bufferSamples )
		return;
	FreeBuffers();

	bufferDuration = (SOUND_SAMPLES44*10)/441;

	bufferSamples = nSamples;
	bufferBytes = mixFormat.BytesPerBlock()*nSamples;

	for ( int i = 0; i < nBuffers; i++ )
	{
		buffer[i] = new XU8[bufferBytes];
	}

	// We allocate scratch buffers large enough to hold 44kHz 16 bit stereo sound
	//	so that we have space for any format conversion
	scratchBytes = SOUND_SAMPLES44*4;

	// Set up the wave headers
	for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
	{
		memset(waveHdr+i, 0, sizeof(_XWaveHeader));
		waveHdr[i].data			= (XU8*)buffer[i];
		waveHdr[i].bufferId		= i;//(U32)this;
	}
	BuffersUpdated();
}

void XSoundMix::FreeBuffers()
{
	LOGWHERESOUND();
	for ( int i = 0; i < MAX_SOUND_BUFFERS; i++ )
	{
		delete[] (XU8*)buffer[i];
		buffer[i] = 0;
	}
	for ( int i = 0; i < MAX_SOUND_CHANNELS; i++ )
	{
		delete[] (XU8*)scratchBuf[i];
		scratchBuf[i] = 0;
	}
	return;
}


void XSoundMix::Construct()
{
	LOGWHERESOUND();

	mixFormat.Init();
	firstPlay = true;
	nVolume=0xafffafff;
	firstChannel = 0;
	nBuffers = INITIAL_SOUND_BUFFERS;
	bufferDuration = SOUND_SAMPLES44*10/441;
		//FixedDiv(SOUND_SAMPLES44, FloatToFixed(44.1));
	nSilent = 0;
	bPause = XFALSE;
	bufferSamples = 0;
	nextScratch = 0;

	memset( buffer, 0, MAX_SOUND_BUFFERS);
	memset( scratchBuf, 0, MAX_SOUND_CHANNELS);

	NativeConstruct();
}

void XSoundMix::Destruct()
{
	LOGWHERESOUND();
	RemoveAll();
	CloseNativeDevice();
	FreeBuffers();
}

XBOOL XSoundMix::SetVolume(int v)
{
	LOGWHERESOUND();
	if(v>0xffff)
		v=0xffff;
	else if(v<0)
		v=0;
	//v=v*0xff/100;
	nVolume=(v<<16)|v;
	if(!IsDeviceOpen())
		return XFALSE;

	mFnAudioSetVolume(v, v );
	//	((AudioTrack*)m_pAudioTrack)->setVolume( v, v );
	return XTRUE;
}

int XSoundMix::GetVolume()
{
	LOGWHERESOUND();
	if(!IsDeviceOpen())
		return nVolume;
	XU32 v=nVolume;
	float left =0;
	float right =0;

	mFnAudioGetVolume(&left, &right);

//	((AudioTrack*)m_pAudioTrack)->getVolume( &left, &right );
	v = left;  //v = value * 65535;
	return v;
}



