// XMp3Decomp.h: interface for the XMp3Decomp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMP3DECOMP_H__F3A3D473_1947_4A59_B7F3_81A9982315ED__INCLUDED_)
#define AFX_XMP3DECOMP_H__F3A3D473_1947_4A59_B7F3_81A9982315ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define __MAD

#ifdef __MAD
#include "mad.h"
#endif

#include "XNativeSoundMix.h"

const int kBufSize = 1024*8;

class XMp3Decomp : public XSoundDecomp
{
private:
    XU8              *src;               // mpeg src data
    int             srcIndex;           // index into the source data to read
    XU8              pcmBuf[kBufSize];   // buffered output data 
    void*		    mp3Handle;          // handle, identifying the decoder instance
    int             bufLength;          // number of bytes last filled with
    int             bufIndex;           // index into the out buffer
	int				blockSamples;
    XSound          *snd;

    // get buffered data in bufOut. Return number of bytes written
    long GetBufferedData(XS8 *dst, XS32 n);
#ifdef __MAD
private:
	struct mad_stream	Stream;
	struct mad_frame	Frame;
	struct mad_synth	Synth;
	mad_timer_t			Timer;


	unsigned char tempBuf[kBufSize];

	bool fillBufferMad();
	void initMad();
	void finishMad();
	bool isInitMad() const {
		return m_isInitMad;
	}
	bool m_isInitMad;
	bool m_firstBuf;
#endif
public:
	XMp3Decomp(void);
    ~XMp3Decomp(void);

    virtual void Setup(XSound* snd, XBOOL reset = false);
    void Decompress(XS16* dst, XS32 nSamples);    // return number of good samples
};


#endif // !defined(AFX_XMP3DECOMP_H__F3A3D473_1947_4A59_B7F3_81A9982315ED__INCLUDED_)
