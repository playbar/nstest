// XWaveIn.cpp: implementation of the XWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XWaveIn.h"
#include "XMicrophone.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XWaveIn::XWaveIn(XMicrophone*p)
{
	//    WORD        wFormatTag;         /* format type */
    //WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    //DWORD       nSamplesPerSec;     /* sample rate */
    //DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    //WORD        nBlockAlign;        /* block size of data */
    //WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    //WORD        cbSize;             /* the count in bytes of the size of */
				    /* extra information (after cbSize) */
	m_nBlocks = 0;
	m_pMicrophone=p;
	isRunning = false;
	m_waveFmt.cbSize=0;
	m_waveFmt.nAvgBytesPerSec;
	m_waveFmt.nChannels=1;
	m_waveFmt.wBitsPerSample=8;
	m_waveFmt.nSamplesPerSec=XPCM_22K;
	m_waveFmt.nBlockAlign=m_waveFmt.nChannels*m_waveFmt.wBitsPerSample/8;
	m_waveFmt.nAvgBytesPerSec=m_waveFmt.nSamplesPerSec*m_waveFmt.nChannels*m_waveFmt.wBitsPerSample/8;
	m_waveFmt.nBlockAlign=m_waveFmt.nChannels*m_waveFmt.wBitsPerSample/8;
	m_waveFmt.wFormatTag=WAVE_FORMAT_PCM;
	m_handle = NULL;
	memset(&m_header1,0,sizeof(m_header1));
	memset(&m_header2,0,sizeof(m_header2));
	//m_nBufferSize=m_waveFmt.nAvgBytesPerSec/100;
}

XWaveIn::~XWaveIn()
{
	Stop();
}

static void CALLBACK CallBack(
  HWAVEIN hwi,       
  UINT uMsg,         
  DWORD dwInstance,  
  DWORD dwParam1,    
  DWORD dwParam2     
)
{
	if(!dwParam1) return;
	XWaveIn* pWave=(XWaveIn*)dwInstance;
	switch(uMsg)
	{
	case WIM_OPEN:
		 break;
	case WIM_CLOSE:
		 break;
	case WIM_DATA:
		{
			WAVEHDR*pHeader=(WAVEHDR*)dwParam1;
			//pHeader->dwFlags=WHDR_PREPARED;
			//pHeader->dwBytesRecorded=0;
			pWave->WaveIn(pHeader);
		}
		 break;
	}
}

bool XWaveIn::GetName(int id,XString8&name)
{
	WAVEINCAPS cap;
	HRESULT hr=::waveInGetDevCaps(id,&cap,sizeof(cap));
	if(hr==MMSYSERR_NOERROR)
	{
		name=cap.szPname;
		return true;
	}
	return false;
	//	list.Add(str);
}

int XWaveIn::GetNames(XString8Array &list)
{
	int nums=waveInGetNumDevs();
	int i;
	for(i=0;i<nums;i++)
	{
		WAVEINCAPS cap;
		::waveInGetDevCaps(i,&cap,sizeof(cap));
		XString8 str(cap.szPname);
		list.Add(str);
	}
	return nums;
}

bool XWaveIn::Start()
{
	 if(isRunning) return false;
//	 m_nBufferSize=m_waveFmt.nChannels*m_waveFmt.wBitsPerSample/8*100;
		 //.nAvgBytesPerSec/10;
	 //PrePareHeader(m_header1);
	 //int nums=waveInGetNumDevs();

	 HRESULT hr;
	 //WAVEFORMATEX fmt;
	 //memset(&fmt,0,sizeof(fmt));
	// hr= waveInOpen (NULL, WAVE_MAPPER,
	//				 &m_waveFmt, 0, 0, WAVE_FORMAT_QUERY);

	 hr=::waveInOpen(&m_handle,WAVE_MAPPER,&m_waveFmt,(DWORD)CallBack,(DWORD)this,CALLBACK_FUNCTION);
	 if(hr!=MMSYSERR_NOERROR)
		 return false;
	 PrePareHeader(m_header1,m_buffer1);
	 PrePareHeader(m_header2,m_buffer2);
	 isRunning=true;
	 hr=::waveInStart(m_handle);
	 return true;
}

