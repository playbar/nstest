// XStream.cpp: implementation of the XStream class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxGGBase.h"
#include "XStream.h"
#include "XFile.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XStream::XStream()
{
	readBytePos=8;
	writeBytePos=8;
	readPos=writePos=0;
	bAttach=XFALSE;
	nParam=0;
}

XStream::~XStream()
{
	if(bAttach) m_pData=XNULL;
}
////////////////////////////////////////////////////
// 从文件中读入数据，获取的数据添加到流的尾部
// 该操作从字节边界开始，将剩余未写位数跳过
// file:文件句柄
XBOOL XStream::ReadFromFile(XFile&file,int nLength)
{
	if(nLength<=0) nLength=BLOCKSIZE;
	//XU8Array data;
	//data.SetSize(nLength);
	FlushWriteBits();
	SetSize(writePos+nLength);
	nLength=file.Read(m_pData+writePos,nLength);
		//::fread(data,1,nLength,file);
	if(nLength>0)
	{
		//Append(data,nLength);
		writePos+=nLength;
	}
	return nLength;
}
////////////////////////////////////////////////
// 写入数据到文件，写入后数据将从流中删除
int XStream::WriteToFile(XFile&file,int nLength)
{
	FlushReadBits();
	if(nLength==0||readPos+nLength>=writePos) nLength=Bytes();
	nLength=file.Write(m_pData+readPos,nLength);
		//::fwrite(m_pData+readPos,1,nLength,file);
	readPos+=nLength;
	return nLength;
}

////////////////////////////////////////////////////////////
// 写入指定长度的数据块到流的尾部
// pData:数据块地址
// nLength:数据块长度

void XStream::WriteData(XPCVOID pData, int nLength)
{
	if(nLength<=0) return;
	SetSize(writePos+nLength);
	if(pData)
		memcpy(m_pData+writePos,pData,nLength);
	//Append((XU8*)pData,nLength);
	writePos+=nLength;
}
/////////////////////////////////////////////////////////////
// 清除缓冲区中的全部数据
void XStream::Reset()
{
	readPos=writePos=0;
	readBytePos=8;
	writeBytePos=8;
	m_nSize=0;
}
/////////////////////////////////////////////////////////////
// 从缓冲区中读取指定长度的位数到变量
// data:返回的数据
// size:读取的位数
static XU8 _bitMask[]={0x1,0x3,0x7,0xf,0x1f,0x3f,0x7f,0xff};

void XStream::AttachFrom(XPCVOID pData, int nLength,XU8 bMem)
{
	 if(!bAttach)
	 {
		 if(m_pData) delete m_pData;
	 }
	 readBytePos=writeBytePos=8;
	 m_pData=(XU8*)pData;
	 m_nSize=m_nMaxSize=nLength;
	 readPos=0;
	 bAttach=XTRUE;
	 if(!bMem)
		writePos=nLength;
	 else
		writePos=0;
}


XBOOL XStream::ReadBits(XS32&data,XU8 size)
{
	if(!ReadBits((XU32&)data,size)) return XFALSE;
	XU32 sign=data&(1<<(size-1));
	if(sign!=0)
	{
		data|=((0xffffffff>>size)<<size);
	}
	return XTRUE;
}

XBOOL XStream::ReadBits(XS16&data,XU8 size)
{
	if(!ReadBits((XU16&)data,size)) return XFALSE;
	XU16 sign=data&(1<<(size-1));
	if(sign!=0)
	{
		data|=((0xffff>>size)<<size);
	}
	return XTRUE;
}


/*XU8 XStream::ReadBit()
{
	if(readPos>=writePos) return 0;
	XU8 nData=(m_pData[readPos]>>(readBytePos-1))&1;
	readBytePos--;
	if(readBytePos<1)
	{
		readBytePos=8;
		readPos++;
	}
	return nData;
}*/


