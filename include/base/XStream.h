// XStream.h: interface for the XStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSTREAM_H__1B1885E6_9077_4A95_9572_6EA90FFC03DA__INCLUDED_)
#define AFX_XSTREAM_H__1B1885E6_9077_4A95_9572_6EA90FFC03DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XString.h"
#include "XMutex.h"
#include "GGType.h"	// Added by ClassView

class XFile;
///////////////////////////////////////////////////////////////////
// ѭ������֧����,֧�ֶ��̷߳���
#define BLOCKSIZE 32768
#define FREEBLOCK 256
#define XWAIT_TIME 1000
#define XWAIT_MONMENT 20

class XStream;

class _XEXT_CLASS XRingBuffer
{
public:
	XU8* ClearNoFree();
	int WriteToStream(XStream*pStream,XU32 nSize);
	int WriteTo(XRingBuffer&buf,XU32 nSize);
	void AttachFrom(XPCVOID pData, int nLength,XU8 bMem);	
	void AttachTo(XU8Array&data);	
	XU8 CopyPrev(int pos,int len);
	XU8* GetAddres(){return m_pData;}
	XRingBuffer(XU32 nSize)
	{
		m_bAttach=XNULL;
		m_pData=XNULL;
		Init(nSize);
	}
	XU8 Init(XU32 nSize);
	XRingBuffer();
	
	~XRingBuffer()
	{	
		 Release();
	}
	void Release();
	void Reset()
	{
		readPos=writePos=0;
		m_nDataSize=0;
	}
	/*XU8 SetSize(XUINT nSize)
	{
		if(m_pData!=XNULL) delete m_pData;
		m_nBufferSize=nSize;
		m_pData=new XU8[nSize];
		eturn m_pData!=XNULL;
	}*/
	//��ȡ�������ַ
	XU8* Memory()	{ return m_pData;	}	// Useful to get the origin of the stream memory. 
	XU8* ReadAddr()   {return m_pData+readPos; }
	XU8* WriteAddr()  {return m_pData+writePos;}
	
	XBOOL IsEmpty() {return m_nDataSize==0;}
	XBOOL IsFull()  {return m_nDataSize>=m_nBufferSize;}
	XBOOL CanWrite(XU32 nSize) {return m_nBufferSize-m_nDataSize>=nSize;}
	XBOOL CanRead(XU32  nSize) {return m_nDataSize>=nSize;}
	XU32  GetDataSize()		{return m_nDataSize;}
	XU32  GetFreeSize()	    {return m_nBufferSize-m_nDataSize;}
	XBOOL  Write(XPCVOID pData,XU32 nSize);	
	XBOOL Read(XPVOID pData,XU32 nSize);	
	XBOOL LockRead(XLock&lock,XU32 nSize=0);	
	XU8 NextByte();
	XBOOL LockWrite(XLock&lock,XU32 nSize);
protected:
	XMutex m_lock;
	XU32 readPos,writePos,m_nBufferSize;
	XU32 m_nDataSize;
	XU8* m_pData;
	XU8  m_bAttach;
};

/////////////////////////////////////////////////////
// SWF�������࣬���������λ��ʽ�������
// ����Ӹ����ж���������FLASH����
class _XEXT_CLASS XStream : public XVector<XU8>
{
public:
	void* DataTo()
	{
		XU8*p=m_pData;
		m_pData=XNULL;
		m_nSize=m_nMaxSize=0;
		Reset();
		return p;
	}
	void SetBuffer(int nSize){SetSize(nSize,XFALSE);}
	XStream(XPCVOID pData,int nLength)
	{
		AttachFrom(pData,nLength);
	}
	XU32 GetDataSize(){return writePos-readPos;}
	XU32 GetFreeSize(){return m_nMaxSize-writePos;}
	void AttachFrom(XPCVOID pData, int nLength,XU8 bMem=0);
	
	XU8* ClearNoFree()
	{
		XU8* pData=m_pData;
		m_pData=XNULL;
		m_nMaxSize=m_nSize=0;
		Reset();
		return pData;
	}
	void AttachTo(XU8Array&data)
	{
		data.DataFrom(m_pData,m_nSize);
		m_pData=XNULL;
		m_nSize=m_nMaxSize=0;
		Reset();
	}

