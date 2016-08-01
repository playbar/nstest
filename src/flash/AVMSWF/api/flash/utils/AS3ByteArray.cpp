#include "StdAfxflash.h"
#include "ShellCore.h"

using namespace avmplus;


#include "AS3ByteArray.h"
#include "AS3Endian.h"
#include "zlib.h"
// sjj
#include "gzipapis.h"
//#include "mozzconf.h"
#include "genericzlib.h"
#include "sobject.h"

namespace avmshell{
ByteArrayClass::ByteArrayClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ByteArrayObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
	m_pCharSetNS = core()->getPublicNamespace(0);
	m_pCharSetMap = new (core()->GetGC())MultinameHashtable();
	m_nDefaultEncoding=kEncodeDefault;
	BuildCharSetMap();
	((ShellToplevel*)toplevel())->shellClasses[abcclass_flash_utils_ByteArray]=this;
};
ScriptObject* ByteArrayClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	//Modify By xie peifu for Init
	ByteArrayObject*pObj=(ByteArrayObject*)new(core()->GetGC(), ivtable->getExtraSize()) ByteArrayObject(ivtable, prototype, 0);
	void*pInitData=ivtable->GetInitData();
	if(pInitData)
	{
		_XSCharacter*ch=(_XSCharacter*)pInitData;
		if(ch->length)
		{
			//pObj->WriteBytes(ch->data,ch->length);
			pObj->Attach(ch->data,ch->length);
			pObj->AS3_position_set(0);
		}
	}
	return pObj;
	//AS3 'new' opt...Add your init code here...
}

ByteArrayObject* ByteArrayClass::CreateArray(int nSize)
{
	ByteArrayObject*pObject=(ByteArrayObject*)createInstance(ivtable(),prototype);
	pObject->AS3_constructor();

	pObject->GetByteArray().SetLength(nSize);
	//pObject->GetByteArray().Seek(0);
	return pObject;
}

//dushixiong 2011-3-31 --start for charSet
void ByteArrayClass::AddCharSetMap(Stringp charSet, char_set charSetID)
{
	m_pCharSetMap->add(charSet, m_pCharSetNS, (Binding)charSetID);
}

void ByteArrayClass::BuildCharSetMap()
{
	AddCharSetMap(core()->internConstantStringLatin1("euc-cn"), EUC_CN);
	AddCharSetMap(core()->internConstantStringLatin1("gb2312"), gb2312);
	AddCharSetMap(core()->internConstantStringLatin1("utf-8"), utf_8);
	AddCharSetMap(core()->internConstantStringLatin1("unicode"), unicode);
	AddCharSetMap(core()->internConstantStringLatin1("us-ascii"), us_ascii);
}

int ByteArrayClass::GetCharSetFormMap(Stringp charSet)
{
	charSet=core()->internString(charSet->toLowerCase());
	Binding bind = m_pCharSetMap->get(charSet, m_pCharSetNS);
	if (bind == BIND_NONE)
	{
		return -1;
	}
	return (int)bind;
}
//dushixiong 2011-3-31 --end for charSet

ByteArrayObject::ByteArrayObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0), m_byteArray(toplevel())
{
	//Add your construct code here...
	m_cleaner.set(&m_byteArray, sizeof(DataFile));
	//m_defaultObjectEncoding = kEncodeDefault;
	m_bLocked = false;
	m_bCleard = false;
	ShellToplevel *pTop = (ShellToplevel *)toplevel();

	m_pByteArrayClass = pTop->getByteArrayClass();
	m_byteArray.SetObjectEncoding((ObjectEncoding)m_pByteArrayClass->AS3_defaultObjectEncoding_get());
	m_bShareable = false;
}

//////////////////////////////////////////////////////////
//Native Method start...
uint32_t ByteArrayClass::AS3_defaultObjectEncoding_get()
{
	//Add your act code here...
	return m_nDefaultEncoding;
}

