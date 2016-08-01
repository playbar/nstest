/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 2004-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#include "StdAfxflash.h"
#include "avmplus.h"



#include "sbitmap.h"
#include "ShellCore.h"
#include "avm2.h"
#include "swfapi.h"
#include "PoolObject.h"
#include "AS3EventDispatcher.h"
#include "AS3ApplicationDomain.h"
#include "AS3Event.h"
#include "AS3ActivityEvent.h"
#include "AS3AsyncErrorEvent.h"
#include "AS3ContextMenuEvent.h"
#include "AS3DataEvent.h"
#include "AS3ErrorEvent.h"
#include "AS3FocusEvent.h"
#include "AS3FullScreenEvent.h"
#include "AS3HTTPStatusEvent.h"
#include "AS3IMEEvent.h"
#include "AS3IOErrorEvent.h"
#include "AS3KeyboardEvent.h"
#include "AS3MouseEvent.h"
#include "AS3NetStatusEvent.h"
#include "AS3ProgressEvent.h"
#include "AS3SecurityErrorEvent.h"
#include "AS3StatusEvent.h"
#include "AS3SyncEvent.h"
#include "AS3TextEvent.h"
#include "AS3TimerEvent.h"
#include "AS3IOError.h"
#include "AS3EOFError.h"
#include "AS3IllegalOperationError.h"
#include "AS3InvalidSWFError.h"
#include "AS3MemoryError.h"
#include "AS3ScriptTimeoutError.h"
#include "AS3StackOverflowError.h"
#include "AS3SecurityDomain.h"
#include "AS3URLRequest.h"
#include "AS3Loader.h"
#include "AS3BitmapData.h"

using namespace avmshell;
using namespace ExtErrorContext;

#include "splayer.h"
#include "splay.h"
#include "TypeDescriber.h"
#if defined FEATURE_NANOJIT && (defined (AVMPLUS_IA32) || defined(AVMPLUS_AMD64))
bool P4Available();
#endif

#ifdef VMCFG_TEST_API_VERSIONING
  #include "api-versions.h"
#else
  #include "noapi-versions.h"
#endif


#define _GCADVANCE_


namespace avmshell
{
	class LoaderImageObject:public MMgc::RCObject
	{
	public:
		_XSCharacter ch;
		bool m_bAttach,m_bFileMemory,m_bImage;
		int  m_nRefs;
		int  m_nLockTime;
		int	 m_nCacheTime;
		//DRCWB(Stringp) m_url;
		DWB(RCHashTable*) m_pLoaders;
		//XXVar m_strFile;
		XXVar m_msg;
		XSWFPlayer*m_pPlayer;
		virtual ~LoaderImageObject()
		{
			//m_pLoaders=NULL;
			//VMPI_memset(&ch,0,sizeof(ch));
			//m_bAttach=false;
			//m_nRefs=0;
			//m_nLockTime=0;
			m_bImage=true;
			m_pLoaders=NULL;
			VMPI_memset(this,0,sizeof(LoaderImageObject));
			//VMPI_memset(this,0,sizeof(LoaderImageObject));
		}
		LoaderImageObject(XSWFPlayer*player,RCHashTable*loaders,Stringp url)
		{
			//VMPI_memset(this,0,sizeof(LoaderImageObject));
			m_pLoaders=loaders;
			m_bFileMemory=false;
			m_msg=_strEmpty;
			//m_url=url;
			m_pPlayer=player;
			ch.next=NULL;
			ch.splayer=NULL;
			ch.tagCode=0;			// the stag code from the define tag
			ch.data=NULL;
			ch.length=0;
			ch.classEntry=NULL;
			m_nCacheTime=4000;
			m_nLockTime=0;//player->m_nRunTime+m_nCacheTime;
			this->m_bImage=true;
			XDOMFILTER*f=player->GetFilter();
			if(f)
				m_nCacheTime=f->nCacheBitmap;
			RectSetEmpty(&ch.rbounds);
			ch.classEntry=url;
#ifdef _WINEMU
			ch.bFilters=NULL;
#endif
			ch.tag=ctagBitmapLoader;
			ch.type=loaderImageChar;
		}
	public:
//		bool CheckToFile()
//		{
//			if(!m_bImage) return false;
//			if(m_bFileMemory||ch.length<10240) return false;
//			XDOMFILTER*pFilter=m_pPlayer->GetFilter();
//			if(!pFilter||!pFilter->nCacheMemory) return false;
//			if(m_pPlayer->m_nRunTime-m_nLockTime<pFilter->nCacheMemory) return false;
//			MemoryToFile();
//			return true;
//		}
//		void MemoryToFile()
//		{
//			if(!m_bImage) return;
//			if(m_bFileMemory||!ch.data) return;
//			XXVar var=m_pPlayer->m_strWorkCache;
//			//var.StringAdd("\\Image");
//			//XXVar ivar((int)this);
//			//var.StringAdd(ivar);
//			XFile file;
//			if(file.Open(m_strFile.strTxt,XFile::XCREATE|XFile::XWRITE|XFile::XBINARY))
//			{
//				file.Write(ch.data,ch.length);
//				m_bFileMemory=true;
//				delete ch.data;
//				ch.data=NULL;
//#ifdef _WINEMU
//				XString16 s("[File Memory] Saved ");
//				XString16 is;
//				is.FromInt(ch.length);
//				s+=is;
//				s+=XString16(" bytes to file!");
//				m_pPlayer->Trace(s);
//#endif
//			}
//		}
//		void FileToMemory()
//		{
//			if(!m_bImage) return;
//			if(!m_bFileMemory||ch.data) return;
//			XXVar var=m_pPlayer->m_strWorkCache;
//			//var.StringAdd("\\Image");
//			//XXVar ivar((int)this);
//			//var.StringAdd(ivar);
//			XFile file;
//			this->m_nLockTime=m_pPlayer->m_nRunTime;
//			if(file.Open(m_strFile.strTxt,XFile::XREAD|XFile::XBINARY))
//			{
//				m_nCacheTime=m_nCacheTime*2;
//				ch.data=new XU8[ch.length];
//				file.Read(ch.data,ch.length);
//				m_bFileMemory=false;
//#ifdef _WINEMU
//				XString16 s("[File Memory] Restore ");
//				XString16 is;
//				is.FromInt(ch.length);
//				s+=is;
//				s+=XString16(" bytes from file!");
//				m_pPlayer->Trace(s);
//#endif
//			}
//		}

		bool IsComplete()
		{
//#if (__CORE_VERSION__>=0x02078000)
			if(ch.data!=NULL) return true;
			if(!this->m_msg.IsNull()) return true;
			return false;
//#else
//			return ch.data!=NULL;
//#endif
		}
		void Connect(LoaderObject*loader)
		{
			m_pLoaders->put(loader);
			//BuildBits();
			//if(m_bImage)
			//	FileToMemory();
			//else
			//	Lock();
		}
		void DisConnect(LoaderObject*loader)
		{
			m_nLockTime=m_pPlayer->m_nRunTime;
			m_pLoaders->remove(loader,true);
			//if(!m_bImage)
			//	this->Unlock();
		}
		void PostComplete(const char*msg,LoaderObject*self)
		{
			int i,iCount=m_pLoaders->numQuads;
			BuildBits();
			if(ch.type==loaderImageChar)
			{
				ch.rbounds.xmin=0;
				ch.rbounds.ymin=0;
				ch.rbounds.xmax=ch.bits.width*20;
				ch.rbounds.ymax=ch.bits.height*20;
			}
			if(self)
				self->CacheComplete(&ch,msg);
			for(i=0;i<iCount;i++)
			{
				LoaderObject*obj=(LoaderObject*)m_pLoaders->valueAt(i);
				if(!obj||obj==self) continue;
				//Lock();
				obj->CacheComplete(&ch,msg);
				m_pLoaders->Clear(i);
			}
			//m_pLoaders->Clear(
		}
//#if (__CORE_VERSION__>=0x02075000)
		int GetRefCount(LoaderObject*self)
		{
			int iNum=0;
			int i,iCount=m_pLoaders->numQuads;
			for(i=0;i<iCount;i++)
			{
				LoaderObject*obj=(LoaderObject*)m_pLoaders->valueAt(i);
				if(!obj||obj==self) continue;
				//Lock();
				iNum++;
			}
			return iNum;
		}
//#endif
		void Attach(void*pData,int l,bool bAttach,bool bImage,const char*msg)
		{
			m_nLockTime=m_pPlayer->m_nRunTime;
			if(ch.data)
			{
				if(!bAttach)
					delete (XU8*)pData;
				//Lock();
				return;
			}
			if(msg)
			{
				m_msg=msg;
				m_msg.ToString(XTRUE);
			}
			else
				m_msg=_strEmpty;
			m_bAttach=bAttach;
			ch.data=(XU8*)pData;
			ch.length=l;
			m_bFileMemory=false;
			m_bImage=bImage;
			if(bImage)
			{
				XDOMFILTER*pFilter=this->m_pPlayer->GetFilter();
				ch.bits.PIInit();
				BuildBits();
				ch.tag=ctagBitmapLoader;
				ch.type=loaderImageChar;
				//ch.bits.LockBits();
				ch.className="Bitmap";
				//m_strFile=m_pPlayer->m_strWorkCache;
				//m_strFile.StringAdd("IMAGE");
				//XXVar iVar((int)this);
				//m_strFile.StringAdd(iVar);


			}
			else
			{
				ch.tag=ctagSWFLoader;
				ch.type=loaderSWFChar;
				ch.className="SWF";
				//XDOMFILTER*pFilter=player->GetFilter();
				//ch.bits.PIInit(pFilter?pFilter->nImgDiv:0);
			}
			//Lock();
		}
		Atom atom()
		{
			return (Atom)this|kObjectType;
		}
		void BuildBits()
		{
			if(!m_bImage) return;
			if(ch.bits.bitmapID) return;
			if(ch.data&&!ch.bits.HasBits())
			{
				ch.bits.PICreate(ch.data,ch.length);
				ch.rbounds.xmin=0;
				ch.rbounds.ymin=0;
				ch.rbounds.xmax=ch.bits.width*20;
				ch.rbounds.ymax=ch.bits.height*20;
				//ch.bits.CreateTexture();//false);
				//ch.bits.LockBits();
				//ch.bits.ReleaseData();
				//ch.bits.CalcTrans();

			}
		}
		bool Lock()
		{
//			FileToMemory();
			if(!ch.data) return false;
			BuildBits();
			m_nLockTime=m_pPlayer->m_nRunTime;
			m_nRefs++;
			return true;
		}
		bool Unlock()
		{
			if(!m_nRefs) return false;
			m_nLockTime=m_pPlayer->m_nRunTime;
			m_nRefs--;
			if(m_nRefs<=0&&m_bImage)
			{
				ch.bits.PIFree();
			}
			return true;
		}
		bool IsOutofTime(bool bImage)
		{
			if(m_nLockTime==0) return false;
			if(!m_bImage) 
			{
				return m_nRefs==0;
			}
			//[Note X]
			//return false;
			if(!ch.bits.bitmapID) return true;
			if(m_nRefs||RasterGL::IsLocked(ch.bits.bitmapID)) return false;
			XDOMFILTER*f=m_pPlayer->GetFilter();
			return (m_pPlayer->m_nRunTime-m_nLockTime)>this->m_nCacheTime;//(int)(f?f->nCacheBitmap:5000);
		}
		virtual void Release()
		{
			if(!m_bAttach&&ch.data)
			{
#ifdef _WINEMU
				XString16 s("[NET File] Free cached image(or swf) ");
				XString16 is;
				is.FromInt(ch.length);
				s+=is;
				s+=XString16(" bytes!");
				m_pPlayer->Trace(s);
#endif
				delete ch.data;
			}
			ch.data=NULL;
			if(m_bImage)
				ch.bits.PIFree();
		}

	};


	void ShellToplevel::AddBitmap(BitmapDataObject*obj)
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		//if(player->m_pDomFilter&&player->m_pDomFilter->nCacheBitmap==0) return;
		XDOMFILTER*pFilter=player->GetFilter();
		if(pFilter&&pFilter->bAdvanceGC)
		//if(m_bitmapCaches->numQuads>0&&obj==m_bitmapCaches->valueAt(0))
		//	int v=0;
			m_bitmapCaches->put(obj);
	}
	void ShellToplevel::RemoveBitmap(BitmapDataObject*obj)
	{
		//TextureInfo info;
		//obj->GetBmInfo(info);
		//if(info.w==1280&&info.h==630)
		//	int v=0;
		//if(m_bitmapCaches->numQuads>0&&obj==m_bitmapCaches->valueAt(0))
		//	int v=0;
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		//if(player->m_pDomFilter&&player->m_pDomFilter->nCacheBitmap==0) return;
		XDOMFILTER*pFilter=player->GetFilter();
		if(pFilter&&pFilter->bAdvanceGC)
			m_bitmapCaches->remove(obj,true);
	}

#ifdef _ANALY
	void ShellToplevel::AddAsBitmap(XDrawDib*obj)
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(player->m_pDomFilter&&player->m_pDomFilter->nCacheBitmap==0) return;
		//XDOMFILTER*pFilter=player->GetFilter();
		//if(pFilter&&pFilter->nCacheMemory)
			m_asBitmaps->put((ScriptObject*)obj);
	}
	void ShellToplevel::RemoveAsBitmap(XDrawDib*obj)
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(player->m_pDomFilter&&player->m_pDomFilter->nCacheBitmap==0) return;
		//XDOMFILTER*pFilter=player->GetFilter();
		//if(pFilter&&pFilter->nCacheMemory)
			m_asBitmaps->remove((ScriptObject*)obj,true);
	}