bool XWaveIn::Stop()
{
	if(!isRunning) return false;
	//if(m_header1.lpData)
	//	delete m_header1.lpData;
	//if(m_header2.lpData)
	//	delete m_header2.lpData;
	::waveInUnprepareHeader(m_handle,&m_header1,sizeof(WAVEHDR));
	::waveInUnprepareHeader(m_handle,&m_header2,sizeof(WAVEHDR));
	::waveInStop(m_handle);
	::waveInClose(m_handle);
	m_handle=false;
	isRunning=false;
	return true;
}

void XWaveIn::PrePareHeader(WAVEHDR &head,XU8*pBuf)
{
	if(!m_handle) return;
	//if(head.lpData)
	//	delete head.lpData;
	head.lpData=NULL;
	head.dwBufferLength=WAVEBUFSIZE;//m_nBufferSize;
	head.dwBytesRecorded=0;
	head.dwFlags=0;
	head.dwLoops=WHDR_DONE;
	head.dwUser=0;
	head.lpData=(LPSTR)pBuf;//new char[m_nBufferSize];
	head.lpNext=NULL;
	head.reserved=0;
	HRESULT hr=::waveInPrepareHeader(m_handle,&head,sizeof(WAVEHDR));
	hr=::waveInAddBuffer(m_handle,&head,sizeof(WAVEHDR));

}

void XWaveIn::WaveIn(WAVEHDR *pHeader)
{
	if(m_pMicrophone)
		m_pMicrophone->DataIn(pHeader);
	::waveInPrepareHeader(m_handle,pHeader,sizeof(WAVEHDR));
	::waveInAddBuffer(m_handle,pHeader,sizeof(WAVEHDR));
}

#define MAD_F(x)		((dct_fixed_t) (x##L))
#  if defined(OPT_SPEED)
#   define mad_f_mul(x, y)	(((x) >> 12) * ((y) >> 16))
#  else
#   define mad_f_mul(x, y)	((((x) + (1L << 11)) >> 12) *  \
				 (((y) + (1L << 15)) >> 16))
#  endif

inline void fastsdct(dct_fixed_t const x[9], dct_fixed_t y[18])
{
  dct_fixed_t a0,  a1,  a2,  a3,  a4,  a5,  a6,  a7,  a8,  a9,  a10, a11, a12;
  dct_fixed_t a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25;
  dct_fixed_t m0,  m1,  m2,  m3,  m4,  m5,  m6,  m7;

  enum {
    c0 =  MAD_F(0x1f838b8d),  /* 2 * cos( 1 * PI / 18) */
    c1 =  MAD_F(0x1bb67ae8),  /* 2 * cos( 3 * PI / 18) */
    c2 =  MAD_F(0x18836fa3),  /* 2 * cos( 4 * PI / 18) */
    c3 =  MAD_F(0x1491b752),  /* 2 * cos( 5 * PI / 18) */
    c4 =  MAD_F(0x0af1d43a),  /* 2 * cos( 7 * PI / 18) */
    c5 =  MAD_F(0x058e86a0),  /* 2 * cos( 8 * PI / 18) */
    c6 = -MAD_F(0x1e11f642)   /* 2 * cos(16 * PI / 18) */
  };

  a0 = x[3] + x[5];
  a1 = x[3] - x[5];
  a2 = x[6] + x[2];
  a3 = x[6] - x[2];
  a4 = x[1] + x[7];
  a5 = x[1] - x[7];
  a6 = x[8] + x[0];
  a7 = x[8] - x[0];

  a8  = a0  + a2;
  a9  = a0  - a2;
  a10 = a0  - a6;
  a11 = a2  - a6;
  a12 = a8  + a6;
  a13 = a1  - a3;
  a14 = a13 + a7;
  a15 = a3  + a7;
  a16 = a1  - a7;
  a17 = a1  + a3;

  m0 = mad_f_mul(a17, -c3);
  m1 = mad_f_mul(a16, -c0);
  m2 = mad_f_mul(a15, -c4);
  m3 = mad_f_mul(a14, -c1);
  m4 = mad_f_mul(a5,  -c1);
  m5 = mad_f_mul(a11, -c6);
  m6 = mad_f_mul(a10, -c5);
  m7 = mad_f_mul(a9,  -c2);

  a18 =     x[4] + a4;
  a19 = 2 * x[4] - a4;
  a20 = a19 + m5;
  a21 = a19 - m5;
  a22 = a19 + m6;
  a23 = m4  + m2;
  a24 = m4  - m2;
  a25 = m4  + m1;

  /* output to every other slot for convenience */

  y[ 0] = a18 + a12;
  y[ 2] = m0  - a25;
  y[ 4] = m7  - a20;
  y[ 6] = m3;
  y[ 8] = a21 - m6;
  y[10] = a24 - m1;
  y[12] = a12 - 2 * a18;
  y[14] = a23 + m0;
  y[16] = a22 + m7;
}