void ByteArrayClass::AS3_defaultObjectEncoding_set(uint32_t defaultObjectEncoding)
{
	//Add your act code here...
//	m_defaultObjectEncoding = (ObjectEncoding)defaultObjectEncoding;
	if (defaultObjectEncoding != kAMF0 && defaultObjectEncoding != kAMF3)
	{
		toplevel()->throwArgumentError(kInvalidArgumentError, INTString(defaultObjectEncoding));//"defaultObjectEncoding");
	}

	m_nDefaultEncoding = (ObjectEncoding)defaultObjectEncoding;
}

uint32_t ByteArrayObject::AS3_bytesAvailable_get()
{
	//Add your act code here...
	return m_byteArray.BytesAvailable();
}

Stringp ByteArrayObject::AS3_endian_get()
{
	//Add your act code here...
	EndianClass*pClass=((ShellToplevel*)toplevel())->getEndianClass();
	return (m_byteArray.GetEndian() == kBigEndian) ? pClass->getSlotBIG_ENDIAN():pClass->getSlotLITTLE_ENDIAN();
		//core()->internConstantStringLatin1("bigEndian") : core()->internConstantStringLatin1("littleEndian");
}

void ByteArrayObject::AS3_endian_set(Stringp endianSet)
{
	//Add your act code here...
	AvmCore* core = this->core();
	endianSet = core->internString(endianSet);
	EndianClass*pClass=((ShellToplevel*)toplevel())->getEndianClass();
	if (endianSet == pClass->getSlotBIG_ENDIAN())//core->internConstantStringLatin1("bigEndian"))
	{
		m_byteArray.SetEndian(kBigEndian);
	}
	else if (endianSet == pClass->getSlotLITTLE_ENDIAN())//core->internConstantStringLatin1("littleEndian"))
	{
		m_byteArray.SetEndian(kLittleEndian);
	}
	else
	{
		toplevel()->throwArgumentError(kInvalidArgumentError,endianSet);// "endianSet");
	}
}

uint32_t ByteArrayObject::AS3_length_get()
{
	//Add your act code here...
	return m_byteArray.GetLength();
}

void ByteArrayObject::AS3_length_set(uint32_t length)
{
	//Add your act code here...
	m_byteArray.SetLength(length);
}

uint32_t ByteArrayObject::AS3_objectEncoding_get()
{
	//Add your act code here...
	return m_byteArray.GetObjectEncoding();
}

void ByteArrayObject::AS3_objectEncoding_set(uint32_t objectEncoding)
{
	//Add your act code here...
	if (objectEncoding != kAMF0 && objectEncoding != kAMF3)
	{
		toplevel()->throwArgumentError(kInvalidArgumentError,INTString(objectEncoding));// "objectEncoding");
	}

	m_byteArray.SetObjectEncoding((ObjectEncoding)objectEncoding);
}

uint32_t ByteArrayObject::AS3_position_get()
{
	//Add your act code here...
	return m_byteArray.GetFilePointer();
}

void ByteArrayObject::AS3_position_set(uint32_t offset)
{
	//Add your act code here...
	if (offset >= 0)
//	if (offset >= 0 && offset < m_byteArray.GetLength())
	{
		m_byteArray.Seek(offset);
	}
}

//AS3 contructor function..
void ByteArrayObject::AS3_constructor(void)
{
	//Add your act code here...

}


void ByteArrayObject::AS3_inflate()
{
#ifdef GZIPBUF
	int len = m_byteArray.GetLength();
	if (len <= 0) // 0 or (uint)len > max int
		return;

	int result_len = 0;//0x7FFFFFFF;
	CFileAlikeBuffer bin(len, (char *)m_byteArray.GetBuffer(), len);
	CFileAlikeBuffer bout(result_len);
	
	if (gzip_decompress(&bin, &bout) < 0)
	{
//#if (__CORE_VERSION__>=0x02070200)
		toplevel()->throwError(kShellCompressedDataError);
//#endif
		return;
	}

	m_byteArray.SetLength(0);
	m_byteArray.Write(bout.GetBuffer(), bout.Length());
	m_byteArray.Seek(0);
#else
	this->AS3_uncompress();
#endif
}

