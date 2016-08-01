#include "StdAfxflash.h"
#include "sobject.h"
#include "stags.h"
#include "sstroker.h"
#include "splay.h"
#include "morphinter.h"
#include "splayer.h"
#include "edittext.h"
#include "XFilter.h"
#include "ObjectCreator.h"
#include "XDrawObj.h"
#include "avm2.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DisplayObject.h"
#include "AS3Bitmap.h"
#include "ShellCore.h"
#include "BWShapeToGL.h"
#include "XFontDraw.h"
#include "AS3Video.h"
#include "AS3Camera.h"

#define ShowDbgInfo( )  \
    if( character != NULL && (character->tag == 94 || character->tag == 97 ) ) \
    { \
         LOGWHEREVAL( character->tag ); \
    }

#ifdef _WINEMU
int _nDebugTimes=0;
#endif

/*extern RColor* CreateEditTextColor(int red,
							int green,
							int blue,
							int alpha,
							int layer,
							STransform* x,
							SObject* obj,EditText*edit);
extern void AddRect(P_SRECT rect,
			 P_MATRIX mat, RColor* color,
			 DisplayList *display, REdge** edgeList,
			 SStroker* stroker);*/

#ifdef EDITTEXT
BOOL PlayerIsFontAvailable(const char *fontName);

void ResolveFontName(char *result, const char *fontName)
{
	const char *ptr;
	if (!strcmp(fontName, SANS_NAME)) {
		ptr = SANS_DEVICE_NAME;
	} else if (!strcmp(fontName, SERIF_NAME)) {
		ptr = SERIF_DEVICE_NAME;
	} else if (!strcmp(fontName, TYPEWRITER_NAME)) {
		ptr = TYPEWRITER_DEVICE_NAME;
 	} else if (!strcmp(fontName, GOTHIC_NAME)) {
		if (PlayerIsFontAvailable(GOTHIC_DEVICE_NAME_1)) {
			ptr = GOTHIC_DEVICE_NAME_1;
		} else {
			ptr = GOTHIC_DEVICE_NAME_2;
		}
	} else if (!strcmp(fontName, GOTHIC_MONO_NAME)) {
		ptr = GOTHIC_MONO_DEVICE_NAME;
	} else if (!strcmp(fontName, MINCHO_NAME)) {
		ptr = MINCHO_DEVICE_NAME;
 	} else {
 		ptr = fontName;
 	}
 	strcpy(result, ptr);
}
#endif


/*RColor* CreateClipColor(DisplayList* display, RColor** colorList)
{
	RColor* color = display->CreateColor();
	if ( !color ) {
		//FLASHASSERT(XFALSE);
		return 0;
	}
	color->SetUp(display->pRaster);//&display->raster);

	color->nextColor = *colorList;
	*colorList = color;

	color->order = 0;

	// A solid color
	color->colorType = colorClip;

	color->transparent = XTRUE;

	// Build the pattern here also
	if ( display->pRaster->bits)//display->raster.bits )
		color->BuildCache();

	return color;
}*/







//
// The Object Methods
//

_XSObject::_XSObject()
{
	parent = XNULL;
	above = XNULL;
}

void _XSObject::FreeCacheAll()
{
	FreeCache();
	//if(this==debugObj)
	//		int v=0;
	::RectSetEmpty(&this->devBounds);
	_XSObject*pObj=this->bottomChild;
	while(pObj)
	{
		pObj->FreeCacheAll();
		pObj=pObj->above;
	}
}

void _XSObject::FreeCache()
{
	//if(character&&character->tag==42)
	//	int v=0;
	FreeShapeData();
	this->mShapeRecord.ReleaseAll();
	if(character)
	{
		//switch(character->type)
		//{
		////case bitsChar:
		//case morphShapeChar:
		//	 character->record.ReleaseAll();
		//	 break;
		//	 break;
		////case bitmapChar:
		////case bitmapEmptyChar:
		////case loaderImageChar:
		////	 if(pASObject)
		////	 {
		////		 BitmapObject* pBmp =(BitmapObject*)pASObject;
		////		 if(pBmp->m_pBitmapData&&pBmp->m_pBitmapData->m_pChar)
		////		 {
		////		 //pBmp->m_pBitmapData->RestoreIfNeed();
		////			SCharacter* ch = pBmp->m_pBitmapData->m_pChar;
		////			ch->record.ReleaseAll();
		////		}
		////	 }
		////	 break;
		//}
	}
	//if(character)
	//	ScriptThread::RemoveCharacter(character);
	//mShapeRecord.ReleaseAll();

	//if(character&&character->tag==717)
	//	int v=0;
	/*if(character)
	{
		if(pASObject&&(character->type==buttonChar||
		   character->type==buttonEmptyChar))
		{
			_XSObject*obj=display->player->m_pAVM2->GetButtonObject(pASObject,bsHitTest,avmshell::XAS3_BUT_HIT);;
			if(obj)
				obj->FreeCache();
		}
	}*/
	

	
	//if(scrollObject)
	//	scrollObject->FreeCache();
	/*if(gridObjects)
	{
		_XSObject*obj=gridObjects;
		while(obj)
		{
			obj->FreeCache();
			obj=obj->above;
		}
	}*/
}

void _XSObject::Release()
{
	//if ( character )
	FreeCacheBmp();
	mShapeRecord.ReleaseAll();
	{
		switch(tagType)//character->type)
		{
		case editTextEmptyChar:
			//character->player->FreeCharacter(character);
		case textLineChar:
		case editTextChar:
			if(editText)
			{
				editText->Release();
				display->FreeEditText(editText);
			}
			
			break;
		case bitmapChar:
			{
				//((XXObjectBitmap*)character->data)->FreeObject();
				//character->player->FreeCharacter(character);
				//character=XNULL;
			}break;
		case buttonChar:
		case bitmapEmptyChar:
		case buttonEmptyChar:
		//case editTextEmptyChar:
		case shapeEmptyChar:
		case videoEmptyChar:
			 //character->player->FreeCharacter(character);
			 break;
		case rootCloneChar:
		case spriteEmptyChar:
			//character->player->FreeCharacter(character);
		case spriteChar:
			if (thread ) {
				//delete thread;
				thread->~ScriptThread();
				display->FreeThread(thread);
				thread = 0;
			}break;
		case spriteExternalChar:
			if(thread)
			{
				thread->rootObject = 0;         
				delete thread->player;
				thread = 0;
			}break;
		case ctagExternalThreadRoot:
			{
				//character->player->FreeCharacter(character);
				//character = NULL;
			}break;
		case ctagSpritePlaceholder:
			{
				//character->player->characterAlloc.Free(character);
				character = NULL;
			}break;
		}
	}

	dropTarget=NULL;//.Release();
	//FreeStr(dropTarget);
	//dropTarget = NULL;
	//FreeCache();
	FreeFilter();
	name.Release();
}

void _XSObject::FreeShapeData()
{
	if(!mpShapeList) return;
	if(character)
	{
		/*if(character->record.m_pImage)
		{
			character->record.m_pImage->useCount--;
			character->record.m_pImage->nTimer = XSWFPlayer::m_pInstance->m_nRunTime;
			//XSWFPlayer::m_pInstance->RemoveShapeImage(character);
		}*/
		ScriptThread::RemoveCharacter(this);
	}
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	GLShapeListData* pList = mpShapeList;
	while(pList)
	{
		
		//GLShapeListData* pList = mpShapeList;
		/*if(pList->matrix)
			pAlloc->FreeRamp(pList->matrix);
		if(pList->colorTransform)
			pAlloc->FreeRamp(pList->colorTransform);*/
		GLShapeData*pData = pList->pData;
		while(pData)
		{
			GLShapeData* pNext = pData->pNext;
			if(pData->matrix)
				pAlloc->FreeRamp(pData->matrix);
			//if(pData->color)
			//	XXVar::_FreeData(pData->color);
			if(pData->pData&&pData->pData->type==GLT_FONT)
			{
				GLReleaseChar(pData->pData->pChar);
				//pNext->pData->pChar->nCount--;
			}
			pAlloc->FreeShapeData(pData);
			pData = pNext;
		}
		GLShapeListData* pNext = pList->pNext;
		pAlloc->FreeShapeList(pList);
		pList = pNext;
	}
	mpShapeList = XNULL;
}

void _XSObject::FreeContext(bool bFinal)
{
	FreeShapeData();
	mShapeRecord.ReleaseAll();

	if(character)
	{
	//	if(character->tag==140)
	//		int v=0;
		switch(character->type)
		{
		case bitsChar:
			 //character->bits.UnlockBits();
			 break;
		case rootCloneChar:
		case spriteEmptyChar:
		case rootChar:
		case spriteExternalChar:
		case spriteChar:
			 OnEvent(0,display->player,XOBJEVENT_UNLOAD,XNULL);
			 break;
		}
	}
#ifdef EDITTEXT

//#if (__CORE_VERSION__>=0x02070200)
	if (IsEditText()) {
//#else
//	if (character && character->type == editTextChar) {
//#endif
		SPlayer* splayer = display->player;
			//character->GetPlayer()->splayer;
		if (splayer && splayer->focus == this) {
			splayer->focus = NULL;
		}
		//delete editText;
		if(editText&&!m_bDestroying)
		{
			editText->~EditText();
			display->FreeEditText(editText);
		}
		editText = 0;
	}
	else if(character&&character->type==textLineChar)
	{
		if(editText&&!m_bDestroying)
		{
			editText->~EditText();
			display->FreeEditText(editText);
		}
	}
#endif
	FreeObject(bFinal);
	FreeCache();
	FreeFilter();
	
	if(scrollRect)
	{
		display->FreeRect(scrollRect);
		scrollRect=NULL;
	}
	if(gridRect)
	{
		display->FreeRect(gridRect);
		gridRect=NULL;
	}
	FreeCacheBmp();
	/*if(gridObjects)
	{
		_XSObject*obj=gridObjects;
		while(obj)
		{
			obj->FreeContext(XTRUE);
			_XSObject*next=obj->above;
			display->FreeObject(obj);
			obj=next;
		}
		gridObjects=NULL;
	}*/
	//FreeCharacter();
//	FreeAction();
//	#ifdef FSSERVER
	if ( character )
	{
	#ifdef FSSERVER
	if ( character )
		character->player->ReleaseData(name);
	#endif
#ifdef DRAGBUTTONS
		//if ( character->player) 
		{
			SPlayer* splayer = display->player;//character->player->splayer;
			if (splayer && splayer->dragObject == this) {
				splayer->dragObject = NULL;
				splayer->dragStarted = XFALSE;
			}
		}
#endif
		switch(character->type)
		{
		case bitmapChar:

	//if(character && character->type==bitmapChar)
			{
				//delete (character->data;
				if(character->data)
					((XXObjectBitmap*)character->data)->FreeObject();
				display->player->FreeCharacter(character);
				//delete character;
				
				character=XNULL;
			}break;
		/*case drawChar:

			//if(character && character->type==drawChar)
			{
				//delete (XDrawObj*)character->data;
				XDrawObj::FreeCommands(character->cmds.head);
				delete character;
				character=XNULL;
			}break;*/
		case buttonChar:


	//if (character && character->type == buttonChar) 
			{
				SPlayer* splayer = display->player;//character->player->splayer;
				if (splayer) {
					if (splayer->currentButton.button == this) {
						splayer->currentButton.button = NULL;
						if (display) {
							display->InvalidateRect(&display->tabFocusRect);
							RectSetEmpty(&display->tabFocusRect);				
						}
					}
					if (splayer->m_lastButton.button == this) {
						splayer->m_lastButton.button = NULL;
					}
				}
			}break;
		
		case editTextChar:
			if(character&&character->tag==0)
			{
				XSWFPlayer::m_pInstance->FreeCharacter(character);
			}
			break;
		//case grid9Char:
		case textLineChar:
		case scrollChar:
		case bitmapEmptyChar:
		case buttonEmptyChar:
		case editTextEmptyChar:
		case shapeEmptyChar:
		case videoEmptyChar:
			 display->player->FreeCharacter(character);
			 character=NULL;
			 break;
		case rootCloneChar:
		case spriteEmptyChar:
			//delete character;
			display->player->FreeCharacter(character);
			character=NULL;
		//case spriteEmptyChar:
		case spriteChar:
			if (thread ) {
				//delete thread;
				thread->~ScriptThread();
				display->FreeThread(thread);
				thread = 0;
			}break;
		case spriteExternalChar:
			if(thread)
			{
				// we are dealing with a sprite that was 
				// loaded externally so delete the whole
				// scriptplayer
				//
				// note: since player inherits from thread
				//          the delete below deletes
				//          ->player and thread and the character 
				//          dictionary for the scriptplayer

				// avoid recursion, because when we try to
				// delete the player below it will try to clear
				// the script and that in turn will try to delete
				// the rootObject which one and the same as this
				// object
				thread->rootObject = 0; 
        
				delete thread->player;
				thread = 0;
			}break;
		case ctagExternalThreadRoot:
			{
				// this character was created by us and so is not in
				// the char dictionary so we must delete it ourselves
				// also it was created using new so use delete to 
				// destroy it
				//delete character;
				display->player->FreeCharacter(character);
				character = NULL;
			}break;
		case ctagSpritePlaceholder:
			{
				// this character was created by us and so is not in
				// the char dictionary so we must delete it ourselves
				display->player->FreeCharacter(character);
				character = NULL;
			}break;
		}
	}

	dropTarget=NULL;//.Release();
	//FreeStr(dropTarget);
	//dropTarget = NULL;
	name.Release();
	thread=NULL;
	//FreeStr(name);
//	name = NULL;
	
//	if(sorts)
//	{
//		sorts->Release();
//		delete sorts;
//	}
//	sorts.Release();
}

void _XSObject::GetName(XString16&name)
{
	if(this->name.IsNull())
	{
		if(pASObject)
		{
			display->player->m_pAVM2->GetName(pASObject,name);
		}
	}
	else
	{
		name=this->name.strTxt;
		if(character)
		{
			if(character->className)
			{
				XString8 s(character->className);
				//s.ConvertToGB();
				name+=XString16("(");
				name+=XString16(s);
				name+=XString16(")");
			}
			else if(pASObject)
			{
				XString16 s;
				display->player->m_pAVM2->GetName(pASObject,s);
				name+=XString16("(");
				name+=s;
				name+=XString16(")");
			}
		}
	}
}

void _XSObject::CalcDevBounds(MATRIX* mat,SRECT*clipRect)
{
	// Calculate the device matrix
//#ifdef _WINEMU
//	if(character&&character->tag==2&&parent&&parent->character&&parent->character->tag==3)
//		int v=0;
//#endif
	if ( !character ) {
		RectSetEmpty(&devBounds);
	} else {
		//if(this==debugObj)
		//	int v=0;
		if ( character->type == morphShapeChar ) {
			SRECT b;
			Interpolate(&character->rbounds, &character->morph.bounds2, ratio, &b);
			MatrixTransformRect(mat, &b, &devBounds);
			//if((exflags&0x40000000)!=0)
			//	int v=0;
		} else {
			/*if(character->type == bitsChar&&RectIsEmpty(&character->bounds))
			{
				display->player->CreateIfEmpty(character);
				if(character->bits.HasBits())
				{
					int nDiv=character->bits.nImgDiv;
					character->bounds.xmin=0;
					character->bounds.ymin=0;
					character->bounds.xmax=character->bits.width<<nDiv;
					character->bounds.ymax=character->bits.height<<nDiv;
				}
			}*/
			SRECT&rr=GETBOUNDS(character,editText);
			MatrixTransformRect(mat, &rr, &devBounds);	
			//if((exflags&0x40000000)!=0)
			//	int v=0;
		}
		/*if(clipRect)
		{
			::RectIntersect(clipRect,&devBounds,&devBounds);
		}*/
	}

}

void _XSObject::SetBackColor(XU32 nColor)
{

	if(nBackColor==nColor||!character) return;
	nBackColor=nColor;
	SRECT&rr=GETBOUNDS(character,editText);
	if(RectIsEmpty(&rr))
	{
		MATRIX mat;
		::MatrixIdentity(&mat);
		GetBoundingBox(this,&mat,&rr);
	}
		//GetSpriteBound(&character->bounds);
	Modify();
}

/*inline bool IsDispose(_XSObject*obj)
{
	if(obj->pASObject)
	{
		BitmapObject*bmp=(BitmapObject*)obj->pASObject;
		if(bmp->m_pBitmapData.value()==XNULL) return true;
		if(!bmp->m_pBitmapData->m_pChar&&
			!bmp->m_pBitmapData->m_pDib) return true;
	}
	return false;
}*/

void _XSObject::BuildChar(ScriptPlayer*player,XBOOL bTess)
{
	_ANALYENTER("Build Character Shapes(with Tess)");
	switch(character->type)
	{
	case bitmapChar:
	case loaderImageChar:
	case bitmapEmptyChar:
		 if(pASObject)
		 {
			 BitmapObject* pBmp =(BitmapObject*)pASObject;
			 if(!pBmp->m_pBitmapData||!pBmp->m_pBitmapData->m_pChar)
				 return;			 
			 //pBmp->m_pBitmapData->RestoreIfNeed();
			 SCharacter* ch = pBmp->m_pBitmapData->m_pChar;
			 if(ch->record.pFirst)
				 return;
			 pBmp->m_pBitmapData->CreateTexture();
			 XDrawObj obj(player,&ch->record);
			 obj.BuildEdges(ch);
		 }
		 break;
	case videoEmptyChar:
		{//break;
			 SBitmapCore* bits = &character->bits;
			 if(bits->HasBits())
			 {
				 XDrawObj obj(player,&character->record);
				 obj.BuildEdgesCamera(bits);
			 }
		}break;
	case bitsChar:
		 {
			 SBitmapCore* bits = &character->bits;
			 if(!bits->bitmapID&&!bits->HasBits()&&character->data)
				 XSWFPlayer::m_pInstance->CreateIfEmpty(character);
			 XDrawObj obj(player,&character->record);
			 obj.BuildEdges(character);
		 }
		 break;
	case morphShapeChar:
		//break;
		//if(false)
		//break;
		{
		//if(pClip) break;
			// Build the edges based on the "ratio"
			//return;
			//if(character->record.pFirst)
			//{
			//	character->record.ReleaseAll();
			//}//	return;
			SCharacterParser parser(character->splayer, character->data, 0, &mShapeRecord);//&character->record);
			parser.getAlpha = (character->tagCode == stagDefineMorphShape||
				               character->tagCode == stagDefineMorphShape2);
			parser.lineStyle2=character->tagCode == stagDefineMorphShape2;
			parser.ratio = ratio;
			//parser.obj = pClip?pClip:this;
			parser.BuildMorphEdges();
		}break;
	case shapeChar:
		{

			//return;
			if(bTess)
			{
				if(character->record.pFirst)
					return;
			}
			else if(character->record.mpPaths)
				return;
			SCharacterParser parser(player, character->data, 0, &character->record);
			if((character->shape.flags)&0x04)
				parser.useWinding=XTRUE;

			//if(character->tag==249)
			//	int v=0;
			parser.getAlpha = 
				character->tagCode >= stagDefineShape3;
			parser.lineStyle2 =
				character->tagCode >= stagDefineShape4;
			//parser.cxform = x->cxform;
			//parser.obj = pClip?pClip:this;
			//parser.info = info;
			parser.BuildEdges(XTRUE,bTess);
			//XSWFPlayer::m_pInstance->SetDataFlag();
		}break;
	}
	//if(character->record.IsEmpty()) return;
	//TessPloy*tess=TessPloy::GetInst();
	//TessPloy::Tess(&character->record);
}

void _XSObject::AddFrame(SRECT&rect,MATRIX&mat,float*color,GLShapeListData*pList,BwShapeRecord*pRec,bool bFill,GLShapeData**pLast)
{
	//return;
	GLTranglesOrLines*pTL = CreateFrame(rect,color,bFill);
	pRec->AddData(pTL);
	GLShapeData*pData = _XSObject::CreateShapeData(&mat,NULL,pTL);
	_XSObject::GLSetShapeListData(pList,pData,pLast);
}

GLTranglesOrLines* _XSObject::CreateFrame(SRECT&rect,float*color,bool bFill)
{
	ShapeAlloc*pAlloc=ShapeAlloc::GetInst();
	//GLShapeData* pData=pAlloc->AllocShapeData();

	GLTranglesOrLines* pFL = pAlloc->AllocTrangleOrLines();
	if(bFill)
		pFL->type = GLT_SIMPLERECT;
	else
		pFL->type = GLT_SIMPLEFRAME;
	pFL->nCount = 0;
	GLSimpleRect* pRect = pAlloc->AllocSimpleRect();
	pRect->pos[0]= rect.xmin;
	pRect->pos[1]= rect.ymin;
	pRect->pos[2]= rect.xmax;
	pRect->pos[3]= rect.ymax;
	memcpy(pRect->color,color,sizeof(float)*4);
	//pRect->color[0] = rgb.rgb.red/255.0;
	//pRect->color[1] = rgb.rgb.green/255.0;
	//pRect->color[2] = rgb.rgb.blue/255.0;
	//pRect->color[3] = rgb.rgb.transparency/255.0;
	pFL->rect = pRect;
	return pFL;
}

GLShapeData* _XSObject::CreateShapeData(MATRIX*pMatrix,float*color,GLTranglesOrLines*pFL)
{
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	GLShapeData* pData = pAlloc->AllocShapeData();
	pData->pData = pFL;
	if(pMatrix)//&&!::MatrixIsIdentity(pMatrix))
	{
		pData->matrix = pAlloc->AllocRamp();
		RasterGL::SetGLMatrix(*pMatrix,pData->matrix);
	}
	if(color)
	{
		pData->color[0] = color[0];
		pData->color[1] = color[1];
		pData->color[2] = color[2];
		pData->color[3] = color[3];
	}
	return pData;
}

void _XSObject::GLSetShapeData(GLShapeData* pData,GLTranglesOrLines*pFL,GLTranglesOrLines**pLast)
{
	pFL->pNext = NULL;
	if(pLast==NULL)
	{
		if(pData->pData == NULL)
			pData->pData = pFL;
		else
		{
			GLTranglesOrLines*pSet = pData->pData;
			while(pSet->pNext)
				pSet = pSet->pNext;
			pSet->pNext = pFL;
		}
	}
	else
	{
		if(!*pLast)
		{
			*pLast = pFL;
			pData->pData = pFL;
		}
		else
		{
			(*pLast)->pNext = pFL;
			*pLast = pFL;
		}
	}
}

void _XSObject::GLSetShapeListData(GLShapeListData*pList,GLShapeData*pData,GLShapeData**pLast)
{
	if(!pList) return;
	pData->pNext = NULL;
	if(!pLast)
	{
		if(pList->pData == NULL)
			pList->pData = pData;
		else
		{
			GLShapeData*pSet=pList->pData;
			while(pSet->pNext)
				pSet = pSet->pNext;
			pSet->pNext = pData;
		}
	}
	else
	{
		if(!*pLast)
		{
			*pLast = pData;
			pList->pData = pData;
		}
		else
		{
			(*pLast)->pNext = pData;
			*pLast = pData;
		}
	}
}

GLShapeListData* _XSObject::CreateShapeList(STransform* x,_XSObject*pObj,bool bMatrix)
{
	ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	//GLShapeData* pData = pAlloc->AllocShapeData();
	//pData->pData = character->record.pFirst;
	GLShapeListData* pList = pAlloc->AllocShapeList();
	//pList->pData = pData;
	if(x->cxform.HasTransform())
	{
		pList->colorTransform = pAlloc->AllocRamp();
		RasterGL::SetGLTransform(x->cxform,pList->colorTransform);
	}
	if(bMatrix&&!MatrixIsIdentity(&x->mat))
	{
		pList->matrix = pAlloc->AllocRamp();
		RasterGL::SetGLMatrix(x->mat,pList->matrix);
	}
	pList->blendMode=x->cxform.nBlendMode;//pObj->blendMode;
	if(!pObj->mpShapeList)
		pObj->mpShapeList = pList;
	else
	{
		GLShapeListData* pL = pObj->mpShapeList;
		while(pL->pNext)
			pL = pL->pNext;
		pL->pNext = pList;
	}
	return pList;
}
/*static GLShapeListData* CreateShapeList(STransform* x,_XSObject*pObj,ShapeAlloc*pAlloc,bool bMatrix=true)
{
	//ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
	//GLShapeData* pData = pAlloc->AllocShapeData();
	//pData->pData = character->record.pFirst;
	GLShapeListData* pList = pAlloc->AllocShapeList();
	//pList->pData = pData;
	if(x->cxform.HasTransform())
	{
		pList->colorTransform = pAlloc->AllocRamp();
		RasterGL::SetGLTransform(x->cxform,pList->colorTransform);
	}
	if(bMatrix&&!MatrixIsIdentity(&x->mat))
	{
		pList->matrix = pAlloc->AllocRamp();
		RasterGL::SetGLMatrix(x->mat,pList->matrix);
	}
	if(!pObj->mpShapeList)
		pObj->mpShapeList = pList;
	else
	{
		GLShapeListData* pL = pObj->mpShapeList;
		while(pL->pNext)
			pL = pL->pNext;
		pL->pNext = pList;
	}
	return pList;
}*/

void _XSObject::GLReleaseChar(GLChar*pChar)
{
	RasterGL::UnLockTexture(pChar->id);
	//if(!pChar->id) return;
	//if(pChar->id->nLockCount)
	//	pChar->id->nLockCount--;
	//pChar->nCount--;
	//if(pChar->nCount>0) return;
	/*SCharacter*font = (SCharacter*)pChar->ch;//&XSWFPlayer::m_pInstance->buildInFont;
	XU32 t = XSWFPlayer::m_pInstance->m_nRunTime;
	if(font->font.deviceState!=textDync)
	{
			GLTranglesOrLines** pList = &font->font.shapeData[pChar->g];
			while(*pList)
			{
				GLTranglesOrLines*pTL = *pList;
				GLTranglesOrLines**pNext = &pTL->pNext;
				if(pTL->pChar->nCount<=0&&t-pTL->pChar->nLastT>MAXCACHET)
				{
					*pList = pTL->pNext;
					pTL->pNext = NULL;
					BwShapeRecord::ReleaseShapeList(pTL);
				}
				else pList = pNext;
			}
	}*/
}

void _XSObject::GLUseChar(GLChar*pChar)
{
	RasterGL::LockTexture(pChar->id);
	RasterGL::SetLastTime(pChar->id,XSWFPlayer::m_pInstance->m_nRunTime);
	//if(!pChar->id) return;
	//pChar->id->nLockCount++;
	//XU32 t=XSWFPlayer::m_pInstance->m_nRunTime;
	//pChar->id->nLastTime = t;
}
#define _FONTBLOCKW 512
#define _FONTBLOCK _FONTBLOCKW*_FONTBLOCKW*4
static XU8 _fontData[_FONTBLOCK];

static inline void AttachDIB(XDrawDib&dib,int w,int h)
{
	//dib.m_bAttach = true;
	dib.m_nBits=32;//nBits=8;
	dib.m_nPixelSize=4;
	dib.m_nLinePitch=((w*dib.m_nPixelSize+3)>>2)<<2;
	dib.m_nWidth=w;
	dib.m_nHeight=h;
	dib.m_nDataLength=dib.m_nLinePitch*h;
	dib.m_pDib=_fontData;
	dib.m_nMemSize=dib.m_nDataLength;
	dib.Reset();
}
static inline void PackALPHA(XDib&dib)
{
	XU8* pDst=(XU8*)dib.m_pDib;
	RGB8* pRgb=(RGB8*)dib.m_pDib;
	int i,t=dib.m_nWidth*dib.m_nHeight;
	for(i=0;i<t;i++)
	{
		pDst[i]=pRgb[i].alpha;
		//pRgb++;
		//pDst++;
	}
}

