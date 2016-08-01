// ObjectClip.cpp: implementation of the ObjectClip class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectClip.h"
#include "ObjectCreator.h"
#include "splayer.h"
#include "XXObjectBitmap1.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void XXObjectClip::GetTargetPath(XSWFCONTEXT&cnt,XXVar&path)//XString8 &strPath)
{
	if(m_pObject==XNULL) return;
	cnt.pPlayer->GetTargetPath(m_pObject,path);//m_pObject);
	//strPath.SetString(str,STRING_ATTACH);
	//m_pDisp->GetTargetPath(strPath);
}



///////////////////////////////////////////////////////////
//#define XXCLIPCLASS(TOKEN) XXCLASS(TOKEN,XXObjectClip,XXOBJ_CLIP)
#define XNEEDCLIPR XNEEDOBJR(m_pObject,cnt)
#define XNEEDCLIP  XNEEDOBJ(m_pObject)

  
XXObjectClip::~XXObjectClip()
{
#ifdef _DEBUG
	nObjCount[XXOBJ_CLIP]--;
#endif	
	//Release();
	m_strPath.Release();
//	m_strPath=_strEmpty;
//	if(this==m_pRoot->rootObject->pObject)
//		int v=0;

/*	XXObjectFunction*pFun=m_pFuns;
	m_pFuns=XNULL;
	while(pFun)
	{
		pFun->ReleaseFunction();
		pFun=pFun->m_pNext;
	}*/
}

void XXObjectClip::ReleaseClip()
{
   //if(m_pObject&&m_pObject->character->tag==109)
//	   int v=0;
   //if(m_pObject)
   {
	   //char*path=m_pRoot->splayer->GetTargetPath(m_pObject);
	   //m_pRoot->splayer->GetTargetPath(m_pObject,m_strPath);
	   //m_strPath.SetString(path,STRING_ATTACH);
	   //if(m_strPath=="/")
		//	int v=0;
	   //if(m_strPath=="/ysound/s1")
		  // int v=0;
   }
   //if(m_pObject&&m_pObject->character->tag==40)
//	   int v=0;
   m_pObject=XNULL;
   //GetTargetPath(m_strPath);
   //XXObjectEvent::Destroy();
}