void XWaveIn::FDCT(void *pData, int length)
{
	//return;
	int i,j;//,nb=m_waveFmt.nBlockAlign;
	//int nbb=m_waveFmt.nChannels*18;
	int nbs=m_waveFmt.nBlockAlign*18;
	//if(m_waveFmt.nChannels==2)
	//	nb=2;
	m_nBlocks=length/nbs*m_waveFmt.nChannels;
	if(m_nBlocks>64)
		m_nBlocks=64;
	dct_fixed_t tmp[9];
	  /* scale[i] = 2 * cos(PI * (2 * i + 1) / (2 * 18)) */
	static dct_fixed_t const scale[9] = {
		MAD_F(0x1fe0d3b4), MAD_F(0x1ee8dd47), MAD_F(0x1d007930),
		MAD_F(0x1a367e59), MAD_F(0x16a09e66), MAD_F(0x125abcf8),
		MAD_F(0x0d8616bc), MAD_F(0x08483ee1), MAD_F(0x02c9fad7)
	  };



	//if(m_waveFmt.wBitsPerSample==16)
	{
		XS8*pm=(XS8*)pData;
		//XS16*pWave=(XS16*)pData;
		int iBlock=0,iLoop=m_nBlocks/m_waveFmt.nChannels;
		memset(m_dctResult,0,sizeof(m_dctResult));
		for(j=0;j<iLoop;j++)
		{
			/* divide the 18-point SDCT-II into two 9-point SDCT-IIs */

			/* even input butterfly */
			
			dct_fixed_t data[2][18];
			if(m_waveFmt.wBitsPerSample==16)
			{
				XS16*pWave=(XS16*)pm;
				for(i=0;i<18;i++)
				{
					data[0][i]=pWave[0];
					pWave++;
					if(m_waveFmt.nChannels==2)
					{
						data[1][i]=pWave[0];
						pWave++;
					}
					//pWave++;
				}
			}
			else
			{
				XS8*pWave=(XS8*)pm;
				for(i=0;i<18;i++)
				{
					data[0][i]=pWave[i];
					pWave++;
					if(m_waveFmt.nChannels==2)
					{
						data[1][i]=pWave[i];
						pWave++;
					}
					//pWave++;
				}
			}
			for(int m=0;m<m_waveFmt.nChannels;m++)
			{
				dct_fixed_t* x=data[m];
				dct_fixed_t* X=m_dct[iBlock];
				for (i = 0; i < 9; i += 3) {
					tmp[i + 0] = x[i + 0] + x[18 - (i + 0) - 1];
					tmp[i + 1] = x[i + 1] + x[18 - (i + 1) - 1];
					tmp[i + 2] = x[i + 2] + x[18 - (i + 2) - 1];
				  }
				fastsdct(tmp, &X[0]);

				/* odd input butterfly and scaling */

				for (i = 0; i < 9; i += 3) {
					tmp[i + 0] = mad_f_mul(x[i + 0] - x[18 - (i + 0) - 1], scale[i + 0]);
					tmp[i + 1] = mad_f_mul(x[i + 1] - x[18 - (i + 1) - 1], scale[i + 1]);
					tmp[i + 2] = mad_f_mul(x[i + 2] - x[18 - (i + 2) - 1], scale[i + 2]);
				  }

				fastsdct(tmp, &X[1]);

				/* output accumulation */

				 for (i = 3; i < 18; i += 8) {
					X[i + 0] -= X[(i + 0) - 2];
					X[i + 2] -= X[(i + 2) - 2];
					X[i + 4] -= X[(i + 4) - 2];
					X[i + 6] -= X[(i + 6) - 2];
				  }
				for(i=0;i<18;i++)
					m_dctResult[i]+=X[i];
				iBlock++;//'=m_waveFmt.nChannels;
			}
			pm+=nbs;
			//pWave+=nbb;
		}
	}
	for(i=0;i<18;i++)
		m_dctResult[i]=m_dctResult[i]/m_nBlocks;
}