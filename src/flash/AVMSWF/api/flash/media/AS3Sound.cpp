#include "StdAfxflash.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"
#include "ShellCore.h"
#include "sdisplay.h"
using namespace avmplus;
#include "AS3Sound.h"
#include "AS3SoundChannel.h"
#include "AS3Event.h"
#include "AS3ProgressEvent.h"
#include "AS3IOErrorEvent.h"
#include "AS3URLRequest.h"
#include "AS3ID3Info.h"

#include "avm2.h"

#define MP3_FRAMETIME 26

namespace avmshell{

SoundClass::SoundClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SoundObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SoundClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SoundObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SoundObject::SoundObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pLoaderInfo=XNULL;
	m_pCharacter=(_XSCharacter*)this->vtable->GetInitData();
	m_pClient=NULL;
	m_bNeedPlay=false;
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t SoundObject::AS3_bytesLoaded_get()
{
	//Add your act code here...
	if(m_pLoaderInfo)
	{
		return m_pLoaderInfo->nBytesTotal;
	}
	else if(m_pCharacter)
	{
		return m_pCharacter->length;
	}
	return 0;

}

void SoundObject::Release()
{
	if(m_pLoaderInfo)
	{
		m_pLoaderInfo->stream.RemoveAll();
		m_pLoaderInfo=NULL;
	}
	EventDispatcherObject::Release();
}

uint32_t SoundObject::AS3_bytesTotal_get()
{
	//Add your act code here...
	if(m_pLoaderInfo)
	{
		return m_pLoaderInfo->stream.GetDataSize();
	}
	else if(m_pCharacter)
	{
		return m_pCharacter->length;
	}
	return 0;
}

ID3InfoObject* SoundObject::AS3_id3_get()
{
	//Add your act code here...
	return m_id3;
	//return NULL;
}

bool SoundObject::AS3_isBuffering_get()
{
	//Add your act code here...
	if(!m_pLoaderInfo||!m_pLoaderInfo->bInitOK)
		return true;
	return false;
}

double SoundObject::AS3_length_get()
{
	//Add your act code here...
	if(m_pLoaderInfo)
	{
		if(!m_pLoaderInfo->bInitOK) return 0;
		return m_pLoaderInfo->nLoadedSamples*1000/m_pLoaderInfo->nRate;
	}
	else if(m_pCharacter)
	{
#ifdef SOUND
		int nRate=m_pCharacter->sound.Rate();
		return (double)m_pCharacter->sound.nSamples*1000/nRate;
#endif
	}

	return 0;
}

bool  SoundObject::StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo)
{
	m_pClient=pClient;
	ShellCore*c=(ShellCore*)core();
	if(m_pLoaderInfo==XNULL)
	{
		m_pLoaderInfo=new (c->GetGC())LoaderData();
	}
	if(!m_pLoaderInfo) return false;
	LoaderData* pInfo=m_pLoaderInfo;
	_XStreamData*pStream=(_XStreamData*)streamData;

	pInfo->strURL=c->newStringLatin1(streamUrl);
	pInfo->stream.SetSize(pStream->nTotalBytes);
	pInfo->stream.SetSize(0);
	pInfo->nBytesTotal=pStream->nTotalBytes;
	pInfo->bCancel=false;

	XContentType type;
	type.SetType(contentType);
	switch(type.m_nClass)
	{
	case XCNT::CAUDIO:
		 switch(type.m_nType)
		 {
		 case XCNT::TMPEG:
		 case XCNT::TMPEG2:
		 case XCNT::TX_MPEG:
		 case XCNT::TX_MPEG2:
		 case XCNT::TX_MPEG4:
		 case XCNT::TX_MPEG3:
			  pInfo->nFormat=XFMT_MP3;
			  break;
		 }
		 break;
	}
	//if(XString8::Compare(contentType,"autdio/x-

	OnEvent(XAS3_EVENT_open,false);

#ifdef _WINEMU
	char*host=strstr(streamUrl,"://");
	if(host)
	{
		//((ShellToplevel*)toplevel())->PushObject(m_pLoaderInfo);

		char* id=strrchr(streamUrl,'?');
		char* start=NULL;//strchr(host+3,'/');
		if(id)
		{
			start=id-1;
			while(start>streamUrl)
			{
				if(*start=='/') break;
				start--;
			}
		}
		else
			start=strrchr(host,'/');

		if(start)
		{
			//start+=3;
			char*s=streamUrl;
			int iCode=0;
			while(s<start)
			{
				iCode+=*s;
				s++;
			}
			//ShellCodeContext*code=(ShellCodeContext*)core()->codeContext();
			XSWFPlayer*player=((ShellCore*)core())->GetPlayer();//character->player;
				//code->m_pRoot;
			XString8 strPath=player->m_strWorkPath.strTxt;
			strPath+="Download\\";
			XFile::CreateFolder(strPath);
			//int is=strPath.GetLength(),ie;
			XString8 strInt;
			strInt.FromInt(iCode);
			strPath+=strInt;
			strPath+="_";
			if(id)
				strPath.AddString(start+1,id-start-1);
			else
				strPath.AddString(start+1);

			/*ie=strPath.GetLength();
			char*buf=strPath.GetData();
			for(int ii=is;ii<ie;ii++)
			{
				switch(buf[ii])
				{
				case '/':
				case '\\':
				//case '.':
					 buf[ii]='_';break;
				}
			}*/
			//strPath+=".swf";
			m_file.Open(strPath,XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
		}
		//int id=strchr(streamUrl,"."
	}
#endif
	/*if(hasEvent(XAS3_EVENT_open))
	{
		EventObject*pObj=((ShellToplevel*)toplevel())->getEventClass()->CreateEventObject(XAS3_EVENT_open);
		pObj->pTarget=this;
		AS3_dispatchEvent(pObj);
	}*/

	return true;
}

#define MAKEIDMARK4(ID1,ID2,ID3,ID4) ((ID1<<24)|(ID2<<16)|(ID3<<8)|ID4)
#define MAKEIDMARK3(ID1,ID2,ID3) ((ID1<<16)|(ID2<<8)|ID3)
inline static int BitsRate(int bitrate,int version,int layer)
{
	const int rateTable[8][16]=
	{
		{0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,0},
		{0,32,48,56,64, 80, 96, 112,128,160,192,224,256,320,384,0},
		{0,32,40,48,56, 64, 80, 96, 112,128,160,192,224,256,320,0},
		{},
		{0,32,48,56,64, 80, 96, 112,128,144,160,176,192,224,256,0},
		{0,8, 16,24,32, 40, 48, 56, 64, 80, 96, 112,128,144,160,0},
		{0,8, 16,24,32, 40, 48, 56, 64, 80, 96, 112,128,144,160,0},
		{},
	};
	return rateTable[((version)<<2)+layer][bitrate];
}
inline static int FrameLength(int bitrate,int version,int layer,int freq)
{
	int rate=BitsRate(bitrate,version,layer);
	const int dataTable[2][3]=
	{
		{48000,144000,144000},
		{24000,72000,72000}};
	return (dataTable[version][layer]*rate)/freq;

}

const XU32 xingMark=	MAKEIDMARK4('X','i','n','g');//('g'<<24)|('n'<<16)|('i'<<8)|'X';
const XU32 tagMark=		MAKEIDMARK3('T','A','G');///('G'<<16)|('A'<<8)|('T');
const XU32 ID3=		    MAKEIDMARK3('I','D','3');////('3'<<16)|('D'<<8)|('I');
const XU32 ID_TIT2=	    MAKEIDMARK4('T','I','T','2');//('2'<<24)|('T'<<16)|('I'<<8)|'T';
const XU32 ID_TALB=		MAKEIDMARK4('T','A','L','B');//('B'<<24)|('L'<<16)|('A'<<8)|'T';
const XU32 ID_TRCK=		MAKEIDMARK4('T','R','C','K');//('K'<<24)|('C'<<16)|('R'<<8)|'T';
const XU32 ID_TYER=		MAKEIDMARK4('T','Y','E','R');//('R'<<24)|('E'<<16)|('Y'<<8)|'T';
const XU32 ID_COMM=		MAKEIDMARK4('C','O','M','M');//('M'<<24)|('M'<<16)|('O'<<8)|'C';
const XU32 ID_TCON=		MAKEIDMARK4('T','C','O','N');//('N'<<24)|('O'<<16)|('C'<<8)|'T';
const XU32 ID_TPE1=		MAKEIDMARK4('T','P','E','1');//('1'<<24)|('E'<<16)|('P'<<8)|'T';

bool SoundObject::StreamProc(SoundChannelObject*snd)
{
	LoaderData* pInfo=m_pLoaderInfo;
	if(!pInfo->bInitOK) return true;
	XSoundChannel*channel=snd->m_pChannel;
	XSound*sound=channel->sound;
	if(pInfo->nPlayedSamples+sound->delay*pInfo->nRate/1000>pInfo->nLoadedSamples&&
	   m_pClient) return true;
	if(!m_pClient&&pInfo->nLoadedSamples<=0)
	{
		snd->Stop();
		channel->streamEnd=true;
		return true;
	}
	XLock lock(&m_dataLock);
	if(!lock.Lock(XNORMALTIME))
		return true;

	channel->blockSamples=0;



	sound->dataLen=0;
	bool bStop=false;
	int nTotal=pInfo->stream.GetSize(),nBlockSamples=0;
	XU8*pBuf=(XU8*)pInfo->stream.GetData();
	sound->samples=pBuf+pInfo->nPlayPos;
	channel->blockCanBeZero=true;
	//while(!nBlockSamples)
	{
		while(pInfo->nPlayPos+36<nTotal)
		{
			XU8*pHeader=pBuf+pInfo->nPlayPos;
			XU32 nMark=MAKEIDMARK4(pHeader[0],pHeader[1],pHeader[2],pHeader[3]);
				//*((XU32*)(pBuf+m_pLoaderInfo->nPlayPos));
			if((nMark>>21)==0x7ff)
			{
				unsigned int padding=(nMark>>9)&1;
				//unsigned int bitrate_index=(nMark>>12)&0xf; //4
				//int nRate=BitsRate(bitrate_index,m_pLoaderInfo->nVersion,m_pLoaderInfo->nLayer);
				int len=0;//;((nRate*26)>>3)+padding+1;
				if(!pInfo->bVBR)
					len=pInfo->nFrameLength+padding;
				else
				{
					unsigned int bitrate_index=(nMark>>12)&0xf;
					len=FrameLength(bitrate_index,pInfo->nVersion,pInfo->nLayer,pInfo->nRate)+padding;
				}
					//m_pLoaderInfo->nFrameLength+padding;
				sound->dataLen+=len;
				pInfo->nPlayPos+=len;
				if(!pInfo->bVBR||pInfo->nPlayPos)
					nBlockSamples+=pInfo->nBlockSamples;
					//MP3_FRAMETIME;
			}
			/*else if((nMark>>8)==xingMark)
			{
				int len=sizeof(VBRHeader);
				m_pLoaderInfo->nPlayPos+=len;
				sound->dataLen+=len;
			}*/
			else if((nMark>>8)==ID3)
			{
				ID3HeaderV2*header=(ID3HeaderV2*)pBuf;
				int total_size;
				total_size =   (header->size[0]&0x7F)*0x200000
								+(header->size[1]&0x7F)*0x4000
								+(header->size[2]&0x7F)*0x80
								+(header->size[3]&0x7F);
				pInfo->nPlayPos+=(total_size+sizeof(ID3HeaderV2));
			}
			else if((nMark>>8)==tagMark)
			{
				break;
			}
			else break;
			//if(nBlockTime>=100) break;
		}
		if(!nBlockSamples)
		{
			/*if(!m_pLoaderInfo->nPlayPos)
				bStop=true;
			else if(--channel->loops>0)
			{
				m_pLoaderInfo->nPlayPos=0;
				sound->samples=pBuf+m_pLoaderInfo->nPlayPos;
			}
			else */

			if(!m_pClient)
			{
				pInfo->nPlayPos=0;
				pInfo->nPlayedSamples=0;
				channel->streamEnd=true;
			}
		}
	}
	lock.UnLock();
	//if(bStop)
	//{
	//	snd->Stop();
	//	return false;
	//}
	//else
	{
		pInfo->nPlayedSamples+=nBlockSamples;//nBlockTime;
		channel->blockSamples = nBlockSamples;
							   //nBlockTime*m_pLoaderInfo->nRate/1000;
		channel->seekSamples = channel->blockSamples;
        channel->blockCanBeZero = true;
		if(channel->decomp&&sound->dataLen)
		{
			int delaySav = sound->delay;
			sound->delay = 0;
			channel->decomp->Setup(sound, false);
			sound->delay = delaySav;
		}


	}
	return true;
}

bool SoundObject::doInMP3(bool bEnd)
{
	/*const XU32 xingMark=('g'<<24)|('n'<<16)|('i'<<8)|'X';
	const XU32 tagMark=('G'<<16)|('A'<<8)|('T');
	const XU32 ID3=('3'<<16)|('D'<<8)|('I');
	const XU32 ID_TIT2=('2'<<24)|('T'<<16)|('I'<<8)|'T';
	const XU32 ID_TALB=('B'<<24)|('L'<<16)|('A'<<8)|'T';
	const XU32 ID_TRCK=('K'<<24)|('C'<<16)|('R'<<8)|'T';
	const XU32 ID_TYER=('R'<<24)|('E'<<16)|('Y'<<8)|'T';
	const XU32 ID_COMM=('M'<<24)|('M'<<16)|('O'<<8)|'C';
	const XU32 ID_TCON=('N'<<24)|('O'<<16)|('C'<<8)|'T';
	const XU32 ID_TPE1=('1'<<24)|('E'<<16)|('P'<<8)|'T';*/
	enum {M3V_25=0,M3V_UNKOWN=1,M3V_20=2,M3V_10=3};
	enum {M3L_1=3,M3L_2=2,M3L_3=1};

	LoaderData*pInfo=m_pLoaderInfo;

	int nPos=pInfo->nPos;
	int nTotal=pInfo->stream.GetSize();
	if(bEnd&&nTotal>=128)
		nPos=nTotal-128;
	ShellCore*c=(ShellCore*)core();
	//XU8*buf=m_pLoaderInfo->stream.GetData();
	while(nPos<nTotal)
	{
		if(nPos+36>nTotal) break;

		XU8*pBuf=pInfo->stream.GetData()+nPos;
		//LPMP3HEADER pHeader=(LPMP3HEADER)pBuf;
		XU32 nMark=(pBuf[0]<<24)|(pBuf[1]<<16)|(pBuf[2]<<8)|pBuf[3];
			//*(XU32*)pBuf;
		if((nMark>>21)==0x7ff)
		{
			unsigned int padding=(nMark>>9)&1;
			unsigned int bitrate_index=(nMark>>12)&0xf; //4
			if(!pInfo->bInitOK)
			{
				pInfo->bInitOK=true;
				pInfo->nSoundHeader=nMark;
				unsigned int version=(nMark>>19)&0x3; //2
				unsigned int layer=(nMark>>17)&0x3;	  //2
				unsigned int error_protection=(nMark>>16)&1; //1

				unsigned int sampling_frequency=(nMark>>10)&3;//:2;
				//unsigned int padding=(nMark>>9)&1;//:1;
				//unsigned int reserved:1;
				unsigned int mode=(nMark>>6)&3;//:2;
				unsigned int mode_extension=(nMark>>4)&3;//:2;
				unsigned int copyright=(nMark>>3)&1;//:1;
				unsigned int original=(nMark>>2)&1;//:1;
				unsigned int emphasis=(nMark&3);//2
				pInfo->snd.format|=sndCompressMP3;
				if(mode!=3)
					pInfo->snd.format|=sndStereo;
				if(layer!=M3L_1)
					pInfo->snd.format|=snd16Bit;
				//unsigned int nRate=24000,nBits=48000;
				//int nVersion=1,nLayer=3;
				switch(layer)
				{
				case M3L_1:
					 pInfo->nLayer=0;break;
				case M3L_2:
					 pInfo->nLayer=1;break;
				case M3L_3:
					 pInfo->nLayer=2;break;
				}
				switch(version)
				{
					case M3V_25:
						 switch(sampling_frequency)
						 {
						 case 0:
							  pInfo->nRate=11025;
							  pInfo->snd.format|=1<<2;
							  break;//11.025
						 case 1:pInfo->nRate=12000;
							  pInfo->snd.format|=1<<2;
							  break;//12
						 case 2:pInfo->nRate=8000;break;//8
						 }
						 pInfo->nVersion=1;
						 break;
					case M3V_20:
						 switch(sampling_frequency)
						 {
						 case 0:pInfo->nRate=22050;
							   pInfo->snd.format|=2<<2;
							   break;//22.05
						 case 1:pInfo->nRate=24000;
							   pInfo->snd.format|=2<<2;
							   break;//24
						 case 2:pInfo->nRate=16000;
							  pInfo->snd.format|=1<<2;
							  break;//16
						 }
						 //pInfo->snd.format|=2<<2;
						  pInfo->snd.format|=1<<24;
						 pInfo->nVersion=1;
						 break;
					case M3V_10:
						 switch(sampling_frequency)
						 {
							 case 0://44.1
								 pInfo->snd.format|=3<<2;
								 pInfo->nRate=44100;
								 break;
							 case 1://48
								 //break;
								 pInfo->snd.format|=3<<2;
								 pInfo->nRate=48000;break;
							 case 2://32
								 pInfo->snd.format|=2<<2;
								 pInfo->nRate=32000;break;
						 }
						 pInfo->snd.format|=1<<24;
						 pInfo->nVersion=0;
						 break;
				}
				pInfo->nFrameLength=FrameLength(bitrate_index,pInfo->nVersion,pInfo->nLayer,pInfo->nRate);
				XU32 nID=MAKEIDMARK4(pBuf[36],pBuf[37],pBuf[38],pBuf[39]);
				if(nID==xingMark)
					pInfo->bVBR=true;

				pInfo->nBlockSamples=MP3_FRAMETIME*pInfo->nRate/1000;//(layer==M3L_1?12:
											//((layer==M3L_3&&(version&1==0))?18:36))*32;
				//# define MAD_NSBSAMPLES(header)  \
				//((header)->layer == MAD_LAYER_I ? 12 :  \
			    //(((header)->layer == MAD_LAYER_III &&  \
				//((header)->flags & MAD_FLAG_LSF_EXT)) ? 18 : 36))

			}
			int l=FrameLength(bitrate_index,pInfo->nVersion,pInfo->nLayer,pInfo->nRate)+padding;
			if(nPos+l>nTotal) break;
			if(!pInfo->bVBR||nPos)
				pInfo->nLoadedSamples+=pInfo->nBlockSamples;
					//pInfo->nLoadedTime+=MP3_FRAMETIME;
			nPos+=l;

		}
		/*else if(nMark==xingMark)
		{
			nPos+=sizeof(VBRHeader);
			m_pLoaderInfo->nStartPos=sizeof(VBRHeader);
		}*/
		else if((nMark>>8)==tagMark)
		{
			nPos+=sizeof(ID3Header);
			ID3InfoClass*pClass=((ShellToplevel*)toplevel())->getID3InfoClass();
			ID3Header*header=(ID3Header*)pBuf;
			m_id3=(ID3InfoObject*)pClass->CreateID3(header);
			this->OnEvent(XAS3_EVENT_id3,false);
		}
		else if((nMark>>8)==ID3)
		{
			ID3HeaderV2*header=(ID3HeaderV2*)pBuf;
			int total_size;
			total_size =   (header->size[0]&0x7F)*0x200000
			   +(header->size[1]&0x7F)*0x4000
			   +(header->size[2]&0x7F)*0x80
			   +(header->size[3]&0x7F);
			int nFrame=sizeof(ID3HeaderV2);
			if(m_id3==NULL)
			{
				ID3InfoClass*pClass=((ShellToplevel*)toplevel())->getID3InfoClass();
				m_id3=(ID3InfoObject*)pClass->newInstance();
			}
			AS3ID3DATA& data=m_id3->GetData();
			XU8*pFrame=pBuf+nFrame;
			while(nFrame<total_size)
			{   ID3Frame*pf=(ID3Frame*)pFrame;
				int cSize=(pf->size[0]<<24)|(pf->size[1]<<16)|(pf->size[2]<<8)|pf->size[3];
				char*pCnt=(char*)(pf+1);
				XString8 strCnt;
				if(*pCnt==0&&cSize>1)
					strCnt.ConvertFrom(pCnt+1,XCHARSET_GB2312,cSize-1);
				else if(cSize)
					strCnt.ConvertFrom(pCnt,XCHARSET_GB2312,cSize);
				Stringp cnt=c->newStringUTF8(strCnt,strCnt.GetLength());
				switch(MAKEIDMARK4(pf->frameID[0],pf->frameID[1],pf->frameID[2],pf->frameID[3]))
				{
				case ID_TIT2:
					 data.m_strSongName=cnt;//c->newStringLatin1((const char*)(pf+1),cSize);
					 break;
				case ID_TALB:
					 data.m_strAlbum=cnt;//c->newStringLatin1((const char*)(pf+1),cSize);
				case ID_TRCK:
					 data.m_strTrack=cnt;//c->newStringLatin1((const char*)(pf+1),cSize);
					 break;
				case ID_TYER:
					 data.m_strTrack=cnt;//c->newStringLatin1((const char*)(pf+1),cSize);
					 break;
				case ID_COMM:
					 data.m_strComment=cnt;//c->newStringLatin1((const char*)(pf+1),cSize);
					 break;
				case ID_TCON:
					 data.m_strGenre=cnt;//c->newStringLatin1((const char*)(pf+1),cSize);
					 break;
				case ID_TPE1:
					 data.m_strArtist=cnt;//c->newStringLatin1((const char*)(pf+1),cSize);
					 break;

				}
				pFrame+=sizeof(ID3Frame);
				pFrame+=cSize;
				nFrame+=(cSize+sizeof(ID3Frame));
			}
			nPos+=(total_size+sizeof(ID3HeaderV2));
			this->OnEvent(XAS3_EVENT_id3,false);
		}
		else
		{
			pInfo->nPos=nPos;
			return false;
		}
		/*if(pHeader->sync==0x7ff)
		{

		}*/

	}
	pInfo->nPos=nPos;
	return true;
}

int SoundObject::StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient)
{
	if(!m_pLoaderInfo)
		return XSWFPlayer::LOAD_BREAK;
#ifdef _WINEMU
			if(m_file.IsValid())
				m_file.Write(buffer,length);
#endif
	//m_pLoaderInfo->nBytesTotal+=length;
	if(m_pLoaderInfo->nFormat==XFMT_UNKOWN)
	{
		XU8*p=(XU8*)buffer;
		XU32 nMark=MAKEIDMARK4(p[0],p[1],p[2],p[3]);
		if(nMark==xingMark||
		   (nMark>>21)==0x7ff||
		   (nMark>>8)==ID3)
		   m_pLoaderInfo->nFormat=XFMT_MP3;
	}
	XLock lock(&m_dataLock);
	if(lock.Lock(XNORMALTIME))
	{
		m_pLoaderInfo->stream.Append((XU8*)buffer,length);
		switch(m_pLoaderInfo->nFormat)
		{
		case XFMT_MP3:
			 doInMP3();
			 break;
		}
		lock.UnLock();
		if(m_pLoaderInfo->bInitOK&&m_pLoaderInfo->pObject)
			m_pLoaderInfo->pObject->InitOK();
	}
	/*if(hasEvent(XAS3_EVENT_progress))
	{
		ProgressEventObject*pEvent=((ShellToplevel*)toplevel())->getProgressEventClass()->CreateEventObject(XAS3_EVENT_progress,
								 m_pLoaderInfo->stream.GetDataSize(),m_pLoaderInfo->nBytesTotal);
		pEvent->pTarget=this;
		AS3_dispatchEvent(pEvent);
	}*/
	OnProgressEvent(XAS3_EVENT_progress,m_pLoaderInfo->stream.GetDataSize(),m_pLoaderInfo->nBytesTotal);
	if(m_pLoaderInfo->bCancel)
		return XSWFPlayer::LOAD_BREAK;
	return XSWFPlayer::LOAD_OK;
}
bool SoundObject::StreamInDestroy(XBOOL bOK,const char*msg,void*pClient)
{
	if(m_pClient&&m_pClient!=pClient)
		return true;
#ifdef _WINEMU
	if(m_file.IsValid())
		m_file.Close();
#endif
	m_pClient=NULL;
	if(!m_pLoaderInfo) return true;
	if(bOK)
	{
		switch(m_pLoaderInfo->nFormat)
		{
		case XFMT_MP3:
			 doInMP3(true);
			 break;
		}
		OnEvent(XAS3_EVENT_complete,false);
	}
	else
	{
		OnIOErrorEvent(msg);
		/*if(hasEvent(XAS3_EVENT_ioError))
		{
			Stringp s=core()->newStringLatin1(msg);
			EventObject*pObj=((ShellToplevel*)toplevel())->getIOErrorEventClass()->CreateEventObject(XAS3_EVENT_ioError,s);
			pObj->pTarget=this;
			AS3_dispatchEvent(pObj);
		}*/
	}
	return true;
}

