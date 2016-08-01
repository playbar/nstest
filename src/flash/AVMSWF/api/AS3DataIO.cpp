#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "../../flash/core/Toplevel.h"
//#include "AS3DataMemory.h"
#include "../ShellCore.h"
#include "splayer.h"

using namespace avmshell;
using namespace ExtErrorContext;

#ifdef _DEBUG
#define _INTString1(II) cr->intToString(II)
#else
#define _INTString1(II) cr->kEmptyString
#endif

#include "AS3DataIO.h"
#include "AS3XMLDocument.h"
#include "AS3ByteArray.h"
#include "AS3Dictionary.h"
namespace avmshell
{
	#define RANGE_BOUND 65535 

	void DataIOBase::SetObjectEncoding(ObjectEncoding objectEncoding)
	{
		m_objectEncoding = objectEncoding;
	}

	void DataInput::CheckEOF(uint32_t count)
	{
		if (BytesAvailable() < count)
		{
			ShellCore *cr = (ShellCore *)m_pcToplevel->core();
			cr->throwExtError(kExtError_EOFError, _INTString1(count));
		}
	}

	bool DataInput::ReadBoolean()
	{
		uint8_t value;		

		Read(&value, 1);
		return value != false;
	}

	uint32_t DataInput::ReadUnsignedByte()
	{
		uint8_t value;

		Read(&value, 1);
		return (uint32_t)value;
	}

	uint32_t DataInput::ReadUnsignedShort()
	{
		uint16_t value;

		Read(&value, 2);
		EndianAdjust16(value);
		return (uint32_t)value;
	}

	uint32_t DataInput::ReadUnsignedInt()
	{
		uint32_t value;
		Read(&value, 4);
		EndianAdjust32(value);
		return value;
	}

	float DataInput::ReadFloat()
	{
		union 
		{
			uint32_t u;
			float f;
		} fValue;

		Read(&fValue.u, 4);
		EndianAdjust32(fValue.u);
		return fValue.f;
	}

	double DataInput::ReadDouble()
	{
		union 
		{
			uint64_t u;
			double d;
		} dValue;

		Read(&dValue.d, 8);
		EndianAdjust64(dValue.u);
		return dValue.d;
	}

	String* DataInput::ReadUTFBytes(uint32_t length)
	{
		CheckEOF(length);	

		char *pBuffer = mmfx_new_array_opt( char, length+1, MMgc::kCanFail );
		if (!pBuffer) 
		{
			m_pcToplevel->throwError(kOutOfMemoryError);
		}

		Read(pBuffer, length);
		pBuffer[length] = 0;

		// Since this is supposed to read UTF8 into a string, it really should ignore the UTF8 BOM that
		// might reasonably occur at the head of the data.
		char* pUTF8chars = pBuffer;
		if (length >= 3 && (uint8_t)pBuffer[0] == 0xEF && (uint8_t)pBuffer[1] == 0xBB && (uint8_t)pBuffer[2] == 0xBF) 
		{
			pUTF8chars += 3;
		}

		Stringp pRet = m_pcToplevel->core()->newStringUTF8(pUTF8chars);
		mmfx_delete_array(pBuffer);

		return pRet;
	}

	String* DataInput::ReadUTF(bool bLong)
	{
		return ReadUTFBytes(bLong?ReadUnsignedInt():ReadUnsignedShort());
	}

	void DataInput::ReadBytes(DataMemory& data, uint32_t offset, uint32_t count)
	{
		CheckEOF(count);

		if (count == 0) {
			count = BytesAvailable();
		}

		// Grow the buffer if necessary
		if (offset + count >= data.GetLength()) {
			data.SetLength(offset + count);
		}

		Read(data.GetBuffer() + offset, count);
	}

	int32_t DataInput::ReadByte()
	{
		int8_t value;

		Read(&value, 1);
		return (int32_t)value;
	}

	int32_t DataInput::ReadShort()
	{
		int16_t value;

		Read(&value, 2);
		EndianAdjust16((uint16_t&)value);
		return (int32_t)value;
	}

	int32_t DataInput::ReadInt()
	{
		int32_t value;

		Read(&value, 4);
		EndianAdjust32((uint32_t&)value);
		return (int32_t)value;
	}

	Stringp DataInput::ReadMultiByte(uint32_t length, Stringp charSet)
	{
		ShellToplevel* pTopLevel = (ShellToplevel *)m_pcToplevel;
		ByteArrayClass *pClass = pTopLevel->getByteArrayClass();

		char_set cSet = (char_set)pClass->GetCharSetFormMap(charSet);
		return ReadData(length, cSet);
	}

	Stringp DataInput::ReadData(uint32_t byteLen, char_set charSet)
	{
		CheckEOF(byteLen);	

		//	char *pBuffer = mmfx_new_array_opt( char, byteLen + 1, MMgc::kCanFail );
		char *pBuffer = mmfx_new_array_opt( char, byteLen, MMgc::kCanFail );

		if (!pBuffer) 
		{
			m_pcToplevel->throwError(kOutOfMemoryError);
		}

		Read(pBuffer, byteLen);
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
		}

		mmfx_delete_array(pBuffer);