GLTranglesOrLines* _XSObject::BuildFontChar(SCharacter*font,int g,char*fontName,int nFontID,float fSize)
{
	//return NULL;
	if(g>=font->font.nGlyphs) return NULL;

	//if(g==36213)
	//	int v=0;
	/*if(font->font.flags&XDOT_FONT12)
	{
		GLTranglesOrLines* pFL = ShapeAlloc::GetInst()->AllocTrangleOrLines();
		pFL->nCount = 0;
		pFL->type = GLT_FONT12;
		pFL->fontID = RasterGL::sharedRasterGL()->texInitWithData(font->font.infoData+(g*12*2),kCCTexture2DPixelFormat_PVRTC2,12,12);
		font->font.shapeData[g]=pFL;
	}
	else if(font->font.flags&XDOT_FONT16)
	{
		GLTranglesOrLines* pFL = ShapeAlloc::GetInst()->AllocTrangleOrLines();
		pFL->nCount = 0;
		pFL->type = GLT_FONT16;
		pFL->fontID = RasterGL::sharedRasterGL()->texInitWithData(font->font.infoData+(g*16*2),kCCTexture2DPixelFormat_PVRTC2,16,16);
		font->font.shapeData[g]=pFL;
	}
	else */
	if(font->font.deviceState==textDync)
	{
		if(fSize<=0) return NULL;
		GLTranglesOrLines* pTL=NULL,*pSet=NULL;// = font->font.shapeData[g];
		GLTranglesOrLines**pList = &font->font.shapeData[g];
		XU32 t=XSWFPlayer::m_pInstance->m_nRunTime;
		int fontSize=(int)(fSize+0.5);
		//int fs=fSize*20,best=fs/2;
		while(*pList)
		{
			pTL = *pList;
			GLTranglesOrLines**pNext = &pTL->pNext;
			GLChar* pChar=pTL->pChar;
			//int df=pChar->h-fs;
			if(pChar->fontID==nFontID&&fontSize==pChar->fontSize)
			   //df>=0&&fs>=pChar->h/2&&df<=best)
			{
				pSet = pTL;
				//pList = pNext;
				//best=df;
				//int df=fs-pChar->h;
			}
			/*else if(pTL->pChar->nCount<=0&&t-pTL->pChar->nLastT>MAXCACHET)
			{
				*pList = pTL->pNext;
				pTL->pNext = NULL;
				BwShapeRecord::ReleaseShapeList(pTL);
			}
			else*/
			pList = pNext;

		}
		/*pList = &font->font.shapeData[g];
		while(*pList)
		{
			pTL = *pList;
			GLTranglesOrLines**pNext = &pTL->pNext;
			if(pTL!=pSet&&pTL->pChar->nCount<=0&&t-pTL->pChar->nLastT>MAXCACHET)
			{
				*pList = pTL->pNext;
				pTL->pNext = NULL;
				BwShapeRecord::ReleaseShapeList(pTL);
			}
			else pList = pNext;
		}*/
		//if(!font->font.shapeData[20248])
		//	int vvv=0;
		if(pSet) return pSet;

		_FONTINFO info={0,0,0,NULL,false};
		XFontDraw*pDraw=XFontDraw::GetInst();
		
		pDraw->SetFont(fontName,nFontID,fontSize);
		U16 txt[]={g,0};
		pDraw->Draw(txt,1,info);
		if(info.pData)
		{
			ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
			_TEXTUREID* id=RasterGL::sharedRasterGL()->texInitWithData(info.pData,Texture2DPixelFormat_A8,info.w,info.h);
			id->type = 2;
			if(info.bAlloc)
				delete info.pData;
			pTL = pAlloc->AllocTrangleOrLines();
			pTL->nCount = 0;
			pTL->type = GLT_FONT;
			GLChar* pChar = pAlloc->AllocChar();
			pChar->ch = font;
			pChar->fontID = nFontID;
			pChar->w = info.w*20;
			pChar->h = info.h*20;
			pChar->fontSize = fontSize;
			float delta=(info.fw-fSize)/fSize;
			if(delta<0) delta=-delta;
			pChar->fw = g>256&&delta<0.2?fSize*20:info.fw*20*fSize/fontSize;
			pChar->g = g;
			pChar->offx = 0;
			pChar->offy = 0;
			pChar->id = id;
			//pChar->nCount = 0;
			pTL->pChar = pChar;
			pTL->pNext = font->font.shapeData[g];
			font->font.shapeData[g] = pTL;
			//if(pTL->pNext==pTL)
			//	int v=0;
		}
		//if(!font->font.shapeData[20248])
		//	int vvv=0;
		return pTL;
	}
	//if(font->font.shapeData[g])
	//		return font->font.shapeData[g];
	//else
	{
		if(fSize<=0) return NULL;
		GLTranglesOrLines* pTL=NULL,*pSet=NULL;// = font->font.shapeData[g];
		GLTranglesOrLines**pList = &font->font.shapeData[g];
		XU32 t=XSWFPlayer::m_pInstance->m_nRunTime;
		int fs=fSize*20,best=fs/2;
		while(*pList)
		{
			pTL = *pList;
			GLTranglesOrLines**pNext = &pTL->pNext;
			GLChar* pChar=pTL->pChar;
			int df=pChar->h-fs;
			if(fs==pChar->fontSize)//df>=0&&fs>=pChar->h/4&&df<=best)
			{
				pSet = pTL;
				best=df;
				//int df=fs-pChar->h;
			}
			pList = pNext;

		}
		pList = &font->font.shapeData[g];
		while(*pList)
		{
			pTL = *pList;
			GLTranglesOrLines**pNext = &pTL->pNext;
			_TEXTUREID* tID = pTL->pChar->id;
			if(pTL!=pSet&&!RasterGL::IsLocked(tID)&&t-RasterGL::GetLastTime(tID)>MAXCACHET)//pTL!=pSet&&pTL->pChar->nCount<=0&&t-pTL->pChar->nLastT>MAXCACHET)
			{
				*pList = pTL->pNext;
				pTL->pNext = NULL;
				BwShapeRecord::ReleaseShapeList(pTL);
			}
			else pList = pNext;
		}

		if(pSet) return pSet;

		CRaster* raster=&XSWFPlayer::m_pInstance->display.raster;
		XDrawDib dib;
		int fw=((int)fSize+3)/4*4;
		bool bAttach = false;
		if(fw<_FONTBLOCKW)
		{
			AttachDIB(dib,fw,fw);
			bAttach = true;
		}
		else dib.Create(fw,fw,32);

		//dib.ResetAlpha(0x7f);

		XBitBuffer bits;
		bits.bitmap=&dib;
		SRECT rect={0,fw,0,fw};//rc.left,rc.right,rc.top,rc.bottom};

		

		U8* s;
		S32 glyphOffset;
		if(font->font.flags & sfontFlagsWideOffsets) 
		{
			s = font->data + 4*g;
			glyphOffset = (U32)s[0] | ((U32)s[1]<<8) | ((U32)s[2]<<16) | ((U32)s[3]<<24);
		} 
		else 
		{
			s = font->data + 2*g;
			glyphOffset = (U16)s[0] | ((U16)s[1]<<8);
		}
		
		
		SRECT r;
		{
			SCharacterParser parser1(font->splayer, font->data, glyphOffset,NULL);
			parser1.GetFontBound(r);
			r.xmin-=80;
			r.ymin-=80;
		}
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.a = mat.d = (fixed_1*fw)/1024;
		float div=20.0f;
		if(font->tagCode<stagDefineFont3)
			div=1.0f;
		int st=raster->antiShift;
		if(st) div=div/(1<<st);
		mat.a = mat.d = mat.a/div;
		mat.tx = (float)-r.xmin*mat.d/fixed_1;
		mat.ty = (float)-r.ymin*mat.d/fixed_1;
		//mat.tx = (fw/2)<<st;//fw/2;
		//mat.ty = mat.tx;//fw/2;
		SCharacterParser parser(font->splayer, font->data, glyphOffset,NULL);
		parser.tagCode=font->tagCode;
		parser.useWinding = XTRUE;
		//BOOL getStyles, SRECT* devBounds,CRaster*r,MATRIX&m
		raster->Attach(&bits,&rect,raster->antialias_b,raster->antiShift);
		raster->getBackground=XTRUE;
		raster->BeginPaint();

		RColor color;
		color.SetUp(raster);
		color.order = 0;
		color.rgb.alpha = 0xff;
		color.rgb.red   = 0xff;
		color.rgb.green = 0xff;
		color.rgb.blue  = 0xff;
		color.BuildCache();
		parser.fillIndex[0] = 0;
		parser.lineIndex[0].color = NULL;
		parser.lineIndex[0].thickness = 0;
		parser.fillIndex[1] = &color;
		parser.nFills = 1;

		parser.BuildRasterEdges(XFALSE,&rect,raster,mat);
		if(parser.pEdges)
		{
			raster->AddEdges(parser.pEdges,parser.pColors,NULL);
			raster->PaintBits();
			PackALPHA(dib);
		}

		{
			ShapeAlloc*pAlloc = ShapeAlloc::GetInst();
			_TEXTUREID* id=RasterGL::sharedRasterGL()->texInitWithData(dib.m_pDib,Texture2DPixelFormat_A8,fw,fw);
			id->type = 2;
			pTL = pAlloc->AllocTrangleOrLines();
			pTL->nCount = 0;
			pTL->type = GLT_FONT;
			GLChar* pChar = pAlloc->AllocChar();
			pChar->fontID = 0;
			pChar->w = fw*20;
			pChar->fontSize = fs;
			pChar->offx = r.xmin;
			pChar->offy = r.ymin;
			pChar->h = fw*20;
			pChar->fw = fSize*20;
			pChar->g = g;
			pChar->id = id;
			pChar->ch = font;
			//pChar->nCount = 0;
			pTL->pChar = pChar;
			pTL->pNext = font->font.shapeData[g];
			font->font.shapeData[g] = pTL;
			//if(pTL->pNext==pTL)
			//	int v=0;
		}

		bits.bitmap = NULL;
		parser.ReleaseData();
		if(bAttach)
			dib.m_pDib = NULL;
		
		//if(!font->font.shapeData[20248])
		//	int vvv=0;
		return pTL;
		/*BwShapeRecord rec;
		rec.Init();
		rec.SetFills(2);
		rec.AddFillSyle(0,XNULL);
		rec.AddFillSyle(1,&XSWFPlayer::m_pInstance->fontColor);

		U8* s;
		S32 glyphOffset;
		if(font->font.flags & sfontFlagsWideOffsets) 
		{
			s = font->data + 4*g;
			glyphOffset = (U32)s[0] | ((U32)s[1]<<8) | ((U32)s[2]<<16) | ((U32)s[3]<<24);
		} 
		else 
		{
			s = font->data + 2*g;
			glyphOffset = (U16)s[0] | ((U16)s[1]<<8);
		}
		
		SCharacterParser parser(font->splayer, font->data, glyphOffset,&rec);
		parser.tagCode=font->tagCode;
		parser.useWinding = XTRUE;

		parser.nFills = 1;
		parser.BuildEdges(XFALSE);
		font->font.shapeData[g] = rec.pFirst;
		
		rec.pFirst = XNULL;
		rec.mnFills = 0;
		rec.mVecFillStyles = XNULL;
		XSWFPlayer::m_pInstance->SetDataFlag();
		return font->font.shapeData[g];*/
	}
}

static bool GetFontData(SCharacter*font,int g,BwShapeRecord&rec)
{
	if(g>=font->font.nGlyphs) return false;

	if(font->font.deviceState==textDync)
	{
		return false;
	}
	{
		rec.Init();
		rec.SetFills(2);
		rec.AddFillSyle(0,XNULL);
		rec.AddFillSyle(1,&XSWFPlayer::m_pInstance->fontColor);

		U8* s;
		S32 glyphOffset;
		if(font->font.flags & sfontFlagsWideOffsets) 
		{
			s = font->data + 4*g;
			glyphOffset = (U32)s[0] | ((U32)s[1]<<8) | ((U32)s[2]<<16) | ((U32)s[3]<<24);
		} 
		else 
		{
			s = font->data + 2*g;
			glyphOffset = (U16)s[0] | ((U16)s[1]<<8);
		}
		
		SCharacterParser parser(font->splayer, font->data, glyphOffset,&rec);
		parser.tagCode=font->tagCode;
		parser.useWinding = XTRUE;

		parser.nFills = 1;
		parser.BuildEdges(XFALSE,false);

		return true;
	}
}

inline static bool UpdateTexture(ScriptPlayer*player,_GLColor*pColor,XU32 nTimer)
{
	/*if(pColor->colorType!=glColorBitmap) return false;
	SBitmapCore*bm=pColor->bm.bitmap;
	if(!bm||!bm->m_pCH) return false;
	if(bm->bitmapID) return false;
	//if(!bm->HasBits())
	//	player->BuildBits((SCharacter*)bm->m_pCH);
	//if(!bm->HasBits()) return false;
	//bm->CreateTexture();
	//bm->ReleaseData();
	return true;*/
	switch(pColor->colorType)
	{
	case glColorBitmap:
		if(!pColor->bm.bitmap) return true;
		if(!pColor->bm.bitmap->bitmapID) 
		{
			if(!pColor->bm.bitmap->m_pCH) return true;
			SCharacter*ch = (SCharacter*)pColor->bm.bitmap->m_pCH;
			if(!ch->splayer) return true;
			XSWFPlayer::m_pInstance->CreateIfEmpty(ch);
			if(pColor->bm.bitmap->bitmapID)
				return false;
			return true;
		}
		/*{
			if(!pColor->bm.bitmap->m_pCH) return true;
			SCharacter*ch = (SCharacter*)pColor->bm.bitmap->m_pCH;
			if(!ch->splayer) return true;
			ch->splayer->BuildBits(ch);
			if(!ch->bits.HasBits()) return true;
			ch->bits.CreateTexture();
			ch->bits.ReleaseData();
			//ch->bits.m_nLockTime = nTimer;
			//return false;
		}
		pColor->bm.bitmap->m_nLockTime = nTimer;*/
		break;
	case glColorDib:
		if(!pColor->dib.bitmapID) return true;
		break;
	case glFrameBuffer:
		if(!pColor->frame.frame) return true;
		break;
	default:
		 return false;
	}
	return false;
}

inline static bool CheckRecord(ScriptPlayer*player,BwShapeRecord*pRec)
{
	BwFillStyle** fills=pRec->mVecFillStyles;
	BwLineStyle** lines=pRec->mVecLineStyles;
	int i;
	_GLColor*pColor;
	XU32 nTimer=XSWFPlayer::m_pInstance->m_nRunTime;
	for(i=0;i<pRec->mnFills;i++)
	{
		if(!fills[i]) continue;
		pColor=fills[i];
		if(UpdateTexture(player,pColor,nTimer))
			return true;
	}
	for(i=0;i<pRec->mnLines;i++)
	{
		if(!lines[i]||!lines[i]->color) continue;
		pColor=lines[i]->color;
		if(UpdateTexture(player,pColor,nTimer))
			return true;
	}
	return false;
}

void _XSObject::BuildRasterObj(_XSCharacter*character,MATRIX&mat,bool bSave)
{
		//return;
			//int v=0;
		//if(character->tag==45)
		//	int v=0;
		bool isComplex = false;
		ScriptPlayer*rootPlayer=character->splayer;
		if(character->record.m_nSampleType==1)
			isComplex = true;
		else
		{
			//ScriptPlayer*rootPlayer=character->splayer;
			if(!rootPlayer)
			{
				character->record.m_nSampleType=2;
				return;
			}
			SCharacterParser parser(rootPlayer, character->data, 0, &character->record);
			if((character->shape.flags)&0x04)
				parser.useWinding=XTRUE;

			//if(character->tag==249)
			//	int v=0;
			parser.getAlpha = 
				character->tagCode >= stagDefineShape3;
			parser.lineStyle2 =
				character->tagCode >= stagDefineShape4;
			if(parser.IsComplex())
				isComplex = true;
		}
		if(isComplex)
		{
			_ANALYENTER("Shape Raster(Old)");
			if(!bSave)
			{
				if(character->record.pFirst)
					character->record.ReleaseAll();
				//BuildChar(rootPlayer,XFALSE);
			}
			character->record.m_nSampleType = 1;
			MATRIX cMat,tMat;
			//if(!bSave)
			{
				XSWFPlayer::m_pInstance->display.GetCameraMatrix(cMat);
				::MatrixConcat(&mat,&cMat,&tMat);
				tMat.c = tMat.b = 0;
				tMat.tx = 0;
				tMat.ty = 0;
				if(tMat.a<0) tMat.a = -tMat.a;
				if(tMat.d<0) tMat.d = -tMat.d;
				if(tMat.a<cMat.a) tMat.a = cMat.a;
				if(tMat.d<cMat.d) tMat.d = cMat.d;
			}
			//else
			//	tMat = mat;
			
			int div=20;
			CRaster* raster=&XSWFPlayer::m_pInstance->display.raster;
			int st=raster->antiShift;
			if(st) div>>=st;
			tMat.a = tMat.a/div;
			tMat.d = tMat.d/div;
			SRECT r;
			::MatrixTransformRect(&tMat,&character->rbounds,&r);
			r.xmin-=5;
			r.ymin-=5;
			r.xmax+=5;
			r.ymax+=5;
			tMat.tx = -r.xmin;
			tMat.ty = -r.ymin;
			MATRIX vMat=tMat;
			if(st)
			{
				vMat.a=(vMat.a)*div;
				vMat.d=(vMat.d)*div;
				vMat.tx=(vMat.tx)*div;
				vMat.ty=(vMat.ty)*div;
			}
			
			int w=RectWidth(&r);
			int h=RectHeight(&r);
			if(st)
			{
				w>>=st;h>>=st;
			}
			if(w*h<1024*1024)
			{
				//if(w==258&&h==166)
				//	int v=0;
				bool bAttach = false;
				XDrawDib dib;
				if(w*h*4<_FONTBLOCK)
				{
					AttachDIB(dib,w,h);
					bAttach = true;
				}
				else dib.Create(w,h,32);


				XBitBuffer bits;
				bits.bitmap=&dib;
				dib.Clear(0);//0xff00ff7f);
				//dib.ResetAlpha(0x7f);
				SRECT rect={0,w,0,h};//rc.left,rc.right,rc.top,rc.bottom};		
				
				SCharacterParser parser2(rootPlayer, character->data, 0, &character->record);
				if((character->shape.flags)&0x04)
					parser2.useWinding=XTRUE;

			//if(character->tag==249)
			//	int v=0;
				parser2.getAlpha = 
					character->tagCode >= stagDefineShape3;
				parser2.lineStyle2 =
					character->tagCode >= stagDefineShape4;
				raster->Attach(&bits,&rect,raster->antialias_b,raster->antiShift);
				raster->getBackground=XFALSE;//XTRUE;
				raster->BeginPaint();


				parser2.BuildRasterEdges(XTRUE,&rect,raster,tMat);
				if(parser2.pEdges)
				{
					raster->AddEdges(parser2.pEdges,parser2.pColors,NULL);
					raster->PaintBits();
				}
				::MatrixInvert(&vMat,&tMat);
				//if(!bSave)
				{
					dib.SwapRB();
					character->record.SetImage(dib,tMat,bSave);
					if(!bSave)
					{	XSWFPlayer::m_pInstance->AddShapeImage(character);
					//else
						XSWFPlayer::m_pInstance->m_bDataProc = true;
					}
				}


				bits.bitmap = NULL;
				parser2.ReleaseData();
				if(bAttach)
					dib.m_pDib = NULL;

				
			}
			else
				character->record.m_nSampleType = 2;
			
		}
		else 
			character->record.m_nSampleType = 2;
		//XSWFPlayer::m_pInstance->AddShapeImage(character);
}

static inline void _UnCompress(_XSCharacter*character)
{
	BwShapeRecord&rec=character->record;
	if(!rec.m_pImage||rec.m_pImage->id) return;
	if(!rec.m_imgData.GetSize()) return;
	
	XImage image;
	image.Append(rec.m_imgData.GetData(),rec.m_imgData.GetSize());
	if(!image.Final(true)) return;
	
	int w=image.Width(),h=image.Height();

	XDrawDib dib;
	bool bAttach = false;
	if(w*h*4<_FONTBLOCK)
	{
		AttachDIB(dib,w,h);
		bAttach = true;
	}
	else dib.Create(w,h,32);

	image.GetBitmapData((XU8*)dib.GetBuffer(),w,h,32,w*h*4);
	rec.m_pImage->id = RasterGL::sharedRasterGL()->texInitWithData(dib.GetBuffer(),Texture2DPixelFormat_RGBA8888,w,h);

	if(bAttach)
		dib.m_pDib = NULL;
	XSWFPlayer::m_pInstance->AddShapeImage(character);
}

void _XSObject::BuildEdges(STransform* x,_XSObject*pClip,bool bClip)//,_GRIDINFO*info,_XSObject*pClip,XU32 nFilterColor)//bool bFilterRender)
{
	
	if(!character) return;

	switch(character->type)
	{
		case bitmapChar:
		case loaderImageChar:
		case bitmapEmptyChar:
			 if(pASObject)
			 {
				//character->record.ReleaseAll();
				 BitmapObject* pBmp =(BitmapObject*)pASObject;
				 if(!pBmp->m_pBitmapData||!pBmp->m_pBitmapData->m_pChar)
				 {
					character->record.ReleaseAll();
					 this->FreeCache();
					 return;
				 }
			 }
			 else
			 {
				 character->record.ReleaseAll();
				 this->FreeCache();
				 return;
			 }
			 break;
	}
	/*if(character->type==bitmapEmptyChar&&pASObject)
	{
			BitmapObject* pBmp =(BitmapObject*)pASObject;
			if(pBmp->m_pBitmapData)
			{
				if(pBmp->m_pBitmapData->m_pChar->bits.width==3200)
				{
					int v=0;
					int c=v+1;
				}
			}
				//pBmp->m_pBitmapData->CreateTexture();
	}*/

	_ANALYENTER("Build Edges");
	bool bFirst=!character->record.pFirst;//IsEmpty();
	ScriptPlayer* rootPlayer = character->splayer;
	if(!rootPlayer||!rootPlayer->display) rootPlayer=CreatePlayer;//return;
	if(character->type==videoEmptyChar)
	{
		XDrawObj draw(rootPlayer,&character->record);
		draw.BuildEdgesCamera(&character->bits);
	}

	
	//if(!rootPlayer||!rootPlayer->display) return;
	//if(pClip&&pClip->colors) return;
	//if(!pClip&&parent&&parent->clipDepth)
	//{
	//	int v=0;
	//}
	//if(pClip)
	if(CheckRecord(rootPlayer,&character->record))
	{
		character->record.ReleaseAll();
		bFirst = true;
	}
	if(!pClip&&mpShapeList) 
	{
		//CheckRecord(rootPlayer,&character->record);
		if(character->type==bitmapEmptyChar&&pASObject)
		{
			BitmapObject* pBmp =(BitmapObject*)pASObject;
			if(pBmp->m_pBitmapData)
				pBmp->m_pBitmapData->CreateTexture();
		}
		return;
	}
	//if(character->tag==65528)
	//	int v = 0;
	//if(character->record.IsEmpty())
	//	BuildChar();
	//if(!character->record.pFirst)
	//	return;
	ShapeAlloc* pAlloc = ShapeAlloc::GetInst();
	switch ( character->type ) {
		case textLineChar:
		case editTextEmptyChar:
		case editTextChar:
			if(editText)
			{
				_ANALYENTER("Edit Texts");
				editText->Draw(x, XTRUE,pClip?pClip:this,this);//,nFilterColor);//bFilterRender);
			} 
			break;
		/*case videoEmptyChar:
			if(pASObject)
			{
				VideoObject*pVideo=(VideoObject*)pASObject;
				CameraObject*pCamera=pVideo->GetAttachCamera();
				if(pCamera&&pCamera->m_pDib)
				{
					XDrawObj draw(rootPlayer,&character->record);
					draw.BuildEdgesCamera(pCamera->m_pDib);
				}
				if(!character->record.pFirst)
					return;
				GLShapeListData* pL = CreateShapeList(x,pClip?pClip:this);
				GLShapeData* pData = pAlloc->AllocShapeData();
				pData->pData = character->record.pFirst;
				pL->pData = pData;
			}
			break;*/
		case bitmapChar:
		case loaderImageChar:
		case bitmapEmptyChar:
			//break;
			if(pASObject)
			{
				BitmapObject* pBmp =(BitmapObject*)pASObject;
				if(pBmp->m_pBitmapData&&pBmp->m_pBitmapData->m_pChar)
				{
					//pBmp->m_pBitmapData->RestoreIfNeed();
					pBmp->m_pBitmapData->CreateTexture();
					SCharacter* ch = pBmp->m_pBitmapData->m_pChar;
					if(!ch->record.pFirst)
						BuildChar(rootPlayer,XTRUE);
					else
						bFirst = false;
					if(ch->record.pFirst)
					{
						GLShapeListData* pL = CreateShapeList(x,pClip?pClip:this);
						GLShapeData* pData = pAlloc->AllocShapeData();
						pData->pData = ch->record.pFirst;
						pL->pData = pData;
					}
				}
			}
			break;
		case textChar:
			//if(pClip) break;
			//break;
			{
				_ANALYENTER("Static Texts");
				MATRIX cMat;
				XSWFPlayer::m_pInstance->display.GetCameraMatrix(cMat);
				GLShapeListData* pL = CreateShapeList(x,pClip?pClip:this);
				GLShapeData* pLast = NULL;
				SParser parser;
				parser.Attach(character->data, 0);
				BOOL getAlpha = character->tagCode == stagDefineText2;

				MATRIX mat;
				parser.GetMatrix(&mat);
				MatrixConcat(&mat, &x->mat, &mat);

				int nGlyphBits = parser.GetByte();
				int nAdvanceBits = parser.GetByte();
				/*int tempClipDepth=pClip?pClip->clipDepth:clipDepth;
				RColor* color = 0,*filterColor=0;
				if ( tempClipDepth ) 
				{
					RColor *colorList = pClip?pClip->colors:colors;//&obj->colors
					if(colorList&&(colorList)->colorType == colorClip)
						color=colorList;
					else
						color = CreateClipColor(display, pClip?&pClip->colors:&colors);
				}*/

				U32 layer = 0;
				int n = 0;
				MATRIX charMat;
				MatrixIdentity(&charMat);
				SRGB rgb;
				SCharacter* font = 0;
				//XBOOL bFont3=XFALSE,bSet=XFALSE;
				float color[4]={0,0,0,1};
				while ( XTRUE ) {
					if ( n == 0 ) {
						// Get a new run
						int code = parser.GetByte();
						if ( !code ) break;

						if ( code & tflagsFont ) {
							font = character->splayer->FindCharacter(parser.GetWord());
						}
						if ( code & tflagsColor ) {
							// Set up a new color
							rgb = parser.GetColor(getAlpha);
							color[0] = rgb.rgb.red/255.0;
							color[1] = rgb.rgb.green/255.0;
							color[2] = rgb.rgb.blue/255.0;
							if(getAlpha)
								color[3] = rgb.rgb.transparency/255.0;
						}
						if ( code & tflagsX ) {
							charMat.tx = (S16)parser.GetWord();
						}
						if ( code & tflagsY ) {
							charMat.ty = (S16)parser.GetWord();
						}
						if ( code & tflagsHeight ) {
							//if(bFont3)
							//	charMat.a = charMat.d = parser.GetWord() * (fixed_1/(1024*20));
							//else
								charMat.a = charMat.d = parser.GetWord() * (fixed_1/1024);
						}

						n = parser.GetByte();
						parser.InitBits();
					}

					{// Handle a glyph
						int g = (int)parser.GetBits(nGlyphBits);
						S32 advance = parser.GetSBits(nAdvanceBits);

						if ( font&&g<font->font.nGlyphs ) {
							//U8* s;
							MATRIX m;
							if(font->tagCode==stagDefineFont3)
							{
								MATRIX mc=charMat;
								mc.a=mc.a/20;
								mc.d=mc.d/20;
								MatrixConcat(&mc,&mat,&m);
							}
							else
								MatrixConcat(&charMat, &mat, &m);
							//if(!font->font.shapeData[g])
							//	BuildFontChar(font,g,NULL,0,0);
							MATRIX tm;
							::MatrixConcat(&m,&cMat,&tm);
							float fSize = 1024;
							int a=tm.a;
							if(a<0) a=-a;
							int d=tm.d;
							if(d<0) d=-d;
							a=(a>d?a:d);
							fSize = (float)fSize*a/fixed_1;
							if(font->tagCode!=stagDefineFont3)
								fSize = fSize/20;

							//tm.tx = -512;
							
							GLTranglesOrLines*pFL = BuildFontChar(font,g,NULL,0,fSize);//font->font.shapeData[g];
							if(pFL)
							{
								MatrixIdentity(&tm);
								if(font->tagCode!=stagDefineFont3)
									fSize=pFL->pChar->fw;
								else
									fSize = pFL->pChar->fw/20;
								tm.a = tm.d = (float)1024*fixed_1/fSize;
								tm.tx = pFL->pChar->offx;
								tm.ty = pFL->pChar->offy;
								::MatrixConcat(&tm,&m,&m);
								/*GLShapeData* pData = pAlloc->AllocShapeData();
								if(pLast == NULL)
								{	pL->pData = pLast = pData;	}
								else 
								{	pLast->pNext = pData;
									pLast = pData;	}
								pData->pData = pFL;
								memcpy(pData->color,color,sizeof(color));
								pData->matrix = pAlloc->AllocRamp();
								RasterGL::SetGLMatrix(m,pData->matrix);*/
								//pFL->pChar->nCount++;
								GLUseChar(pFL->pChar);
								GLShapeData* pData = _XSObject::CreateShapeData(&m,color,pFL);

								if(pLast == NULL)
								{	pL->pData = pLast = pData;	}
								else 
								{	pLast->pNext = pData;
									pLast = pData;	}
								//_XSObject::GLSetShapeListData(pList,pData,&pLast);
							}
						}
						charMat.tx += advance;
					}
					n--;
				}
			}
			break;
		case morphShapeChar:
			{
				mShapeRecord.SetMeter(x->mat);
				if(!mShapeRecord.pFirst)
					mShapeRecord.ReleaseAll();
				if(mShapeRecord.IsEmpty())
					BuildChar(rootPlayer,XTRUE);
				if(mShapeRecord.pFirst)
				{
					GLShapeListData* pL = CreateShapeList(x,pClip?pClip:this);
					GLShapeData* pData = pAlloc->AllocShapeData();
					pData->pData = mShapeRecord.pFirst;
					pL->pData = pData;
				}
			}break;
		case shapeChar:
			//if(character->tag==10)
			//	int v=0;
			if(!bClip&&character->splayer&&character->record.m_nSampleType!=2)
			{
				
				if(!character->record.m_pImage)
				{
					/*if(character->record.m_imgData.GetSize())
					{
						if(character->record.pFirst)
							character->record.ReleaseAll();
						BuildChar(rootPlayer,XFALSE);
						character->record.UnCompress();
					}
					else*/
					{
						BuildRasterObj(character,x->mat,false);
						if(character->record.m_pImage)
						{
							BuildChar(rootPlayer,XFALSE);
							//XSWFPlayer::m_pInstance->m_bDataProc = true;
						}
					}
				}
				else if(!character->record.m_pImage->id)
				{
					if(character->record.pFirst)
							character->record.ReleaseAll();
					BuildChar(rootPlayer,XFALSE);
					_UnCompress(character);
				}
				if(character->record.m_pImage)
				{
					if(!character->record.pFirst)
					{
						 XDrawObj obj(rootPlayer,&character->record);
						 obj.BuildEdgesImage(character);
						 
						 //obj.BuildEdges(ch);
					}
					if(character->record.pFirst)
					{
						//character->record.m_pImage->useCount++;
						//character->record.m_pImage->nTimer = XSWFPlayer::m_pInstance->m_nRunTime;
						if(character->record.m_pImage->id)
						{
							_TEXTUREID*tID = character->record.m_pImage->id;
							RasterGL::LockTexture(tID);
							RasterGL::SetLastTime(tID,XSWFPlayer::m_pInstance->m_nRunTime);
							//tID->nLockCount++;
							//tID->nLastTime = XSWFPlayer::m_pInstance->m_nRunTime;
						}
						STransform x1=*x;
						::MatrixConcat(&character->record.m_pImage->mat,&x1.mat,&x1.mat);
						GLShapeListData* pL = CreateShapeList(&x1,pClip?pClip:this);
						GLShapeData* pData = pAlloc->AllocShapeData();
						pData->pData = character->record.pFirst;
						pL->pData = pData;
					}
					
					break;
				};
			}
		
		default:
			{
				character->record.SetMeter(x->mat);
				if(!character->record.pFirst)
					character->record.ReleaseAll();
				if(character->record.IsEmpty())
					BuildChar(rootPlayer,XTRUE);
				if(character->record.pFirst)
				{
					GLShapeListData* pL = CreateShapeList(x,pClip?pClip:this);
					GLShapeData* pData = pAlloc->AllocShapeData();
					pData->pData = character->record.pFirst;
					pL->pData = pData;
				}
			}break;

		case spriteChar:
		case rootChar:
		case rootCloneChar:
		case spriteEmptyChar:
		case spriteExternalChar:
			//if(pClip) break;
			if(display->player->m_pAVM2==NULL&&thread)
				thread->BuildEdges(rootPlayer,&character->record);
			if(character->record.pFirst)
			{
				GLShapeListData* pL = CreateShapeList(x,pClip?pClip:this);
				GLShapeData* pData = pAlloc->AllocShapeData();
				pData->pData = character->record.pFirst;
				pL->pData = pData;
			}
			break;
	}
	if(pASObject&&IsDisplayObject())
	{
		_XCOMMAND*pCmds=display->player->m_pAVM2->GetCommands(pASObject);
		if(pCmds)
		{
			if(mShapeRecord.IsEmpty())
			{
				mShapeRecord.ReleaseAll();
				XDrawObj draw(rootPlayer,&mShapeRecord);
				//draw.obj=pClip?pClip:this;
			//draw.cxform=x->cxform;
				draw.BuildEdges(pCmds);
			}
			if(mShapeRecord.pFirst)
			{
				GLShapeListData* pL = CreateShapeList(x,pClip?pClip:this);
				GLShapeData* pData = pAlloc->AllocShapeData();
				pData->pData = mShapeRecord.pFirst;
				pL->pData = pData;
			}
		}
	}
	if(mpShapeList)
	{
		//if(bFirst)
		//	character->placeCount++;
		//else
			ScriptThread::PlaceCharacter(this,bFirst);
	}
}

int _XSObject::GetDrawCmds()
{
	if(pASObject&&IsDisplayObject())
	{
		_XCOMMAND*pCmds=display->player->m_pAVM2->GetCommands(pASObject);
		int nCount=0;
		while(pCmds)
		{
			nCount++;
			pCmds=pCmds->pNext;
		}
		return nCount;
	}
	return 0;
}


#ifdef EDITTEXT
void _XSObject::EditTextMouse(SPOINT* pt, BOOL mouseIsDown)
{
	if(editText)
		editText->DoMouse(pt, mouseIsDown);
}

void _XSObject::DblClick(SPOINT*pt)
{
	if(editText)
	{
//#if  (__CORE_VERSION__>=0x02070200)
		if(IsEditText())
//#else
//		if(character->type==editTextChar)
//#endif
			editText->DblClick(pt);
	}
}

BOOL _XSObject::DrawEditText(STransform* x)
{

	if(editText)
		editText->Draw(x, XFALSE,this,this);

	return XTRUE;
}