XBOOL XStream::ReadBits0(XU32 &data, XU8 size)
{
	//if(size==0) return XTRUE;
	if(readPos>=writePos) return XFALSE;
	data=0;
	int k=0;
	while(size>0)
	{
		 //当前字节的剩余位数足够
		 if(readBytePos>size)
		 {
			 //data=data<<size;
			 data|=((m_pData[readPos]>>(8-readBytePos))&
				   _bitMask[size-1])<<k;
			 readBytePos-=size;
			 if(readBytePos<1)
			 {
				 readBytePos=8;
				 readPos++;
			 }
			 k+=size;
			 size=0;
		 }
		 //当前字节的剩余位数不够
		 else 
		 {
			 //data=data<<readBytePos;
			 data|=((m_pData[readPos]>>(8-readBytePos))&_bitMask[readBytePos-1])<<k;
			 readPos++;
			 size-=readBytePos;
			 k+=readBytePos;
			 readBytePos=8;	
		 }
	}	
	//FreeSpace();
	return XTRUE;
}

void XStream::WriteBits(int data,int size)
{
	if(size==0) return;
	SetSize(writePos+2+size/8);
	while(size>0)
	{
		 //当前字节的剩余位数足够
		 if(writeBytePos>size) //readBytePos>size)
		 {
			 //data=data<<size;
			 //data|=(m_pData[readPos]>>(readBytePos-size))&
			 //	   _bitMask[size-1];
			 //readBytePos-=size;
			 //if(readBytePos<1)
			 //{
			//	 readBytePos=8;
			 //	 readPos++;
			 //}
			 //size=0;
			 m_pData[writePos]|=(data&_bitMask[writeBytePos-1])<<(writeBytePos-size);
			 writeBytePos-=size;
			 if(writeBytePos<1)
			 {
				 writePos++;
				 writeBytePos=8;
			 }
			 size=0;
		 }
		 //当前字节的剩余位数不够
		 else 
		 {
			 m_pData[writePos]|=((data>>(size-writeBytePos))&_bitMask[writeBytePos-1]);
			 //data=data>>writeBytePos;
			 size-=writeBytePos;
			 writeBytePos=8;
			 writePos++;

			 //data=data<<readBytePos;
			 //data|=m_pData[readPos]&_bitMask[readBytePos-1];
			 //readPos++;
			 //size-=readBytePos;
			 //readBytePos=8;			 
		 }
	}	
	//FreeSpace();
	//return XTRUE;
}

XBOOL XStream::ReadBits(XU32 &data, XU8 size)
{
	if(size==0) return XTRUE;
	if(readPos>=writePos) return XFALSE;
	data=0;
	while(size>0)
	{
		 //当前字节的剩余位数足够
		 if(readBytePos>size)
		 {
			 data=data<<size;
			 data|=(m_pData[readPos]>>(readBytePos-size))&
				   _bitMask[size-1];
			 readBytePos-=size;
			 if(readBytePos<1)
			 {
				 readBytePos=8;
				 readPos++;
			 }
			 size=0;
		 }
		 //当前字节的剩余位数不够
		 else 
		 {
			 data=data<<readBytePos;
			 data|=m_pData[readPos]&_bitMask[readBytePos-1];
			 readPos++;
			 size-=readBytePos;
			 readBytePos=8;			 
		 }
	}	
	//FreeSpace();
	return XTRUE;
}

/////////////////////////////////////
// 读入一个字节
XBOOL XStream::ReadByte(XU8 &data)
{
	if(readPos>=writePos||readBytePos!=8) return XFALSE;
	data=m_pData[readPos];
	readPos++;
	return XTRUE;
}
//////////////////////////////////////
// 读入一个字
XBOOL XStream::ReadWord(XU16 &data)
{
	if(readPos+1>=writePos||readBytePos!=8) return XFALSE;
	//data=m_pData[readPos]|(m_pData[readPos+1]<<8);//(hb<<8)|lb;
	data=*((XU16*)&m_pData[readPos]);
	readPos+=2;
	return XTRUE;
}

