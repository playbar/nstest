#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ByteArray.h"
#include "AS3Endian.h"
#include "HashAlgorithms.h"


#include "gzipapis.h"
#include "genericzlib.h"

namespace avmshell{


//SETARRAY(AR,IDX,V) AR->setUIntProperty

HashAlgorithmsClass::HashAlgorithmsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* HashAlgorithmsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) HashAlgorithmsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}


int HashAlgorithmsClass::AS3_fnvHash(ByteArrayObject*pObj)
{
	if(pObj==NULL)
		return 0;
	int _loc_2 = 16777619;
    int _loc_3 = 2166136261;
     
	pObj->Seek(0);
	//pObj->AS3_readByte(
	//param1.position = 0;
    int _loc_4 = 0;
	int l=pObj->GetLength();
	signed char* data=(signed char*)pObj->GetByteArray().GetBuffer();
    while (_loc_4 < l)
    {              
         _loc_3 = (_loc_3 ^ data[_loc_4]) * _loc_2;
          _loc_4++;
    }
    _loc_3 = _loc_3 + (_loc_3 << 13);
    _loc_3 = _loc_3 ^ _loc_3 >> 7;
    _loc_3 = _loc_3 + (_loc_3 << 3);
    _loc_3 = _loc_3 ^ _loc_3 >> 17;
    _loc_3 = _loc_3 + (_loc_3 << 5);

     return _loc_3;
}

HashAlgorithmsObject::HashAlgorithmsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SwfEnDeUtilClass::SwfEnDeUtilClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SwfEnDeUtilClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SwfEnDeUtilObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

Atom SwfEnDeUtilClass::AS3_unCompress(ByteArrayObject *dat, Stringp key)
{
	ShellToplevel*top = (ShellToplevel*)toplevel();
	ByteArrayClass*pClass=top->getByteArrayClass();
	int dataLen = dat->GetLength();
	ByteArrayObject*pNew=pClass->CreateArray(dat->GetLength());

	memcpy(pNew->GetByteArray().GetBuffer(),dat->GetByteArray().GetBuffer(),dataLen);
	unCompress(pNew,key);

	ScriptObject*pObj = top->objectClass->newInstance();
	pObj->setStringProperty(core()->internStringLatin1("bytes"),pNew->atom());
	return pObj->atom();
}
void SwfEnDeUtilClass::unCompress(ByteArrayObject *dat, Stringp key)
{
	int kLen = key->get_length();
	int _loc_3 = dat->GetLength() - kLen -1;
	int _loc_9 = 0;
	int _loc_11 = 0;
	uint8*pDat=dat->GetByteArray().GetBuffer();
	//Pointers ptrs(key);
	//return (getWidth() == k8) ? ptrs.p8[index] : ptrs.p16[index];
	if(key->getWidth()==String::k8)
	{
		uint8_t*pKey=key->GetBuffer()->p8;
		while(_loc_11 <= _loc_3-2)
		{
			*pDat = *pDat-pKey[_loc_9];
			_loc_11++;
			_loc_9++;
			pDat++;
			if(_loc_9>=kLen)
				_loc_9 = 0;
		}
	}
	else
	{
		wchar*pKey=key->GetBuffer()->p16;
		while(_loc_11 <= _loc_3-2)
		{
			*pDat = *pDat-pKey[_loc_9];
			_loc_11++;
			_loc_9++;
			pDat++;
			if(_loc_9>=kLen)
				_loc_9 = 0;
		}
	}
	
    //new Object().bytes = _loc_10;
	/*double _loc_3;
    var _loc_4:* = new ByteArray();
    var _loc_5:String = "";
    var _loc_6:String = "";
    param1.position = param1.length - 1;
    //var _loc_7:String = "";
	Stringp _loc_7 = key;
    
    _loc_3 = param1.length - param2.length - 1;
    var _loc_9:Number = 0;
    var _loc_10:* = new ByteArray();
    var _loc_11:int = 0;
    while (_loc_11 <= _loc_3 - 2)
    {
        
        if (_loc_9 >= _loc_7.length)
        {
            _loc_9 = 0;
        }
        _loc_10.writeByte(param1[_loc_11] - _loc_7.charCodeAt(_loc_9));
        _loc_11++;
        _loc_9 = _loc_9 + 1;
    }
    var _loc_12:* = new Object();
    new Object().bytes = _loc_10;
    return _loc_12;*/
}