//DEL BOOL _XSObject::ScrollEditText(/*ScriptThread *thread, XPCTSTR name,*/ int operation, int& value)
//DEL {
//DEL 	if (character && character->type == editTextChar) {
//DEL 		SPlayer* splayer = character->player->splayer;
//DEL 
//DEL 		// Get the ScriptThread enclosing this edit text object
//DEL 		//ScriptThread* editTextThread = this->thread->rootObject->thread;
//DEL 
//DEL 		//char *varName;
//DEL 		//ScriptThread* targetThread = splayer->ResolveVariable(editText->m_variable,
//DEL 		//													  editTextThread, &varName,XNULL);
//DEL 
//DEL 		//if (targetThread == thread && StrEqual(varName, name)) 
//DEL 		{
//DEL 			// Found it!
//DEL 			switch (operation) {
//DEL 			case editTextGetScroll:
//DEL 				value = editText->m_vscroll + 1;
//DEL 				break;
//DEL 			case editTextSetScroll:
//DEL 				{
//DEL 					int maxScroll = editText->CalcMaxVScroll();
//DEL 					value--;
//DEL 					if (value < 0) {
//DEL 						value = 0;
//DEL 					} else if (value > maxScroll) {
//DEL 						value = maxScroll;
//DEL 					}
//DEL 					editText->m_vscroll = value;
//DEL 					Modify();
//DEL 				}
//DEL 				break;
//DEL 			case editTextGetMaxScroll:
//DEL 				value = editText->CalcMaxVScroll() + 1;
//DEL 				break;
//DEL 			}
//DEL 			return XTRUE;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	/*for ( _XSObject* obj = bottomChild; obj; ) {
//DEL 		if (obj->ScrollEditText(thread, name, operation, value)) {
//DEL 			return XTRUE;
//DEL 		}
//DEL 		obj = obj->above;
//DEL 	}*/
//DEL 	return XFALSE;
//DEL }

/*void _XSObject::UpdateEditText(ScriptThread *thread, XPCTSTR name, XPCTSTR value, BOOL updateFocus)
{
	if (character) {
		if (character->type == editTextChar) {

			if (updateFocus || character->player->splayer->focus != this) {
				SPlayer* splayer = character->player->splayer;

				// Get the ScriptThread enclosing this edit text object
				ScriptThread* editTextThread = this->thread->rootObject->thread;

				char *varName;
				ScriptThread* targetThread = splayer->ResolveVariable(editText->m_variable,
																	  editTextThread, &varName);

				if (targetThread == thread && !stricmp(varName, name)) {
					editText->SetBuffer(value);
					Modify();
				}
			}
		}
	}

	for ( _XSObject* obj = bottomChild; obj; ) {
		obj->UpdateEditText(thread, name, value, updateFocus);
		obj = obj->above;
	}
}*/

#endif

//SBitmapCore* _XSObject::GetBits()
//{
//	if(!character) return NULL;
//	switch(character->type)
//	{
//		case loaderImageChar:
//			{
//				if(!character->bits.HasBits())
//				{
//					if(!character->data&&character->length)
//						display->player->m_pAVM2->SetActive(character);
//					character->bits.PICreate(character->data,character->length);
//				}
//				return &character->bits;
//				//obj.BuildEdges(&devBounds,&character->bits);
//			}
//			break;
//		case bitsChar:
//			if(!AS3IsReplace())
//			{
//				if(!character->bits.HasBits())
//				{
//					character->splayer->BuildBits(character);
//				}
//				return &character->bits;
//				//obj.BuildEdges(&devBounds,&character->bits);
//				//break;
//			}			
//		case bitmapEmptyChar:
//			if(pASObject)
//			{				
//			   SBitmapCore* sbitmap=NULL;
//			   BitmapObject* pBmp = (BitmapObject*)pASObject;
//			   if(pBmp&&pBmp->m_pBitmapData&&pBmp->m_pBitmapData->m_pChar)
//				   sbitmap = &pBmp->m_pBitmapData->m_pChar->bits;
//				   //display->player->m_pAVM2->GetDrawDib(pASObject,(void**)&sbitmap);//pBitmap->GetBitmap();
//			   if(sbitmap)
//			   {
//			    //if(x->cxform.aa==128)
//				//	int v=0;
//			    if(!sbitmap->HasBits())
//					break;
//				return sbitmap;				
//			   }
//			}
//			//else 
//			//	int v=0;
//			break;
//
//		case bitmapChar:
//			return &character->bits;
//			/*if(character->data)
//			{
//			   XXObjectBitmap*pBitmap=(XXObjectBitmap*)(character->data);
//			   XDrawDib*pDib=pBitmap->GetBitmap();
//			   if(pDib)
//			   {
//					return &pBitmap->m_sbitmap;
//				//obj.BuildEdges(&devBounds,&pBitmap->m_sbitmap);
//			   }
//			}*/
//			break;
//	}
//	return  NULL;
//}

BOOL _XSObject::HitTestOne(STransform* x, SPOINT* pt,XBOOL bTest,XBOOL bClip)
{
	//return TRUE;
	//return true;
	// If this is a text-edit object and it hit, focus on the
	// text-edit.
	//MATRIX cmat,mat;
	//XSWFPlayer::m_pInstance->display.GetCameraMatrix(cmat);
	//::MatrixConcat(&mat,&x->mat,&mat);
	MATRIX mat = x->mat;
	if (character ) 
	{
		//if(character->type==32)
			//int v=0;
		switch(character->type)
		{
		//case textChar:
		//	 break;
		case scrollChar:
			 return RectPointIn(&devBounds, pt);
		case textLineChar:
		case editTextChar:
		case editTextEmptyChar:
			if(editText&&(editText->IsEditAble()||bTest ))
			{
				if (bTest) 
				{
					MATRIX invmat;
					SPOINT localpt;

					MatrixInvert(&mat, &invmat);
					MatrixTransformPoint(&invmat, pt, &localpt);
					

					if (RectPointIn(&editText->editRect, &localpt)) 
					{
						if(!editText->IsEditAble()&&editText->m_length==0)
							break;
						if(!editText->IsEditAble())
						{
							//editText->TranslatePoint((int&)localpt.x,(int&)localpt.y);
							int nChar=editText->GetCharIndextAtPoint(localpt.x,localpt.y);
							if(nChar>=0) return XTRUE;
							return XFALSE;
						}
						return XTRUE;
					}
				}
			}
			else if(!editText||editText->m_length==0)
				return XFALSE;
			break;
		case buttonChar:
		case buttonEmptyChar:
			if(pASObject)
			{
				avmshell::XAVM2*pAVM2=display->player->m_pAVM2;
				SObject*pObject=pAVM2->GetButtonObject(pASObject,bsHitTest,avmshell::XAS3_BUT_HIT);
				if(pObject==NULL)
					pObject=pAVM2->GetButtonObject(pASObject,state,avmshell::XAS3_BUT_HIT);
				if(pObject&&pObject->AS3HitTest(*x,pt,bTest) )
				{
					return XTRUE;
				}
				return XFALSE;
			}
			else if(character->data)
			{
				ScriptPlayer* player = character->splayer;
				SParser parser;
				parser.Attach(character->data, 0);

				if ( character->tagCode == stagDefineButton2 ) 
					parser.GetWord();	// skip the action offset

				XBOOL bFirst=XTRUE;
				for (;;) {
					U8 stateFlags = parser.GetByte();
					if ( !stateFlags ) break;

					MATRIX m;
					U16 tag = parser.GetWord();
					U16 d = depth + parser.GetWord();
					parser.GetMatrix(&m);

					ColorTransform cx;
					if ( character->tagCode == stagDefineButton2 )
					{	parser.GetColorTransform(&cx, XTRUE);

						if(stateFlags&sbtnFilterList)
						{
						   XU8 num=parser.GetByte();
						   //XFilter**root=&info.pFilter;
						   for(int i=0;i<num;i++)
						   {
							   XFilter*pFilter=XFilter::CreateFilter(&parser,display->player);
							   if(!pFilter) break;
							   //pFilter->pNext=XNULL;
							   display->player->FreeFilter( pFilter );
						   }
						}
						if(stateFlags&sbtnBlendMode)
						{
							parser.SkipBytes(1);
							//if(!pStream-ReadByte(blendMode)) return XFALSE;
						}
					}
					if ( stateFlags & sbtnHitTestState ) {
						// Check the child
						SCharacter* ch = player->FindCharacter(tag);
						if ( ch ) {
							_XSObject child;

							// Set up object
							memset(&child, 0, sizeof(child));
							child.display = display;
							//child.parent = 0;
							//child.above = 0;
							//child.bottomChild = 0;
							child.character = ch;
							child.depth = d;
							//child.ratio = 0;
							//child.name = 0;
							//child.thread = 0;
							child.xform.mat = m;
							child.xform.cxform.Clear();
							//child.state = 0;
							//child.drawn = 0;
							//child.dirty = 0;
							//child.clipDepth = 0;
							//child.edges = 0;
							//child.colors = 0;
		#if defined(EDITTEXT)
							if (ch->type == editTextChar) {
								//child.editText = new EditText(&child);
								child.editText = display->CreateEditText();//new EditText(obj);
								child.editText->Init(&child);
							}
		#endif

							// Calculate the device matrix and bounds
							STransform cx = *x;
							MatrixConcat(&child.xform.mat, &cx.mat, &cx.mat);
							child.CalcDevBounds(&cx.mat,NULL);

							BOOL hit = XFALSE;
							//if(ch->tag==127)
							//	int v=0;
							if(ch->type==editTextChar||(bFirst&&ch->type==textChar))
							{
								/*if(display->nAspect)
								{
									SRECT erect=child.devBounds;
									::RectInset(-display->nAspect,&erect);
									hit=RectPointIn(&erect,pt);							
								}
								else*/
									hit=::RectPointIn(&child.devBounds,pt);
							}
							else
								hit=child.HitTestOneChild(*x, pt,bClip);//,1);
							child.FreeContext();
							if ( hit ) return true;
							bFirst=XFALSE;
							//break;
						}
					}
				}
			}
			break;
		}
	}
	
	BOOL hit = XFALSE;
	XBOOL bInRect=XFALSE;

	
	bInRect=RectPointIn(&devBounds, pt);
	
	if(!bInRect&&character&&RectIsEmpty(&devBounds))
	{
		SRECT&rr=GETBOUNDS(character,editText);
		if(!RectIsEmpty(&rr))
		{
			SRECT rt;
			::MatrixTransformRect(&mat,&rr,&rt);
			bInRect=RectPointIn(&rt,pt);
		}
	}

	
	/*else if(!RectIsEmpty(&character->bounds))
	{
		SRECT rect;
		::MatrixTransformRect(&x->mat,&character->bounds,&rect);
		bInRect=RectPointIn(&rect,pt);
	}*/
	if ( bInRect ) 
	{
		XBOOL bButton=bTest;
		bool bCmds=false;
		if(pASObject&&IsDisplayObject())
		{
			_XCOMMAND*pCmds=display->player->m_pAVM2->GetCommands(pASObject);
			if(pCmds)
			{
				bTest=0;
				bCmds=true;
			}
		}

		//if(bTest)
		{
			if((character&&character->type==textChar))
				return XTRUE;
		}

		SPOINT localpt;
		/*if(this->pCacheBmp)
		{
				SRECT *pRect = NULL;
				SRECT dr;
				if(gridRect)
				{
					dr = *gridRect;
					pRect=&dr;
					pRect->xmin=pRect->xmin*20-pCacheMat->tx;
					pRect->ymin=pRect->ymin*20-pCacheMat->ty;
					pRect->xmax=pRect->xmax*20-pCacheMat->tx;
					pRect->ymax=pRect->ymax*20-pCacheMat->ty;
					MATRIX mat;
					::MatrixIdentity(&mat);
					mat.tx = pCacheMat->tx;
					mat.ty = pCacheMat->ty;
					::MatrixConcat(&mat,&x->mat,&mat);
					localpt.x = pt->x-mat.tx;
					localpt.y = pt->y-mat.ty;
					float fw=(float)pCacheBmp->mWidth*mat.a/fixed_1;
					float fh=(float)pCacheBmp->mHeight*mat.d/fixed_1;
					int xmin=dr.xmin;
					int xmax=fw*20-(pCacheBmp->mWidth*20-dr.xmax);
					int ymin=dr.ymin;
					int ymax=fh*20-(pCacheBmp->mHeight*20-dr.ymax);
					if(localpt.x>xmin)
					{
						if(localpt.x<xmax)
						{
							localpt.x = (float)(localpt.x-xmin)*fixed_1/mat.a+xmin;
						}
						else
							localpt.x = localpt.x+dr.xmax-xmax;
					}
					if(localpt.y>ymin)
					{
						if(localpt.y<ymax)
							localpt.y = (float)(localpt.y-ymin)*fixed_1/mat.d+ymin;
						else
							localpt.y = localpt.y+dr.ymax-ymax;
					}
					localpt.x/=20;
					localpt.y/=20;
					if(localpt.x>=0&&localpt.x<pCacheBmp->mWidth&&
					   localpt.y>=0&&localpt.y<pCacheBmp->mHeight)
					{
						RGB8 c;
						pCacheBmp->GetPixels(localpt.x,localpt.y,1,1,(XU8*)&c,true);
						if(c.alpha)
							hit = true;
					}
				}
				else
				{
					//filtersize(pFilter,XNULL,&x.mat);
					SPOINT localpt=*pt;
					localpt.x-=pCacheMat->tx;
					localpt.y-=pCacheMat->ty;
					localpt.x/=20;
					localpt.y/=20;
					if(localpt.x>=0&&localpt.x<pCacheBmp->mWidth&&
					   localpt.y>=0&&localpt.y<pCacheBmp->mHeight)
					{
						RGB8 c;
						pCacheBmp->GetPixels(localpt.x,localpt.y,1,1,(XU8*)&c,true);
						if(c.alpha)
							hit = true;
					}
				}
				
		}
		else
		{

		}*/
		BOOL colorsInited = XFALSE;
		MATRIX invmat;//,mat=x->mat;
		//SPOINT localpt;

		MatrixInvert(&mat, &invmat);
		MatrixTransformPoint(&invmat, pt, &localpt);
		//color->bm.bitmap->GetPixel();
		switch(character->type)
		{
			case bitmapChar:
			case loaderImageChar:
			case bitmapEmptyChar:
				 if(pASObject)
				 {
					 BitmapObject* pBmp =(BitmapObject*)pASObject;
					 if(pBmp->m_pBitmapData&&pBmp->m_pBitmapData->m_pChar)
					 {
						 //pBmp->m_pBitmapData->RestoreIfNeed();
						 //pBmp->m_pBitmapData
						 TextureInfo info;
						 pBmp->m_pBitmapData->GetBmInfo(info);
						 localpt.x/=20;
						 localpt.y/=20;
						 if(localpt.x>=0&&localpt.x<=info.w&&
						    localpt.y>=0&&localpt.y<=info.h)
						 {
							 hit = true;
						 }
						//if(pBmp->m_pBitmapData->AS3_getPixel32(localpt.x/20,localpt.y/20))
						//	hit = true;
					 }
					 /*FrameTexture* f=new FrameTexture();
					 f->init(100,100,true);
					 delete f;*/
				 }
				 break;
			case videoEmptyChar:
				{//break;
					 SBitmapCore* bits = &character->bits;
					 if(bits->HasBits())
					 {
						 localpt.x/=20;
						 localpt.y/=20;
						 if(localpt.x>=0&&localpt.x<=bits->width&&
						    localpt.y>=0&&localpt.y<=bits->height)
						 {
							 //RGBI c;
							 //bits->GetRGBPixel(localpt.x,localpt.y,&c);
							 //if(c.alpha||c.red||c.green||c.blue)
							 hit=true;
						 }
						 //XDrawObj obj(player,&character->record);
						 //obj.BuildEdgesCamera(bits);
					 }
				}break;
			case bitsChar:
				 {
					 SBitmapCore* bits = &character->bits;
					 localpt.x/=20;
					 localpt.y/=20;
					 if(localpt.x<0&&localpt.x>bits->width&&
					    localpt.y<0&&localpt.y>bits->height)
						break;
					 hit = true;
					 /*if(bits->HasBits())
					 {
						 RGBI c;
						 bits->GetRGBPixel(localpt.x,localpt.y,&c);
						 if(c.red||c.alpha||c.green||c.blue)
							 hit=true;
						 //XDrawObj obj(player,&character->record);
						 //obj.BuildEdgesCamera(bits);
					 }
					 else if(bits->bitmapID)
					 {
						  FrameTexture frame;
						  frame.init(bits->width,bits->height,true);
						  TextureInfo info={bits->width,bits->height,
											0,true,true,bits->bitmapID};
						  frame.DrawBitmap(info);
						  RGB8 c;
						  frame.GetPixels(localpt.x,localpt.y,1,1,(XU8*)&c,true);
						  if(*((XU32*)&c))
							  hit = true;
					 }*/
					 //XDrawObj obj(player,&character->record);
					 //obj.BuildEdges(character);
				 }
		 break;
		default:
			if(character->record.IsEmpty())
			{
				BuildChar(character->splayer,XFALSE);
			}
			if(mShapeRecord.IsEmpty()&&bCmds)
			{
				_XCOMMAND*pCmds=display->player->m_pAVM2->GetCommands(pASObject);
				if(pCmds)
				{
					mShapeRecord.ReleaseAll();
					XDrawObj draw(character->splayer,&mShapeRecord);
					draw.BuildEdges(pCmds);
				}
			}

			hit = character->record.IsInPaths( localpt.x, localpt.y )||
				  mShapeRecord.IsInPaths(localpt.x,localpt.y);
		}
		//XU32 nColor=0xffffffff;
		
	
	}

	return hit;
}

void _XSObject::FreeChildren(bool bFinal)
{

	_XSObject* obj = bottomChild;
	//bottomChild = 0;
	while ( obj ) {
		_XSObject* next = obj->above;

		if(obj->IsThread())
			display->actionList.RemoveObject(obj);
		// Remove any children
		
		// Free the object
 		if ( obj->drawn )
			display->InvalidateRect(&obj->devBounds);

		if ( obj == display->button )
			display->button = 0;
		//else if(obj==display->button[1])
		//	display->button[1] = 0;

		if(!(bFinal)&&(obj->pASObject||obj->IsRootObject()))//((obj->pASObject||obj->tagType==rootChar)&&!avmshell::m_bDestroying&&obj->thread->player->m_pContext))
		{
			//if(obj->drawn)
			//	display->InvalidateRect(&obj->devBounds);
			//obj->FreeCache();
			
			this->display->AS3RemoveObject(this,obj);

			//obj->AS3SetLoaderInfo(NULL);
			if(obj->pASObject)
			{
				display->player->m_pAVM2->RemoveSystem(obj->pASObject);
				display->player->m_pAVM2->RemoveObject(obj->pASObject);
				if(obj->IsDisplayObject())
					((DisplayObjectObject*)obj->pASObject)->m_bNewObject=true;
			}
			
			//if(obj->pASObject)
			//{
				//((DisplayObjectObject*)obj->pASObject)->m_bNewObject=true;
				//display->player->m_pAVM2->RemoveObject(obj->pASObject);
			//}
		}
		else
		{

			/*if(obj->character&&obj->character->player)
			{
				ScriptPlayer*pl=obj->character->player;
				if(pl->rootObject==obj)
					pl->rootObject=XNULL;
			}*/
			obj->FreeChildren(bFinal);

			obj->FreeContext(bFinal);
			obj->parent=XNULL;
			display->FreeObject(obj);
		}

		obj = next;
	}
	bottomChild = 0;
}

void _XSObject::Modify()
{
	display->dirty = XTRUE;
	//if(this->pCacheBmp)
	//	int v=0;
	dirty = XTRUE;
	if(this->clipObject)
		clipObject->Modify();
	if(this->clipDepth!=0&&parent)
		parent->dirty=XTRUE;
	/*_XSObject*obj=parent;
	if(obj)
	{
		if(obj->pCacheBmp)
		{
			obj->dirty=true;
			obj->pCacheBmp->m_invalid=true;
		}
		obj=obj->parent;
	}*/
	/*if(parent)
	{
		//if(clipDepth)
		//	parent->Modify();
		//else 
		if(clipObject&&clipObject->parent!=parent)
		{
			clipObject->Modify();
			//parent->Modify();
		}
	}*/
}

void _XSObject::Modify1()
{
	display->dirty = XTRUE;
	dirty1 = XTRUE;
	if(this->clipObject)
		clipObject->Modify();
	if(this->clipDepth!=0&&parent)
		parent->dirty=XTRUE;
}

void _XSObject::CalcDrawUpdate(MATRIX *m,SRECT*clipRect,XBOOL bMat)
{
	FreeCache();
	MATRIX mat;
	if(bMat)
		::MatrixConcat(&xform.mat,m,&mat);
	else
		mat=*m;
	SRECT*pRectc=clipRect,rectc={0,0};
	if(scrollRect)
	{
		MATRIX mats;
		::MatrixIdentity(&mats);
		mats.tx=-scrollRect->xmin;
		mats.ty=-scrollRect->ymin;
		rectc.xmax=RectWidth(scrollRect);
		rectc.ymax=RectHeight(scrollRect);
		::MatrixTransformRect(&mat,&rectc,&rectc);
		pRectc=&rectc;
		if(clipRect)
			::RectIntersect(clipRect,pRectc,pRectc);
		::MatrixConcat(&mats,&mat,&mat);
	}
	CalcDevBounds(&mat,pRectc);
	/*if(character&&pASObject)
	{
		switch(character->type)
		{
		case buttonEmptyChar:
		case buttonChar:
			 {
				 SObject*pObject=character->player->m_pAVM2->
								 GetButtonObject(pASObject,state);
				 if(pObject)
					pObject->CalcDrawUpdate(&mat,XTRUE);
			 }
			 break;
		}
	}*/

	for ( _XSObject* obj = bottomChild; obj; obj = obj->above )
		obj->CalcDrawUpdate(&mat,pRectc,XTRUE);
}

bool _XSObject::CalcUpdate(MATRIX* m,SRECT*clipRect, BOOL forceDirty)//,XBOOL bUpdate)
{
	//if(character&&character->tag==48)
	//	int v=0;
	//if(this==debugObj)
	//	int v=0;

	bool bUpdate=false;
	MATRIX mat;

	MatrixConcat(&xform.mat, m, &mat);
#ifdef _WINEMU
	/*if(mat.tx<-1000000)
	{
		int v=0;
		MATRIX mat0;
		MatrixConcat(&xform.mat, m, &mat0);
	}*/
#endif
//	if(character&&character->tag==3)
//		int v=0;
	if(dirty) bUpdate=true;
	forceDirty |= dirty;
	SRECT*pRectc=clipRect,rectc={0,0};
	if(scrollRect)
	{
		MATRIX mats;
		::MatrixIdentity(&mats);
		mats.tx=-scrollRect->xmin;
		mats.ty=-scrollRect->ymin;
		rectc.xmax=RectWidth(scrollRect);
		rectc.ymax=RectHeight(scrollRect);
		::MatrixTransformRect(&mat,&rectc,&rectc);
		pRectc=&rectc;
		if(clipRect)
			::RectIntersect(clipRect,pRectc,pRectc);
		::MatrixConcat(&mats,&mat,&mat);
	}

	if ( forceDirty ) {

		
		//if(character&&character->tag==5)
		//	int v=0;
		// The cache is no longer valid
		FreeCache();
		if(pCacheBmp)
			pCacheBmp->SetModify();
		if(pASObject&&character)
		{
			switch(character->type)
			{
			case buttonChar:
			case buttonEmptyChar:
				{
				 avmshell::XAVM2*pAVM2=display->player->m_pAVM2;
				 SObject*pObject=pAVM2->GetButtonObject(pASObject,bsHitTest,avmshell::XAS3_BUT_HIT);
				 if(pObject!=NULL)
				 {
					if(pObject->CalcUpdate(&mat,pRectc,forceDirty))
						bUpdate=true;
				 }
					//pObject=pAVM2->GetButtonObject(pASObject,state,avmshell::XAS3_BUT_HIT);				
				}break;
			}
		}

		if ( drawn )
		{
			//if(bUpdate)
			/*if(this==debugObj)
			{
				int v=0;
				devBounds.xmin=0;
				devBounds.ymin=0;
				devBounds.xmax=5000;
				devBounds.ymax=5000;
			}*/
			display->InvalidateRect(&devBounds);
			drawn = XFALSE;

			//debugObj=XNULL;
			//exflags&=~0x40000000;
			//if(pRectc)
			//	::RectUnion(&devBounds,pRectc,pRectc);
		}
		//else if(tagType==shapeChar)
		//	int v=0;

		CalcDevBounds(&mat,pRectc);

		//if(bUpdate)
		display->InvalidateRect(&devBounds);
		//if(pRectc)
		//		::RectUnion(&devBounds,pRectc,pRectc);
		dirty = XFALSE;
		dirty1 = XFALSE;
	}
	else
	{
		if ( drawn )
		{			
			display->InvalidateRect(&devBounds);
			drawn = XFALSE;

			//debugObj=XNULL;
			//exflags&=~0x40000000;
			//if(pRectc)
			//	::RectUnion(&devBounds,pRectc,pRectc);
		}
		//else if(tagType==shapeChar)
		//	int v=0;
		
		CalcDevBounds(&mat,pRectc);

		//if(bUpdate)
		//display->InvalidateRect(&devBounds);		
		dirty1 = XFALSE;
	}
	/*else if(bottomChild)
	{
		_XSObject*obj=bottomChild;
		int iCount=0;
		bool bSet=false;
		while(obj)
		{
			if(obj->clipDepth&&obj->dirty) bSet=true;
			obj=obj->above;
			iCount++;
		}
		if(bSet)
		{
			int i,j,nSize=sizeof(_XSObject*)*iCount;
			_XSObject**list=(_XSObject**)XXVar::AllocData(nSize);
			obj=bottomChild;
			for(i=0;i<iCount;i++)
			{
				list[i]=obj;
				obj=obj->above;
			}
			for(i=0;i<iCount;i++)
			{
				obj=list[i];
				if(!obj->clipDepth||!obj->dirty) continue;
				if(obj->clipDepth>0)
				{
					for(j=i+1;j<iCount;j++)
					{
						//if(list[i]->depth>obj->clipDepth) break;
						if(list[j]->depth<=obj->clipDepth||
							list[j]->clipObject==obj)
							list[j]->Modify();
					}
					for(j=i-1;j>=0;j--)
					{
						if(list[j]->depth==obj->clipDepth||
						   list[j]->clipObject==obj)
						   list[j]->Modify();
					}
				}
				else
				{
					for(j=0;j<iCount;j++)
					{
						if(list[j]->clipObject==obj)
							list[j]->Modify();
					}
				}
			}
			XXVar::FreeData(list,nSize);
		}
	}*/
	/*if(character&&pASObject)
	{
		switch(character->type)
		{
		case buttonEmptyChar:
		case buttonChar:
			 {
				 SObject*pObject=character->player->m_pAVM2->
								 GetButtonObject(pASObject,state);
				 if(pObject)
					pObject->CalcUpdate(&mat,forceDirty);
			 }
			 break;
		}
	}*/
	//SRECT sr,*prr=XNULL;
	//::RectSetEmpty(&sr);
	//if(pCacheBmp)//|pRectc)
	//		prr=&sr;
	//else if(pRectc)
	//		prr=pRectc;
	
	for ( _XSObject* obj = bottomChild; obj; obj = obj->above )
	{
		if(obj->CalcUpdate(&mat,pRectc, forceDirty))
		{
			bUpdate=true;
			//if(pCacheBmp)
			//	pCacheBmp->m_invalid=true;
		}
	}
	if(bUpdate&&pCacheBmp)
	{
		delete pCacheBmp;
		pCacheBmp = NULL;
		//pCacheBmp->m_invalid=true;		
	}

	/*if(bUpdate&&edges&&(clipObject||clipDepth))
	{
		FreeCache();
	}*/

	/*if(name.strTxt!=XNULL&&
		strstr(name.strTxt,"Resident")!=XNULL)
	{
		//if(this->bottomChild==XNULL)
		//	int v=0;
		//bObj=XTRUE;
		_nDebugTimes++;
		if(RectIsEmpty(&bottomChild->devBounds))
			int v=0;
	}*/
	/*if(character)
	{
		if(!RectIsEmpty(&character->rbounds)&&
			RectIsEmpty(&devBounds))
			int v=0;
	}*/
	//if(bUpdate&&pCacheBmp)
		//pCacheBmp->m_bInvalid=true;

	return bUpdate;
}

void _XSObject::BuildClipEdges(STransform x,_XSObject*pClip)
{
	if(!pClip)
	{
		if(mpShapeList)//!this->mShapeRecord.IsEmpty())
			return;
		//if ( edges || colors  ) return;
	}
	x.Concat(&xform);
	if(scrollRect)
	{
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.tx=-scrollRect->xmin;
		mat.ty=-scrollRect->ymin;
		::MatrixConcat(&mat,&x.mat,&x.mat);
	}
	BuildEdges(&x,pClip,true);
	if(bottomChild)
	{
		//int ta=raster->bitArea>>1;//::RectArea(&raster->bitClip)>>1;
		SObject* pObj=bottomChild;			
		_XSObject*p=pClip?pClip:this;
		while(pObj)
		{
			pObj->BuildClipEdges(x,p);
			//int tao=::RectArea(&pObj->devBounds);
			//if(tao<=ta)
			//{
			//	STransform xxx=xx;
			//	xxx.Concat(&pObj->xform);
			//	pObj->BuildEdges(&xxx,p);
			//}
			//pObj->DrawClipBracket(raster,xx,clipColor,p);
			pObj=pObj->above;
		}
	}
}

/*_XSObject* _XSObject::DrawClipBracket(CRaster* raster, STransform x, RColor* clipColor,_XSObject*pClip)
{
	{// Create a clip color and build the edges
		//STransform xx = x;
		//xx.Concat(&xform);
		//if(!pClip)
		BuildClipEdges(x,pClip);

	}
	//FLASHASSERT(colors && colors->colorType == colorClip);
	//if ( !colors ) colors=clipColor;
	//	
	drawn = XTRUE;
	if(!colors)
		return above;
	

	
	if(pClip) 
		return above;

	// Draw the in between objects
	_XSObject* obj = above;
	//if(clipDepth<0)
	//	int v=0;
	if(clipDepth>0)
	{
		while ( obj && obj->depth <= clipDepth ) {
			// Draw an object
			//RColor*cols=colors;
				//cols=clipColor;
			if ( obj->clipDepth ) 
			{
				if(obj->depth>0||obj->clipObject==this)
					obj = obj->DrawClipBracket(raster, x, colors);
				else
					obj = obj->DrawClipBracket(raster, x, clipColor);
			} else {
				if(obj->depth>0||obj->clipObject==this)
					obj->Draw(raster, x, colors);
				else
					obj->Draw(raster, x, clipColor);
				obj = obj->above;
			}
			//if(clipDepth<0) break;
			//if(clipDepth<0&&obj->clipObject!=this)
			//	break;
			
		}
	}
	else
	{
		bool bUse=false;
		while ( obj ) {
			// Draw an object
			RColor*cols=colors;
			if(obj->clipObject!=this)
				cols=clipColor;
			else bUse=true;
				//cols=clipColor;
			if ( obj->clipDepth ) 
			{
				obj = obj->DrawClipBracket(raster, x, cols);
			} else {
				obj->Draw(raster, x, cols);
				obj = obj->above;
			}
			//if(clipDepth<0&&obj->clipObject!=this)
			//	break;
			
		}
		if(!bUse)
			FreeCache();
			//int v=0;
	}

	// Add the clip color to the raster above the clipped colors
	raster->AddEdges(edges, colors, clipColor ? clipColor : (RColor*)1);

	return obj;
}*/

#define SETGRIDOBJ(BASE,OBJ) {OBJ->above=BASE;BASE=OBJ;}

inline void ToPixel(SRECT*r,int b)
{
	if(!b) return;
	int h=1<<(b-1);
	//if(r->xmin>0)
		r->xmin+=h;
	//else
	//	r->xmin-=h;
	//if(r->ymin>0)
		r->ymin+=h;
	//else
	//	r->ymin-=h;
	//if(r->xmax>0)
		r->xmax+=h;
	//else
	//	r->xmax-=h;
	//if(r->ymax>0)
		r->ymax+=h;
	//else
	//	r->ymax-=h;
	r->xmin>>=b;
	r->xmax>>=b;
	r->ymin>>=b;
	r->ymax>>=b;
}

inline _GRIDINFO* SetGGRect(_XSObject*obj,_GRIDINFO*info,MATRIX*mat,bool&bSet)
{ return XNULL; }


/*inline void FreeGGRect(DisplayList*display,_GRIDINFO*info)
{
	if(info->devRect)
		display->FreeRect(info->devRect);
	if(info->gridBy)
		display->FreeRect(info->gridBy);
	//if(info->orgRect)
	//	display->FreeRect(info->orgRect);
}*/