		return pRet;
	}

	Stringp DataInput::BuildUpUTF8(const char *pData, int byteLen)
	{
		if (byteLen >= 3 && (uint8_t)pData[0] == 0xEF && (uint8_t)pData[1] == 0xBB && (uint8_t)pData[2] == 0xBF) 
		{
			pData += 3;
			byteLen -= 3;
		}

		return m_pcToplevel->core()->newStringUTF8(pData, byteLen);
	}

	Stringp DataInput::BuildUpUTF16(const wchar *pData, int byteLen)
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
			return m_pcToplevel->core()->newStringEndianUTF16(/*littleEndian*/false, p_w, byteLen);
		}
		else if ((p[0] == 0xff) && (p[1] == 0xfe))
		{
			//UTF-16 little endian
			byteLen = byteLen >> 1;
			return m_pcToplevel->core()->newStringEndianUTF16(/*littleEndian*/true, p_w, byteLen);
		}
		else
		{
			byteLen = byteLen >> 1;
			return m_pcToplevel->core()->newStringEndianUTF16(false, pData, byteLen);
		}
	}

	Stringp DataInput::BuildUpMultiByte(const char *pData, int byteLen)
	{
		return m_pcToplevel->core()->newStringLatin1(pData, byteLen);
	}


	/* *
	Class DataOutput
	* */
	void DataOutput::WriteBoolean(bool data)
	{
		WriteUnsignedByte(data ? 1 : 0);
	}

	void DataOutput::WriteUnsignedByte(uint8_t data)
	{
		Write(&data, 1);
	}

	void DataOutput::WriteByte(int32_t data)
	{
		WriteUnsignedByte((uint8_t)data);
	}

	void DataOutput::WriteUnsignedShort(uint16_t data)
	{
		EndianAdjust16(data);
		Write(&data, 2);
	}

	void DataOutput::WriteShort(int32_t data)
	{
		WriteUnsignedShort((uint16_t)data);
	}

	void DataOutput::WriteInt(int32_t data)
	{
		WriteUnsignedInt((uint32_t)data);
	}

	void DataOutput::WriteUnsignedInt(uint32_t data)
	{
		EndianAdjust32(data);
		Write(&data, 4);
	}

	void DataOutput::WriteFloat(float data)
	{
		union 
		{
			uint32_t u;
			float v;
		} fValue;

		fValue.v = data;
		EndianAdjust32(fValue.u);
		Write(&fValue, 4);
	}

	void DataOutput::WriteDouble(double data)
	{
		union {
			uint64_t u;
			double v;
		} dValue;

		dValue.v = data;
		EndianAdjust64(dValue.u);
		Write(&dValue.u, 8);
	}


	void DataOutput::WriteUTF(String *data,bool bLong)
	{
		StUTF8String utf8(data);
		uint32_t length = utf8.length();
		if (length > RANGE_BOUND) 
		{
			AvmCore*cr=m_pcToplevel->core();
			m_pcToplevel->throwRangeError(kOutOfRangeError,_INTString1(length),_INTString1(RANGE_BOUND));
		}
		if(bLong)
			WriteUnsignedInt(length);
		else
			WriteUnsignedShort((unsigned short)length);
		Write((void *)utf8.c_str(), length * sizeof(char));
	}

	void DataOutput::WriteUTFBytes(String *data)
	{
		StUTF8String utf8(data);
		int len = utf8.length();
		Write((void *)utf8.c_str(), len * sizeof(char));
	}

	void DataOutput::WriteBytes(DataMemory& data, uint32_t offset, uint32_t count)
	{
		if (data.GetLength() < offset)
			offset = data.GetLength();

		if (count == 0) {
			count = data.GetLength()-offset;
		}

		if (count > data.GetLength()-offset) {
			AvmCore*cr=m_pcToplevel->core();
			m_pcToplevel->throwRangeError(kOutOfRangeError,_INTString1(count),_INTString1(data.GetLength()-offset));
		}

		if (count > 0) {
			Write(data.GetBuffer()+offset, count);
		}
	}
	////////////////////////////////////////
	// Add By xie
	int DataInput::ReadU29()
	{
		int value=this->ReadUnsignedByte();//ReadByte();
		if(!(value&0x80))
			return value;
		//if(value==135)
		//	int v=0;
		value&=0x7f;
		int v=this->ReadUnsignedByte();//ReadByte();
		value=(value<<7)|(v&0x7f);
		if(!(v&0x80)) return value;
		v=this->ReadUnsignedByte();//ReadByte();
		value=(value<<7)|(v&0x7f);
		if(!(v&0x80)) return value;
		v=this->ReadUnsignedByte();//ReadByte();
		value=(value<<8)|v;
		if(value>>28)
			value|=0xe0000000;
		return value;

	}


	Stringp DataInput::ReadString(ArrayObject*refObject)
	{
		uint32_t ref=ReadU29();
		int		 iCount=ref>>1;
		
		if(ref&1)
		{
			if(!iCount) return NULL;	
			Stringp name=ReadUTFBytes(iCount);
			name=m_pcToplevel->core()->internString(name);
			//StUTF16String s(name);
			//XSWFPlayer::m_pInstance->Trace(s.c_str());
			//if(strcmp(s.c_str(),"defaultScene")==0)
			//	int v=0;
			Atom atom=name->atom();
			refObject->push(&atom,1);
			return name;
		}
		else
		{
			if(iCount>=refObject->getDenseLength())//;get_length())
			{
				AvmCore*cr=m_pcToplevel->core();
				m_pcToplevel->throwArgumentError(kInvalidArgumentError,_INTString1(iCount));
				//return;
			}
			Atom atom=refObject->getUintProperty(iCount);
			return AvmCore::atomToString(atom);			
			//return name;
		}
	}

	void DataInput::ReadScriptObjectTraits(Atom thisAtom,ScriptObject*pObject,
			ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits,int iRef)
	{
		
		if(iRef>=refTraits->getDenseLength()>>2)//;get_length()>>2)
		{
			AvmCore*cr=m_pcToplevel->core();
			m_pcToplevel->throwArgumentError(kInvalidArgumentError,_INTString1(iRef));
			return;
		}
		ArrayObject*pArray=(ArrayObject*)AvmCore::atomToScriptObject(refTraits->getUintProperty((iRef<<2)+1));
		bool bHash=AvmCore::boolean(refTraits->getUintProperty((iRef<<2)+2));
		AvmCore*c=m_pcToplevel->core();
		int iCount=pArray->getDenseLength();//get_length();
		Namespacep publicNS=c->findPublicNamespace();
		TraitsBindingsp t=pObject->traits()->getTraitsBindings();
		for(int i=0;i<iCount;i++)
		{
			Atom v=ReadObject(thisAtom,refString,refObject,refTraits);
			
			Atom name=pArray->getIntProperty(i);			
			ShellCore::_setProperty(pObject,name,v,publicNS,t);	
		}
		if(bHash)
		{
			for(;;)
			{
				Stringp name=ReadString(refString);//c->internString(ReadUTFBytes(ic));
				if(!name) break;				
				Atom atom=ReadObject(thisAtom,refString,refObject,refTraits);
				ShellCore::_setProperty(pObject,name->atom(),atom,publicNS,t);
				//pObject->setStringProperty(name,atom);
			}
		}
	}

	void	DataInput::ReadMembers0(Atom thisAtom,ScriptObject*pObj,ArrayObject*refString,
								    ArrayObject*refObject,ArrayObject*refTraits,
									int iCount)
	{
		AvmCore*c=m_pcToplevel->core();
		TraitsBindingsp t=pObj->traits()->getTraitsBindings();
		Namespacep publicNS=c->findPublicNamespace();
		for(int i=0;i<iCount;i++)
		{
			Stringp name=ReadUTF();
			//if(name->get_length()==0)
			//	break;
			Atom value=ReadObject0(thisAtom,refString,refObject,refTraits);
			if(name->get_length()<=0||value==kUnusedAtomTag)
			{
				//m_pcToplevel->throwArgumentError(kReadSealedError,c->kEmptyString);
				return;
				//break;
			}
			double id=c->number(name->atom());
			if(id==MathUtils::kNaN)
				ShellCore::_setProperty(pObj,name->atom(),value,publicNS,t);
			else
			{
				((ArrayObject*)pObj)->setIntProperty((int)id,value);
			}
			//if(id>=iCount)
			//	break;
		}
		for(;;)
		{
			Stringp name=c->internString(ReadUTF());
			//StUTF8String ss(name);
			//if(strstr(ss.c_str(),"zhuxian")!=NULL)
			//	int v=0;
			Atom	value=ReadObject0(thisAtom,refString,refObject,refTraits);
			if(name->get_length()==0||value==kUnusedAtomTag)
				break;
			ShellCore::_setProperty(pObj,name->atom(),value,publicNS,t);
		}
	}

	void DataInput::ReadScriptObject(Atom thisAtom,ScriptObject*pObject,int iCount,
									 bool bHash,ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits,Atom classAtom)
	{
		ArrayObject*pArray=m_pcToplevel->arrayClass->newArray(0);
		AvmCore*c=m_pcToplevel->core();
		int i;
		
		for(i=0;i<iCount;i++)
		{
			//uint32_t ic=ReadU29()>>1;
			Stringp name=ReadString(refString);//ReadUTFBytes(ic);
			//name=c->internString(name);
			Atom atom=name->atom();
			pArray->push(&atom,1);
		}
		Atom atom[]={classAtom,pArray->atom(),bHash?trueAtom:falseAtom,falseAtom};
		//bool bAdd=false;
		//if(iCount)
		{
			refTraits->push(atom,4);
			//bAdd=true;
		}
		Namespacep publicNS=c->findPublicNamespace();
		TraitsBindingsp t=pObject->traits()->getTraitsBindings();
		for(i=0;i<iCount;i++)
		{
			Atom v=ReadObject(thisAtom,refString,refObject,refTraits);
			Atom name=pArray->getIntProperty(i);								
			Stringp pname = AvmCore::atomToString(name);
			//if(atomKind(name)==kStringType)
			/*{
				Binding b=t->findBinding(AvmCore::atomToString(name),publicNS);
				if(b==BIND_NONE)
					pObject->setAtomProperty(name,v);
				else
					pObject->coerceAndSetSlotAtom(c->bindingToSlotId(b),v);
			}*/
			ShellCore::_setProperty(pObject,name,v,publicNS,t);
			//else
			//	pObject->setAtomProperty(name,v);
		}
		
		if(bHash)
		{
			for(;;)
			{
				//uint32_t ic=ReadU29()>>1;
				//if(!ic) break;
				Stringp name=ReadString(refString);//c->internString(ReadUTFBytes(ic));
				if(!name) break;
				//StUTF8String ss(name);
				//if(strstr(ss.c_str(),"HomeTime"))
				//{
					//ScriptObject*ppObj=(ScriptObject*)(value&~7);
				//	int v=0;
				//}
				//Atom atom=name->atom();
				//pArray->push(&atom,1);
				/*if(!bAdd)
				{
					bAdd=true;
					Atom atoms[]={classAtom,pArray->atom()};
					refTraits->push(atoms,2);
				}*/
				Atom atom=ReadObject(thisAtom,refString,refObject,refTraits);
				ShellCore::_setProperty(pObject,name->atom(),atom,publicNS,t);
				//pObject->setStringProperty(name,atom);
			}
		}
	}

	Atom DataInput::ReadObject0(Atom thisAtom,ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits)
	{
		uint32_t type=ReadByte();
		ShellCore*c=(ShellCore*)m_pcToplevel->core();
		ShellToplevel*top=(ShellToplevel*)m_pcToplevel;
		switch(type)
		{
		case AMF0_AMF3:
			{
				//ArrayObject*refString=top->arrayClass->newArray(0);
				//ArrayObject*refObject=top->arrayClass->newArray(0);
				//ArrayObject*refTraits=top->arrayClass->newArray(0);
				this->SetObjectEncoding(kAMF3);
				return ReadObject(thisAtom,refString,refObject,refTraits);
			}break;
		case AMF0_NUMBER://=0,
			 return c->doubleToAtom(this->ReadDouble());
			 break;
		case AMF0_BOOLEAN://=1,
			{
				int v=this->ReadByte();
				return v?trueAtom:falseAtom;
			}break;
		case AMF0_STRING://=2,
			{
				return this->ReadUTF()->atom();
			}break;
		case AMF0_OBJECT://=3,
			{
				ScriptObject*obj=top->objectClass->newInstance();
				Atom atom=obj->atom();
				refObject->push(&atom,1);
				ReadMembers0(thisAtom,obj,refString,refObject,refTraits);
				return atom;
			}break;
		case AMF0_MOVIECLIP://=4,
			{
				top->throwTypeError(kUnsupportedTraitsKindError,c->kEmptyString);
			}break;
		case AMF0_NULL://=5,
			return nullObjectAtom;
		case AMF0_UNDEFINED://=6,
			return undefinedAtom;
		case AMF0_REFERENCE://=7,
			{
				XU16 id=ReadShort();
				if(id<0||id>refObject->getDenseLength())//;get_length())
					top->throwTypeError(kNumberOutOfRangeError,c->kEmptyString);
				return refObject->getUintProperty(id);
				//return obj->atom();
			}break;
		case AMF0_ECMAARRAY://=8,
			{
				ArrayObject*pArray=top->arrayClass->newArray();
				XU32 iCount=this->ReadUnsignedInt();
				Atom atom=pArray->atom();
				refObject->push(&atom,1);
				ReadMembers0(thisAtom,pArray,refString,refObject,refTraits,iCount);
				return atom;
			}break;
		case AMF0_OBJECTEND://=9,
			{
				return kUnusedAtomTag;
			}break;
		case AMF0_STRICTARRAY://=0xa,
			{
				ArrayObject*pArray=top->arrayClass->newArray();
				XU32 iCount=this->ReadUnsignedInt();
				Atom atom=pArray->atom();
				refObject->push(&atom,1);
				for(XU32 i=0;i<iCount;i++)
				{
					Atom value=ReadObject0(thisAtom,refString,refObject,refTraits);
					pArray->setIntProperty(i,value);
				}
				return atom;
			}break;
		case AMF0_DATE://=0xb,
			{
				Date time=ReadDouble();
				DateClass*pClass=m_pcToplevel->dateClass(true);

				DateObject*pObj=new (c->GetGC(),pClass->ivtable()->getExtraSize())DateObject(pClass,time);
				Atom atom=pObj->atom();
					//pObj->date=time;
				//	refObject->push(&atom,1);
				return atom;
			}break;
		case AMF0_LONGSTRING://=0x0c,
			{
				return ReadUTF(true)->atom();
			}break;
		case AMF0_UNSUPPORTED://=0x0d,
			{
				//top->throwTypeError(kUnsupportedTraitsKindError,c->kEmptyString);
			}break;
		case AMF0_RECORDSET://=0x0e,
			 break;
		case AMF0_XMLDOC://=0x0f,
			{
				Stringp str=ReadUTF(true);
//#if (__CORE_VERSION__>=0x02070100)
					XMLDocumentObject*pDoc=XNULL;
					TRY(c,kCatchAction_ReportAsError)
					{
						pDoc=((ShellToplevel*)m_pcToplevel)->getXMLDocumentClass()->CreateXML(str);
					}
					CATCH(Exception *exception)
					{
						pDoc=((ShellToplevel*)m_pcToplevel)->getXMLDocumentClass()->CreateXML(XNULL);
					}
					END_CATCH
					END_TRY
//#else					
//				XMLDocumentObject*pDoc=((ShellToplevel*)m_pcToplevel)->getXMLDocumentClass()->CreateXML(str);
//#endif
				Atom atom=pDoc->atom();
				//refObject->push(&atom,1);
				//	return pDoc->atom();
				return atom;
			}
			 break;
		case AMF0_TYPEDOBJECT://=0x10
			{
				Stringp className=ReadUTF();
				className=c->internString(className);
				ClassClosure*cs=c->getAliasClass(className);
				ScriptObject*pObject=NULL;
				if(cs)
				{
					VTable* ivtable = cs->ivtable();
					ScriptObject*prototype = cs->prototype;//AvmCore::atomToScriptObject(c->GetToplevel()->objectClass->get_prototype());
					pObject=cs->createInstance(ivtable,prototype);
					Atom atom=pObject->atom();
					ivtable->init->coerceEnter(0,&atom);//0,&a);//argc, argv);				

					ClassClosure*iext=(ClassClosure*)((ShellToplevel*)m_pcToplevel)->getBuiltinExtensionClass(abcclass_flash_utils_IExternalizable,false);
					bool bExt=ShellCore::IsTypeClass(pObject,iext);
					atom=pObject->atom();
					//refObject->push(&atom,1);
					if(bExt)
					{
						Multiname name;
						name.setName(c->kReadExternal);
						name.setNamespace(c->getPublicNamespace(0));
						
						VTable*vtable=pObject->vtable;//->vtable();
						Binding b = avmplus::getBinding(m_pcToplevel, vtable, &name);
						
						
						if(AvmCore::bindingKind(b)==BKIND_METHOD)
						{
							MethodEnv*fun=vtable->methods[AvmCore::bindingToMethodId(b)];
							fun->coerceEnter(pObject->atom(),1,&atom);
						}
					}		
					//return nullObjectAtom;
				}
				if(!pObject)
					pObject=top->objectClass->newInstance();				
				Atom atom=pObject->atom();
				refObject->push(&atom,1);
				ReadMembers0(thisAtom,pObject,refString,refObject,refTraits);
				//ReadScriptObject(thisAtom,pObject,iCount,bHash,refString,refObject,refTraits,nullObjectAtom);
				//Atom atom=pObject->atom();
				//refObject->push(&atom,1);
				return atom;
			}
			 break;
		}
		return nullObjectAtom;
	}

	Atom DataInput::ReadObject(Atom thisAtom,ArrayObject*refString,ArrayObject*refObject,ArrayObject*refTraits)
	{
		//ScriptObject*pObj=m_pcToplevel->objectClass->newInstance();
		uint32_t type=ReadByte();
		ShellCore*c=(ShellCore*)m_pcToplevel->core();

		switch(type)
		{
		case AMF_DICTIONARY:
			 //type=ReadByte();
			 //goto Again;
			 {
				uint32_t l=ReadU29();
				int		 iCount=l>>1;
				if(l&1)
				{
					DictionaryObject*pObj=((ShellToplevel*)m_pcToplevel)->getDictionaryClass()->CreateDictionary();
					Atom atom=pObj->atom();
					refObject->push(&atom,1);
					int iSet=ReadByte();//[Note X]
					for(int i=0;i<iCount;i++)
					{
						Atom key=ReadObject(thisAtom,refString,refObject,refTraits);
						Atom value=ReadObject(thisAtom,refString,refObject,refTraits);
						pObj->setAtomProperty(key,value);
					}
					return atom;
				}
				else
					return refObject->getUintProperty(iCount);
			 }
			 break;
		case AMF_UNDEFINE:
			 return undefinedAtom;
		case AMF_NULL:
			 return nullObjectAtom;
		case AMF_FALSE:
			 return falseAtom;
		case AMF_TRUE:
			 return trueAtom;
		case AMF_INT:
			 {
				int v=ReadU29();
				return c->intToAtom(v);
			 }break;
		case AMF_DOUBLE:
			 {
				double v=ReadDouble();
				return c->doubleToAtom(v);
			 }break;
		case AMF_STRING:
			{
				Stringp s=ReadString(refString);
				if(s) return s->atom();
/*#ifdef _WINEMU&&_DEBUG
				{
					ByteArrayObject*pObj=(ByteArrayObject*)AvmCore::atomToScriptObject(thisAtom);
					XFile file;
					file.Open("D:\\error.dat",XFile::XBINARY|XFile::XCREATE|XFile::XWRITE);
					file.Write(pObj->GetByteArray().GetBuffer(),pObj->GetLength());
					file.Close();
				}
#endif*/
				return
					c->kEmptyString->atom();
					//nullObjectAtom;
					//c->intToAtom(l>>1);
			}break;
		case AMF_XMLDOC:
			{
				uint32_t l=this->ReadU29();//>>1;
				int		 iCount=l>>1;
				//if(!iCount) 
				//	return nullObjectAtom;
				if(l&1)
				{					
					Stringp s=this->ReadUTFBytes(l>>1);
//#if (__CORE_VERSION__>=0x02070100)
					XMLDocumentObject*pDoc=XNULL;
					TRY(c,kCatchAction_ReportAsError)
					{
						pDoc=((ShellToplevel*)m_pcToplevel)->getXMLDocumentClass()->CreateXML(s);
					}
					CATCH(Exception *exception)
					{
						pDoc=((ShellToplevel*)m_pcToplevel)->getXMLDocumentClass()->CreateXML(XNULL);
					}
					END_CATCH
					END_TRY
//#else
//					XMLDocumentObject*pDoc=((ShellToplevel*)m_pcToplevel)->getXMLDocumentClass()->CreateXML(s);
//#endif
					Atom atom=pDoc->atom();
					refObject->push(&atom,1);
					return pDoc->atom();

				}
				else
					return refObject->getUintProperty(iCount);
				//Stringp s=this->ReadUTFBytes(l);
				//return s->atom();
			}break;
		case AMF_DATE:
			{
				uint32_t f=ReadU29();
				int iCount=f>>1;
				//if(!iCount) return nullObjectAtom;
				if(f&1)
				{
					Date time=ReadDouble();
					DateClass*pClass=m_pcToplevel->dateClass(true);

					DateObject*pObj=new (c->GetGC(),pClass->ivtable()->getExtraSize())DateObject(pClass,time);
					Atom atom=pObj->atom();
					//pObj->date=time;
					refObject->push(&atom,1);
					return atom;
				}
				else return refObject->getUintProperty(iCount);
					//(DateObject*)
					//m_pcToplevel->dateClass()->newInstance();
			}break;
		case AMF_ARRAY:
			{
				uint32_t i,iRef=ReadU29();
				int iCount=iRef>>1;
				if((iRef&1)==0) return refObject->getUintProperty(iCount);
				ArrayObject*pArray=m_pcToplevel->arrayClass->newArray(0);
				//ArrayObject*pStack=m_pcToplevel->arrayClass->newArray(0);
				// Modify for Ref Object
				Atom atom=pArray->atom();
				refObject->push(&atom,1);
				for(;;)
				{
					Stringp name=ReadString(refString);
					if(!name) break;					
					Atom value=ReadObject(thisAtom,refString,refObject,refTraits);
					pArray->setAtomProperty(name->atom(),value);
				}
				if(iCount)
				{
					for(i=0;i<iCount;i++)
					{
						Atom value=ReadObject(thisAtom,refString,refObject,refTraits);
						pArray->setIntProperty(i,value);
					}
				}
				//if(!iCount)
				
				//else
				//Atom atom=pArray->atom();
				//refObject->push(&atom,1);
				return atom;
			}
			break;
		case AMF_OBJECT:
			{
				uint32_t ref=ReadU29();
				
				if(!(ref&1))
				{
					return refObject->getUintProperty((ref>>1));
				}
				if(!(ref&2))
				{
					ScriptObject*pObject=NULL;//m_pcToplevel->objectClass->newInstance();
					int iRef=ref>>2;
					if(iRef<refTraits->getDenseLength()>>1)//get_length()>>1)
					{
						ClassClosure*cs=(ClassClosure*)AvmCore::atomToScriptObject(refTraits->getIntProperty(iRef<<2));
						if(cs)
						{
							VTable* ivtable = cs->ivtable();
		
							ScriptObject*prototype = cs->prototype;//AvmCore::atomToScriptObject(c->GetToplevel()->objectClass->get_prototype());
							pObject=cs->createInstance(ivtable,prototype);
							Atom atom=pObject->atom();
							ivtable->init->coerceEnter(0,&atom);//0,&a);//argc, argv);
							if(AvmCore::boolean(refTraits->getIntProperty((iRef<<2)+3)))
							{
								refObject->push(&atom,1);
								Multiname name;
								name.setName(c->kReadExternal);
								name.setNamespace(c->getPublicNamespace(0));								
								VTable*vtable=pObject->vtable;//->vtable();
								Binding b = avmplus::getBinding(m_pcToplevel, vtable, &name);
								if(AvmCore::bindingKind(b)==BKIND_METHOD)
								{
									MethodEnv*fun=vtable->methods[AvmCore::bindingToMethodId(b)];
									fun->coerceEnter(pObject->atom(),1,&thisAtom);
								}
								return atom;
							}
						}
						
					}
					if(!pObject)
						pObject=m_pcToplevel->objectClass->newInstance();
					// Modify for Ref Object
					Atom atom=pObject->atom();
					refObject->push(&atom,1);
					ReadScriptObjectTraits(thisAtom,pObject,refString,refObject,refTraits,iRef);
					//Atom atom=pObject->atom();
					//refObject->push(&atom,1);
					return atom;
				}
				Stringp  className=ReadString(refString);//NULL;
				//bool bExterl=(ref&7)==7;
				bool bHash=(ref&8)!=0;
				uint32_t iCount=ref>>4;
				if(ref&4) iCount=0;
				//uint32_t iName=ReadU29()>>1;
				
				/*if(iName)
				{
					className=this->ReadUTFBytes(iName);
					refString->push(
				}*/
				ScriptObject*pObject=NULL;
				if(className)
				{  //
					
					className=c->internString(className);
					ClassClosure*cs=((ShellCore*)c)->getAliasClass(className);
					/*if(!cs)
					{
						Multiname name;
						name.setName(className);
						name.setNamespace(c->getPublicNamespace(0));
						m_pcToplevel->throwReferenceError(kUndefinedVarError,name);
					}
					else*/
					if(cs)
					{
						VTable* ivtable = cs->ivtable();
	
						ScriptObject*prototype = cs->prototype;//AvmCore::atomToScriptObject(c->GetToplevel()->objectClass->get_prototype());
						pObject=cs->createInstance(ivtable,prototype);
						Atom atom=pObject->atom();
						ivtable->init->coerceEnter(0,&atom);//0,&a);//argc, argv);				

						ClassClosure*iext=(ClassClosure*)((ShellToplevel*)m_pcToplevel)->getBuiltinExtensionClass(abcclass_flash_utils_IExternalizable,true);
						bool bExt=ShellCore::IsTypeClass(pObject,iext);
						// Modify for Ref Object
						atom=pObject->atom();
						refObject->push(&atom,1);
						if(bExt)
						{
							Multiname name;
							name.setName(c->kReadExternal);
							name.setNamespace(c->getPublicNamespace(0));
							ArrayObject*pArray=m_pcToplevel->arrayClass->newArray(0);
							Atom objAtoms[]={cs->atom(),pArray->atom(),falseAtom,trueAtom};
							refTraits->push(objAtoms,4);
							VTable*vtable=pObject->vtable;//->vtable();
							Binding b = avmplus::getBinding(m_pcToplevel, vtable, &name);
							if(AvmCore::bindingKind(b)==BKIND_METHOD)
							{
								MethodEnv*fun=vtable->methods[AvmCore::bindingToMethodId(b)];
								fun->coerceEnter(pObject->atom(),1,&thisAtom);
							}
						}
						else
						{
							ReadScriptObject(thisAtom,pObject,iCount,bHash,refString,refObject,refTraits,cs->atom());
						}
						//atom=pObject->atom();
						//refObject->push(&atom,1);
						return atom;
					}					
					//return nullObjectAtom;
				}
				//if(className==NULL)

				pObject=m_pcToplevel->objectClass->newInstance();
				// Modify for Ref Object
				Atom atom=pObject->atom();
				refObject->push(&atom,1);
				ReadScriptObject(thisAtom,pObject,iCount,bHash,refString,refObject,refTraits,nullObjectAtom);
				//Atom atom=pObject->atom();
				//refObject->push(&atom,1);
				return atom;
			}break;
		case AMF_XML:
			{
				int l=ReadU29();
				int iCount=l>>1;
				if(l&1)
				{
					Stringp str=ReadUTFBytes(iCount);
					
					XMLClass*pClass=m_pcToplevel->xmlClass();
					//Atom atom=str->atom();
					Atom atoms[]={nullObjectAtom,str->atom()};
					Atom atom=pClass->construct(1,atoms);
					refObject->push(&atom,1);
					return atom;
				}
				else
					return refObject->getUintProperty(iCount);
				//XMLObject*pObj=m_pcToplevel->xmlClass()->
			}break;
		case AMF_BYTEARRAY:
			{
				int l=ReadU29();
				int iCount=l>>1;
				if(l&1)
				{
					ByteArrayClass*pClass=((ShellToplevel*)m_pcToplevel)->getByteArrayClass();
					ByteArrayObject*pObj=pClass->CreateArray(iCount);
					Atom atom=pObj->atom();
					refObject->push(&atom,1);
					//pObj->AS3_readBytes(
					if(iCount)
					{
						//DataMemory buffer;
						//buffer.SetLength(iCount);
						//ReadBytes(buffer,0,iCount);
						//pObj->GetByteArray().WriteBytes(buffer,0,iCount);
						ReadBytes(pObj->GetByteArray(),0,iCount);
						pObj->Seek(0);
					}
					
					return atom;
				}
				return 
					refObject->getUintProperty(iCount);
			}
			 break;
			 
		case AMF_VECTORINT:
			{
				int len = ReadU29();
				
				//Stringp utfvr = ReadString( refString );
				int i,iCount = len >> 1;
				if ( len & 1 )
				{
					bool fixlen = ReadByte();
					IntVectorClass *pClass = m_pcToplevel->intVectorClass;
					IntVectorObject *pObject = pClass->newVector( iCount );
					pObject->set_fixed(fixlen);
					Atom atom = pObject->atom();
					refObject->push( &atom, 1 );
					if( iCount )
					{
						for(i=0;i<iCount;i++)
							pObject->m_array[i]=ReadInt();
						//Read(pobject->m_array, iCount * sizeof( uint32 ) );
					}
					
					return atom;
				}
				else
					return refObject->getUintProperty( iCount );
				
			}
			break;
		case AMF_VECTORUINT:
			{
				int len = ReadU29();
				
				//Stringp utfvr = ReadString( refString );
				int i,iCount = len >> 1;
				if ( len & 1 )
				{
					bool fixlen = ReadByte();
					UIntVectorClass *pClass = m_pcToplevel->uintVectorClass;
					UIntVectorObject *pObject = pClass->newVector( iCount );
					pObject->set_fixed(fixlen);
					Atom atom = pObject->atom();
					refObject->push( &atom, 1 );
					if( iCount )
					{
						for(i=0;i<iCount;i++)
							pObject->m_array[i]=ReadUnsignedInt();
						//Read(pobject->m_array, iCount * sizeof( uint32 ) );
					}
					
					return atom;
				}
				else
					return refObject->getUintProperty( iCount );
				
			}
			break;
		case AMF_VECTORDOUBLE:
			{
				int len = ReadU29();
				
				//Stringp utfvr = ReadString( refString );
				int i,iCount = len >> 1;
				if ( len & 1 )
				{
					bool fixlen = ReadByte();
					DoubleVectorClass *pClass = m_pcToplevel->doubleVectorClass;
					DoubleVectorObject *pObject = pClass->newVector( iCount );
					pObject->set_fixed(fixlen);
					Atom atom = pObject->atom();
					refObject->push( &atom, 1 );
					if( iCount )
					{
						for(i=0;i<iCount;i++)
							pObject->m_array[i]=ReadDouble();
						//Read(pobject->m_array, iCount * sizeof( uint32 ) );
					}
					
					return atom;
				}
				else
					return refObject->getUintProperty( iCount );
				
			}
			break;
		case AMF_VECTOROBJECT:
			{
				int len = ReadU29();
				
				//Stringp utfvr = ReadString( refString );
				int i,iCount = len >> 1;
				if ( len & 1 )
				{
					bool fixlen = ReadByte();
					
					Stringp alias= ReadString(refString);
					//if(!alias)
					//{
					VectorClass *pClass = m_pcToplevel->vectorClass;
					ObjectVectorObject *pObject = NULL;//pClass->newVector( iCount );
					
					if(alias==NULL)
						pObject = pClass->newVector(m_pcToplevel->objectClass,iCount);
						//pObject->set_type(m_pcToplevel->objectClass->atom);
					else
					{
						ClassClosure*cs=((ShellCore*)c)->getAliasClass(alias);
						if(cs)
							pObject = pClass->newVector(cs,iCount);
						else
							pObject = pClass->newVector(m_pcToplevel->objectClass,iCount);
							//pObject->set_type(cs->atom());
					}
					pObject->set_fixed(fixlen);
					Atom atom = pObject->atom();
					refObject->push( &atom, 1 );
					if( iCount )
					{
						for(i=0;i<iCount;i++)
						{
							Atom vAtom = ReadObject(thisAtom,refString,refObject,refTraits);
							pObject->setUintProperty(i,vAtom);
							//pObject->m_array[i]=ReadObject(thisAtom,refString,refObject,refTraits);
						}
						//
						//Read(pobject->m_array, iCount * sizeof( uint32 ) );
					}
					
					return atom;
					
					//else
					//{
					//	//className=c->internString(className);
					//	ClassClosure*cs=((ShellCore*)c)->getAliasClass(alias);
					//	VectorClass *pClass = m_pcToplevel->vectorClass;
					//	ObjectVectorObject *pObject = pClass->newVector(cs,iCount );
					//	pObject->set_fixed(fixlen);
					//	
					//	if( iCount )
					//	{
					//		for(i=0;i<iCount;i++)
					//			pObject->m_array[i]=ReadObject(thisAtom,refString,refObject,refTraits);
					//		//Read(pobject->m_array, iCount * sizeof( uint32 ) );
					//	}
					//	Atom atom = pObject->atom();
					//	refObject->push( &atom, 1 );
					//	return atom;
					//}
					
				}
				else
					return refObject->getUintProperty( iCount );
				
			}
			break;
		default:
			LOGWHERE();
			FLASHASSERT();
			break;
		}

		return nullObjectAtom;
	}
	
	void DataOutput::WriteScriptObjectEx(Atom thisAtom,ScriptObject*pObj,Stringp alias,ArrayObject*refString,ArrayObject*refObject)
	{
		ShellCore*core=(ShellCore*)m_pcToplevel->core();

		ClassClosure*iext=(ClassClosure*)((ShellToplevel*)m_pcToplevel)->getBuiltinExtensionClass(abcclass_flash_utils_IExternalizable,true);
		bool bExt=ShellCore::IsTypeClass(pObj,iext);

		uint32 t=AMF_OBJECT;
		WriteByte(t);
		//t=7;
		if(!bExt)
			//t=((core->GetSlotsCount(pObj,true,true,true))<<4)|3;
			t=(WriteMembers(thisAtom,pObj,AMF_OUTCOUNT,refString,refObject)<<3)|3;
		else
			t=7;
		this->WriteU29(t);
		WriteString(alias);
		if(bExt)
		{
			Multiname name;
			name.setName(core->kWriteExternal);
			name.setNamespace(core->getPublicNamespace(0));
			
			VTable*vtable=pObj->vtable;//->vtable();
			Binding b = avmplus::getBinding(m_pcToplevel, vtable, &name);
			if(AvmCore::bindingKind(b)!=BKIND_METHOD) return;
			MethodEnv*fun=vtable->methods[AvmCore::bindingToMethodId(b)];
		
			//Atom atoms[]={pObj->atom(),thisAtom};
			fun->coerceEnter(pObj->atom(),1,&thisAtom);
		}
		else
		{
			ArrayObject*pArray=m_pcToplevel->arrayClass->newArray(0);
			WriteMembers(thisAtom,pObj,AMF_OUTNAME,refString,refObject);
			WriteMembers(thisAtom,pObj,AMF_OUTVALUE,refString,refObject);
		}
		/*if(pObj->hasStringProperty(core->kWriteExternal))//hasMultinameProperty(&name))
		{
			Atom atoms[]={pObj->atom(),thisAtom};
			pObj->callProperty(&name,1,atoms);
		}*/
	}

	void DataOutput::WriteScriptObject(Atom thisAtom,ScriptObject*pObj,ArrayObject*refString,ArrayObject*refObject)
	{
		uint32 t=AMF_OBJECT;
		WriteByte(t);
		t=WriteMembers(thisAtom,pObj,AMF_OUTCOUNT,refString,refObject);
			//((ShellCore*)m_pcToplevel->core())->GetSlotsCount(pObj,true,true,true);
		t=(t<<3)|3;
		WriteU29(t);
		t=1;
		WriteByte(t);
		ArrayObject*pArray=m_pcToplevel->arrayClass->newArray(0);
		WriteMembers(thisAtom,pObj,AMF_OUTNAME,refString,refObject);
		WriteMembers(thisAtom,pObj,AMF_OUTVALUE,refString,refObject);
		//WriteByte(1);
	}

	

	void DataOutput::WriteArray(Atom thisAtom,ArrayObject*pObj,ArrayObject*refString,ArrayObject*refObject)
	{
		uint32 t=AMF_ARRAY;
		WriteByte(t);
		t=pObj->getDenseLength();//get_length();//ShellCore::GetSlotsCount(pObj,true);
	
		//if(!t&&pObj->get_length())
		//	int v=0;

		t=(t<<1)|1;
		WriteU29(t);
		//ArrayObject*pArray=m_pcToplevel->arrayClass->newArray(0);
		//WriteMembers(pObj,pArray,AMF_OUTNAME|AMF_OUTVALUE);

		InlineHashtable* b=pObj->getTable();
		int index=0,iSet=0;
		AvmCore*c=m_pcToplevel->core();
		for(;;)
		{
			index=b->next(index);
			if(index<=0) break;
			//Stringp str=c->atomToString(b->keyAt(index));
			Atom key=b->keyAt(index);
			if(!c->isNullOrUndefined(key))
			{
				Atom atom=b->valueAt(index);
				Stringp str=c->string(key);
				WriteString(str);
				WriteObject(thisAtom,atom,refString,refObject);
				//pArray->push(&atom,1);
			}
		}

		t=1;
		WriteByte(t);
		t=pObj->getDenseLength();//get_length();
		//if(!pObj->getDenseLength())//hasDense())
	
		for(uint32 i=0;i<t;i++)
			WriteObject(thisAtom,pObj->getIntProperty(i),refString,refObject);
		//t=1;
		//WriteByte(t);
	}


	void DataOutput::WriteDictionary(Atom thisAtom,ScriptObject*pObj,ArrayObject*refString,ArrayObject*refObject)
	{
		uint32 t=AMF_DICTIONARY;
		WriteByte(t);

		WriteByte(0);//[Note X]

		t=WriteDictMembers(thisAtom,pObj,refString,refObject,false);
		this->WriteU29(t|1);
		WriteDictMembers(thisAtom,pObj,refString,refObject,true);

	}

	int	DataOutput::WriteDictMembers(Atom thisAtom,ScriptObject*pObj,ArrayObject*refString,ArrayObject*refObject,bool writeOut)
	{
		AvmCore*core=this->m_pcToplevel->core();
		int index=0,iSet=0,iCount=0;
		Namespacep publicNS=core->getPublicNamespace(0);
		/*Traits* traits=pObj->vtable->traits;
		{
			TraitsBindingsp b=traits->getTraitsBindings();
			for(;;)
			{
				index=b->next(index);
				if(index<=0) break;
				Namespacep ns=b->nsAt(index);
				if(ns!=publicNS) continue;
				Stringp str=b->keyAt(index);
				
				if(str)
				{
					Binding bd=b->valueAt(index);
					if(bd==BIND_NONE) continue;
					Atom atom=0;
					switch (AvmCore::bindingKind(bd))
					{
						case BKIND_VAR:
							 if(writeOut)
							 {
							 	WriteString(str);
							 	atom=pObj->getSlotAtom(AvmCore::bindingToSlotId(bd),traits);
								WriteObject(thisAtom,atom,refString,refObject);							
							 }
							 iCount+=2;
							 break;
						case BKIND_CONST:
							 break;
						case BKIND_GET:
							 break;
						case BKIND_GETSET:
							 //if(t!=(AMF_OUTNAME|AMF_OUTVALUE))
							 {
								if(writeOut)
								{
									WriteString(str);
									int m = AvmCore::bindingToGetterId(bd);
									MethodEnv *f = pObj->vtable->methods[m];//vtable->method[m];
										//env->methods[m];
									Atom atomv=pObj->atom();
									atom = f->coerceEnter(atomv);
									WriteObject(thisAtom,atom,refString,refObject);
									//pArray->push(&atom,1);
								}
								iCount+=2;
							 }
							 break;							
					}
				}
			}
		}*/

		if(pObj->traits()->getHashtableOffset()!=0)
		{
			InlineHashtable*b=pObj->getTable();
			if(b)
			{
				int index=0;
				for(;;)
				{
					index=b->next(index);
					if(index<=0) break;				
					Atom str=b->keyAt(index);
					iCount+=2;
					//if(t&AMF_OUTNAME)
						
					if(writeOut)//t&AMF_OUTVALUE)
					{
						WriteObject(thisAtom,str,refString,refObject);
						Atom atom=b->valueAt(index);
						WriteObject(thisAtom,atom,refString,refObject);
						//pArray->push(&atom,1);
					}
				}
			}
		}

		return iCount;
	}

	int DataOutput::WriteMembers(Atom thisAtom,ScriptObject*pObj,int t,ArrayObject*refString,ArrayObject*refObject)
	{
		
		AvmCore*core=this->m_pcToplevel->core();
		int index=0,iSet=0,iCount=0;
		Namespacep publicNS=core->getPublicNamespace(0);
		Traits* traits=pObj->vtable->traits;
		while(traits)
		{
			TraitsBindingsp b=traits->getTraitsBindings();
			for(;;)
			{
				//if(iCount>6) break;
				//if(iCount==6)
				//	int v=0;
				index=b->next(index);
				if(index<=0) break;
				Namespacep ns=b->nsAt(index);
				if(ns!=publicNS) continue;
				Stringp str=b->keyAt(index);
				
				if(str)
				{
					Binding bd=b->valueAt(index);
					if(bd==BIND_NONE) continue;
					Atom atom=0;
					switch (AvmCore::bindingKind(bd))
					{
						case BKIND_VAR:
							 if(t&AMF_OUTNAME)
								WriteString(str);
							 if(t&AMF_OUTVALUE)
							 {
								atom=pObj->getSlotAtom(AvmCore::bindingToSlotId(bd),traits);
								WriteObject(thisAtom,atom,refString,refObject);
								//pArray->push(&atom,1);
							 }
							 iCount+=2;
							 break;
						case BKIND_CONST:
							 break;
						case BKIND_GET:
							 break;
						case BKIND_GETSET:
							 //if(t!=(AMF_OUTNAME|AMF_OUTVALUE))
							 {
								if(t&AMF_OUTNAME)
								{
									WriteString(str);
								}
								if(t&AMF_OUTVALUE)
								{
									int m = AvmCore::bindingToGetterId(bd);
									MethodEnv *f = pObj->vtable->methods[m];//vtable->method[m];
										//env->methods[m];
									Atom atomv=pObj->atom();
									atom = f->coerceEnter(atomv);
									WriteObject(thisAtom,atom,refString,refObject);
									//pArray->push(&atom,1);
								}
								iCount+=2;
							 }
							 break;
							
					}
				}
			}
			
			traits=traits->base;
		}

		if(pObj->traits()->getHashtableOffset()!=0)
		{
			InlineHashtable*b=pObj->getTable();
			if(b)
			{
				int index=0;
				iCount|=1;
				for(;;)
				{
					index=b->next(index);
					if(index<=0) break;				
					Stringp str=core->string(b->keyAt(index));
					if(!str) continue;
					
					//if(t&AMF_OUTNAME)
						
					if(t&AMF_OUTVALUE)
					{
						WriteString(str);
						Atom atom=b->valueAt(index);
						WriteObject(thisAtom,atom,refString,refObject);
						//pArray->push(&atom,1);
						//int v=0;
					}
				}
				if((t&AMF_OUTVALUE)&&(iCount&1))
					WriteByte(1);
			}
		}

		return iCount;
	}


	int DataOutput::WriteMembers0(ScriptObject*pObj,ArrayObject*refObjects,bool bNamed)
	{
		
		AvmCore*core=this->m_pcToplevel->core();
		int index=0,iSet=0,iCount=0;
		if(bNamed)
		{
			Namespacep publicNS=core->getPublicNamespace(0);
			Traits* traits=pObj->vtable->traits;
			while(traits)
			{
				TraitsBindingsp b=traits->getTraitsBindings();
				for(;;)
				{
					index=b->next(index);
					if(index<=0) break;
					Namespacep ns=b->nsAt(index);
					if(ns!=publicNS) continue;
					Stringp str=b->keyAt(index);				
					if(str)
					{
						Binding bd=b->valueAt(index);
						if(bd==BIND_NONE) continue;
						Atom atom=0;
						switch (AvmCore::bindingKind(bd))
						{
							case BKIND_VAR:
								 WriteUTF(str,false);							 
								 {
									atom=pObj->getSlotAtom(AvmCore::bindingToSlotId(bd),traits);
									WriteObject0(atom,refObjects);
								 }
								 iCount++;
								 break;
							case BKIND_CONST:
								 break;
							case BKIND_GET:
								 break;
							case BKIND_GETSET:
								 //if(t!=(AMF_OUTNAME|AMF_OUTVALUE))
								 {
									WriteUTF(str,false);								
									{
										int m = AvmCore::bindingToGetterId(bd);
										MethodEnv *f = pObj->vtable->methods[m];//vtable->method[m];
											//env->methods[m];
										Atom atomv=pObj->atom();
										atom = f->coerceEnter(atomv);
										WriteObject0(atom,refObjects);
									}
									iCount++;
								 }
								 break;							
						}
					}
				}
				
				traits=traits->base;
			}
		}
		if(pObj->traits()->getHashtableOffset()!=0)
		{
			InlineHashtable*b=pObj->getTable();
			if(b)
			{
				int index=0;
				for(;;)
				{
					index=b->next(index);
					if(index<=0) break;				
					Stringp str=core->string(b->keyAt(index));
					if(!str) continue;					
					WriteUTF(str);
					Atom atom=b->valueAt(index);
					WriteObject0(atom,refObjects);
					iCount++;
				}
			}
		}
		WriteShort(0);
		WriteByte(AMF0_OBJECTEND);
		return iCount;
	}

	void DataOutput::WriteObject0(Atom atom,ArrayObject*refObjects)
	{
		ShellCore*c=(ShellCore*)m_pcToplevel->core();
		
		if(c->isNull(atom))
		{
			WriteByte(AMF0_NULL);
			return;
		}
		switch(atomKind(atom))
		{		
		case kSpecialType:
		case kUnusedAtomTag:
			 WriteByte(AMF0_UNDEFINED);
			 break;
		case kObjectType:
			{
				int iRef=this->FindObject(refObjects,atom);
//#if (__CORE_VERSION__>=0x02071000)
				if(iRef>=0)
//#else
//				if(iRef)
//#endif
				{
					WriteByte(AMF0_REFERENCE);
					WriteShort(iRef);
					break;
				}
				refObjects->push(&atom,1);
				ScriptObject*obj=c->atomToScriptObject(atom);
				ShellToplevel*top=(ShellToplevel*)this->m_pcToplevel;
				if(c->IsTypeClass(obj,top->dateClass()))
				{//Date
					WriteByte(AMF0_DATE);
					WriteShort(1);
					DateObject*date=(DateObject*)obj;
					WriteDouble(date->date.getTime());
				}
				else if(c->IsTypeClass(obj,top->getXMLDocumentClass())||
						c->IsTypeClass(obj,top->xmlClass()))
				{
						//uint32 t=AMF_XMLDOC;
						WriteByte(AMF0_XMLDOC);
						Stringp s=c->string(atom);
						WriteUTF(s,true);
						//WriteString(s);
				}
				else if(c->IsTypeClass(obj,top->arrayClass))
				{
					ArrayObject*pArray=(ArrayObject*)obj;
					int i,iCount=pArray->getLength();
					bool bECMA=false;
					if(pArray->traits()->getHashtableOffset()!=0)
					{
						InlineHashtable*b=pArray->getTable();
						if(b)
						{
							int idd=b->next(0);
							if(idd)
								bECMA=true;
						}
					}
					if(bECMA)
					{  //ECMA Array
					   WriteByte(AMF0_ECMAARRAY);
					   WriteUnsignedInt((iCount<<1)|1);
					   for(i=0;i<iCount;i++)
					   {
						  Stringp s=c->intToString(i);
						  WriteUTF(s);
						  WriteObject0(pArray->getIntProperty(i),refObjects);
					   }
					   WriteMembers0(obj,refObjects,false);
					}
					else
					{						
						WriteByte(AMF0_STRICTARRAY);
						this->WriteUnsignedInt(iCount);
						for(i=0;i<iCount;i++)
							WriteObject0(pArray->getIntProperty(i),refObjects);
					}
				}
				
				else
				{//Normal Object
					Stringp alias=c->getClassAlias(obj);
					if(alias)//register class
					{
						WriteByte(AMF0_TYPEDOBJECT);
						WriteUTF(alias,false);
						WriteMembers0(obj,refObjects);
					}
					else
					{//anonymous object
						WriteByte(AMF0_OBJECT);
						WriteMembers0(obj,refObjects);
						//WriteByte(AMF0_OBJECTEND);
					}
				}
			}
			 break;
		case kStringType:
			{
				Stringp str=c->atomToString(atom);
				if(str->get_length()<=0xffff)
				{
					WriteByte(AMF0_STRING);
					this->WriteUTF(str);
				}
				else
				{
					WriteByte(AMF0_LONGSTRING);
					WriteUTF(str,true);
				}
			}break;
		case kNamespaceType:break;		
		case kBooleanType:
			{
				int v=c->boolean(atom);
				WriteByte(AMF0_BOOLEAN);
				this->WriteBoolean(v);
			}
			 break;
		case kIntptrType:
		case kDoubleType:
			{
				WriteByte(AMF0_NUMBER);
				double v=c->number(atom);
				this->WriteDouble(v);
			}break;
		}
	}
	

	void DataOutput::WriteObject(Atom thisAtom,Atom atom,ArrayObject*refString,ArrayObject*refObject)
	{
		AvmCore*c=m_pcToplevel->core();
		if(c->isNull(atom))
		{
			WriteByte(AMF_NULL);
			return;
		}
		switch(atomKind(atom))
		{
		case kSpecialType:
		case kUnusedAtomTag:
			{
				uint32_t type=AMF_UNDEFINE;
				WriteByte(type);
			}break;
		case kNamespaceType:
			 break;
		case  kObjectType:
			  {
			    int iRef=FindObject(refObject,atom);
				ScriptObject*pObj=c->atomToScriptObject(atom);
				ShellToplevel*sh=(ShellToplevel*)m_pcToplevel;
				Stringp alias=((ShellCore*)c)->getClassAlias(pObj);
				//if(ShellCore::IsProtoTypeOf(sh->dateClass(false),pObj))
				if(ShellCore::IsTypeClass(pObj,sh->dateClass(false)))
				{
					WriteByte(AMF_DATE);
					WriteByte(1);
					DateObject*date=(DateObject*)pObj;
					WriteDouble(date->date.getTime());
				}
				//else if(ShellCore::IsProtoTypeOf(sh->arrayClass,pObj))
				else if(ShellCore::IsTypeClass(pObj,sh->arrayClass))
				{//Array
//#if (__CORE_VERSION__>=0x02071000)
					if(iRef>=0)
//#else
//					if(iRef)
//#endif
					//if(iRef)
						WriteRef(AMF_ARRAY,iRef);
					else
					{
						refObject->push(&atom,1);
						WriteArray(thisAtom,(ArrayObject*)pObj,refString,refObject);
					}
				}
				else if(ShellCore::IsTypeClass(pObj,sh->getDictionaryClass()))
				{
//#if (__CORE_VERSION__>=0x02071000)
					if(iRef>=0)
/*#else
					if(iRef)
#endif*/					
						WriteRef(AMF_DICTIONARY,iRef);
					else
					{
						refObject->push(&atom,1);
						WriteDictionary(thisAtom,(DictionaryObject*)pObj,refString,refObject);
					}
				}
				//else if(ShellCore::IsProtoTypeOf(sh->xmlClass(false),pObj))
				else if(ShellCore::IsTypeClass(pObj,sh->xmlClass(false)))
				{//XML
					//WriteXML((XMLObject*)pObj);
//#if (__CORE_VERSION__>=0x02071000)
					if(iRef>=0)
/*#else
					if(iRef)
#endif	*/				
						WriteRef(AMF_XML,iRef);
					else
					{
						refObject->push(&atom,1);
						uint32 t=AMF_XMLDOC;
						WriteByte(t);
						Stringp s=c->string(atom);
						WriteString(s);
					}
				}
				//else if(ShellCore::IsProtoTypeOf(sh->getByteArrayClass(false),pObj))
				else if(ShellCore::IsTypeClass(pObj,sh->getByteArrayClass(false)))
				{	//ByteArray
					//WriteByteArray((ArrayObject*)pObj);
//#if (__CORE_VERSION__>=0x02071000)
					if(iRef>=0)
//#else
//					if(iRef)
//#endif
						WriteRef(AMF_BYTEARRAY,iRef);
					else
					{
						refObject->push(&atom,1);
						uint32 t=AMF_BYTEARRAY;
						WriteByte(t);
						DataFile&pFile=((ByteArrayObject*)pObj)->GetByteArray();
						
						int l=(pFile.GetLength());//.BytesAvailable());
						//DataMemory buffer;
						//buffer.SetLength(l);
						//pFile.ReadBytes(buffer,0,l);
						t=(l<<1)|1;
						WriteU29(t);
						//WriteByte(t);
						WriteBytes(pFile,0,l);//buffer,0,l);
					}
				}
				//else if(ShellCore::IsProtoTypeOf(sh->getXMLDocumentClass(false),pObj))
				else if(ShellCore::IsTypeClass(pObj,sh->getXMLDocumentClass(false)))
				{//XML Document
					//WriteXMLDocument((XMLDocumentObject*)pObj);
//#if (__CORE_VERSION__>=0x02071000)
					if(iRef>=0)
/*#else
					if(iRef)
#endif*/					
						WriteRef(AMF_XMLDOC,iRef);
					else
					{
						refObject->push(&atom,1);
						uint32 t=AMF_XMLDOC;
						WriteByte(t);
						Stringp s=c->string(atom);
						WriteString(s);
					}
				}
				else if( ShellCore::IsTypeClass(pObj, m_pcToplevel->intVectorClass ) )
				{
					 if(iRef>=0)
						 WriteRef(AMF_VECTORINT,iRef);
					 else
					 {
						 refObject->push(&atom,1);
						 uint32 t=AMF_VECTORINT;
						 WriteByte(t);
						 IntVectorObject*pIntObj=(IntVectorObject*)pObj;
						 t=(pIntObj->get_length()<<1)|1;
						 WriteU29(t);
						 t=pIntObj->get_fixed();
						 WriteByte(t);
						 int i,iCount=pIntObj->get_length();
						 for(i=0;i<iCount;i++)
							 WriteInt(pIntObj->m_array[i]);
						 //Write(pIntObj->m_array,pIntObj->get_length()*sizeof(int));
					 }
				}
				else if( ShellCore::IsTypeClass(pObj, m_pcToplevel->uintVectorClass ) )
				{
					if(iRef>=0)
						 WriteRef(AMF_VECTORUINT,iRef);
					 else
					 {
						 refObject->push(&atom,1);
						 uint32 t=AMF_VECTORUINT;
						 WriteByte(t);
						 UIntVectorObject*pIntObj=(UIntVectorObject*)pObj;
						 t=(pIntObj->get_length()<<1)|1;
						 WriteU29(t);
						 t=pIntObj->get_fixed();
						 WriteByte(t);
						 int i,iCount=pIntObj->get_length();
						 for(i=0;i<iCount;i++)
							 WriteUnsignedInt(pIntObj->m_array[i]);
						 //Write(pIntObj->m_array,pIntObj->get_length()*sizeof(int));
					 }
				}
				else if( ShellCore::IsTypeClass(pObj, m_pcToplevel->doubleVectorClass ) )
				{
					if(iRef>=0)
						 WriteRef(AMF_VECTORDOUBLE,iRef);
					 else
					 {
						 refObject->push(&atom,1);
						 uint32 t=AMF_VECTORDOUBLE;
						 WriteByte(t);
						 DoubleVectorObject*pIntObj=(DoubleVectorObject*)pObj;
						 t=(pIntObj->get_length()<<1)|1;
						 WriteU29(t);
						 t=pIntObj->get_fixed();
						 WriteByte(t);
						 int i,iCount=pIntObj->get_length();
						 for(i=0;i<iCount;i++)
							 WriteDouble(pIntObj->m_array[i]);
						 //Write(pIntObj->m_array,pIntObj->get_length()*sizeof(int));
					 }
				}
				else if( ShellCore::IsTypeClass(pObj, m_pcToplevel->objectVectorClass ) )
				{
					 if(iRef>=0)
						 WriteRef(AMF_VECTOROBJECT,iRef);
					 else
					 {
						 refObject->push(&atom,1);
						 uint32 t=AMF_VECTOROBJECT;
						 WriteByte(t);
						 ObjectVectorObject*pIntObj=(ObjectVectorObject*)pObj;
						 t=(pIntObj->get_length()<<1)|1;
						 WriteU29(t);
						 t=pIntObj->get_fixed();
						 WriteByte(t);
						 ClassClosure*cs=(ClassClosure*)c->atomToScriptObject(pIntObj->get_type());
						 if(cs==NULL)
							 WriteString(c->kAsterisk);
						 else
						 {
							 Stringp aliasData=((ShellCore*)c)->getClassAliasClass(cs);
							 WriteString(aliasData);
						 }
						 int i,iCount=pIntObj->get_length();
						 if(iCount)
						 {
							//ScriptObject*pDataObj=c->atomToScriptObject(pIntObj->m_array[0]);
							//Stringp aliasData=((ShellCore*)c)->getClassAlias(pDataObj);
							//WriteString(aliasData);
							for(i=0;i<iCount;i++)
								 WriteObject(thisAtom,pIntObj->m_array[i],refString,refObject);
	
						 }
						 //else
						 //	 WriteString(NULL);
						 //Write(pIntObj->m_array,pIntObj->get_length()*sizeof(int));
					 }
				}
				else if(alias)
				{
//#if (__CORE_VERSION__>=0x02071000)
					if(iRef>=0)
/*#else
					if(iRef)
#endif*/					
						WriteRef(AMF_OBJECT,iRef);
					else
					{
						refObject->push(&atom,1);
						WriteScriptObjectEx(thisAtom,pObj,alias,refString,refObject);
					}
				}				
				else
				{//Normal Object
//#if (__CORE_VERSION__>=0x02071000)
					if(iRef>=0)
/*#else
					if(iRef)
#endif*/	
						WriteRef(AMF_OBJECT,iRef);
					else 
					{
						refObject->push(&atom,1);
						WriteScriptObject(thisAtom,pObj,refString,refObject);
					}
				}
			}
			 break;
		case kStringType:
			{
				int iRef=0;//FindObject(refObject,atom);
				if(iRef)
					WriteRef(AMF_STRING,iRef);
				else
				{
					uint32_t t=AMF_STRING;
					WriteByte(t);
					Stringp s=c->atomToString(atom);
					WriteString(s);
				}
				//WriteUTFBytes(str.c_str(),str.length());
			}break;
		
		case kBooleanType:
			{
				uint32_t type=c->boolean(atom)?AMF_TRUE:AMF_FALSE;
				WriteByte(type);
			}
			 break;
		case kIntptrType:
			  {
				  int v=c->integer_i(atom);
				  uint32_t type=AMF_INT;
				  WriteByte(type);
				  WriteU29(v);
			  }break;
		case kDoubleType:
			{
				uint32 t=AMF_DOUBLE;
				WriteByte(t);
				double v=c->atomToDouble(atom);
				WriteDouble(v);
			}
			  break;
		}
	}

	void DataOutput::WriteU29(int u)
	{
		/*if(big)
		{
			for(int i=0;i<4;i++)
			{
				uint32_t v=u&0x7f;
				WriteByte(v);
				u=u>>7;
				if(!u) break;
			}
		}
		else*/
		{
			if(u>>21)
			{
				this->WriteUnsignedByte((u>>22)|0x80);
				WriteUnsignedByte((u>>15)|0x80);
				WriteUnsignedByte((u>>8)|0x80);
				WriteUnsignedByte((u&0xff));
			}
			else if(u>>14)
			{
				WriteUnsignedByte((u>>14)|0x80);
				WriteUnsignedByte((u>>7)|0x80);
				WriteUnsignedByte(u&0x7f);				
			}
			else if(u>>7)
			{
				WriteUnsignedByte((u>>7)|0x80);
				WriteUnsignedByte(u&0x7f);
			}
			else
				WriteUnsignedByte(u);
			/*XU8 flag=0;
			uint32_t v=u>>21;
			if(v)
			{			
				v|=0x80;
				WriteByte(v);
			}
			v=(u>>14)&0x7f;
			if(v||flag)
			{
				v|=0x80;
				WriteByte(v);
			}
			v=(u>>7)&0x7f;
			if(v||flag)
			{
				v|=0x80;
				WriteByte(v);
			}
			//v=(u&0x7f)|flag;
			v=u&0x7f;
			WriteByte(v);*/
		}
		/*uint32_t v=u&0x7f;
		u=u>>7;
		if(u)
		{ v|=0x80;
		  WriteByte(v);
		}
		else {WriteByte(v);return;}

		v=u&0x7f;
		u=u>>7;
		if(u)
		{ v|=0x80;
		  WriteByte(v);
		}
		else {WriteByte(v);return;}

		v=u&0x7f;
		u=u>>7;
		if(u)
		{ v|=0x80;
		  WriteByte(v);
		}
		else {WriteByte(v);return;}

		v=u&0xff;
		//u=u>>7;
		//if(!u)
		//{ v|=0x80;
		//  WriteByte(v);
	///	}
		//else 
		{WriteByte(v);return;}*/

		//m_pcToplevel->throwRangeError(kNumberOutOfRangeError);
	}

	void DataOutput::WriteMultiByte(Stringp data, Stringp charSet)
	{
		ByteArrayClass *pClass = ((ShellToplevel *)m_pcToplevel)->getByteArrayClass();
		char_set cSet = (char_set)pClass->GetCharSetFormMap(charSet);
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
		}
	}

	void DataOutput::WriteUTF8(Stringp data)
	{
		StUTF8String utf8Data(data);

		Write(utf8Data.c_str(), strlen(utf8Data.c_str()));
	}

	void DataOutput::WriteUTF16(Stringp data)
	{
		StUTF16String utf16Data(data);

		wchar *pUtf16Data = (wchar *)utf16Data.c_str();

		for (int i = 1; i <= utf16Data.length(); i++)
		{
			EndianAdjust16(*pUtf16Data);
			pUtf16Data++;
		}

		Write(utf16Data.c_str(), utf16Data.length() * sizeof(wchar));
	}

	void DataOutput::WriteMultiByte(Stringp data, char_set charSet)
	{
		StUTF8String utf8Data(data);
		XString8 str8(utf8Data.c_str());
		str8.ConvertToGB();
		Write(str8.GetData(), str8.GetLength());
	}

}