////////////////////////////////////////
// 读入一个64位整数
XBOOL XStream::ReadDWord64(XU64 &data)
{
	if(readPos+7>=writePos||readBytePos!=8) return XFALSE;
	data=*((XU64*)&m_pData[readPos]);
	readPos+=8;
	return XTRUE;
}
////////////////////////////////////////
XBOOL XStream::ReadEU32(XU32&data)
{
	data=0;
	for(int i=0;i<5;i++)
	{
		if(readPos+i>=writePos) return XFALSE;
		XU8 n=m_pData[readPos];
		data=data|((n&0x7f)<<i);
		readPos++;
		if(!(n&0x80)) break;
	}
	return XTRUE;
}

////////////////////////////////////////
// 读入一个double
XBOOL XStream::ReadDouble(double&data)
{
	if(readPos+7>=writePos||readBytePos!=8) return XFALSE;
	XU32*p=(XU32*)&data;
	XU32*ps=(XU32*)(m_pData+readPos);
	p[0]=ps[1];
	p[1]=ps[0];
	//data=*((double *)&m_pData[readPos]);
	readPos+=8;
	//if(data!=0)
	//	int v=0;
	return XTRUE;
}

////////////////////////////////////////
// 读入一个16位定点数
XBOOL XStream::ReadFixed16(double &data)
{
	if(readPos+1>=writePos||readBytePos!=8) return XFALSE;
	data=m_pData[readPos]/256.0+m_pData[readPos+1];
	readPos+=2;
	return XTRUE;
}

////////////////////////////////////////
// 读入一个16位浮点数
XBOOL XStream::ReadFloat16(float &data)
{
	XU16 v;
	if(!ReadWord(v)) return XFALSE;
	int exp=((v>>10)&0x1f)+112;
	XU32 d=((v&0x8000)<<16)|(exp<<23)|((v&0x3ff)<<13);
	memcpy(&data,&d,sizeof(float));
	return XTRUE;
}

////////////////////////////////////////
// 读入一个32位定点数
XBOOL XStream::ReadFixed32(double &data)
{
	if(readPos+3>=writePos||readBytePos!=8) return XFALSE;
	data=((m_pData[readPos+1]<<8)|m_pData[readPos])+((m_pData[readPos+3]<<8)|m_pData[readPos+2]);
	readPos+=3;
	return XTRUE;
}

////////////////////////////////////////
// 读入一个32位整数
XBOOL XStream::ReadDWord(XU32 &data)
{
	if(readPos+3>=writePos||readBytePos!=8) return XFALSE;
	//data=(m_pData[readPos+3]<<24)|(m_pData[readPos+2]<<16)|(m_pData[readPos+1]<<8)|m_pData[readPos];
	data=*((XU32*)&m_pData[readPos]);
	readPos+=4;
	return XTRUE;
}

////////////////////////////////////////
// 读入一个24位整数
XBOOL XStream::ReadDWord24(XU32 &data)
{
	if(readPos+2>=writePos||readBytePos!=8) return XFALSE;
	data=(m_pData[readPos+2]<<16)|(m_pData[readPos+1]<<8)|m_pData[readPos];
	readPos+=3;
	return XTRUE;
}

/////////////////////////////////////////////////
// 读入数据块
XBOOL XStream::ReadData(void *pData, int nLength)
{
	FlushReadBits();
	if(readPos+nLength>writePos) return XFALSE;
	memcpy(pData,&m_pData[readPos],nLength);
	readPos+=nLength;
	//FreeSpace();
	return XTRUE;
}

XBOOL XStream::Skip(int nLength)
{	
	FlushReadBits();
	if(readPos+nLength>writePos) return XFALSE;
	readPos+=nLength;
//	FreeSpace();
	return XTRUE;
}

XBOOL XStream::ReadString(XString8 &text,int nLen)
{
	FlushReadBits();
 	if(nLen==0)
	{
	   while(readPos<writePos)
	   {
		   if(m_pData[readPos]==0){ readPos++;break;}
		   text+=(XTCHAR)m_pData[readPos];
		   readPos++;
	   }
	}
	else
	{
		if(readPos+nLen>writePos) return XFALSE;
		//text+=m_pData+readPos;
		text.AddString((XPCTSTR)m_pData+readPos,nLen);
		readPos+=nLen;
    }
	return XTRUE;
}