void _XSObject::FreeCacheBmp()
{
	if(pCacheBmp!=XNULL)
	{
		delete pCacheBmp;
		pCacheBmp=XNULL;
	}
	if(pCacheMat!=XNULL)
	{
		//delete pCacheMat;
		display->FreeMatrix(pCacheMat);
		pCacheMat=XNULL;
	}
	this->FreeCache();
	this->Modify();
}

//
//void _XSObject::DrawDraw( Raster *pRasterGL, STransform x,GLColor *clipColor,_GRIDINFO*gridBy,XBOOL bTrans,XU32 nFilterColor,bool setDrawn)
//{
//
//}


_XSObject* _XSObject::GetItemParent(_XSObject*child,_XSObject*parent)
{
	_XSObject*obj=bottomChild;
	while(obj)
	{
		if(obj==child&&parent!=this)
			return this;
		_XSObject*p=obj->GetItemParent(child,parent);
		if(p) return p;
		obj=obj->above;
	}
	return NULL;
}


/*static bool IsModify(_XSObject*obj)
{
	if(obj->dirty) return true;
	_XSObject*ch=obj->bottomChild;
	while(ch)
	{
		if(IsModify(ch)) return true;
		ch=ch->above;
	}
	return false;
}*/


inline bool _XSObject::CacheIfNeed(MATRIX&mat)
{
	if(!cacheMode&&!gridRect&&!pFilter) return false;
	//GetBoundingBox(this,XNULL,&dr,false);
	//if(!bBG)
	//::MatrixTransformRect(&mat,&dr,&dr);
	//else
	//{
	//	dr.xmin=(dr.xmin+mat.tx)/20;
	//	dr.ymin=(dr.ymin+mat.ty)/20;
	//	dr.xmax=(dr.xmax+mat.tx)/20;
	//	dr.ymax=(dr.ymax+mat.ty)/20;
	//}
	if(pCacheBmp==XNULL||
	   pCacheMat==XNULL) 
	   return true;
	
	//if(pCacheBmp->IsEmpty()) return true;
	/*if(pCacheMat->tx!=dr.xmin-mat.tx||
	   pCacheMat->ty!=dr.ymin-mat.ty)
	{
		FreeCacheBmp();
		return true;
	}*/

	if((pCacheMat->a!=mat.a||
	   pCacheMat->b!=mat.b||
	   pCacheMat->c!=mat.c||
	   pCacheMat->d!=mat.d)&&!gridRect)
	{
	  //if(pCacheBmp->IsModify())
	  //{
		//FreeCacheBmp();
		return true;
	  //}
	}

	//if(pCacheBmp->m_invalid)
	//	return true;

	//pCacheMat->tx=mat.tx;
	//pCacheMat->ty=mat.ty;
	//_XSObject* sc=bottomChild;
    //while(sc)
	//{
	//	if(sc->CacheIfNeed(mat)) return true;
	//	sc=sc->above;
	//}

	return false;
}

static void FreeCacheAll(_XSObject*obj)
{
	obj->FreeCache();

	if(obj->pASObject&&obj->character)
		{
			switch(obj->character->type)
			{
			case buttonChar:
			case buttonEmptyChar:
				{
				 avmshell::XAVM2*pAVM2=obj->display->player->m_pAVM2;
				 SObject*pObject=pAVM2->GetButtonObject(obj->pASObject,bsHitTest,avmshell::XAS3_BUT_HIT);
				 if(pObject!=NULL)
				 {
					 pObject->FreeCache();
					//if(pObject->CalcUpdateBG(raster,&mat,true))//&mat,pRectc,forceDirty,bBG))
					//	bUpdate=true;
				 }
					//pObject=pAVM2->GetButtonObject(pASObject,state,avmshell::XAS3_BUT_HIT);				
				}break;
			}
		}

	obj=obj->bottomChild;
	while(obj)
	{
		FreeCacheAll(obj);
		obj=obj->above;
	}
}


static void filtersize(XFilter*pFilter,P_SRECT dr,P_MATRIX mat)
{
	if(!pFilter) return;
	int tempx,tempy;
	switch (pFilter->m_nFilterID)
	{
		case XFilterBlur:
			 tempx=pFilter->pBlur.blurX*20;
			 tempy=pFilter->pBlur.blurY*20;
			 break;
		case XFilterGlow:
			 tempx=pFilter->pGlow.blurX*20;
			 tempy=pFilter->pGlow.blurY*20;
			 break;
		case XFilterDropShadow:
			 tempx=pFilter->pDropShadow.blurX*20;
			 tempy=pFilter->pDropShadow.blurY*20;
			 break;
		case XFilterGradientGlow:
			 tempx=pFilter->pGradientGlow.blurX*20;
			 tempy=pFilter->pGradientGlow.blurY*20;
			 break;
		default:return;
	}

	tempx = XMIN(tempx,200);
	tempy = XMIN(tempy,200);

	if(dr!=XNULL&&(pFilter->m_nFilterID==XFilterBlur||pFilter->m_nFilterID==XFilterGlow))
	{
		dr->xmin-=(tempx/2);
		dr->ymin-=(tempy/2);
		dr->xmax+=(tempx/2);
		dr->ymax+=(tempy/2);
	}
	else if(dr!=XNULL&&(pFilter->m_nFilterID==XFilterDropShadow||pFilter->m_nFilterID==XFilterGradientGlow))
	{
		dr->xmax+=tempx;
		dr->ymax+=tempx;
	}

	if(mat!=XNULL&&(pFilter->m_nFilterID==XFilterBlur||pFilter->m_nFilterID==XFilterGlow))
	{
		mat->tx-=(tempx/2);
		mat->ty-=(tempy/2);
	}
}


static void buildCacheGrid(RasterGL*raster,_XSObject*pObj,STransform&form,XFilter*pFilter)
{
	//if(m_pObject)
	//SRECT dr;
	//return;
	//if(pObj->character->tag==96)
	//	int v=0;
	MATRIX cMat,tMat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(cMat);
	MatrixConcat(&form.mat,&cMat,&tMat);
	if(!pObj->pCacheBmp||!pObj->pCacheMat)//pObj->CacheIfNeed(form.mat)) //return;
	{
		//if(pObj->xform.mat.ty==260)
		//	int vv=0;
		SRECT dr;
		GetBoundingBox(pObj,XNULL,&dr,false,true);
		int bw=dr.xmax-dr.xmin;
		int bh=dr.ymax-dr.ymin;
		filtersize(pFilter,&dr,XNULL);
		int w = (dr.xmax-dr.xmin+19)/20;
		int h = (dr.ymax-dr.ymin+19)/20;
		if(w<=0||h<=0) 
		{
			if(pObj->pCacheBmp)
				pObj->FreeCacheBmp();
			return;
		}
		if(pObj->pCacheBmp&&(w>pObj->pCacheBmp->mWidth||h>pObj->pCacheBmp->mHeight))
			pObj->FreeCacheBmp();
		if(pObj->pCacheMat==XNULL)
			pObj->pCacheMat=pObj->display->CreateMatrix();//displaynew MATRIX();
		pObj->pCacheMat->a=(float)bw*fixed_1/w/20;
			//form.mat.a;
		pObj->pCacheMat->b=0;
			//form.mat.b;
		pObj->pCacheMat->c=0;
			//form.mat.c;
		pObj->pCacheMat->d=(float)bh*fixed_1/h/20;
			//form.mat.d;
		{
			SRECT dr1=dr;
			pObj->pCacheMat->tx=dr1.xmin;
			pObj->pCacheMat->ty=dr1.ymin;
		}
		STransform x;//=form;
		x.Clear();
		if(dr.xmin||dr.ymin)
		{
			x.mat.tx=-dr.xmin;
			x.mat.ty=-dr.ymin;
			//::MatrixConcat(&x.mat,&mat,&x.mat);
		}

		//if(pDib->Width()==83&&pDib->Height()==24)
		//	int v=0;
		SRECT dev=dr;
		
		if(pObj->pCacheBmp==XNULL)
		{
			pObj->pCacheBmp=new FrameTexture();//XDrawDib();
			pObj->pCacheBmp->init(w,h,true);
			//pObj->pCacheBmp->bPreAlpha = true;
		}
		//else
		//	pObj->pCacheBmp->clear();
		if(w>0&&h>0)
		{
			pObj->pCacheBmp->beginPaint(true,false);
			pObj->pCacheBmp->clear();
			//pObj->pCacheBmp->DrawRect(0,0,w*20,h*20,0xffffff);
			pObj->Draw(raster,x,true,NULL);
			pObj->pCacheBmp->endPaint();
			pObj->CalcDrawUpdate(&tMat,NULL,XFALSE);
			pObj->pCacheBmp->ClearModify();
		}
		pObj->dirty=false;
		pObj->dirty1=false;
		//pObj->devBounds=dev;
		//::RectSetEmpty(&pObj->devBounds);
	}
	if(pObj->pCacheBmp)
	//else
	{
		SRECT dr={0,pObj->pCacheBmp->mWidth*20,0,pObj->pCacheBmp->mHeight*20};

		/*MATRIX mat;
		mat.a = fixed_1;
		mat.d = mat.a;
		mat.b = mat.c = 0;
		mat.tx = pObj->pCacheMat->tx;
		mat.ty = pObj->pCacheMat->ty;*/
		SRECT dev;
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.tx = pObj->pCacheMat->tx;
		mat.ty = pObj->pCacheMat->ty;
		::MatrixConcat(&mat,&tMat,&mat);
		//tMat.tx+=pObj->pCacheMat->tx;
		//tMat.ty+=pObj->pCacheMat->ty;
		::MatrixTransformRect(&mat,&dr,&dev);
		//::MatrixTransformRect(&form.mat,&dev,&dev);
		pObj->dirty=false;
		pObj->dirty1=false;
		pObj->devBounds=dev;
	}
}

static void buildCache(RasterGL*raster,_XSObject*pObj,STransform&form,XFilter*pFilter)
{
	//if(m_pObject)
	//SRECT dr;
	//return;
	MATRIX cMat,tMat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(cMat);
	MatrixConcat(&form.mat,&cMat,&tMat);
	if(pObj->CacheIfNeed(tMat)) //return;
	{
		//if(pObj->xform.mat.ty==260)
		//	int vv=0;
		SRECT dr;
		GetBoundingBox(pObj,&tMat,&dr,false,true);
		filtersize(pFilter,&dr,XNULL);
		int w = (dr.xmax-dr.xmin)/20;
		int h = (dr.ymax-dr.ymin)/20;
		if(w<=0||h<=0) 
		{
			if(pObj->pCacheBmp)
				pObj->FreeCacheBmp();
			return;
		}
		if(pObj->pCacheBmp&&(w>pObj->pCacheBmp->mWidth||h>pObj->pCacheBmp->mHeight))
			pObj->FreeCacheBmp();
		if(pObj->pCacheMat==XNULL)
			pObj->pCacheMat=pObj->display->CreateMatrix();//displaynew MATRIX();
		pObj->pCacheMat->a=tMat.a;
		pObj->pCacheMat->b=tMat.b;
		pObj->pCacheMat->c=tMat.c;
		pObj->pCacheMat->d=tMat.d;
		{
			pObj->pCacheMat->tx=dr.xmin;
			pObj->pCacheMat->ty=dr.ymin;
		}
		STransform x;//=form;
		x.Clear();
		x.mat = form.mat;//tMat;
		if(dr.xmin||dr.ymin)
		{
			x.mat.tx-=(float)dr.xmin*fixed_1/cMat.a;
			x.mat.ty-=(float)dr.ymin*fixed_1/cMat.d;
			//x.mat.tx=-dr.xmin;
			//x.mat.ty=-dr.ymin;
			//::MatrixConcat(&x.mat,&mat,&x.mat);
		}

		//if(pDib->Width()==83&&pDib->Height()==24)
		//	int v=0;
		SRECT dev=dr;
		
		if(pObj->pCacheBmp==XNULL)
		{
			pObj->pCacheBmp=new FrameTexture();//XDrawDib();
			pObj->pCacheBmp->init(w,h,true);
			pObj->pCacheBmp->bPreAlpha = false;
		}
		//else
		//	pObj->pCacheBmp->clear();
		if(w>0&&h>0)
		{
			pObj->pCacheBmp->beginPaint(true,true);
			pObj->pCacheBmp->clear();
			//pObj->pCacheBmp->DrawRect(0,0,w*20,h*20,0xffffff);
			pObj->Draw(raster,x,true,NULL);
			pObj->pCacheBmp->endPaint();
			pObj->CalcDrawUpdate(&tMat,NULL,XFALSE);
			pObj->pCacheBmp->ClearModify();
		}
		pObj->dirty=false;
		pObj->dirty1=false;
		pObj->devBounds=dev;
		//::RectSetEmpty(&pObj->devBounds);
	}
	else
	{
		SRECT dr;
		GetBoundingBox(pObj,&tMat,&dr,false,true);
		filtersize(pFilter,&dr,XNULL);
		pObj->pCacheMat->tx=dr.xmin;
		pObj->pCacheMat->ty=dr.ymin;
		pObj->dirty=false;
		pObj->dirty1=false;
		pObj->devBounds=dr;
		/*SRECT dr={0,pObj->pCacheBmp->mWidth,0,pObj->pCacheBmp->mHeight};

		SRECT dev;
		::MatrixTransformRect(&tMat,&dr,&dev);
		//::MatrixTransformRect(&form.mat,&dev,&dev);
		pObj->dirty=false;
		pObj->dirty1=false;
		pObj->devBounds=dev;*/
	}
}




/*static bool NeedCache(_XSObject*pObj)
{
	return false;
	if(pObj->pASObject&&pObj->IsDisplayObject())
	{
		_XCOMMAND*pCmds=pObj->display->player->m_pAVM2->GetCommands(pObj->pASObject);
		if(pCmds)
		{
			return true;
		}
	}
	if(pObj->character)
	{
		switch(pObj->character->type)
		{
		case editTextChar:
		case commandChar:
		case morphShapeChar:
		case textChar:
		case buttonChar:
		case editTextEmptyChar:
		case buttonEmptyChar:
		case textLineChar:
		case shapeChar:return true;
		case bitsChar:
		//case buttonChar:
		case videoChar:
		case bitmapChar:
		case bitmapEmptyChar:
		case playerChar:
		case videoEmptyChar:
		case loaderChar:
		case loaderImageChar:
		case loaderSWFChar:
			 //iCount++;
			 //if(iCount>2) return true;
			 break;			
		}
	}
	_XSObject*ch=pObj->bottomChild;
	while(ch)
	{
		if(NeedCache(ch))
			return true;
		ch = ch->above;
	}
	return false;
}*/

void _XSObject::DrawCached( RasterGL* pRastergl, STransform x,SRECT*clip)//bool bFilterRender)
{
	//return;
	_ANALYENTER("Draw Cached Objects");
	if(character)//&&character->type==15)
	{
		//if (character->tag == 172&&this->gridRect)
		//{
		//	int v=0;
		//	int b=0;
		//}
		if(RectIsEmpty(&devBounds)&&!RectIsEmpty(&character->rbounds))
			::MatrixTransformRect(&x.mat,&character->rbounds,&devBounds);
	}

	/*if(!RectIsEmpty(&devBounds)&&clip)
	{
		if(!::RectTestIntersect(&devBounds,clip))
			return;
	}*/


	if (!IsVisible())//||this->blendMode>drawBlendNormal1)//visible)
	{
		return;
	}
	
	if(pASObject&&display->m_bInvalidate)
	{
		if(character)
		{
			this->AS3_Onrender();
		}
		else
		{
			register XSWFPlayer*player=display->player;
			player->m_pAVM2->OnEvent(avmshell::XAS3_EVENT_render,false,pASObject,GetContext());
		}
	}

	x.Concat(&xform);
	if(x.cxform.nBlendMode<2&&blendMode)
		x.cxform.nBlendMode = blendMode;

	if(bottomChild)
	{
		SObject*obj=this->bottomChild;
		while(obj)
		{	
			obj->DrawCached(pRastergl,x,clip);
			obj=obj->above;	
		}
	}
	
	if(/*cacheMode||*/gridRect)//||pFilter)
	{
		_ANALYENTER("Build Cached Images");
		if(gridRect)
			buildCacheGrid(pRastergl,this,x,pFilter);
		else
			buildCache(pRastergl,this,x,pFilter);
	}

	
}


void _XSObject::Draw( RasterGL* pRastergl, STransform x,bool bDraw,SRECT*clip,_XFilter*pf)//bool bFilterRender)
{
	if(character)//&&character->type==15)
	{
		/*if (character->tag == 172&&this->gridRect)
		{
			int v=0;
			int b=0;
		}*/
		//if(character->tag==4)
			//return;
		//	int v=0;
		if(RectIsEmpty(&devBounds)&&!RectIsEmpty(&character->rbounds))
			::MatrixTransformRect(&x.mat,&character->rbounds,&devBounds);
	}

	/*if(!RectIsEmpty(&devBounds)&&clip)
	{
		if(!RectTestIntersect(&devBounds,clip))
			return;
	}*/
	_ANALYENTER("Draw Objects");

		/*if(pASObject&&display->m_bInvalidate)
		{
			if(character)
			{
				this->AS3_Onrender();
			}
			else
			{
				register XSWFPlayer*player=display->player;
				player->m_pAVM2->OnEvent(avmshell::XAS3_EVENT_render,false,pASObject,GetContext());
			}
		}*/
	#ifdef _ANALY	
		#ifdef WIN32
		if(_ANIsDisabled((XU32)this))
		return;
		#endif
		
	#endif
		if (!IsVisible())//||this->blendMode>drawBlendNormal1)//visible)
		{
			return;
		}
		if(pFilter) pf=pFilter;
		int iCount = 0;
		if(!bDraw)
		{
			if(pCacheBmp)
			{
				//return;
				//if(x.cxform.aa==0&&x.cxform.ab==0) return;
				_ANALYENTER("Draw Cached Images");
				SRECT *pRect = NULL;
				SRECT dr;
				MATRIX cMat,mat;
				XSWFPlayer::m_pInstance->display.GetCameraMatrix(cMat);
				if(gridRect)
				{
					x.Concat(&xform);
					dr = *gridRect;
					pRect=&dr;
					//pRect->xmin=pRect->xmin*20;//+pCacheMat->tx;
					//pRect->ymin=pRect->ymin*20;//+pCacheMat->ty;
					//pRect->xmax=pRect->xmax*20;//+pCacheMat->tx;
					//pRect->ymax=pRect->ymax*20;//+pCacheMat->ty;
					pRect->xmin=pRect->xmin*20-pCacheMat->tx;
					pRect->ymin=pRect->ymin*20-pCacheMat->ty;
					pRect->xmax=pRect->xmax*20-pCacheMat->tx;
					pRect->ymax=pRect->ymax*20-pCacheMat->ty;
				
					filtersize(pf,XNULL,&x.mat);
					mat=*pCacheMat;
					//MATRIX mat;
					//::MatrixIdentity(&mat);
					//mat = x.mat;
					//mat.tx = pCacheMat->tx;
					//mat.ty = pCacheMat->ty;
					::MatrixConcat(&mat,&x.mat,&mat);
					pRastergl->DrawFrame(pCacheBmp,mat,x.cxform,pRect,pf,&cMat,(enBlendMode)this->blendMode);
				}
				else
				{
					filtersize(pf,XNULL,&x.mat);
					
					//cMat.tx=0;//-pCacheMat->tx;
					//cMat.ty=0;//-pCacheMat->ty;
					::MatrixInvert(&cMat,&mat);
					SPOINT pt={pCacheMat->tx,pCacheMat->ty};
					::MatrixTransformPoint(&mat,&pt,&pt);
					mat.tx = pt.x;//pCacheMat->tx;
					mat.ty = pt.y;//pCacheMat->ty;
					//mat.tx = pCacheMat->tx;
					//mat.ty = pCacheMat->ty;
					pRastergl->DrawFrame(pCacheBmp,mat,x.cxform,pRect,pf,NULL,(enBlendMode)this->blendMode);
				}
				return;
			}
			x.Concat(&xform);
		}
		//if(x.cxform.aa==0&&x.cxform.ab==0) return;
		//if(blendMode)
		{
			/*if(character&&character->tag==45)
				int v=0;
			if(character&&character->tag==44)
				int v=0;*/
			//if(xform.cxform.nForeColor)
			//	int v=0;
			//if(!bDraw)
			//	x.Concat(&xform);
			//if(blendMode)
			//	int v=0;
			if(x.cxform.nBlendMode<2&&blendMode)
				x.cxform.nBlendMode = blendMode;
			switch(blendMode)
			{
				case drawBlendErease:
				case drawBlendAlpha:
				return;
				break;
			}
			/*switch(blendMode)
			{
			case drawBlendAdd://	 = 8,
			case drawBlendLighten:// = 5,
				 x.cxform.nBlendMode=blendMode;
				 break;
			case drawBlendNormal0:// = 0,
			case drawBlendNormal1:// = 1,
			case drawBlendLayer://	 = 2,
			
			//case drawBlendDarken://	 = 6,
			case drawBlendDefrence://= 7,
			
			//case drawBlendSubtrace://= 9,
			//case drawBlendInvert://	 =10,
			//case drawBlendAlpha://	 =11,
				//drawBlendErease  =12,
			case drawBlendScreen:
			case drawBlendHardLight://=14
				break;
			case drawBlendOverlay:// =13,
			case drawBlendInvert://	 =10,
				{
					ColorTransform form;
					form.Clear();
					form.aa=0xd0;//0xa0;
					form.CalcFlags();
					x.cxform.Concat(&form);
				}
				break;
			
			default:
				return;
				//drawBlendMulty	 = 3,
				//drawBlendScreen	 = 4,
				
			}*/
		}

	
		//x.cxform.nBackColor=nBackColor;
	if(nForeColor)
		x.cxform.nForeColor=nForeColor;
	
	

	ClipMask* pMask = XNULL;
	if(scrollRect)
	{
		
		pMask = new ClipMask();
		pMask->beginPaint();
		pMask->setInverted( true );
		pMask->setupClip();
		float color[]={1,1,1,1};
		SRECT rr={0,RectWidth(scrollRect),0,RectHeight(scrollRect)};
		RasterGL::sharedRasterGL()->DrawRectangle(rr,x.mat,color);//DrawShapeList(clipData,true);
		pMask->setupDraw();
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.tx=-scrollRect->xmin;
		mat.ty=-scrollRect->ymin;
		::MatrixConcat(&mat,&x.mat,&x.mat);
	}


	 {
		
		BuildEdges(&x,NULL);//gridBy,NULL,nFilterColor);//bFilterRender);
		if(mpShapeList)//!mShapeRecord.IsEmpty())
		{
			pRastergl->DrawShapeList(mpShapeList,x.mat,pf);
			drawn = XTRUE;
		}
		
	}


	if(bottomChild)
	{

		struct OBJINFO
		{
			SObject*object;
			int		edgesOK;
			int		clipBy;
			bool	bChildClip;
		};
		int i,iCount=0;
		SObject*obj=this->bottomChild;
		while(obj)
		{	
			if(obj->clipDepth!=-2)
			{
				iCount++;
				if(obj->clipObject&&obj->clipObject->parent!=this)
				{
					iCount++;
					obj->clipObject->FreeCache();
					obj->clipObject->clipDepth=-2;
				}
			}
			obj=obj->above;	
		}
		int nSize=iCount*sizeof(OBJINFO);

		OBJINFO*info=(OBJINFO*)XXVar::AllocData(nSize);

		//iCount--;
		obj=this->bottomChild;
		i=0;
		//memset(info,0,nSize);
		while(obj)
		{
			if(obj->clipDepth!=-2)
			{
				info[i].object=obj;
				info[i].edgesOK=0;
				info[i].clipBy=0;
				info[i].bChildClip=false;
				if(obj->clipObject&&obj->clipObject->parent!=this)
				{
					i++;
					info[i].object=obj->clipObject;
					info[i].edgesOK=0;
					info[i].clipBy=0;
					info[i].bChildClip=true;
				}
				i++;
			}
			
			obj=obj->above;
			
		}
		for(i=0;i<iCount;i++)
		{
			
			obj=info[i].object;
			if(!obj->clipDepth) continue;
			int j;
			if(obj->clipDepth>0&&obj->depth>0)
			{
				if(obj->clipDepth>obj->depth)
				{
					bool maskset=true;
					for(j=i+1;j<iCount;j++)
					{
						//if(info[j].object->depth>obj->clipDepth) break;
						//if(info[j].object->depth<0){continue;}
						//info[j].clipBy=i+1;
						//Modify by xiepf
						int depth=info[j].object->depth;
						if(depth>obj->clipDepth)
							maskset=false;
						if(info[j].object->clipObject==obj||maskset)
							//(maskset&&depth<obj->clipDepth))
								info[j].clipBy=i+1;
					}
				}
				else
				{
					for(j=i-1;j>=0;j--)
					{
						int depth=info[j].object->depth;
						if(info[j].object->clipObject==obj)
							info[j].clipBy=i+1;
						else if(depth>0&&depth==obj->clipDepth)
						{
							info[j].clipBy=i+1;
							break;
						}
					}
				}
			}
			else
			{
				for(j=0;j<iCount;j++)
				{
					if(info[j].object->clipObject!=obj) continue;
					info[j].clipBy=i+1;
				}
			}
		}

		for(i=0;i<iCount;i++)
		{
			obj=info[i].object;
			if(obj == NULL || obj->clipDepth)
                continue;
			//if(strcmp(obj->name.strTxt,"headImg")==0)
			//	int v=0;
			int ic=info[i].clipBy;
			//GLColor*clip=clipColor;
			//Modify by xiepeifu
			GLShapeListData* clipData = NULL;
			if(ic)//&&info[ic-1].object->IsVisible())
			{
				ic--;
				_XSObject*cobj=info[ic].object;

				if(!info[ic].edgesOK)
				{
					if(!info[ic].bChildClip)
						cobj->BuildClipEdges(x,XNULL);
					else
					{
						STransform x1=x;
						if(cobj->parent)
							cobj->parent->GetTotalMatrix(x1.mat,XFALSE);						
						cobj->BuildClipEdges(x1,XNULL);
					}
					cobj->drawn=true;
					
					info[ic].edgesOK=true;
				}
				if ( cobj->mpShapeList)//cobj->mShapeRecord.IsEmpty())//cobj->mShapeRecord.mVecPaths.size() > 0 )
					clipData = cobj->mpShapeList;
				//clip=cobj->colors;
				//if(!clip) continue;
			}
			//obj->Draw(raster,x,clip,gridBy,nFilterColor);//bFilterRender);
			RasterGL* raster = RasterGL::sharedRasterGL();
			/*if (character&&character->tag == 109)
			{
				int v=0;
				int b=0;
			}*/
			if(clipData)
			{
				ClipMask gClipMask;
				gClipMask.beginPaint();
				gClipMask.setInverted( true );
				gClipMask.setupClip();
				raster->DrawShapeList(clipData,x.mat,NULL,true);
				gClipMask.setupDraw();
				obj->Draw( pRastergl, x,false,clip,pf);
				gClipMask.endPaint();
			}
			else
			{
				/*if(obj->character&&obj->character->tag==45)
					int v=0;*/
				obj->Draw( pRastergl, x,false,clip,pf);
			}
		}

		

		XXVar::FreeData(info,nSize);
		AS3ClearFilterRender();
	}

	if(pMask)
	{
		pMask->endPaint();
		delete pMask;
	}
}

void GetBoundingBox(_XSObject* obj, P_MATRIX parentMatrix, P_SRECT bounds,bool transSelf,bool setall)
{
	/*MATRIX mat;
	if(transSelf)
	{
		if(parentMatrix)
			MatrixConcat(&obj->xform.mat, parentMatrix, &mat);
		else
			mat=obj->xform.mat;
	}
	else
	{
		if(parentMatrix)
			mat=*parentMatrix;
		else
			::MatrixIdentity(&mat);
	}

	if(obj->character)
	{
		SRECT&rr=GETBOUNDS(obj->character,obj->editText);
		MatrixTransformRect(&mat, &rr, bounds);
	}
	
	
	int iMask=0;
	SRECT clipRect;
	::RectSetEmpty(&clipRect);

	for (_XSObject* child = obj->bottomChild; child; child = child->above) {		
		if(child->clipDepth<0) continue;		
		SRECT childBounds;
		::RectSetEmpty(&childBounds);
		GetBoundingBox(child, &mat,&childBounds,child->clipDepth>0?false:setall);
		if(iMask)
		{	
			if(child->depth<=iMask)
			{
				::RectIntersect(&childBounds,&clipRect,&childBounds);
				//continue;
			}
			else
				iMask=0;
			//::RectUnion(&childBounds,&bounds,&bounds);
		}
		else if(child->clipDepth>0)
		{
			iMask=child->clipDepth;
			clipRect = childBounds;
			continue;
		}
		if(setall||child->IsVisible())
			RectUnion(&childBounds,bounds,bounds);
	}
	if(!setall&&obj->clipObject)
	{
			SRECT childBounds;
			::RectSetEmpty(&childBounds);
			GetBoundingBox(obj->clipObject, &mat, &childBounds,true);
			::RectIntersect(bounds,&childBounds,bounds);
			//RectUnion(bounds, &childBounds, bounds);
	}
	if(obj->scrollRect)
	{
		clipRect.xmin=0;
		clipRect.ymin=0;
		clipRect.xmax=RectWidth(obj->scrollRect);
		clipRect.ymax=RectHeight(obj->scrollRect);
		MatrixTransformRect(&mat, &clipRect, &clipRect);
		::RectIntersect(bounds,&clipRect,bounds);
	}*/
	MATRIX mat;
	if(transSelf)
	{
		if(parentMatrix)
			MatrixConcat(&obj->xform.mat, parentMatrix, &mat);
		else
			mat=obj->xform.mat;
	}
	else
	{
		if(parentMatrix)
			mat=*parentMatrix;
		else
			::MatrixIdentity(&mat);
	}

	if(obj->character)
	{
		SRECT&rr=GETBOUNDS(obj->character,obj->editText);
		MatrixTransformRect(&mat, &rr, bounds);
	}
	
	
	int iMask=0;
	for (_XSObject* child = obj->bottomChild; child; child = child->above) {
		if(child->clipObject) 
		{
			SRECT childBounds;
			::RectSetEmpty(&childBounds);
			GetBoundingBox(child->clipObject, &mat, &childBounds);
			RectUnion(bounds, &childBounds, bounds);
			continue;
		}
		if(iMask)
		{
			if(child->depth<=iMask)
				continue;
			else
				iMask=0;
		}
		else if(child->clipDepth>0)
			iMask=child->clipDepth;
		SRECT childBounds;
		::RectSetEmpty(&childBounds);
		GetBoundingBox(child, &mat, &childBounds);
		RectUnion(bounds, &childBounds, bounds);
	}
	if(obj->scrollRect)
	{
		SRECT clipRect;
		clipRect.xmin=0;
		clipRect.ymin=0;
		clipRect.xmax=RectWidth(obj->scrollRect);
		clipRect.ymax=RectHeight(obj->scrollRect);
		MatrixTransformRect(&mat, &clipRect, &clipRect);
		::RectIntersect(bounds,&clipRect,bounds);
	}

}

void GetBoundingRect(SObject*obj, P_SRECT bounds)
{
	if(obj->character)
		*bounds=GETBOUNDS(obj->character,obj->editText);
	else
		::RectSetEmpty(bounds);
		//obj->character->bounds;
	for (_XSObject* child = obj->bottomChild; child; child = child->above) {
		SRECT childBounds;
		GetBoundingBox(child, XNULL, &childBounds);
		RectUnion(bounds, &childBounds, bounds);
	}
}