Atom SwfEnDeUtilClass::AS3_getDecryptDataByByteArray(ByteArrayObject *dat, Stringp key, avmplus::Stringp info)
{
	//return nullObjectAtom;
	ScriptObject* dataObj=toplevel()->objectClass->newInstance();
    //var byteObj:Object;
    //var content:ByteArray;
    //var bytes:* = param1;
    //var veriKey:* = param2;
    //var logInfo:* = param3;
    if (dat)
    {		
		TRY(core(), kCatchAction_ReportAsError)		
        {
            //bytes.uncompress();
			dat->AS3_uncompress();
			unCompress(dat,key);
            //byteObj = unCompress(bytes, veriKey);
            //content = byteObj.bytes;
            //content.position = 0;
            //content.uncompress();
			dat->AS3_position_set(0);
			dat->AS3_uncompress();
            //LogTraceManager.debug(null, "com.my9yu.framework.utils.SwfEndeUtil: ��ݽ�ѹ���ɹ�: " + logInfo);
			return dat->AS3_readObject();
				//content.readObject();
        }
        CATCH(Exception *exception)
		{        
            //LogTraceManager.debug(null, "com.my9yu.framework.utils.SwfEndeUtil: ��ݽ�ѹ��ʧ��: " + logInfo);
        }
		END_CATCH
		END_TRY
    }
    return dataObj->atom();
}


SwfEnDeUtilObject::SwfEnDeUtilObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassUtilClass::ClassUtilClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ClassUtilClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ClassUtilObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ClassUtilObject::ClassUtilObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}


Atom ClassUtilClass::AS3_clone(Atom obj)
{
	if(atomKind(obj)!=kObjectType)
	{
		return obj;
	}
	ScriptObject*pObj=core()->atomToScriptObject(obj);
	return pObj->atom();
	
}
Atom ClassUtilClass::AS3_createNewObject(Atom obj)
{
	if(atomKind(obj)!=kObjectType)
	{
		return obj;
	}
	ScriptObject*newObj = toplevel()->objectClass->newInstance();

	return newObj->atom();
	//return nullObjectAtom;
}

void ClassUtilClass::CopyObject(ScriptObject*pObj1,ScriptObject*pObj2,bool isObject)
{
	AvmCore*c=core();	
	
	//bool isObject = pObj1->traits()==objClass->traits()->itraits;
	TraitsBindingsp t=pObj2->traits()->getTraitsBindings();
		
	Namespacep publicNS=c->getPublicNamespace(0);
	if(!isObject)
	{
		//TraitsBindingsp t=pObj2->traits()->getTraitsBindings();
		//int index = 0;	
		//Namespacep publicNS=c->getPublicNamespace(0);
		Traits* traits=pObj1->vtable->traits;
		while(traits)
		{
			TraitsBindingsp b=traits->getTraitsBindings();
			int index = 0;
			for(;;)
			{
				//if(iCount>6) break;
				//if(iCount==6)
				//	int v=0;
				index=b->next(index);
				if(index<=0) break;
				Stringp name=b->keyAt(index);
				Namespacep ns=b->nsAt(index);
				if(ns!=publicNS) continue;
								
				if(name)
				{
					Binding bd=b->valueAt(index);
					if(bd==BIND_NONE) continue;
					Atom atom=0;
					switch (AvmCore::bindingKind(bd))
					{
						case BKIND_VAR:
							 //if(isObject)
							 {
								atom=pObj1->getSlotAtom(AvmCore::bindingToSlotId(bd),traits);
								//pObj2->setStringProperty(str,atom);
								ShellCore::_setProperty(pObj2,name->atom(),atom,publicNS,t);
							 }
							 break;
						case BKIND_CONST:
							 break;
						case BKIND_GET:
						case BKIND_GETSET:
							 //if(t!=(AMF_OUTNAME|AMF_OUTVALUE))
							 {
								int m = AvmCore::bindingToGetterId(bd);
								MethodEnv *f = pObj1->vtable->methods[m];//vtable->method[m];
										//env->methods[m];
								Atom atomv=pObj1->atom();
								atom = f->coerceEnter(atomv);
								
								ShellCore::_setProperty(pObj2,name->atom(),atom,publicNS,t);
								//WriteObject(thisAtom,atom,refString,refObject);
									//pArray->push(&atom,1);
								//}
								//iCount+=2;
							 }
							 break;
							
					}
				}
			}
			
			traits=traits->base;
		}
	}
	else
	{
		/*if(pObj1->traits()->getHashtableOffset()!=0)
		{
				InlineHashtable*b=pObj1->getTable();
				if(b)
				{
					int index=0;
					for(;;)
					{
						index=b->next(index);
						if(index<=0) break;	
						Atom key = b->keyAt(index);
						if(!key) continue;
						//if(atomKind(atom)!=kStringType) continue;
						//Stringp name=c->atomToString(atom);
						//if(!name) continue;
						
						Atom atom=b->valueAt(index);
						ShellCore::_setProperty(pObj2,key,atom,publicNS,t);
						//pObj2->setAtomProperty(key,atom);
					}				
				}
		}*/
		int index = 0;
		for(;;)
		{
			index = pObj1->nextNameIndex(index);
			if(index<=0) break;
			Atom key = pObj1->nextName(index);
			if(!key) continue;						
			Atom atom=pObj1->nextValue(index);
			ShellCore::_setProperty(pObj2,key,atom,publicNS,t);
						//pObj2->setAtomProperty(key,atom);
		}
	}
		//return obj2;	
}

