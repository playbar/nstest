#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ShaderData.h"
namespace avmshell{
ShaderDataClass::ShaderDataClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ShaderDataClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShaderDataObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ShaderDataObject::ShaderDataObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void ShaderDataObject::AS3_setByteCode(ByteArrayObject *pByteCode)
{
	//Add your act code here...
	/*if(pByteCode)
	{
		XFile file;
		if(file.Open("D:\\shader.pbj",XFile::XCREATE|XFile::XBINARY|XFile::XWRITE))
		{
			file.Write(pByteCode->GetByteArray().GetBuffer(),pByteCode->GetLength());
			file.Close();
		}
	}*/
	if(pByteCode)
	{
		int l=pByteCode->GetLength();
		XU8*pBuf=pByteCode->GetByteArray().GetBuffer();
		ShellCore*c=(ShellCore*)core();
		while(l>0)
		{
			int iCode=*pBuf;
			pBuf++;
			l--;
			switch(iCode)
			{
			case 0xA0:
				{
					int iType=*pBuf;
					pBuf++;
					l--;
					Stringp name=c->internStringUTF8((char*)pBuf);
					int len=name->get_length()+1;
					l-=len;
					pBuf+=len;
					Atom cnt=0;
					switch(iType)
					{
					case 0xC:
						 {
							 Stringp var=c->newStringUTF8((char*)pBuf);
							 len=var->get_length()+1;
							 l-=len;
							 pBuf+=len;
							 cnt=var->atom();
						 }
						 break;
					case 0x8:
						{
							int var=pBuf[0]|(pBuf[1]<<8);
							l-=2;
							pBuf+=2;
							cnt=c->intToAtom(var);
						}break;
					default:
						l=0;
						break;
					}
					if(cnt)
						this->setStringProperty(name,cnt);
				}break;
			case 0xA4:
				{
					Stringp name=c->internConstantStringLatin1("name");
					XU16   len=pBuf[0]|(pBuf[1]<<8);
					Stringp cnt=c->newStringUTF8((char*)pBuf+2,len);
					l-=len+2;
					pBuf+=len+2;
					this->setStringProperty(name,cnt->atom());
				}break;
			case 0xA5:
				 pBuf+=4;
				 l-=4;
				 break;
			default:
				 l=0;
				 break;
			}
		}
	}
}

}