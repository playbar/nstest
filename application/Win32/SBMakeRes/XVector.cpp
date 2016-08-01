// XVector.cpp: implementation of the XVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XVector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static XU8 _bitMask[]={0x1,0x3,0x7,0xf,0x1f,0x3f,0x7f,0xff};

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

void XStream::WriteBits(XU32 data, XU8 size)
{
	if(size==0) return;
	
	//SetSize(
	//if(readPos>=writePos) return XFALSE;
	//data=0;
	while(size>0)
	{
		 //当前字节的剩余位数足够
		 if(writeBytePos>=size)
		 {
			 m_pData[writePos]|=(data&_bitMask[size-1])<<(writeBytePos-size);
			 writeBytePos-=size;
			 if(writeBytePos<1)
			 {
				 writeBytePos=8;
				 writePos++;
				 SetSize(writePos+1);
			 }
			 size=0;
			 /*data|=(m_pData[readPos]>>(readBytePos-size))&
				   _bitMask[size-1];
			 readBytePos-=size;
			 if(readBytePos<1)
			 {
				 readBytePos=8;
				 readPos++;
			 }
			 size=0;*/
		 }
		 //当前字节的剩余位数不够
		 else 
		 {
			 int sb=size-writeBytePos;
			 m_pData[writePos]|=(data>>(sb))&_bitMask[writeBytePos-1];//&_bitMask[writeBytePos-1]);
			 
			 //(data<<writeBytePos)>>writeBytePos;
			 data&= ~((data>>sb)<<sb);
			 size-=writeBytePos;
			 data=
				 data&_bitMask[size];
			 writeBytePos=8;
			 writePos++;
			 SetSize(writePos+1);
			 /*data=data<<readBytePos;
			 data|=m_pData[readPos]&_bitMask[readBytePos-1];
			 readPos++;
			 size-=readBytePos;
			 readBytePos=8;*/
		 }
	}	
	//FreeSpace();
//	return XTRUE;
	
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