#endif
/*	static int Compress(XDib*pDib,XU8Array&data)
	{
		int nSize=pDib->m_nDataLength;
		data.SetSize(nSize+32);
		XU8*pData=data.GetData();
		XU32*nHeader=(XU32*)data.GetData();
		nHeader[0]=pDib->Width();
		nHeader[1]=pDib->Height();
		nHeader[2]=0;
		pData+=12;
		int gzlen=data.GetSize()-12;
		compress2((Bytef*)pData, (unsigned long*)&gzlen,
				  (Bytef*)pDib->GetBuffer(), nSize, 9);
		nHeader[2]=gzlen;
		return gzlen+12;
	}
	static XDrawDib* UnCompress(XU8Array&data)
	{
		XU32*nHeader=(XU32*)data.GetData();
		if(nHeader[0]>10000||nHeader[1]>10000||
           nHeader[0]<=0||nHeader[1]<=0) return NULL;
		if(nHeader[2]+12>data.GetSize()) return NULL;
		XDrawDib*pDib=new XDrawDib();
		if(!pDib->Create(nHeader[0],nHeader[1],32))
		{
			delete pDib;
			return NULL;
		}
		int error = Z_OK;
		{
			// Decompress the data
			z_stream zstream;

			XGlobal::Memset(&zstream, 0, sizeof(zstream));
			error = inflateInit(&zstream);

			zstream.next_in = (Bytef*)(data.GetData()+12);
			zstream.avail_in = nHeader[2];

			//uint8_t *buffer = mmfx_new_array( uint8_t, kBufferSize );

			do {
				zstream.next_out=pDib->GetAddress();
				zstream.avail_out=pDib->m_nDataLength;
				//zstream.SetNextOut(buffer);
				//zstream.SetAvailOut(kBufferSize);
				error = inflate(&zstream, Z_NO_FLUSH);
					//zstream.InflateWithStatus();
				//m_byteArray.Write(buffer, kBufferSize - zstream.AvailOut());
			} while (error == Z_OK);

			error = inflateEnd(&zstream);

		}
		return pDib;
	}*/
	/*bool ShellToplevel::RestoreBitmap(BitmapDataObject*obj)
	{
		if(m_bitmapCaches->indexOf(obj)<0) return false;
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		XXVar strFile=player->m_strWorkCache;
		strFile.StringAdd("BITMAP");
		XXVar iVar((int)obj);
		strFile.StringAdd(iVar);
		XFile file;
		if(!file.Open(strFile.strTxt,XFile::XBINARY|XFile::XREAD)) return false;
		int l=file.GetLength();
		if(l<=16) return false;
		XU32 nFlag;
		file.Read(&nFlag,4);
		l-=4;
		if(nFlag==0xffffffff)
		{
			XU8Array data;
			data.SetSize(l);
			file.Read(data.GetData(),l);
			file.Close();		
			obj->m_pDib=UnCompress(data);
		}
		else if(nFlag==0xfffffff1)
		{
			
			XU32 nHeader[3];
			file.Read(nHeader,12);
			if(nHeader[0]>0&&nHeader[1]>0&&
               nHeader[1]<10000&&nHeader[1]<10000&&
			   l>=nHeader[0]*nHeader[1]*4&&nHeader[2]<=l+4)
			{
				obj->m_pDib=new XDrawDib();
				if(!obj->m_pDib->Create(nHeader[0],nHeader[1],32))
				{
					delete obj->m_pDib;
					obj->m_pDib=NULL;
				}
				else
					file.Read(obj->m_pDib->GetBuffer(),obj->m_pDib->m_nDataLength);
			}
			file.Close();
		}
		
		obj->m_nCacheTime=obj->m_nCacheTime*2;
		obj->m_nLockTime=player->m_nRunTime;
#ifdef _WINEMU
		if(obj->m_pDib)
		{
			XString16 s("[Note:Bitmap] Restore destoried bitmap(");
			XString16 is;
			is.FromInt(obj->m_pDib->m_nDataLength);
			s+=is;
			s+=XString16(" bytes)");
			player->Trace(s);
		}
#endif
		return true;
	}*/

#ifdef _ANALY
	static inline _DIBINFO* _AddInfo(_DIBINFO*pRoot,int nType)
	{
		_DIBINFO*pInfo=new _DIBINFO();
		pInfo->pNext=NULL;
		pInfo->pDib=0;
		pInfo->isAttach=false;
		pInfo->nType=nType;
		pInfo->nCacheTime=0;
		pInfo->nLastTime=0;
		pInfo->nLockCount=0;
		pInfo->nUseTime=0;
		pRoot->pNext=pInfo;
		return pInfo;
	}

	_DIBINFO* ShellToplevel::GetDibInfo(_DIBINFO*pRoot,XU32 nTime)
	{
		//int i;
		//InlineHashtable*hash=m_loadImagesTable->get_ht();
		//Atom *atoms=hash->getAtoms();
		//int ic=hash->getCapacity();
		//for(i=0;i<ic;i+=2)
		//{
		//	Atom atom=atoms[i+1];//m_loadImagesTable->valueAt(i);
		//	if(AvmCore::isNullOrUndefined(atom)) continue;
		//	LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		//	if(obj->ch.type!=loaderImageChar||!obj->ch.bits.HasBits()) continue;
		//	pRoot=_AddInfo(pRoot,1);
		//	pRoot->nCacheTime = obj->m_nCacheTime;
		//	pRoot->nLastTime = obj->m_nLockTime;
		//	pRoot->nLockCount = obj->m_nRefs;
		//	pRoot->nUseTime = nTime - obj->m_nLockTime;
		//	if(obj->ch.bits.width==2142)
		//		int v=0;
		//	//if(obj->ch.type==loaderImageChar&&obj->ch.bits.HasBits())
		//	{
		//		XDrawDib*pDib = new XDrawDib();
		//		pRoot->pDib = pDib;
		//		pDib->CreateFrom(&obj->ch.bits,false);
		//	}
		//}
		//ic=m_bitmapCaches->numQuads;
		//	
		////XU32 nTime=player->m_nRunTime;
		//for(i=0;i<ic;i++)
		//{
		//	BitmapDataObject* obj=(BitmapDataObject*)m_bitmapCaches->valueAt(i);
		//	if(obj==NULL||obj==m_bitmapCaches->m_pDump) continue;
		//	if(!obj->m_pChar->bits.HasBits()) continue;
		//	pRoot=_AddInfo(pRoot,2);
		//	SBitmapCore*pb = &obj->m_pChar->bits;
		//	pRoot->nCacheTime = pb->m_nMaxCacheTime;
		//	pRoot->nLastTime = pb->m_nLockTime;
		//	pRoot->nUseTime = nTime - pb->m_nLockTime;
		//	pRoot->nLockCount = pb->lockCount;
		//	pRoot->isAttach = pb->pAlloc==XNULL;//false;//obj->m_pDib->IsAttach();
		//	{
		//		XDrawDib*pDib = new XDrawDib();
		//		pRoot->pDib = pDib;
		//		pDib->CreateFrom(&obj->m_pChar->bits,false);
		//	}
		//	//pRoot->pDib = obj->m_pDib->Clone();
		//}

		//ic=m_asBitmaps->numQuads;
		//	
		////XU32 nTime=player->m_nRunTime;
		//for(i=0;i<ic;i++)
		//{
		//	XDrawDib* obj=(XDrawDib*)m_asBitmaps->valueAt(i);
		//	if(obj==NULL||(ScriptObject*)obj==m_bitmapCaches->m_pDump) continue;
		//	if(!obj->m_pDib) continue;
		//	pRoot=_AddInfo(pRoot,3);
		//	//pRoot->nCacheTime = obj->m_nCacheTime;
		//	//pRoot->nLastTime = obj->m_nLockTime;
		//	//pRoot->nUseTime = nTime - obj->m_nLockTime;
		//	//if(obj->m_pSbitmap)
		//	//	pRoot->nLockCount = obj->m_pSbitmap->lockCount;
		//	pRoot->isAttach = obj->IsAttach();
		//	pRoot->pDib = obj->Clone();
		//}

		return pRoot;
	}
#endif
	int ShellToplevel::ClearDeadImage(int id,XU32 t,XU32 tMax)
	{
		int i,ic=m_bitmapCaches->numQuads;
		id=id*32;
		int iCount=0;
		//if(id==0)
		//	int v=0;
		for(i=0;i<32;i++)
		{
			if(id+i>=ic) return iCount;
			BitmapDataObject* obj=(BitmapDataObject*)m_bitmapCaches->valueAt(id+i);
			if(obj==NULL||obj==m_bitmapCaches->m_pDump) continue;
			if(obj->SaveIfNeed(t,tMax))
			{
				m_bitmapCaches->Clear(id+i);
				iCount++;
			}
			id++;
		}
		return iCount;
		//return 0;
	}

	int ShellToplevel::ClearDeadImage(bool bForce)
	{
		int iCount=0;
		int i=0;
		InlineHashtable*hash=m_loadImagesTable->get_ht();
		Atom *atoms=hash->getAtoms();
		MMgc::GC* gc=MMgc::GC::GetGC(atoms);
		int ic=hash->getCapacity();
		for(i=0;i<ic;i+=2)
		{
			//i=m_loadImagesTable->next(i);
			//if(!i) break;
			Atom atom=atoms[i+1];//m_loadImagesTable->valueAt(i);
			if(AvmCore::isNullOrUndefined(atom)) continue;
			LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);

			if(bForce||obj->IsOutofTime(true))
			{
				//obj->IsOutofTime(true);
				obj->Release();
				WBATOM(gc,atoms,&atoms[i+1],nullObjectAtom);
			}
			//else obj->CheckToFile();
		}
////#ifdef _GCADVANCE_
//	//	XSWFPlayer*player=core()->GetPlayer();
//	//	//XDOMFILTER*f=player->GetFilter();
//	//	//if(f&&f->bAdvanceGC)
//	//	{
//	//		ic=m_bitmapCaches->numQuads;
//	//		
//	//		//if(player->m_pDomFilter&&player->m_pDomFilter->nCacheBitmap==0) return iCount;
//	//		XU32 nTime=player->m_nRunTime;
//	//		for(i=0;i<ic;i++)
//	//		{
//	//			BitmapDataObject* obj=(BitmapDataObject*)m_bitmapCaches->valueAt(i);
//	//			if(obj==NULL||obj==m_bitmapCaches->m_pDump) continue;
//	//			if(!obj->m_pChar->bits.HasBits()) continue;
//	//			SBitmapCore* bits=&obj->m_pChar->bits;
//	//			//if(!obj->m_pDib||obj->m_pObject) continue;
//	//			//if(obj->m_pDib->IsAttach()) continue;
//	//			//if(obj->m_sbitmap.lockCount) continue;
//	//			//XDOMFILTER*f=player->GetFilter();
//	//			if((player->m_nRunTime-bits->m_nLockTime)>bits->m_nMaxCacheTime)
//	//			{
//	//				bits->m_nLockTime=player->m_nRunTime;
//	//				obj->CreateTexture();//FreeBits();
//	//				/*XXVar strFile=player->m_strWorkCache;
//	//				strFile.StringAdd("BITMAP");
//	//				XXVar iVar((int)obj);
//	//				strFile.StringAdd(iVar);
//	//				XFile file;
//	//				if(file.Open(strFile.strTxt,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
//	//				{
//	//#ifdef _WINEMU
//	//							XString16 s("[Note:Bitmap] Destory no used bitmap(");
//	//							XString16 is;
//	//							is.FromInt(obj->m_pDib->m_nDataLength);
//	//							s+=is;
//	//							s+=XString16(" bytes)");
//	//							player->Trace(s);
//	//#endif
//	//					if(obj->m_pDib->m_nDataLength<25600000)
//	//					{
//	//						XU32 nFlag=0xfffffff1;						
//	//						XU32 nHeader[4]={0xfffffff1,obj->m_pDib->Width(),obj->m_pDib->Height(),
//	//										 obj->m_pDib->m_nDataLength+16};
//	//						file.Write(nHeader,16);
//	//						file.Write(obj->m_pDib->GetBuffer(),obj->m_pDib->m_nDataLength);
//	//						delete obj->m_pDib;
//	//						obj->m_pDib=NULL;
//	//					}
//	//					else
//	//					{
//	//						XU8Array data;
//	//						int l=Compress(obj->m_pDib,data);
//	//						if(l>0)
//	//						{	
//	//							XU32 nFlag=0xffffffff;
//	//							file.Write(&nFlag,4);
//	//							file.Write(data.GetData(),l);
//	//							//obj->AS3_dispose();
//	//							obj->m_sbitmap.PIFree(true);
//	//							delete obj->m_pDib;
//	//							obj->m_pDib=XNULL;
//	//							//delete obj->m_pDib;
//	//							//obj->m_pDib=NULL;
//
//	//						}
//	//					}
//	//				}*/
//	//			}
//	//		}
//	//	}
		return iCount;
	}

	void ShellToplevel::SetActive(Stringp url)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		obj->BuildBits();
		//obj->FileToMemory();
	}

	bool ShellToplevel::TriggerComplete(Stringp url,LoaderObject*self)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return false;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		if(obj->IsComplete())
		{
			obj->PostComplete(obj->m_msg.IsNull()?NULL:obj->m_msg.strTxt,self);
			return true;
		}
		return false;
	}

	bool ShellToplevel::RemoveImage(Stringp url,bool bPost)
	{
		if(bPost)
		{
			Atom atom=m_loadImagesTable->get(url->atom());
			if(AvmCore::isNullOrUndefined(atom)) return false;
			LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
			if(obj)
			{
				if(!obj->IsComplete())
					obj->PostComplete("error",NULL);
				else
					obj->PostComplete(obj->m_msg.IsNull()?NULL:obj->m_msg.strTxt,NULL);
			}
		}
		return m_loadImagesTable->remove(url->atom())!=undefinedAtom;
	}

	void* ShellToplevel::CreateImage(Stringp url)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(!AvmCore::isNullOrUndefined(atom))
		{
			return (LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		}
		ShellCore*c=(ShellCore*)core();
		RCHashTable*hash=new (c->GetGC())RCHashTable((ScriptObject*)this->m_pDumpObject.value(),4,false);
		LoaderImageObject*obj=new (core()->GetGC())LoaderImageObject(c->GetPlayer(),hash,url);
		this->m_loadImagesTable->add(url->atom(),obj->atom());
		return obj;
	}
	bool  ShellToplevel::Connect(Stringp url,LoaderObject*loader)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return false;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		obj->Connect(loader);
		return true;
	}

