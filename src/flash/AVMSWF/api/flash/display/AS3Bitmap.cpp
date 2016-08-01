#include "StdAfxflash.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"

#include "ShellCore.h"
using namespace avmplus;
#include "AS3Bitmap.h"
namespace avmshell{
BitmapClass::BitmapClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BitmapClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BitmapObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

BitmapObject* BitmapClass::CreateBitmap(int w,int h,bool bTransparent)
{
	BitmapObject*pObj=new (core()->GetGC(), ivtable()->getExtraSize()) BitmapObject(ivtable(), prototype, 0);	
	pObj->Create(w,h,bTransparent);
	return pObj;
}



BitmapObject* BitmapClass::CreateBitmap(XImage*pImage,ShellCodeContext*pCode,void*obj)
{
	BitmapObject*pObj=new (core()->GetGC(), ivtable()->getExtraSize()) BitmapObject(ivtable(), prototype, 0);	
	//pObj->m_codeContext=pCode;
	pObj->m_pObject=(_XSObject*)obj;
	pObj->Create(pImage);
	pObj->SetCodeContext(pCode);
	return pObj;
}

BitmapObject* BitmapClass::CreateSBitmap(void*ch,ShellCodeContext*pCode,void*obj)
{
	BitmapObject*pObj=new (core()->GetGC(), ivtable()->getExtraSize()) BitmapObject(ivtable(), prototype, 0);	
	//pObj->m_pCodeContext=pCode;
	pObj->SetCodeContext(pCode);
	pObj->m_pObject=(_XSObject*)obj;
	pObj->CreateS(ch);
	return pObj;
}

BitmapObject* BitmapClass::CreateBitmap(BitmapDataObject*pData,ShellCodeContext*pCnt)
{
	BitmapObject*pObj=new (core()->GetGC(), ivtable()->getExtraSize()) BitmapObject(ivtable(), prototype, 0);
	pObj->m_codeContext=pCnt;
	pObj->AS3_constructor(pData,NULL,false);
	return pObj;
}

BitmapObject::BitmapObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pBitmapData=NULL;
	m_strPixelSnapping=core()->kEmptyString;
	m_bSmoothing=false;
//	m_sbitmap.PIInit();
}
//////////////////////////////////////////////////////////
//Native Method start...
BitmapDataObject* BitmapObject::AS3_bitmapData_get()
{
	//Add your act code here...
	//if(m_pBitmapData)
	//	m_pBitmapData->ExpandDummy();
	//if(m_pBitmapData)
	//	m_pBitmapData->ExpandDummy();
	return m_pBitmapData;
	//return kAvmThunkUndefined;//Modify this please!
}

void BitmapObject::Release()
{
	
	if(m_pBitmapData)
		m_pBitmapData->m_pObject=NULL;
	m_pBitmapData = NULL;
	DisplayObjectObject::Release();
}

void BitmapObject::AS3_bitmapData_set(BitmapDataObject*pData)
{
	//Add your act code here...
//	this->m_sbitmap.PIFree();
	//if(m_pObject&&m_pObject->xform.mat.tx==180)
	//	int a = 0;
	if(m_pBitmapData==pData) return;
	if(m_pBitmapData)
	{
		if(m_pBitmapData->m_pObject)
		{
			m_pBitmapData->m_pObject->FreeCache();
			//if(XSWFPlayer::m_pInstance->display.root.IsParentOf(m_pObject))
			//	ScriptThread::RemoveCharacter(m_pBitmapData->m_pChar);
		}
		//	m_pBitmapData->m_pChar->record.ReleaseAll();
			//ScriptPlayer::RemoveCharacter(m_pBitmapData->m_pChar);
		//m_pBitmapData->m_pChar->record.ReleaseAll();
		m_pBitmapData->m_pObject=NULL;
	}
	m_pBitmapData=pData;
	if(pData)
	{
		//pData->LockBitmap();
		//if(pData->AS3_width_get()==150&&pData->AS3_height_get()==150)
		//if(m_pBitmapData->m_pChar)
		//	ScriptPlayer::PlaceCharacter(m_pBitmapData->m_pChar);
		//	int v=0;
		pData->m_pObject=m_pObject;
	}
	if(m_pObject)
	{
		m_pObject->AS3SetReplace();
		//int w=0;	
		//int h=0;
		if(m_pObject->character->type!=bitmapEmptyChar&&m_pObject->character->tag!=loaderImageChar)
		{
			XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
			SCharacter*ch=player->CreateCharacter();//new SCharacter;
			if (ch) //return XNULL;
			{
				ch->record.Init();
				ch->data=XNULL;
				ch->className=XNULL;
				ch->gridRect=XNULL;
				ch->gridRect=XNULL;
				ch->splayer=m_pObject->character->splayer;
				ch->sprite.length=0;
				ch->sprite.numFrames=0;
				ch->classEntry=NULL;
				ch->className=NULL;
				ch->rbounds=m_pObject->character->rbounds;
				ch->tag=0; //spriteEmptyChar
				ch->type=bitmapEmptyChar;//(U8)type;
				m_pObject->character=ch;
			}
			//int v=0;
		}
		if(m_pObject->drawn)
			m_pObject->display->InvalidateRect(&m_pObject->devBounds);
		if(m_pBitmapData)
		{
			/*if(m_pBitmapData->m_pDib)
			{
				if(m_pBitmapData->m_pDib->Width()==75&&
				   m_pBitmapData->m_pDib->Height()==120)
				   int v=0;
			}*/
			int h=m_pBitmapData->AS3_height_get()*20;
			int w=m_pBitmapData->AS3_width_get()*20;
			//if(w==1120&&h==2660)
			//	int v=0;
			SRECT&r=m_pObject->character->rbounds;
			
			r.xmin=0;
			r.ymin=0;
			r.xmax=w;
			r.ymax=h;
		}
		
		m_pObject->Modify();
	}
	//m_pBitmapData->m_pDib->SaveFile("D:\\bitmap.bmp");
	//return kAvmThunkUndefined;//Modify this please!
}