/*XBOOL _XSObject::HitTest(int x, int y,XBOOL bAnti)
{
	SObject*p=XNULL;
	SPOINT pt;
	pt.x=x*20;
	pt.y=y*20;
	//if(pt.y<0)
	//	int v=0;
	MATRIX dmat=character->player->rootObject->xform.mat;
	::MatrixConcat(&dmat,&display->camera.mat,&dmat);
	if(!bAnti)
	{
		SRECT rect;
		MATRIX mat;
		parent->GetTotalMatrix(mat,XTRUE);
		::MatrixTransformPoint(&dmat,&pt,&pt);
		GetBoundingBox(this,&mat,&rect);
		if(display->nAspect)
			::RectInset(-display->nAspect,&rect);
//		if(rect.ymax<140)
//			int v=0;
		return ::RectPointIn(&rect,&pt);
		//if(bOK)
		//	int v=0;
		//return bOK;
	}

	STransform t;
	
	MATRIX mat;//=display->camera.mat;
	//::MatrixIdentity(&mat);
	if(parent)
		parent->GetTotalMatrix(mat,XTRUE);
	else
		mat=display->camera.mat;
	::MatrixTransformPoint(&dmat,&pt,&pt);
	
	t.mat=mat;
	

	HitTest(p,t,&pt,XTRUE);
	if(p) 
		return XTRUE;
	return XFALSE;//p!=XNULL;
}

void _XSObject::HitTest(_XSObject*& hitObj, STransform x, SPOINT* pt,XBOOL bTest)
{
    if (!bTest&&(!IsVisible()||!IsEnable()))
        return;

	x.Concat(&xform);
	if ( HitTestOne(&x, pt,bTest) )
		hitObj = this;

	for ( _XSObject* obj = bottomChild; obj;  ) {
		if ( obj->clipDepth ) {
			obj = obj->HitClipper(hitObj,x, pt);
		} else {
			obj->HitTest(hitObj, x, pt,bTest);
			obj = obj->above;
		}
	}
}*/

inline bool _XSObject::HitTestOneChild(STransform x,SPOINT*pt,bool bClip)
{
	x.Concat(&xform);
	if(HitTestOne(&x,pt,false,bClip)) return true;
	for ( _XSObject* obj = bottomChild; obj;  ) {
		if(obj->HitTestOneChild(x,pt,bClip)) return true;
		obj = obj->above;		
	}
	return false;
}

void _XSObject::CalcButtonBounds(P_SRECT bounds)
{
	RectSetEmpty(bounds);
	
	MATRIX mat;
	MatrixIdentity(&mat);
	_XSObject* current = this;
	while (current && current != &display->root) {
		MatrixConcat(&mat, &current->xform.mat, &mat);
		current = current->parent;
	}
	MatrixConcat(&mat, &display->camera.mat, &mat);

	/*if(character&&pASObject)
	{
		switch(character->type)
		{
		case buttonEmptyChar:
		case buttonChar:
			 {
				 SObject*pObject=character->player->m_pAVM2->
								 GetButtonObject(pASObject,state);
				 if(pObject)
				 {
					pObject->CalcDevBounds(&mat);
					pObject->GetDevBound(bounds);
					//bounds=pObject->devBounds;
				 }
			 }
			 break;
		}
	}*/
	// Initialize button boundary
	
//#if (__CORE_VERSION__>=0x02070200)
	if(IsEditText()){
//#else
//	if (character && (character->type == editTextChar||character->type==editTextEmptyChar)) {
//#endif
		if(editText)
			MatrixTransformRect(&mat, &editText->editRect, bounds);
		return;
	}

	FLASHASSERT(character && character->type == buttonChar);
	if ( character && character->type == buttonChar ) {
		// Check the hit area of the button character
		ScriptPlayer* player = character->splayer;
		SParser parser;
		parser.Attach(character->data, 0);

		if ( character->tagCode == stagDefineButton2 ) 
			parser.GetWord();	// skip the action offset

		// Calculate union of all children's device boundaries.
		for (;;) {
			U8 stateFlags = parser.GetByte();
			if ( !stateFlags ) break;

			MATRIX m;
			U16 tag = parser.GetWord();
			U16 d = depth + parser.GetWord();
			parser.GetMatrix(&m);

			ColorTransform cx;
			if ( character->tagCode == stagDefineButton2 )
			{
				parser.GetColorTransform(&cx, XTRUE);
				if(stateFlags&sbtnFilterList)
				{
				   XU8 num=parser.GetByte();
				   //XFilter**root=&info.pFilter;
				   for(int i=0;i<num;i++)
				   {
					   XFilter*pFilter=XFilter::CreateFilter(&parser,display->player);
					   if(!pFilter) break;
					   //pFilter->pNext=XNULL;
					   display->player->FreeFilter( pFilter );
				   }
				}
				if(stateFlags&sbtnBlendMode)
				{
					parser.SkipBytes(1);
					//if(!pStream-ReadByte(blendMode)) return XFALSE;
				}
			}

			if ( stateFlags & sbtnHitTestState ) {
				// Check the child
				SCharacter* ch = player->FindCharacter(tag);
				if ( ch ) {
					_XSObject child;

					// Set up object
					memset(&child, 0, sizeof(child));
					child.display = display;
					//child.parent = 0;
					//child.above = 0;
					//child.bottomChild = 0;
					child.character = ch;
					child.depth = d;
					//child.ratio = 0;
					//child.name = 0;
					//child.thread = 0;
					child.xform.mat = m;
					child.xform.cxform.Clear();
					//child.state = 0;
					//child.drawn = 0;
					//child.dirty = 0;
					//child.clipDepth = 0;
					//child.edges = 0;
					//child.colors = 0;

					// Calculate the device matrix and bounds
					MATRIX childmat;
					MatrixConcat(&m, &mat, &childmat);
					child.CalcDevBounds(&childmat,NULL);

					// Calculate union of child's boundary with
					// boundary of button object so far
					RectUnion(&child.devBounds, bounds, bounds);
				}
			}
		}
	}
}

void GetBoundingRect(_XSObject* obj, P_MATRIX parentMatrix, P_SRECT bounds,XBOOL& bHaveEmptyChar, bool transSelf=true)
{
	
	MATRIX mat;
	if(transSelf)
	{
		if(parentMatrix)
			MatrixConcat(&obj->xform.mat, parentMatrix, &mat);
		else
			mat=obj->xform.mat;
	}
	else
	{
		if(parentMatrix)
			mat=*parentMatrix;
		else
			::MatrixIdentity(&mat);
	}

	if(obj->character)
	{
		switch(obj->character->type)
		{
		case bitmapEmptyChar://if(obj->character->type==bitmapEmptyChar&&RectIsEmpty(&obj->character->rbounds))
		case bitmapChar:
		case loaderImageChar:
		case bitsChar:
			 if(RectIsEmpty(&obj->character->rbounds))
			 {
				bounds->xmin=mat.tx;
				bounds->ymin=mat.ty;
				bounds->xmax=bounds->xmin;
				bounds->ymax=bounds->ymin;
				bHaveEmptyChar = XTRUE;
				break;
			 }
		default:
			SRECT&rr=GETBOUNDS(obj->character,obj->editText);	
			MatrixTransformRect(&mat, &rr, bounds);
			break;
		}
	}
	
	
	int iMask=0;
	for (_XSObject* child = obj->bottomChild; child; child = child->above) {
		if(child->IsEditText())
		{
			if(child->editText->m_length<=0) continue;
		}
		SRECT childBounds;
		::RectSetEmpty(&childBounds);
		GetBoundingRect(child, &mat, &childBounds,bHaveEmptyChar);
		RectUnion(bounds, &childBounds, bounds);
	}
	if(obj->scrollRect)
	{
		SRECT clipRect;
		clipRect.xmin=0;
		clipRect.ymin=0;
		clipRect.xmax=RectWidth(obj->scrollRect);
		clipRect.ymax=RectHeight(obj->scrollRect);
		MatrixTransformRect(&mat, &clipRect, &clipRect);
		::RectIntersect(bounds,&clipRect,bounds);
	}

}

void GetBoundingRectWidth(_XSObject* obj, P_MATRIX parentMatrix, P_SRECT bounds,XBOOL& bHaveEmptyChar,bool transSelf=true)
{
	
	MATRIX mat;
	if(transSelf)
	{
		if(parentMatrix)
			MatrixConcat(&obj->xform.mat, parentMatrix, &mat);
		else
			mat=obj->xform.mat;
	}
	else
	{
		if(parentMatrix)
			mat=*parentMatrix;
		else
			::MatrixIdentity(&mat);
	}

	if(obj->character)
	{
		switch(obj->character->type)
		{
		case bitmapEmptyChar://if(obj->character->type==bitmapEmptyChar&&RectIsEmpty(&obj->character->rbounds))
		case bitmapChar:
		case loaderImageChar:
		case bitsChar:
			 if(RectIsEmpty(&obj->character->rbounds))
			 {
				bounds->xmin=mat.tx;
				bounds->ymin=mat.ty;
				bounds->xmax=bounds->xmin;
				bounds->ymax=bounds->ymin;
				bHaveEmptyChar = XTRUE;
				break;
			 }
		default:
			SRECT&rr=GETBOUNDS(obj->character,obj->editText);
			MatrixTransformRect(&mat, &rr, bounds);
			break;
		}
	}
	
	
	int iMask=0;
	for (_XSObject* child = obj->bottomChild; child; child = child->above) {		
		SRECT childBounds;
		::RectSetEmpty(&childBounds);
		GetBoundingRectWidth(child, &mat, &childBounds,bHaveEmptyChar);
		RectUnion(bounds, &childBounds, bounds);
	}
	if(obj->scrollRect)
	{
		SRECT clipRect;
		clipRect.xmin=0;
		clipRect.ymin=0;
		clipRect.xmax=RectWidth(obj->scrollRect);
		clipRect.ymax=RectHeight(obj->scrollRect);
		MatrixTransformRect(&mat, &clipRect, &clipRect);
		::RectIntersect(bounds,&clipRect,bounds);
	}

}

void _XSObject::GetBoundBox(SRECT &rect, MATRIX *pMatrix,bool transSelf)
{
	GetBoundingBox(this,pMatrix,&rect,transSelf);
}

void _XSObject::GetBoundRect(SRECT &rect, MATRIX *pMatrix,bool transSelf)
{
	XBOOL b;
	GetBoundingRect(this,pMatrix,&rect,b,transSelf);
}

void _XSObject::GetBoundBox(SRECT &rect, XBOOL bWithCamera)
{
	MATRIX mat;
	if(parent)
		parent->GetTotalMatrix(mat,bWithCamera);
	else
		::MatrixIdentity(&mat);
	GetBoundingBox(this,&mat,&rect);
}

void GetBoundingBox(_XSObject* obj, P_SRECT bounds)
{
	MATRIX mat;
	MatrixIdentity(&mat);
	GetBoundingBox(obj, &mat, bounds);
}

// Helper for AddRect
/*void AddRectEdge(P_SPOINT a, P_SPOINT b,
			     RColor* color,
				 DisplayList* display, REdge** edgeList,
				 SStroker* stroker)
{
	if (stroker) {
		CURVE c;
		CurveSetLine(a, b, &c);
		stroker->AddStrokeCurve(&c);
		return;
	}

	if (a->y == b->y) {
		// Ignore horizontal lines
		return;
	}

	CURVE c;
	CurveSetLine(a, b, &c);

	int dir;
	if (c.anchor1.y <= c.anchor2.y) {
		dir = 1;
	} else {
		dir = -1;
		Swap(c.anchor1, c.anchor2, SPOINT);
	}

	REdge* edge = display->CreateEdge();
	if (!edge) {
		return;
	}

	edge->nextActive=NULL;
	// Set up the curve
	edge->Set(&c);
	edge->dir = (S8)dir;

	// Track the colors
	edge->fillRule = fillWindingRule;
	edge->color1 = color;
	edge->color2 = 0;

	// Add to the edge list
	edge->nextObj = *edgeList;
	*edgeList = edge;
}

// AddRect: Adds a transformed rectangle to the display list
void AddRect(P_SRECT rect,
			 P_MATRIX mat, RColor* color,
			 DisplayList *display, REdge** edgeList,
			 SStroker* stroker)
{
	// Corners in world space
	SPOINT TL, TR, BL, BR;

	TL.x = rect->xmin;
	TL.y = rect->ymin;
	MatrixTransformPoint(mat, &TL, &TL);

	TR.x = rect->xmax;
	TR.y = rect->ymin;
	MatrixTransformPoint(mat, &TR, &TR);

	BL.x = rect->xmin;
	BL.y = rect->ymax;
	MatrixTransformPoint(mat, &BL, &BL);

	BR.x = rect->xmax;
	BR.y = rect->ymax;
	MatrixTransformPoint(mat, &BR, &BR);

	if (stroker) {
		stroker->BeginStroke(1, color);
	}

	AddRectEdge(&TL, &BL, color, display, edgeList, stroker);
	AddRectEdge(&BL, &BR, color, display, edgeList, stroker);
	AddRectEdge(&BR, &TR, color, display, edgeList, stroker);
	AddRectEdge(&TR, &TL, color, display, edgeList, stroker);

	if (stroker) {
		stroker->EndStroke();
	}
}*/


void _XSObject::FreeFilter()
{
	if(!cacheMode&&!gridRect)
		FreeCacheBmp();
	XFilter*p=pFilter;
	int iCount=0;
	while(p)
	{
		XFilter*pNext=p->m_pNext;
		//delete p;
		display->player->FreeFilter(p);
		p=pNext;
		iCount++;
		if(iCount>10)
			break;
	}
	pFilter=XNULL;
}

//DEL void _XSObject::FreeAction()
//DEL {
//DEL 	XClipAction*p=pAction;
//DEL 	while(p)
//DEL 	{
//DEL 		XClipAction*pNext=p->pNext;
//DEL 		delete p;
//DEL 		p=pNext;
//DEL 	}
//DEL 	pAction=XNULL;
//DEL }

void _XSObject::UpdateClass(XXVar &className, XXObject *pSuper, XSWFCONTEXT &cnt)
{
	XBOOL bClass=XFALSE;
	if(!name.IsNull()&&
		XString8::Compare(name.strTxt,className.strTxt,0,XTRUE)==0)
		bClass=XTRUE;
	if(!bClass&&character&&character->className)
	{
		if(XString8::Compare(character->className,className.strTxt,0,XTRUE)==0)
			bClass=XTRUE;
	}
	if(bClass)
	{
		if(pObject&&pObject->m_pSuperObj==pSuper)
		{
		}
		else
		{
			//if(character&&character->tag==488)
			//	int v=0;

			XXObject*pObj=CreateObject(pSuper,&cnt);
			if(pObj)
				Constructor(pObj,&cnt);
		}
	}

	SObject*pObj=bottomChild;
	while(pObj)
	{
		pObj->UpdateClass(className,pSuper,cnt);
		pObj=pObj->above;
	}
}



XXObject* _XSObject::CreateObject(XXObject*pSuper,XSWFCONTEXT*pCnt)
{
//	return;	
	FreeObject();
	ScriptPlayer*player=character->splayer;
	if(!player||!player->m_pGlobal) return XNULL;
	
	/*if(player->IsAS3())
	{
		AS3CreateInstance(NULL);
		return XNULL;
	}*/

//	bConstructor=0;
	if(!pSuper)
	{
		if(character&&character->className)
		{
			
			char*realName=SkipPrefix(character->className,"__Packages.");
			if(realName)
			{
				XXObject*pObj=player->m_pGlobal;
				for(;;)
				{
					char*name=strrchr(realName,'.');
					XXVar var;
					if(name==XNULL)
					{
					  if(pObj->FindVariable(realName,var))
					  {
						  if(var.IsObject()) pSuper=var.pObject;
					  }
					  break;
					}
					else
					{
						char*newName=CreateStr(realName,name-realName);
						if(!pObj->FindVariable(realName,var)||!var.IsObject())
						{
							FreeStr(newName);
							break;
						}
						else pObj=var.pObject;
						FreeStr(newName);
						realName=name;
					}
				}
			}
			else
				pSuper=player->m_pGlobal->FindRegisterClass(character->className);
		}
		if(!pSuper&&!name.IsNull())//&&!pSuper)
			pSuper=player->m_pGlobal->FindRegisterClass(name.strTxt);
		
	}
	switch ( character->type ) {
		case buttonChar:
			pObject=player->m_pGlobal->CreateButton(this);
			flags|=OB_OBJECT;
			break;

#ifdef EDITTEXT
		case editTextChar:
			//if(character->tag==28)
			//	int v=0;
			pObject=player->m_pGlobal->CreateText(this);
			flags|=OB_OBJECT;
			break;
#endif
		case rootChar:	
		case spriteExternalChar:
  		case spriteChar: 
		case rootCloneChar:
		case spriteEmptyChar:
			//if(character->tag==109)
			//	int v=0;
			flags|=OB_OBJECT;
			pObject=player->m_pGlobal->CreateClip(this);
			break;
	}
	if(pObject)
	{
		pObject->RefObject();
		if(pSuper)
		{
#ifdef _ANALY
			if(!pSuper->m_strClassNameT.IsNull()&&pSuper)
				pObject->m_strClassNameT=pSuper->m_strClassNameT;
#endif

			//XSWFCONTEXT cnt;
			//player->splayer->InitContext(cnt);
			//cnt.InitContextA(XNULL,pObject,pObject,pObject,XNULL);
			//XXVARLIST list;
			pObject->SetSuper(pSuper,XFALSE);
			pObject->m_pSuperObj=pSuper->GetSuperObject();
			pSuper->CopyMembers(pObject);
			//pSuper->Construct(&cnt,&list,pObject);
			//bConstructor=1;
			//player->splayer->ReleaseContext(cnt);

		}
		//else
		//	pObject->m_pSuperObj=pObject->GetSuperObject();
	}
	SetInstance();
	return pSuper;
}

void _XSObject::Constructor(XXObject*pSuper,XSWFCONTEXT *pCnt)
{
	//return;
	if(!pSuper) return;
	ScriptPlayer*player=character->splayer;
	if(player==XNULL) return;
	//XU32 t1=XTime::GetTickCount();
	if(pCnt==XNULL)
	{
		XSWFCONTEXT cnt;
		player->splayer->InitContext(cnt);
		cnt.InitContextA(XNULL,pObject,pObject,pObject,pObject,XNULL);
		XU8 vars[sizeof(XXVar)*2];
		XXVARLIST list(vars,2);
		pSuper->Construct(&cnt,list,pObject);
		player->splayer->ReleaseContext(cnt);
		//cnt.pStack->FreePop();
		//display->PushAction((XU8*)pSuper->RefObject(),
		//					  this,pObject,XActionList::XA_FUNCTION,XNULL);
	}
	else
	{
		pCnt->InitContextA(XNULL,pObject,pObject,pObject,pObject,XNULL);
		XU8 vars[sizeof(XXVar)*2];
		XXVARLIST list(vars,2);
		XXStack stack(64);
		XXStack*pStack=pCnt->pStack;
		pCnt->pStack=&stack;
		//int s=pCnt->pStack->GetSize();
		pSuper->Construct(pCnt,list,pObject);
		pCnt->pStack=pStack;
		//display->PushAction((XU8*)pSuper->RefObject(),
		//					  this,pObject,XActionList::XA_FUNCTION,XNULL);
		//int s1=pCnt->pStack->GetSize();
		//while(s1>s)
		//
		//{
		//	s1--;
		//	pCnt->pStack->FreePop();
		//}
		//--;
		//
	}
	//XU32 t2=XTime::GetTickCount();
	//if(t2-t1>500)
	//	int v=0;
}



void _XSObject::SetAlpha(XFDOUBLE value)
{
	S16 aa = (S16) (value * 256/100);
	if ( xform.cxform.aa != aa ) {
		 xform.cxform.aa = aa;
		 xform.cxform.CalcFlags();
		 Modify();
	}
	puppet = XTRUE;
}

void _XSObject::SetScaleX(XFDOUBLE scale)
{
	//return;
	//if(scale<0) //return;
	//	int v=0;
	//if(scale>0) scale=-scale;
	//scale=scale/100;
	
	if(scale==control.sx) return;
	//int old=xform.mat.b;
	control.sx=scale;
	SFIXED scalex = (SFIXED)(scale*fixed_1/100);
		//DoubleToFixed(scale);

	//if(character&&character->tag==404)
	//	int v=0;

	//if(!FixedEqual(control.scaleX,scalex,0x10))
	{
		control.scaleX=scalex;
		xform.mat.a =  FixedMul(scalex, FixedCos(control.rotX));
		xform.mat.b =  FixedMul(scalex, FixedSin(control.rotX));
		//if(old==0&&xform.mat.b!=0)
		//	int v=0;
		Modify();
		//if(xform.mat.a>=10&&character&&character->type==textChar)
		//	int v=0;
	}
	/*SPOINT pt;
	pt.x = xform.mat.a;
	pt.y =  xform.mat.b;
	SFIXED curScalex = PointLength(&pt);
	if ( !FixedEqual(scalex, curScalex, 0x10) ) {
		SFIXED rotX = FixedAtan2( xform.mat.b,  xform.mat.a);
		xform.mat.a =  FixedMul(scalex, FixedCos(rotX));
		xform.mat.b =  FixedMul(scalex, FixedSin(rotX));
		Modify();
	}*/
	puppet = XTRUE;
}

void _XSObject::SetScaleY(XFDOUBLE value)
{
	//return;
	if(control.sy==value) return;
	control.sy=value;
	SFIXED scaley = DoubleToFixed(value/100);
	//SPOINT pt;
	//pt.x = xform.mat.c;
	//pt.y = xform.mat.d;
	//SFIXED curScaley = PointLength(&pt);
	//if ( !FixedEqual(scaley, control.scaleY, 0x10) ) 
	{
		//SFIXED rotY = FixedAtan2(-xform.mat.c, xform.mat.d);
		control.scaleY=scaley;
		xform.mat.c = -FixedMul(scaley, FixedSin(control.rotY));
		xform.mat.d =  FixedMul(scaley, FixedCos(control.rotY));
/*#if (__CORE_VERSION__>=0x02075000)
		if(this->gridRect&&xform.mat.d<0)
			xform.mat.d=-xform.mat.d;
#endif*/
		Modify();
	}
	puppet = XTRUE;
}

void _XSObject::SetRotate(XFDOUBLE v)
{
//	return;
//	if(character->type==rootChar)
//		int vv=0;
		/*MATRIX mat = xform.mat;
		SPOINT vec;
		vec.x = mat.a;
		vec.y = mat.b;
		SFIXED scalex = PointLength(&vec);

		vec.x = mat.c;
		vec.y = mat.d;
		SFIXED scaley = PointLength(&vec);

		SFIXED rotX = FixedAtan2(mat.b, mat.a);
		SFIXED rotY = FixedAtan2(-mat.c, mat.d);
		SFIXED newRot = DoubleToFixed(v);

		mat.a =  FixedMul(scalex, FixedCos(newRot));
		mat.b =  FixedMul(scalex, FixedSin(newRot));
		mat.c = -FixedMul(scaley, FixedSin(rotY-rotX + newRot));
		mat.d =  FixedMul(scaley, FixedCos(rotY-rotX + newRot));

		if ( !MatrixEqual(&mat, &xform.mat, 0x10) ) {	// note that we don't change the tx or ty values ever...
			xform.mat = mat;
			Modify();
		} else {
			//FLASHOUTPUT("skip\n");
		}*/
		//if(character->tag!=936&&character->tag!=951)
		//	int vi=0;
	//	if(character->tag==672)
	//		int v=0;

		//while(v>180) v-=360;
		//while(v<-180) v+=360;
		//return;
	    if(control.rotate==v) return;
		SFIXED newRot = DoubleToFixed(v);
		//if(FixedEqual(control.rotX,newRot,0x10)) 
		//	return;

		MATRIX mat = xform.mat;

		mat.a =  FixedMul(control.scaleX, FixedCos(newRot));
		mat.b =  FixedMul(control.scaleX, FixedSin(newRot));
		mat.c = -FixedMul(control.scaleY, FixedSin(control.rotY-control.rotX + newRot));
		mat.d =  FixedMul(control.scaleY, FixedCos(control.rotY-control.rotX + newRot));

		
		if ( !MatrixEqual(&mat, &xform.mat, 0x10) ) {	// note that we don't change the tx or ty values ever...
			xform.mat = mat;
			Modify();

			control.rotY = newRot+control.rotY-control.rotX;
			control.rotX = newRot;//FixedAtan2(xform.mat.b, xform.mat.a);
			control.rotate=v;

			
			
				//FixedAtan2(-xform.mat.c, xform.mat.d);
			//FixedDiv(control.rotY,control.rotX);
			//control.rotY = FixedMul(control.rotY,newRot);
				//newRot;//FixedAtan2(-xform.mat.c, xform.mat.d);
			//UpdateControl();
			//control.rotX=newRot;
			//control.rotY=control.rotY-control.rotX+newRot;//control.rotX-newRot;
		}

        puppet = XTRUE;
}

XFDOUBLE _XSObject::GetScaleX()
{
	//SPOINT vec;
	//vec.x = xform.mat.a;
	//vec.y = xform.mat.b;
	return control.sx;//FixedToDouble(control.scaleX)*100;
		//(control.scaleX*100)>>16;//FixedToDouble(control.scaleX*100);//PointLength(&vec)*100);
}

XFDOUBLE _XSObject::GetScaleY()
{
	//SPOINT vec;
	//vec.x = xform.mat.c;
	//vec.y = xform.mat.d;
	return control.sy;//FixedToDouble(control.scaleY)*100;////(control.scaleY*100)>>16; 
		//FixedToDouble(control.scaleY*100);//PointLength(&vec)*100);
}

XFDOUBLE _XSObject::GetRotate()
{
	//SFIXED rotX = FixedAtan2(xform.mat.b, xform.mat.a);
				//if ( rotX < 0 )
				//	rotX += FC(360);
	//if(character->tag==672)
	//	return 0;
	//		int vi=0;
	//return 0;
	//SFIXED rotX=control.rotX;
	//if(rotX<0)
	//	int v=0;
	//return rotX;
	return control.rotate;
	//return FixedToDouble(rotX);
}

XBOOL _XSObject::IsThread()
{
	//if(name==XNULL) return XFALSE;
	if(!character) return XFALSE;
	switch(character->type)
	{
	case spriteExternalChar:
	case spriteChar:
	case rootChar:
	case rootCloneChar:
	case spriteEmptyChar:
		 return XTRUE;
	}
	return XFALSE;
}


XBOOL _XSObject::IsNamedThread()
{
	if(name.IsNull()) return XFALSE;
	//if(strName.IsEmpty()) return XFALSE;
	if(!character) return XFALSE;
	switch(character->type)
	{
	case spriteExternalChar:
	case spriteChar:
	case rootChar:
	case rootCloneChar:
	case spriteEmptyChar:
	case loaderChar:
		 return XTRUE;
	}
	return XFALSE;
}

XFDOUBLE _XSObject::GetWidth()
{
	SRECT bounds;
	RectSetEmpty(&bounds);
	//GetBoundingBox(this, &bounds);
	MATRIX mat;
	MatrixIdentity(&mat);
	XBOOL bBmpEmpty = XFALSE;
	GetBoundingRectWidth(this,&mat,&bounds,bBmpEmpty);
	//if(bBmpEmpty)
	//{
	//	SPOINT pt;
	//	pt.x=xform.mat.tx;
	//	pt.y=xform.mat.ty;
	//	RectUnionPoint(&pt,&bounds);
	//}
	return (XFDOUBLE) (bounds.xmax - bounds.xmin) / 20;
}

XFDOUBLE _XSObject::GetHeight()
{
	SRECT bounds;
	::RectSetEmpty(&bounds);
	//GetBoundingBox(this, &bounds);
	//if(bounds.ymax - bounds.ymin<0)
	//	int v=0;
	MATRIX mat;
	MatrixIdentity(&mat);
	XBOOL bBmpEmpty = XFALSE;
	GetBoundingRectWidth(this,&mat,&bounds,bBmpEmpty);
	if(bBmpEmpty)
	{
		XPOINT pt;
		pt.x = xform.mat.tx;
		pt.y = xform.mat.ty;
		RectUnionPoint(&pt,&bounds);
	}
	return (XFDOUBLE) (bounds.ymax - bounds.ymin) / 20;
}

void _XSObject::SetWidth(XFDOUBLE v)
{
	XFDOUBLE oldv=GetWidth();
	if(oldv==v) return;
	if(oldv>0)
	{
		Scale(v/oldv,1);
		Modify();
	}
	else if(v!=0)
	{
		SFIXED fv=DoubleToFixed(v);
		control.scaleX=fv;//scalex;
		control.sx=v*100;
		xform.mat.a =  fv;//FixedMul(scalex, FixedCos(control.rotX));
		//xform.mat.b =  FixedMul(scalex, FixedSin(control.rotX));
		oldv=GetWidth();
		if(oldv)
			Scale(v/oldv,1);
		Modify();
		//SetScaleX(v);
	}
	//if(xform.mat.a>=10&&character&&character->type==textChar)
	//	int v=0;
	puppet = XTRUE;
}

void _XSObject::SetHeight(XFDOUBLE v)
{
	XFDOUBLE oldv=GetHeight();
	if(oldv==v) return;
	if(oldv>0)
	{
		Scale(1,v/oldv);
		Modify();
	}
	else if(v!=0)
	{

		SFIXED fv=DoubleToFixed(v);
		control.scaleY=fv;//scalex;
		control.sy=v*100;
		xform.mat.d =  fv;//FixedMul(scalex, FixedCos(control.rotX));
		//xform.mat.b =  FixedMul(scalex, FixedSin(control.rotX));
		oldv=GetHeight();
		if(oldv)
			Scale(1,v/oldv);
		Modify();
		//SetScaleX(v);
	}
	//	SetScaleY(v);
	puppet = XTRUE;
}


void _XSObject::SetBoundBox(SRECT &rect, XBOOL bCamera)
{
	SRECT r;
	GetBoundBox(r,bCamera);
	int w=r.xmax-r.xmin;
	int h=r.ymax-r.ymin;
	if(w>0&&h>0) 
	{
		XFDOUBLE fw=(XFDOUBLE)(rect.xmax-rect.xmin)/w;
		XFDOUBLE fh=(XFDOUBLE)(rect.ymax-rect.ymin)/h;
		Scale(fw,fh);
		Modify();
	}
	puppet=XTRUE;
}


void _XSObject::Scale(XFDOUBLE sx, XFDOUBLE sy)
{
	//MATRIX mat;
	if(sx!=1||sy!=1)
	{
		//if(sy>8)
		//	int vv=0;
		MATRIX mat;
		mat.a=DoubleToFixed(sx);
		mat.d=DoubleToFixed(sy);
		mat.tx=0;
		mat.ty=0;
		mat.c=0;
		mat.b=0;
		MatrixConcat(&mat,&xform.mat,&xform.mat);
/*#if (__CORE_VERSION__>=0x02075000)
		if(this->gridRect&&xform.mat.d<0)
			xform.mat.d=-xform.mat.d;
#endif*/
		//if(xform.mat.d>8*65536)
		//	int v=0;
		UpdateControl();
		Modify();
		//if(xform.mat.a>=10&&character&&character->type==textChar)
		//	int v=0;
	}
	puppet = XTRUE;
}

void _XSObject::GetTotalMatrix(MATRIX &mat, XBOOL bWithCamera)
{
	mat=xform.mat;
	SObject*p=parent;

	if(scrollRect)
	{
		//RColor*clip=clipColor;				
		MATRIX mt;
		::MatrixIdentity(&mt);
		mt.tx=-scrollRect->xmin;
		mt.ty=-scrollRect->ymin;
		::MatrixConcat(&mt,&mat,&mat);
	}

	while(p)
	{
		MatrixConcat(&mat,&p->xform.mat,&mat);

		if(p->scrollRect)
		{
			//RColor*clip=clipColor;				
			MATRIX mt;
			::MatrixIdentity(&mt);
			mt.tx=-p->scrollRect->xmin;
			mt.ty=-p->scrollRect->ymin;
			::MatrixConcat(&mt,&mat,&mat);
		}

		p=p->parent;
	}
	if(bWithCamera)
	{
		MatrixConcat(&mat,&display->camera.mat,&mat);
		MatrixConcat(&mat,&display->mControlMat,&mat);
	}
}