Stringp SoundObject::AS3_url_get()
{
	//Add your act code here...
	if(m_pLoaderInfo)
		return m_pLoaderInfo->strURL;
	else if(m_pCharacter&&m_pCharacter->splayer)
	{
		return core()->newStringLatin1(m_pCharacter->splayer->m_url.strTxt);
	}

	return NULL;
}

//AS3 contructor function..
void SoundObject::AS3_constructor(URLRequestObject*req,SoundLoaderContextObject*cnt)
{
	//Add your act code here...
	if(req)
	{
		AS3_load(req,cnt);
	}
}

void SoundObject::AS3_close()
{
	//Add your act code here...
	if(m_pClient)
	{
		((ShellCore*)core())->GetPlayer()->CloseClient(m_pClient);
	}
	if(m_pLoaderInfo)
		m_pLoaderInfo->bCancel=true;
}

void SoundObject::AS3_load(URLRequestObject*url,SoundLoaderContextObject*cnt)
{
	//Add your act code here...
	if(!m_codeContext) return;
	if(m_pLoaderInfo&&m_pLoaderInfo->bPlaying) return;
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	if(m_pCharacter) m_pCharacter=NULL;
	if(!m_pLoaderInfo)
	{
		m_pLoaderInfo=new (gc())LoaderData();
	}

	if(m_pLoaderInfo->snd.dataLen)
		int v=0;
	m_pLoaderInfo->Init();

	EventDispatcherObject::LoadURL(url,NULL);

	StUTF8String strURL(url->m_strURL);
	//const char*str=strstr(strURL.c_str(),"core.swf");
	//if(str!=NULL)
	//	return;
	/*XU8Array datas;
	Stringp strType=url->GetPostData(datas,false);
	if(strType==NULL) strType=url->m_strContext;
	int nType=kHttpLoadTarget;
	if(url->m_nMethod==URLRequestMethodClass::POST)
		nType|=kHttpSendUsePost;
	else
		nType|=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=this->m_pLoaderInfo;//m_pCodeContext->m_pLoaderInfo;
		//(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,this,strType?StUTF8String(strType).c_str():NULL);
	if(datas.GetSize())
		pInfo->postData.Append(datas);
	/*ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	StUTF8String strURL(url->m_strURL);
	XU8Array datas;
	Stringp strType=url->GetPostData(datas,false);
	if(strType==NULL) strType=url->m_strContext;
	int nType=kHttpLoadTarget|kHttpLoadData;
	if(url->m_nMethod==URLRequestMethodClass::POST)
		nType|=kHttpSendUsePost;
	else
		nType|=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,this,strType?StUTF8String(strType).c_str():NULL);
	if(datas.GetSize())
		pInfo->postData.Append(datas);*/
}