////////////////////////////////////////
// 读入一个24位整数
void XStream::WriteDWord24(XU32 data)
{
	FlushWriteBits();
	SetSize(m_nSize+3);
	m_pData[writePos]=(XU8)(data&0xff);writePos++;
	m_pData[writePos]=(XU8)((data>>8)&0xff);writePos++;
	m_pData[writePos]=(XU8)((data>>16)&0xff);writePos++;
	readPos+=3;
}

////////////////////////////////////////
void XStream::WriteEU32(XU32 data)
{
	FlushWriteBits();
	for(int i=0;i<5;i++)
	{
		XU8 v=(XU8)(data&0x7f);
		data=data>>7;
		if(data!=0) v|=0x80;
		Add(v);
		writePos++;
		if(data==0) break;
	}
}


XU8 XRingBuffer::CopyPrev(int pos, int len)
{
	if(len>pos)
	{
		int rs=writePos-pos;
		//int rd=writePos;
		if(rs<0) rs+=m_nBufferSize;
		for(int i=0;i<len;i++)
		{
			m_pData[writePos]=m_pData[rs];
			rs++;
			if(rs>=(int)m_nBufferSize) 
				rs=0;
			writePos++;
			if(writePos>=m_nBufferSize) writePos=0;
		}
		m_nDataSize+=len;
		return len;
	}
	
	//if(len>(int)m_nDataSize) len=m_nDataSize;
	if(len<=0) return 0;
	if((int)writePos>=pos)
	   Write(m_pData+writePos-pos,len);
	else
	{
	   //int lPos=m_pBufferSize-pos+writePos;
	   //int nSize=
	   int lPos=pos-writePos;
	   if(lPos>=len)
	   	   Write(m_pData+m_nBufferSize-lPos,len);
	   else
	   {
		   Write(m_pData+m_nBufferSize-lPos,lPos);
		   Write(m_pData,len-lPos);
	   }
	   //int nSize=writePos;
	   //int lSize=len-nSize;
	   //len-=nSize;
	   //Write(m_pData+m_nBufferSize-lSize,lSize);
	   //Write(m_pData,nSize);
	   
	}
//	if(m_nDataSize>m_nBufferSize)
//		int v=0;
	return len;
}

int XRingBuffer::WriteTo(XRingBuffer &buf, XU32 nSize)
{
	if(nSize>m_nDataSize) nSize=m_nDataSize;
	if(nSize>buf.GetFreeSize()) nSize=buf.GetFreeSize();
	if(nSize<=0) return 0;
	if(readPos+nSize<=m_nBufferSize)
	{
		buf.Write(m_pData+readPos,nSize);
		readPos+=nSize;
	}
	else
	{
		int size=m_nBufferSize-nSize;
		buf.Write(m_pData+readPos,size);
		buf.Write(m_pData,nSize-size);
		readPos=nSize-size;
	}
	m_nDataSize-=nSize;
	return nSize;
}

int XRingBuffer::WriteToStream(XStream *pStream,XU32 nSize)
{
    if(IsEmpty()) return 0;
	if(nSize==0||nSize>m_nDataSize) 
		nSize=m_nDataSize;
	if(readPos+nSize<=m_nBufferSize)
	{
		pStream->WriteData(m_pData+readPos,nSize);
		readPos+=nSize;
	}
	else
	{
		int size=m_nBufferSize-readPos;
		pStream->WriteData(m_pData+readPos,size);
		pStream->WriteData(m_pData,nSize-size);
		readPos=nSize-size;
	}
	m_nDataSize-=nSize;
	return nSize;
}

XU8* XRingBuffer::ClearNoFree()
{
	XU8* pData=m_pData;
	m_pData=XNULL;
	m_nBufferSize=0;
	Reset();
	return pData;
}


void XRingBuffer::AttachFrom(XPCVOID pData, int nLength,XU8 bMem)
{
	 Release();
	 m_pData=(XU8*)pData;
	 m_nBufferSize=nLength;
	 m_bAttach=XTRUE;
	 if(!bMem)
	 {
		writePos=nLength;
		m_nDataSize=writePos;
	 }
}
void XRingBuffer::AttachTo(XU8Array&data)
{
	data.DataFrom(m_pData,m_nBufferSize);
	m_pData=XNULL;
	m_nBufferSize=0;
	m_nDataSize=0;
	Reset();
}