void ByteArrayObject::AS3_deflate()
{
#ifdef GZIPBUF
	int len = m_byteArray.GetLength();
	if (len <= 0) // 0 or (uint)len > max int
		return;

	int result_len = 0;//0x7FFFFFFF;
	CFileAlikeBuffer bin(len, (char *)m_byteArray.GetBuffer(), len);
	CFileAlikeBuffer bout(result_len);
	if (gzip_compress(&bin, &bout) < 0 || bout.Length()<18)
	{
//#if (__CORE_VERSION__>=0x02070200)
		toplevel()->throwError(kShellCompressedDataError);
//#endif
		return; // compress error
	}

	m_byteArray.SetLength(0);
	if(bout.Length()>18)
		m_byteArray.Write(bout.GetBuffer()+10, bout.Length()-18);
	m_byteArray.Seek(0);
#else
	this->AS3_compress();
#endif
}

void ByteArrayObject::AS3_compress()
{
	//FILE *pFile = fopen( "d://temp.dat", "wb" );
	//fwrite( m_byteArray.GetBuffer(), m_byteArray.GetLength(), 1, pFile );
	//fclose( pFile );
	//Add your act code here...
	int len = m_byteArray.GetLength();
	if (!len) // empty buffer should give us a empty result
		return;	//����ֵ�Ƿ�ǡ��

	unsigned long gzlen = len * 3/2 + 32; // enough for growth plus zlib headers
	uint8_t *gzdata = mmfx_new_array(uint8_t, gzlen);

	// Use zlib to compress the data
	compress2((uint8_t*)gzdata, (unsigned long*)&gzlen,
		m_byteArray.GetBuffer(), len, 9);

	// Replace the byte array with the compressed data
	m_byteArray.SetLength(0);
	//m_byteArray.WriteU32((U32)len);
	m_byteArray.Write(gzdata, gzlen);

	mmfx_delete_array( gzdata );
}

void ByteArrayObject::AS3_uncompress()
{
	// Snapshot the compressed data.
	unsigned long gzlen = m_byteArray.GetLength();
	//if ( gzlen == 9011)
	//{
	//	int i = 0;
	//}
	if (!gzlen) // empty buffer should give us a empty result
		return;		//����ֵ�Ƿ�ǡ��

/*	if(gzlen==71614)
	{
		int len = m_byteArray.GetLength();
		if (len <= 0) // 0 or (uint)len > max int
			return;

		int result_len = 0;//0x7FFFFFFF;
		CFileAlikeBuffer bin(len, (char *)m_byteArray.GetBuffer(), len);
		CFileAlikeBuffer bout(result_len);
		if (gzip_compress(&bin, &bout) < 0 || bout.Length()<18)
		{
	//#if (__CORE_VERSION__>=0x02070200)
			toplevel()->throwError(kShellCompressedDataError);
	//#endif
			return; // compress error
		}

		m_byteArray.SetLength(0);
		if(bout.Length()>18)
			m_byteArray.Write(bout.GetBuffer()+10, bout.Length()-18);
		m_byteArray.Seek(0);
		return;
	}*/
	//	int a=0;
		//gzlen = m_byteArray.GetLength();

	uint8_t *gzdata = mmfx_new_array( uint8_t, gzlen );
	VMPI_memcpy(gzdata, m_byteArray.GetBuffer(), gzlen);

	/*XFile file;
	file.Open("e:\\comp.dat",XFile::XWRITE|XFile::XBINARY|XFile::XCREATE);
	file.Write(gzdata,gzlen);
	file.Close();*/

	// Clear the buffer
	m_byteArray.Seek(0);
	m_byteArray.SetLength(0);

	// The following block is to force destruction
	// of zstream before potential exception throw.
	//int nTotalIn=0;
	int error = Z_OK;
	{
		// Decompress the data
		PlatformZlibStream zstream;
		zstream.SetNextIn(gzdata);
		zstream.SetAvailIn(gzlen);

		const int kBufferSize = 8192;
		uint8_t *buffer = mmfx_new_array( uint8_t, kBufferSize );

		//XFile file;
		//file.Open("D:\\dc.dat",XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
		do {
			zstream.SetNextOut(buffer);
			zstream.SetAvailOut(kBufferSize);
			error = zstream.InflateWithStatus();
			m_byteArray.Write(buffer, kBufferSize - zstream.AvailOut());
			//file.Write(buffer,kBufferSize - zstream.AvailOut());
		} while (error == Z_OK);

		//file.Close();

		//nTotalIn=zstream.AvailIn();
		if (error != Z_OK && error != Z_STREAM_END) {
			m_byteArray.SetLength(gzlen);//.Write(gzdata,gzlen);
			//toplevel()->throwError(kShellCompressedDataError);
		}

		//XFile file;
		//file.Open("D:\\dc.dat",XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
		//file.Write(m_byteArray.,8192);
		//file.Close();
	
		mmfx_delete_array( buffer );
		mmfx_delete_array( gzdata );
	}

	// position byte array at the beginning
	m_byteArray.Seek(0);

	if (error != Z_OK && error != Z_STREAM_END) {
			m_byteArray.SetLength(gzlen);//.Write(gzdata,gzlen);
			toplevel()->throwError(kShellCompressedDataError);
		}


}