Atom ClassUtilClass::AS3_samePropertiesCopyValue(Atom obj1, Atom obj2)
{
	if(AvmCore::isNullOrUndefined(obj1)||
		AvmCore::isNullOrUndefined(obj2)) return nullObjectAtom;
	if(atomKind(obj1)!=kObjectType||
	   atomKind(obj2)!=kObjectType) return obj2;
	ClassClosure*objClass=toplevel()->objectClass;
	AvmCore*c=core();
	ScriptObject*pObj1=c->atomToScriptObject(obj1);
	ScriptObject*pObj2=c->atomToScriptObject(obj2);
	
	bool isObject = pObj1->traits()==objClass->traits()->itraits;
	
	CopyObject(pObj1,pObj2,isObject);

	return obj2;
}
void ClassUtilClass::AS3_objectSamePropertyCopyValueByXML(Atom obj1,Atom obj2)
{
	if(AvmCore::isNullOrUndefined(obj1)||
		AvmCore::isNullOrUndefined(obj2)) return;
	if(atomKind(obj1)!=kObjectType||
	   atomKind(obj2)!=kObjectType) return;
	AvmCore*c=core();
	Toplevel*top=toplevel();
	ScriptObject*pObj1=c->atomToScriptObject(obj1);
	ScriptObject*pObj2=c->atomToScriptObject(obj2);

	TraitsBindingsp t=pObj2->traits()->getTraitsBindings();
	
	
	Namespacep publicNS=c->getPublicNamespace(0);
	Traits* traits=pObj2->vtable->traits;
	while(traits)
	{
		int index = 0;
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
			Stringp name=b->keyAt(index);				
			if(name)
			{
				Binding bd=b->valueAt(index);
				if(bd==BIND_NONE) continue;
				Atom atom=0;
				Atom setAtom=0;
				Traitsp type=NULL;
				switch (AvmCore::bindingKind(bd))
				{
					case BKIND_VAR:
						 if(!pObj1->hasStringProperty(name)) break;
						 {
							atom = pObj1->getStringProperty(name);
							const uint32_t slotID = AvmCore::bindingToSlotId(bd);
							type = b->getSlotTraits(slotID);
						 }
						 //setAtom=pObj2->getSlotAtom(AvmCore::bindingToSlotId(bd),traits);												
						 break;
					case BKIND_CONST:
						 break;
					case BKIND_GET:
						 break;
					case BKIND_SET:
					case BKIND_GETSET:
						 {
							if(!pObj1->hasStringProperty(name)) break;
							atom = pObj1->getStringProperty(name);
							const uint32_t methodID = AvmCore::hasGetterBinding(bd) ?
														AvmCore::bindingToGetterId(bd) :
														AvmCore::bindingToSetterId(bd);

							MethodInfo* mi = b->getMethod(methodID);
							mi->resolveSignature(top);
							MethodSignaturep ms = mi->getMethodSignature();

							Traitsp declaringTraits = mi->declaringTraits();

							type = AvmCore::hasGetterBinding(bd) ?
														ms->returnTraits() :
														ms->paramTraits(1);
						 }
						 break;
						 //if(t!=(AMF_OUTNAME|AMF_OUTVALUE))
						 /*{
							int m = AvmCore::bindingToGetterId(bd);
							MethodEnv *f = pObj1->vtable->methods[m];//vtable->method[m];
									//env->methods[m];
							Atom atomv=pObj1->atom();
							atom = f->coerceEnter(atomv);
							
							ShellCore::_setProperty(pObj2,name->atom(),atom,publicNS,t);
							//WriteObject(thisAtom,atom,refString,refObject);
								//pArray->push(&atom,1);
							//}
							//iCount+=2;
						 }*/
						 break;
						
				}
				if(atom)
				{
					//ShellCore::_setProperty(pObj2,name->atom(),atom,publicNS,t);
					switch(type->builtinType)
					{
					case BUILTIN_number:
						 atom = c->doubleToAtom(c->number(atom));
						 break;
					case BUILTIN_int:
						 atom = c->intToAtom(c->toInteger(atom));
						 break;
					case BUILTIN_uint:
						 atom = c->uintToAtom(c->toUInt32(atom));
						 break;
					case BUILTIN_boolean:
						 atom = BooleanAtom(atom);
						 break;
					case BUILTIN_string:
						 atom = c->string(atom)->atom();
						 break;
					}
					ShellCore::_setProperty(pObj2,name->atom(),atom,publicNS,t);
				}
			}
		}
		
		traits=traits->base;
		}
	}

	inline Atom ClassUtilClass::BooleanAtom(Atom atom)
	{
		if(atomKind(atom)==kBooleanType) return atom;
		Stringp s=core()->string(atom);
		if(s->equalsLatin1("0"))
			return falseAtom;
		else if(s->equalsLatin1("1"))
			return trueAtom;
		else if(s->equalsLatin1("true"))
			return trueAtom;
		else if(s->equalsLatin1("false"))
			return falseAtom;
		return core()->boolean(atom);
	}
	//return obj2;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	MD5Class::MD5Class(VTable* cvtable):ClassClosure(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* MD5Class::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) MD5Object(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}

	MD5Object::MD5Object(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}

	Stringp MD5Class::AS3_hashBinary(ByteArrayObject*pData)
	{
		MD5 md5;
		XU8 passwd[16];
		DataFile&file=pData->GetByteArray();
		md5.MakeMD5((char*)file.GetBuffer(),file.GetLength(),(char*)passwd);
		XString8 strOut;
		int l;
		for(l=0;l<16;l++)
		{
			XU8 c=passwd[l]>>4;
			if(c<10)
				strOut+=(char)('0'+c);
			else
				strOut+=(char)('a'+(c-10));
			c=passwd[l]&0xf;
			if(c<10)
				strOut+=(char)('0'+c);
			else
				strOut+=(char)('a'+(c-10));
		}
		return core()->newStringLatin1(strOut);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//

	SamePropertyObjectUtilClass::SamePropertyObjectUtilClass(VTable* cvtable):ClassClosure(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* SamePropertyObjectUtilClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) SamePropertyObjectUtilObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}

	SamePropertyObjectUtilObject::SamePropertyObjectUtilObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}

	void SamePropertyObjectUtilClass::AS3_copySamePropertyValue(Atom atom1,Atom atom2)
	{
		if(AvmCore::isNullOrUndefined(atom1)) return;
		if(AvmCore::isNullOrUndefined(atom2)) return;
		if(atomKind(atom1)!=kObjectType) return;
		if(atomKind(atom2)!=kObjectType) return;
		ScriptObject*obj1 = core()->atomToScriptObject(atom1);
		ScriptObject*obj2 = core()->atomToScriptObject(atom2);
		CopySamePropertyValue(obj1,obj2);
	}

	/*Atom SamePropertyObjectUtilClass::GetPropety(ScriptObject*obj,Atom name)
	{
		Toplevel top;
		top->getproperty
	}*/

	void SamePropertyObjectUtilClass::CopySamePropertyValue(ScriptObject*src,ScriptObject*target)
	{
		AvmCore*c=core();			
		Namespacep publicNS=c->getPublicNamespace(0);
		TraitsBindingsp t=target->traits()->getTraitsBindings();
		Toplevel*top=toplevel();
		if(IsList(src)&&IsList(target))
		{
			CopyDirectList(src,target);
		}
		else
		{
			bool isSrcObj = IsObject(src);
			if(IsObject(target))
			{
				int index = 0;
				for(;;)
				{
					index = target->nextNameIndex(index);
					if(index<=0) break;
					Atom name=target->nextName(index);
					Atom tarValue=target->nextValue(index);
					//Atom srcValue=src->getAtomProperty(name);
					Multiname nm(publicNS,c->string(name));
					Atom srcValue = top->getproperty(src->atom(),&nm,src->vtable);
					//Atom srcValue = GetProperty(src,name);

					if(srcValue == NULL) continue;
					tarValue=CopyValue(srcValue,tarValue,isSrcObj,true);
					if(tarValue)
						ShellCore::_setProperty(target,name,tarValue,publicNS,t);
						//target->setAtomProperty(name,tarValue);
				}
			}
			else
			{
				
				Traits* traits=target->vtable->traits;
				while(traits)
				{
					int index = 0;
					TraitsBindingsp b=traits->getTraitsBindings();
					for(;;)
					{
						//if(iCount>6) break;
						//if(iCount==6)
						//	int v=0;
						index=b->next(index);
						if(index<=0) break;
						Stringp name=b->keyAt(index);
						Namespacep ns=b->nsAt(index);
						if(ns!=publicNS) continue;
						
						if(name)
						{
							Multiname nm(publicNS,name);
							Atom srcValue = top->getproperty(src->atom(),&nm,src->vtable);//GetPeoperty(src,name->atom());
								//src->getStringProperty(name);
							if(!c->isNullOrUndefined(srcValue))
							{
								Binding bd=b->valueAt(index);
								if(bd==BIND_NONE) continue;
								Atom atom=0;
								switch (AvmCore::bindingKind(bd))
								{
									case BKIND_VAR:
										 //if(isObject)
										 {
											atom=target->getSlotAtom(AvmCore::bindingToSlotId(bd),traits);
											//pObj2->setStringProperty(str,atom);
											//ShellCore::_setProperty(pObj2,name->atom(),atom,publicNS,t);
										 }
										 break;
									case BKIND_CONST:
										 break;
									case BKIND_SET:
									case BKIND_GETSET:
										 //if(t!=(AMF_OUTNAME|AMF_OUTVALUE))
										 {
											int m = AvmCore::bindingToGetterId(bd);
											MethodEnv *f = target->vtable->methods[m];//vtable->method[m];
													//env->methods[m];
											Atom atomv=target->atom();
											atom = f->coerceEnter(atomv);
											
											//ShellCore::_setProperty(pObj2,name->atom(),atom,publicNS,t);
											//WriteObject(thisAtom,atom,refString,refObject);
												//pArray->push(&atom,1);
											//}
											//iCount+=2;
										 }
										 break;
										
								}
								if(atom)
								{
									atom = CopyValue(srcValue,atom,isSrcObj,false);
									if(atom)
										ShellCore::_setProperty(target,name->atom(),atom,publicNS,t);
										//target->setStringProperty(name,atom);
								}
							}

						}
						//Atom srcValue = src->getAtomProperty(
					}
					
					traits=traits->base;
				}
			}
		}
	}

	Atom SamePropertyObjectUtilClass::CopyValue(Atom src,Atom target,bool isSrcObj,bool isTarObj)
	{
		AvmCore*c = core();
		
		if(isSrcObj&&isTarObj) return src;
		if(isSrcObj)
		{
			if(!IsComplex(target)) return src;
			ScriptObject*obj = c->atomToScriptObject(target);		
			ClassClosure* pClass=obj->vtable->_ctor;
			Atom args[]={NULL};
			Atom atom=pClass->construct(0,args);
			this->AS3_copySamePropertyValue(src,atom);
			return atom;
		}
		if(isTarObj)
		{
			if(!IsComplex(src)) return src;
			ScriptObject*obj=c->atomToScriptObject(src);
			ClassClosure* pClass=obj->vtable->_ctor;
			Atom args[]={NULL};
			Atom atom=pClass->construct(0,args);
			this->AS3_copySamePropertyValue(src,atom);
		}
		if(IsListAtom(src)&&IsListAtom(target))
		{
			ScriptObject*obj=c->atomToScriptObject(target);
			Toplevel*top=toplevel();
			if(IsArray(obj))
			{
				target=top->arrayClass->newArray()->atom();
			}
			else
			{
				ObjectVectorObject*pObject=(ObjectVectorObject*)obj;
				target=top->vectorClass->newVector((ClassClosure*)c->atomToScriptObject(pObject->get_type()))->atom();
			}
			this->CopyDirectList(c->atomToScriptObject(src),c->atomToScriptObject(target));
			return target;
		}
		return src;
	}

	bool SamePropertyObjectUtilClass::IsComplex(Atom atom)
	{
		if(AvmCore::isNullOrUndefined(atom)) return false;
		if(atomKind(atom)!=kObjectType) return false;
		ScriptObject*obj=core()->atomToScriptObject(atom);
		if(IsObject(obj)) return false;
		return true;
	}

	bool SamePropertyObjectUtilClass::IsListAtom(Atom atom)
	{
		if(core()->isNullOrUndefined(atom)) return false;
		if(atomKind(atom)!=kObjectType) return false;
		return IsList(core()->atomToScriptObject(atom));
	}

	bool SamePropertyObjectUtilClass::IsComplex(ClassClosure*pClass)
	{
		Toplevel*top=toplevel();
		if(pClass==top->intClass) return false;
		if(pClass==top->uintClass) return false;
		if(pClass==top->numberClass) return false;
		if(pClass==top->stringClass) return false;
		if(pClass==top->booleanClass) return false;
		return true;
	}

	void SamePropertyObjectUtilClass::CopyDirectList(ScriptObject*src,ScriptObject*target)
	{
		bool tarIsVector=IsVector(target);
		ClassClosure* typeClass = NULL;
		ObjectVectorObject*vecObj=NULL;
		ArrayObject*arrayObj=NULL;
		if(tarIsVector)
		{
			vecObj = (ObjectVectorObject*)target;
			typeClass = (ClassClosure*)AvmCore::atomToScriptObject(vecObj->get_type());
			if(!IsComplex(typeClass))
				typeClass = NULL;
			
			//target->traits()->itraits->commonBase
		}
		else
			arrayObj = (ArrayObject*)target;
		//if(src->traits()->getHashtableOffset()!=0)
		//MethodEnv*pEnv=this->getMeth
		{
			int index = 0;	
			//InlineHashtable*b=src->getTable();
			//if(b)
			{
				int index=0;
				for(;;)
				{
					index = src->nextNameIndex(index);
					if(index<=0) break;
					Atom atom=src->nextValue(index);
					//index=b->next(index);
					if(AvmCore::isNullOrUndefined(atom)) break;	
					//index++;
					/*Atom key = b->keyAt(index);
					if(!key) continue;
					//if(atomKind(atom)!=kStringType) continue;
					Stringp name=core()->atomToString(key);
					//if(!name) continue;
					
					Atom atom=b->valueAt(index);*/

					if(typeClass != NULL) //Simple
					{
						Atom atoms[]={NULL};
						Atom targetAtom = typeClass->construct(0,atoms);
						AS3_copySamePropertyValue(atom,targetAtom);
						atom = targetAtom;
					}
					{
						//Atom atoms[]={atom};
						if(vecObj)
						{
							int l = vecObj->get_length();
							vecObj->set_length(l+1);							
							vecObj->setUintProperty(l,atom);
							//vecObj->AS3_push(atoms,1);
						}
						else
						{
							int l = arrayObj->get_length();
							arrayObj->set_length(l+1);
							arrayObj->setUintProperty(l,atom);
							//arrayObj->AS3_push(atoms,1);
						}
					}
					//ShellCore::_setProperty(pObj2,key,atom,publicNS,t);
					//pObj2->setAtomProperty(key,atom);
				}				
			}
		}
	}

	inline bool SamePropertyObjectUtilClass::IsArray(ScriptObject*obj)
	{
		ClassClosure*arrayClass=toplevel()->arrayClass;		
		return    obj->traits()==arrayClass->traits()->itraits;
		//return false;
	}

	inline bool SamePropertyObjectUtilClass::IsVector(ScriptObject*obj)
	{
		ClassClosure*vecClass=toplevel()->vectorClass;
		return   obj->traits()==vecClass->traits()->itraits;
		//return false;
	}

	inline bool SamePropertyObjectUtilClass::IsObject(ScriptObject*obj)
	{
		ClassClosure*objClass=toplevel()->objectClass;
		return obj->traits()==objClass->traits()->itraits;
	}

}