	//д���ֽ�
	void WriteByte(XU8 data)
	{
		FlushWriteBits();
		Add(data);
	}
	XU8* ReadAddr(){return m_pData+readPos;}
	XU8* WriteAddr(){return m_pData+writePos;}
	XU8* BufferAddr(){return m_pData;}
	void WriteByte(XS8 data) {WriteData(&data,sizeof(char));}
	void WriteWord(XS16 data){WriteData(&data,sizeof(short));}
	void WriteWord(XU16 data) {WriteData(&data,sizeof(XU16));}
	void WriteDWord(XU32 data){WriteData(&data,sizeof(XU32));}
	void WriteDWord(XS32 data) {WriteData(&data,sizeof(long));}
	void WriteDouble(double data){WriteData(&data,sizeof(double));}
	void WriteDWord64(XS64 data){WriteData(&data,sizeof(XS64));}
	void WriteFloat(float data){WriteData(&data,sizeof(float));}
	void WriteString(XString8&string)
	{	WriteData((XPCTSTR)string,string.GetLength());}
	void WriteString(XPCTSTR pData)
	{
		if(pData==XNULL) return;
		WriteData(pData,strlen(pData)+1);
	}
	void WriteEU32(XU32 data);
	void WriteDWord24(XU32 data);
	int  WriteToFile(XFile&file,int nLength=0);
	//void WriteFixed16(double data);
	//void WriteFixed32(double data);
	//д����ݿ鵽��
	void WriteData(XPCVOID pData,int nLength);
	//���ļ��ж�����ݵ�������
	int  ReadFromFile(XFile&file,int nLength);
	XU8 NextByte(){return m_pData[readPos++];}
	void FlushWriteBits()
	{
		if(writeBytePos!=8)
		{
			writePos++;
			writeBytePos=8;
		}
	}
	//int GetDataSize(){return writePos-readPos;}
public:
	XPCTSTR ReadString();
	//void AttachTo(XU8Array&data);
	//void AttachData(XPCVOID pData,int nLength,XU8 bMem=0);
	// ��ȡһ���ַ�
	XBOOL ReadString(XString8&string,int nLen);
	XBOOL Skip(int nLength);
	XBOOL ReadData(void*pData,int nLength);
	// ����ʣ������λ
	void FlushReadBits()
	{
		if(readBytePos!=8)
		{
			readPos++;
			readBytePos=8;
		}
	}
	// ��黺��������Ƿ�Ϊ��
	XBOOL IsEmpty(){ return writePos<=readPos; }
	//����һλ
	//XU8 Bit(){ return (m_pData[readPos]>>(readBytePos-1))&1;}
	//XU8 ReadBit();
	// ����һ���䳤����
	XBOOL ReadEU32(XU32&data);
	// ����һ��64λ����
	XBOOL ReadDWord64(XU64& data);
	//����һ��������
	XBOOL ReadFixed16(double&data);
	XBOOL ReadFixed32(double&data);
	// ����һ��32λ����
	XBOOL ReadDWord(XU32&data);
	XBOOL ReadDWord24(XU32&data);
	XBOOL ReadDWord(long&data){return ReadDWord((XU32&)data);}
	XBOOL ReadDouble(double&data);
	XBOOL ReadFloat(float&data){return ReadDWord((XU32&)data);}
	XBOOL ReadFloat16(float&data);
	// ����һ��16λ����
	XBOOL ReadWord(XU16&data);
	XBOOL ReadWord(short&data){return ReadWord((XU16&)data);}
	// ����һ��8λ����
	XBOOL ReadByte(XU8&data);
	XBOOL ReadByte(char&data){return ReadByte((XU8&)data);}

	// ����ָ��λ��ı���
	void WriteBits(int data,int size);
	XBOOL ReadBits0(XU32&data,XU8 size);
	XBOOL ReadBits(XU32&data,XU8 size);
	XBOOL ReadBits(XS16&data,XU8 size);
	XBOOL ReadBits(XU8&data,XU8 size)
	{
		//if(size>8) return XFALSE;
		XU32 d32=0;
		if(!ReadBits(d32,size)) return XFALSE;
		data=(XU8)d32;
		return XTRUE;
	}
	XBOOL ReadBits(XU16&data,XU8 size)
	{
		//if(size>8) return XFALSE;
		XU32 d32=0;
		if(!ReadBits(d32,size)) return XFALSE;
		data=(XU16)d32;
		return XTRUE;
	}
	XBOOL ReadBits(XS32&data,XU8 size);	
	void Reset();
	//��ȡ��ݳ���
	XU32 Bytes(){return writePos-readPos;}
	XU32 Bits(){ return ((writePos-readPos)<<3)+(writeBytePos-readBytePos);}