void _XSObject::GetTotalCXForm(XCXForm&form)
{
	form=xform.cxform;
	SObject*p=parent;	
	while(p)
	{
		//MatrixConcat(&mat,&p->xform.mat,&mat);
		XCXForm f=form;
		form=p->xform.cxform;
		form.Concat(&f);		
		p=p->parent;
	}
	//if(bWithCamera)
	//	MatrixConcat(&mat,&display->camera.mat,&mat);
}


_XSObject* _XSObject::GetChildOf(int nDepth)
{
	SObject*pObj=bottomChild;
	while(pObj)
	{
		//XBOOL bOK=!bThread||pObj->IsNamedThread();
		if(pObj->depth==nDepth)
			return pObj;
		pObj=pObj->above;
	}
	return XNULL;
}

int _XSObject::GetMaxDepth()
{
	SObject*pObj=bottomChild;
	while(pObj)
	{
		if(!pObj->above) 
			return pObj->depth;
		pObj=pObj->above;
	}
	return 0;
}


SObject* _XSObject::GetChildOf(XPCTSTR strName, XBOOL bThread)
{
/*	ScriptPlayer*p=character->player;
	XBOOL bLowCase=p?p->NeedLowCase():XFALSE;
	SObject*pObj=sorts->Find((char*)strName,bLowCase);
	if(pObj)
	{
		if(bThread&&pObj->IsNamedThread()) return pObj;
		return XNULL;
	}
	return XNULL;*/
	ScriptPlayer*p=character?character->splayer:this->CreatePlayer;
	SObject*pObj=bottomChild;
	XBOOL bLowCase=p?p->NeedLowCase():XFALSE;
	//if(p)
	{
		while(pObj)
		{
			if(pObj->character)//&&pObj->character->tag!=ctagSpritePlaceholder)
			{
				XBOOL bOK=!bThread||pObj->IsNamedThread();
				if(!pObj->name.IsNull()&&bOK)
				{
					if(XString8::Compare(pObj->name.strTxt,strName,0,bLowCase)==0) 
						return pObj;
				}
			}
			pObj=pObj->above;
		}
	}
	/*else
	{
		while(pObj)
		{
			XBOOL bOK=!bThread||pObj->IsNamedThread();
			if(pObj->name&&bOK)
			{
				if(XString8::Compare(pObj->name,strName,XFALSE)==0) 
					return pObj;
			}
			pObj=pObj->above;
		}
	}*/
	return XNULL;
}

ScriptThread* _XSObject::GetThread()
{
	if(IsThread()) return thread;
	return XNULL;
}

void _XSObject::GlobalToLocal(XFDOUBLE &x, XFDOUBLE &y)
{
	MATRIX mat0,mat;
	GetTotalMatrix(mat0,XFALSE);
	::MatrixInvert(&mat0,&mat);
	SPOINT pt;
	x=x*20;
	y=y*20;
	pt.x=XROUND(x);//FloatToFixed(x);
	pt.y=XROUND(y);//FloatToFixed(y);
	MatrixTransformPoint(&mat,&pt,&pt);
	x=pt.x/20.0;
	y=pt.y/20.0;
	//x=FixedToFloat(pt.x);
	//y=FixedToFloat(pt.y);
}

void _XSObject::LocalToGlobal(XFDOUBLE &x, XFDOUBLE &y)
{
	MATRIX mat;
	GetTotalMatrix(mat,XFALSE);//XTRUE);//XFALSE);
	//::MatrixInvert(&mat,&mat);
	SPOINT pt;
	x=x*20;
	y=y*20;
	pt.x=XROUND(x);//FloatToFixed(x);
	pt.y=XROUND(y);//FloatToFixed(y);
	MatrixTransformPoint(&mat,&pt,&pt);
	x=pt.x/20.0;
	y=pt.y/20.0;
}

XBOOL _XSObject::HitObject(SObject *pObject)
{
	if(pObject==XNULL) return XFALSE;
	//if(!IsVisible()||!pObject->IsVisible()) return XFALSE;
	SRECT r1,r2;
	RectSetEmpty(&r1);
	RectSetEmpty(&r2);
	
	/*this->GetDevBound(&r1);
	pObject->GetDevBound(&r2);
	
	int nOff=1;//<<display->raster.antialias_b;
	::RectInset(-nOff,&r1);*/
	GetBoundBox(r1,XFALSE);
	pObject->GetBoundBox(r2,XFALSE);	
	::RectInset(-1,&r1);
	if( ::RectTestIntersect(&r1,&r2))
	{
		
		//GetBoundBox(r1,XFALSE);
		//pObject->GetBoundBox(r2,XFALSE);
		/*RectSetEmpty(&r1);
		RectSetEmpty(&r2);
		MATRIX mat;
		MATRIX mat0;
		::MatrixIdentity(&mat);
		::MatrixIdentity(&mat0);
		GetTotalMatrix(mat,XFALSE);
		pObject->GetTotalMatrix(mat0,XFALSE);
		GetBoundingBox(this,&mat,&r1);
		GetBoundingBox(pObject,&mat0,&r2);*/
		return XTRUE;
	}
	return XFALSE;
}



void _XSObject::SwapDepth(SObject *pObject, int nDepth)
{
	//return;
	if(!bottomChild)
		return;
//	if(nDepth==16485)
//		int v=0;
	
	SObject*pDstObj=XNULL;
	SObject**p=&bottomChild;
	//SObject**pSrc=p,**pDst=p;
	XBOOL bSeek=XFALSE;
	int oldDepth=pObject->depth;
	//if(oldDepth==16485||nDepth==16485)
	//	int v=0;


	for(;;)
	{
		SObject*pObj=*p;
		if(!pObj)//||pObj->depth>nDepth)
		{
			break;
		}
		else if(pObj->depth==nDepth)
		{
			pDstObj=pObj;
			*p=pObj->above;
		}
		else if(pObj==pObject)
		{
			bSeek=XTRUE;
			*p=pObj->above;
		}
		else p=&pObj->above;
	}
	if(bSeek)
	{
		p=&bottomChild;
		for(;;)
		{
			SObject*pObj=*p;
			if(!pObj||pObj->depth>=nDepth)
			{
				if(!(pObject->flags&OB_CLONEOBJ))
				{
					//if(strcmp(pObject->name.strTxt,"StarZen")==0)
					//	int v=0;
					pObject->oldDepth=pObject->depth;
				}
				pObject->depth=nDepth;
				pObject->above=pObj;
				*p=pObject;
				//if(character->player->version<=7)
					pObject->flags|=OB_CLONEOBJ;
				break;
			}
			else
				p=&pObj->above;
		}
	}
	if(pDstObj)
	{
		p=&bottomChild;
		for(;;)
		{
			SObject*pObj=*p;
			if(!pObj||pObj->depth>=oldDepth)
			{
				pDstObj->depth=oldDepth;
				//->depth=nDepth;
				pDstObj->above=pObj;
				*p=pDstObj;
				break;
			}
			else
				p=&pObj->above;
		}
	}
/*	if(!pDst||!pSrc)
	   return;
	if(pDstObj)
		pDstObj->depth=pObject->depth;
	pObject->depth=nDepth;
	if(pDstObj)
	{
		pDstObj->above=(*pSrc)->above;
		*pSrc=pDstObj;
	}
	else
		*pSrc=(*pSrc)->above;
	if(!pDstObj)
		pObject->above=*pDst;
	else
		pObject->above=(*pDst)->above;
	*pDst=pObject;*/
}

void _XSObject::SwapDepth(SObject *pObject, XPCTSTR name)
{
	SObject* pObj=GetChildOf(name,XFALSE);
	if(pObj)
		SwapDepth(pObject,pObj->depth);
}

_XSObject* _XSObject::GetChildByIndex(XU32 index)
{
	_XSObject*pObj=bottomChild;
	while(index&&pObj)
	{
		pObj=pObj->above;
		index--;
	}
	return pObj;
}

void _XSObject::GetCXForm(XCXForm &form)
{
	//form.cxform=xform.cxform;
	//form.mat=xform.mat;
	form=xform.cxform;
#ifdef _ARGB_PIXEL_
	int tmp=form.ra;
	form.ra=form.ba;
	form.ba=tmp;
	tmp=form.rb;
	form.rb=form.bb;
	form.bb=tmp;
#endif
}

void _XSObject::SetCXForm(XCXForm &form)
{
	//xform.mat=form.mat;
	xform.cxform=form;
	//if(xform.cxform.nBlendMode)
	//	int v=0;
#ifdef _ARGB_PIXEL_
	int tmp=form.ra;
	xform.cxform.ra=xform.cxform.ba;
	xform.cxform.ba=tmp;
	tmp=xform.cxform.rb;
	xform.cxform.rb=xform.cxform.bb;
	xform.cxform.bb=tmp;
#endif
//	puppet=XTRUE;
	Modify();
}

void _XSObject::GetMatrix(MATRIX &mat)
{
	mat=xform.mat;
}

void _XSObject::SetMatrix(MATRIX &mat)
{
	if(!::MatrixEqual(&mat,&xform.mat,0x10))
	{
		Modify();
		xform.mat=mat;
		UpdateControl();
	}
	puppet=XTRUE;
	
}


XBOOL _XSObject::GetText(XXVar&var)//XString8 &strTxt)
{
	if (character) {
//#if (__CORE_VERSION__>=0x02070200)
		if(IsEditText()){
//#else
//		if (character->type == editTextChar) {
//#endif
			if(editText)
				(editText->GetBuffer(var));
			//strTxt.SetString(editText->GetBuffer(),STRING_ATTACH);
			//Modify();
			return XTRUE;
		}
	}
	return XFALSE;
}

XBOOL _XSObject::SetText(XPCTSTR strBuf)
{
	if (character) {
//#if (__CORE_VERSION__>=0x02070200)
		if(IsEditText()) {
//#else
//		if (character->type == editTextChar) {
//#endif
			if(editText)
				editText->SetBuffer(strBuf,false);
			//Modify();
			return XTRUE;
		}
	}
	return XFALSE;
}

//DEL XBOOL _XSObject::AddCommand(_XCOMMAND *pCmd)
//DEL {
//DEL 	if(!character||character->type!=drawChar) return XFALSE;
//DEL 
//DEL 
//DEL 	pCmd->pNext=XNULL;
//DEL 	if(character->cmds.head==XNULL)
//DEL 	{
//DEL 		character->cmds.head=pCmd;
//DEL 		character->cmds.tail=pCmd;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		character->cmds.tail->pNext=pCmd;
//DEL 	}
//DEL 
//DEL 	return XTRUE;
//DEL }

bool _XSObject::IsOverBounds(int x,int y,int w,int h,int nThick)
{
	SRECT&rr=GETBOUNDS(character,editText);
	if(RectIsEmpty(&rr)) return false;
	if(nThick<=0)
	{
		if(x<=rr.xmin&&y<=rr.xmin&&
			x+w>=rr.xmax&&y+h>=rr.ymax) return true;
	}
	else
	{
		if(x-nThick<=rr.xmin&&y-nThick<=rr.xmin&&
			x+w+nThick>=rr.xmax&&y+h+nThick>=rr.ymax) return true;
	}
	return false;
}

void _XSObject::UpdateBounds(int x,int y,int w,int h,int nThick)
{
	if(!character) return;
	SRECT r={x-nThick,x+w+nThick,y-nThick,y+h+nThick};
	SRECT&rr=GETBOUNDS(character,editText);
	RectUnion(&r,&rr,&rr);//&character->bounds,&character->bounds);
}


void _XSObject::UpdateBounds(int x, int y,int nThick)
{
	if(!character) return;
	//if(!nThick)
	{
		SPOINT pt;
		pt.x=x;
		pt.y=y;
		SRECT&rr=GETBOUNDS(character,editText);
		::RectUnionPoint(&pt,&rr);//&character->bounds);
	}
	/*else
	{
		SRECT rect={x-nThick,y-nThick,x+nThick,y+nThick};
		::RectUnion(&rect,&character->bounds,&character->bounds);
	}*/
}

inline bool AS3HasMouseHandle(void*pASObject)
{
	if(!pASObject) return false;
	return ((avmshell::DisplayObjectObject*)pASObject)->hasMouseEvent();
}

XBOOL _XSObject::HasClipEvent(int type)
{
	
	if(pASObject) 
	{
		switch(type)
		{
		case XEVENT_MOUSE:
			 return ((avmshell::DisplayObjectObject*)pASObject)->hasMouseEvent();
		case XEVENT_KEY:
			 return ((avmshell::DisplayObjectObject*)pASObject)->hasKeyEvent();
		default:
			 return ((avmshell::DisplayObjectObject*)pASObject)->hasAnyEvent();
		}
	}
	else if(CreatePlayer&&CreatePlayer->IsAS3()) return XFALSE;
	
	if(!IsThread()) return XFALSE;
	if(thread&&thread->pActions) 
	{
		switch(type)
		{
		case XEVENT_MOUSE:
			if(thread->nEventFlags&(CLIPMOUSEEVENT))
			return XTRUE;
			break;
		case XEVENT_KEY:
			if(thread->nEventFlags&(CLIPKEYEVENT))
				return XTRUE;
			break;
		case XEVENT_ALL:
			return thread->nEventFlags;
		}
	}
	if(pObject)//&&pObject->IsObject(XXOBJ_CLIP))
	{
		switch(type)
		{
		case XEVENT_MOUSE:
			return pObject->HasMouseEvent();
			break;
		case XEVENT_KEY:
			//if(thread->nEventFlags&(CLIPKEYEVENT))
			//	return XTRUE;
			return pObject->HasKeyEvent();
			break;
		case XEVENT_ALL:
			return pObject->HasAnyEvent();
			//return thread->nEventFlags;
		}
		//return pObject->HasEvent(type);
		//XXObjectEvent*p=(XXObjectEvent*)pObject;
		//if(p->m_pEvents||
          // p->m_events.GetSize()
	}
	return XFALSE;
}

bool _XSObject::hasPressEvent()
{
	if(character&&character->type==buttonChar) return true;	
	if(!IsThread()) return false;
	if(thread&&thread->pActions) 
	{
		if(thread->nEventFlags&(CLIPPOINTDOWN|CLIPPOINTUP|CLIPPRESS|CLIPRELEASE)) return true;
	}
	if(pObject)//&&pObject->IsObject(XXOBJ_CLIP))
	{
		if(pObject->HasPressEvent()) return true;
	}
	return false;
}

XBOOL _XSObject::PostEvent(XU32 nTime,XSWFPlayer *player, XU32 id,int nCode)
{
	XBOOL bOK=XFALSE;
		
	int nHit=0;
	if(character)
	{
		switch(character->type)
		{
		//case buttonChar:nHit=2;break;
		case spriteChar:
		case rootChar:
		case spriteExternalChar:
		case spriteEmptyChar:
		case rootCloneChar:
			 nHit=1;
			 if(OnEvent(nTime,player,id,XNULL,nCode)) bOK=XTRUE;
			 break;
		}
	}

	if(nHit&&id==XOBJEVENT_MOUSEUP&&!IsVisible())
	{
		int tf=XOBJEVENT_RELEASEOUTSIDE;
		//if(!IsVisible()&&IsEnable())
		//	tf|=XOBJEVENT_RELEASE;
		if(thread&&thread->HasEvent(tf))//(nHit==1&&thread->HasEvent(tf))||
		   //(pObject&&pObject->HasEvent(tf)))
		{
		   double x,y;
		   player->GetMousePos(XNULL,x,y);
		   XBOOL bIn=AS3HitTest(x,y,XTRUE);
		   if(bIn)
		   {
			   if(tf&XOBJEVENT_RELEASE)
				   thread->DoEvent(CLIPRELEASE,0);  
				//OnEvent(player,XOBJEVENT_RELEASE,XNULL,0);
		   }
		   else
			   thread->DoEvent(CLIPRELEASEOUTSIDE,0);
				//OnEvent(player,XOBJEVENT_RELEASEOUTSIDE,XNULL,0);
		}
	}

	SObject*p=bottomChild;
	while(p)
	{
		if(p->PostEvent(nTime,player,id,nCode))
			bOK=XTRUE;
		p=p->above;
	}
	return bOK;
}



void _XSObject::UpdateControl()
{

//	if(character&&character->tag==672)
//		int v=0;

	if(::MatrixIsIdentity(&xform.mat))
	{
		control.rotX=0;
		control.rotY=0;
		control.rotate=0;
		control.scaleX=fixed_1;
		control.scaleY=fixed_1;
		control.sx=100;
		control.sy=100;
		return;
	}
	control.rotX = FixedAtan2(xform.mat.b, xform.mat.a);
	control.rotate=(XFDOUBLE)control.rotX/fixed_1;
	//if(abs(control.rotate)>2&&abs(control.rotate-180)>2&&
	//   xform.mat.b==0)
	//  int v=0;
	//double dv=control.rotX/65535.0;
	//if(abs(dv)>1&&abs(dv-180)>1)
	//	int v=0;
	control.rotY = FixedAtan2(-xform.mat.c, xform.mat.d);
	//if(character->tag==672&&control.rotX>10000)
	//		int vi=0;

	SPOINT vec;
	vec.x = xform.mat.a;
	vec.y = xform.mat.b;
	control.scaleX=(PointLength(&vec));
	control.sx=(XFDOUBLE)control.scaleX*100/fixed_1;
	vec.x = xform.mat.c;
	vec.y = xform.mat.d;
	control.scaleY=(PointLength(&vec));
	control.sy=(XFDOUBLE)control.scaleY*100/fixed_1;

	//control.rotateX=FixedToDouble(rotX);
	//control.rotateY=FixedToDouble(rotY);
	//control.scaleX=FixedToDouble(scaleX);
	//control.scaleY=FixedToDouble(scaleY);
				//if ( rotX < 0 )
				//	rotX += FC(360);	
}

//DEL _XSObject* _XSObject::GetMemberOf(char *name)
//DEL {
//DEL 	SObject*p=GetChildOf(name,XFALSE);
//DEL 	if(!p&&pObject)
//DEL 	{
//DEL 		XXVar var;
//DEL 		if(pObject->FindVariable(name,var)&&var.IsObject())
//DEL 			p=var.pObject->m_pObject;
//DEL 	}
//DEL 	return p;
//DEL }

XBOOL _XSObject::IsVisibleAll()
{
	if(!IsVisible()) return XFALSE;
	SObject*p=parent;
	while(p)
	{
		if(!p->IsVisible()) return XFALSE;
	}
	return XTRUE;
}


XBOOL _XSObject::HasMouseEvent()
{
	SCharacter* ch = character;
//	return;
	if(ch)
	{
	if ( ch->tagCode == stagDefineButton2 ) {
		// Handle the new style button
		SParser parser;
		parser.Attach(ch->data,0);
		BOOL first = XTRUE;
		for (;;) {
			S32 link = parser.pos;
			int offset = parser.GetWord();
			if ( !first ) {
				int code = parser.GetWord();
				return XTRUE;
			}
			if ( !offset ) break;
			parser.pos = link + offset;
			first = XFALSE;
		}

	} else if(ch->tagCode == stagDefineButton){
		// Handle the old style button
		
		//if ( transition == bsOverDownToOverUp ) 
		{
			// Do the button action on the mouse up in
			SParser parser;
			parser.Attach(ch->data, 0);

			// Skip over the child data
			for (;;) {
				U8 stateFlags = parser.GetByte();
				if ( !stateFlags ) break;
				MATRIX m;
				parser.SkipBytes(4);//GetWord();
				//parser.GetWord();
				parser.GetMatrix(&m);
			}
		  return parser.pos<ch->length;
			// Handle the action
			//PushAction(parser.script+parser.pos,target->thread->rootObject,target->thread->rootObject->pObject,XActionList::XA_BLOCK);
		}
		}
		else if(IsThread())

		if(thread&&thread->pActions)
		{
			return thread->HasEvent(CLIPMOUSEEVENT);
			//return XTRUE;
		}
	}
	

	if(pObject)
		return pObject->HasMouseEvent();//HasAnyEvent();
	return XFALSE;
}

void _XSObject::SetMask(_XSObject *pObj)
{
	if(pObj)
	{
		pObj->clipDepth=depth?depth:-1;
		//if(pObj->character->tag==loaderChar)
		//	int v=0;
	}
	this->clipObject=pObj;
}

_XSObject* _XSObject::AS3GetMask()
{
	//if(clipObject==NULL) return NULL;
	//return clipObject;
	return clipObject;
}

//////////////////////////////////////////////////////////////////////////////////////
#include "avm2.h"



bool _XSObject::CallFrame(int iFrame)
{

	//if(iFrame==219)
	//	int v=0;
	//if(iFrame==112)
	//if(name.strTxt&&strstr(name.strTxt,"instance69"))
	//	int v=0;
	//	int v=0;
	if(pASObject)
	{
		AS3SetCalling();
		XSWFPlayer*player=XSWFPlayer::m_pInstance;
		bool bDoFrame = player->m_bDoFrame;
		player->m_bDoFrame = false;
		//ScriptPlayer*player=GetPlayer();
		bool ret=display->player->m_pAVM2->CallFrame(pASObject,iFrame);
		player->m_bDoFrame = bDoFrame;
		AS3ClearCalling();
		return ret;
	}
	return false;
}

void* _XSObject::GetContext()
{
	ScriptPlayer*player=GetPlayer();
	if(player) return player->m_pCodeContext;
	return NULL;
}

void* _XSObject::AS3ConstructInst(void*pSuper)
{
	if(!pASObject||!character) return NULL;
	int argc=0;
#ifdef _WINEMU64
	_int64 args[4];
#else
	int	args[4];
#endif
	switch(character->type)
	{
	case bitsChar:
		// return NULL;
		 argc=2;
		 character->splayer->BuildBits(character);
		 character->bits.CreateTexture();
		 character->bits.ReleaseData();
		 args[1]=(character->bits.width<<3) | kIntptrType;
		 args[2]=(character->bits.height<<3) | kIntptrType;
			 //AvmCore::intToAtom(character->bits.width);
		 break; 

	case loaderImageChar:
		 argc=2;		 
		 character->bits.PICreate(character->data,character->length);
		 character->bits.CreateTexture(false);
		 character->bits.ReleaseData();
		 args[1]=(character->bits.width<<3) | kIntptrType;
		 args[2]=(character->bits.height<<3) | kIntptrType;
			 //AvmCore::intToAtom(character->bits.width);
		 break; 
	}
	display->player->m_pAVM2->ConstructInstance(this,pSuper,pASObject,GetContext(),argc,args);
	return pASObject;
}

void* _XSObject::AS3CreateInst(void*pSuper,XBOOL bForce)
{
	//XBOOL bActive=pSuper!=NULL;
	//void*pSuper=NULL;
	//if(pSuper==NULL)
	//{
	//return NULL;
	//ScriptPlayer*player=character->player;
	//if(!player->m_bLoadedOK) 
	//	return NULL;
	//if(player->m_pLoaderInfo==NULL)
	//	player->AS3CreateLoader();
		//player->m_pLoaderInfo=player->m_pAVM2->CreateLoaderInfo(player);
	//if(character&&character->tag==372)
	//	int v=0;
	//bool bSet = false;
	if(!pSuper)
	{
		//bSet = true;
		if(character&&character->classEntry)
			pSuper=character->classEntry;
		if(!pSuper&&bForce&&display->player->m_pAVM2)
		{
			switch(character->type)
			{
			case videoChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_video);
				 break;
			case bitmapEmptyChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_bitmap);
				 break;
			case morphShapeChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_morphshape);
				 break;
			case textChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_statictext);
				 break;
			case shapeChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_shape);
				 break;
			case buttonChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_button);
				 break;
			case editTextChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_edittext);
				 break;
		    case rootCloneChar:
			case rootChar:
			case spriteExternalChar:
  			case spriteChar: 
			case spriteEmptyChar:
				 pSuper=display->player->m_pAVM2->GetDefaultClass(avmshell::XAS3_CLASS_movieclip);
				 break;
			}
		}
	}
	//}
	if(pSuper)
	{
		void*parentObj=NULL;
		if(parent)
			parentObj=parent->pASObject;
		display->player->m_pAVM2->CreateInstance(this,(avmplus::ScriptObject*)pSuper,&pASObject,GetContext(),parentObj);//,this,player->m_pContext);
		/*if(bActive&&pASObject)
		{
			if(thread)
			{
				player->m_pAVM2->CallFrame(thread->curFrame
			}
			player->m_pAVM2->DispatchEvent(pASObject,avmshell::XAS3_EVENT_activate,0,NULL);
		}*/
		this->SetInstance();
		//if(bSet)
			//pASObject = (void*)1;
	}
	this->pSuper=pSuper;
	return pSuper;
	//return pASObject;
}


void _XSObject::FreeObject(bool bFinal)
{

	if(pASObject)
	{
		if(IsRunning())
		{
			//ScriptPlayer*player=character->player;
			void*parentObj=NULL;
			if(parent)
				parentObj=parent->pASObject;
			display->player->m_pAVM2->ReleaseInstance(&pASObject,GetContext(),true,parentObj);//this->IsDisplayObject());		
		}
	}

	if(pObject==XNULL) return;
	
	switch ( character->type ) {
		case buttonChar:

			((XXObjectButton*)pObject)->Destroy();

			//m_pObject=XXObjectCreator::CreateButton(this);
			break;

#ifdef EDITTEXT
		case editTextChar:
			((XXObjectText*)pObject)->Destroy();
			//m_pObject=XXObjectCreator::CreateText(this);
			break;
#endif
		case rootCloneChar:
		case rootChar:
		case spriteExternalChar:
  		case spriteChar:
		case spriteEmptyChar:
			//=XXObjectCreator::CreateClip(this);
			//if(name&&strcmp(name,"splashMC")==0)
//			if(character->tag==138)
//				int v=0;
			((XXObjectClip*)pObject)->Destroy();
			break;
		default:
			pObject->Destroy();
			break;
	}
	//if(name)
	//	int v=0;
	/*if(flags&OB_OBJECT)
		pObject->Destroy();
	else
	{
		pObject->m_pObject=XNULL;
		pObject->FreeObject();
	}*/
	//pObject=XNULL;

	pObject=XNULL;
}

/*XBOOL _XSObject::AS3DoEvent(void*pEvent)
{
	if(!pASObject) return false;
	return character->player->m_pAVM2->DispatchEvent(pEvent,pASObject);
}*/

/*XBOOL _XSObject::AS3PostEvent(void*pEvent)
{
	if(!character) return XFALSE;
	XBOOL bOK=XFALSE;
	ScriptPlayer*player=character->player;
	if(!player->m_pAVM2) return bOK;
		//player->m_pAVM2->CallFrame(pASObject,iFrame);
	if(pASObject)
		player->m_pAVM2->DispatchEvent(pASObject,pEvent);
	SObject*p=bottomChild;
	while(p)
	{
		if(p->AS3PostEvent(pEvent))
			bOK=XTRUE;
		p=p->above;
	}
	return bOK;
}*/
int _XSObject::AS3FillMembers(void* pObj,void*p)
{
	int iCount=0;
	avmshell::XAVM2* pAVM2=(avmshell::XAVM2*)p;
	
	SObject*ps=bottomChild;
	while(ps)
	{
		if(ps->pASObject&&ps->AS3IsNamed()&&!ps->name.IsNull())
		{
			if(pAVM2->SetMember(pObj,ps->name.strTxt,ps->pASObject))
				ps->SetAS3Flag();
		}
		ps=ps->above;
	}

	return iCount;
}

XBOOL _XSObject::AS3SetText(XPCWSTR strBuf,int l)
{	
	if (character&&editText) {
//#if (__CORE_VERSION__>=0x02070200)
		if(IsEditText()){
//#else
//		if (character->type == editTextChar) {
//#endif
			editText->SetBuffer(strBuf,l);
			//Modify();
			if(editText->m_pFormat->child)
			{
				editText->m_pFormat->child->iEnd=l;
			}
			return XTRUE;
		}
	}
	return XFALSE;
}


/*XBOOL _XSObject::AS3OnEvent(XSWFPlayer*player,int id,int type,int argc,int*args)
{
	if(pASObject)
	{
		void*pEvent=NULL;
		avmshell::XAVM2*pAVM2=player->m_pAVM2;
		 pEvent=pAVM2->CreateEventObject(id,type,false,0,NULL);
		if(pEvent)
			return pAVM2->DispatchEvent(pEvent,pASObject);
		
	}
	return false;
}*/



XBOOL _XSObject::OnEvent(XU32 nTime,XSWFPlayer *player, XU32 id,XXVar*pArg,int nCode)//_XSObject*pObj)
{
	if(player==XNULL||!pObject) return XFALSE;
	if(!character) return XFALSE;
	if(nTime&&nTime<nCreateTime) return XFALSE;
//	if(character->tag==27)
//		int v=0;
	switch(character->type)
	{
		case buttonChar:break;
#ifdef EDITTEXT
		case editTextChar:break;
#endif
		case rootCloneChar:
		case rootChar:	
		case spriteExternalChar:
  		case spriteChar:
		case spriteEmptyChar:
			 switch(id)
			 {
			 case XOBJEVENT_DATA:
				  //thread->DoEvent(CLIPDATA,0);break;
				  break;
			 case XOBJEVENT_DRAGOUT://=1,
				  if(thread)
				  thread->DoEvent(CLIPDRAGOUT,0);break;
			 case XOBJEVENT_DRAGOVER://=2,
				  if(thread)
				  thread->DoEvent(CLIPDRAGOVER,0);break;
			 case XOBJEVENT_ENTERFRAME:
				  if(thread)
				  thread->DoEvent(CLIPENTERFRAME,0);break;
			 case XOBJEVENT_KEYDOWN:
				  if(thread)
				  thread->DoEvent(CLIPKEYDOWN|CLIPKEYPRESS,nCode);break;
			 case XOBJEVENT_KEYUP://=5,
				  if(thread)
				  thread->DoEvent(CLIPKEYUP,nCode);break;
			 case XOBJEVENT_KILLFOCUS:
//				  thread->DoEvent(CLIPKILLFOCUS);break;
				  break;
			 case XOBJEVENT_LOAD:
				  if(thread)
				  thread->DoEvent(CLIPLOAD|CLIPINIT|CLIPCONSTRUCT,0);break;
			 case XOBJEVENT_MOUSEDOWN:
				  if(thread)
				  thread->DoEvent(CLIPPOINTDOWN,0);break;
			 case XOBJEVENT_MOUSEMOVE:
				  if(thread)
				  thread->DoEvent(CLIPPOINTMOVE,0);break;
			 case XOBJEVENT_MOUSEUP:
				  if(thread)
				  thread->DoEvent(CLIPPOINTUP,0);break;
			 case XOBJEVENT_PRESS:
				  //break;
				  if(thread)
				  thread->DoEvent(CLIPPRESS,0);break;
			 case XOBJEVENT_RELEASE:
				  if(thread)
				  thread->DoEvent(CLIPRELEASE,0);break;
			 case XOBJEVENT_RELEASEOUTSIDE:
				  if(thread)
				  thread->DoEvent(CLIPRELEASEOUTSIDE,0);break;
			 case XOBJEVENT_ROLLOUT:
				  if(thread)
				  thread->DoEvent(CLIPROLLOUT,0);break;
			 case XOBJEVENT_ROLLOVER:
				  if(thread)
				  thread->DoEvent(CLIPROLLOVER,0);break;
			 case XOBJEVENT_SETFOCUS:
				  break;
			 case XOBJEVENT_UNLOAD:
				  if(thread)
				  thread->DoEvent(CLIPUNLOAD,0);break;
			 }
			 break;
		default:return XFALSE;
	}
	//XXObjectEvent*pEvent=(XXObjectEvent*)pObject;
	//XXObjectClip*pClip=
	/*switch(id)
	{
		case XOBJEVENT_MOUSEDOWN:
		case XOBJEVENT_MOUSEMOVE:
		case XOBJEVENT_MOUSEUP:
		case XOBJEVENT_PRESS:
		case XOBJEVENT_RELEASE:
	} */

	if(pObject->HasAnyEvent()&&thread)//pEvent->IsObject(XXOBJ_CLIP))
	{
//		if(character->tag==13)
//				int v=0;
		//XXObjectEvent*pEvent=(XXObjectEvent*)pObject;
		XXObject*pClip=(XXObject*)thread->rootObject->pObject;
		if(pArg==XNULL)
			return pObject->HandlerEvent(player,pClip,id,XNULL);
		else
		{
			XU8 vars[sizeof(XXVar)*2];
			XXVARLIST list(vars,2);
			list.Push(*pArg);
			//list.Add(new XXVar(*pArg));
			XBOOL bOK=pObject->HandlerEvent(player,pClip,id,&list);
			list.Release();
			//list.FreeAll();
			return bOK;
		}
	}

	


	return XTRUE;
	//XSWFPlayer* splayer = m_character->player->splayer;
	//if(!splayer) return
}