//#if (__CORE_VERSION__>=0x02075000)
	int   ShellToplevel::GetRefCount(LoaderObject*loader,Stringp url)
	{
		if(url==NULL) return 0;
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return 0;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		return obj->GetRefCount(loader);
	}
//#endif

	bool  ShellToplevel::DisConnect(Stringp url,LoaderObject*loader)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return false;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		obj->DisConnect(loader);
		if(obj->IsOutofTime(obj->m_bImage))
			m_loadImagesTable->remove(atom);
		return true;
	}

	void* ShellToplevel::GetImage(Stringp url)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return NULL;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		if(!obj->Lock())
		{
			obj->Release();
			m_loadImagesTable->add(url->atom(),nullObjectAtom);//remove(url->atom());
			return NULL;
		}
		return &obj->ch;
	}
	bool	ShellToplevel::LockImage(Stringp url)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return false;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		return obj->Lock();
	}
	bool	ShellToplevel::UnLockImage(Stringp url)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return false;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		return obj->Unlock();
	}
	/*bool ShellToplevel::ReleaseImage(Stringp url)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		if(AvmCore::isNullOrUndefined(atom)) return false;
		LoaderImageObject*obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		return obj->Unlock();
	}*/
	void* ShellToplevel::Attach(Stringp url,void*pData,int l,bool bAttach,bool bImage,const char*msg)
	{
		Atom atom=m_loadImagesTable->get(url->atom());
		LoaderImageObject*obj=NULL;
		if(AvmCore::isNullOrUndefined(atom))
		{
			obj=(LoaderImageObject*)CreateImage(url);
				//return NULL;
		}
		else
			obj=(LoaderImageObject*)AvmCore::atomToScriptObject(atom);
		
		if(!obj) return XNULL;
		obj->Attach(pData,l,bAttach,bImage,msg);
		obj->PostComplete(msg,NULL);
		return &obj->ch;
	}

#ifdef _WINEMU
	ShellToplevel::XXObjects ShellToplevel::m_allObjects;
#endif

	ShellCodeContext::~ShellCodeContext()
	{
		//for(int i=0;i<XCHAIN_MAX;i++)
		//	FreeChain((ObjectChain**)&m_eventChains[i]);

	}
	ShellCodeContext::ShellCodeContext(ShellCore*pCore,ScriptObject*pDump,MMgc::GC* gc):
		CodeContext()
//#if (__CORE_VERSION__>=0x02070200)
//#else
//#ifdef _CODEPOST_
//#else
//	    ,m_eventsBinding(new (gc) RCHashTable(pDump,4,false))
//#endif
//#endif
		,m_bLazy(false)
		,m_pCore(pCore)

	{
		//for(int i=0;i<XCHAIN_MAX;i++)
		//	m_eventChains[i]=NULL;
	}

	/*void	ShellCodeContext::AddEvent(int type,ScriptObject*pObject)
	{
		AvmAssert(type<XCHAIN_MAX);
		if(!InChain((ObjectChain*)m_eventChains[type],pObject))
		{
			if(m_eventChains[type]==NULL)
				AddChainHead((ObjectChain**)&m_eventChains[type],pObject);
			else
				AddChainTail((ObjectChain**)&m_eventChains[type],pObject);
		}
	}
	void	ShellCodeContext::RemoveEvent(int type,ScriptObject*pObject)
	{
		AvmAssert(type<XCHAIN_MAX);
		if(m_eventChains[type])
			RemoveChain((ObjectChain**)&m_eventChains[type],pObject);
	}*/
	Stringp ShellCodeContext::GetHost(AvmCore*core)
	{
		if(!m_pRoot) return NULL;
		const char* url=m_pRoot->m_url.strTxt;
		const char* start=strstr(url,"://");
		if(start==NULL)
			return core->newConstantStringLatin1("localhost");
		const char* end=strchr(url,'/');
		if(!end)
			return core->newStringUTF8(start);
		else
			return core->newStringUTF8(start,end-start);
	}
//#if (__CORE_VERSION__>=0x02070200)
	bool ShellCore::AddPostEvent(ScriptObject*pObject)
	{
		POSTCHAIN*pNewChain=(POSTCHAIN*)postAlloc.Alloc();
		if(pNewChain==NULL) return false;
		pNewChain->EventObject=pObject;
		pNewChain->pNext=NULL;
		POSTCHAIN**pLink=&m_pPostChain;
		for(;;)
		{
			POSTCHAIN*p=*pLink;
			if(p==NULL)
			{
				*pLink=pNewChain;
				return true;
			}
			else if(p->EventObject==pObject)
			{
				postAlloc.Free(pNewChain);
				return false;
			}
			pLink=&p->pNext;
		}
		return true;
	}
	bool ShellCore::RemovePostEvent(ScriptObject*pObject)
	{
		POSTCHAIN**pChain=&m_pPostChain;
		for(;;)
		{
			POSTCHAIN*p=*pChain;
			if(p==NULL) return false;
			if(p->EventObject==pObject)
			{
				*pChain=p->pNext;
				postAlloc.Free(p);
				return true;
			}
			pChain=&p->pNext;
		}
		return false;
	}
	bool ShellCore::PostEvent(class EventObject*pEvent,bool bMast)
	{
		bool r=false;
		ShellToplevel*top=(ShellToplevel*)shell_toplevel;
		if(pEvent->nEventID>0)
		{
			//RCHashTable*hash=top->m_eventsBinding;
			//int i=0,nSize=hash->numQuads;
			POSTCHAIN*pChain=m_pPostChain;
			while(pChain)
			{		
				POSTCHAIN*pNext=pChain->pNext;
				EventDispatcherObject*p=(EventDispatcherObject*)pChain->EventObject;
				if(p->IsEnter())
				{
					pEvent->pTarget=p;
					pEvent->bStoped=false;
					pEvent->bStopedLater=false;
					p->DispatchEvent(pEvent);
				}
				else
					p->SetEnter(true);
				pChain=pChain->pNext;
			}
		}
		return r;
	}
//#else
//#ifdef _CODEPOST_
//	bool	ShellCore::PostEvent(class EventObject*pEvent,bool bMask)
//	{
//
//		bool r=false;
//		ShellToplevel*top=(ShellToplevel*)shell_toplevel;
//		if(pEvent->nEventID>0)
//		{
//			RCHashTable*hash=top->m_eventsBinding;
//			int i=0,nSize=hash->numQuads;
//			for(i=0;i<nSize;i++)
//			{				
//				EventDispatcherObject*p=(EventDispatcherObject*)hash->valueAt(i);
//				if(!p||p==hash->m_pDump) continue;
//				if(p->IsEnter())
//				{
//					pEvent->pTarget=p;
//					pEvent->bStoped=false;
//					pEvent->bStopedLater=false;
//					p->DispatchEvent(pEvent);
//				}
//				else
//					p->SetEnter(true);
//			}
//		}
//		return r;
//	}
//	
//#else
//	bool	ShellCodeContext::PostEvent(class EventObject*pEvent,bool bMask)
//	{
//
//		bool r=false;
//		if(pEvent->nEventID>0)
//		{
//			RCHashTable*hash=m_eventsBinding;
//			int i=0,nSize=hash->numQuads;
//			for(i=0;i<nSize;i++)
//			{				
//				EventDispatcherObject*p=(EventDispatcherObject*)hash->valueAt(i);
//				if(!p||p==hash->m_pDump) continue;
//				if(p->IsEnter())
//				{
//					pEvent->pTarget=p;
//					pEvent->bStoped=false;
//					pEvent->bStopedLater=false;
//					p->DispatchEvent(pEvent);
//				}
//				else
//					p->SetEnter(true);
//			}
//		}
//		return r;
//	}
//#endif
//#endif
	const int kScriptTimeout = 15;
	const int kScriptGracePeriod = 5;
#ifdef _DEBUG
	ShellCoreSettings::ShellCoreSettings()
		: arguments(NULL)
		, numargs(-1)
		, nodebugger(false)
		, astrace_console(0)
		, do_verbose(0)
		, enter_debugger_on_launch(false)
		, interrupts(AvmCore::interrupts_default)
		, verifyall(AvmCore::verifyall_default)
		, sse2(AvmCore::sse2_default)
        , fixed_esp(AvmCore::fixed_esp_default)
		, greedy(false)
		, nogc(true)
		, incremental(true)
		, langID(-1)
		, cseopt(AvmCore::cseopt_default)
		, jitordie(AvmCore::jitordie_default)
		, runmode(AvmCore::runmode_default)
		, st_component(NULL)
		, st_category(NULL)
		, st_name(NULL)
		, api(0xffffffff)

#else
	ShellCoreSettings::ShellCoreSettings()
		: arguments(NULL)
		, numargs(-1)
		, nodebugger(true)
		, astrace_console(0)
		, do_verbose(0)
		, enter_debugger_on_launch(true)
		, interrupts(AvmCore::interrupts_default)
		, verifyall(AvmCore::verifyall_default)
		, sse2(AvmCore::sse2_default)
        , fixed_esp(AvmCore::fixed_esp_default)
		, greedy(false)
		, nogc(false)
		, incremental(true)
		, langID(-1)
		, cseopt(AvmCore::cseopt_default)
		, jitordie(AvmCore::jitordie_default)
		, runmode(AvmCore::runmode_default)
		, st_component(NULL)
		, st_category(NULL)
		, st_name(NULL)
		, api(0xffffffff)