SoundChannelObject* SoundObject::AS3_play(double start,int loops,SoundTransformObject*trans)
{
	//Add your act code here...
	SoundChannelClass*pClass=((ShellToplevel*)toplevel())->getSoundChannelClass();
	SoundChannelObject*pObj=NULL;
	if(m_pLoaderInfo)
	{
		if(m_pLoaderInfo->bPlaying)
			return m_pLoaderInfo->pObject;
		else
		{
			m_pLoaderInfo->nPlayPos=0;
			m_pLoaderInfo->nPlayedSamples=0;
			m_pLoaderInfo->bPlaying=true;
			pObj=pClass->CreateSoundChannel(this,trans,start,loops,m_pLoaderInfo->bInitOK);
			if(pObj->m_pChannel)
				this->StreamProc(pObj);
			m_pLoaderInfo->pObject=pObj;
		}
		m_pLoaderInfo->bPlaying=true;
	}
	else if(m_pCharacter) //return NULL;
		pObj=pClass->CreateSoundChannel(m_pCharacter,trans,start,loops);
	else
		pObj=pClass->CreateSoundChannel(this,trans,start,loops,false);
	return pObj;
}

//new adds
bool SoundObject::AS3_isURLInaccessible_get()
{
	return false;
}

double SoundObject::AS3_extract(ByteArrayObject *pTarget, double length, double startPosition)
{
	return 0;
}
//new adds end


//new adds 11
void SoundObject::AS3_loadCompressedDataFromByteArray(ByteArrayObject *pBytes, uint32_t bytesLength)
{
	LOGWHERE();
}
void SoundObject::AS3_loadPCMFromByteArray(ByteArrayObject *pBytes, uint32_t samples, Stringp format, bool bStereo, double sampleRate)
{
	LOGWHERE();
}
//new adds 11 end
}