	XStream();
	void RestoreBits(XU8 bits)
	{
		if(bits<=0) return;
		readBytePos+=bits-1;
		readPos-=readBytePos>>3;
		readBytePos=(readBytePos&7)+1;
	}
	void FreeReadSpace()
	{
		if(readPos>FREEBLOCK)
		{
			RemoveAt(0,readPos);
			writePos-=readPos;
			readPos=0;
		}
	}
	virtual ~XStream();
	void SetParam(XU32 p){nParam=p;}
	XU32 GetParam(){return nParam;}
protected:
	
	XU32  readPos,bAttach;			 //��ȡ��ǰ�������λ��
	XU8   readBytePos;			 //��ǰ�ֽڵ�ʣ��λ��
	XU32  writePos;
	XU8   writeBytePos;
protected:
	XU32  nParam;
	////////////////////////////////////////
	// �ͷſռ�
	
	//�����SWFд����
};

/*#define XENUM_UNKNOW -1

class _XEXT_CLASS XEnum
{
public:
	struct TOKEN
	{
		XPCTSTR strKey;
		XU8		ID;
	};
	XEnum()
	{
		m_nCount=0;
		m_pList=XNULL;
	}
	XEnum(XPCTSTR*pList,XU32 nCount)
	{
		m_nCount=nCount;
		m_pList=pList;
	}
	void AddEnum(XPCTSTR strEnum){m_list.Add(strEnum);}
	XINT Match(XPCTSTR pData,XBOOL bFirst=XFALSE)
	{
		XU32 nCount=m_nCount;
		if(nCount<=0) nCount=m_list.GetSize();
		XU32 id=0;
		for(XU32 i=0;i<nCount;i++)
		{
			XString8 strTmp=m_nCount==0?m_list[i]:m_pList[i];
			XString8Array list;
			list.SplitString(strTmp);
			if(list.GetSize()>=2)
			{
				strTmp=list[0];
				id=list[1].ToInt();
			}
			if((bFirst&&pData[0]==strTmp[0])||strTmp.Compare(pData,XTRUE)==0)
				return id;
			id++;
		}
		return XENUM_UNKNOW;
	}
	XPCTSTR operator[](XU32 nIndex)
	{	 
		if(m_nCount>0)
		{
			return nIndex<m_nCount?m_pList[nIndex]:XNULL;
		}
		else
		{
			return nIndex<m_list.GetSize()?(XPCTSTR)m_list[nIndex]:XNULL;
		}
		//return (XPCTSTR)//m_nCount>0?(nIndex<m_nCount?m_pList[nIndex]:XNULL):
		//	(m_nCount<m_list.GetSize()?m_list[nIndex]:XNULL);
	}
	static XBOOL SetValue(XString8Array&list,XPCTSTR strKey,XPCTSTR strValue)
	{
		for(XU32 i=0;i<list.GetSize();i+=2)
		{
		   if(list[i].Compare(strKey,XTRUE)==0)
		   {
			   if(list[i+1]==strValue) return XFALSE;
			   list[i+1]=strValue;
			   return XTRUE;
		   }
		}
		list.Add(strKey);
		list.Add(strValue);
		return XTRUE;
	}
	static XBOOL SetValue(XString8Array&list,XPCTSTR strValue)
	{
		for(XU32 i=0;i<list.GetSize();i++)
			if(list[i].Compare(strValue,XTRUE)==0) return XFALSE;
		list.Add(strValue);
		return XTRUE;
	}
	static int Find(XString8Array&list,XPCTSTR strValue,XBOOL bLowCase=XFALSE)
	{
		for(XU32 i=0;i<list.GetSize();i++)
			if(list[i].Compare(strValue,bLowCase)==0) return i;
		return -1;
	}
	static int Find(XPCTSTR list[],int nCount,XString8&strValue,XBOOL bLowCase=XFALSE)
	{
	    for(int i=0;i<nCount;i++)
			if(strValue.Compare(list[i],bLowCase)==0) return i;
		return -1;
	}
public:
	XString8Array m_list;
	XPCTSTR		  *m_pList;
	XU32		  m_nCount;
};*/

#endif // !defined(AFX_XSTREAM_H__1B1885E6_9077_4A95_9572_6EA90FFC03DA__INCLUDED_)