#endif
	{

	}

	/*void ShellToplevel::ExportPoolTraits(PoolObject*pool)
	{
		int nc=pool->scriptCount();
		if(nc<=0) return;
		Traits*scriptTraits=pool->getScriptTraits(nc-1);
		StTraitsBindingsIterator iter(scriptTraits->getTraitsBindings());
		while (iter.next())
		{
			// don't need to check for DELETED because we never remove trait bindings
			Stringp name = iter.key();
			if (!name) continue;
			Namespacep ns = iter.ns();

			// not already in the table then export it
			// otherwise we keep the first one that was encountered.
			if (!ns->isPrivate())
			{
				if (!domainEnv->getNamedScript(name, ns))
				{
					// add ns/name to global table
					// ISSUE should we filter out Object traits and/or private members?
					#ifdef AVMPLUS_VERBOSE
					if (scriptTraits->pool->isVerbose(VB_parse))
						console << "exporting " << ns << "::" << name << "\n";
					#endif
					domainEnv->addNamedScript(name, ns, scriptEnv);
				}
			}
			else
			{
				if (!abcEnv->getPrivateScriptEnv(name, ns))
				{
					abcEnv->addPrivateScriptEnv(name, ns, scriptEnv);
				}
			}
		}
	}*/

	ShellToplevel::ShellToplevel(AbcEnv* abcEnv) : Toplevel(abcEnv)
	{
		shellClasses = (ClassClosure**) core()->GetGC()->Calloc(avmplus::NativeID::TotalInclude_abc_class_count,
																sizeof(ClassClosure*),
																MMgc::GC::kZero | MMgc::GC::kContainsPointers);
		m_pDumpObject=new(core()->GetGC())RCObject();
		pSystemEvents=new (core()->GetGC())MultinameHashtable();
		m_loadImagesTable=new (core()->GetGC(), 0) HeapHashtable(core()->GetGC());
//		m_traitsPool=new (core()->GetGC(),0) HeapHashtable(core()->GetGC());
		m_extFuns=new (core()->GetGC(),0)HeapHashtable(core()->GetGC());
		m_classAlias=new(core()->GetGC(),0)HeapHashtable(core()->GetGC());
		m_aliasClasses=new(core()->GetGC(),0)HeapHashtable(core()->GetGC());
		pRCObjects=new (core()->GetGC())RCHashTable((ScriptObject*)m_pDumpObject.value(),1024,true);
//#if (__CORE_VERSION__>=0x02080000)
		m_pCallbackObjects=new (core()->GetGC())RCHashTable((ScriptObject*)m_pDumpObject.value(),8,true);
//#endif
		m_bitmapCaches=new (core()->GetGC())RCHashTable((ScriptObject*)this->m_pDumpObject.value(),8,false);
#ifdef _ANALY
		m_asBitmaps=new (core()->GetGC())RCHashTable((ScriptObject*)this->m_pDumpObject.value(),8,false);
#endif
		pReleaseObjects=new (core()->GetGC())RCHashTable((ScriptObject*)m_pDumpObject.value(),32,false);
		pEventNames=new Stringp[XAS3_EVENT_MAX];
		VMPI_memset((Stringp*)pEventNames,0,XAS3_EVENT_MAX*sizeof(Stringp));

#ifdef _CODEPOST_
		m_eventsBinding=new (core()->GetGC()) RCHashTable((ScriptObject*)m_pDumpObject.value(),4,false);
#else
		
#endif

		//pAllObjects=new RCHashTable(1024,true);
		//for(int i=0;i<XAS3_EVENT_MAX;i++)
		//pEventNames[i]=(Stringp)NULL;
	}

	#define _BIND_SYS(NAME,ID)\
	pSystemEvents->add(pClass->getSlot##NAME(),ns,(Binding)XAS3_EVENT_##ID);\
	pEventNames[XAS3_EVENT_##ID]=pClass->getSlot##NAME()

	void ShellToplevel::InitSystemEvents()
	{
		Namespacep ns=core()->getPublicNamespace(0);
		{
			ActivityEventClass*pClass=getActivityEventClass();
			_BIND_SYS(ACTIVITY,activity);
			//pSystemEvents->add(pClass->getSlotACTIVITY(),ns,(Binding)XAS3_EVENT_activity);
			//pEventNames[XAS3_EVENT_activity]=pClass->getSlotACTIVITY();
		}
		{
			AsyncErrorEventClass*pClass=getAsyncErrorEventClass();
			//pSystemEvents->add(pClass->getSlotASYNC_ERROR(),ns,(Binding)XAS3_EVENT_asyncError);
			//pEventNames[XAS3_EVENT_asyncError]=pClass->getSlotASYNC_ERROR();
			_BIND_SYS(ASYNC_ERROR,asyncError);
		}
		{
			ContextMenuEventClass*pClass=getContextMenuEventClass();
			_BIND_SYS(MENU_ITEM_SELECT,menuItemSelect);
			_BIND_SYS(MENU_SELECT,menuSelect);
			//pSystemEvents->add(pClass->getSlotMENU_ITEM_SELECT(),ns,(Binding)XAS3_EVENT_menuItemSelect);
			//pSystemEvents->add(pClass->getSlotMENU_SELECT(),ns,(Binding)XAS3_EVENT_menuSelect);
		}
		{
			DataEventClass*pClass=getDataEventClass();
			_BIND_SYS(DATA,data);
			_BIND_SYS(UPLOAD_COMPLETE_DATA,uploadCompleteData);
			//pSystemEvents->add(pClass->getSlotDATA(),ns,(Binding)XAS3_EVENT_data);
			//pSystemEvents->add(pClass->getSlotUPLOAD_COMPLETE_DATA(),ns,(Binding)XAS3_EVENT_uploadCompleteData);


		}
		{
			ErrorEventClass*pClass=getErrorEventClass();
			_BIND_SYS(ERROR_FORSPACE,error);
			//pSystemEvents->add(pClass->getSlotERROR_FORSPACE(),ns,(Binding)XAS3_EVENT_error);
		}
		{
			HTTPStatusEventClass*pClass=getHTTPStatusEventClass();
			//pSystemEvents->add(pClass->getSlotHTTP_STATUS(),ns,(Binding)XAS3_EVENT_httpStatus);
			_BIND_SYS(HTTP_STATUS,httpStatus);
		}
		{
			StatusEventClass*pClass=getStatusEventClass();
			//pSystemEvents->add(pClass->getSlotSTATUS(),ns,(Binding)XAS3_EVENT_status);
			_BIND_SYS(STATUS,status);
		}
		{
			SyncEventClass*pClass=getSyncEventClass();
			//pSystemEvents->add(pClass->getSlotSYNC(),ns,(Binding)XAS3_EVENT_sync);
			_BIND_SYS(SYNC,sync);
		}
		{
			TextEventClass*pClass=getTextEventClass();
			//pSystemEvents->add(pClass->getSlotLINK(),ns,(Binding)XAS3_EVENT_link);
			//pSystemEvents->add(pClass->getSlotTEXT_INPUT(),ns,(Binding)XAS3_EVENT_textInput);
			_BIND_SYS(LINK,link);
			_BIND_SYS(TEXT_INPUT,textInput);
		}
		{
			TimerEventClass*pClass=getTimerEventClass();
			//pSystemEvents->add(pClass->getSlotTIMER(),ns,(Binding)XAS3_EVENT_timer);
			//pSystemEvents->add(pClass->getSlotTIMER_COMPLETE(),ns,(Binding)XAS3_EVENT_timerComplete);
			_BIND_SYS(TIMER,timer);
			_BIND_SYS(TIMER_COMPLETE,timerComplete);
		}
		{
			IMEEventClass*pClass=getIMEEventClass();
			//pSystemEvents->add(pClass->getSlotIME_COMPOSITION(),ns,(Binding)XAS3_EVENT_imeComposition);
			_BIND_SYS(IME_COMPOSITION,imeComposition);
		}
		{
			KeyboardEventClass*pClass=getKeyboardEventClass();
			//pSystemEvents->add(pClass->getSlotKEY_DOWN(),ns,(Binding)XAS3_EVENT_keyDown);
			//pSystemEvents->add(pClass->getSlotKEY_UP(),ns,(Binding)XAS3_EVENT_keyUp);
			_BIND_SYS(KEY_DOWN,keyDown);
			_BIND_SYS(KEY_UP,keyUp);
		}
		{
			MouseEventClass*pClass=getMouseEventClass();
			//pSystemEvents->add(pClass->getSlotCLICK(),ns,(Binding)XAS3_EVENT_click);
			//pSystemEvents->add(pClass->getSlotDOUBLE_CLICK_FORSPACE(),ns,(Binding)XAS3_EVENT_doubleClick);
			//pSystemEvents->add(pClass->getSlotMOUSE_DOWN(),ns,(Binding)XAS3_EVENT_mouseDown);
			//pSystemEvents->add(pClass->getSlotMOUSE_MOVE(),ns,(Binding)XAS3_EVENT_mouseMove);
			//pSystemEvents->add(pClass->getSlotMOUSE_OUT(),ns,(Binding)XAS3_EVENT_mouseOut);
			//pSystemEvents->add(pClass->getSlotMOUSE_OVER(),ns,(Binding)XAS3_EVENT_mouseOver);
			//pSystemEvents->add(pClass->getSlotMOUSE_UP(),ns,(Binding)XAS3_EVENT_mouseUp);
			//pSystemEvents->add(pClass->getSlotMOUSE_WHEEL(),ns,(Binding)XAS3_EVENT_mouseWheel);
			//pSystemEvents->add(pClass->getSlotROLL_OUT(),ns,(Binding)XAS3_EVENT_rollOut);
			//pSystemEvents->add(pClass->getSlotROLL_OVER(),ns,(Binding)XAS3_EVENT_rollOver);
			_BIND_SYS(CLICK,click);
			_BIND_SYS(DOUBLE_CLICK_FORSPACE,doubleClick);
			_BIND_SYS(MOUSE_DOWN,mouseDown);
			_BIND_SYS(MOUSE_OVER,mouseOver);
			_BIND_SYS(MOUSE_OUT,mouseOut);
			_BIND_SYS(MOUSE_MOVE,mouseMove);
			_BIND_SYS(MOUSE_UP,mouseUp);
			_BIND_SYS(MOUSE_WHEEL,mouseWheel);
			_BIND_SYS(ROLL_OUT,rollOut);
			_BIND_SYS(ROLL_OVER,rollOver);
		}
		{
			ProgressEventClass*pClass=getProgressEventClass();
			//pSystemEvents->add(pClass->getSlotPROGRESS(),ns,(Binding)XAS3_EVENT_progress);
			//pSystemEvents->add(pClass->getSlotSOCKET_DATA(),ns,(Binding)XAS3_EVENT_socketData);
			_BIND_SYS(PROGRESS,progress);
			_BIND_SYS(SOCKET_DATA,socketData);
		}
		{
			IOErrorEventClass*pClass=getIOErrorEventClass();
			//pSystemEvents->add(pClass->getSlotIO_ERROR(),ns,(Binding)XAS3_EVENT_ioError);
			_BIND_SYS(IO_ERROR,ioError);
		}
		{
			SecurityErrorEventClass*pClass=getSecurityErrorEventClass();
			//pSystemEvents->add(pClass->getSlotSECURITY_ERROR(),ns,(Binding)XAS3_EVENT_securityError);
			_BIND_SYS(SECURITY_ERROR,securityError);
		}
		{
			NetStatusEventClass*pClass=getNetStatusEventClass();
			//pSystemEvents->add(pClass->getSlotNET_STATUS(),ns,(Binding)XAS3_EVENT_netStatus);
			_BIND_SYS(NET_STATUS,netStatus);
		}
		{
			EventClass*pClass=getEventClass();
			//pSystemEvents->add(pClass->getSlotACTIVATE(),ns,(Binding)XAS3_EVENT_activate);
			_BIND_SYS(ACTIVATE,activate);
			//pSystemEvents->add(pClass->getSlotADDED(),ns,(Binding)XAS3_EVENT_added);
			_BIND_SYS(ADDED,added);
			//pSystemEvents->add(pClass->getSlotADDED_TO_STAGE(),ns,(Binding)XAS3_EVENT_addedToStage);
			_BIND_SYS(ADDED_TO_STAGE,addedToStage);
			//pSystemEvents->add(pClass->getSlotCANCEL(),ns,(Binding)XAS3_EVENT_cancel);
			_BIND_SYS(CANCEL,cancel);
			//pSystemEvents->add(pClass->getSlotCHANGE(),ns,(Binding)XAS3_EVENT_change);
			_BIND_SYS(CHANGE,change);
			//pSystemEvents->add(pClass->getSlotCLOSE(),ns,(Binding)XAS3_EVENT_close);
			_BIND_SYS(CLOSE,close);
			//pSystemEvents->add(pClass->getSlotCOMPLETE(),ns,(Binding)XAS3_EVENT_complete);
			_BIND_SYS(COMPLETE,complete);
			//pSystemEvents->add(pClass->getSlotCONNECT(),ns,(Binding)XAS3_EVENT_connect);
			_BIND_SYS(CONNECT,connect);
			//pSystemEvents->add(pClass->getSlotDEACTIVATE(),ns,(Binding)XAS3_EVENT_deactivate);
			_BIND_SYS(DEACTIVATE,deactivate);
			//pSystemEvents->add(pClass->getSlotENTER_FRAME(),ns,(Binding)XAS3_EVENT_enterFrame);
			_BIND_SYS(ENTER_FRAME,enterFrame);
//#if (__CORE_VERSION__>=0x02076000)
			_BIND_SYS(EXIT_FRAME,exitFrame);
//#endif
			//pSystemEvents->add(pClass->getSlotFULLSCREEN(),ns,(Binding)XAS3_EVENT_fullscreen);
			_BIND_SYS(FULLSCREEN,fullscreen);
			//pSystemEvents->add(pClass->getSlotID3(),ns,(Binding)XAS3_EVENT_id3);
			_BIND_SYS(ID3,id3);
			//pSystemEvents->add(pClass->getSlotINIT(),ns,(Binding)XAS3_EVENT_init);
			_BIND_SYS(INIT,init);
			//pSystemEvents->add(pClass->getSlotMOUSE_LEAVE(),ns,(Binding)XAS3_EVENT_mouseLeave);
			_BIND_SYS(MOUSE_LEAVE,mouseLeave);
			//pSystemEvents->add(pClass->getSlotOPEN(),ns,(Binding)XAS3_EVENT_open);
			_BIND_SYS(OPEN,open);
			//pSystemEvents->add(pClass->getSlotREMOVED(),ns,(Binding)XAS3_EVENT_removed);
			_BIND_SYS(REMOVED,removed);
			//pSystemEvents->add(pClass->getSlotREMOVED_FROM_STAGE(),ns,(Binding)XAS3_EVENT_removedFromStage);
			_BIND_SYS(REMOVED_FROM_STAGE,removedFromStage);
			//pSystemEvents->add(pClass->getSlotRENDER(),ns,(Binding)XAS3_EVENT_render);
			_BIND_SYS(RENDER,render);
			//pSystemEvents->add(pClass->getSlotRESIZE(),ns,(Binding)XAS3_EVENT_resize);
			_BIND_SYS(RESIZE,resize);
			//pSystemEvents->add(pClass->getSlotSCROLL(),ns,(Binding)XAS3_EVENT_scroll);
			_BIND_SYS(SCROLL,scroll);
			//pSystemEvents->add(pClass->getSlotSELECT(),ns,(Binding)XAS3_EVENT_select);
			_BIND_SYS(SELECT,select);
			//pSystemEvents->add(pClass->getSlotSOUND_COMPLETE(),ns,(Binding)XAS3_EVENT_soundComplete);
			_BIND_SYS(SOUND_COMPLETE,soundComplete);
			//pSystemEvents->add(pClass->getSlotTAB_CHILDREN_CHANGE(),ns,(Binding)XAS3_EVENT_tabChildrenChange);
			_BIND_SYS(TAB_CHILDREN_CHANGE,tabChildrenChange);
			//pSystemEvents->add(pClass->getSlotTAB_ENABLED_CHANGE(),ns,(Binding)XAS3_EVENT_tabEnabledChange);
			_BIND_SYS(TAB_ENABLED_CHANGE,tabEnabledChange);
			//pSystemEvents->add(pClass->getSlotTAB_INDEX_CHANGE(),ns,(Binding)XAS3_EVENT_tabIndexChange);
			_BIND_SYS(TAB_INDEX_CHANGE,tabIndexChange);
			//pSystemEvents->add(pClass->getSlotUNLOAD(),ns,(Binding)XAS3_EVENT_unload);
		}
		{
			FocusEventClass*pClass=getFocusEventClass();
			//pSystemEvents->add(pClass->getSlotFOCUS_IN(),ns,(Binding)XAS3_EVENT_focusIn);
			//pSystemEvents->add(pClass->getSlotFOCUS_OUT(),ns,(Binding)XAS3_EVENT_focusOut);
			//pSystemEvents->add(pClass->getSlotKEY_FOCUS_CHANGE(),ns,(Binding)XAS3_EVENT_keyFocusChange);
			//pSystemEvents->add(pClass->getSlotMOUSE_FOCUS_CHANGE(),ns,(Binding)XAS3_EVENT_mouseFocusChange);
			_BIND_SYS(FOCUS_IN,focusIn);
			_BIND_SYS(FOCUS_OUT,focusOut);
			_BIND_SYS(KEY_FOCUS_CHANGE,keyFocusChange);
			_BIND_SYS(MOUSE_FOCUS_CHANGE,mouseFocusChange);
		}

	}
	int ShellToplevel::MapSysEvent(Stringp strName)
	{
		Namespacep ns=core()->getPublicNamespace(0);
		Binding b=pSystemEvents->get(strName,ns);
		if(b==BIND_NONE) return 0;
		return (int)b;
	}

	ShellCore::ShellCore(XSWFPlayer*player,MMgc::GC* gc)
		: AvmCore(gc)
		, m_pPlayer(player)
//#if (__CORE_VERSION__>=0x02070200)
		, postAlloc(sizeof(POSTCHAIN),128)
		, m_pPostChain(NULL)
//#endif
		//, eventAlloc(sizeof(POSTEVENT),256)
		//, m_bLazy(false)
		//, lazyPools(NULL)
		//, lazyTail(NULL)
		//, m_pContextStack(NULL)
		//, m_pContextList(NULL)
	{
//	poolObject = NULL;
		//m_stopEvent.Create();
		m_pShellContext=NULL;
		//m_pPostEvents=NULL;
		//m_pLastEvents=NULL;
		//m_pJavaContext=NULL;
		shell_domain=NULL;
		shell_domainEnv=NULL;
		gracePeriod = false;
		inStackOverflow = false;
		java_domain=NULL;
		java_domainEnv=NULL;
		m_pJavaContext=NULL;

		kCallProperty=internConstantStringLatin1("callProperty");
		kDeleteProperty=internConstantStringLatin1("deleteProperty");
		kGetDescendants=internConstantStringLatin1("getDescendants");
		kGetProperty=internConstantStringLatin1("getProperty");
		kHasProperty=internConstantStringLatin1("hasProperty");
		kIsAttribute=internConstantStringLatin1("isAttribute");
		kNextName=internConstantStringLatin1("nextName");
		kNextNameIndex=internConstantStringLatin1("nextNameIndex");
		kNextValue=internConstantStringLatin1("nextValue");
		kSetProperty=internConstantStringLatin1("setProperty");
		kFlashProxy=internConstantStringLatin1("http://www.adobe.com/2006/actionscript/flash/proxy");
		kAS3=internConstantStringLatin1("http://www.adobe.com/2006/actionscript/flash/AS3");
		kObjectProxy=internConstantStringLatin1("http://www.adobe.com/2006/actionscript/flash/object");
		kProtoHttp=internConstantStringLatin1("HTTP");
		kProtoHttps=internConstantStringLatin1("HTTPS");
		kProtoNone=internConstantStringLatin1("none");
		kProtoConnect=internConstantStringLatin1("CONNECT");
		kWriteExternal=internConstantStringLatin1("writeExternal");
		kReadExternal=internConstantStringLatin1("readExternal");
		kAMFContent=internConstantStringLatin1("application/x-amf");
		kOnResult=internConstantStringLatin1("onResult");
		kApplication=internConstantStringLatin1("mx.core.Application");
		kJQNonamed=internConstantStringLatin1("_JQNonamedFunction=");

		kSpaceString=internConstantStringLatin1(" ");
		kEqualString=internConstantStringLatin1("=");
		kQuotString=internConstantStringLatin1("\"");
		kSpString=internConstantStringLatin1(",");
		kLeftQ=internConstantStringLatin1("[");
		kRightQ=internConstantStringLatin1("]");
		kDrawNow=internConstantStringLatin1("DrawNow");
		kHookUIInit=internConstantStringLatin1("hookUIInit");
		kCreateAccessibilityImplementation=
			internConstantStringLatin1("createAccessibilityImplementation");
		kLocalHost=internConstantStringLatin1("localhost");
		kScript=internConstantStringLatin1("script");
		kJavaScriptPackage=internConstantStringLatin1("import JavaScript.top\r\nimport JavaScript.window\r\n");//package JavaScript{\r\n");
		allowDebugger = -1;	// aka "not yet set"

		//dsx new adds
		kEvenOddStr = internConstantStringLatin1("evenOdd");
		kNoneStr = internConstantStringLatin1("none");
		kEulerAnglesStr = internConstantStringLatin1("eulerAngles");
		kSortingStr = internConstantStringLatin1("sorting");
		kLongStr = internConstantStringLatin1("long");
		kFullStr = internConstantStringLatin1("full");
		kStandaloneStr = internConstantStringLatin1("standalone");
		kUserinterfaceStr = internConstantStringLatin1("userinterface");
		kBRStr = internConstantStringLatin1("BR");
		kStandardStr = internConstantStringLatin1("standard");
		kDefaultStr = internConstantStringLatin1("default");
		kFloatStr = internConstantStringLatin1("float");
		kXMethodDict=internConstantStringLatin1("~x~MDICT~");
		kCreateModalWindowRequest=internConstantStringLatin1("createModalWindowRequest");
		//dsx new adds end
//#if (__CORE_VERSION__>=0x02070100)
		kUri=internConstantStringLatin1("uri");
		kLocalName=internConstantStringLatin1("localName");
//#endif

		consoleOutputStream = new (gc) ConsoleOutputStream(player);

		setConsoleStream(consoleOutputStream);

		setAPIInfo(_min_version_num,
				   _max_version_num-_min_version_num+1,
				   _uris_count,
				   (const char**) _uris,
				   (int32_t*) _api_compat);

	}


	/*bool ShellCore::PostEvent(class EventObject*pEvent,bool bMask)
	{

		bool r=false;
		if(pEvent->nEventID>0)
		{
			POSTEVENT*pe=m_pPostEvents;
			while(pe)
			{
				POSTEVENT*pNext=pe->pNext;
				EventDispatcherObject*p=pe->pObject;//(EventDispatcherObject*)hash->valueAt(i);
				if(p->IsEnter())
				{
					pEvent->pTarget=p;
					pEvent->bStoped=false;
					pEvent->bStopedLater=false;
					p->DispatchEvent(pEvent);
				}
				else
					p->SetEnter(true);
				pe=pNext;
			}
			
		}
		return r;
	}*/

	void ShellCore::Release()
	{
		if(consoleOutputStream)
			((ConsoleOutputStream*)consoleOutputStream)->m_output.RemoveAll(XTRUE);
//#if (__CORE_VERSION__>=0x02070200)
		POSTCHAIN*pChain=m_pPostChain;
		while(pChain)
		{
			POSTCHAIN*pNext=pChain->pNext;
			postAlloc.Free(pChain);
			pChain=pNext;
		}
		m_pPostChain=NULL;
//#endif
	}

	void ShellCore::stackOverflow(Toplevel* toplevel)
	{
		if (inStackOverflow)
		{
			// Already handling a stack overflow, so do not
			// re-enter handler.
			return;
		}

		// Temporarily disable stack overflow checks
		// so that we can construct an exception object.
		// There should be plenty of margin before the
		// actual stack bottom to do this.
		inStackOverflow = true;

		Stringp errorMessage = getErrorMessage(kStackOverflowError);
		Atom args[2] = { nullObjectAtom, errorMessage->atom() };
		Atom errorAtom = toplevel->errorClass()->construct(1, args);
		Exception *exception = new (GetGC()) Exception(this, errorAtom);

		// Restore stack overflow checks
		inStackOverflow = false;

		// Throw the stack overflow exception
		throwException(exception);
	}

	/* static */
	/*void ShellCore::interruptTimerCallback(void* data)
	{
		((AvmCore*)data)->raiseInterrupt(ScriptTimeout);
	}*/

	void ShellCore::interrupt(Toplevel *toplevel, InterruptReason)
	{
		//this->m_pPlayer->Trace(XString16("---->Inter"));
		if (gracePeriod) {
			// This script has already had its chance; it violated
			// the grace period.
			// Throw an exception it cannot catch.
#ifdef FEATURE_NANOJIT
			m_bBreakOutTime=false;
#endif
			Stringp errorMessage = getErrorMessage(kScriptTerminatedError);
			Atom args[2] = { nullObjectAtom, errorMessage->atom() };
			Atom errorAtom = toplevel->errorClass()->construct(1, args);
			Exception *exception = new (GetGC()) Exception(this, errorAtom);
			exception->flags |= Exception::EXIT_EXCEPTION;
			throwException(exception);
		}

		// Give the script an additional grace period to
		// clean up, and throw an exception.
		gracePeriod = true;

		//Platform::GetInstance()->setTimer(kScriptGracePeriod, interruptTimerCallback, this);

		toplevel->throwError(kScriptTimeoutError);
	}

	//extern void avmplus::NativeID::TestA();

	void ShellCore::initShellPool()
	{

#ifdef VMCFG_AOT
        NativeInitializer shellNInit(this,
			&shell_toplevel_aotInfo,
            avmplus::NativeID::shell_toplevel_abc_method_count,
            avmplus::NativeID::shell_toplevel_abc_class_count);
        shellNInit.fillInClasses(avmplus::NativeID::shell_toplevel_classEntries);
        shellNInit.fillInMethods(avmplus::NativeID::shell_toplevel_methodEntries);
        shellPool = shellNInit.parseBuiltinABC(builtinDomain);
#else
		//shellPool = AVM_INIT_BUILTIN_ABC(shell_toplevel, this);
		shellPool = AVM_INIT_BUILTIN_ABC(TotalInclude, this);
#endif
//#if (__CORE_VERSION__>=0x02070200)
		XDOMFILTER*pFilter=m_pPlayer->m_pDomFilter;
		if(pFilter&&pFilter->bExternAPI)
			externPool = AVM_INIT_BUILTIN_ABC(XExternAPI,this);
		else
			externPool = NULL;
//#endif
	}

	Toplevel* ShellCore::initShellBuiltins()
	{
		// Initialize a new Toplevel.  This will also create a new
		// DomainEnv based on the builtinDomain.
		Toplevel* toplevel = initTopLevel();

		// Initialize the shell builtins in the new Toplevel
		handleActionPool(shellPool,
						 toplevel->domainEnv(),
						 toplevel,
						 NULL);
		shellPool->m_bExeced=true;
//#if (__CORE_VERSION__>=0x02070200)
		if(externPool!=NULL)
		{
			handleActionPool(externPool,toplevel->domainEnv(),toplevel,NULL);
		}
//#endif
		return toplevel;
	}

	Toplevel* ShellCore::createToplevel(AbcEnv* abcEnv)
	{
		ShellToplevel*p=new (GetGC()) ShellToplevel(abcEnv);
		return p;
	}

#ifdef VMCFG_EVAL

	// FIXME, this is currently hokey for several reasons:
	//
	//  - Does not try to determine whether input is Latin1, UTF8, or indeed, already UTF16,
	//    but assumes UTF8, which can be dangerous.  Falls back to latin1 if the utf8 conversion
	//    fails, this seems ill-defined in the string layer though so it's just one more hack.
	//
	//  - Does not create an UTF16 string.  The string layer is actually broken on this count,
	//    because requesting an empty UTF16 string returns a constant that is a Latin1 string,
	//    and appending to it won't force the representation to UTF16 unless the data require
	//    that to happen.  See <URL:https://bugzilla.mozilla.org/show_bug.cgi?id=473995>.
	//
	//  - May incur copying because the terminating NUL is not accounted for in the original
	//    creation

	String* ShellCore::decodeBytesAsUTF16String(uint8_t* bytes, uint32_t nbytes, bool terminate)
	{
		String* s = newStringUTF8((const char*)bytes, nbytes);
		if (s == NULL)
			s = newStringLatin1((const char*)bytes, nbytes);
		if (terminate)
			s = s->appendLatin1("\0", 1);
		return s;
	}

	Atom	  ShellCore::DoJavaScript(Stringp name,ArrayObject*args)
	{
		int argc=args?args->getDenseLength():0;//get_length():0;
		MMgc::GC::AllocaAutoPtr _atomv;
		Atom* atomv = (Atom*)VMPI_alloca(this, _atomv, sizeof(Atom)*(argc+1));
		
		if(args)
		 {
			VMPI_memcpy(atomv+1, args->GetAtoms(), sizeof(Atom)*argc);
		 }
		TRY(this, kCatchAction_ReportAsError)
		{
			
			if(m_pJavaContext)
			{
				ApplicationDomainObject*domain=m_pJavaContext->m_pAppDomain;
				Atom atom=domain->FindDefinition(name);
				atomv[0] = atom;
				if(atom!=kUnusedAtomTag)
				{
					if(atomKind(atom)==kObjectType&&!isNullOrUndefined(atom))
					{
						 if (istype(atom, traits.class_itraits)) //class
						 {
							ClassClosure*pClass=(ClassClosure*)atomToScriptObject(atom);
							return pClass->construct(argc,atomv);
						 }
						 else if(istype(atom,traits.function_itraits))//function
						 {
							FunctionObject*pFun=(FunctionObject*)atomToScriptObject(atom);
							return pFun->call(argc,atomv);
						 }
					}
					else return atom;
				}
			}
		    
			name=String::concatStrings(this->kJQNonamed,name);			
			//return 
			Atom atom=this->evaluateString(name);
			if(atomKind(atom)==kObjectType&&!isNullOrUndefined(atom))
			{				
				 atomv[0]=atom;
				 if (istype(atom, traits.class_itraits)) //class
				 {
					ClassClosure*pClass=(ClassClosure*)atomToScriptObject(atom);
					atom= pClass->construct(argc,atomv);
				 }
				 else if(istype(atom,traits.function_itraits))//function
				 {
					FunctionObject*pFun=(FunctionObject*)atomToScriptObject(atom);
					atom= pFun->call(argc,atomv);
				 }
			}
			return atom;
		}
		CATCH(Exception *exception)
		{
	#ifdef DEBUGGER
				if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
				{
					console << string(exception->atom) << "\n";
				}
				if (exception->getStackTrace()) {
					console << exception->getStackTrace()->format(this) << '\n';
				}
	#else
				console << string(exception->atom) << "\n";
	#endif		
				return undefinedAtom;
		}
		END_CATCH
		END_TRY
		return undefinedAtom;
	}
	
	// input is always NUL-terminated
	Atom ShellCore::evaluateString(String* input, bool record_time)
	{
		setStackLimit();

		//for Bugs
		//ShellCodeContext* m_pJavaContext = new (GetGC()) ShellCodeContext(this,(ScriptObject*)((ShellToplevel*)shell_toplevel)->m_pDumpObject.value(),GetGC());
		//m_pJavaContext->m_domainEnv = shell_domainEnv;
		//m_pJavaContext->m_bAccessiable=true;

		if(m_pJavaContext==NULL)
		{
			java_domain = new (GetGC()) Domain(this, shell_domain);

			// Return a new DomainEnv for the user code
			java_domainEnv = new (GetGC()) DomainEnv(this, java_domain, shell_domainEnv);

			m_pJavaContext =new (GetGC()) ShellCodeContext(this,shell_toplevel->objectClass,GetGC());
			SaveContext(m_pJavaContext);
			m_pJavaContext->m_bAccessiable=true;
			m_pJavaContext->m_domain=java_domain;
			m_pJavaContext->m_domainEnv=java_domainEnv;
			m_pJavaContext->m_pRoot=m_pPlayer->player;
			m_pJavaContext->m_pAppDomain=((ShellToplevel*)shell_toplevel)->getApplicationDomainClass()->
				CreateAppDomain(NULL,m_pJavaContext,shell_toplevel);
			m_pJavaContext->m_pSecurityDomain=
				(SecurityDomainObject*)
				((ShellToplevel*)shell_toplevel)->getSecurityDomainClass()->newInstance();
		}
		//ShellCodeContext* codeContext = new (GetGC()) ShellCodeContext(this,(ScriptObject*)((ShellToplevel*)shell_toplevel)->m_pDumpObject.value(),GetGC());
		//codeContext->m_domainEnv = shell_domainEnv;
		//codeContext->m_bAccessiable=true;

		TRY(this, kCatchAction_ReportAsError)
		{
			// Always Latin-1 here
			input = input->appendLatin1("\0", 1);
			double then = 0, now = 0;
			if (record_time)
				then = VMPI_getDate();
			uint32_t api = this->getAPI(NULL);
			Atom result = handleActionSource(input, NULL, m_pJavaContext->m_domainEnv, (Toplevel*&)shell_toplevel, NULL, m_pJavaContext, api);
			if (record_time)
				now = VMPI_getDate();
			return result;
#ifdef DEBUGGER
			if (result != undefinedAtom)
				console << string(result) << "\n";
			if (record_time)
				console << "Elapsed time: " << (now - then)/1000 << "s\n";
#endif
		}
		CATCH(Exception *exception)
		{
#ifdef DEBUGGER
			if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
			{
				console << string(exception->atom) << "\n";
			}
			if (exception->getStackTrace()) {
				console << exception->getStackTrace()->format(this) << '\n';
			}
#else
			console << string(exception->atom) << "\n";
#endif
			return undefinedAtom;
		}
		END_CATCH
		END_TRY
		return undefinedAtom;
	}

#endif // VMCFG_EVAL

#ifdef AVMSHELL_PROJECTOR_SUPPORT


#endif // AVMSHELL_PROJECTOR_SUPPORT

#ifdef AVMPLUS_SELFTEST
	void ShellCore::executeSelftest(ShellCoreSettings& settings)
	{
		setStackLimit();
		::selftests(this, settings.st_component, settings.st_category, settings.st_name);
	}
#endif

	bool ShellCore::setup(ShellCoreSettings& settings)
	{
#if defined FEATURE_NANOJIT && (defined (AVMPLUS_IA32) || defined(AVMPLUS_AMD64))
	#ifdef AVMPLUS_SSE2_ALWAYS
		config.sse2 = true;
	#else
		if (!P4Available())
			config.sse2 = false;
	#endif
#endif

		// set the default api version
		if (settings.api <= _max_version_num) {
			this->defaultAPIVersion = settings.api;
		}
		else {
			// if there is at least on versioned uri, then there must be a version matrix
			if (_uris_count > 0) {
				// last api of any row is largestApiUtils::getLargestVersion(this);
				this->defaultAPIVersion = ((uint32_t*)_versions)[_versions_count[0]-1];
			}
			else {
				this->defaultAPIVersion = 0;
			}
		}
		//console << "defaultAPIVersion=" << defaultAPIVersion;
		this->setActiveAPI(ApiUtils::toAPI(this, this->defaultAPIVersion));

		config.interrupts = settings.interrupts;
#ifdef AVMPLUS_VERIFYALL
		config.verifyall = settings.verifyall;
#endif
#if defined FEATURE_NANOJIT
		config.cseopt = settings.cseopt;
		config.jitordie = settings.jitordie;
	#if defined (AVMPLUS_IA32) || defined(AVMPLUS_AMD64)
		config.sse2 = settings.sse2;
        config.fixed_esp = settings.fixed_esp;
	#endif
#endif

#ifdef AVMPLUS_VERBOSE
        if (settings.do_verbose & VB_builtins)
            config.verbose_vb = settings.do_verbose;  // ~builtins then skip verbose settings during setup()
#endif
		config.runmode = settings.runmode;

#ifdef VMCFG_METHOD_NAMES
		// verbose requires methodnames (in avmshell, anyway), before calling initBuiltinPool.
		if (settings.do_verbose)
			config.methodNames = true;
    #ifdef DEBUGGER
		// debugger in avmshell always enables methodnames.
		if (allowDebugger)
			config.methodNames = true;
	#endif
#endif // VMCFG_METHOD_NAMES

#ifdef DEBUGGER
		langID = settings.langID;
#endif

		TRY(this, kCatchAction_ReportAsError)
		{
			setStackLimit();

			allowDebugger = !settings.nodebugger;

			setCacheSizes(settings.cacheSizes);

//			SystemClass::user_argc = settings.numargs;
//			SystemClass::user_argv = settings.arguments;

#ifdef DEBUGGER
			initBuiltinPool((avmplus::Debugger::TraceLevel)settings.astrace_console);
#else
			initBuiltinPool();
#endif
			initShellPool();

			// init toplevel internally
			shell_toplevel = (ShellToplevel*)initShellBuiltins();

			// Create a new Domain for the user code
			shell_domain = new (GetGC()) Domain(this, builtinDomain);

			// Return a new DomainEnv for the user code
			shell_domainEnv = new (GetGC()) DomainEnv(this, shell_domain, shell_toplevel->domainEnv());

			m_pShellContext =new (GetGC()) ShellCodeContext(this,shell_toplevel->objectClass,GetGC());
			SaveContext(m_pShellContext);
			m_pShellContext->m_bAccessiable=true;
			m_pShellContext->m_domain=shell_domain;
			m_pShellContext->m_domainEnv=shell_domainEnv;
			m_pShellContext->m_pRoot=NULL;
			m_pShellContext->m_pAppDomain=((ShellToplevel*)shell_toplevel)->getApplicationDomainClass()->
				CreateAppDomain(NULL,m_pShellContext,shell_toplevel);
			m_pShellContext->m_pSecurityDomain=
				(SecurityDomainObject*)
				((ShellToplevel*)shell_toplevel)->getSecurityDomainClass()->newInstance();

#ifdef AVMPLUS_VERBOSE
            config.verbose_vb = settings.do_verbose;  // builtins is done, so propagate verbose
#endif
			((ShellToplevel*)shell_toplevel)->InitSystemEvents();

			{
				DisplayObjectClass* pObj=((ShellToplevel*)shell_toplevel)->getDisplayObjectClass();
				TraitsBindingsp t = pObj->traits()->getTraitsBindings();
				Binding b = t->findBinding(kHookUIInit);
				if(b!=BIND_NONE)
				{
					//this->hookUIFunction=(FunctionObject*)pObj->getSlotObject(AvmCore::bindingToSlotId(b));
					MethodEnv *f = pObj->vtable->methods[AvmCore::bindingToMethodId(b)];
					this->hookUIFunction=(FunctionObject*)
							AvmCore::atomToScriptObject(f->coerceEnter(pObj->atom()));
				}
				//MethodEnv *f = vtable->methods[AvmCore::bindingToMethodId(b)];
				//return (ClassClosure*)AvmCore::atomToScriptObject(f->coerceEnter(this->atom()));
			}

			return true;
		}
		CATCH(Exception *exception)
		{
#ifdef DEBUGGER
			if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
				console << string(exception->atom) << "\n";

			if (exception->getStackTrace())
				console << exception->getStackTrace()->format(this) << '\n';
#else
			// [ed] always show error, even in release mode,
			// see bug #121382
			console << string(exception->atom) << "\n";
#endif /* DEBUGGER */
			return false;
		}
		END_CATCH
		END_TRY
	}
	bool ShellCore::InitClassClosure(ClassClosure*pObj)
	{
		TraitsBindingsp t = pObj->traits()->getTraitsBindings();
		Binding b = t->findBinding(kCreateAccessibilityImplementation);
		if(b!=BIND_NONE)
		{
			uint32_t id=AvmCore::bindingToSlotId(b);
			if(pObj->getSlotObject(id)==NULL)
				pObj->coerceAndSetSlotAtom(id,this->hookUIFunction->atom());
			//this->hookUIFunction=(FunctionObject*)pObj->getSlotObject(AvmCore::bindingToSlotId(b));
			//MethodEnv *f = pObj->vtable->methods[AvmCore::bindingToMethodId(b)];
			//this->hookUIFunction=(FunctionObject*)
			//		AvmCore::atomToScriptObject(f->coerceEnter(pObj->atom()));
		}
		return true;
	}

	void ShellCore::doLazyPools(const Multiname&name,ShellCodeContext*codeContext)
	{
		//ScriptEnv*env;

		/*if(!codeContext->m_bLazy) return;
		LivePoolNode*node=this->livePools;
		codeContext->m_bLazy=false;
		//PoolObject*poolExec=NULL;
		ShellCodeContext*execCode=codeContext;
		//XArray<PoolObject*> pools;
		while(node)
		{
			PoolObject*pool=(PoolObject*)node->pool->get();
			if(!pool->m_bExeced&&pool->codeContext==codeContext&&pool->mainEnv)//&&((ShellCodeContext*)pool->codeContext)->m_pAppDomain==codeContext->m_pAppDomain)
			{
				//pool->m_bExeced=true;
				//DomainEnv*domainEnv=pool->codeContext->domainEnv();
				//ScriptEnv*pEnv=domainEnv->getNamedScript(name.getName(),name.getNamespace());
				//handleActionPool(pool, domainEnv, shell_toplevel, execCode);
				//if(pEnv)
				//	int v=0;
				//pools.Add(pool);
				//if(pEnv)
				//if(pEnv)
				{
					//pEnv->coerceEnter(pEnv->global->atom());
					ScriptEnv*main=pool->mainEnv;
					main->coerceEnter(main->global->atom());
					pool->mainEnv=NULL;
					//return;
				}
				//break;
				//poolExec=pool;

			}
			pool->m_bExeced=true;
			node=node->next;
		}*/
		/*int i,nc=pools.GetSize();
		if(nc)
		{

			for(i=nc-1;i>=0;i--)
			{
				DomainEnv*domainEnv=((ShellCodeContext*)pools[i]->codeContext)->domainEnv();
				handleActionPool(pools[i], domainEnv, shell_toplevel, execCode);
			}
		}*/
		/*if(poolExec)
		{
			codeContext->m_bLazy=false;
			DomainEnv*domainEnv=((ShellCodeContext*)poolExec->codeContext)->domainEnv();
			handleActionPool(poolExec, domainEnv, shell_toplevel, execCode);
		}*/
		/*LazyPoolNode**root=&lazyPools;
		while(*root)
		{
			PoolObject*pool=(*root)->pool;
			if(1)//pool->getNamedScript()
			{

				LazyPoolNode*node=*root;
				DomainEnv*domainEnv=node->codeContext->domainEnv();
				//ScriptEnv*pEnv=domainEnv->getNamedScript(name.getName(),name.getNamespace());
				handleActionPool(node->pool, domainEnv, shell_toplevel, node->codeContext);
				*root=(*root)->next;
			}
			else
				root=&(*root)->next;
			//Domain* domain = domainEnv ? domainEnv->domain() : builtinDomain;
			//DomainEnv*domainEnv=lazyPools->codeContext->domainEnv();
			//handleActionPool(lazyPools->pool, domainEnv, shell_toplevel, lazyPools->codeContext);
			//lazyPools=lazyPools->next;

		}*/
		//lazyPools=lazyTail=NULL;
	}

	PoolObject* ShellCore::handleABC(const void*pCode,int nLength,int nFlags,ShellCodeContext*codeContext)
	{
		setStackLimit();

		//PoolObject* poolObj=NULL;

		TRY(this, kCatchAction_ReportAsError)
		{
			MMgc::GC *gc = shell_toplevel->gc();
			const int kDoAbcLazyInitializeFlag = 1;

			//MMGC_GCENTER(gc);

			//ShellCodeContext* codeContext = new (GetGC()) ShellCodeContext();
			//codeContext->m_domainEnv = shell_domainEnv;


			ReadOnlyScriptBufferImpl* abcbuf = new (gc) ReadOnlyScriptBufferImpl((const byte*)pCode, nLength);
			ScriptBuffer code(abcbuf);

		// FIXME get this from the SWF
#ifdef VMCFG_AOT
            if (filename == NULL) {
				handleAOT(this, shell_domain, shell_domainEnv, shell_toplevel, codeContext);
			} else
#endif


			uint32_t api = getAPI(NULL);

			if ( (nFlags & kDoAbcLazyInitializeFlag))
			{
				DomainEnv*domainEnv=codeContext->m_domainEnv;
				PoolObject* poolObj=parseActionBlock(code, 0, shell_toplevel, domainEnv->domain(), NULL, api);

				ScriptEnv* main = prepareActionPool(poolObj, domainEnv, (Toplevel*&)shell_toplevel, codeContext);

				//main->initGlobal();
				poolObj->codeContext=codeContext;
				//ScriptEnv*main=this->prepareActionPool(
				poolObj->mainEnv=main;
				poolObj->m_bExeced=false;
				codeContext->m_bLazy=true;
				//this->addLazyPool((avmplus::PoolObject*)poolObj,codeContext);
				//handleActionPool(
				return poolObj;
			} else
			{
				//poolObj=

				//poolObj=(PoolObject*)
				//handleActionBlock(code, 0, codeContext->m_domainEnv, (Toplevel*&)shell_toplevel, NULL, codeContext, api);
				//poolObj->m_bExeced=true;
				DomainEnv*domainEnv=codeContext->m_domainEnv;
				Domain* domain = domainEnv->domain();// ? domainEnv->domain() : builtinDomain;
		
				// parse constants and attributes.
				PoolObject *pool = parseActionBlock(code,
										0,
										shell_toplevel,
										domain,
										NULL,
										api);
				pool->m_bExeced=true;
				pool->codeContext=codeContext;
				//return 
				handleActionPool(pool, domainEnv, shell_toplevel, codeContext);
				return pool;
			}
			//poolObj->codeContext=codeContext;




		}
		CATCH(Exception *exception)
		{
#ifdef DEBUGGER
			if (!(exception->flags & Exception::SEEN_BY_DEBUGGER))
			{
				console << string(exception->atom) << "\n";
			}
			if (exception->getStackTrace()) {
				console << exception->getStackTrace()->format(this) << '\n';
			}
#else
			// [ed] always show error, even in release mode,
			// see bug #121382
			console << string(exception->atom) << "\n";
#endif /* DEBUGGER */

			return NULL;
		}
		END_CATCH
		END_TRY

		/*if(poolObj)
		{
			avmplus::PoolObject*obj=(avmplus::PoolObject*)poolObj;
			obj->nextObject=(avmplus::PoolObject*)poolObject;
			poolObject=poolObj;
		}*/
		return NULL;
		//return poolObj;
	}


	bool ShellCore::MultinameOf(Stringp name,Multiname&multiname,AvmCore*core)
	{
		if(name==NULL) 
			return false;
		int ddot = 0;
		if ( name->lastIndexOf(core->cachedChars[(int)'<']) > 0 )
		{
			Stringp strTmp = name->substring( 0, name->lastIndexOf(core->cachedChars[(int)'<']) - 1 );
			ddot = strTmp->lastIndexOf(core->cachedChars[(int)':']);
		}
		else
		{
			ddot= name->lastIndexOf(core->cachedChars[(int)':']);
		}

		Namespace* ns;
		Stringp className;
		if(ddot>0&&ddot<name->length()-1)
		{
			Stringp uri= core->internString(name->substring(0,ddot-1));
			ns = core->internNamespace(core->newNamespace(uri, Namespace::NS_Public, core->getAPI(NULL)));
			className = core->internString(name->substring(ddot+1, name->length()));
		}
		else
		{
			int dot = name->lastIndexOf(core->cachedChars[(int)'.']);

			// If there is a '.', this is a fully-qualified
			// class name in a package.  Must turn it into
			// a namespace-qualified multiname.

			if (dot >= 0) {
				Stringp uri = core->internString(name->substring(0, dot));
				ns = core->internNamespace(core->newNamespace(uri, Namespace::NS_Public, core->getAPI(NULL)));
				className = core->internString(name->substring(dot+1, name->length()));
			} else {
				ns = core->findPublicNamespace();
				className = core->internString(name);
			}
		}
		multiname.setName(className);
		multiname.setNamespace(ns);
		return true;
	}

	ScriptObject* ShellCore::FindClass(Stringp name,ShellCodeContext*env,bool bMust)
	{

		AvmCore *core = this;

		if (name == NULL) {
			shell_toplevel->throwArgumentError(kNullArgumentError, "null");//core->toErrorString("name"));
		}


		// Search for a dot from the end.
		/*int ddot= name->lastIndexOf(core->cachedChars[(int)':']);
		Namespace* ns;
		Stringp className;
		if(ddot>0&&ddot<name->length()-1)
		{
			Stringp uri= core->internString(name->substring(0,ddot-1));
			ns = core->internNamespace(core->newNamespace(uri, Namespace::NS_Public, core->getAPI(NULL)));
			className = core->internString(name->substring(ddot+1, name->length()));
		}
		else
		{
			int dot = name->lastIndexOf(core->cachedChars[(int)'.']);

			// If there is a '.', this is a fully-qualified
			// class name in a package.  Must turn it into
			// a namespace-qualified multiname.

			if (dot >= 0) {
				Stringp uri = core->internString(name->substring(0, dot));
				ns = core->internNamespace(core->newNamespace(uri, Namespace::NS_Public, core->getAPI(NULL)));
				className = core->internString(name->substring(dot+1, name->length()));
			} else {
				ns = core->findPublicNamespace();
				className = core->internString(name);
			}
		}

		Multiname multiname(ns, className);*/
		Multiname multiname;
		ShellCore::MultinameOf(name,multiname,core);
//#ifdef __XIEPF__
		Stringp nm = multiname.getName();
		if(nm->contains(this->newConstantStringLatin1(".<")))
		{
			StUTF8String str(nm);
			const char* pname=str.c_str();
			int iStart= strstr(pname,".<")-pname;
			int iEnd =	strrchr(pname,'>')-pname;
			if(iStart>0&&iEnd>iStart)
			{
				Stringp vecName=this->internStringUTF8(pname,iStart);
				Stringp typeName=this->internStringUTF8(pname+iStart+2,iEnd-iStart-2);
				Multiname vec(multiname);
				vec.setName(vecName);
				ScriptObject* pClass=FindClass(vec,env,bMust);
				if(pClass==XNULL) return XNULL;
				{
					Multiname type;//(multiname);
					ShellCore::MultinameOf(typeName,type,core);
					//type.setName(typeName);
					ScriptObject*pType=FindClass(type,env,bMust);
					Atom args[]={pType->atom()};
					Atom atom=pClass->applyTypeArgs(1,args);
					return this->atomToScriptObject(atom);
				}
			}
		}
//#endif
		return FindClass(multiname,env,bMust);
	}

	ScriptObject* ShellCore::FindClass(Multiname&multiname,ShellCodeContext*code,bool bMust)
	{
		Toplevel*toplevel=shell_toplevel;
		if(code==NULL)
		{
			code=m_pContextList;
			while(code)
			{
				if(code->m_bLazy)
					this->doLazyPools(multiname,code);
				ScriptEnv* script = (ScriptEnv*) (bMust?code->m_domainEnv->getScriptEntry(multiname):code->m_domainEnv->getScriptInit(multiname));
				if (script == (ScriptEnv*)BIND_AMBIGUOUS)
				{
					code=code->m_pNext;
					continue;
				}

				if (script == (ScriptEnv*)BIND_NONE)
				{
					code=code->m_pNext;
					continue;
				}

				if (script->global == NULL)
				{
					script->initGlobal();
					script->coerceEnter(script->global->atom());
				}

				ScriptObject *container = script->global;
				if (container) {
					Atom atom = shell_toplevel->getproperty(container->atom(),
													&multiname,
													container->vtable);
					if (AvmCore::istype(atom, traits.class_itraits))
						return (ClassClosure*)AvmCore::atomToScriptObject(atom);
					}
				code=code->m_pNext;
			}
		}
		else
		{

			this->doLazyPools(multiname,code);

			//ScriptObject*o=env->finddef(&name);

			ScriptEnv* script = (ScriptEnv*) (bMust?code->m_domainEnv->getScriptEntry(multiname):code->m_domainEnv->getScriptInit(multiname));
			if (script == (ScriptEnv*)BIND_AMBIGUOUS)
			{
				if(bMust)
					toplevel->throwReferenceError(kAmbiguousBindingError, multiname);
				else
					return NULL;
			}

			if (script == (ScriptEnv*)BIND_NONE)
			{
				if(bMust)
					toplevel->throwReferenceError(kUndefinedVarError, multiname);
				else
					return NULL;
			}

			if (script->global == NULL)
			{
				script->initGlobal();
				script->coerceEnter(script->global->atom());
			}

			ScriptObject *container = script->global;
			if (!container) {
				if(bMust)
					shell_toplevel->throwTypeError(kClassNotFoundError, toErrorString(&multiname));
				else
					return NULL;
			}
			Atom atom = shell_toplevel->getproperty(container->atom(),
												&multiname,
												container->vtable);

			if (!AvmCore::istype(atom, traits.class_itraits)) {
				if(bMust)
					shell_toplevel->throwTypeError(kClassNotFoundError, toErrorString(&multiname));
				else return NULL;
			}
			return (ClassClosure*)AvmCore::atomToScriptObject(atom);
		}
		return NULL;
		/*return script->global;

		//Atom global=env->findglobalproperty((Atom)shell_toplevel->global(),&name);

		if(isNullOrUndefined(global)) return NULL;

		Binding b = BIND_NONE; //getBinding(me, pGlobal->traits(), &name);
		{

			//MethodEnv*env=me;
			Multiname*ref=&name;
			Traits*traits=toplevel->toVTable(global)->traits;//pGlobal->traits();
			//Binding b = BIND_NONE;
			if (traits && ref->isBinding())
			{
				if (!traits->isResolved())
					traits->resolveSignatures(env->toplevel());

				TraitsBindingsp tb = traits->getTraitsBindings();
				if (!ref->isNsset())
				{
					b = tb->findBinding(ref->getName(), ref->getNamespace());
				}
				else
				{
					b = tb->findBinding(ref->getName(), ref->getNsset());
					if (b == BIND_AMBIGUOUS)
					{
						// ERROR.  more than one binding is available.  throw exception.
						env->toplevel()->throwTypeError(kAmbiguousBindingError, env->core()->toErrorString(ref));
					}
				}
			}
		}

		Atom obj = global;//atomv[0];
		ScriptObject*pObject=NULL;
		switch (AvmCore::bindingKind(b))
		{
			case BKIND_METHOD:
			{
				// can't invoke method as constructor
				break;
			}
			case BKIND_VAR:
			case BKIND_CONST:
			{
				ScriptObject* ctor = AvmCore::atomToScriptObject(obj)->getSlotObject(bindingToSlotId(b));
				AvmCore* core = toplevel->core();
				if (ctor&&
					(ctor->traits()->subtypeof(CLASS_TYPE) || ctor->traits()->subtypeof(FUNCTION_TYPE)))
					//toplevel->throwTypeError(kNotConstructorError, core->toErrorString(name));
				// inlined equivalent of op_construct
					pObject=ctor;
				break;

				//return ctor->construct(argc, atomv);
			}
			case BKIND_GET:
			case BKIND_GETSET:
			case BKIND_SET:
			default:
				break;
		}

		return pObject*/
	}

	ClassClosure* ShellCore::getAliasClass(Stringp name)
	{
		ShellToplevel*top=(ShellToplevel*)GetToplevel();
		InlineHashtable*hash=top->m_aliasClasses->get_ht();
		Atom atom=hash->get(name->atom());
		if(AvmCore::isNullOrUndefined(atom)) return NULL;
		return (ClassClosure*)AvmCore::atomToScriptObject(atom);
	}
	Stringp	ShellCore::getClassAliasClass(ClassClosure*cs)
	{
		ShellToplevel*top=(ShellToplevel*)GetToplevel();
		//Traits*traits=obj->traits();
		InlineHashtable*hash=top->m_classAlias->get_ht();
		Atom atom=hash->get(cs->atom());
		if(!AvmCore::isNullOrUndefined(atom)) //return NULL;
			return AvmCore::atomToString(atom);
		return NULL;
	}
	Stringp		  ShellCore::getClassAlias(ScriptObject*obj)
	{
		ShellToplevel*top=(ShellToplevel*)GetToplevel();
		Traits*traits=obj->traits();
		InlineHashtable*hash=top->m_classAlias->get_ht();
		while(traits)
		{
			if(traits->_classEntry)
			{
				//traits->itraits
				ClassClosure*cs=(ClassClosure*)traits->_classEntry;
				Atom atom=hash->get(cs->atom());
				if(!AvmCore::isNullOrUndefined(atom)) //return NULL;
					return AvmCore::atomToString(atom);

			}
			traits=traits->base;
		}
		return NULL;
	}


	ClassClosure* ShellCore::getClassByAlias(AvmMethodEnv env,Stringp alias)
	{
		ShellToplevel*top=(ShellToplevel*)((ShellCore*)env->core())->GetToplevel();
		alias=env->core()->internString(alias);
		InlineHashtable*hash=top->m_aliasClasses->get_ht();
		Atom atom=hash->get(alias->atom());
		if(AvmCore::isNullOrUndefined(atom)) return NULL;
		return (ClassClosure*)AvmCore::atomToScriptObject(atom);
	}

	void ShellCore::registerClassAlias(AvmMethodEnv env,Stringp name,ClassClosure*cs)
	{
		if ( cs == NULL )
		{
			LOGWHERE();
			return;
		}
		ShellToplevel*top=(ShellToplevel*)((ShellCore*)env->core())->GetToplevel();
		name=env->core()->internString(name);
		InlineHashtable*hash=top->m_aliasClasses->get_ht();
		Traits*traits=cs->traits()->itraits;
		traits->_classEntry=cs;
		hash->add(name->atom(),cs->atom());
		hash=top->m_classAlias->get_ht();
		hash->add(cs->atom(),name->atom());
		//ShellCore*c=(ShellCore*)env->core();
		//URLRequestClass*pClass=((ShellToplevel*)c->GetToplevel())->getURLRequestClass();
		//Namespacep ns=c->getPublicNamespace(0);
		//pClass->m_aliasTable->put(name,pClass,(Binding)cs);
		//Binding b=core->m_aliasTable->get(alias,ns);
		//if(b==BIND_NONE) return NULL;
		//return (ClassClosure*)b;
	}

	Stringp ShellCore::MMExecute(AvmMethodEnv env,Stringp)
	{
		return env->core()->kEmptyString;
	}

	void ShellCore::showRedrawRegions(AvmMethodEnv,bool on,uint32_t color)
	{

	}

	void ShellCore::nativeTrace(AvmMethodEnv env,ArrayObject*pObject)
	{
		//(XMLObject*)AvmCore::atomToScriptObject(pObject->getUintProperty(0));
		//o->IncrementRef();
		ShellCore*core=(ShellCore*)env->core();
		Stringp s=core->string(pObject->getIntProperty(0));
		for(int i=1;i<pObject->getDenseLength();i++)//get_length();i++)
		{
			s=s->concatStrings(s,core->kSpaceString);
			s=s->concatStrings(s,core->string(pObject->getIntProperty(i)));
		}
#ifdef _WINEMU
		//StUTF8String sss(s);
		//if(strstr(sss.c_str(),"completeHandler")!=NULL)
		//{
		//	int i = 0;
		//}

		StUTF16String st(s);
		//XString8 str;
		//str.ConvertFrom(st.c_str(),XCHARSET_UTF8,st.length());
		//str.ConvertToGB();
		core->GetPlayer()->Trace(st.c_str());//str.GetData());
#endif
#ifdef __ANDROID__
		StUTF8String str(s);

#endif
	}

	void ShellCore::clearInterval(AvmMethodEnv env,uint32 id)//Cancels a specified setInterval() call. flash.utils
	{
		XSWFPlayer*p=((ShellCore*)env->core())->GetPlayer();
		p->RemoveCaller(id);

	}
	void ShellCore::clearTimeout(AvmMethodEnv env,uint32 id) //Cancels a specified setTimeout() call. flash.utils
	{
		XSWFPlayer*p=((ShellCore*)env->core())->GetPlayer();
		p->RemoveCaller(id);
	}

	Stringp ShellCore::escapeMultiByte(AvmMethodEnv,ScriptObject* self,Stringp v) //Returns an escaped copy of the input string encoded as either UTF-8 or system code page, depending on the value of System.useCodePage. flash.utils
	{
		//return Toplevel::escape(self,v);
		AvmCore* core = self->core();

		if (!v) v = core->knull;

		StringBuffer buffer(core);

		//StringIndexer str_idx(in);
		StUTF8String str(v);
		const char* tmp=str.c_str();
		for (int i=0, n=str.length(); i<n; i++)
		{
			char ch = tmp[i];
			/*if (contains(unescaped, ch)) {
				buffer << ch;
			}
			else {
				buffer << '%';
				buffer.writeHexByte((uint8)ch);
			}*/
			switch(ch)
			{
				case '.':
				//case '_':
				case '!':
				case '*':
				case '(':
				case ')':
				case '\'':
				case '@':
					//str+=c;
					buffer << ch;
					break;
				default:
					if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
						buffer << ch;
						//str+=c;
					else //if()
					{
					   /*str+=XPEC;
					   XString8 strTmp;
					   strTmp.FromInt((XU8)c,XTRUE);
					   str+=strTmp.GetData()+2;*/
						buffer << '%';
						buffer.writeHexByte((uint8)ch);
					}
			}
					//str+=c;

		}

		return core->newStringUTF8(buffer.c_str());
		//return Toplevel::escape(self,v);
	}
	Atom ShellCore::getDefinitionByName(AvmMethodEnv env,ScriptObject*,Stringp value) //Returns a reference to the class object of the class specified by the name parameter. flash.utils
	{
		//StUTF8String ss(value);
		//if(strstr(ss.c_str(),"OPEN_LEVEL_NUM")!=0)
		//{
		//	int i = 0;
		//}

		ShellCore*pCore=(ShellCore*)env->core();
		ShellCodeContext*pContext=(ShellCodeContext*)pCore->codeContext();
		ScriptObject*pObj=pCore->FindClass(value,pContext,true);//->atom();
		return pObj?pObj->atom():kAvmThunkNull;
	}

	Atom ShellCore::describeTypeJSON(AvmMethodEnv env, Atom v, uint32_t flags)
	{
		TypeDescriber td(env->toplevel());
		ScriptObject* o = td.describeType(v, flags);
		return o ? o->atom() : nullObjectAtom;
	}

	Stringp ShellCore::getQualifiedClassName(AvmMethodEnv env,ScriptObject*,Atom value) //Returns the fully qualified class name of an object. flash.utils
	{
		//ScriptObject*self=env->getGlo
		TypeDescriber td(env->toplevel());
		return td.getQualifiedClassName(value);
		//return DescribeTypeClass::getQualifiedClassName(self,value);
		//return NULL;
	}
	Stringp ShellCore::getQualifiedSuperclassName(AvmMethodEnv env,ScriptObject*self,Atom value) //Returns the fully qualified class name of the base class of the object specified by the value parameter. flash.utils
	{
		TypeDescriber td(env->toplevel());
		return td.getQualifiedSuperclassName(value);
		//return DescribeTypeClass::getQualifiedSuperclassName(self,value);
		//return NULL;
	}
	int ShellCore::getTimer(AvmMethodEnv env) //Returns the number of milliseconds that have elapsed since Flash Player was initialized, and is used to compute relative time. flash.utils
	{
		XSWFPlayer*p=((ShellCore*)env->core())->GetPlayer();
//#if defined( __ANDROID__ ) && defined( __ANDROID_TEST__ )
//		return (XTime::GetTimeMSec() - p->mTimerOffset) / 3;
//#else
		return XTime::GetTimeMSec() - p->mTimerOffset;
//#endif
		//return 0;
	}
	uint32 ShellCore::nativeSetInterval(AvmMethodEnv env,FunctionObject*closure,double delay,ArrayObject*arguments)
	{
		ShellCore*pCore=(ShellCore*)env->core();
		ShellToplevel*top=(ShellToplevel*)pCore->GetToplevel();
		XSWFPlayer*p=pCore->GetPlayer();

		top->PushObject(closure);
		top->PushObject(arguments);
		//closure->IncrementRef();
		//arguments->IncrementRef();
		return p->AS3AddCaller(closure,(XU32)delay,arguments,1,pCore->codeContext());//pCore->GetCodeContext());
	}
	uint32 ShellCore::nativeSetTimeout(AvmMethodEnv env,FunctionObject*closure,double delay,ArrayObject*arguments)
	{
		ShellCore*pCore=(ShellCore*)env->core();
		ShellToplevel*top=(ShellToplevel*)pCore->GetToplevel();
		XSWFPlayer*p=pCore->GetPlayer();
		top->PushObject(closure);
		top->PushObject(arguments);
		//closure->IncrementRef();
		//arguments->IncrementRef();
		return p->AS3AddCaller(closure,(XU32)delay,arguments,2,pCore->codeContext());//pCore->GetCodeContext());
	}
	Stringp ShellCore::unescapeMultiByte(AvmMethodEnv,ScriptObject* self,Stringp value) //Returns an unescaped copy of the input string, which is decoded from either system code page page or UTF-8 depending on the value of System.useCodePage.
	{
		return Toplevel::unescape(self,value);
		//return NULL;
	}


	/*bool ShellCore::IsProtoTypeOf(Atom thisAtom,Atom V)
	{
		if (AvmCore::isNullOrUndefined(V))
			return false;

		ScriptObject* o = shell_toplevel->toPrototype(V);
		for (; o != NULL; o = o->getDelegate())
		{
			if (o->atom() == thisAtom)
				return true;
		}
		return false;
	}*/

	bool ShellCore::_setProperty(ScriptObject*pObject,Atom name,Atom value,Namespacep publicNS,TraitsBindingsp t)
	{
		AvmCore*pCore=pObject->core();
		TRY(pCore,kCatchAction_ReportAsError)
		{
			//	ShellCore::_setProperty(this,str->atom(),value,m_pCore->findPublicNamespace(),t->getTraitsBindings());
		
			Binding b=t->findBinding(AvmCore::atomToString(name),publicNS);
			switch (AvmCore::bindingKind(b))
			{
				case BKIND_METHOD:
				case BKIND_CONST:
				case BKIND_GET:
					 break;
				case BKIND_VAR:
				{
					pObject->coerceAndSetSlotAtom(AvmCore::bindingToSlotId(b), value);
					return true;
				}

				case BKIND_SET:
				case BKIND_GETSET:
				{
					// Invoke the setter
					VTable*vtable=pObject->vtable;
					uint32 m = AvmCore::bindingToSetterId(b);
					AvmAssert(m < vtable->traits->getTraitsBindings()->methodCount);

					MethodEnv* method = vtable->methods[m];
					Atom atomv_out[2] = { pObject->atom(), value };
					method->coerceEnter(1, atomv_out);
					return true;
				}break;

				case BKIND_NONE:
				{
					if(pObject->traits()->needsHashtable())
						pObject->setAtomProperty(name,value);
					return true;
				}
			}
		
		}
		CATCH(Exception *exception)
		{	
			return false;
		}
		END_CATCH
		END_TRY
		return false;
	}

	void ShellCore::ExportClass(Stringp name)
	{
		if(m_pPlayer->player->m_strWaitClass.strTxt!=NULL)
		{
			StUTF8String s(name);
			if(strcmp(s.c_str(),m_pPlayer->player->m_strWaitClass.strTxt)==0)
			{
				m_pPlayer->m_bPreLoad=true;
				m_pPlayer->player->m_strWaitClass.Release();
				m_pPlayer->player->m_strWaitClass.strTxt=NULL;
			}
		}
	}

	void ShellCore::throwExtError(int type,Stringp message)
	{
		ScriptObject*pClass=NULL;
		switch(type)
		{
		case kExtError_EOFError:// 如果尝试读取的内容超出可用数据的末尾，则会引发 EOFError 异常。
			 pClass=((ShellToplevel*)shell_toplevel)->getEOFErrorClass();
			 break;
		case kExtError_IllegalOperationError:// 当方法未实现或者实现中未涉及当前用法时，将引发 IllegalOperationError 异常。
			 pClass=((ShellToplevel*)shell_toplevel)->getIllegalOperationErrorClass();
			 break;
		case kExtError_InvalidSWFError:// Flash,// Player 遇到损坏的 SWF 文件时，将引发此异常。
			 pClass=((ShellToplevel*)shell_toplevel)->getInvalidSWFErrorClass();
			 break;
		case kExtError_IOError:// 某些类型的输入或输出失败时，将引发 IOError 异常。
			 pClass=((ShellToplevel*)shell_toplevel)->getIOErrorClass();
			 break;
		case kExtError_MemoryError:// 内存分配请求失败时，将引发 MemoryError 异常。
			 pClass=((ShellToplevel*)shell_toplevel)->getMemoryErrorClass();
			 break;
		case kExtError_ScriptTimeoutError:// 达到脚本超时间隔时，将引发 ScriptTimeoutError 异常。
			 pClass=((ShellToplevel*)shell_toplevel)->getScriptTimeoutErrorClass();
			 break;
		case kExtError_StackOverflowError:
			 pClass=((ShellToplevel*)shell_toplevel)->getStackOverflowErrorClass();
			 break;
		default:
			 AvmAssert(false);
		}
		Atom args[2] = { nullObjectAtom, message->atom() };
		throwAtom(pClass->construct(1, args));
	}

	void ShellCore::fscommand(AvmMethodEnv env,Stringp strCmd,Stringp strArgs)
	{
		ShellCore*core=(ShellCore*)env->core();
		StUTF8String cmd(strCmd),args(strArgs);
		core->GetPlayer()->ProcessFSCommand((char*)cmd.c_str(),(char*)args.c_str());
	}

/*	int ShellCore::GetSlotsCount(ScriptObject*pObj,bool bWrite,bool bPublic,bool bAll)
	{
		int iCount=0;
		Traits*traits=pObj->vtable->traits;
		while(traits)
		{
			TraitsBindingsp b=traits->getTraitsBindings();
			int index=0,iSet=0;
			Namespacep publicNS=getPublicNamespace(0);
			for(;;)
			{
				index=b->next(index);
				if(index<=0) break;
				if(bPublic)
				{
					Namespacep ns=b->nsAt(index);
					if(ns!=publicNS) continue;
				}
				Stringp str=b->keyAt(index);
				if(str)
				{
					Binding bd=b->valueAt(index);
					if(bd==BIND_NONE) continue;
					switch (AvmCore::bindingKind(bd))
					{
						case BKIND_VAR:
							 iCount++;break;
						case BKIND_CONST:
							 break;
						case BKIND_GET:
							 if(!bWrite) iCount++;
							 break;
						case BKIND_GETSET:
							 iCount++;
							 break;
					}
				  }
			}
			if(traits->getHashtableOffset()!=0)
			{
				InlineHashtable*b=NULL;
				union {
					uint8_t* p;
					InlineHashtable* iht;
					HeapHashtable** hht;
				};
				p = (uint8_t*)pObj + traits->getHashtableOffset();
				if(!traits->isDictionary)
				{
					if (iht->getCapacity() )
						//const_cast<ScriptObject*>(pObj)->initHashtable();
						b=iht;
					//return iht;
				}
				else
				{
					//DictionaryObjects store pointer to HeapHashtable at
					//the hashtable offset
					b=(*hht)->get_ht();
				}

				//InlineHashtable*b=pObj->getTable();
				if(b)
				{
					int index=0;//,iCount=0;
					for(;;)
					{
						index=b->next(index);
						if(index<=0) break;
						//Stringp str=b->keyAt(index);
						//if(str)
						{
							iCount++;
						}
					}
				}
			}
			if(!bAll) break;
			traits=traits->base;
		}
		return iCount;

		return iCount;

	}*/
}

int ConsoleOutputStream::write(const void *buffer, int count)
{
#ifdef _WINEMU
	if(buffer&&count)
	{
		char*buf=(char*)buffer;
		for(int i=0;i<count;i++)
		{
			switch(buf[i])
			{
			case '\r':
			case '\n':
				if(!m_output.IsEmpty())
				{
					XString16 s(m_output);
					this->m_pPlayer->Trace(s);
					m_output.Empty();
				}break;
			default:
				m_output+=buf[i];
				break;
			}
		}
		//buf[count]=0;

	}
#endif
	return count;
}

//new adds
void ShellCore::scanHardware()
{

}
void ShellCore::MMEndCommand(bool endStatus, Stringp pNotifyString)
{

}

void ShellCore::stopSampling()
{

}
void ShellCore::startSampling()
{

}
void ShellCore::setSamplerCallback(FunctionObject *pFun)
{

}
void ShellCore::sampleInternalAllocs(bool b)
{

}
void ShellCore::pauseSampling()
{

}
bool ShellCore::isGetterSetter(Atom obj, QNameObject *pQName)
{
	return false;
}
double ShellCore::getSize(Atom o)
{
	return 0.0;
}
double ShellCore::getSetterInvocationCount(Atom obj, QNameObject *pQName)
{
	return 0.0;
}
Atom ShellCore::getSavedThis(FunctionObject *pObj)
{
	return kAvmThunkUndefined;
}
Atom ShellCore::getSamples()
{
	return kAvmThunkUndefined;
}
double ShellCore::getSampleCount()
{
	return 0;
}
Atom ShellCore::getMemberNames(Atom o, bool instanceNames/* = false*/)
{
	return kAvmThunkUndefined;
}
Stringp ShellCore::getMasterString(Stringp pStr)
{
	return NULL;
}
ArrayObject* ShellCore::getLexicalScopes(FunctionObject *pObj)
{
	return NULL;
}
double ShellCore::getInvocationCount(Atom obj, QNameObject *pQName)
{
	return 0;
}
double ShellCore::getGetterInvocationCount(Atom obj, QNameObject *pQName)
{
	return 0;
}
void ShellCore::clearSamples()
{

}
//new adds end