bool ByteArrayObject::AS3_readBoolean()
{
	//Add your act code here...
	return m_byteArray.ReadBoolean();
}

int32_t ByteArrayObject::AS3_readByte()
{
	//Add your act code here...
	return m_byteArray.ReadByte();
}

void ByteArrayObject::AS3_readBytes(ByteArrayObject* byteArray, uint32_t offset, uint32_t length)
{
	//Add your act code here...
	CheckNull(byteArray, "byteArray");

	if (length == 0) {
		length = m_byteArray.BytesAvailable();
	}

	m_byteArray.ReadBytes(byteArray->GetByteArray(), offset, length);
}

double ByteArrayObject::AS3_readDouble()
{
	//Add your act code here...
	return m_byteArray.ReadDouble();
}

double ByteArrayObject::AS3_readFloat()
{
	//Add your act code here...
	return m_byteArray.ReadFloat();
}

int32_t ByteArrayObject::AS3_readInt()
{
	//Add your act code here...
	return m_byteArray.ReadInt();
}

void ByteArrayObject::WriteMultiByte(Stringp data, char_set charSet)
{
	//char_set cSet = (char_set)m_pByteArrayClass->GetCharSetFormMap(charSet);
	switch (charSet)
	{
	default:
	case EUC_CN:
	case gb2312:
	case us_ascii:
		//pRet = BuildUpMultiByte(pBuffer, byteLen);
		{
			StUTF8String utf8Data(data);
			XString8 str8(utf8Data.c_str());
			str8.ConvertToGB();
			m_byteArray.Write(str8.GetData(), str8.GetLength());
		}
		break;
	case utf_8:
		//pRet = BuildUpUTF8(pBuffer, byteLen);
		{
			//StUTF8String s(data);
			//this->WriteBytes(s.c_str(),s.length());
			WriteUTF8(data);
		}
		break;
	case unicode:
		{
			//StUTF16String s(data);
			//this->WriteUTF16
			//this->WriteBytes(s.c_str(),s.length());
			WriteUTF16(data);
		}
		//pRet = BuildUpUTF16((const wchar *)pBuffer, byteLen);
		break;
	//default:
	//	break;
	}

}

Stringp ByteArrayObject::AS3_readMultiByte(uint32_t length, Stringp charSet)
{
	//Add your act code here...
	char_set cSet = (char_set)m_pByteArrayClass->GetCharSetFormMap(charSet);


	//Stringp data = NULL;

	//switch (cSet)
	//{
	//case EUC_CN:
	//case gb2312:
	//case us_ascii:
	//	data = ReadData(length);
	//	return ConvertToMultiByte(data, cSet);

	//case utf_8:
	//	data = ReadData(length);
	//	return ConvertToUTF8(data, cSet);

	//case unicode:
	//	data = ReadData(length);
	//	return ConvertToUTF16(data, cSet);

	//default:
	//	break;
	//}

	return ReadData(length, cSet);
}