U16* _XSObject::AS3GetStaticText(int&l)
{
	//return NULL;
	if(!character||character->type!=textChar) return NULL;
	U16*chars=NULL;
	SParser parser;
	parser.Attach(character->data, 0);
	BOOL getAlpha = character->tagCode == stagDefineText2;
	MATRIX mat;
	parser.GetMatrix(&mat);
	int nGlyphBits = parser.GetByte();
	int nAdvanceBits = parser.GetByte();
	int n=0,offset=0;
	SCharacter* font = 0;
	l=0;
	//int nGlyphBits = parser.GetByte();
	//int nAdvanceBits = parser.GetByte();
	while ( XTRUE ) 
	{
		if ( n == 0 ) 
		{
					// Get a new run
			int code = parser.GetByte();
			if ( !code ) break;

			if ( code & tflagsFont ) 
			{
				font = character->splayer->FindCharacter(parser.GetWord());
				if(!font||font->tagCode!=stagDefineFont3) return chars;
			}
			if ( code & tflagsColor ) 
			{
				// Set up a new color
				parser.GetColor(getAlpha);
			}
			if ( code & tflagsX ) {
				parser.GetWord();
			}
			if ( code & tflagsY ) {
				parser.GetWord();
			}
			if ( code & tflagsHeight ) {
				parser.GetWord();
			}
			n = parser.GetByte();
			l = n;
			parser.InitBits();
			chars=new U16[n];
			if(!chars) return chars;
		}

		{// Handle a glyph
			int g = (int)parser.GetBits(nGlyphBits);
			S32 advance = parser.GetSBits(nAdvanceBits);

			if ( font ) {
				U8* s;				


				if(font->font.flags & sfontFlagsWideOffsets) {
					s = font->data + 4*font->font.nGlyphs;
					offset = (U32)s[0] | ((U32)s[1]<<8) | ((U32)s[2]<<16) | ((U32)s[3]<<24);
				} else {
					s = font->data + 2*font->font.nGlyphs;
					offset = (U16)s[0] | ((U16)s[1]<<8);
				}
				offset+=g*2;
				chars[l-n]=*((U16*)(font->data+offset));
			}
		}
		n--;
	}
	return chars;
}



AS3ONEVENT(_XSObject,enterFrame)
AS3ONEVENT(_XSObject,added)
AS3ONEVENT(_XSObject,addedToStage)
AS3ONEVENT(_XSObject,removed)
AS3ONEVENT(_XSObject,removedFromStage)
AS3ONEVENT(_XSObject,render)
AS3ONEVENT(_XSObject,change)
AS3ONEVENT(_XSObject,scroll)

bool _XSObject::AS3_OnLink(U16* data,U8*s8)
{
	if(!pASObject) return false;
	avmshell::XAVM2*pAVM2=display->player->m_pAVM2;
	pAVM2->OnTextEvent(XAS3_EVENT_link,data,s8,pASObject,GetContext());
	return false;
}
bool _XSObject::AS3_OnTextinput(U16*s16,U8*s8)
{
	if(!pASObject) return false;
	avmshell::XAVM2*pAVM2=display->player->m_pAVM2;
	pAVM2->OnTextEvent(XAS3_EVENT_textInput,s16,s8,pASObject,GetContext());
	return false;	
}

bool _XSObject::AS3Instance(bool bAdd,void*pObj)
{
	//void*pSuper=XNULL;
	bool bAS3=GetPlayer()?GetPlayer()->IsAS3():false;
	bool bInit=false;

	
	if(this->IsNotInstance()||this->IsNotInitChild())
	{
		this->SetInitChild();
		if(this->IsNotInstance())
		{
			bInit=true;
			if(bAS3)
			{
				pSuper=this->AS3CreateInst(NULL,XTRUE);
				if(pASObject&&pObj&&this->AS3IsNamed()&&!name.IsNull())
				{
					if(display->player->m_pAVM2->SetMember(pObj,name.strTxt,pASObject))
						SetAS3Flag();
				}
			}
			else
			{
				pSuper=this->CreateObject(NULL,NULL);
				this->AS2SetInit();
			}
		}
		UpdateControl();
		if(character)
		{
		switch ( character->type ) 
		{
			case rootCloneChar:
			case rootChar:
				 break;
			case buttonChar: 
				 {
					// Set up the button state
				    if(pASObject)
						display->AS3SetStateObjects(this,pASObject,0xff);
					int state = bsIdle;
					if ( depth == display->buttonDepth 
						 && parent == display->buttonParent 
						 && display->buttonCharacter == character 
						 && ratio == display->buttonRatio ) {
						// The button is currently being tracked
						display->button = this;
						state = display->buttonState;
					}
					this->state=bsNull;
					display->UpdateButton(this,state,bAS3);									
				} break;

	#ifdef EDITTEXT
			case editTextChar:
				if(!editText)
				{
					editText = display->CreateEditText();//new EditText(obj);
					editText->Init(this);	
					editText->UpdateFromVariable(XTRUE);
				}
				break;
	#endif			
			case spriteEmptyChar:
  			case spriteChar: {
				if ( thread ) {					
					if(bAdd&&!thread->m_bInDisplay)//!thread->rootObject->IsInDisplay())//thread->m_bInDisplay)
						display->AddThread(thread);

					//thread->AS3BuildSecnesAndFrames(pASObject);
					thread->DrawFrame(0, XFALSE,XFALSE);
					thread->m_bAS3Frame0=false;
					AS3SetCallFrame();
					bInit=true;
					//bFrame=true;
				}
			} break;
		}

	//	if(obj->name)
	//	{
	//		parent->sorts->Add(obj,obj->character->player->NeedLowCase());
	//	}
		//int flags=SObject::OB_VISIABLE|SObject::OB_ENABLE;
		if(character)
		{
			XConfig*pConfig=display->player->GetConfig();
			if(pConfig->m_nAutoTag)//&&!pConfig->m_strTagPlace.IsEmpty())
			{
				if(character->tag==pConfig->m_nAutoTag)
				{
					if(pConfig->m_nTagPlace||
					   (parent->character&&pConfig->m_nTagPlace==parent->character->tag))
					   display->player->PostScore();
				}
			}
			int idParent=0;
			if(parent&&parent->character) idParent=parent->character->tag;
			int id=pConfig->FindShowMap(character->tag,idParent);
			if(id>=0)
			{			    
					_TAGSHOW&show=pConfig->m_showMaps[id];
					if(show.nZoom==0)
						flags&=~SObject::OB_VISIABLE;
						//flags=SObject::OB_ENABLE;
					else
					{
						MATRIX mat;
						mat.a=show.nZoom;
						mat.d=show.nZoom;
						mat.c=0;
						mat.b=0;
						mat.tx=0;//ow.offsetx;
						mat.ty=0;//.offsety;
						//if(xform.mat.a>=10&&character&&character->type==textChar)
						//	int v=0;
						//if(show.align!=_TAGSHOW::TOPLEFT)
						{
							SRECT rect;
							GetBoundingBox(this,&rect);
							int cx=(rect.xmin+rect.xmax)/2;
							int cy=(rect.ymin+rect.ymax)/2;
							int w=(rect.xmax-rect.xmin);
							int h=(rect.ymax-rect.ymin);
							int zz=show.nZoom;
							switch(show.offsetx)
							{
							case 0:
								mat.tx=-cx*(zz-fixed_1)/fixed_1;
								break;
							case 1:
								mat.tx=-rect.xmax*(zz-fixed_1)/fixed_1;
								break;
							case -1:
								mat.tx=-rect.xmin*(zz-fixed_1)/fixed_1;
								break;
							default:
								mat.tx=-cx*(zz-fixed_1)/fixed_1+FixedMul(w,show.offsetx);
							}
							switch(show.offsety)
							{
							case 0:
								mat.ty=-cy*(zz-fixed_1)/fixed_1;
								break;
							case 1:
								mat.ty=-rect.ymax*(zz-fixed_1)/fixed_1;
								break;
							case -1:
								mat.ty=-rect.ymin*(zz-fixed_1)/fixed_1;
								break;
							default:
								mat.ty=-cy*(zz-fixed_1)/fixed_1+FixedMul(h,show.offsety);
							}
						}
						::MatrixConcat(&xform.mat,&mat,&xform.mat);						
						UpdateControl();
					}
			}
		}
		}
		
	}
	else if(AS3IsNamed()&&!name.IsNull())
	{
		if(display->player->m_pAVM2->SetMember(pObj,name.strTxt,pASObject))
						SetAS3Flag();
	}

	/*_XSObject*obj=this->bottomChild;
	while(obj)
	{
		obj->AS3Instance();
		obj=obj->above;
	}*/

	/*if(bInit)
	{		
		if(bAS3)
		{
			this->AS3ConstructInst(pSuper);
			void*pContext=XNULL;
			if(character)
				pContext=character->player->m_pContext;
			//this->AS3_OnAddtotheStage();
			if(pASObject)
				display->player->m_pAVM2->OnEvent(XAS3_EVENT_addedToStage,false,pASObject,pContext);
			if(pASObject)
				display->player->m_pAVM2->OnEvent(XAS3_EVENT_added,true,pASObject,pContext);
		}
		else
		{
			this->Constructor((XXObject*)pSuper,XNULL);
			OnEvent(0,display->player,XOBJEVENT_LOAD,XNULL);
		}
	}
	if(bFrame)
	{
		thread->m_bAS3Frame0=false;
		CallFrame(0);
	}*/
	return bInit;
}

void _XSObject::AS3Construct()
{

	if(pSuper)
	{		
		if(pASObject)
		{
			this->AS3ConstructInst(pSuper);
			void*pContext=XNULL;
			if(character&&thread)
			{
				pContext=GetContext();
				if(character->type!=bitsChar&&thread->m_bInDisplay)//IsInDisplay())//thread->m_bInDisplay)
				{
					if(pASObject)
						display->player->m_pAVM2->OnEvent(XAS3_EVENT_addedToStage,false,pASObject,pContext);
					if(pASObject)
						display->player->m_pAVM2->OnEvent(XAS3_EVENT_added,true,pASObject,pContext);
				}
			}
		}
		/*else
		{
			this->Constructor((XXObject*)pSuper,XNULL);
			OnEvent(0,display->player,XOBJEVENT_LOAD,XNULL);
		}*/
	}
	if(display->player->m_pAVM2==NULL&&AS2NeedInit())
	{
		this->Constructor((XXObject*)pSuper,XNULL);
		if(thread)
			OnEvent(0,display->player,XOBJEVENT_LOAD,XNULL);
		AS2ClearInit();
	}
	if(pASObject&&AS3NeedCallFrame()&&thread&&thread->m_bInDisplay)//IsInDisplay())//&&thread&&thread->m_bInDisplay)
	{
		AS3ClearCallFrame();
		thread->m_bAS3Frame0=false;
		CallFrame(0);
	}
}


bool _XSObject::AS3HasMouseEvent()
{
	//Mye be bug
	//if(IsEnable())
	if(!this->IsDisplayObject()) return false;
	if(!this->IsMouseEnabled()) return false;
	if(IsEnable()&&AS3HasMouseHandle(pASObject)) //;
		return XTRUE;
		//return AS3HasMouseHandle(pASObject);
		//return false;
	ScriptPlayer*player=NULL;
	if(character)
	{
		player=GetPlayer();
		switch(character->type)
		{
		case textLineChar:
		case editTextChar:
		case editTextEmptyChar:
			 if(!editText) return false;
			 if(editText->m_bURL) return true;
			 if(editText->m_flags&seditTextFlagsWasStatic)
				 break;
			 else if(!editText->IsSelectable()&&!editText->IsEditAble())//&&!editText->m_bURL)
			 {					 
				 break;
			 }
			 //else if(!editText->IsEditAble())//if(!display->player->m_pAVM2)
			 //{
				///*_XSObject*obj=parent;
				//bool bOK=true;
				//while(obj)
				//{
				//	if(obj->hasPressEvent())
				//	{
				//		bOK=false;
				//		break;
				//	}
				//	obj=obj->parent;
				//}
				//if(!bOK) break;*/
				// break;
			 //}
			 
		case buttonChar:
		case buttonEmptyChar:
			 return true;
		}
	}
	return HasClipEvent(XEVENT_MOUSE)!=0;
}
bool _XSObject::AS3HasKeyEvent()
{
	if(character)
	{
		switch(character->type)
		{
//#if (__CORE_VERSION__>=0x02070200)
		case textLineChar:
//#endif
		case editTextChar:
		case editTextEmptyChar:
			 if(!editText)
				 break;
			 if(!editText->IsSelectable()) break;
			 return true;
		}
	}
	return HasClipEvent(XEVENT_KEY)!=0;
}

inline _XSObject* _XSObject::AS3HitObjectClipper(_XSObject*&next,STransform x,SPOINT*pt)
{
	/*if(clipDepth>0)
	{
		//x.Concat(&this->xform);
		if(HitTestOneChild(x,pt))
		{
			next=above;
			return XNULL;
		}
		else 
		{
			next=above;
			while(next)
			{
				if(next->depth>clipDepth) return XNULL;
				next=next->above;
			}
		}
	}
	else
	{
		//STransform x1=x;
		//x1.Concat(&xform);
		XBOOL bHit=HitTestOneChild(x,pt);
		next=above;
		XBOOL hitOne=false;
		SRECT rect;
		::RectSetEmpty(&rect);
		while(next)
		{
			if(bHit||next->clipObject==this)
			{
				if(next->clipDepth)
				{
					if(next->HitButtonClipper(next,hitObj,hitClip,x,pt))
						hitOne=true;
				}
				else
				{
					if(next->HitButton(hitObj,hitClip,x,pt,&rect))
						hitOne=true;
					next=next->above;
				}
			}
			else
				next=next->above;
		}
		return hitOne;
	}
	return false;*/
	return XNULL;
}
bool _XSObject::AS3HitTest(STransform x,SPOINT*pt,bool bTest)
{
	x.Concat(&xform);
	if(scrollRect)
	{
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.tx=-scrollRect->xmin;
		mat.ty=-scrollRect->ymin;
		::MatrixConcat(&mat,&x.mat,&x.mat);
	}
	if ( HitTestOne(&x, pt,bTest) )
		return true;

	for ( _XSObject* obj = bottomChild; obj;  ) {
		if ( obj->clipDepth ) {
			if(obj->AS3HitTestClipper(obj,x, pt,bTest)) return true;
		} else {
			if(obj->AS3HitTest(x, pt,bTest)) return true;
			obj = obj->above;
		}
	}
	return false;
}
inline bool _XSObject::AS3HitTestClipper(_XSObject*&next,STransform x,SPOINT*pt,bool bTest)
{
	if(clipDepth>0)
	{		
		//x.Concat(&xform);
		if ( HitTestOneChild(x, pt,true) ) 
		{
		// It hit the clipper, so check the children
			next=above;			
		} else {
			//if(!this->IsMouseChildren()) bButton=false;
			// We missed the clipper so skip the children
			next = above; 
			while ( next ) {				
				if ( !next || next->depth > clipDepth )
					return false;
				if(next->depth<0&&next->clipObject!=this)
				{
					if(next->AS3HitTest(x,pt,bTest)) return true;
				}
				next = next->above;
			}
		}
		return false;
	}
	else
	{
		//STransform x1=x;
		//x1.Concat(&xform);
		XBOOL bHit=HitTestOneChild(x,pt,true);
		{
			//_XSObject* obj = above; 
			next=above;
			while ( next ) {
				if(bHit||next->clipObject!=this)
				{
					/*if(next->clipDepth)
					{
						if(AS3HitTestClipper(next,x,pt,bTest)) return true;
					}
					else*/
					{
						if(AS3HitTest(x,pt,bTest)) return true;
						next=next->above;
					}
					//if(obj->HitTestOne(x,pt)) return obj;
				}
				else
					next = next->above;				
			}
		}				
	}
	return false;
}

_XSObject* _XSObject::AS3GetTargetObject(bool bMouse)
{
	_XSObject*pObj=this;
	//[Note X]
	/*_XSObject*p=pObj;
	while(p)
	{
		if(!p->IsMouseChildren()||p->IsButtonMode())
		{
			pObj=p;
			//break;
		}
		p=p->parent;
	}*/
	while(pObj)
	{
		//if((pObj->pASObject||pObj->pObject)&&pObj->IsEnable()) return pObj;
		if(pObj->character)
		{
			switch(pObj->character->type)
			{
			case editTextChar:
			case editTextEmptyChar:
			case textLineChar:
				 if(!editText)
					 break;
				 if(editText->m_flags&seditTextFlagsWasStatic) break;
				 //if(!editText->IsSelectable()) break;
			case buttonChar:
			case buttonEmptyChar:
			
			
			//case buttonEmptyChar:
			case playerChar:
			case spriteChar:
			case spriteEmptyChar:
			case spriteExternalChar:
			case rootChar:
			case rootCloneChar:
			case loaderChar:
			//case loaderImageChar:
				 return pObj;
				 //if(pObj->IsEnable()||AS3HasMouseHandle(pObj->pASObject)) return pObj;
			}
		}
		pObj=pObj->parent;
	}
	return NULL;
}

_XSObject* _XSObject::AS3HitObject(STransform x,SPOINT*pt,_XSObject**hitObject,XU32 t,bool bButton)
{
	//if(name.strTxt&&strcmp(name.strTxt,"privateChannel")==0)
	//	int v=0;
	//if( character && character->tag==37)
	//	int v=0;
	
	if(!IsVisible()&&!pLink)
		return XNULL;

	if(IsButtonMode()||!IsMouseChildren()||pLink)
		bButton=true;
		
	
	bool bAS3=this->CreatePlayer?this->CreatePlayer->bAS3:display->player->m_pAVM2!=NULL;//false;
	bool bChild=true;
	if(character)
	{
		switch(character->type)
		{
		case textLineChar:
		case editTextChar:
		case editTextEmptyChar:
			 //if(editText->m_buffer[0]=='N')
			 //	 int v=0;
		case buttonChar:
		case buttonEmptyChar:
			 bButton=true;
		case shapeChar:
		case shapeEmptyChar:
			 bChild=false;
			 break;
		}
	}
	//if(!IsMouseChildren())
	//	bChild=false;
	_XSObject*hitObj=XNULL;
	x.Concat(&xform);
	if(scrollRect)
	{
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.tx=-scrollRect->xmin;
		mat.ty=-scrollRect->ymin;
		::MatrixConcat(&mat,&x.mat,&x.mat);
		SRECT rt=*scrollRect;
		MatrixTransformRect(&x.mat,&rt,&rt);
		if(!RectPointIn(&rt,pt))
			return NULL;
	}

	if(bChild&&bottomChild)
	{
		
		struct OBJINFO
		{
			SObject*object;
			int		hitType;
			int		clipBy;
			bool	bChildClip;
		};

		int i,iCount=0;
		SObject*obj=this->bottomChild;
		while(obj)
		{	
			if(obj->clipDepth!=-2)
			{
				iCount++;
				if(obj->clipObject&&obj->clipObject->parent!=this)
				{
					//obj->clipObject->FreeCache();
					obj->clipObject->clipDepth=-2;
					iCount++;
				}
			}
			obj=obj->above;	
		}
		int nSize=iCount*sizeof(OBJINFO);

		OBJINFO*info=(OBJINFO*)XXVar::AllocData(nSize);

		//iCount--;
		obj=this->bottomChild;
		i=iCount;
		//memset(info,0,nSize);
		while(obj)
		{
			if(obj->clipDepth!=-2)
			{
				i--;
				info[i].object=obj;
				info[i].hitType=0;
				info[i].clipBy=0;
				info[i].bChildClip=false;
				if(obj->clipObject&&obj->clipObject->parent!=this)
				{
					i--;
					info[i].object=obj->clipObject;
					info[i].hitType=0;
					info[i].clipBy=0;
					info[i].bChildClip=false;
				}
			}
			obj=obj->above;
		}

		for(i=iCount-1;i>=0;i--)
		{
			obj=info[i].object;
			if( obj == NULL || !obj->clipDepth)
                continue;
			int j;//=i-1;
			if(obj->clipDepth>0&&obj->depth>0)
			{
				if(obj->clipDepth>obj->depth)
				{
					bool maskset=true;
					for(j=i-1;j>=0;j--)//j;j>=0;j--)
					{
						//if(info[j].object->depth>obj->clipDepth) break;
						//if(info[j].object->depth<0){continue;}
						//Modify by xiepeifu
						if ( info[j].object == NULL )
						{
							continue;
						}
						int depth=info[j].object->depth;
						if(depth>obj->clipDepth)
							maskset=false;
						if(info[j].object==obj||maskset)
							//(maskset&&depth<obj->clipDepth))
							info[j].clipBy=i+1;
						
					}
				}
				else
				{
					for(j=i+1;j<iCount;j++)
					{
						int depth=info[j].object->depth;
						if(info[j].object->clipObject==obj)
							info[j].clipBy=i+1;
						else if(depth>0&&depth==obj->clipDepth)
						{
							info[j].clipBy=i+1;
							break;
						}
					}
				}
			}
			else
			{
				for(j=0;j<iCount;j++)//iCount-1;j>=0;j--)
				{
					if(info[j].object->clipObject!=obj) 
						continue;
					info[j].clipBy=i+1;
				}
			}
		}

		for(i=0;i<iCount;i++)
//		for(i=iCount-1; i>=0; i--)
		{
			obj=info[i].object;
			if(obj != NULL && obj->clipDepth) 
				continue;
			int ic=info[i].clipBy;
			//Modify by xiepeifu
			if(ic)//&&info[ic-1].object->IsVisible())
			{
				ic--;
				if(!info[ic].hitType)
				{
					if(info[ic].bChildClip)
					{
						STransform x1=x;
						info[ic].object->parent->GetTotalMatrix(x1.mat,XTRUE);
						if(info[ic].object->HitTestOneChild(x1,pt,true))
							info[ic].hitType=1;
						else
							info[ic].hitType=2;
					}
					else
					{
						if(info[ic].object->HitTestOneChild(x,pt,true))
							info[ic].hitType=1;
						else
							info[ic].hitType=2;
					}
				}
				if(info[ic].hitType!=1) 
					continue;
			}
            if ( obj == NULL )
			{
                return NULL;
            }
			_XSObject* hobj=obj->AS3HitObject(x,pt,hitObject,t,bButton);
		
			if(hobj) 
			{
				
				if(*hitObject==NULL)
					*hitObject=hobj;
				if(!hobj->AS3HasMouseEventAt(this,bAS3))
				{
					if(!hitObj||!hitObj->IsMouseEnabled())
						hitObj=hobj;
				}
				else
				{
					if(XSWFPlayer::m_pInstance->m_pAVM2!=NULL)
						XXVar::FreeData(info,nSize);
					if(!this->IsMouseChildren()||(!hobj->IsEnable()&&!AS3HasMouseHandle(hobj->pASObject))||!hobj->IsMouseEnabled())//bButton||!hobj->IsEnable()||!hobj->IsMouseEnabled())
						return (pLink&&pLink->IsVisible())?pLink:this;
					else 
						return (pLink&&pLink->IsVisible())?pLink:hobj;
				}
			}
		}

		XXVar::FreeData(info,nSize);
	}
	if(hitObj) 
		return pLink?pLink:this;
	if(!pLinkBy&&HitTestOne(&x,pt,bButton )) 
	{
		if(*hitObject==NULL)
			*hitObject=this;
		return (pLink&&pLink->IsVisible())?pLink:this;
	}
	return XNULL;
	//return XNULL;
}

bool _XSObject::AS3HitTestPixel(STransform x,SPOINT*pt)
{
	x.Concat(&xform);
	if(scrollRect)
	{
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.tx=-scrollRect->xmin;
		mat.ty=-scrollRect->ymin;
		::MatrixConcat(&mat,&x.mat,&x.mat);
	}
//#if (__CORE_VERSION__>=0x02081000)
	if ( HitTestOne(&x, pt, true) )
//#else
//	if ( HitTestOne(&x, pt,100) )
//#endif
		return true;

	for ( _XSObject* obj = bottomChild; obj;  ) {
		if ( obj->clipDepth ) {
			if(obj->AS3HitTestClipper(obj,x, pt,1)) return true;
		} else {
			if(obj->AS3HitTestPixel(x, pt)) return true;
			obj = obj->above;
		}
	}
	return false;
}

bool   _XSObject::AS3HitTest(int x,int y,XBOOL bAnti)
{
	SObject*p=XNULL;
	SPOINT pt;
	pt.x=x*20;
	pt.y=y*20;
	//if(pt.y<0)
	//	int v=0;
	ScriptPlayer*player=GetPlayer();
	MATRIX dmat;
	//[Note X]
	//if(player)
	//	dmat=player->rootObject->xform.mat;
	//else
		::MatrixIdentity(&dmat);
	::MatrixConcat(&dmat,&display->camera.mat,&dmat);
	if(!bAnti)
	{
		SRECT rect;
		MATRIX mat;
		if(parent)
			parent->GetTotalMatrix(mat,XTRUE);
		else
			mat=display->camera.mat;
		::MatrixTransformPoint(&dmat,&pt,&pt);
		GetBoundingBox(this,&mat,&rect);
//		if(display->nAspect)
//			::RectInset(-display->nAspect,&rect);
//		if(rect.ymax<140)
//			int v=0;
		return ::RectPointIn(&rect,&pt);
		//if(bOK)
		//	int v=0;
		//return bOK;
	}

	STransform t;
	
	MATRIX mat;//=display->camera.mat;
	//::MatrixIdentity(&mat);
	if(parent)
		parent->GetTotalMatrix(mat,XTRUE);
	else
		mat=display->camera.mat;
	::MatrixTransformPoint(&dmat,&pt,&pt);
	
	t.mat=mat;	

	return AS3HitTestPixel(t,&pt);
	/*HitTest(p,t,&pt,XTRUE);
	if(p) 
		return XTRUE;
	return XFALSE;//p!=XNULL;*/
}

bool _XSObject::AS3HasMouseEventAt(_XSObject*pObj,bool bAS3)
{
	_XSObject*p=this;
	while(p&&p!=pObj)
	{
		if(p->AS3HasMouseEvent()) return true;
		if(!p->IsMouseEnabled()||!p->IsEnable()) return false;
		if(p->character&&bAS3)
		{
			switch(p->character->type)
			{
//#if (__CORE_VERSION__>=0x02070200)
			case textLineChar:
//#endif
			case editTextChar:
			case editTextEmptyChar:
				 if(!editText)
					 break;
				 //[Note X]
				 if(editText->m_flags&seditTextFlagsWasStatic) break;
				 //if(!editText->HasClickEvent()) break; 
			case buttonChar:
			case spriteChar:
			
			case spriteEmptyChar:
			case loaderChar:
			case buttonEmptyChar:
			case playerChar:
			case spriteExternalChar:
			case rootChar:
			case rootCloneChar:
				 return true;
			}
		}
		p=p->parent;
	}
	return false;
}
/*void _XSObject::AS3SetLoaderInfo(bool bIn)
{
	if(!pASObject) return;
	DisplayObjectObject*obj=(DisplayObjectObject*)pASObject;
	//rootPlayer=player;
	if(bIn)
		obj->m_pLoaderInfo=player->m_pLoaderInfo;
	else
		obj->m_pLoaderInfo=XNULL;
}*/

void _XSObject::AS3AddChild()
{
	if(character)
	{
		
		switch(character->type)
		{
		case spriteChar:
		case spriteEmptyChar:
		case spriteExternalChar:
		case rootChar:
		case rootCloneChar:
			 if(!thread->m_bInDisplay)//!this->IsInDisplay())
				display->AddThread(thread);
			 break;
		}
	}
	//AS3SetLoaderInfo(bIn);
//AS3AddedToStage();
	_XSObject*obj=bottomChild;
	while(obj)
	{
		obj->AS3AddChild();
		obj=obj->above;
	}
}

void _XSObject::AS3RemoveChild()
{

	if(character)
	{	
		switch(character->type)
		{
		case spriteChar:
		case spriteEmptyChar:
		case spriteExternalChar:
		case rootChar:
		case rootCloneChar:
			 if(thread&&thread->m_bInDisplay)//IsInDisplay())//thread->m_bInDisplay)
				display->RemoveThread(thread);
			 break;
		}
	}
	FreeCache();
	FreeCacheBmp();
//AS3SetLoaderInfo(XNULL);
//this->AS3RemoveFromStage();
	_XSObject*obj=bottomChild;
	while(obj)
	{
		obj->AS3RemoveChild();
		obj=obj->above;
	}
}

bool _XSObject::IsAS3()
{
	if(pASObject) return true;
	if(thread) return thread->player->bAS3;
	return false;
}

SRECT* _XSObject::AS3GetScrollRect()
{
	return scrollRect;
	
}
void _XSObject::AS3SetScrollRect(SRECT*rect)
{
	if(rect)
	{
		if(!scrollRect)
			scrollRect=display->CreateRect();
		*scrollRect=*rect;
		/*if(!scrollObject)
		{
			display->player->AS3CreateSObject(scrollObject,scrollChar,NULL,NULL,GetPlayer());
		}*/
		//scrollObject->character->bounds=*rect;
	}
	else
	{
		if(scrollRect)
			display->FreeRect(scrollRect);
		scrollRect=XNULL;
	}
	Modify();
}

void _XSObject::SetGridRect(SRECT*rect)
{
	if(!character) return;
	if(rect)
	{		
		SRECT&r=GETBOUNDS(character,editText);//character->bounds;
		//if(rect->xmin<r.xmin||rect->ymin<
		if(!gridRect)
			gridRect=display->CreateRect();
		*gridRect=*rect;
/*#if (__CORE_VERSION__>=0x02075000)
		if(xform.mat.d<0)
			xform.mat.d=-xform.mat.d;
#endif*/
	}
	else
	{
		if(gridRect)
			display->FreeRect(gridRect);
		gridRect=NULL;
	}
}
SRECT* _XSObject::GetGridRect()
{
	return gridRect;
}


XBOOL _XSObject::HasClickEvent()
{
	_XSObject*p=this;
	//while(p)
	{
		if(p->pASObject)
		{
			avmshell::DisplayObjectObject*pObject=(avmshell::DisplayObjectObject*)p->pASObject;
			if(pObject->hasEvent(XAS3_EVENT_click)) return XTRUE;
		}
		if(character)

		{
			switch(character->type)
			{
				case textLineChar:
				case editTextChar:
				case editTextEmptyChar:
					 if(editText)
					 {
						 if(editText->m_bURL)
						 {
							 return XTRUE;
						 }
						 else if(((editText->m_flags&(seditTextFlagsWasStatic|seditTextFlagsReadOnly))==0))
						 {
							 return XTRUE;
						 }
					 }
			}			
		}
		p=p->parent;
	}
	return XFALSE;
}