XBOOL XXObjectClip::SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	var.ToConst(XOCT_UNDEFINE);
	if(m_pObject)
	{
		ScriptThread*pThread=m_pObject->GetThread();
		switch(id)
		{
		case _SYSID(attachBitmap):
		case _SYSID(attachAudio):
		case _SYSID(beginBitmapFill):
		case _SYSID(beginFill):
		case _SYSID(beginGradientFill):
		case _SYSID(clear):
		case _SYSID(createEmptyMovieClip):
		case _SYSID(createTextField)://
		case _SYSID(curveTo):
		case _SYSID(duplicateMovieClip):
		case _SYSID(endFill):
		case _SYSID(getRect):
		case _SYSID(getBounds):
		case _SYSID(getDepth):
		case _SYSID(getInstanceAtDepth):
		case _SYSID(getNextHighestDepth):
		case _SYSID(getSWFVersion):
		case _SYSID(getTextSnapshot)://
		case _SYSID(getURL)://
		case _SYSID(globalToLocal):
		case _SYSID(hitTest):
		case _SYSID(lineGradientStyle)://break;
		case _SYSID(lineStyle):
		case _SYSID(lineTo):
		case _SYSID(loadMovie)://
		case _SYSID(loadVariables)://break;
		case _SYSID(localToGlobal):
		case _SYSID(moveTo):
		case _SYSID(nextFrame):
		case _SYSID(play):
		case _SYSID(prevFrame):
        case _SYSID(unloadMovie):
		case _SYSID(removeMovieClip):
		case _SYSID(setMask)://
		case _SYSID(stop):
		case _SYSID(startDrag):
		case _SYSID(stopDrag):
		case _SYSID(swapDepths):
		case _SYSID(gotoAndStop):
		case _SYSID(gotoAndPlay):
		case _SYSID(attachMovie):
		case _SYSID(getBytesLoaded):
		case _SYSID(getBytesTotal):
			 var=this;
			 var.ToFunction(id);
			 return XTRUE;
		case _SYSID(transform):
			 var=m_pRoot->m_pGlobal->CreateTransform(this);
			 return XTRUE;
		case _SYSID(hitArea):
			 if(m_pObject->pLink==XNULL)
			 {
				GetClipOf(_SYSNAME(hitArea),var);
				 //var.ToConst(XOCT_UNDEFINE);//=this;//->ToConst(XOCT_NULL);
			 }
			  else
				var=m_pObject->pLink->pObject;//GetObject();
				//m_pDisp->m_pLink->GetObject();
			  return XTRUE;
		case _SYSID(_framesloaded):
			 //var=(int)m_pDisp->m_frames.GetSize();
			 if(pThread)
			 {
				 if (pThread == pThread->player) {
                    // this is the main movie
					if (pThread->player->numFramesComplete < pThread->numFrames) {
						var=pThread->player->numFramesComplete;
					} else {
						var=pThread->numFrames;
					}
				} else {
                    // movie sprites are alwayes fully loaded 
                    // before they start playing
                    var = pThread->numFrames;
				}
			 }
			 break;
		case _SYSID(_totalframes):
			 if(pThread)
				var=(int)pThread->numFrames;
			 return XTRUE;
		case _SYSID(_currentframe):
			 if(pThread)
				var=pThread->curFrame+1;
			 return XTRUE;
		case _SYSID(_alpha):
			 var=m_pObject->GetAlpha();
				 //m_pDisp->m_pDisp->GetAlpha();
			 return XTRUE;
		case _SYSID(enabled):
			 var=m_pObject->IsEnable();
				 //m_pDisp->IsDisabled();
			 var.ToLogic();
			 return XTRUE;
		case _SYSID(_name):
			 var=m_pObject->name;
				 //m_pDisp->m_pDisp->m_strName;
			 return XTRUE;
		case _SYSID(_parent):
			 if(m_pObject->parent)//m_pDisp->m_pParent)
				 var=m_pObject->parent->pObject;
				 //var=m_pDisp->m_pParent->GetObject();
			 return XTRUE;
		case _SYSID(_rotation):
			{
			  //XSWFDispObj*pObj=m_pDisp->m_pDisp;
			  var=m_pObject->GetRotate();
				  //pObj->GetRotate();
			}return XTRUE;
		case _SYSID(_target):
			{

				pCnt->pPlayer->GetTargetPath(m_pObject,var);

			}return XTRUE;
		case _SYSID(_url):
			if(m_pObject->GetPlayer())
			{
				var=m_pObject->GetPlayer()->m_url;
			 //XString16 str(cnt.pPlayer->GetURL());
			 //var=str;
			}return XTRUE;
		case _SYSID(_visible):
			var=m_pObject->IsVisible();//visible;//m_pDisp->IsVisiable();
			var.ToLogic();break;
		case _SYSID(_width):
			var=m_pObject->GetWidth();
				//m_pDisp->GetWidth(cnt);
			return XTRUE;
		case _SYSID(_height):
			var=m_pObject->GetHeight();
				//m_pDisp->GetHeight(cnt);
			return XTRUE;

		case _SYSID(_x):
			{
			  //XSWFDispObj*pObj=m_pDisp->m_pDisp;
			  //if(m_pObject->character->tag==1032)
			  //	  int v=0;
			  var=m_pObject->GetPosX();
				  //pObj->GetX()/SWFTRIP;
			}return XTRUE;
		case _SYSID(_y):
			{
			  //XSWFDispObj*pObj=m_pDisp->m_pDisp;
			  var=m_pObject->GetPosY();
				  //(float)pObj->GetY()/SWFTRIP;
			}return XTRUE;
		case _SYSID(_xmouse):
			{
				double x,y;
				pCnt->pPlayer->GetMousePos(m_pObject,x,y);
				var=x;
			}return XTRUE;
		case _SYSID(_ymouse):
			{
				double x,y;
				pCnt->pPlayer->GetMousePos(m_pObject,x,y);
				var=y;
			}return XTRUE;
		case _SYSID(_xscale):
			{
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				var=m_pObject->GetScaleX();
			}return XTRUE;
		case _SYSID(_yscale):
			{
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				var=m_pObject->GetScaleY();
			}return XTRUE;
		}
	}
	return XXObject::SysGetMember(id,pCnt,var);
}
XBOOL XXObjectClip::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{	
	if(m_pObject)
	{
		ScriptThread*pThread=m_pObject->GetThread();
		int nSize=list.GetSize();
		switch(id)
		{
		case _SYSID(attachBitmap):			 
			 if(!pThread) break;
			 if(nSize>=2&&list[0].IsObject(XXOBJ_BITMAP))
			 {
				XXObjectBitmap*pBitmap=(XXObjectBitmap*)list[0].pObject;
				if(pBitmap->m_pChar)//!pBitmap->IsEmpty())
				{
					XS32 nDepth=list[1].ToInt();
					pThread->AttachBitmap(pBitmap,nDepth,XTRUE);
						//pBitmap->LockBitmap();
				}
			 }
			 return XTRUE;
		case _SYSID(attachAudio):
			 if(!pThread) break;
			 if(nSize>=1)//
			 {
				 //m_pDisp->AttachAudio(
			 }return XTRUE;
		case _SYSID(beginBitmapFill):
			 if(!pThread) break;
			 if(nSize&&list[0].IsObject(XXOBJ_BITMAP))
			 {
			    XXObjectBitmap*pBmp=(XXObjectBitmap*)list[0].pObject;
				if(pBmp->m_pChar)//!pBmp->IsEmpty())//GetBitmap())// return;
				{
					MATRIX*pMatrix=XNULL;
					if(nSize>1&&list[1].IsObject(XXOBJ_MATRIX))
						pMatrix=&((XXObjectMatrix*)list[1].pObject)->matrix;
					XBOOL bRepeat=nSize>2?list[2].ToLogic():XFALSE;
					//XU8 bCreate=pBmp->IsCreated();
					//pBmp->LockBitmap();
					pThread->BeginFill(pBmp,pMatrix,bRepeat,XTRUE);
				}
			 }return XTRUE;
		case _SYSID(beginFill):
			 if(nSize&&pThread)
			 {
				XU32 nColor=list[0].ToInt();
				if(nSize>1)
				{
				  int alpha=((list[1].ToInt()*0xff)/100);
				  if(alpha>0xff) alpha=0xff;
				  else if(alpha<0) alpha=0;
				  nColor|=alpha<<24;
				}
				else
				  nColor|=0xff000000;
#ifdef _ARGB_PIXEL_
				SWAPPIXELRB(nColor);
#endif
				 pThread->BeginFill(nColor,0);
			 }return XTRUE;
		case _SYSID(beginGradientFill):
			 beginGradientFill(*pCnt,list);
			 return XTRUE;
		case _SYSID(clear):
			 if(pThread)
				 pThread->ReleaseDraw();
			 return XTRUE;
		case _SYSID(createEmptyMovieClip):
			 //break;
			 if(nSize>=2&&pThread)
			 {
			   list[0].ToString(XFALSE);
			   int nDepth=list[1].ToInt()+0x4000;
			   SObject*pObj=pThread->CreateEmptyClip(*pCnt,list[0].strTxt,nDepth/*|0x4000*/);
			   if(pObj)
				   pCnt->pStack->Push(pObj->pObject);
			   else
				   pCnt->pStack->PushConst(XOCT_NULL);
			   //if(pObj)
				//   var=pObj->pObject;
			 }return XTRUE;
		case _SYSID(createTextField)://
			 pCnt->pStack->PushConst(XOCT_NULL);
			 return XTRUE;
		case _SYSID(curveTo):
			 if(nSize>=4&&pThread)
			 {
				XFDOUBLE cx=list[0].ToFloat()*20;
				XFDOUBLE cy=list[1].ToFloat()*20;
				XFDOUBLE ax=list[2].ToFloat()*20;
				XFDOUBLE ay=list[3].ToFloat()*20;
				pThread->CurveTo(XROUND(ax),XROUND(ay),XROUND(cx),XROUND(cy));
			 }return XTRUE;
		case _SYSID(duplicateMovieClip):
			 if(pThread&&nSize>=2&&m_pObject->parent)
			 {
				if(pThread!=pThread->player)
				{
					ScriptThread*parent=m_pObject->parent->GetThread();
					XXObject*pInit=nSize>2&&list[2].IsObject()?list[2].pObject:XNULL;
					list[0].ToString(XFALSE);
					int nDepth=list[1].ToInt()+0x4000;//|0x4000;
					//m_pContext->InitContextA(this,m_thread,GetThis(),m_pWith);
					SObject*pDst=parent->CloneSprite(*pCnt,pThread,list[0],nDepth/*|0x4000*/,pInit);
					/*XSWFDispObj*pDisp=m_pDisp->m_pDisp;

					XDTSpriteDisp*pDst=m_pDisp->GetParent()->AddClip(cnt,nDepth,m_pDisp,list[0].strData,pInit,
												   pDisp?pDisp->GetMatrix():XNULL,
												   pDisp?pDisp->GetForm():XNULL,
												   m_pDisp->GetEvent());*/
					if(pDst)
					{
						pCnt->pStack->Push(pDst->pObject);//(XXObjectClip*)pDst->GetObject();
						//pDst->SetEvent(m_pDisp->GetEvent());
					}
					else
						pCnt->pStack->PushConst(XOCT_NULL);
				}
			 }return XTRUE;
		case _SYSID(endFill):
			 if(pThread)
				 pThread->EndFill();
			 return XTRUE;
		case _SYSID(getRect):
		case _SYSID(getBounds):
			 if(nSize&&list[0].IsObject(XXOBJ_CLIP))//&&pThread)
			 {
				//XXObjectRectangle*pr=m_pRoot->m_pGlobal->CreateRectangle();
				XXObject*pObj=m_pRoot->m_pGlobal->CreateObject();
				if(pObj)
				{
					XXObjectClip*pClip=(XXObjectClip*)list[0].pObject;
					MATRIX*pMatrix=XNULL;
					MATRIX mat;
					if(pClip->m_pObject&&pClip->m_pObject!=m_pObject->parent)//m_pDisp)
					{
						::MatrixIdentity(&mat);
						SObject*p=m_pObject->parent;//->parent;
						while(p)
						{
							if(p==pClip->m_pObject||p->character==XNULL||
							    p->character->type==rootChar) break;
							::MatrixConcat(&mat,&p->xform.mat,&mat);
							//if(p->character==XNULL||p->character->type==rootChar) break;
							p=p->parent;
						}
						//::MatrixConcat(&mat,&m_pRoot->display->camera.mat,&mat);
						//pClip->m_pObject->GetTotalMatrix(mat,XFALSE);
						//::MatrixInvert(&mat,&mat);
						pMatrix=&mat;
					}
					SRECT rect;
					
					m_pObject->GetBoundBox(rect,pMatrix);
					//rect.ToPixel();
					//pr->left=(float)rect.xmin/20;
					//pr->top=(float)rect.ymin/20;
					//pr->width=(float)(rect.xmax-rect.xmin)/20;
					//pr->height=(float)(rect.ymax-rect.ymin)/20;
					pObj->AddMember(XXVar(_strXMin),XXVar(rect.xmin/20.0F));
					pObj->AddMember(XXVar(_strYMin),XXVar(rect.ymin/20.0F));
					pObj->AddMember(XXVar(_strXMax),XXVar(rect.xmax/20.0F));
					pObj->AddMember(XXVar(_strYMax),XXVar(rect.ymax/20.0F));
					//pCn=pObj;
				}
				pCnt->pStack->Push(pObj);
			 }break;
		case _SYSID(getDepth):
			 pCnt->pStack->Push(m_pObject->depth);//m_pDisp->GetDepth();
			 return XTRUE;
		case _SYSID(getInstanceAtDepth):
			 if(nSize)
			 {
				int nDepth=list[0].ToInt();
				//XSWFDT*p=m_pDisp->GetDepth(nDepth);
				SObject*p=m_pObject->GetChildOf(nDepth);
				if(p)//p->GetClassType()==_objSprite)
				{
				   pCnt->pStack->Push(p->pObject);//GetObject();
				}
				else
					pCnt->pStack->PushConst(XOCT_NULL);
			 }return XTRUE;
		case _SYSID(getNextHighestDepth):
			{
				int v=m_pObject->GetMaxDepth()+1-0x4000;
				if(v<0) v=0;
				pCnt->pStack->Push(v);
				//var=v;//m_pObject->GetMaxDepth()+1-0x4000;
			}
			 return XTRUE;
		case _SYSID(getSWFVersion):
			if(m_pObject->GetPlayer())
			 pCnt->pStack->Push(m_pObject->GetPlayer()->version);
				 //cnt.pPlayer->GetVersion();
			 return XTRUE;
		case _SYSID(getTextSnapshot)://
			 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		case _SYSID(getURL)://
			if(m_pObject->GetPlayer())
			{
			  pCnt->pStack->Push(m_pObject->GetPlayer()->m_url);
			}
				 //cnt.pPlayer->GetURL());
			 return XTRUE;
		case _SYSID(globalToLocal):
			 globalToLocal(*pCnt,list);
			 return XTRUE;
		case _SYSID(hitTest):
			 pCnt->pStack->PushBool(hitTest(*pCnt,list));
			 return XTRUE;
		case _SYSID(lineGradientStyle)://break;
		case _SYSID(lineStyle):
			 if(nSize&&pThread)
			 {
			    int nWidth=list[0].ToInt();
				if(nWidth<1) nWidth=1;
				XU32 nColor=nSize>1?list[1].ToInt():0xffffff;
				nColor|=nSize>2?(((list[2].ToInt()*0xff)/100)<<24):0xff000000;
#ifdef _ARGB_PIXEL_
				SWAPPIXELRB(nColor);
#endif
				pThread->LineStyle(nWidth,nColor);
			 }return XTRUE;
		case _SYSID(lineTo):
			 if(nSize>=2)
			 {
				XFDOUBLE x=list[0].ToFloat()*20;
				XFDOUBLE y=list[1].ToFloat()*20;
				pThread->LineTo(XROUND(x),XROUND(y));
			 }return XTRUE;
		case _SYSID(loadMovie)://
		case _SYSID(loadVariables)://break;
			 if(nSize&&pThread)
				{

					list[0].ToString(XFALSE);
					if(!list[0].strTxt)
						break;
					XXVar strPath;
					GetTargetPath(*pCnt,strPath);
					XU8 method=kHttpSendUseGet|kHttpLoadTarget;
					if(id==_SYSID(loadVariables))
						method=kHttpLoadVariables|kHttpLoadTarget;
					else if(nSize>=2)
					{
						list[1].ToString(XFALSE);
						switch(list[1].strTxt[0])
						{
						case 'p':
						case 'P':
							 method=kHttpSendUsePost|kHttpLoadTarget;
							 break;
						}
					}
					pCnt->pPlayer->GetURL2(*pCnt,pThread,list[0].strTxt,strPath.strTxt,method);
				}
			 return XTRUE;
		case _SYSID(localToGlobal):
			 localToGlobal(*pCnt,list);
			 return XTRUE;
		case _SYSID(moveTo):
			 //bPush=XFALSE;
			 if(nSize>=2&&pThread)
			 {
				XFDOUBLE x=list[0].ToFloat()*20;
				XFDOUBLE y=list[1].ToFloat()*20;
				pThread->MoveTo(XROUND(x),XROUND(y));
			 }return XTRUE;
		case _SYSID(nextFrame):
			 if(pThread)
				pThread->Seek(pThread->GetFrame()+1,false,ScriptPlayer::SK_STOP);//->NextFrame();
			 return XTRUE;
		case _SYSID(play):
			 if(pThread)
			 {
				 //pThread->m_bNoEnter=true;
				 pThread->Play();
				 
			 }
			 //m_pDisp->StartPlay();
			 return XTRUE;
		case _SYSID(prevFrame):
			 if(pThread)
				 pThread->Seek(pThread->GetFrame()-1,false,ScriptPlayer::SK_STOP);
			 //m_pDisp->PrevFrame();
			 return XTRUE;
        case _SYSID(unloadMovie):
		case _SYSID(removeMovieClip):
			 if(pThread)//&&m_pObject->parent)
			 {
				 //break;
				 if(!pThread->rootObject->IsCloneObject()&&
					 m_pRoot->version<=8) break;
//				 if(m_pObject->character->tag==94)
//					 break;
//					 int v=0;
				 pCnt->pPlayer->RemoveSprite(pThread,XFALSE);
				 //ScriptThread*parent=m_pObject->parent->GetThread();
				 //parent->RemoveSprite(pThread);
			 }
			 //if(m_pDisp->GetParent())
			//	 m_pDisp->GetParent()->Remove(cnt,m_pDisp);
			 return XTRUE;
		case _SYSID(setMask)://
			 if(nSize&&list[0].IsObject())
			 {
				 XXObject*pObj=list[0].pObject;
				 if(pObj->m_pObject)
					 m_pObject->SetMask(pObj->m_pObject);
			 }
			 return XTRUE;
		case _SYSID(stop):
			 if(pThread)
				pThread->StopPlay();return XTRUE;
		case _SYSID(startDrag):
			{
				XBOOL bLookCenter=nSize>0?list[0].ToLogic():XFALSE;
				SRECT rect;
				if(nSize>4)
				{
					rect.xmin=(XINT)(list[1].ToFloat()*20);
					rect.ymin=(XINT)(list[2].ToFloat()*20);
					rect.xmax=(XINT)(list[3].ToFloat()*20);
					rect.ymax=(XINT)(list[4].ToFloat()*20);
				}
				pCnt->pPlayer->StartDrag(m_pObject,rect,bLookCenter,nSize>4);//cnt,bLookCenter,rect);				
			}return XTRUE;
		case _SYSID(stopDrag):
			pCnt->pPlayer->StopDrag();
			//cnt.pDragObj=XNULL;
			return XTRUE;
		case _SYSID(swapDepths):
			if(nSize)
			{
			   SObject*parent=m_pObject->parent;
			   if(parent)
			   {
					if(list[0].IsObject())
					{
						if(list[0].pObject&&list[0].pObject->m_pObject)
							parent->SwapDepth(m_pObject,list[0].pObject->m_pObject->depth);
					}
				    if(list[0].IsNum3())
					   parent->SwapDepth(m_pObject,list[0].ToInt()+0x4000);
						//m_pDisp->Swap(list[0].ToInt());
					else
					{
						list[0].ToString(XFALSE);
						parent->SwapDepth(m_pObject,list[0].strTxt);
						//m_pDisp->Swap(list[0].strData);
					}
			   }
			}
			return XTRUE;
		case _SYSID(gotoAndStop):
			//if(pThread)
			//{
				
			//}
		case _SYSID(gotoAndPlay):
			 
			 if(nSize&&pThread)
			 {
				
				if(list[0].IsString2())
				{
					list[0].ToString(XFALSE);
					//XString16 str(list[0].strData,STRING_ATTACH);
					int f=pThread->FindLabel(list[0].strTxt);//str);
					//m_pDisp->GotoLabel(str);
					//str.EmptyNoFree();
					
					if(f>=0)
					{
						//if(f==4)
						//	int v=0;
						pThread->Seek(f,false,id==_SYSID(gotoAndPlay)?ScriptPlayer::SK_PLAY:ScriptPlayer::SK_STOP);
						//if(id==_SYSID(gotoAndPlay))
						//	pThread->Play();
						break;
					}
					else
					{
						char c=list[0].strTxt[0];
						if(c<'0'||c>'9') break;
						//if(id==_SYSID(gotoAndStop))
							
					}
					//	int v=0;
				}
				//else
				{
					int f=list[0].ToInt()-1;
					//if(f==4)
					//	int v=0;
					//m_pDisp->GotoFrame(f);
					if(f>=0)
						pThread->Seek(f,false,id==_SYSID(gotoAndPlay)?ScriptPlayer::SK_PLAY:ScriptPlayer::SK_STOP);
				}
				//if(id==_SYSID(gotoAndPlay))
				//	pThread->Play();
				//else
				//	pThread->StopPlay();
			 }return XTRUE;
		case _SYSID(attachMovie):
			 //break;
			 if(nSize>=3)
			 {

				XXVar*pID=&list[0];
				//XXVar*pName=&list[1];
				XXVar*pDepth=&list[2];
				XXObject*pObject=XNULL;
				if(list.GetSize()>3&&list[3].IsObject())
				  pObject=list[3].pObject;
				pID->ToString(XFALSE);
				//pName->ToString(XFALSE);
				list[1].ToString(XFALSE);
				//if(strcmp(pID->strTxt,"lib_boat")==0)
				//	int v=0;
				XS32 depth=pDepth->ToInt()+0x4000;
				SCharacter*pDT=pThread->player->ImportChar(pID->strTxt);
				//if(pDT->tag==68)
				//	int v=0;
				if(pDT)
				{
					SObject*p=pThread->AttachClip(*pCnt,pDT,list[1],depth/*|0x4000*/,pObject);
					//XDTSpriteDisp*p=
					//	m_pDisp->AddClip(cnt,depth,pDT,pName->strData,pObject);
					if(p) pCnt->pStack->Push(p->pObject);
					//else  pCnt->pStack->PushConst(XOCT
				}
			 }return XTRUE;
		case _SYSID(getBytesLoaded):
			pCnt->pStack->Push(m_pObject->thread->player->len);
				//m_pDisp->m_nBytesLoaded;
			return XTRUE;
		case _SYSID(getBytesTotal):
			pCnt->pStack->Push((int)m_pObject->thread->player->scriptLen);
				//m_pDisp->m_nBytesTotal;
			return XTRUE;
		}
	}

	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectClip::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
	if(m_pObject)
	{
		ScriptThread*pThread=m_pObject->GetThread();
		switch(id)
		{
		case _SYSID(attachBitmap):
		case _SYSID(attachAudio):
		case _SYSID(beginBitmapFill):
		case _SYSID(beginFill):
		case _SYSID(beginGradientFill):
		case _SYSID(clear):
		case _SYSID(createEmptyMovieClip):
		case _SYSID(createTextField)://
		case _SYSID(curveTo):
		case _SYSID(duplicateMovieClip):
		case _SYSID(endFill):
		case _SYSID(getRect):
		case _SYSID(getBounds):
		case _SYSID(getDepth):
		case _SYSID(getInstanceAtDepth):
		case _SYSID(getNextHighestDepth):
		case _SYSID(getSWFVersion):
		case _SYSID(getTextSnapshot)://
		case _SYSID(getURL)://
		case _SYSID(globalToLocal):
		case _SYSID(hitTest):
		case _SYSID(lineGradientStyle)://break;
		case _SYSID(lineStyle):
		case _SYSID(lineTo):
		case _SYSID(loadMovie)://
		case _SYSID(loadVariables)://break;
		case _SYSID(localToGlobal):
		case _SYSID(moveTo):
		case _SYSID(nextFrame):
		case _SYSID(play):
		case _SYSID(prevFrame):
        case _SYSID(unloadMovie):
		case _SYSID(removeMovieClip):
		case _SYSID(setMask)://
		case _SYSID(stop):
		case _SYSID(startDrag):
		case _SYSID(stopDrag):
		case _SYSID(swapDepths):
		case _SYSID(gotoAndStop):
		case _SYSID(gotoAndPlay):
		case _SYSID(attachMovie):
		case _SYSID(getBytesLoaded):
		case _SYSID(getBytesTotal):
			 return false;
		case _SYSID(transform):
			 if(var.IsObject(XXOBJ_TRANSFORM))
			 {
				XXObjectTransform*p=(XXObjectTransform*)var.pObject;	
				XXObjectClip*ps=p->GetClip();
				if(ps&&ps->m_pObject&&m_pObject!=ps->m_pObject)
				{	
					//STransform form;
					MATRIX mat;
					ps->GetMatrix(mat);
					SetMatrix(mat);
					XCXForm form;
					ps->GetCXForm(form);
					SetCXForm(form);
					//MATRIX mat;
					//ps->GetMatrix(mat);
					//SetMatrix(mat);
					//STransform form;
					//ps->GetCXForm(form);
					//SetCXForm(form);
				}
			 }return XTRUE;
		case _SYSID(hitArea):
			 if(var.IsMovieClip())
			 {
			   XXObjectClip*pLink=(XXObjectClip*)var.pObject;
				m_pObject->LinkBy(pLink->m_pObject);
				//m_pDisp->Link(pLink->m_pDisp);
			 }
			 else
				 m_pObject->LinkBy(XNULL);
			 return XTRUE;			
		case _SYSID(_alpha):
			 {
				int alpha=var.ToInt();	
				//if(alpha==0)
				//	int v=0;
				m_pObject->SetAlpha(alpha);
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				//pObj->SetAlpha(alpha);
				//m_pDisp->ResetFrame();
			 }return XTRUE;
		case _SYSID(enabled):
			 //if(nSize)
			//	 m_pObject->SetEnable(var.ToLogic());//->enable=var.ToLogic();
				 //m_pObject->Enable(var.ToLogic());
				// m_pDisp->Enable(cnt,var.ToLogic());
			 return XTRUE;
		case _SYSID(_name):
			 //if(nSize)
			 {
				var.ToString(XFALSE);
				//FreeStr(m_pObject->name);
				m_pObject->name=var;//CreateStr(var.strTxt);
				//var.nType=XODT_INT;
				//m_pDisp->SetName(var.strData);
			 }return XTRUE;
		case _SYSID(_rotation):
			//if(nSize)
			{
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				//if(m_pObject->character->type==rootChar)
				//	int vv=0;
				XFDOUBLE vi=var.ToFloat();
				m_pObject->SetRotate((XFDOUBLE)vi);
				//m_pDisp->ResetBounds();
				//m_pDisp->ResetFrame();	
			}return XTRUE;
		case _SYSID(_visible):
//			if(nSize)
			{
				m_pObject->SetVisible(var.ToLogic());
			}
				//m_pDisp->Visiable(var.ToLogic());
			return XTRUE;
		case _SYSID(_width):
			//break;
			//if(nSize)//&&!m_pDisp->m_bounds.IsRectEmpty())
			{
				XFDOUBLE w=var.ToFloat();
				m_pObject->SetWidth(w);
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				//pObj->SetWidth(w,m_pDisp->GetWidth(cnt));//m_bounds.Width()/SWFTRIP);
				//m_pDisp->ResetBounds();
				//m_pDisp->ResetFrame();
			}return XTRUE;
		case _SYSID(_height):
			//break;
			//bPush=XFALSE;
			//if(nSize)//&&!m_pDisp->m_bounds.IsRectEmpty())
			{
			    XFDOUBLE h=var.ToFloat();
				m_pObject->SetHeight(h);
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				//pObj->SetHeight(w,m_pDisp->GetHeight(cnt));//m_bounds.Height()/SWFTRIP);
				//m_pDisp->ResetBounds();
				//m_pDisp->ResetFrame();
			}return XTRUE;
		case _SYSID(_x):
			if(!var.IsNaN())
			{
			    //XSWFDispObj*pObj=m_pDisp->m_pDisp;
				
				XFDOUBLE v=var.ToFloat();
				m_pObject->SetPosX(v);
				//pObj->SetX((int)(v*SWFTRIP));
				//m_pDisp->ResetFrame();
			}return XTRUE;
		case _SYSID(_y):
			if(!var.IsNaN())
			{
			    //XSWFDispObj*pObj=m_pDisp->m_pDisp;

				XFDOUBLE v=var.ToFloat();
				m_pObject->SetPosY(v);
				//pObj->SetY((int)(v*SWFTRIP));
				//m_pDisp->ResetFrame();
			}return XTRUE;
		case _SYSID(_xscale):
			//if(nSize)
			{
			    XFDOUBLE v=var.ToFloat();
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				m_pObject->SetScaleX(v);
				//m_pDisp->ResetFrame();
			}return XTRUE;
		case _SYSID(_yscale):
			//bPush=XFALSE;
			//if(nSize)
			{
			    XFDOUBLE v=var.ToFloat();
				//XSWFDispObj*pObj=m_pDisp->m_pDisp;
				m_pObject->SetScaleY(v);
				//m_pDisp->ResetFrame();
			}return XTRUE;		
		}
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectClip::InitObject()
{
	_SYSCALL(beginBitmapFill);//XXVar(&_beginBitmapFill_));
	_SYSCALL(beginFill);//XXVar(&_beginFill_));
	_SYSCALL(beginGradientFill);//XXVar(&_beginGradientFill_));
	_SYSCALL(clear);//XXVar(&_clear_));
	_SYSCALL(createEmptyMovieClip);//XXVar(&_createEmptyMovieClip_));
	_SYSCALL(createTextField);//XXVar(&_createTextField_));
	_SYSCALL(curveTo);//XXVar(&_curveTo_));
	_SYSCALL(duplicateMovieClip);//XXVar(&_duplicateMovieClip_));
	_SYSCALL(endFill);
	_SYSCALL(getBounds);
	_SYSCALL(getDepth);
	_SYSCALL(getInstanceAtDepth);
	_SYSCALL(getNextHighestDepth);
	_SYSCALL(getRect);
	_SYSCALL(getSWFVersion);
	_SYSCALL(getTextSnapshot);
	_SYSCALL(getURL);
	_SYSCALL(globalToLocal);
	_SYSCALL(hitTest);
	_SYSCALL(lineGradientStyle);
	_SYSCALL(lineStyle);
	_SYSCALL(lineTo);
	_SYSCALL(loadMovie);
	_SYSCALL(loadVariables);
	_SYSCALL(localToGlobal);
	_SYSCALL(moveTo);
	_SYSCALL(setMask);
	_SYSCALL(startDrag);
	_SYSCALL(stopDrag);
	_SYSCALL(swapDepths);
	_SYSCALL(unloadMovie);
	 //AddMember(XXVar(_strConstructor,XXVar(this));
	_SYSCALL(_alpha);
	_SYSCALL(enabled);
	_SYSCALL(_name);
	_SYSCALL(_parent);
	_SYSCALL(_rotation);
	_SYSCALL(_target);
	_SYSCALL(_url);
	_SYSCALL(_visible);
	_SYSCALL(_width);
	_SYSCALL(_height);
	_SYSCALL(_x);
	_SYSCALL(_y);
	_SYSCALL(_xmouse);
	_SYSCALL(_ymouse);
	_SYSCALL(_xscale);
	_SYSCALL(_yscale);
	_SYSCALL(_currentframe);
	_SYSCALL(_totalframes);
	_SYSCALL(_framesloaded);
	_SYSCALL(hitArea);
	_SYSCALL(transform);
	_SYSCALL(nextFrame);
	_SYSCALL(prevFrame);
	_SYSCALL(attachMovie);
	_SYSCALL(removeMovieClip);
	_SYSCALL(attachBitmap);

	_SYSCALL(gotoAndPlay);
	_SYSCALL(gotoAndStop);
	_SYSCALL(play);
	_SYSCALL(stop);

	_SYSCALL(_target);
	_SYSCALL(getBytesLoaded);
	_SYSCALL(getBytesTotal);

}

void XXObjectClip::beginGradientFill(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDCLIP
	ScriptThread*pThread=m_pObject->GetThread();
	if(!pThread) return;
	int nSize=list.GetSize();
	if(nSize<5) return;
	if(!list[1].IsObject(XXOBJ_ARRAY)||
	   !list[2].IsObject(XXOBJ_ARRAY)||
	   !list[3].IsObject(XXOBJ_ARRAY)||
	   !list[4].IsObject(XXOBJ_MATRIX)) return;
	MATRIX*pMatrix=&((XXObjectMatrix*)list[4].pObject)->matrix;
	XU32Array colors;
	((XXObjectArray*)list[1].pObject)->GetData(colors);
	XU8Array alphas;
	((XXObjectArray*)list[2].pObject)->GetData(alphas);
	XU8Array ratios;
	((XXObjectArray*)list[3].pObject)->GetData(ratios);
	for(XU32 i=0;i<colors.GetSize();i++)
	{
		if(i>=alphas.GetSize()) break;
		colors[i]|=(alphas[i]*0xff/100)<<24;
#ifdef _ARGB_PIXEL_
		SWAPPIXELRB(colors[i]);
#endif
	}
	if(colors.GetSize()<=0||
	   ratios.GetSize()<=0) return;
	XU8 nType=0,nSpread=0,nInter=0;
	list[0].ToString(XFALSE);
	switch(list[0].strTxt[0])
	{
	case 'r':
	case 'R':nType=1;break;
	}
	if(nSize>5)
	{
		list[5].ToString(XFALSE);
		switch(list[5].strTxt[0])
		{
		case 'R':
		case 'r':
			 if(list[5].strTxt[1]=='e')
			 {
			    if(list[5].strTxt[2]=='f')
					nSpread=1;
				else
					nSpread=2;
			 }
		}
	}
	pThread->BeginFill(nType,pMatrix,colors,ratios,nSpread,nInter);
}

void XXObjectClip::globalToLocal(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	if(list.GetSize()<=0||!list[0].IsObject()) return;
	if(list[0].IsObject(XXOBJ_POINT))
	{
		XXObjectPoint*pt=(XXObjectPoint*)list[0].pObject;
		// x=(int)(pt->xPos*20);
		//int y=(int)(pt->yPos*20);
		m_pObject->GlobalToLocal(pt->xPos,pt->yPos);//x,y);
		//pt->xPos=x/20.0F;
		//pt->yPos=y/20.0F;
		//m_pDisp->GlobalToLocal(cnt,pt->xPos,pt->yPos);
	}
	else 
	{	
		XXObject*pObj=list[0].pObject;
		XXVar varx,vary;
		if(!pObj->GetMember(cnt,_SYSNAME(x),varx,XFALSE)) return;
		if(!pObj->GetMember(cnt,_SYSNAME(y),vary,XFALSE)) return;
		varx.ToFloat();
		vary.ToFloat();
		m_pObject->GlobalToLocal(varx.fData,vary.fData);
		pObj->SetMember(cnt,_SYSVAR(x),varx,XNULL);
		pObj->SetMember(cnt,_SYSVAR(y),vary,XNULL);
	}
}

void XXObjectClip::localToGlobal(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//XNEEDCLIP
	if(list.GetSize()<=0||!list[0].IsObject()) return;//||!list[0].IsObject(XXOBJ_POINT)) return;
	if(list[0].IsObject(XXOBJ_POINT))
	{
		XXObjectPoint*pt=(XXObjectPoint*)list[0].pObject;
		m_pObject->LocalToGlobal(pt->xPos,pt->yPos);
	}
	else 
	{	
		XXObject*pObj=list[0].pObject;
		XXVar varx,vary;
		if(!pObj->GetMember(cnt,_SYSNAME(x),varx,XFALSE)) return;
		if(!pObj->GetMember(cnt,_SYSNAME(y),vary,XFALSE)) return;
		varx.ToFloat();
		vary.ToFloat();
		m_pObject->LocalToGlobal(varx.fData,vary.fData);
		pObj->SetMember(cnt,XXVar(_SYSNAME(x)),varx,XNULL);
		pObj->SetMember(cnt,XXVar(_SYSNAME(y)),vary,XNULL);
	}
}

//DEL XBOOL XXObjectClip::GetMousePos(XSWFCONTEXT&cnt,int &x, int &y)
//DEL {
//DEL 	return cnt.pPlayer->GetMousePos(m_pObject,x,y);
//DEL 	/*x=cnt.nPointX;
//DEL 	y=cnt.nPointY;
//DEL 	x<<=ANTIZOOM;
//DEL 	y<<=ANTIZOOM;
//DEL 
//DEL 	XSWFMatrix mat;
//DEL 	m_pDisp->GetTotalMatrix(mat);
//DEL 	//XSWFMatrix*pMat=m_pDisp->m_pDisp->GetPlaceMatrix();
//DEL 	//if(!pMat)
//DEL 	//	pMat=cnt.pPlayer->GetMatrix();
//DEL 	//if(pMat)
//DEL 		//pMat->RevTrans(x,y);
//DEL 	mat.RevTrans(x,y);
//DEL 	x=x/XSWFTRIP;
//DEL 	y=y/XSWFTRIP;
//DEL 	//XSWFMatrix*pMatrix=m_pDisp->m_pDisp->m_pPlaceMatrix;
//DEL 	//if(pMatrix!=XNULL)// return XFALSE;
//DEL 	//	pMatrix->RevTrans(x,y);	
//DEL 	return cnt.nPointType;*/
//DEL }

XBOOL XXObjectClip::hitTest(XSWFCONTEXT&cnt,XXVARLIST&list)
{
	//return XFALSE;
	int nSize=list.GetSize();
//	if(m_pObject->character->tag==1065)
//		int v=0;
	if(nSize>0)
	{
		//XXObject*pObj=list[0].pObject;
		//XXVar var;
		if(list[0].IsString2())
		{
			XXVar var;
			list[0].ToString(XFALSE);

			if(cnt.pPlayer->GetVariable(cnt,list[0].strTxt,var,XFALSE))
				list[0]=var;
			//str.EmptyNoFree();
		}
		
		if(list[0].IsObject(XXOBJ_CLIP))
		{
//			if(m_pObject->character->tag==8)
//				int v=0;
			XXObjectClip*pObj=(XXObjectClip*)list[0].pObject;
			if(m_pObject->HitObject(pObj->m_pObject)) 
				return XTRUE;
			return XFALSE;
		}
		else if(nSize>1)
		{
			int x=list[0].ToInt();
			int y=list[1].ToInt();
			XBOOL bHit=XFALSE;//XTRUE;
			if(nSize>2)
				bHit=list[2].ToLogic();
			return m_pObject->AS3HitTest(x,y,bHit);   
			//if(var.iData32)
			//	int v=0;
		}
	}
	return XFALSE;
}

XXObject* XXObjectClip::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	pCnt->pStack->PushConst(XOCT_NULL);
	return XNULL;
}






XU32 XXObjectClip::GetIndex(XXVar *pVar, XU32 id,XXVar*pName)
{	
//	return XFALSE;
	//XNEEDCLIP
	/*if(m_pTokens&&id<m_pTokens->GetSize())
	{
		return XXObject::GetIndex(id,*pVar,pName);
	}
	if(m_pDisp==XNULL) return XFALSE;*/
	//if(m_pTokens&&id<
	SObject*pObj=m_pObject->GetChildByIndex(id);
		//m_pDisp->GetIndexOfName(id);
	if(pObj==XNULL) 
		return XFALSE;
	
	*pVar=pObj->pObject;//pDisp->GetObject();
	if(pName)
	{
		if(pObj->name.IsNull())
			*pName=pObj->name;
		
	}	
//	if(pDisp->m_nDepth!=id)
//		int v=0;

	return XTRUE;
}

XBOOL XXObjectClip::GetClipOf(XPCTSTR strName, XXVar&var)
{
	if(m_pObject==XNULL) return XFALSE;
    //if(m_pDisp->GetClipOf(strName,*pVar)) return XTRUE;
	SObject*pObj=m_pObject->GetChildOf(strName,XFALSE);
	if(pObj)
	{
		var=pObj->pObject;
		return XTRUE;
	}
	/*XXObjectClip*p=(XXObjectClip*)GetParent();
	while(p&&p->IsObject(XXOBJ_CLIP)&&p->m_pDisp)
	{
		if(p->m_pDisp->GetClipOf(strName,*pVar)) return XTRUE;
		p=(XXObjectClip*)p->GetParent();
	}*/
	return XFALSE;
}

XXObject* XXObjectClip::GetParent()
{
	if(m_pObject==XNULL) return XNULL;//return m_pContext;
	//XDTSpriteDisp*p=m_pDisp->GetParent();
	SObject*p=m_pObject->parent;
	if(p) return p->pObject;
	else return XNULL;//m_pContext;
//	if(p) return p->GetObject();
//	else  return m_pParent;
}

XU32 XXObjectClip::GetForeColor()
{
	if(m_pObject==XNULL) return 0;
	ScriptThread*pThread=m_pObject->GetThread();
	if(pThread)
		return m_pObject->nForeColor;
	return 0;
	//return m_pObject->GetForeColor();
}

void XXObjectClip::SetForeColor(XU32 color)
{
    XNEEDCLIP
	ScriptThread*pThread=m_pObject->GetThread();
	if(pThread)
		pThread->SetForeColor(color);
	//m_pDisp->SetForeColor(color);
}

void XXObjectClip::GetCXForm(XCXForm &form)
{
	XNEEDCLIP
	m_pObject->GetCXForm(form);
}

void XXObjectClip::SetCXForm(XCXForm &form)
{
	XNEEDCLIP
	m_pObject->SetCXForm(form);
}

void XXObjectClip::GetMatrix(MATRIX &mat)
{
	XNEEDCLIP
	m_pObject->GetMatrix(mat);
}

void XXObjectClip::SetMatrix(MATRIX &mat)
{
	XNEEDCLIP
	m_pObject->SetMatrix(mat);
}

void XXObjectClip::GetPixelRect(SRECT &rect)
{
	XNEEDCLIP
	m_pObject->GetBoundBox(rect,XTRUE);
}

void XXObjectClip::GetTotalMatrix(MATRIX &mat)
{
   XNEEDCLIP
   m_pObject->GetTotalMatrix(mat,XTRUE);
}

void XXObjectClip::SetPixelRect(SRECT &rect)
{
	XNEEDCLIP
	m_pObject->SetBoundBox(rect,XTRUE);
}

void XXObjectClip::GetString(XXVar&var)//XString8 &str)
{
	if(!m_pObject) return;
	XSWFPlayer::GetTargetPath(m_pObject,var);
	//char*path=XSWFPlayer::GetTargetPath(m_pObject);
	//str.SetString(path,STRING_ATTACH);
}

/*void XXObjectClip::AddFunction(XXObjectFunction *pFun)
{
//	pFun->m_pNext=m_pFuns;
//	m_pFuns=pFun;
} */

/*void XXObjectClip::RemoveFunction(XXObjectFunction *pFun)
{
//	if(pFun->m_strClassName=="Tween")
//		int v=0;
	XXObjectFunction**pp=&m_pFuns;
	for(;;)
	{
		XXObjectFunction*p=*pp;
		if(!p) break;
		if(p==pFun)
		{
			*pp=p->m_pNext;
			break;
		}
		else
			pp=&p->m_pNext;
	}
}  */