XU8 XRingBuffer::Init(XU32 nSize)
{
	if(!m_bAttach&&m_pData!=XNULL) 
		delete m_pData;
	m_bAttach=0;
	m_pData=new XU8[nSize];
	if(m_pData==XNULL) return XFALSE;
	m_nBufferSize=nSize;
	m_nDataSize=0;
	return XTRUE;
}
XRingBuffer::XRingBuffer()
{
	m_pData=XNULL;
	m_nDataSize=0;
	m_nBufferSize=0;
	readPos=writePos=0;
	m_bAttach=0;
}

void XRingBuffer::Release()
{
   if(m_pData!=XNULL)
   {
	if(!m_bAttach)
		delete m_pData;	
	m_pData=XNULL;
   }
   Reset();
}

XBOOL  XRingBuffer::Write(XPCVOID pData,XU32 nSize)
{
	//if(!pData)return XFALSE;
	if(nSize>m_nBufferSize-m_nDataSize) return XFALSE;
	if(writePos+nSize<=m_nBufferSize)
	{
		if(pData!=XNULL)
			XCOPY(m_pData+writePos,(XU8*)pData,nSize);
		writePos+=nSize;
		if(writePos==m_nBufferSize) writePos=0;
	}
	else
	{
		XU32 ns=m_nBufferSize-writePos;
		if(pData!=XNULL)
		{
			XCOPY(m_pData+writePos,(XU8*)pData,ns);
			XCOPY(m_pData,(XU8*)pData+ns,nSize-ns);
		}
		writePos=nSize-ns;
	}
	m_nDataSize+=nSize;
	
	return XTRUE;
}
XBOOL XRingBuffer::Read(XPVOID pData,XU32 nSize)
{
	if(nSize>m_nDataSize) return XFALSE;
	if(readPos+nSize<=m_nBufferSize)
	{
		if(pData!=XNULL)
			XCOPY((XU8*)pData,m_pData+readPos,nSize);
		readPos+=nSize;
		if(readPos==m_nBufferSize) readPos=0;
	}
	else
	{
	   XU32 ns=m_nBufferSize-readPos;
	   if(pData!=XNULL)
	   {
		XCOPY((XU8*)pData,m_pData+readPos,ns);
		XCOPY((XU8*)pData+ns,m_pData,nSize-ns);
	   }
	   readPos=nSize-ns;
	}
	m_nDataSize-=nSize;
	if(m_nDataSize==0)
	{
		readPos=writePos=0;
	}
	return XTRUE;
}
XBOOL XRingBuffer::LockRead(XLock&lock,XU32 nSize)
{		
	lock.SetObject(&m_lock);
	if(!lock.Lock(XWAIT_MONMENT)) return XFALSE;
	if(!CanRead(nSize)) 
	{
		lock.UnLock();
		return XFALSE;
	}
	return XTRUE;
}
XU8 XRingBuffer::NextByte()
{
	XU8 v=m_pData[readPos];
	m_nDataSize--;
	readPos++;
	if(readPos>=m_nBufferSize) readPos=0;
	return v;
}
XBOOL XRingBuffer::LockWrite(XLock&lock,XU32 nSize)
{
	lock.SetObject(&m_lock);
	if(!lock.Lock(XWAIT_MONMENT)) return XFALSE;
	if(!CanWrite(nSize)) 
	{
		lock.UnLock();
		return XFALSE;
	}
	return XTRUE;
}

XPCTSTR XStream::ReadString()
{
	FlushReadBits();
	XPCTSTR buf=(XPCTSTR)m_pData+readPos;
	readPos+=XString8::SafeStrlen(buf)+1;
 	/*while(readPos<writePos)
	{
	   if(m_pData[readPos]==0){ readPos++;break;}
	   readPos++;
	}*/
	return buf;
}