AS3ButtonInfomation* _XSObject::CreateBI(XBOOL&bParent,SRECT&rect,SPOINT*pt)
{
	if(!pASObject) return NULL;
	if(RectIsEmpty(&rect)) return NULL;
	XU32 nFlag=0,nImportant=0;
	avmshell::DisplayObjectObject*pObject=(avmshell::DisplayObjectObject*)pASObject;
	if(pObject->hasEvent(XAS3_EVENT_mouseDown))
	{
		nFlag|=AS3ButtonInfomation::BI_MOUSEDOWN;
		nImportant|=0x100;
	}
	if(pObject->hasEvent(XAS3_EVENT_mouseUp))
	{
		nFlag|=AS3ButtonInfomation::BI_MOUSEUP;
		nImportant|=0x2000;
	}
	if(pObject->hasEvent(XAS3_EVENT_click))
	{
		nFlag|=AS3ButtonInfomation::BI_MOUSECLICK;
		nImportant|=40000;
	}
	if(character)
	{
		switch(character->type)
		{
			case textLineChar:
			case editTextChar:
			case editTextEmptyChar:
				 if(!editText) return NULL;
				 if(editText->m_bURL)
				 {
					 nFlag|=AS3ButtonInfomation::BI_MOUSECLICK;
					 nImportant|=40000;
				 }
				 else if((editText->m_flags&(seditTextFlagsWasStatic|seditTextFlagsReadOnly))==0)
				 {
					 nFlag|=AS3ButtonInfomation::BI_MOUSECLICK;
					 nImportant|=20000;
				 }
				 //if(editText->HasClickEvent()) //break;
		}
	}
	if(!nFlag) return NULL;
	int wx=RectWidth(&rect);
	int wy=RectHeight(&rect);
	int nMaxLen=64;
	//if(display->antialias_b)
	//	nMaxLen<<=display->antialias_b;
	if(wx>nMaxLen&&wy>nMaxLen) return NULL;
	nImportant<<=8;
	AS3ButtonInfomation*pBi=this->display->AllocInfo();
	pBi->pNext=NULL;
	pBi->nFlag=nFlag;
	
	/*if(pt->x<rect.xmin)
	{
		pBi->nDeltaX=rect.xmin-pt->x;
	}
	else if(pt->x>rect.xmax)
		pBi->nDeltaX=pt->x-rect.xmax;
	else pBi->nDeltaX=0;
	if(pt->y<rect.ymin)
		pBi->nDeltaY=rect.ymin-pt->y;
	else if(pt->y>rect.ymax)
		pBi->nDeltaY=pt->y-rect.ymax;
	else
		pBi->nDeltaY=0;
	pBi->nLength=XMAX(pBi->nDeltaX,pBi->nDeltaY);*/
	int cx=(rect.xmin+rect.xmax)/2;
	int cy=(rect.ymin+rect.ymax)/2;
	pBi->nDeltaX=pt->x-cx;
	pBi->nDeltaY=pt->y-cy;
	SPOINT ptt;
	ptt.x=pBi->nDeltaX<<16;
	ptt.y=pBi->nDeltaY<<16;
	pBi->nLength=PointLength(&ptt)>>16;
	//int nArea=(wx*wy)+1;
	pBi->nImportant=(nImportant/(0x10+pBi->nLength));///nArea;
	pBi->buttonBounds=rect;
	pBi->button=this;
	return pBi;

}


XBOOL _XSObject::HitBI(STransform x,SPOINT*pt,int nCap,SRECT&devRect,XBOOL bEnable)
{
	if(!IsVisible()) return XFALSE;

	//if(character&&character->tag==168)
	//	int v=0;
	
	x.Concat(&xform);
	XBOOL bHit=false;
	SRECT rect;
	::RectSetEmpty(&rect);
	if(!RectIsEmpty(&devBounds))
	{
		rect=devBounds;
		SRECT rt=rect;
		int sx=RectWidth(&rt);
		int sy=RectHeight(&rt);
		sx=(sx*nCap)>>8;
		sy=(sy*nCap)>>8;
		rt.xmin-=sx;
		rt.xmax+=sx;
		rt.ymin-=sy;
		rt.ymax+=sy;
		if(RectPointIn(&rt,pt))
			bHit=XTRUE;
	}
	
	XBOOL bEnableChild=XTRUE;
	if(!this->IsMouseChildren()||!IsEnable()) 
		bEnableChild=XFALSE;

	_XSObject*obj=this->bottomChild;
	while(obj)
	{
		if(obj->HitBI(x,pt,nCap,rect,bEnable&&bEnableChild))
			bHit=true;
		obj=obj->above;
	}
	if(!RectIsEmpty(&rect))
		RectUnion(&rect,&devRect,&devRect);

	if(bHit&&bEnable)
	{
		AS3ButtonInfomation*pNew=CreateBI(bHit,rect,pt);
		if(pNew)
		{
			/*AS3ButtonInfomation*pRoot=*display->player->m_pBIInfo;
			while(pRoot)
			{
				if(pRoot==pNew)
					break;
				pRoot=pRoot->pNext;
			}*/
			*display->player->m_pBIInfo=pNew;
			display->player->m_pBIInfo=&pNew->pNext;
		}
	}

	return bHit;
}
//#if (__CORE_VERSION__>=0x02070100)
void _XSObject::SetUpRemove()
{
	_XSObject*obj=this;
	XSWFPlayer*player=display->player;
	if(obj==player->m_pUpObject||(player->m_pUpObject&&
	   obj->IsParentOf(player->m_pUpObject)))
		player->m_pUpRemovedObject=player->m_pUpObject;
}
//#endif

//////////////////////////////////////////////////////////////////////////////////
void SCharacterParser::AddCurve(P_CURVE src)
{
	CURVE c;
	S32 dir;
	
	int dx=src->anchor2.x-src->anchor1.x;
	int dy=src->anchor2.y-src->anchor1.y;
	if(dx<0) dx=-dx;
	if(dy<0) dy=-dy;

	c = *src;

	// Sort the anchors
	if ( c.anchor1.y <= c.anchor2.y )
	{
		dir = 1;
	} 
	else
	{
		dir = -1;
		Swap(c.anchor1, c.anchor2, SPOINT);
	}
	
	if ( !c.isLine )
	{
		if ( c.control.y < c.anchor1.y || c.control.y > c.anchor2.y ) {
			// Handle a minimum or maximum
		
			// If it is close, just adjust the curve
			if ( c.control.y < c.anchor1.y && c.anchor1.y - c.control.y < 3 ) {
				c.control.y = c.anchor1.y;
			
			} else if ( c.control.y > c.anchor2.y && c.control.y - c.anchor2.y < 3 ) {
				c.control.y = c.anchor2.y;   
			
			} else {
				SCOORD a, b;
				CURVE c1, c2;
				
				// Use the curve with the original order
				a = src->anchor1.y - 2*src->control.y + src->anchor2.y;
				b = src->anchor1.y - src->control.y;
			
				// See if b/a is in the range 0..1
				// a and b must have the same sign and abs(b) <= abs(a)
				FLASHASSERT(((a > 0 && b >= 0) || (a < 0 && b <= 0)) && Abs(b) <= Abs(a));
				
				CurveDivide(src, FixedDiv(b, a), &c1, &c2);
				if ( ++depth > 16 ) {
					FLASHASSERT(XFALSE);
					return;
				}
				AddCurve(&c1);
				AddCurve(&c2);
				depth--;
				return;
			}
		}

		// Split any curves that are too long
		if ( c.anchor2.y - c.anchor1.y > 256 ) {
			CURVE c1, c2;
			CurveDivide(src, &c1, &c2);
			if ( ++depth > 16 ) {
				FLASHASSERT(XFALSE);
				return;
			}
			AddCurve(&c1);
			AddCurve(&c2);
			depth--;
			return;
		}
	}

	if ( c.anchor1.y == c.anchor2.y ) 
		return;	// this is a horizontal edge, we can ignore it

	if ( preclip ) {
		SRECT b;

		// Check to clip
		CurveBounds(&c, &b);

		// If the curve is above or below, we can completely ignore it
		// We assumme that the largest image will be 4000x4000 pixels
		if ( b.ymax < -16000 || b.ymin > 16000 ) return;

		// Pre-clip to prevent overflow...
		if ( b.xmin < -32000 || b.xmax > 32000 || b.ymin < -32000 || b.ymax > 32000 ) {
			if ( b.xmax < -16000 ) {
				// The curve is completely off to the left, make a vertical line
				MakeVertLine(&c, -32000);
			} else if ( b.xmin > 16000 ) {
				// Curve is completely off to the right, make a vertical line
				MakeVertLine(&c, 32000);
			} else if ( depth < 16 ) {
				// Split the curve
				CURVE c1, c2;
				CurveDivide(src, &c1, &c2);
				depth++;
				AddCurve(&c1);
				AddCurve(&c2);
				depth--;
				return;
			}
		}
	}

	/*if(obj&&obj->clipRect)
	{
		int xmin=obj->clipRect->xmin;
		int ymin=obj->clipRect->ymin;
		int xmax=obj->clipRect->xmax;
		int ymax=obj->clipRect->ymax;
		SRECT b;
		// Check to clip
		CurveBounds(&c, &b);
		if(b.ymax<ymin||b.ymin>ymax) 
			return;
		//if(b.ymin>ymax) return;
	}*/

	// Set up a new edge
	REdge* edge = display->CreateEdge();
	if ( !edge ) return;

	edge->nextActive=NULL;
	/*REdge*t=obj->edges;
	while(t)
	{
		if(t==edge)
			int v=0;
		t=t->nextObj;
	}*/

    // Set up the curve
	edge->Set(&c);
	edge->dir = (S8)dir;
	
	// Track the colors
	edge->fillRule = (U8)fillRule;
	edge->color1 = color1;
	edge->color2 = color2;
	
	// Add to the edge list
	edge->nextObj = pEdges;
	pEdges = edge;
}
int SCharacterParser::GetEdge(P_CURVE c)
{
	BOOL isEdge = (int)GetBits(1);
	if ( !isEdge ) {
		// Handle a state change
		int flags = (int)GetBits(5);

		if ( flags == 0 )
			// at end, do nothing
			return eflagsEnd;

		// Process a state change

		// Process a MoveTo
		if ( flags & eflagsMoveTo ) {
			int nBits = (int)GetBits(5);
			curPt.x = GetSBits(nBits);
			curPt.y = GetSBits(nBits);
			/*if(div)
			{
				curPt.x/=div;
				curPt.y/=div;
			}*/
			MatrixTransformPoint(&mat, &curPt, &curPtX);
		}

		// Get new fill info
		if ( flags & eflagsFill0 )
			fill[0] = (int)GetBits(nFillBits);
		if ( flags & eflagsFill1 )
			fill[1] = (int)GetBits(nFillBits);

		// Get new line info
		if ( flags & eflagsLine )
		{
			line = (int)GetBits(nLineBits);
			//if(line>nLines)
			//	int v=0;
		}

		// Check to get a new set of styles for a new shape layer
		if ( flags & eflagsNewStyles ) {
		 	layer += nFills+nLines;
			GetStyles();

		}

		if(fill[0]>nFills)
			fill[0]=0;
		if(fill[1]>nFills)
			fill[1]=0;
		if(line>nLines)
			line=0;

		//FLASHASSERT(fill[0]<=nFills && fill[1]<=nFills && line<=nLines);	// be sure the styles are loaded

		return flags;

	} else {
		// Create an edge
		c->anchor1 = curPtX;

		c->isLine = (int) GetBits(1);
		if ( c->isLine ) {
			// Handle a line
			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

			// Save the deltas
			BOOL generalLine = (int)GetBits(1);
			if ( generalLine ) {
				// Handle a general line
				/*int x=GetSBits(nBits);
				int y=GetSBits(nBits);
				if(div)
				{
					x/=div;
					y/=div;
				}*/
				curPt.x += GetSBits(nBits);//x;
				curPt.y += GetSBits(nBits);//y;
			} else {
				// Handle a vert or horiz line
				BOOL vertical = (int)GetBits(1);
				//int v=GetSBits(nBits);
				//if(div) v/=div;
				if ( vertical ) {
					// Vertical line
					curPt.y += GetSBits(nBits);

				} else {
					// Horizontal line
					curPt.x += GetSBits(nBits);
				}
			}

			MatrixTransformPoint(&mat, &curPt, &c->anchor2);
			PointAverage(&c->anchor1, &c->anchor2, &c->control);

		} else {
		 	// Handle a curve
			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

			// Get the control
			/*int x=GetSBits(nBits);
			int y=GetSBits(nBits);
			if(div)
			{
				x/=div;
				y/=div;
			}*/
			curPt.x += GetSBits(nBits);
			curPt.y += GetSBits(nBits);



			MatrixTransformPoint(&mat, &curPt, &c->control);

			// Get the anchor
			/*x=GetSBits(nBits);
			y=GetSBits(nBits);
			if(div)
			{
				x/=div;
				y/=div;
			}*/

			curPt.x += GetSBits(nBits);
			curPt.y += GetSBits(nBits);

			MatrixTransformPoint(&mat, &curPt, &c->anchor2);
		}

		curPtX = c->anchor2;
		// FLASHOUTPUT("Edge-f1=%i, f2=%i, l=%i, a2.x=%i, a2.y=%i\n", fill[0], fill[1], line, pt.x, pt.y);
		return 0;
	}
}

int SCharacterParser::GetNormalEdge(P_CURVE c)
{
	BOOL isEdge = (int)GetBits(1);
	if ( !isEdge ) {
		// Handle a state change
		int flags = (int)GetBits(5);

		if ( flags == 0 )
			// at end, do nothing
			return eflagsEnd;

		// Process a state change

		// Process a MoveTo
		if ( flags & eflagsMoveTo ) {
			int nBits = (int)GetBits(5);
			curPt.x = GetSBits(nBits);
			curPt.y = GetSBits(nBits);
		}

		if ( flags & eflagsFill0 )
			GetBits(nFillBits);
		if ( flags & eflagsFill1 )
			GetBits(nFillBits);

		// Get new line info
		if ( flags & eflagsLine )
		{
			line = GetBits(nLineBits);
			//if(line>nLines)
			//	int v=0;
		}
		if ( flags & eflagsNewStyles ) {
			if(!GetNormalStyles()) return 0x7fffffff;
			//GetNormalStyles();
		}

		return flags;

	} else {
		// Create an edge
		c->anchor1 = curPt;

		c->isLine = (int) GetBits(1);
		if ( c->isLine ) {
			// Handle a line
			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2

			// Save the deltas
			BOOL generalLine = (int)GetBits(1);
			if ( generalLine ) {
				curPt.x += GetSBits(nBits);//x;
				curPt.y += GetSBits(nBits);//y;
			} else {
				// Handle a vert or horiz line
				BOOL vertical = (int)GetBits(1);
				if ( vertical ) {
					// Vertical line
					curPt.y += GetSBits(nBits);

				} else {
					// Horizontal line
					curPt.x += GetSBits(nBits);
				}
			}
			c->anchor2 = curPt;

		} else {
		 	// Handle a curve
			int nBits = (int)GetBits(4)+2;	// nBits is biased by 2


			curPt.x += GetSBits(nBits);
			curPt.y += GetSBits(nBits);
			c->control = curPt;
			curPt.x += GetSBits(nBits);
			curPt.y += GetSBits(nBits);
			c->anchor2 = curPt;

		}

		curPt = c->anchor2;
		// FLASHOUTPUT("Edge-f1=%i, f2=%i, l=%i, a2.x=%i, a2.y=%i\n", fill[0], fill[1], line, pt.x, pt.y);
		return 0;
	}
}

typedef RColor *PRColor;

XBOOL SCharacterParser::ReadColor(RColor *color)
{
		color->SetUp(raster);

		//color->nextColor = *colorList;
		//*colorList = color;

		FLASHASSERT(color->order < 0x10000);

		int fillStyle = GetByte();
		if ( fillStyle & fillGradient ) {
			color->colorType = colorGradient;
			color->grad.gradStyle = fillStyle;
 			color->grad.colorRamp = 0;

			GetMatrix(&color->grad.savedMat);
			XU8 nColor=GetByte();
			color->grad.ramp.nColors	= nColor&0xf;
			color->grad.ramp.spreadMode	= (nColor>>6);
			color->grad.ramp.interMode	= (nColor>>4)&3;
				//GetByte();
			for ( int j = 0; j < color->grad.ramp.nColors; j++ ) {
				color->grad.ramp.colorRatio[j] = GetByte();
					SRGB c = GetColor(getAlpha);
					if ( c.rgb.transparency < 255 )
						color->transparent = true;
					color->grad.ramp.color[j] = c;
				}

				// merge fix
				
			 if(fillStyle&1)
				 SkipBytes(2);
            } else if ( fillStyle & fillBits ) {
				// A bitmap fill
				U16 tag = GetWord();		// the bitmap tag
				GetMatrix(&color->bm.savedMat);	// the bitmap matrix
				SCharacter* ch = player->FindCharacter(tag);

				// Default to red if we can't find the bits
				color->rgb.alpha = 255;
				color->rgb.red = 0;
				color->rgb.green = color->rgb.blue = 0;

				if ( ch && ch->type == bitsChar ) {
					// We found the bits, set up the color
					color->colorType = colorBitmap;
					color->bm.bitmap = &ch->bits;
					color->bm.bitsStyle = (U8)fillStyle;
				//	color->bm.cxform = cxform;
				//	color->bm.rowIndex = 0;
					//if(ch->tag==141)
					//	int v=0;
					//	color->bm.savedMat.ty=0;

					player->BuildBits(ch);	// be sure the bits are decompressed and available
					//ch->bits.CreateTexture();
					//ch->bits.ReleaseData();
					color->SetBitmapDiv();
					
				}
				//color->colorType=colorSolid;

			} else {
				// A solid color
				GetColor(&color->rgb, getAlpha);

				color->transparent = (U8)PreMulAlpha(&color->rgb);
			}
			//SetupColor(color);
			return true;
}

BOOL SCharacterParser::GetStyles()
{
	//else clipColor=CreateClipColor(display, colorList);
	{// Get the fills
		nFills = GetByte();
		if ( nFills == 255 ) {
			// We have a "large number"
			nFills = GetWord();
			if ( fillIndex != fillIndexMem&&fillIndex )
				delete [] fillIndex;
			fillIndex = new PRColor[nFills+1];
			if ( !fillIndex ) {
				fillIndex = fillIndexMem;
				return false;
			}
		}

		fillIndex[0] = 0;
		for ( int i = 1; i <= nFills; i++ ) {
			if(pos>=tagEnd) return false;
			RColor* color = display->CreateColor();
			if ( !color )
				return false;
			if(!ReadColor(color))
			{
				display->FreeColor(color);
				//delete color;
				return false;
			}
			
			{
				color->nextColor = pColors;
				pColors = color;
				fillIndex[i] = color;
				color->order = layer + i;
				//this->
				SetupColor(color);
			}
				// the buildcache in this function must be called immediately after BuildBits()
		}
	}

	{// Get the lines
		nLines = GetByte();
		if ( nLines == 255 ) {
			// We have a "large number"
			nLines = GetWord();
			if ( lineIndex != lineIndexMem&&lineIndex )
				delete [] lineIndex;
			lineIndex = new SLine[nLines+1];
			if ( !lineIndex ) {
				lineIndex = lineIndexMem;
				return false;
			}
		}

		lineIndex[0].color = 0;
		lineIndex[0].thickness = 0;
		for ( int i = 1; i <= nLines; i++ ) {
			//XU16 thick=GetWord();
			//if(!thick) return false;
			if(pos>=tagEnd) return false;
			RColor* color = display->CreateColor();
			if ( !color )
				return false;

			color->SetUp(raster);

			lineIndex[i].color = color;
			lineIndex[i].thickness = (int)MatrixTransformThickness(&mat, GetWord());
			//if(lineIndex[i].thickness>=4)
			//	int v=0;
			lineIndex[i].nMiterFactor=0;
			lineIndex[i].nFlags=0;
			color->order = layer + (i+nFills);
			FLASHASSERT(color->order < 0x10000);

			if(lineStyle2)
			{
				XU16 flags=(GetByte()<<8)|GetByte();
				lineIndex[i].nFlags=flags;
				if((flags&XLS_JOINSTYLEMITER))
					lineIndex[i].nMiterFactor=GetWord();
				if((flags&XLS_FILLFLAG))
					ReadColor(color);
				else
				{
					GetColor(&color->rgb, getAlpha);
					//if ( cxform.HasTransform() )
					//	cxform.Apply(&color->rgb);
					color->transparent = (U8)PreMulAlpha(&color->rgb);
				}
			}
			else
			{
				GetColor(&color->rgb, getAlpha);
				//if ( cxform.HasTransform() )
				//cxform.Apply(&color->rgb);
				color->transparent = (U8)PreMulAlpha(&color->rgb);
			}
			
			{
				color->nextColor = pColors;
				pColors = color;
				//_GRIDINFO*fo=this->info;
				SetupColor(color);
				//this->info=fo;
			}
		}
	}

	InitBits();
	nFillBits = (int)GetBits(4);
	nLineBits = (int)GetBits(4);
	return true;
}

void SCharacterParser::SetupColor(RColor* color)
// Set up the colors for the current CRaster
{
	if ( !raster ) return;

	//color->raster = raster;
	switch ( color->colorType ) {
		case colorSolid:
			if(!display->player->m_pDomFilter||
				!display->player->m_pDomFilter->bEnuFilter)
				break;
			break;
		case colorGradient: {
			// Set up the inverse matrix
			MATRIX gradMat, devMat;
			gradMat = color->grad.savedMat;
			if ( raster->antialias_b ) {
				MATRIX aaToDev;
				switch(raster->antiShift)
				{
				case 2:
					MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
					break;
				case 1:
					MatrixScale(fixed_1/2, fixed_1/2, &aaToDev);
					break;
				case 3:
					MatrixScale(fixed_1/8, fixed_1/8, &aaToDev);
					break;
				}
				//MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
				MatrixConcat(&mat, &aaToDev, &devMat);	// we don't want the supersampled coords
			} else {
				devMat = mat;
			}

			// We need to work with the translation components in fixed point
			devMat.tx <<= 8;	devMat.ty <<= 8;
			gradMat.tx <<= 8;	gradMat.ty <<= 8;
			MatrixConcat(&gradMat, &devMat, &gradMat);
			MatrixInvert(&gradMat, &color->grad.invMat);
		} break;

		case colorBitmap: {
			// Set up the transforms
			MATRIX bitsMat, devMat;
			if ( raster->antialias_b ) {
				MATRIX aaToDev;
				switch(raster->antiShift)
				{
				case 2:
					MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
					break;
				case 1:
					MatrixScale(fixed_1/2, fixed_1/2, &aaToDev);
					break;
				case 3:
					MatrixScale(fixed_1/8, fixed_1/8, &aaToDev);
					break;
				}
				//MatrixScale(fixed_1/4, fixed_1/4, &aaToDev);
				MatrixConcat(&mat, &aaToDev, &devMat);	// we don't want the supersampled coords
			} else {
				devMat = mat;
			}

			// We need to work with the translation components in fixed point
			bitsMat = color->bm.savedMat;

			/*if(info)
			{
				if(bitsMat.b)
				{
					if(bitsMat.a)
					{
						info=NULL;
					}
				}
				if(bitsMat.c&&bitsMat.d)
				{
					if(bitsMat.d)
					{
						info=NULL;
					}
				}
				if(info&&bitsMat.c&&bitsMat.b)
				{
					bitsMat.a=bitsMat.c;
					bitsMat.d=bitsMat.b;
					bitsMat.b=bitsMat.c=0;
					color->bm.savedMat=bitsMat;
				}
			}*/
			//color->bm.scaleMat=devMat;
			//MatrixConcat(&bitsMat,&devMat,&color->bm.scaleMat);
			bitsMat.tx <<= XBITMAPOFF;	bitsMat.ty <<= XBITMAPOFF;
			devMat.tx <<= XBITMAPOFF;	devMat.ty <<= XBITMAPOFF;
			
			{
				//bitsMat.tx <<= XBITMAPOFF;	bitsMat.ty <<= XBITMAPOFF;
				//devMat.tx <<= XBITMAPOFF;	devMat.ty <<= XBITMAPOFF;

				MatrixConcat(&bitsMat, &devMat, &bitsMat);
				//color->bm.devMat=bitsMat;
				MatrixInvert(&bitsMat, &color->bm.invMat);
			}

			SBitmapCore* bits = color->bm.bitmap;
			color->transparent =  int( bits->transparent );
			if ( bits->HasBits() ) {
				color->BuildCache();	// this locks the bitmap so it cannot be purged...
				
			} else {
				color->colorType = colorSolid;
			}
		} break;
	}
	color->BuildCache();
}

void SCharacterParser::ReleaseData()
{
	{// Free the edges
		REdge* e = pEdges;
		while ( e ) {
			REdge* next = e->nextObj;
			display->FreeEdge(e);
			e = next;
		}
		pEdges = 0;
	}

	{// Free the colors
		RColor* c = pColors;
		while ( c ) {
			RColor* next = c->nextColor;
			//if(b&&c->colorType==bitmapColor)
			//	c->FreeCache();
			display->FreeColor(c);
			c = next;
		}
		pColors = 0;
	}
	if(fillIndex!=fillIndexMem)
	{
		delete []fillIndex;
		fillIndex = fillIndexMem;
	}
	if(lineIndex!=lineIndexMem)
	{
		delete[] lineIndex;
		lineIndex = lineIndexMem;
	}
}

static inline void SetLimitRect(SRECT&r,SPOINT&pt,bool&bSet)
{
	if(!bSet)
	{
		r.xmin=r.xmax=pt.x;
		r.ymin=r.ymax=pt.y;
	}
	else
	{
		if(r.xmin>pt.x) r.xmin = pt.x;
		else if(r.xmax<pt.x) r.xmax = pt.x;
		if(r.ymin>pt.y) r.ymin = pt.y;
		else if(r.ymax<pt.y) r.ymax = pt.y;
	}
	bSet = true;
}

void SCharacterParser::GetFontBound(SRECT&r)
{
	InitBits();
	nFillBits = (int)GetBits(4);
	nLineBits = (int)GetBits(4);
	
	bool bSet=false;
	//int div=tagCode==stagDefineFont3?20:0;
	for (;;){
		CURVE c;
		int flags = GetNormalEdge(&c);//,div);
		if ( flags ) {
		 	if ( flags == eflagsEnd ) {
		 		break;
			}
		} 
		else 
		{
			// Add an edge
			if(c.isLine)
			{
				SetLimitRect(r,c.anchor1,bSet);
				SetLimitRect(r,c.anchor2,bSet);
			}
			else
			{
				SetLimitRect(r,c.anchor1,bSet);
				SetLimitRect(r,c.control,bSet);
				SetLimitRect(r,c.anchor2,bSet);
			}
		}
	}
}

void SCharacterParser::BuildRasterEdges(BOOL getStyles, SRECT* devBounds,CRaster*r,MATRIX&m)
{
	this->raster = r;
	mat = m;
	layer = 0;
	display = &XSWFPlayer::m_pInstance->display;
	pEdges = NULL;
	pColors = NULL;
	depth = 0;

	//REdge* pEdges = NULL;
	// Set up a stroker for any lines
	SStroker stroker(r, &pEdges);
	stroker.SetAntialias(r->antialias_b,r->antiShift);//obj->display->antialias_b,obj->display->antiShift);
	//stroker.antialias = obj->display->antialias;

	stroker.preclip = preclip = devBounds && (
				devBounds->xmin < -32000 || devBounds->xmax > 32000 || 
				devBounds->ymin < -32000 || devBounds->ymax > 32000
			  );

	if ( getStyles ) {
		if ( !GetStyles() ) 
		{
			
			return;
		}
	} else {
		// See how large we need the fill and line tags to be
		InitBits();
		nFillBits = (int)GetBits(4);
		nLineBits = (int)GetBits(4);
	}
	
	// Get the edges
	BOOL hasFill, hasLine;

	color1 = color2 = 0;
	hasLine = hasFill = XFALSE;
	//int div=tagCode==stagDefineFont3?20:0;
	for (;;){
		CURVE c;
		int flags = GetEdge(&c);//,div);
		if ( flags ) {
			// Process a state change
			
			// Are we at the end
		 	if ( flags == eflagsEnd ) {
				if ( hasLine )
					stroker.EndStroke();
		 		break;
			}

			if ( flags & (eflagsFill0|eflagsFill1) ) {
				// Get new fill info
				color1 = fillIndex[fill[0]];
				color2 = fillIndex[fill[1]];

				if ( !color1 && color2 ) {
					color1 = color2;
					color2 = 0;
				}
				fillRule = color2 ? fillEdgeRule : (useWinding ? fillWindingRule : fillEvenOddRule);
				//if(fillRule==fillWindingRule)
				//	int v=0;
				hasFill = //nFilterColor?XFALSE:
						 (color1 || color2);
			}

			// Check to start a new stroke
			if ( flags & (eflagsLine|eflagsMoveTo) ) {
				// Cap off the current line
				if ( hasLine )
					stroker.EndStroke();

				// Start a new stroke
				{
					RColor* lineColor = lineIndex[line].color;
					if ( lineColor ) {
						stroker.BeginStroke(lineIndex[line].thickness, lineColor);
						hasLine = XTRUE;
					} else {
				 		hasLine = XFALSE;
					}
				}
			}
		} else {
			// Add an edge
			if ( hasFill )
				AddCurve(&c);
			if ( hasLine )
				stroker.AddStrokeCurve(&c);
		}
	}
}

bool SCharacterParser::IsComplex()
{
	if ( !GetNormalStyles() ) 
	{
		return false;
	} 
	
	line = 0;
	int iCount = 0;
	//int div=tagCode==stagDefineFont3?20:0;
	for (;;){
		CURVE c;
		int flags = GetNormalEdge(&c);//,div);
		if ( flags ) {
			// Process a state change
			if(flags==0x7fffffff)
				return false;
			// Are we at the end
		 	if ( flags == eflagsEnd ) {
		 		break;
			}

			// Check to start a new stroke
			if ( flags & (eflagsLine|eflagsMoveTo) ) {
				// Cap off the current line
				//if(line) return true;
			}
		} else {
			// Add an edge
			if(!c.isLine) 
				return false;
			else 
			{
				if(c.anchor1.x==c.anchor2.x||
				   c.anchor1.y==c.anchor2.y)
				{ 
					//iCount++;
				}
				else
				{
					//if(line) return true;
					iCount++;
					if(line) iCount+=4;
				}
				//iCount++;
				//if(iCount>=8) return true;
			}
		}
	}
	if(iCount>8) 
		return true;
	return false;
}

BOOL SCharacterParser::GetNormalStyles()
{
	//else clipColor=CreateClipColor(display, colorList);
	{// Get the fills
		nFills = GetByte();
		if ( nFills == 255 ) {
			// We have a "large number"
			nFills = GetWord();
		}

		for ( int i = 1; i <= nFills; i++ ) {
			if(pos>=tagEnd) return false;
			if(!SkipColor())
			{
				return false;
			}
		}
	}

	{// Get the lines
		nLines = GetByte();
		if ( nLines == 255 ) {
			// We have a "large number"
			nLines = GetWord();
			}

		for ( int i = 1; i <= nLines; i++ ) {
			//XU16 thick=GetWord();
			//if(!thick) return false;
			if(pos>=tagEnd) return false;

			GetWord();

			if(lineStyle2)
			{
				XU16 flags=(GetByte()<<8)|GetByte();
				
				if((flags&XLS_JOINSTYLEMITER))
					GetWord();
				if((flags&XLS_FILLFLAG))
				{	if(!SkipColor()) return false;}
				else
				{
					RGBI rgb;
					GetColor(&rgb, getAlpha);
				}
			}
			else
			{
				RGBI rgb;
				GetColor(&rgb, getAlpha);
			}
		}
	}

	InitBits();
	nFillBits = (int)GetBits(4);
	nLineBits = (int)GetBits(4);
	return true;
}

XBOOL SShapeParser::SkipColor()
{
		int fillStyle = GetByte();
		if ( fillStyle & fillGradient ) {
			MATRIX mat;
			GetMatrix(&mat);
			XU8 nColor=GetByte();
			int nColors	= nColor&0xf;
			for ( int j = 0; j < nColors; j++ ) {
				GetByte();
				GetColor(getAlpha);
				}

				// merge fix
				
			 if(fillStyle&1)
				 SkipBytes(2);
            } else if ( fillStyle & fillBits ) {
				// A bitmap fill
				U16 tag = GetWord();		// the bitmap tag
				MATRIX mat;
				GetMatrix(&mat);	// the bitmap matrix
				return XFALSE;

			} else {
				// A solid color
				RGBI rgb;
				GetColor(&rgb, getAlpha);
			}
			//SetupColor(color);
			return true;
}