Atom ByteArrayObject::AS3_readObject()
{
	/*if ( GetLength() == 329739 )
	{
		int i = 0;
	}*/
	//Add your act code here...
	//if(this->GetLength()==63498)
	//{
	//	int v=0;
	//	XFile file;
	//	file.Open("D:\\objects.dat",XFile::XBINARY|XFile::XCREATE|XFile::XWRITE);
	//	file.Write(m_byteArray.GetBuffer() + m_byteArray.GetFilePointer(), GetLength() - m_byteArray.GetFilePointer() );
	//	file.Close();
	//}

	Atom atom=nullObjectAtom;
	bool bSet=false;


	TRY(core(),kCatchAction_ReportAsError)
	{

		if(refString==NULL)
		{
			refString=toplevel()->arrayClass->newArray(0);
			refObjects=toplevel()->arrayClass->newArray(0);
			refTraits=toplevel()->arrayClass->newArray(0);
			bSet=true;
		}
		if(this->m_byteArray.GetObjectEncoding()==kAMF0)
		{
			atom=m_byteArray.ReadObject0(this->atom(),refString,refObjects,refTraits);
		}
		else
		{
			atom=m_byteArray.ReadObject(this->atom(),refString,refObjects,refTraits);
		}
	}
	CATCH(Exception *exception)
	{
		refString=NULL;
		refObjects=NULL;
		refTraits=NULL;
		core()->throwException(exception);
		//_TraceOut(exception,"Find Class",strName);
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY
	if(bSet)
	{
		refString=NULL;
		refObjects=NULL;
		refTraits=NULL;
	}
	return atom;
	//return (void*)nullObjectAtom;
}

int32_t ByteArrayObject::AS3_readShort()
{
	//Add your act code here...
	return m_byteArray.ReadShort();
}


uint32_t ByteArrayObject::AS3_readUnsignedByte()
{
	//Add your act code here...
	return m_byteArray.ReadUnsignedByte();
}

uint32_t ByteArrayObject::AS3_readUnsignedInt()
{
	//Add your act code here...
	return m_byteArray.ReadUnsignedInt();
}

uint32_t ByteArrayObject::AS3_readUnsignedShort()
{
	//Add your act code here...
	return m_byteArray.ReadUnsignedShort();
}

Stringp ByteArrayObject::AS3_readUTF()
{
	//Add your act code here...
	return m_byteArray.ReadUTF();
}

Stringp ByteArrayObject::AS3_readUTFBytes(uint32_t length)
{
	//Add your act code here...
	return m_byteArray.ReadUTFBytes(length);
}

Stringp ByteArrayObject::AS3_toString()
{
	//Add your act code here...
	union
	{
		uint8_t*	c;
		wchar*		c_w;
	};

	c = (uint8_t*)m_byteArray.GetBuffer();
	uint32_t len = m_byteArray.GetLength();

	if (len >= 3)
	{
		// UTF8 BOM
		if ((c[0] == 0xef) && (c[1] == 0xbb) && (c[2] == 0xbf))
		{
			return core()->newStringUTF8((const char*)c + 3, len - 3);
		}
		else if ((c[0] == 0xfe) && (c[1] == 0xff))
		{
			//UTF-16 big endian
			//c += 2;
			len = (len - 2) >> 1;
			return core()->newStringEndianUTF16(/*littleEndian*/false, c_w+1, len);
		}
		else if ((c[0] == 0xff) && (c[1] == 0xfe))
		{
			//UTF-16 little endian
			len = (len-2) >> 1;
			return core()->newStringEndianUTF16(/*littleEndian*/true, c_w+1, len);
		}
	}

	// Use newStringUTF8() with "strict" explicitly set to false to mimick old,
	// buggy behavior, where malformed UTF-8 sequences are stored as single characters.
	return core()->newStringUTF8((const char*)c, len, false);
}

void ByteArrayObject::AS3_writeBoolean(bool data)
{
	//Add your act code here...
	m_byteArray.WriteBoolean(data);
}

void ByteArrayObject::AS3_writeByte(int32_t data)
{
	//Add your act code here...
	m_byteArray.WriteByte(data);
}

ByteArrayObject* ByteArrayObject::Clone()
{
	ByteArrayClass*pClass = ((ShellToplevel*)toplevel())->getByteArrayClass();
	ByteArrayObject*pClone = pClass->CreateArray(this->GetLength());
	pClone->WriteBytes(m_byteArray.GetBuffer(),GetLength());
	pClone->Seek(this->GetFilePointer());
	return pClone;
}

void ByteArrayObject::AS3_writeBytes(ByteArrayObject *data, uint32_t offset, uint32_t length)
{
	//Add your act code here...
	CheckNull(data, "bytes");

	if (length == 0) {
		length = data->GetLength() - offset;
	}

	//if(m_byteArray.GetLength()==504402)
	//	int v=0;

	//if(m_byteArray.GetFilePointer()>=504000)
	//	int v=0;

	m_byteArray.WriteBytes(data->GetByteArray(), offset, length);
}

void ByteArrayObject::AS3_writeDouble(double data)
{
	//Add your act code here...
	m_byteArray.WriteDouble(data);
}

void ByteArrayObject::AS3_writeFloat(double data)
{
	//Add your act code here...
	m_byteArray.WriteFloat((float)data);
}

void ByteArrayObject::AS3_writeInt(int32_t data)
{
	//Add your act code here...
	m_byteArray.WriteInt(data);
}

void ByteArrayObject::AS3_writeMultiByte(Stringp data, Stringp charSet)
{

	//char_set cSet = (char_set)m_pByteArrayClass->GetCharSetFormMap(charSet);

	//UTF16
	//StUTF16String utf16Data(data);
	//Stringp pEndianData = core()->newStringEndianUTF16(false, (const wchar *)utf16Data.c_str());
	//StUTF16String utf16EndianData(pEndianData);

//	int byteLen = data->get_length()<<data->getWidth();

	//StUTF8String	utf8Data(data);
	//StUTF16String	utf16Data(data);
	//XString8 str8((XPCTSTR)utf8Data.c_str(), STRING_ATTACH);

	char_set cSet = (char_set)m_pByteArrayClass->GetCharSetFormMap(charSet);
	switch (cSet)
	{
	default:
	case EUC_CN:
	case gb2312:
	case us_ascii:
		WriteMultiByte(data, cSet);
		break;
	case utf_8:
		//m_byteArray.Write(utf8Data.c_str(), strlen(utf8Data.c_str()));
		WriteUTF8(data);
		break;
	case unicode:
		WriteUTF16(data);
		break;
	//default:
	//	break;
	}
}

void ByteArrayObject::AS3_writeObject(Atom atom)
{
	//Add your act code here...
	//ScriptObject*pObj=AvmCore::atomToScriptObject(atom);
	if(refObjects==NULL)
	{
		refString=toplevel()->arrayClass->newArray(0);
		refObjects=toplevel()->arrayClass->newArray(0);
		//refTraits=toplevel()->arrayClass->newArray(0);
	}
	TRY(core(),kCatchAction_ReportAsError)
	{


		if(this->m_byteArray.GetObjectEncoding()==kAMF0)
		{
			//ArrayObject*refObject=toplevel()->arrayClass->newArray(0);
			m_byteArray.WriteObject0(atom,refObjects);
		}
		else
		{
			//ArrayObject*refObject=toplevel()->arrayClass->newArray(0);
			//ArrayObject*refString=toplevel()->arrayClass->newArray(0);
			m_byteArray.WriteObject(this->atom(),atom,refString,refObjects);
		}
	}
	CATCH(Exception *exception)
	{
		refString=NULL;
		refObjects=NULL;
		refTraits=NULL;
		core()->throwException(exception);
		//_TraceOut(exception,"Find Class",strName);
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY
	refString=NULL;
	refObjects=NULL;
}

void ByteArrayObject::AS3_writeShort(int32_t data)
{
	//Add your act code here...
	m_byteArray.WriteShort(data);
}


void ByteArrayObject::AS3_writeUnsignedInt(uint32_t data)

{
	//Add your act code here...

	m_byteArray.WriteUnsignedInt(data);

}

void ByteArrayObject::AS3_writeUTF(Stringp data)
{
	//Add your act code here...
	m_byteArray.WriteUTF(data);
}

void ByteArrayObject::AS3_writeUTFBytes(Stringp data)
{
	//Add your act code here...
	m_byteArray.WriteUTFBytes(data);
}

void ByteArrayObject::CheckNull(void *instance, const char *name)
{
	if (instance == NULL)
	{
		toplevel()->throwTypeError(kNullArgumentError,core()->knull);
	}
}


Atom ByteArrayObject::getUintProperty(uint32_t i) const
{
	if (i < (uint32_t)m_byteArray.GetLength()) {
		return core()->intToAtom(m_byteArray[i]);
	} else {
		return undefinedAtom;
	}
}

void ByteArrayObject::setUintProperty(uint32_t i, Atom value)
{

	m_byteArray[i] = (U8)(AvmCore::integer(value));
}

Atom ByteArrayObject::getAtomProperty(Atom name) const
{
	uint32_t index;
	if (AvmCore::getIndexFromAtom(name, &index)) {
		if (index < (uint32_t) m_byteArray.GetLength()) {
			return core()->intToAtom(m_byteArray[index]);
		} else {
			return undefinedAtom;
		}
	}

	return ScriptObject::getAtomProperty(name);
}

void ByteArrayObject::setAtomProperty(Atom name, Atom value)
{
	uint32_t index;
	if (AvmCore::getIndexFromAtom(name, &index)) {
		int intValue = AvmCore::integer(value);
		m_byteArray[index] = (U8)(intValue);
	} else {
		ScriptObject::setAtomProperty(name, value);
	}
}

bool ByteArrayObject::hasAtomProperty(Atom name) const
{
	return ScriptObject::hasAtomProperty(name) || getAtomProperty(name) != undefinedAtom;
}

Stringp ByteArrayObject::ReadData(uint32_t byteLen, char_set charSet)
{
	m_byteArray.CheckEOF(byteLen);

//	char *pBuffer = mmfx_new_array_opt( char, byteLen + 1, MMgc::kCanFail );
	char *pBuffer = mmfx_new_array_opt( char, byteLen, MMgc::kCanFail );

	if (!pBuffer)
	{
		toplevel()->throwError(kOutOfMemoryError);
	}

	m_byteArray.Read(pBuffer, byteLen);
//	pBuffer[byteLen] = 0;

	Stringp pRet = NULL;

	// Since this is supposed to read UTF8 into a string, it really should ignore the UTF8 BOM that
	// might reasonably occur at the head of the data.
	switch (charSet)
	{
	default:
	case EUC_CN:
	case gb2312:
	case us_ascii:
		pRet = BuildUpMultiByte(pBuffer, byteLen);
		break;
	case utf_8:
		pRet = BuildUpUTF8(pBuffer, byteLen);
		break;
	case unicode:
		pRet = BuildUpUTF16((const wchar *)pBuffer, byteLen);
		break;
	//default:
	//	break;
	}

	mmfx_delete_array(pBuffer);

	return pRet;
}


//Stringp ByteArrayObject::WriteMultiByte(Stringp data, char_set charSet)
//{
//	String::Width width = data->getWidth();
//	//void *pRet = NULL;
//
//	if (String::k8 == width)
//	{
//		StUTF8String utf8Data(data);
//
//		XString8 str8((XPCTSTR)utf8Data.c_str(), STRING_ATTACH);
//		str8.ConvertToGB();
//		return core()->newStringLatin1(str8.GetData());
//	}

	//else if (String::k16 == width)
	//{
	//	XString16 str16(data->GetBuffer(), STRING_ATTACH);
	//	XString8 str8;
	//	str8.SetSize(data->GetBuffer()<<data->getWidth());
	//	char *pBuffer =
	//}
	//return data;
//}

void ByteArrayObject::WriteUTF8(Stringp data)
{
	StUTF8String utf8Data(data);

	m_byteArray.Write(utf8Data.c_str(), strlen(utf8Data.c_str()));
}

void ByteArrayObject::WriteUTF16(Stringp data)
{
	StUTF16String utf16Data(data);

	wchar *pUtf16Data = (wchar *)utf16Data.c_str();

	for (int i = 1; i <= utf16Data.length(); i++)
	{
		m_byteArray.EndianAdjust16(*pUtf16Data);
		pUtf16Data++;
	}

	m_byteArray.Write(utf16Data.c_str(), utf16Data.length() * sizeof(wchar));
}



Stringp ByteArrayObject::BuildUpUTF8(const char *pData, int byteLen)
{
	if (byteLen >= 3 && (uint8_t)pData[0] == 0xEF && (uint8_t)pData[1] == 0xBB && (uint8_t)pData[2] == 0xBF)
	{
		pData += 3;
		byteLen -= 3;
	}

	return core()->newStringUTF8(pData, byteLen);
}

Stringp ByteArrayObject::BuildUpUTF16(const wchar *pData, int byteLen)
{
	union
	{
		uint8_t*	p;
		wchar*		p_w;
	};

	p = (uint8_t *)pData;


	if ((p[0] == 0xfe) && (p[1] == 0xff))
	{
		//UTF-16 big endian
		p += 2;
		byteLen = (byteLen - 2) >> 1;
		return core()->newStringEndianUTF16(/*littleEndian*/false, p_w, byteLen);
	}
	else if ((p[0] == 0xff) && (p[1] == 0xfe))
	{
		//UTF-16 little endian
		byteLen = byteLen >> 1;
		return core()->newStringEndianUTF16(/*littleEndian*/true, p_w, byteLen);
	}
	else
	{
		byteLen = byteLen >> 1;
		return core()->newStringEndianUTF16(false, pData, byteLen);
	}
}

Stringp ByteArrayObject::BuildUpMultiByte(const char *pData, int byteLen)
{
	//LOGWHERE();
	XString8 str;
#if defined(_SYMBIAN) && defined(_DEBUG)
	SYMBIANDEB1(DefaultLog,"Stringp ByteArrayObject::BuildUpMultiByte   %s\r\n",pData);
#endif
	str.ConvertFrom(pData,XCHARSET_GB2312,byteLen);
#if defined(_SYMBIAN) && defined(_DEBUG)
	SYMBIANDEB1(DefaultLog,"Stringp ByteArrayObject::BuildUpMultiByte   %s\r\n",str.GetData());
#endif
	return core()->newStringUTF8(str.GetData(),str.GetLength());// byteLen);
}

void ByteArrayObject::AS3_clear()
{
	if(m_bLocked)
		m_bCleard = true;
	else
		this->m_byteArray.Clear();
}

//new adds 11
bool ByteArrayObject::AS3_shareable_get()
{
	//LOGWHERE();
	return m_bShareable;//false;
}
void ByteArrayObject::AS3_shareable_set(bool b)
{
	//LOGWHERE();
	m_bShareable = b;
}
 
int ByteArrayObject::AS3_atomicCompareAndSwapIntAt(int byteIndex, int expectedValue, int newValue)
{
	//LOGWHERE();
	//ByteArrayObject* pArray = this;
	//ReadBytes( pArray, byteIndex );
	Seek(byteIndex);
	int data = m_byteArray.ReadInt();
	if (data == expectedValue && data != newValue)
	{
		Seek(byteIndex);
		m_byteArray.WriteInt(newValue);
	}
	return data;
}
int ByteArrayObject::AS3_atomicCompareAndSwapLength(int expectedLength, int newLength)
{
	//LOGWHERE();
	int l = GetLength();
	if(l == expectedLength && l != newLength)
		SetLength(newLength);
	return l;
}
Atom ByteArrayObject::AS3_toJSON(Stringp k)
{
	LOGWHERE();
	return undefinedAtom;
}
//new adds 11 end

}