Stringp BitmapObject::AS3_pixelSnapping_get()
{
	//Add your act code here...
	return m_strPixelSnapping;
	//return kAvmThunkUndefined;//Modify this please!
}

void BitmapObject::AS3_pixelSnapping_set(Stringp s)
{
	//Add your act code here...
	m_strPixelSnapping=s;
}

bool BitmapObject::AS3_smoothing_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	return m_bSmoothing;
}

void BitmapObject::AS3_smoothing_set(bool b)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	m_bSmoothing=b;
}


void BitmapObject::Create(int w,int h,bool bTransparent)
{
	//DisplayObjectObject::AS3_constructor();
	BitmapDataObject*pBitmapData=((ShellToplevel*)toplevel())->getBitmapDataClass()->CreateBitmapData(w,h,bTransparent);
		//pData;
	AS3_constructor(pBitmapData,NULL,false);
}

void BitmapObject::Create(XImage*pImage)
{
	//DisplayObjectObject::AS3_constructor();
	BitmapDataObject*pBitmapData=((ShellToplevel*)toplevel())->getBitmapDataClass()->CreateBitmapData(pImage);
		//pData;
	//m_strPixelSnapping=core()->newConstantStringLatin1("auto");
	//m_bSmoothing=false;
	//DisplayObjectObject::CreateNewObject(bitmapEmptyChar);	
	AS3_constructor(pBitmapData,NULL,false);
}

void BitmapObject::CreateS(void*ch)
{
	//m_pBitmapData=((ShellToplevel*)toplevel())->getBitmapDataClass()->CreateBitmapData(pImage);
	m_pBitmapData=((ShellToplevel*)toplevel())->getBitmapDataClass()->CreateDumpBitmap(ch);
	_XSCharacter*c=(_XSCharacter*)ch;
	//if(c->bits.width==320)
	//	int v=0;
	/*if(c->type==loaderImageChar&&c->classEntry)
	{
		((ShellToplevel*)toplevel())->LockImage((Stringp)c->classEntry);
	}*/
	m_pBitmapData->m_pObject=m_pObject;
}

//AS3 contructor function..
void BitmapObject::AS3_constructor(BitmapDataObject*pData,Stringp s,bool b)
{
	//Add your act code here...
	DisplayObjectObject::AS3_constructor();
	
	m_pBitmapData=pData;
	//if(pData)
	//	pData->LockBitmap();
	
	m_strPixelSnapping=s?s:core()->newConstantStringLatin1("auto");
	m_bSmoothing=b;	
	if(m_pObject==NULL)
	{
		void*pInitData=this->vtable->GetInitData();
		/*if(pInitData==XNULL&&pData)
		{
			vtable->m_pInitData=pData->m_pChar;
			DisplayObjectObject::CreateNewObject(bitmapEmptyChar);
			vtable->m_pInitData=XNULL;			
			//if(m_pObject&&m_pBitmapData&&m_pBitmapData->m_pDib)
			//	m_pBitmapData->m_pObject=m_pObject;
		}
		else*/
		{
			DisplayObjectObject::CreateNewObject(bitmapChar);
			if(pInitData&&m_pBitmapData==NULL)
				m_pBitmapData=((ShellToplevel*)toplevel())->getBitmapDataClass()->CreateDumpBitmap(pInitData);
		}
	}	
	if(m_pBitmapData)
	{
		int w=m_pBitmapData->AS3_width_get()*20;
		int h=m_pBitmapData->AS3_height_get()*20;
		SRECT&r=m_pObject->character->rbounds;
		r.xmin=0;
		r.ymin=0;
		r.xmax=w;
		r.ymax=h;
		m_pBitmapData->m_pObject=m_pObject;
	}
}

}
