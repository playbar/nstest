//	990325	mnk	"fixed1" -> "fixed_1"
#include "StdAfxflash.h"
#include "splay.h"
#include "splayer.h"

#ifdef EDITTEXT
#include "edittext.h"
#endif
#include "GlobalPortingFunction.h"
//#include "Bitbuf.h"
#include "stags.h"
#include "sobject.h"
#include "ObjectButton.h"
#include "XFilter.h"
#include "avm2.h"
#include "Raster_gl.h"
#include <math.h>
#include "ShellCore.h"
#include "as3bitmapData.h"
#include "as3bitmap.h"
#include "AS3DisplayObjectContainer.h"
inline static void CacheText(SObject*obj)
{
	/*if(obj->character&&(obj->character->type==textChar||obj->character->type==editTextChar))
	{
		obj->cacheMode=true;
	}*/
}

#define DPShowDbgInfo(  ) \
    if( obj->character != NULL && (obj->character->tag == 94 || obj->character->tag == 97 )) \
    { \
        LOGWHEREVAL( obj->character->tag ); \
    }

//
// Display List Methods
//
#define DISABLE_TAB_FOCUS
DisplayList::DisplayList() :
				//edgeAlloc(sizeof(REdge), 600),//, XTRUE, 0x22222222),
				//colorAlloc(sizeof(RColor), 128),//, XTRUE, 0x44444444),
				objectAlloc(sizeof(SObject), 64),//, XTRUE, 0x55555555),
				commandAlloc(sizeof(XCOMMAND),64),//, XTRUE, 0x66666666),
				curveAlloc(sizeof(_XCURVE),64),//,XTRUE,0x77777777)
				formatAlloc(sizeof(_XTEXTFORMAT),64),
				loadAlloc(sizeof(_XLoadInfo),32),
				threadAlloc(sizeof(ScriptThread),200),
				editAlloc(sizeof(EditText),12),
				cmdAlloc(sizeof(XCTRLCMD),64),
				rectAlloc(sizeof(SRECT),16),
				infoAlloc(sizeof(AS3ButtonInfomation),16),
				matrixAlloc(sizeof(MATRIX),128),
				edgeAlloc(sizeof(REdge), 600),//, XTRUE, 0x22222222),
				colorAlloc(sizeof(RColor), 64)//, XTRUE, 0x44444444),
				//objectAlloc(sizeof(SObject), 32, XTRUE, 0x55555555),
				//commandAlloc(sizeof(XCOMMAND),32, XTRUE, 0x66666666),
				//curveAlloc(sizeof(_XCURVE),32,XTRUE,0x77777777)
{
	// Set up the root object
	mfPadW = 0;
	mfPadH = 0;
	mpRasterGL = RasterGL::sharedRasterGL();

	MatrixIdentity( &mControlMat );

//	m_bIsLocked=false;
	root.display = this;
	bFocusRect=true;
	m_bInvalidate=false;
	player=XNULL;
	root.parent = 0;
	nAspect = 0;

	root.above = 0;
	root.CreatePlayer=NULL;
	root.bottomChild = 0;
//	root.scrollObject = 0;
	root.pFilter=NULL;
	root.gridRect=NULL;
	//root.gridObjects=NULL;
	//root.pNext=NULL;
	//root.pPrev=NULL;
	root.character = 0;
	root.depth = 0;
	root.clipObject=NULL;
	root.clipDepth=0;
	root.pASObject=NULL;
	root.ratio = 0;
	//root.name = 0;
	root.xform.Clear();
	RectSetEmpty(&root.devBounds);
	root.state = 0;
	root.drawn = XFALSE;
    root.puppet = XFALSE;
	root.name=XNULL;
	root.blendMode=0;
	root.cacheMode=0;
	root.scrollRect=NULL;
	root.nForeColor=root.nBackColor=0;
	root.exflags=_XSObject::XEN_ALL;
	root.tabIndex=-1;
	root.tagType=rootChar;
	root.nCreateTime=0;//player->m_nLoopCount;
	//root.edges = 0;
	//root.colors = 0;
	root.thread = 0;
	root.flags = SObject::OB_ENABLE|SObject::OB_VISIABLE;//XTRUE;
	//root.enable = XTRUE;
	root.pObject=XNULL;
	root.pCacheBmp=XNULL;
	root.pCacheMat=XNULL;
	root.pLink=XNULL;
	root.pLinkBy=XNULL;
	root.mpShapeList = XNULL;
	root.mShapeRecord.Init();
//	thread=0;
//	threadState=0;

	root.dragCenter = XFALSE;
	RectSetEmpty(&root.dragConstraint);
	root.dropTarget = 0;

#ifdef EDITTEXT
	root.editText = 0;
#endif

	holdList = 0;
	holdParent = 0;

	threads = 0;

	instanceNameCount = 0;

	camera.Clear();

	//camera.mat.a/=20;
	//camera.mat.d/=20;

//	antialias_b = XFALSE;
//	antiShift = 1;
	faster = XFALSE;
	useDeviceFont = XTRUE;
//	bits = 0;
	dirty = XFALSE;
	RectSetEmpty(&screenDirtyRgn);
	RectSetEmpty(&devDirtyRgn);
	nDirty = 0;
	UpdateDevViewRect();

	backgroundColor.all = SRGBBlack;//SRGBWhite;
	backgroundColorPriority = 0;

	// Button info
	buttonState = 0;//buttonState[1] =0;
	button = 0;//button[1] =0;
//	thread = 0;
	RectSetEmpty(&tabFocusRect);
	useFocusRect = XTRUE;
	buttonParent= 0;
	buttonCharacter= 0;
	buttonDepth= 0;
	buttonRatio= 0;
	actionURL[0] = 0;

#ifdef EDITTEXT
	// Text editing
	iBeam = XFALSE;
#endif
//	player=XNULL;
	hasDeviceEditText = XFALSE;
	//pRaster=&raster;
	mpRasterGL = RasterGL::sharedRasterGL();
	//mpRasetGL->Init();
//	actionCallStackTop = 0;

//	freeObjs=XNULL;
}

DisplayList::~DisplayList()
{
	FreeAll();
}

#define CheckObject(AA)

_XSObject* DisplayList::CreateObject(ScriptPlayer*player,XU32 t)
{
	_XSObject* p = (_XSObject*)objectAlloc.Alloc();
#ifdef _WINEMU
	if(p->nCreateTime)
	{
		this->player->Trace(XString16("Error: Alloc display object error!(Alloc an exist object)"));
	}
#endif
	p->CreatePlayer=player;
	p->dropTarget=NULL;//.nType = 0;
	p->name.nType=0;
	p->nCreateTime=t;//this->player->m_nLoopCount;
	p->scrollRect=NULL;
	p->gridRect=NULL;
	p->mShapeRecord.Init();
	//p->gridObjects=NULL;
	p->name=_strEmpty;
	return p;
}

//_XSObject* __obj=NULL;

void DisplayList::FreeObject(_XSObject* o)
	{
		//if(o->character&&o->character->tag==190)
		//	int v=0;
		//_XSObject*p=root.GetItemParent(o,NULL);
		//if(p)
		//	int v=0;
		//if(o->character&&o->character->type==98)//parent&&o->parent->character&&o->parent->character->tag==129)//o->character&&o->character->type==20)
		//	int v=0;
		//if(o==__obj)//->character&&o->character->type==loaderImageChar)
		//	int v=0;
		if(this->button==o)
			button=NULL;
		if(!o->nCreateTime)
		{
#ifdef _WINEMU
			player->Trace(XString16("Error: Free display object error(Free null)!"));
#endif
			return;
		}

			//int v=0;
		/*if(IsRunning()&&o->GetPlayer()&&o==o->GetPlayer()->rootObject)//(o->character->type==rootChar||o->character->tagCode==ctagBitmapRoot))
		{
			//FreeRoot(o);
			o->GetPlayer()->rootObject=NULL;
		}*/
		o->nCreateTime=0;
		objectAlloc.Free(o);
	}

void DisplayList::AddThread(ScriptThread* thread)
{
	FLASHASSERT(!thread->next);

	//if(rootObject)
	//	rootObject->AS3AddedToStage();
	thread->m_bInDisplay=true;
	_XSObject*rootObject=thread->rootObject;

	thread->next = threads;
	threads = thread;

	//if(rootObject)
	//	rootObject->AS3AddedToStage();
	if(rootObject&&rootObject->AS3NeedCallFrame())
	{
		rootObject->AS3ClearCallFrame();
		//Modify for...,Mybe BUG
		if(thread->m_bAS3Frame0&&thread->curFrame<=0)
		{
			thread->m_bAS3Frame0=false;
			thread->m_bNeedCallFrame=false;
			rootObject->CallFrame(0);
		}
	}
}

void DisplayList::RemoveAllThread(ScriptPlayer*pl)
{
	if(!threads) return;
	ScriptThread** link = &threads;
	while ( *link ) {
		ScriptThread*thread=*link;
		if ( thread->player==pl ) {
			*link = thread->next;
			//if(thread->rootObject)
			//	thread->rootObject->AS3RemoveFromStage();
			thread->m_bInDisplay=false;
		}
		else link = &(*link)->next;
	}
}
void DisplayList::RemoveThread(ScriptThread* thread)
{
	/*if(thread->rootObject&&
		thread->rootObject->character&&
		thread->rootObject->character->tag==187)
		int v=0;/*
	// Clear any pending actions on a thread that is going away

	//player->RemoveThread(thread);
	// If we are inside a DoActions, remove pending actions on
	// the action context stack
/*	ActionContext* actionContext = actionCallStack;
	for (int i=0; i<actionCallStackTop; i++) {
		if (actionContext->targetThread == thread) {
			actionContext->targetThread = 0;
			actionContext->useTarget = XFALSE;
		}
		actionContext->actionList->RemoveObject(thread->rootObject);
		actionContext++;
	}*/


	actionList.RemoveObject(thread->rootObject);

	// Remove the thread from the list
	ScriptThread** link = &threads;
	while ( *link ) {
		if ( *link == thread ) {
			*link = thread->next;
			thread->m_bInDisplay=false;
			if(player->m_pNextDoThread&&player->m_pNextDoThread==thread)
			{
				player->m_pNextDoThread=thread->next;
			}
			//if(thread->rootObject)
			//	thread->rootObject->AS3RemoveFromStage();
			return;
		}
		link = &(*link)->next;
	}

	FLASHASSERT(XFALSE);
}

void DisplayList::Reset()
{
	if ( backgroundColorPriority <= 1 ) {
		backgroundColor.all = SRGBBlack;//SRGBWhite;
		backgroundColorPriority = 0;
		Invalidate();
 	}
}

S32 RectArea(SRECT* area)
{
	return (area->xmax-area->xmin) * (area->ymax-area->ymin);
}

inline S32 UnionArea(SRECT* r1, SRECT* r2)
{
	SRECT area;
	RectUnion(r1, r2, &area);
	return RectArea(&area);
}

BOOL RectTestOverlap(P_SRECT r1, P_SRECT r2)
{
	return r1->xmin < r2->xmax && r2->xmin < r1->xmax &&
		   r1->ymin < r2->ymax && r2->ymin < r1->ymax;
}

BOOL DisplayList::MergeDirtyList(BOOL forceMerge)
// Merge the pair of rectangles that will cause the smallest increase in the total dirty area
{
	if ( nDirty > 1 ) {
		// Check to merge areas to reduce the number
		S32 bestDelta = forceMerge ? 0x7FFFFFFFL : 0;	// if there is no empty slot, we must merge
		int mergeA = 0;
		int mergeB = 0;
		for ( int i = 0; i < nDirty-1; i++ ) {
			for ( int j = i+1; j < nDirty; j++ ) {
				S32 delta = UnionArea(devDirtyRect+i, devDirtyRect+j) - devDirtyArea[i] - devDirtyArea[j];
				if ( bestDelta > delta ) {
					mergeA = i;
					mergeB = j;
					bestDelta = delta;
				}
			}
		}

		if ( mergeA != mergeB ) {
			RectUnion(devDirtyRect+mergeA, devDirtyRect+mergeB, devDirtyRect+mergeA);
			devDirtyArea[mergeA] = RectArea(devDirtyRect+mergeA);
			for ( int i = mergeB+1; i < nDirty; i++ ) {
				devDirtyRect[i-1] = devDirtyRect[i];
				devDirtyArea[i-1] = devDirtyArea[i];
			}
			nDirty--;
			return XTRUE;
		}
	}
	return XFALSE;
}


void DecomposeRect(SRECT* r1, SRECT* r2)
// Restructure two overlaping rectangles to eliminate the intersecting area
//	while still covering the same area and perhaps more area
{
	FLASHASSERT(RectTestOverlap(r1, r2));

	// Build the 3 rect slabs on y-axis
	SRECT r[3];
	if ( r1->ymin < r2->ymin ) {
		r[0].ymin = r1->ymin;
		r[0].ymax = r2->ymin;
		r[0].xmin = r1->xmin;
		r[0].xmax = r1->xmax;
	} else {
		r[0].ymin = r2->ymin;
		r[0].ymax = r1->ymin;
		r[0].xmin = r2->xmin;
		r[0].xmax = r2->xmax;
	}
	if ( r1->ymax < r2->ymax ) {
		r[2].ymin = r1->ymax;
		r[2].ymax = r2->ymax;
		r[2].xmin = r2->xmin;
		r[2].xmax = r2->xmax;
	} else {
		r[2].ymin = r2->ymax;
		r[2].ymax = r1->ymax;
		r[2].xmin = r1->xmin;
		r[2].xmax = r1->xmax;
	}
	r[1].ymin = r[0].ymax;
	r[1].ymax = r[2].ymin;
	r[1].xmin = Min(r1->xmin, r2->xmin);
	r[1].xmax = Max(r1->xmax, r2->xmax);

	// Combine the middle slab with the slab that will generate the smallest area
	S32 a[3];
	for ( int i = 0; i < 3; i++ )
		a[i] = RectArea(r+i);

	SRECT u1, u2;
	RectUnion(&r[0], &r[1], &u1);
	RectUnion(&r[1], &r[2], &u2);

	S32 delta0 = a[0] + a[1] - RectArea(&u1);
	S32 delta1 = a[1] + a[2] - RectArea(&u2);

	if ( delta0 > delta1 ) {
		*r1 = u1;
		*r2 = r[2];
	} else {
		*r1 = r[0];
		*r2 = u2;
	}
}

void DisplayList::DecomposeDirtyList()
// Decompose any overlapping rectangles into non-overlapping rectangles
{
	// Merge any areas that would reduce the total area
	while ( MergeDirtyList(XFALSE) ) {}

	for ( int i = 0; i < nDirty-1; i++ ) {
		for ( int j = i+1; j < nDirty; j++ ) {
			if ( RectTestOverlap(devDirtyRect+i, devDirtyRect+j) ) {
				DecomposeRect(devDirtyRect+i, devDirtyRect+j);
			}
		}
	}
}



void DisplayList::InvalidateRect(SRECT* r)
{
	SRECT rect;
	rect.xmin = r->xmin;
	rect.xmax = r->xmax;
	rect.ymin = r->ymin;
	rect.ymax = r->ymax;

	RectInset(-4, &rect);
	if ( RectTestIntersect(&devViewRect, &rect) )
	//if(r->xmin!=r->xmax||r->ymin!=r->ymax)
	{
		//if(r->xmin<100)
		//	int vv=0;
		/*FLASHASSERT(nDirty < maxDirtyAreas);
		// Add to the list
		RectIntersect(&devViewRect, &rect, &devDirtyRect[nDirty]);
		RectUnion(&devDirtyRgn, &devDirtyRect[nDirty], &devDirtyRgn);	// add to the dirty region
		devDirtyArea[nDirty] = RectArea(devDirtyRect+nDirty);	// add to the list*/
		nDirty++;

		//MergeDirtyList(nDirty == maxDirtyAreas);
	}
}

void FreeCache(SObject* parent)
{
	parent->FreeCache();
	for ( SObject* obj = parent->bottomChild; obj; obj = obj->above )
		FreeCache(obj);
}

void DisplayList::FreeCache()
// free the cached data to reduce memory load
{
	::FreeCache(&root);
}

void DisplayList::TransMat( float x, float y )
{
	mControlMat.tx += x * 20;
	mControlMat.ty += y * 20;
	UpdateMat();
}

void DisplayList::ScaleMat( float fa, float fd )
{
	MATRIX temp;
	MatrixIdentity( &temp );
	//temp.a = fa * fixed_1;
	//temp.d = fd * fixed_1;
	mControlMat.a = fa * fixed_1;
	mControlMat.d = fd * fixed_1;
	//MatrixConcat( &mControlMat, &temp, &mControlMat );
	UpdateMat();
}

void DisplayList::ScaleTranMat( float fa, float fd, SPOINT pos )
{
	
	//MatrixTransformPoint( &mControlMat, &pos, &p1 );
	float tx0 = (pos.x*20-mControlMat.tx)*65536.0/mControlMat.a;
	float ty0 = (pos.y*20-mControlMat.ty)*65536.0/mControlMat.d;
	mControlMat.a = fa * fixed_1;
	mControlMat.d = fd * fixed_1;
	//mControlMat.tx = 
	mControlMat.tx = pos.x*20-tx0*fa;
	mControlMat.ty = pos.y*20-ty0*fd;
	
	//MatrixTransformPoint( &mControlMat, &pos, &p2 );
	//mControlMat.tx = (p2.x - p1.x) * 20;
	//mControlMat.ty = (p2.y - p1.y) * 20;

	//mControlMat.tx = (1 - fa )* pos.x * 20;
	//mControlMat.ty = (1 - fd )* pos.y * 20;
	
	UpdateMat();
}

void DisplayList::ZoomMat( SRECT *rect1, SRECT *rect2 )
{
	MATRIX temp;
	MatrixIdentity( &temp );
	float fw = (float)RectWidth( rect2 ) / RectWidth( rect1 );
	float fh = (float)RectHeight( rect2 ) / RectHeight( rect1 );
	float ff = 0;
	if ( fabsf( fw -1 ) > fabsf( fh -1 ))
	{
		ff = fw;
	}
	else
	{
		ff = fh;
	}
	temp.a = fixed_1 * ff;
	temp.d = temp.a;
	temp.tx = ((rect2->xmax - rect2->xmin) / 2 - ( rect1->xmax - rect1->xmin ) /2) * 20;
	temp.ty = (( rect2->ymax - rect2->ymin) / 2 - (rect1->ymax - rect1->ymin) / 2) * 20;
	MatrixConcat( &mControlMat, &temp, &mControlMat );
	UpdateMat();

}

void DisplayList::UpdateMat()
{
	MATRIX tempMat;
	MatrixIdentity( &tempMat );
	MatrixConcat( &camera.mat, &mControlMat, &tempMat );
	RasterGL::sharedRasterGL()->SetCamera( tempMat );
	ModifyCamera(false);
}

void DisplayList::SetCamera( SRECT* frame )
{
	this->player->m_nDelayShow = 20;
	UpdateDevViewRect();
	//return;
	//RasterGL::sharedRasterGL()->setViewPort( 0, 100, RectWidth(viewRct),RectHeight(viewRct) );
	//RasterGL::sharedRasterGL()->setOrthoProjection( RectWidth(&viewPort),RectHeight(&viewPort));//RectWidth( frame), RectHeight( frame ) );
	float width = RasterGL::sharedRasterGL()->m_WinSize.cx * 20; // RectWidth( &viewPort);
	float height = RasterGL::sharedRasterGL()->m_WinSize.cy * 20; // RectHeight( &viewPort );
	float flashw =this->player->GetStageWidth() * 20; // RectWidth( frame );
	float flashh = this->player->GetStageHeight() * 20; // RectHeight( frame );
	float sx = width/flashw;
	float sy = height/flashh;
	MATRIX newMat;
	::MatrixIdentity(&newMat);
	mfPadW = 0;
	mfPadH = 0;
	if(player->GetConfig()->m_nZoomMode==2)
	{
		newMat.a = sx*fixed_1;
		newMat.d = sy*fixed_1;
		newMat.tx = 0;
		newMat.ty = 0;
		mfPadH = 0;
		mfPadW = 0;
	}
	else if(sx<sy)
	{
		newMat.a = newMat.d = sx*fixed_1;
		newMat.tx = 0;
		newMat.ty = ( height - flashh*sx)/2;
		
		mfPadH = (height-flashh*sx);
		//newMat.tx = -frame->xmin * sx;
		//newMat.ty = ( height - flashh*sx)/2 - frame->ymin * sx;
	}
	else
	{
		newMat.a = newMat.d = sy*fixed_1;
		newMat.tx = ( width - flashw*sy)/2;
		newMat.ty = 0;
		mfPadW = (width - flashw*sy);
		//newMat.tx = ( width - flashw*sy)/2 + frame->xmin * sy;
		//newMat.ty = frame->ymin;
	}

	camera.mat = newMat;

	//MATRIX scalmat, tranMat, resultmat;
	//MatrixIdentity( &scalmat );
	//MatrixIdentity( &resultmat );
	//MatrixIdentity( &tranMat );
	//float scale1 = flashw / RectWidth( frame );
	//scalmat.a = scalmat.d = fixed_1 * scale1;
	////scalmat.tx = -frame->xmin * scale1;
	////scalmat.ty = -frame->ymin * scale1;

	//tranMat.a = tranMat.d = fixed_1;
	//tranMat.tx = -frame->xmin * scale1 * sx;
	//tranMat.ty = -frame->ymin * scale1 * sx;
	//
	//MatrixConcat( &newMat, &scalmat, &newMat );
	//MatrixConcat( &newMat, &tranMat, &resultmat);
	
	//camera.mat = newMat;

	//MATRIX tempMat;
	//MatrixIdentity( &tempMat );
	//MatrixConcat( &camera.mat, &mControlMat, &tempMat );
	::MatrixIdentity(&mControlMat);
	RasterGL::sharedRasterGL()->SetCamera( camera.mat );//tempMat );
	root.Modify();
	/*float scalex =width / flashw;
	float scaley = height / flashh;

	//newMat.a = FixedDiv( )

	MATRIX newMat;
	if( scalex < scaley )
	{
		float scalxy = scaley / scalex;
		newMat.a = fixed_1;
		newMat.d = fixed_1 * scalxy;
		newMat.b = newMat.c = 0;
		newMat.tx = frame->xmin;
		newMat.ty = -((height - flashh * scalex) / 2 ) / scalex + frame->ymin;// -100 * 20;
	}
	else
	{
		float scalxy = scalex / scaley;
		newMat.a = fixed_1 * scalxy;
		newMat.d = fixed_1;// * scalxy;
		newMat.b = newMat.c = 0;
		newMat.tx = -((width - flashw * scaley) / 2) / scaley + frame->xmin;
		newMat.ty = frame->ymin;// -((height - flashh * scalex) / 2 ) / scalex;// -100 * 20;
	}*/

	

	//RasterGL::sharedRasterGL()->setViewPort( 0, 0, 821, 592 );
	//RasterGL::sharedRasterGL()->setOrthoProjection( RectWidth( frame), RectHeight( frame ) );
	


	////return;
	//MATRIX newMat;
	//#define minMapSize 0x10L
	//FLASHASSERT(!RectIsEmpty(&viewPort) && !RectIsEmpty(frame)); 
	//newMat.a = FixedDiv(FixedMax(RectWidth(&viewPort), minMapSize), FixedMax(RectWidth(frame), minMapSize));
	////if(newMat.a>4000)
	////	int v=0;
	//newMat.d = FixedDiv(FixedMax(RectHeight(&viewPort), minMapSize), FixedMax(RectHeight(frame), minMapSize));
	//newMat.b = newMat.c = 0;

	//// Adjust the scaling to maintain the aspect ratio
	//switch ( scaleMode & scaleMask ) 
	//{
	//	case showAll:
	//		newMat.a = newMat.d = FixedMin(newMat.a, newMat.d);
	//		break;
	//	case noBorder:
	//		newMat.a = newMat.d = FixedMax(newMat.a, newMat.d);
	//		break;
	//	case exactFit:
	//		//MatrixMap(frame, &viewPort, &newMat);
	//		break;
	//	case noScale:
	//		newMat.a = newMat.d = fixed_1;
	//		break;
	//}

	//// Pick the alignment point
	//SPOINT srcRef, dstRef;
	//if ( scaleMode & alignLeft ) {
	//	srcRef.x = frame->xmin;
	//	dstRef.x = viewPort.xmin;
	//} else if ( scaleMode & alignRight ) {
	//	srcRef.x = frame->xmax;
	//	dstRef.x = viewPort.xmax;
	//} else {
	//	srcRef.x = FixedAverage(frame->xmin, frame->xmax);
	//	dstRef.x = FixedAverage(viewPort.xmin, viewPort.xmax);
	//}
	//if ( scaleMode & alignTop ) {
	//	srcRef.y = frame->ymin;
	//	dstRef.y = viewPort.ymin;
	//} else if ( scaleMode & alignBottom ) {
	//	srcRef.y = frame->ymax;
	//	dstRef.y = viewPort.ymax;
	//} else {
	//	srcRef.y = FixedAverage(frame->ymin, frame->ymax);
	//	dstRef.y = FixedAverage(viewPort.ymin, viewPort.ymax);
	//}

	//// Calculate the translation based on the alignment
	//newMat.tx = dstRef.x - FixedMul(srcRef.x, newMat.a);
	//newMat.ty = dstRef.y - FixedMul(srcRef.y, newMat.d);

	//newMat.tx = newMat.tx / 20;
	//newMat.ty = newMat.ty / 20;

	////if(newMat.tx>0) newMat.tx=0;
	////if(newMat.ty>0) newMat.ty=0;

	//// Don't invalidate unless there was a change
	////BOOL smoothBits = nQuality>1 && !faster;
	////if ( newMat.a  != camera.mat.a  || newMat.b  != camera.mat.b ||
	////	 newMat.c  != camera.mat.c  || newMat.d  != camera.mat.d ||
	////	 newMat.tx != camera.mat.tx || newMat.ty != camera.mat.ty ||
	////	 antialias_b != smooth || raster.smoothBitmaps != smoothBits ) {
	////	antialias_b = smooth;
	//	camera.mat = newMat;
	////	RasterGL::sharedRasterGL()->SetCamera( camera.mat );
	////	raster.smoothBitmaps = smoothBits;
	////	raster.bitmapDither = antialias_b ? errorDiffusionDither : orderedDither;
	////	ModifyCamera();
	////}

}

void DisplayList::UpdateDevViewRect()
// set the dirty area to the whole view
{

	devViewRect.xmin = devViewRect.ymin = 0;
	/*if ( bits && bits->BitsValid()) {
// 		devViewRect.xmax = bits->m_bufWidth;
// 		devViewRect.ymax = bits->m_bufHeight;
		devViewRect.xmax = bits->width();
		devViewRect.ymax = bits->height();

	}*/
	if(this->mpRasterGL)
	{
		devViewRect.xmax = mpRasterGL->m_WinSize.cx*20;
		devViewRect.ymax = mpRasterGL->m_WinSize.cy*20;
	}
	else {
		devViewRect.xmax = 2000;
		devViewRect.ymax = 2000;
	}

}

void DisplayList::ModifyCamera(bool bModify)
{
	// Free all of the transformed edges and update the object matricies
	UpdateDevViewRect();
	Invalidate();
	if(bModify)
		root.Modify();
}

void DisplayList::SetBackgroundColor(SRGB color, int priority)
{
	if ( priority > backgroundColorPriority ) {
		if ( backgroundColor.all != color.all ) {
			Invalidate();
			backgroundColor = color;
		}
		backgroundColorPriority = priority;
	}
}

void DisplayList::CalcBitsDirty(SRECT* devDirty, SRECT* bitsDirtyRgn)
{
	*bitsDirtyRgn = *devDirty;
}

//class SStroker;
/*void AddRect(P_SRECT rect,
			 P_MATRIX mat, RColor* color,
			 DisplayList *display, REdge** edgeList,
			 SStroker* stroker);*/

void DisplayList::GetCameraMatrix(MATRIX&mat)
{
	::MatrixConcat(&camera.mat,&mControlMat,&mat);
	//mat = camera.mat;
}

void DisplayList::UpdateRect(SRECT* clip)
{
	 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//if ( bits->LockBits() )
	//return;
	_ANALYENTER("Update Rect");
	{
		//mpRasetGL->QualityAntiShift( antiShift );
		//this->backgroundColor
		float bkColor[4]={backgroundColor.rgb.red/255.0,
						  backgroundColor.rgb.green/255.0,
						  backgroundColor.rgb.blue/255.0,
						  backgroundColor.rgb.transparency/255.0};
		//float zero[]={0,0,0,0};
		RasterGL*raster=RasterGL::sharedRasterGL();
		
		//RasterGL::sharedRasterGL()->drawBlendMode();
		//RasterGL::sharedRasterGL()->drawOffScreen( true );
        //RasterGL::sharedRasterGL()->drawStyleLine();
		//MATRIX mat;
		STransform form;
		::MatrixIdentity(&form.mat);
		form.cxform = camera.cxform;
		float w=this->player->GetStageWidth()*20;
		float h=this->player->GetStageHeight()*20;
		//raster->DrawRectangle(0,0,w,h,bkColor);
		root.DrawCached(this->mpRasterGL,form,clip);
		raster->BeginPaint(bkColor);
		//raster->drawStencil();
		//raster->EndPaint();
		//return;
		
		root.Draw( this->mpRasterGL,form,false,clip);

		
		float blackColor[]={0,0,0,1};
		if(mfPadH>0)
		{
			//float hh=mfPadH/2;
			raster->DrawRectangle(0,-mfPadH,w,mfPadH,blackColor,NULL);
			raster->DrawRectangle(0,h,w,mfPadH,blackColor,NULL);
		}
		if(mfPadW>0)
		{
			//float hw=mfPadW/2;
			raster->DrawRectangle(-mfPadW,0,mfPadW,h,blackColor,NULL);
			raster->DrawRectangle(w,0,mfPadW,h,blackColor,NULL);
		}

		int nCount=this->player->m_buttons.GetSize();
		if(nCount)
		{
			MATRIX mat,invMat;
			this->GetCameraMatrix(mat);
			::MatrixInvert(&mat,&invMat);
			float fMat[16];
			raster->SetGLMatrix(invMat,fMat);
			int i;
			for(i=0;i<nCount;i++)
			{
				_BUTTONDRAWINFO&info=player->m_buttons[i];
				XPOINT ppt={info.pt.x*20,info.pt.y*20};
				XRECT  rrt={info.rect.left*20,info.rect.top*20,info.rect.right*20,info.rect.bottom*20};
				raster->DrawButton(fMat,info.pDraw,ppt,rrt,info.alpha,info.pLight);
			}
		}
        //setupGraphics(1232, 800);
        //renderFrame();
		raster->EndPaint();
	}
	
	//CCEGLView::sharedOpenGLView()->swapBuffers();
	//if(camera.mat.tx!=0)
	//	int v=0;
	//if(!raster.bits) return;
	//XLock lock;
	//if(RectHeight(clip)==0) return;
	//if(player->m_bDragOrZoom)
	//return;
//	if(m_bIsLocked) return;
//	//	int v=0;
//	if ( bits->LockBits() ) {
//		raster.Attach(bits, clip, antialias_b,antiShift);
//		raster.BeginPaint();
//
//		// Add the background edges
//		REdge bkEdges[2];
//		RColor bkColor;
//		SRGB backColor;
//		backColor.all = 0;
//#ifdef _BACKLAYER
//		bool bBG=player->m_bBGLayer;//this==player->m_pBGDisplay;
//		if(bBG)
//			backColor.all=0;//0xffffff;//player->display.backgroundColor.all;
//		else
//		{
//			backColor.all=backgroundColor.all;
//			//backColor.rgb.transparency=0xff;
//		}
//#else
//        backColor.all = backgroundColor.all;
//		/*else
//		{
//			bits->UnlockBits();
//			return;
//			if(player->m_bBGLayer)
//				backColor.all=0;
//			else
//				backColor.all=backgroundColor.all;
//		}*/
//#endif
//		if ( backColor.all != 0 ) {
//			// Set up the color
//			bkColor.SetUp(&raster);
//			bkColor.order = 0;
//			bkColor.rgb.alpha = backColor.rgb.transparency;
//			bkColor.rgb.red   = backColor.rgb.red;
//			bkColor.rgb.green = backColor.rgb.green;
//			bkColor.rgb.blue  = backColor.rgb.blue;
//			bkColor.BuildCache();
//
//			// Set up the edges
//			bkEdges[0].nextObj = &bkEdges[1];
//			bkEdges[1].nextObj = 0;
//
//			SPOINT pt1, pt2;
//			CURVE c;
//			pt1.y = raster.edgeClip.ymin;
//			pt2.y = raster.edgeClip.ymax;
//
//			pt1.x = pt2.x = raster.edgeClip.xmin;
//			CurveSetLine(&pt1, &pt2, &c);
//			bkEdges[0].Set(&c);
//
//			pt1.x = pt2.x = raster.edgeClip.xmax;
//			CurveSetLine(&pt1, &pt2, &c);
//			bkEdges[1].Set(&c);
//
//			bkEdges[0].dir 		= bkEdges[1].dir      = 0;
//			bkEdges[0].fillRule = bkEdges[1].fillRule = fillEvenOddRule;
//			bkEdges[0].color1 	= bkEdges[1].color1   = &bkColor;
//			bkEdges[0].color2 	= bkEdges[1].color2   = 0;
//
//			raster.AddEdges(bkEdges, &bkColor);
//		} else {
//			// It's transparent...
//#if _BACKLAYER
//			if(bBG)
//			{
//				XRect rect(clip->xmin,clip->ymin,clip->xmax,clip->ymax);
//				bits->bitmap->Clear(rect);
//			}
//			else
//				raster.getBackground = XTRUE;
//#else
//			raster.getBackground = XTRUE;
//#endif
//		}
//
//		{
//	
//			root.Draw(&raster, camera,XNULL);
//		}
//
//#ifndef DISABLE_TAB_FOCUS
//		// Add button focus rectangle
//		RColor yellow;
//		REdge* edges = NULL;
//
//		if (useFocusRect && !RectIsEmpty(&tabFocusRect)) {
//			// Set up the color
//			yellow.SetUp(&raster);
//			yellow.order = 0;
//			yellow.rgb.alpha = 255;
//			yellow.rgb.red   = 255;
//			yellow.rgb.green = 255;
//			yellow.rgb.blue  = 0;
//			yellow.BuildCache();
//
//			SRECT bounds = tabFocusRect;
//			MATRIX mat;
//			MatrixIdentity(&mat);
//
//			int border = 3;
//			if (antialias_b) {
//				border=ToAnti(border);
//				//border *= 4;
//			}
//
//			SRECT rect = bounds;
//			rect.xmax = bounds.xmin + border;
//			AddRect(&rect, &mat, &yellow, this, &edges, NULL);
//
//			rect.xmin = bounds.xmax - border;
//			rect.xmax = bounds.xmax;
//			AddRect(&rect, &mat, &yellow, this, &edges, NULL);
//
//			rect.xmin = bounds.xmin;
//			rect.ymax = bounds.ymin + border;
//			AddRect(&rect, &mat, &yellow, this, &edges, NULL);
//
//			rect.ymin = bounds.ymax - border;
//			rect.ymax = bounds.ymax;
//			AddRect(&rect, &mat, &yellow, this, &edges, NULL);
//
//			raster.AddEdges(edges, &yellow);
//		}
//#endif
//
//
//		raster.PaintBits();
//		//tt=XTime::GetTickCount()-tt;
//		//if(tt>50)
//		//	int v=0;
//
//#ifndef DISABLE_TAB_FOCUS
//		if (!RectIsEmpty(&tabFocusRect)) {
//			// Free the edges
//			REdge* e = edges;
//			while ( e ) {
//				REdge* next = e->nextObj;
//				FreeEdge(e);
//				e = next;
//			}
//			edges = 0;
//			yellow.FreeCache();
//		}
//#endif
//
//		if ( backgroundColor.all != 0 )
//			bkColor.FreeCache();
//
//		bits->UnlockBits();
//	}
}




void DisplayList::CalcUpdate()
{
	_ANALYENTER("CalcUpdate");
	if ( dirty ) {
		MATRIX mat;
		::MatrixConcat(&camera.mat,&this->mControlMat,&mat);
		root.CalcUpdate(&mat,NULL);
		dirty = XFALSE;

	}
}

void DisplayList::Update()
{
	//return;
	//if(!raster.bits) return;
	//if(m_bIsLocked) return;
	_ANALYENTER("Update Display");
	// Use multiple dirty rectangles to minimize area
	if(!this->mpRasterGL) return;
	bool bForce = false;
	if(this->mpRasterGL->mbFresh||dirty)
	{
		this->mpRasterGL->mbFresh = false;
		//this->dirty = true;
		bForce = true;
		//root.Modify();
	}
	
	CalcUpdate();

	/*RectSetEmpty(&devDirtyRgn);

	if ( nDirty > 0 ) {
		_ANALYENTER("Update");
		// Update the frame buffer
		DecomposeDirtyList();
		for ( int i = 0; i < nDirty; i++ ) {
			SRECT bitsDirtyRgn;
			CalcBitsDirty(devDirtyRect+i, &bitsDirtyRgn);
			RectUnion(&bitsDirtyRgn, &screenDirtyRgn, &screenDirtyRgn);
			UpdateRect(&bitsDirtyRgn);
		}
	}

	RectSetEmpty(&devDirtyRgn);*/
	if(nDirty||bForce)
	{
		UpdateRect(&this->devViewRect);
	}
	//else RasterGL::sharedRasterGL()->EndPaint();
	//RasterGL::sharedRasterGL()->mbFresh = (nDirty != 0);
	nDirty = 0;
	m_bInvalidate=false;
//	_ANALYLEAVE();
}


SObject* DisplayList::PlaceObject3(SObject* parent, PlaceInfo* info,void*pCnt)//XBOOL bUpdate)
{
	if(!parent)
		return XNULL;
	avmshell::XAVM2* pAVM2=XSWFPlayer::m_pInstance->m_pAVM2;
	
	SObject*obj=XNULL;
	//avmplus::ClassClosure* pClass=XNULL;
	FlashObject*pObj=XNULL;
	if(!info->className.IsNull())
	{
		ClassClosure*pClass=(ClassClosure*)pAVM2->FindClass(info->className.strTxt,pCnt,false);
		if(!pClass) return XNULL;
	
		VTable* ivtable = pClass->ivtable();
		ScriptObject*prototype = pClass->prototype;
		pObj=(FlashObject*)pClass->createInstance(ivtable,prototype);
		pObj->SetCodeContext((ShellCodeContext*)pCnt);//m_pCodeContext=(ShellCodeContext*)pContext;
		//DisplayObjectContainerObject*pCon=(DisplayObjectContainerObject*)parent;
		int argc=0,args[]={0,zeroIntAtom,zeroIntAtom};
		if(info->bImage) argc=2;
		pAVM2->ConstructInstance(NULL,pClass,pObj,pCnt,argc,args);
	}
	else
	{
		BitmapDataClass*pClass = pAVM2->GetToplevel()->getBitmapDataClass();
		pObj=pClass->CreateDumpBitmap(info->character);
	}

	if(info->bImage)
	{
		BitmapClass*pClass = pAVM2->GetToplevel()->getBitmapClass();
		if(!pAVM2->GetCore()->IsTypeClass(pObj,pAVM2->GetToplevel()->getBitmapDataClass()))
			return XNULL;
		pObj=pClass->CreateBitmap((BitmapDataObject*)pObj,(ShellCodeContext*)pCnt);
	}
	if(!pAVM2->GetCore()->IsTypeClass(pObj,pAVM2->GetToplevel()->getDisplayObjectClass()))
		return obj;

	obj = ((DisplayObjectObject*)pObj)->m_pObject;
	
	if(parent)
	{
		SObject** link = &parent->bottomChild;
		SObject** ilink=NULL;
		for (;;) {
			SObject* obj = *link;
			if ( !obj ) break;
			if ( obj->depth==-1)
			{
				if(ilink==NULL)
					ilink=link;
			}
	 		else if ( obj->depth >= (info->depth) )
			{
				//if(obj->depth==info->depth)
				//	int v=0;
				FLASHASSERT(obj->depth != info->depth);
				break;
			}
			else ilink=NULL;
			link = &obj->above;
		}
		if(ilink) link=ilink;
		obj->parent = parent;
		obj->above = *link;
		*link = obj;
		DisplayObjectContainerObject*pCon=(DisplayObjectContainerObject*)parent->pASObject;
		pCon->AddChildRef(pObj);
	}

	obj->pFilter=info->pFilter;
	obj->blendMode=info->blendMode;
	obj->cacheMode=info->cacheMode;
	obj->pFilter=info->pFilter;
	obj->blendMode=info->blendMode;
	
	obj->tagType=obj->character->type;
	obj->mpShapeList = XNULL;
	
	obj->xform = *info;
	obj->depth = info->depth;
	obj->ratio = info->ratio;
    obj->puppet = info->puppet;
	obj->drawn = XFALSE;
	obj->flags=SObject::OB_VISIABLE|SObject::OB_ENABLE;
	obj->dragCenter = XFALSE;
	bool bInst=false;

	if (info->name.IsNull()) 
	{
		obj->AS3SetNoName();
	} else {
		obj->name = info->name;//CreateStr(info->name);
		bInst=!obj->name.IsNull();//true;
		obj->exflags|=_XSObject::XEN_ISAS3NAMED;
	}

	if(!bInst&&parent)
		obj->SetInstance();

	obj->Modify();
	return obj;
}

SObject* DisplayList::PlaceObject2(SObject* parent, PlaceInfo* info,XSWFCONTEXT*pCnt,bool bInstance,ScriptPlayer*scplayer)//XBOOL bUpdate)
{
	// Find the insertion point
	//if(info->depth<0)
	//	int v=0;
	//if(parent&&parent->character&&parent->character->tag==129)
	//	int v=0;
	//if(info->character->tag==58)
	//	int v=0;
	SObject* obj = CreateObject(parent?parent->CreatePlayer:scplayer,player->m_nLoopCount);
	if ( !obj ) return 0;

	//if(info->blendMode)
	//	int v=0;
	//_XSObject*p=root.GetItemParent(obj,NULL);
	//if(p)
	//	int v=0;

	if(parent)
	{
		SObject** link = &parent->bottomChild;
		SObject** ilink=NULL;
		for (;;) {
			SObject* obj = *link;
			if ( !obj ) break;
			if ( obj->depth==-1)
			{
				if(ilink==NULL)
					ilink=link;
			}
	 		else if ( obj->depth >= (info->depth) )
			{
				//if(obj->depth==info->depth)
				//	int v=0;
				FLASHASSERT(obj->depth != info->depth);
				break;
			}
			else ilink=NULL;
			link = &obj->above;


		}

		if(ilink) link=ilink;
		obj->parent = parent;
		obj->above = *link;
		*link = obj;
	}
	//if(!*link&&*ilink)
	//	link=ilink;
	
	//if(info->character)
	//	ScriptThread::PlaceCharacter(info->character);

	// Create a new object



	// Add to list

//	obj->sorts=new SObject::XSortObject;

	//obj->nCreateTime=player->m_nLoopCount;
	// Set up object
	//info->
	obj->pFilter=info->pFilter;
	obj->blendMode=info->blendMode;
	//obj->cacheMode=0;
//#ifdef _WINEMU
//	if(info->character->bFilters)
//	{
//		info->character->bFilters=false;
//		if(obj->pFilter)
//		{
//			this->player->Trace(XString16("Waring:Filters set but not support!"));
//		}
//		if(obj->blendMode>1)
//		{
//			XString16 s("Waring:Not support blend mode ");
//			XString16 id;
//			id.FromInt(obj->blendMode);
//			s+=id;
//			this->player->Trace(s);
//		}
//	}
//#endif
//	obj->pAction=info->pAction;
	obj->clipObject=XNULL;
	obj->pLink=XNULL;
	obj->pLinkBy=XNULL;
	obj->pCacheBmp=XNULL;
	obj->pCacheMat=XNULL;
	obj->nForeColor=obj->nBackColor=0;
	obj->exflags=_XSObject::XEN_ALL;
	obj->tagType=info->character->type;
	obj->mpShapeList = XNULL;
	
	obj->tabIndex=-1;

	obj->cacheMode=info->cacheMode;
/*
    int static bshow = 0;
    if (info->character != NULL && info->character->tag == 94 ) {
        static int ii = 0;
        ii++;
        if ( ii > 2 )
        {
            bshow++;
            LOGWHEREMSG( "------------->");
        }
    }
    if (bshow ==1 )
    {
        LOGWHEREVAL( info->character->tag );
    }
    if ( bshow >= 2 && bshow < 50 )
    {
        bshow++;
        LOGWHEREVAL( info->character->tag );
    }
*/
    
	obj->character = info->character;
    
     //DPShowDbgInfo( );
    
	obj->xform = *info;
	obj->depth = info->depth;
	obj->ratio = info->ratio;
    obj->puppet = info->puppet;
	obj->drawn = XFALSE;
	//obj->nForeColor=0;
	//obj->nBackColor=0;
//    obj->visible = XTRUE;
//	obj->enable = XTRUE;
	obj->flags=SObject::OB_VISIABLE|SObject::OB_ENABLE;
	if(info->bClone)
		obj->flags|=SObject::OB_CLONEOBJ;
	obj->dragCenter = XFALSE;
	RectSetEmpty(&obj->devBounds);
	RectSetEmpty(&obj->dragConstraint);
	obj->dropTarget = NULL;
	obj->display = this;
	obj->bottomChild = 0;
//	obj->scrollObject = 0;
	//obj->pNext=NULL;
	//obj->pPrev=NULL;
	//obj->pFilter=NULL;
//	obj->nEventFlags=0;
//	obj->pActions=XNULL;
//	obj->edges = 0;
//	obj->colors = 0;
	//obj->blendMode=0;
	obj->cacheMode=0;
	obj->state = 0;
	obj->pObject=XNULL;
	obj->pASObject=XNULL;
	//if(bUpdate)
	//	obj->UpdateControl();
	bool bAS3=obj->character->splayer->IsAS3();
	//else
	//	int v=0;
	CacheText(obj);
	//if(obj->xform.cxform.nBlendMode)
//		int v=0;

#ifdef EDITTEXT
	obj->editText = 0;
#endif
	//if(info->name&&strcmp(info->name,"")==0)
	//	int v=0;


	//if(obj->character->tag==353)
	//	int v=0;
	//if(info->blendMode>drawBlendNormal1)

	if(obj->character->gridRect)
	{
		//if(obj->character->tag==154)
		//	int v=0;
		obj->gridRect=CreateRect();
		SParser parser;
		parser.Attach((U8*)obj->character->gridRect, 0);
		parser.GetRect(obj->gridRect);
		obj->gridRect->xmax/=20;
		obj->gridRect->ymax/=20;
		obj->gridRect->xmin/=20;
		obj->gridRect->ymin/=20;
	}
	// If this is a movie clip and it has no name,
	// generate a default name
	bool bInst=false;

	if (obj->character->type == spriteChar && info->name.IsNull()) {
		if(info->character->className)
		{
			obj->name=info->character->className;
			bInst=true;
		}
		else
		{
			XXVar var;
			var.SetString(_strInstance,32);
			//URLBuilder ub;
			//ub.AppendString("instance");
			//ub.AppendInt(++instanceNameCount);
			XXVar varc(++instanceNameCount);
			var.StringAdd(varc);
			//obj->strName=ub.buf;
			obj->name = var;//CreateStr(ub.buf);
		}
		obj->AS3SetNoName();
	} else {
		obj->name = info->name;//CreateStr(info->name);
		bInst=!obj->name.IsNull();//true;
	}

	if(!bInst&&parent)
		obj->SetInstance();


	obj->thread = parent ? parent->thread : 0;
	obj->clipDepth = info->flags & splaceDefineClip ? info->clipDepth : 0;//(info->flags & splaceDefineClip) != 0;
	obj->Modify();

	if(info->character->classEntry)
		obj->ClearInstance();

	switch ( obj->character->type ) {
		case bitsChar:
			 if(RectIsEmpty(&info->character->rbounds))
			 {
				 this->player->CreateIfEmpty(info->character);
				// if(info->character->bits.HasBits())
				 {
					 //int nDiv=info->character->bits.nImgDiv;
					 info->character->rbounds.xmin=0;
					 info->character->rbounds.ymin=0;
					 info->character->rbounds.xmax=(info->character->bits.width)*20;
					 info->character->rbounds.ymax=(info->character->bits.height)*20;
				 }
			 }
			 break;
		case rootChar:
			 if(bAS3)
				 obj->SetInstance();
			 else
				obj->CreateObject(XNULL,pCnt);
			 break;
		case buttonChar: {
			// Set up the button state
			int state = bsIdle;
			if ( obj->depth == buttonDepth && obj->parent == buttonParent &&
				 buttonCharacter == obj->character && obj->ratio == buttonRatio ) {
				// The button is currently being tracked
				FLASHASSERT(obj->character->type == buttonChar);
				button = obj;
				state = buttonState;
			}
			obj->ClearInstance();
		} break;

#ifdef EDITTEXT
		case editTextChar:
			obj->ClearInstance();
			break;
#endif

		case spriteExternalChar:
        {
			ScriptThread    *thread = 0;
            ScriptPlayer    *player = obj->GetPlayer();//character->player;
            int             startPos = 0;
            int             len = 0;
            int             numFrames = 0;

            if (info->flags & splaceCloneExternalSprite)
            {
                // means we are cloning an existing
                // external sprite
                if (player->len >= player->scriptLen)
                {
                    // we only clone the external
                    // sprite if it has been completely loaded
                    // allocate full player instead of thread
                    // it is easier to delete the external
                    // sprite if it has its own player
                    thread = new ScriptPlayer(XTRUE);
					thread->splayer=this->player;
                    startPos = player->startPos;
                    len = player->len;
                    numFrames = player->numFrames;
                    thread->script = player->script;
                    ((ScriptPlayer *) thread)->numFramesComplete = player->numFramesComplete;
                    player = (ScriptPlayer *) thread;
                    player->gotHeader = XTRUE;

                    // add a ref count to the script
                    // we don't want to duplicate it
                    // so we want to make sure it's
                    // protected from deletion for
                    // cloned external sprites
                    player->scriptRefCount = obj->GetPlayer()->scriptRefCount;
                    player->ScriptAddRefCount();
                }
            }
            else
            {
                // note that it's ok to have some of the
                // initial values of the len, etc. set
                // to zero. they will be set during pushdata
                thread = player;

                // add a ref count to the script
                // we don't want to duplicate it
                // so we want to make sure it's
                // protected from deletion for
                // cloned external sprites
                player->ScriptAddRefCount();
            }

            if ( thread )
            {

				thread->ClearState();
				thread->nEventFlags=info->nEventFlags;
				thread->pActions=info->pAction;
				obj->thread = thread;
				thread->player = player;
				thread->splayer=this->player;
				thread->layerDepth = obj->depth;
				thread->rootObject = obj;
				thread->SetDisplay(obj->display,info->bAdd);
				thread->Attach(obj->GetPlayer()->script, startPos);
				thread->len = len;
				thread->startPos = startPos;
				thread->numFrames = numFrames;

                // for non cloned sprite this won't draw the frame
                // untill we push data into the script (this is ok)
				//thread->DoEvent(CLIPLOAD,0);
				XXObject*pObj=obj->CreateObject(XNULL,pCnt);
				void*pSuper=bAS3?obj->AS3CreateInst(XNULL,XTRUE):XNULL;
				obj->OnEvent(0,this->player,XOBJEVENT_LOAD,XNULL);
				thread->DrawFrame(0, XFALSE,false);
				if(bAS3)
				{
					//obj->AS3CreateInstance(NULL,!info->name.IsNull());
					obj->AS3ConstructInst(pSuper);
					thread->m_bAS3Frame0=false;
					obj->CallFrame(0);
				}
				else if(pObj)
					obj->Constructor(pObj,pCnt);

			}
		} break;
		case spriteEmptyChar:
  		case spriteChar: {
			// Create a sprite thread
			ScriptThread* thread = CreateThread();
			thread->InitThread();
				//new ScriptThread();
			//int v=sizeof(ScriptThread);
			if ( thread ) {
				//if(obj->character->tag==1418)
				//	int v=0;
				thread->nEventFlags=info->nEventFlags;
				thread->pActions=info->pAction;
				thread->ClearState();
				obj->thread = thread;
				thread->player = obj->GetPlayer();//character->player;
				thread->splayer=this->player;
				thread->layerDepth = obj->depth;
				thread->rootObject = obj;
				thread->SetDisplay(obj->display,info->bAdd);
				thread->Attach(obj->character->data, 0);

				thread->startPos = 0;
				//if(obj->character->data)
				thread->len = obj->character->sprite.length;
				thread->numFrames = obj->character->sprite.numFrames;
				obj->ClearInstance();
				//if(!bInst)
 
			}
		} break;
		default:
			break;
	}
	if(bInstance)
	{
		obj->AS3Instance(info->bAdd,parent?parent->pASObject:XNULL);
		obj->AS3Construct();
	}

	return obj;
}

void FreeFilters(XFilter*p)
{
	while(p)
	{
		XFilter*pNext=p->m_pNext;
		delete p;
		p=pNext;
	}
}
void FreeActions(XClipAction*p)
{
	while(p)
	{
		XClipAction*pNext=p->pNext;
		delete p;
		p=pNext;
	}
}

SObject* DisplayList::MoveObject(SObject* parent, PlaceInfo* info)
{
	// See if there is already an object at this depth or find the insertion point
	SObject* obj = parent->bottomChild;
	while ( obj ) {
	 	if ( obj->depth>=0 && obj->depth >= info->depth )
			break;
		obj = obj->above;
	}

	if ( obj ) {
		if ( obj->depth != info->depth ) {
			FLASHASSERT(XFALSE);
			FreeFilters(info->pFilter);
			FreeActions(info->pAction);
			return 0;		// don't move the wrong object
		}

		if(info->pFilter)
		{
			obj->FreeFilter();
			obj->pFilter=info->pFilter;
		}
		if(info->pAction)
		{
			obj->thread->ClearActions();
			obj->thread->pActions=info->pAction;
		}
		// We found an object at this depth


        if (obj->puppet)
		{
			//FreeFilters(info->pFilter);
			//FreeActions(info->pAction);
            return 0;       // don't move the object if it's puppeted
		}

		// Move the proper fields of the object
		obj->Modify();

		if ( info->flags & splaceCharacter ) {
			// The type of sprite or button objects must never change
			if ( obj->character->type != info->character->type &&
				 (obj->character->type == spriteChar || info->character->type == spriteChar ||
				  obj->character->type == buttonChar || info->character->type == buttonChar) ) {
				FLASHASSERT(XFALSE);		// this should never happen, there was a beta build where we did export like this, so prevent the crash
			} else {
				if(obj->character!=info->character&&obj->mpShapeList)
				{
					ScriptThread::RemoveCharacter(obj);
					//obj->character->useCount--;
					//if(obj->character->record.m_pImage)
					//	obj->character->record.m_pImage->useCount++;
					//info->character->useCount++;
					//if(info->character->record.m_pImage)
					//	info->character->record.m_pImage->useCount++;
				}
				obj->character = info->character;
			}
		}
		if ( info->flags & splaceMatrix )
		{
			obj->xform.mat = info->mat;
			//if(obj->character->tag==672&&obj->parent->thread->curFrame==15)
			//	int v=0;


			if(info->character)
			{
				XConfig*pConfig=player->GetConfig();
				int id=pConfig->FindShowMap(info->character->tag,parent->character?parent->character->tag:0);
				if(id>=0)
				{
						_TAGSHOW&show=pConfig->m_showMaps[id];
						if(show.nZoom==0)
							obj->flags&=~SObject::OB_VISIABLE;
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
							//if(show.align!=_TAGSHOW::TOPLEFT)
							{
								SRECT rect;
								GetBoundingBox(obj,&rect);
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
							::MatrixConcat(&obj->xform.mat,&mat,&obj->xform.mat);
						}
				}
			}
			obj->UpdateControl();

		}
		if ( info->flags & splaceColorTransform )
			obj->xform.cxform = info->cxform;
		if ( info->flags & splaceRatio )
			obj->ratio = info->ratio;
	}
	else
	{
		FreeFilters(info->pFilter);
		FreeActions(info->pAction);
	}
	return obj;
}

inline static void FreeCacheBmp(SObject*obj)
{
	if(obj->pCacheBmp)
		obj->FreeCacheBmp();
	obj=obj->bottomChild;
	while(obj)
	{
		FreeCacheBmp(obj);
		obj=obj->above;
	}
}

void DisplayList::DoRemove(SObject** link,bool bEvent,bool bFinal)
{
	SObject* obj = *link;
	FLASHASSERT(obj);

	if(obj->pASObject&&!avmshell::m_bDestroying)
	{
		if(bEvent)
		{
			void*pASObject=obj->pASObject;
			void*pContext=obj->GetContext();
			//if(obj->character)
			//	pContext=obj->character->player->m_pContext;
			player->m_pAVM2->OnEvent(XAS3_EVENT_removedFromStage,false,pASObject,pContext);
			player->m_pAVM2->OnEvent(XAS3_EVENT_removed,false,pASObject,pContext);
		}
		if(obj->parent&&obj->parent->pASObject&&obj->AS3IsNamed())//!obj->name.IsNull())
		{
			player->m_pAVM2->ClearMember(obj->parent->pASObject,obj->name.strTxt);
		}
		player->m_pAVM2->RemoveSystem(obj->pASObject);
	}
	// Remove the contents of the old object
	if(IsRunning()) FreeCacheBmp(obj);
	if ( obj->drawn )
	{
		//if(obj->character&&obj->character->tag==46)
		//	int v=0;

		InvalidateRect(&obj->devBounds);

	}

	obj->FreeChildren(bFinal);
	obj->FreeContext(bFinal);
	actionList.RemoveObject(obj);


	// Remove from list
	if ( obj == button )
		button = 0;
	if(player->m_pFocus)
	{
		if( obj == player->m_pFocus||obj->IsParentOf(player->m_pFocus))
		{
			player->AS3SetFocus(NULL);//m_pFocus=NULL;
		}
	}
	//else if(obj==button[1])
	//	button[1]=0;
	*link = obj->above;
	obj->parent=XNULL;
	
	FreeObject(obj);
}

void DisplayList::RemoveObject(SObject* parent, int depth)
{
	// See if there is already an object at this depth or find the insertion point
	SObject* obj;
	SObject** link = &parent->bottomChild;
	for (;;) {
		obj = *link;
		if ( !obj )
            break;
        //DPShowDbgInfo( );
		if ( obj->oldDepth == depth )
			break;
	 	if ( obj->depth>=0 && obj->depth >= depth )
			break;
		link = &obj->above;
	}

	if ( parent->pASObject && obj && obj->depth != depth)
	{
		link = &obj->above;
		for (;;) 
		{
			obj = *link;
			if ( !obj )
                break;
            //DPShowDbgInfo();
			if ( obj->depth<0 && obj->oldDepth == depth )
			{
				DoRemove(link,true,false);
				return;
			}
	 	
			link = &obj->above;
		}
	}

	if ( obj && obj->depth == depth )
	{
	//	if(obj->character->tag==1177)
	//		int v=0;
//		if(obj->name)
//			parent->sorts->Remove(obj->name,obj->character->player->NeedLowCase());
//		obj->AS3SetLoaderInfo(XNULL);
		DoRemove(link,true,false);
	}
}


void DisplayList::RemoveObject(SObject* parent, SObject*robj)
{
	// See if there is already an object at this depth or find the insertion point
	SObject* obj;
	SObject** link = &parent->bottomChild;
	for (;;) {
		obj = *link;
		if ( !obj ) break;
	 	if ( obj==robj )
			break;
		link = &obj->above;
	}

	if ( obj )//&& obj->depth == depth )
	{
	//	if(obj->character->tag==1177)
	//		int v=0;
//		if(obj->name)
//			parent->sorts->Remove(obj->name,obj->character->player->NeedLowCase());
//		obj->AS3SetLoaderInfo(XNULL);
		DoRemove(link,true,false);
	}
}


void DisplayList::BeginHold(SObject* parent)
// This is used only for backwards seeks because removes all of the children
{
	// Save the old state for a later compare
	holdParent = parent;
	holdList = holdParent->bottomChild;
	holdParent->bottomChild = 0;
}

void DisplayList::FinishHold(bool bAVM2)
// Compare the old and the new list
// If an old object matches a new object, replace the new object with the old object
{
	FLASHASSERT(holdParent);

	SObject** oldLink = &holdList;
	SObject** newLink = &holdParent->bottomChild;

	/*if(!holdParent->bottomChild)
	{
		holdParent->bottomChild=holdList;
		holdList=NULL;
		return;
	}*/

	//SObject** newLink0=newLink;
	if(!bAVM2)
	{
		SObject** oldLink0=oldLink;
		for(;;)
		{
			SObject*oldObj=*oldLink0;
			if(!oldObj) break;
			if(oldObj->flags&SObject::OB_CLONEOBJ)
			{
				SObject**newLink0=newLink;
				for(;;)
				{
					SObject*newObj=*newLink0;
					if(!newObj) break;
					if(newObj->depth==oldObj->oldDepth)
					{
						//if(newLink==newLink0)
						//	newLink=&newObj->above;
						//DoRemove(newLink0);
						//DoRemove(oldLink0);
						oldObj->flags&=~SObject::OB_CLONEOBJ;
					}
					newLink0=&newObj->above;
				}
			}
			oldLink0=&oldObj->above;
		}
	/*SObject **newLink0=newLink;
	for(;;)
	{
		SObject*newObj=*newLink0;
		if(!newObj) break;

	}*/

		oldLink = &holdList;
		newLink = &holdParent->bottomChild;
	}
    for (;;) {
		SObject* oldObj = *oldLink;
		SObject* newObj = *newLink;

		if ( !oldObj )
			break;	// we are done

		//if(oldObj->character->tag==461)
		//	int v=0;
		//if(strcmp(oldObj->name.strTxt,"DocNur")==0)
		//	int v=0;
		if(newObj&&newObj->depth<0)
		{
			newLink = &newObj->above;
		}
		else if ( !newObj || oldObj->depth < newObj->depth || oldObj->depth<0 ) {
			if (oldObj->depth>0&&oldObj->IsAS3Flag())
			{
				if(!newObj)
					DoRemove(oldLink,true,false);
				else if(oldObj->depth==newObj->depth||oldObj->depth==newObj->oldDepth)
				{
					//if(newObj->depth>0)
					//	DoRemove(newLink,false);
					*oldLink = oldObj->above;	// remove from old list
					oldObj->above = *newLink;	// place on new list
					*newLink = oldObj;
				}
				else DoRemove(oldLink,true,false);
			}
			else if ( /*oldObj->depth >= 0x4000*/oldObj->IsCloneObject()||oldObj->depth<0 )
			{
				// This is a cloned object, it always gets copied to the new list
				//FLASHASSERT(!newObj);	// none of the new obj's should have a depth > 4000
				//if(newObj)
				//	DoRemove(newLink);

				//if(oldObj->flags&SObject::OB_CLONEOBJ)
				//	int v=0;
				if(newObj)//&&newObj->depth>=0)
				{
					if(oldObj->flags&SObject::OB_CLONEOBJ&&
					   oldObj->oldDepth==newObj->depth)
						DoRemove(newLink,false,true);
					else //Mybe Bugs
						newLink=&newObj->above;
				}
				*oldLink = oldObj->above;	// remove from old list
				oldObj->above = *newLink;	// place on new list
				*newLink = oldObj;

			}
			/*else if(oldObj->depth!=newObj->depth)
			{
				DoRemove(newLink);			// delete the new obj
				*oldLink = oldObj->above;	// remove from old list
				oldObj->above = *newLink;	// place on new list
				*newLink = oldObj;
			}*/
			else {
				// Get rid of the old object
				DoRemove(oldLink,true,false);
			}

		} else if ( oldObj->depth == newObj->depth) {
			// Compare these objects

            BOOL remove = XTRUE;
			if (oldObj->character == newObj->character && oldObj->ratio == newObj->ratio && oldObj->clipDepth == newObj->clipDepth)
            {
				// Check the matrix and cxform and ratio
				BOOL moved = !Equal(&oldObj->xform, &newObj->xform);
				//if (!moved || oldObj->character->type == spriteChar)
				if(oldObj->pFilter!=newObj->pFilter)
				{
					oldObj->FreeFilter();
					oldObj->pFilter=newObj->pFilter;
					newObj->pFilter=XNULL;
				}
				if(oldObj->ratio!=newObj->ratio)
				{
					oldObj->Modify();
					oldObj->ratio=newObj->ratio;
				}
                {
					if ( moved && !oldObj->puppet)
                    {
						oldObj->Modify();
						oldObj->xform = newObj->xform;
						oldObj->control=newObj->control;
						//oldObj->ratio = newObj->ratio;
					}

					if ( button == newObj )
                    {
						FLASHASSERT(oldObj->state == buttonState);
						button = oldObj;
						//UpdateButton(obj, state);
					}
					//else if(button[1] == newObj)
					//	button[1]=oldObj;

                    remove = XFALSE;
				}
			}
		//	else if(oldObj->character->type==drawChar||oldObj->character->type==spriteEmptyChar||oldObj->char
            else if (
                oldObj->ratio == newObj->ratio &&
                (oldObj->character->type == spriteExternalChar || oldObj->character->type == spriteChar) &&
                (newObj->character->type == spriteChar || newObj->character->type == spriteExternalChar))
            {
                // we are dealing with externally loaded sprite
                // or an empty placeholder. It inherits the
                // sprite ratio of the sprite we are replacing.
                // This means that it goes away when the same
                // time the replaced sprite should have gone away

                if (!Equal(&oldObj->xform, &newObj->xform))
                {
                        if (!oldObj->puppet)
                        {
						    oldObj->Modify();
						    oldObj->xform = newObj->xform;
							oldObj->control=newObj->control;
                        }
			    }
                remove = XFALSE;
            }

            if (!remove)
            {
				DoRemove(newLink,false,true);			// delete the new obj
				*oldLink = oldObj->above;	// remove from old list
				oldObj->above = *newLink;	// place on new list
				*newLink = oldObj;
				continue;
            }

			newLink = &newObj->above;
			DoRemove(oldLink,true,false);

		} else {
			// Just look at the next new object
			FLASHASSERT(oldObj->depth > newObj->depth);
			newLink = &newObj->above;
		}
	}

/*#ifdef EDITTEXT
	// Update edit texts on hold list
	SObject* obj = holdParent->bottomChild;
	while (obj) {
		if (obj->character && obj->character->type == editTextChar) {
			if(!obj->IsCloneObject()&&!obj->IsAS3Flag())
				obj->editText->UpdateFromVariable(XTRUE);
		}
		obj = obj->above;
	}
#endif*/

    holdParent = 0;
}


//
// Button and mouse target handling
//

const U8 btnStateMap[] = {
	0,				//bsNull,		// the button is not initialized
	sbtnUpState,	//bsIdle,		// the button is idle
	sbtnOverState,	//bsOverUp,		// the mouse is up and over the button
	sbtnDownState,	//bsOverDown,	// the mouse is down and over the button
	sbtnOverState,	//bsOutDown		// the mouse is down and outside the button
};

void DisplayList::UpdateButton(SObject* obj, int newState,bool bAS3)
// Add or remove child characters so that the button matches newState
{
	SCharacter* ch = obj->character;
	if (obj->pASObject||bAS3)
	{
		if(obj->state==newState) return;
		//SRECT rect;
		//obj->GetDevBound(&rect);
		//obj->CalcButtonBounds(&rect);
		//InvalidateRect(&rect);
		if(ch&&obj->pASObject&&(ch->type==buttonChar||ch->type==buttonEmptyChar))
		{

			SObject*pObj=player->m_pAVM2->GetButtonObject(obj->pASObject,obj->state,avmshell::XAS3_BUT_REMOVE);
			if(pObj)
			{
				//pObj->FreeCache();
				AS3RemoveObject(obj,pObj);
			}
			pObj=player->m_pAVM2->GetButtonObject(obj->pASObject,newState,avmshell::XAS3_BUT_ADD);
			if(pObj)
			{
				this->AS3PlaceObject(obj,pObj,-1);
				//pObj->Modify();
			}
			obj->Modify();
		}
		//SObject*pObj=player->m_pAVM2->GetButtonObject(
		obj->state=newState;

		return;
	}
	if ( !ch||ch->type != buttonChar )
		return;

	int oldState = obj->state;
	if ( oldState == newState ) return;

	U8 * cxData = ch->button.cxformData;

	ScriptPlayer* player = ch->splayer;
	for ( int pass = 0; pass < 2; pass++ ) {
		SParser parser;
		parser.Attach(ch->data, 0);
		//int nCode=0;
		if ( ch->tagCode == stagDefineButton2 )
			parser.GetWord();	// skip the action offset

		SParser cxParser;
		if ( cxData )
			cxParser.Attach(cxData, 0);

		for (;;) {
			U8 stateFlags = parser.GetByte();
			if ( !stateFlags ) break;

			PlaceInfo info;
			info.flags = splaceCharacter|splaceMatrix|splaceColorTransform;
			U16 tag = parser.GetWord();
			info.depth = obj->depth + parser.GetWord();
			parser.GetMatrix(&info.mat);
			XFilter*pFilterList=NULL,*pNextFilter=NULL;
			if ( ch->tagCode == stagDefineButton2 )
			{
				parser.GetColorTransform(&info.cxform, XTRUE);	// new style button
				if(stateFlags&sbtnFilterList)
				{
				   XU8 num=parser.GetByte();
				   //XFilter**root=&info.pFilter;
				   for(int i=0;i<num;i++)
				   {
					   XFilter*pFilter=XFilter::CreateFilter(&parser,this->player);
					   if(!pFilter) break;
					   pFilter->m_pNext=NULL;
					   if(pNextFilter==NULL)
					   {
						   pNextFilter=pFilter;
						   pFilterList=pFilter;
					   }
					   else
					   {
						   pNextFilter->m_pNext=pFilter;
						   pNextFilter=pFilter;
					   }
					   //pFilter->pNext=XNULL;
					   //this->player->FreeFilter(pFilter);
					   //delete pFilter;
				   }
				}
				if(stateFlags&sbtnBlendMode)
				{
					parser.SkipBytes(1);
					//if(!pStream-ReadByte(blendMode)) return XFALSE;
				}
			}
			else if ( cxData )
				cxParser.GetColorTransform(&info.cxform,XFALSE);// separate cxfrom data
			else
				info.cxform.Clear();							// no cxform data

			// Remove the objects first and then add in case we have conflicting layer/id combos
			BOOL newVis = stateFlags & btnStateMap[newState];
			BOOL oldVis = stateFlags & btnStateMap[oldState];
			if ( pass == 0 && !newVis && oldVis ) {
				// Remove the child
				RemoveObject(obj, info.depth);
			}
			if ( pass == 1 && newVis && !oldVis ) {
				// Add the child
				info.character = player->FindCharacter(tag);
				info.name = _strEmpty;
				info.pFilter=pFilterList;
				info.bAdd = XTRUE;
				pFilterList=NULL;
				if ( info.character )
				{
					SObject*child=PlaceObject2(obj, &info, XNULL,true);//XTRUE);
					//if(child&&child->IsThread()&&child->thread->numFrames>1)
					//	AddThread(child->thread);
				}
			}
			if(pFilterList)
			{
				XFilter*p=pFilterList;
				while(p)
				{
					XFilter*pNext=p->m_pNext;
					//delete p;
					this->player->FreeFilter(p);
					p=pNext;
				}
				pFilterList=XNULL;
			}
		}
	}
	obj->state = newState;
	obj->Modify();
}

void DisplayList::DoButtonAction(SObject* target, int transition)
{
	SCharacter* ch = target->character;
	FLASHASSERT(ch->type == buttonChar);

//	return;
	if(target->pASObject) return;


	if ( ch->tagCode == stagDefineButton2 )
	{
		// Handle the new style button
		SParser parser;
		parser.Attach(ch->data,0);
		BOOL first = XTRUE;
		for (;;) {
			S32 link = parser.pos;
			int offset = parser.GetWord();
			if ( !first ) {
				int code = parser.GetWord();
				if ( code & (1<<transition) ) {
					//int len=offset?offset-2:parser.tagEnd-parser.pos;
					PushAction(parser.script+parser.pos, target->thread->rootObject,target->thread->rootObject->pObject,
							   XActionList::XA_BLOCK);
				}
			}
			if ( !offset ) break;
			parser.pos = link + offset;
			first = XFALSE;
		}

	} else {
		// Handle the old style button
		if ( transition == bsOverDownToOverUp ) {
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

			// Handle the action
			if(parser.pos<ch->length)
				PushAction(parser.script+parser.pos,target->thread->rootObject,target->thread->rootObject->pObject,XActionList::XA_BLOCK);
		}
	}

#ifdef SOUND

	int state = 0;
	switch ( transition )
	{
		case bsIdleToOverDown:
		case bsIdleToOverUp:
			state = sbtnOverState;
			break;

		case bsOverUpToOverDown:
			state = sbtnDownState;
			break;

		case bsOverDownToOverUp:
			state = sbtnHitTestState;
			break;

		case bsOverUpToIdle:
		case bsOutDownToIdle:
		case bsOverDownToIdle:
			state = sbtnUpState;
			break;
	}

	if ( state && ch->button.soundData )
	{
		SParser parser;
		parser.Attach(ch->button.soundData, 0);

		// Skip the sounds
		for ( int i = 1; i < state; i <<= 1 )
		{
			U16 tag = parser.GetWord();
			if ( tag )
				parser.GetSoundInfo(0);// skip the data
		}

		// Get the sound we want
		U16 tag = parser.GetWord();
		if ( tag )
		{
			SCharacter* sound = ch->splayer->FindCharacter(tag);
			if ( !sound || sound->type != soundChar )
				return;

			XSoundChannel* channel = new XSoundChannel();
			if ( channel )
			{
				channel->AddRef();
				channel->sound = &sound->sound;
				channel->tag = (U32)this;
				parser.GetSoundInfo(channel);	// set up envelope, inPoint, loops, etc...

				theSoundMix->AddSound(channel);
				channel->Release();
			}
		}
	}

#endif
}

void DisplayList::DoButtonStateTransition(SObject* newButton, int transition)
{
	//return;

	FLASHASSERT(newButton);
	//XXObjectEvent*pObject=(XXObjectEvent*)newButton->pObject;//(XXObjectButton*)newButton->pObject;
	if(newButton&&!newButton->pASObject)//&&pObject->IsObject(XXOBJ_BUTTON))
	{
		//XXObjectClip*thread=newButton->thread?(XXObjectClip*)newButton->thread->rootObject->pObject:NULL;
		//if(thread==XNULL) thread=(XXObjectClip*)newButton->character->player->rootObject->pObject;
//		XSWFPlayer*player=root.splplayer;
		switch ( transition )
		{
		case bsIdleToOverUp:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_ROLLOVER,XNULL);
			 newButton->OnEvent(0,player,XOBJEVENT_ROLLOVER,XNULL);
			 break;
		case bsOverDownToOverUp:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_RELEASE,XNULL);
			 newButton->OnEvent(0,player,XOBJEVENT_RELEASE,XNULL);
			 break;
		case bsOverUpToIdle:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_ROLLOUT,XNULL);break;
			 newButton->OnEvent(0,player,XOBJEVENT_ROLLOUT,XNULL);
		case bsOutDownToIdle:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_RELEASEOUTSIDE,XNULL);break;
			 //newButton->OnEvent(player,XOBJEVENT_RELEASEOUTSIDE,XNULL);
			 break;
		case bsOverDownToIdle:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_DRAGOUT,XNULL);break;
			 newButton->OnEvent(0,player,XOBJEVENT_DRAGOUT,XNULL);
			//buttonState = bsIdle;
			 break;
		case bsOverUpToOverDown:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_PRESS,XNULL);break;
			 newButton->OnEvent(0,player,XOBJEVENT_PRESS,XNULL);
			 break;
		case bsOutDownToOverDown:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_DRAGOVER,XNULL);break;
			 newButton->OnEvent(0,player,XOBJEVENT_DRAGOVER,XNULL);
			 break;
		case bsIdleToOverDown:
			 //buttonState = bsOverDown;
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_DRAGOVER,XNULL);break;
			 newButton->OnEvent(0,player,XOBJEVENT_DRAGOVER,XNULL);
			 break;
		case bsOverDownToOutDown:
			 //pObject->HandlerEvent(player,thread,XOBJEVENT_DRAGOUT,XNULL);break;
			 newButton->OnEvent(0,player,XOBJEVENT_DRAGOUT,XNULL);
			 //buttonState = bsOutDown;
			 break;
		//default:
		//	FLASHASSERT(XFALSE);
		}
	}


	switch ( transition ) {
		case bsIdleToOverUp:
		case bsOverDownToOverUp:
			buttonState = bsOverUp;
			break;
		case bsOverUpToIdle:
		case bsOutDownToIdle:
		case bsOverDownToIdle:
			buttonState = bsIdle;
			break;
		case bsOverUpToOverDown:
		case bsOutDownToOverDown:
		case bsIdleToOverDown:
			buttonState = bsOverDown;
			/*{
				XConfig*pConfig=player->GetConfig();
				if(pConfig->m_nAutoTag==newButton->character->tag&&
				   pConfig->m_strTagPlace.IsEmpty())
				{
					player->PostScore();
				}
			}*/
			break;
		case bsOverDownToOutDown:
			buttonState = bsOutDown;
			break;
		default:
			FLASHASSERT(XFALSE);
	}

	UpdateButton(newButton, buttonState);
	if(newButton->character&&newButton->character->type==buttonChar)
	{
		//AfxMessageBox("OK");
		DoButtonAction(newButton, transition);
	}

	if ( buttonState == bsIdle ) {
		button = 0;
		//if(!type)
		{
			buttonParent = 0;
			buttonCharacter = 0;
			buttonDepth = 0;
			buttonRatio = 0;
		}
	} else {
		button = newButton;
		//if(!type)
		{
			buttonParent = newButton->parent;
			buttonCharacter = newButton->character;
			buttonDepth = newButton->depth;
			buttonRatio = newButton->ratio;
		}
	}
}


//DEL XBOOL DisplayList::SetClip(SObject *newThread, XBOOL mouseIsDown)
//DEL {
//DEL 	//FLASHASSERT(! || newButton->character->type == buttonChar);
//DEL
//DEL 	if ( !thread ) {				// just in case the button was removed
//DEL 		threadState = bsIdle;
//DEL 		//threadParent = 0;
//DEL 		//threadCharacter = 0;
//DEL 		//threadDepth = 0;
//DEL 		//threadRatio = 0;
//DEL 	}
//DEL 	//if(newButton)
//DEL 	//	thread=XNULL;
//DEL
//DEL 	BOOL changed = XFALSE;
//DEL 	switch ( threadState ) {
//DEL 		case bsIdle: {
//DEL 			FLASHASSERT(!button);
//DEL 			if ( newButton ) {
//DEL 				if ( !mouseIsDown ) {
//DEL 					// The mouse has come over the button
//DEL 					DoButtonStateTransition(newButton, bsIdleToOverUp);
//DEL 					changed = XTRUE;
//DEL 				} else if ( newButton->character->button.trackAsMenu ) {
//DEL 					// Enter a button while the mouse is down
//DEL 					DoButtonStateTransition(newButton, bsIdleToOverDown);
//DEL 					changed = XTRUE;
//DEL 				}
//DEL 			}
//DEL 		} break;
//DEL
//DEL 		case bsOverUp: {
//DEL 			FLASHASSERT(button);
//DEL 			if ( button == newButton ) {
//DEL 				if ( mouseIsDown ) {
//DEL 					// The mouse clicked on the button
//DEL 					DoButtonStateTransition(button, bsOverUpToOverDown);
//DEL 					changed = XTRUE;
//DEL 				}
//DEL 			} else {
//DEL 				// The mouse is over a different button
//DEL 				DoButtonStateTransition(button, bsOverUpToIdle);
//DEL 				changed = XTRUE;
//DEL 			}
//DEL 		} break;
//DEL
//DEL 		case bsOverDown: {
//DEL 			FLASHASSERT(button);
//DEL 			if ( button == newButton ) {
//DEL 				if ( !mouseIsDown ) {
//DEL 					// The mouse was released over the button
//DEL 					DoButtonStateTransition(button, bsOverDownToOverUp);
//DEL 					changed = XTRUE;
//DEL 				}
//DEL 			} else {
//DEL 				if ( button->character->button.trackAsMenu ) {
//DEL 					// Exit the button
//DEL 					DoButtonStateTransition(button, bsOverDownToIdle);
//DEL 					changed = XTRUE;
//DEL 				} else {
//DEL 					// The mouse is over a different button
//DEL 					DoButtonStateTransition(button, bsOverDownToOutDown);
//DEL 					changed = XTRUE;
//DEL 				}
//DEL 			}
//DEL 		} break;
//DEL
//DEL 		case bsOutDown: {
//DEL 			FLASHASSERT(button);
//DEL 			if ( button == newButton ) {
//DEL 				// The mouse went back over the button
//DEL 				DoButtonStateTransition(button, bsOutDownToOverDown);
//DEL 				changed = XTRUE;
//DEL 			} else if ( !mouseIsDown ) {
//DEL 				// The mouse was released outside the button
//DEL 				DoButtonStateTransition(button, bsOutDownToIdle);
//DEL 				changed = XTRUE;
//DEL 			}
//DEL 		} break;
//DEL 	}
//DEL
//DEL 	// If we deactivated a button, we may need to activate a new button
//DEL 	if ( changed && !button && newButton )
//DEL 		SetButton(newButton, mouseIsDown);	// call again
//DEL
//DEL 	return changed;
//DEL }

BOOL DisplayList::SetButton(SObject* newButton, BOOL mouseIsDown)
{
	//FLASHASSERT(!newButton || newButton->character->type == buttonChar);
	//if(newButton&&newButton->character->type!=buttonChar) return XFALSE;
	//if(!newButton->character) return XFALSE;
	if ( !button ) {				// just in case the button was removed
		buttonState = bsIdle;
		//if(!type)
		{
			buttonParent = 0;
			buttonCharacter = 0;
			buttonDepth = 0;
			buttonRatio = 0;
		}
	}
	//if(newButton)
	//	thread=XNULL;

	BOOL changed = XFALSE;
	switch ( buttonState ) {
		case bsIdle: {
			FLASHASSERT(!button);
			if ( newButton ) {
				if ( !mouseIsDown ) {
					// The mouse has come over the button
					DoButtonStateTransition(newButton, bsIdleToOverUp);
					changed = XTRUE;
				} else if ( !newButton->character||newButton->character->button.trackAsMenu ) {
					// Enter a button while the mouse is down
					DoButtonStateTransition(newButton, bsIdleToOverDown);
					changed = XTRUE;
				}
			}
		} break;

		case bsOverUp: {
			FLASHASSERT(button);
			if ( button == newButton ) {
				if ( mouseIsDown ) {
					// The mouse clicked on the button
					DoButtonStateTransition(button, bsOverUpToOverDown);
					changed = XTRUE;
				}
			} else {
				// The mouse is over a different button
				DoButtonStateTransition(button, bsOverUpToIdle);
				changed = XTRUE;
			}
		} break;

		case bsOverDown: {
			FLASHASSERT(button);
			if ( button == newButton ) {
				if ( !mouseIsDown ) {
					// The mouse was released over the button
					DoButtonStateTransition(button, bsOverDownToOverUp);
					changed = XTRUE;
				}
			} else {
				if ( button->character&&
					 button->character->type==buttonChar&&
					 button->character->button.trackAsMenu ) {
					// Exit the button
					DoButtonStateTransition(button, bsOverDownToIdle);
					changed = XTRUE;
				} else {
					// The mouse is over a different button
					DoButtonStateTransition(button, bsOverDownToOutDown);
					changed = XTRUE;
				}
			}
		} break;

		case bsOutDown: {
			FLASHASSERT(button);
			if ( button == newButton ) {
				// The mouse went back over the button
				DoButtonStateTransition(button, bsOutDownToOverDown);
				changed = XTRUE;
			} else if ( !mouseIsDown ) {
				// The mouse was released outside the button
				DoButtonStateTransition(button, bsOutDownToIdle);
				changed = XTRUE;
			}
		} break;
	}

	// If we deactivated a button, we may need to activate a new button
	if ( changed && !button && newButton )
		SetButton(newButton, mouseIsDown);	// call again

	return changed;
}

/*SObject* DisplayList::HitTest(SPOINT* src)
// Find which object is hit by pt
{
	SPOINT pt = *src;
	if ( antialias_b ) {
		ToAnti(pt);
	 	//pt.x *= 4;
	 	//pt.y *= 4;
	}
	SObject* hitObj = 0;
	root.HitTest(hitObj, camera, &pt,XTRUE);

	return hitObj;
}*/
SObject* DisplayList::HitObject(SPOINT*src,_XSObject**hitObj,XU32 t)
{
	//return NULL;
	SPOINT pt = *src;
	//pt.x = pt.x * 20;
	//pt.y = pt.y * 20;
	STransform ts=camera;
	::MatrixConcat(&camera.mat,&this->mControlMat,&ts.mat);
	//::MatrixIdentity(&ts.mat);
	_XSObject*obj=root.AS3HitObject(ts,&pt,hitObj,t);
	if(obj&&!obj->IsMouseEnabled())
		obj=XNULL;
	return obj;
}

/*SObject* DisplayList::HitButton(_XSObject*&hitClip,SPOINT* src)
// Find which button is hit by pt if any
{
	SPOINT pt = *src;
	if ( antialias_b ) {
	 	//pt.x *= 4;
	 	//pt.y *= 4;
		ToAnti(pt);
	}



	if ( !RectPointIn(&devViewRect, &pt) )
		return 0;

	SObject* hitObj = 0;
	root.HitButton(hitObj, hitClip,camera, &pt);

	//this->ToPixel(*local);

	return hitObj;
}*/

// XActionList

XActionList::XActionList()
{
    maxActions = 32;
    actionMask = 31;
	actionBase = actionTop = 0;

	entries=new XActionEntry[maxActions];
	memset(entries,0,sizeof(XActionEntry)*maxActions);
//    actionObject = (SObject**) new U8[maxActions*4];
//    actionList = (U8 **) new U8[maxActions*4];
//	actionFlags= new U8[maxActions*4];
//	actionEnd  = (U8 **) new U8[maxActions*4];
}

void XActionList::FreeOne(XU32 j,XBOOL bForce)
{
	j=j&actionMask;
	if(!bForce)
	{
		if(j==(actionBase&actionMask)&&
		   (entries[j].actionFlag&XA_TOP)) return;
	}
	FREEOBJECT(entries[j].actionThis);
	if(entries[j].actionArgs)
	{
		entries[j].actionArgs->Release();
		delete entries[j].actionArgs;
	}
	switch(entries[j].actionFlag&XA_TYPEMASK)
	{
	case XA_FUNCTION:
		{
			XXObject*pObj=(XXObject*)entries[j].actionEntry;
			FREEOBJECT(pObj);
		};
	}
	entries[j].actionArgs=XNULL;
	entries[j].actionBase=XNULL;
	entries[j].actionFlag=0;
	entries[j].actionThis=XNULL;
	entries[j].actionEntry=XNULL;
}

void XActionList::FreeAll()
{
	for(U32 i=actionBase;i<actionTop;i++)
	{
		FreeOne(i,XTRUE);
		/*int j = i & actionMask;
		FREEOBJECT(entries[j].actionThis);
		if(entries[j].actionArgs)
		{
			entries[j].actionArgs->FreeAll();
			delete entries[j].actionArgs;
		}
		switch(entries[i].actionFlag&XA_TYPEMASK)
		{
		case XA_FUNCTION:
			{
				XXObject*pObj=(XXObject*)entries[i].actionEntry;
				FREEOBJECT(pObj);
			};
		}*/
	}
	//delete entries;
	//entries=XNULL;
}

XActionList::~XActionList()
{
	FreeAll();
	delete entries;
	entries=XNULL;
//    delete []actionObject;
//    delete []actionList;
//	delete actionFlags;
//	delete []actionEnd;
}

void XActionList::RemoveObject(SObject*pObject)//ScriptThread *thread)
{
	//if(pObject->character->tag==1177)
	//	int v=0;
	for ( U32 i = actionBase; i < actionTop; i++ ) {
		int j = i & actionMask;
 		if ( entries[j].actionBase == pObject ) {
			FreeOne(j,XFALSE);
		}
	}
}

// Save the action on the action list
void XActionList::PushAction(U8 * a,SObject* t,XXObject*pThis,XBOOL bTop,XXVARLIST*pArgs)
{
	U32	actionsDelta = actionTop - actionBase;

//	if(pThis&&pThis->m_pObject&&
//	   pThis->m_pObject->character->type==rootChar)
//		int v=0;
//	if(t&&t->character->tag==337)
//		int v=0;

    if (actionsDelta >= maxActions && (maxActions * 2) <= maxActionsLimit)
    {
        XActionEntry* tmp=new XActionEntry[maxActions*2];
		//U8 **	 tmpEnd = (U8 **) new U8[maxActions*4*2];

        //if (!tmp)
        //{
         //   return;
			//if(tmpEnd) delete []tmpEnd;
        //}
        //else
		if(tmp)
        {
			memset(tmp,0,maxActions*2*sizeof(XActionEntry));
			int srcMask = actionMask;
			int dstMask = (maxActions <<1) - 1;

			//memcpy(tmp+actionBase,
			for (U32 i = actionBase; i < actionTop; i++)
			{
				tmp[i&dstMask]=entries[i&srcMask];
				//tmpThread[i & dstMask] = actionObject[i & srcMask];
				//tmpList[i & dstMask] = actionList[i & srcMask];
				//tmpFlags[i& dstMask] = actionFlags[i&srcMask];
				//tmpEnd[i & dstMask ] = actionEnd[i & srcMask];
			}
			delete entries;
			entries=tmp;

            maxActions = dstMask + 1;
            actionMask = dstMask;
        }


    }

    if (actionsDelta < maxActions)
    {

		int i=actionTop&actionMask;

		if(!(bTop&XA_TOP))
		{
			i = actionTop & actionMask;
			actionTop++;
			//if(entries[i].actionFlag)
			//	int v=0;
			//actionList[i] = a;
			//actionObject[i] = t;
			//actionFlags[i]=0;
		}
		else
		{
			//
			//XU32 i=0;
			i=actionBase&actionMask;
			for(int j=actionTop;j>(int)actionBase;j--)
			{
				int iSrt=(j-1)&actionMask;
				int iDst=j&actionMask;
				if(entries[iSrt].actionFlag&XA_TOP){i=iDst; break;}

				/*entries[iDst].actionBase=entries[iSrt].actionBase;
				entries[iDst].actionEntry=entries[iSrt].actionEntry;
				entries[iDst].actionFlag=entries[iSrt].actionFlag;
				entries[iDst].actionArgs=entries[iSrt].actionArgs;
				entries[iDst].actionThis=entries[*/
				entries[iDst]=entries[iSrt];

				//entries[i]=entries[iSrt];
				//actionList[iDst]=actionList[iSrc];
				//actionObject[iDst]=actionObject[iSrc];
				//actionList[(i+1)&actionMask]=actionList[i&actionMask];
				//actionObject[(i+1)&actionMask]=actionObject[i&actionMask];
			}
			actionTop++;
		}
		entries[i].actionBase=t;
		entries[i].actionEntry=a;
		entries[i].actionThis=REFOBJECT(pThis);
		entries[i].actionFlag=bTop;
		entries[i].actionArgs=XNULL;
		if(pArgs)
		{
			entries[i].actionArgs=new XXVARLIST(pArgs->GetSize());
			int nSize=pArgs->GetSize();
			for(int j=0;j<nSize;j++)
			   entries[i].actionArgs->Push((*pArgs)[j]);
			//entries[i].actionArgs->Append(*pArgs);
			//pArgs->RemoveAll();
		}
	}
}

void DisplayList::ShutDownAlloc()
{
	XFastAlloc::XDATA* m_pData=objectAlloc.m_pData;
	/*XFastAlloc::XBLOCK*pLast=objectAlloc.m_pFreeList;
	while(pLast)
	{
		if(!pLast->pNext) break;
		pLast=pLast->pNext;
	}*/
	while(m_pData)
	{
		XFastAlloc::XBLOCK*pBlock=(XFastAlloc::XBLOCK*)(m_pData+1);
		for(int i=0;i<objectAlloc.m_nInitCount;i++)
		{
			//if(!pBlock->pNext&&pBlock!=pLast)
			{
				_XSObject*obj=(_XSObject*)(pBlock+1);
				if(obj->nCreateTime)
				//obj->FreeCache();
				//obj->pASObject=XNULL;
				//obj->pObject=XNULL;
					obj->Release();
				//obj->FreeFilter();
			}
			pBlock=(XFastAlloc::XBLOCK*)(((XU8*)pBlock)+objectAlloc.m_nBlockSize);
		}
		m_pData=m_pData->pNext;
	}
}

void DisplayList::FreeAll()
{
	root.FreeChildren();

	actionList.FreeAll();
	// Zero out the thread list
	while ( threads ) {
		threads->ReleaseDraw();
		threads->display = 0;
		threads = threads->next;
	}
}

#include "AS3DisplayObject.h"



void DisplayList::AS3ToTop(SObject*parent,SObject*obj)
{
	if(obj->above==NULL)
		return;

	CheckObject(parent);

	SObject** link = &parent->bottomChild;
	for (;;) {
		SObject* o = *link;
		if(!o)
		{
			*link=obj;
			obj->above=XNULL;
			break;
		}
		else if ( o==obj )
		{
			*link=o->above;
			//break;
		}
		else
			link = &o->above;
	}
	if(obj->depth>0)
		obj->oldDepth=obj->depth;
	obj->depth=-1;
	parent->Modify();
	CheckObject(parent);
	//CheckObject(parent);
}


void DisplayList::AS3PlaceObject(SObject*parent,SObject*obj,int nDepth)
{
	//if(nDepth<0)
		//nDepth=parent->GetMaxDepth()+1;
	//if(obj->character->tag==1762)
	//	int v=0;

	CheckObject(parent);

	if(obj->parent)
		AS3RemoveObject(obj->parent,obj);
	//nDepth+=0x4000;
	if(!obj->IsCloneObject())
		obj->oldDepth=obj->depth;
	obj->SetCloneFlag();
//	obj->nCreateTime=player->m_nRunTime;
	obj->depth=-1;
	if(obj->thread==NULL) obj->thread=parent->thread;
	if(obj->IsDisplayObject()&&obj->pASObject)
		((DisplayObjectObject*)obj->pASObject)->m_bNewObject=false;
	SObject** link = &parent->bottomChild;
	for (;;) {
		SObject* object = *link;
		if ( !object ) break;

	 	if ( !nDepth) //(obj->depth) >= nDepth )
		{
			//if(obj->depth==info->depth)
			//	int v=0;
			FLASHASSERT(object->depth != info->depth);
			break;
		}
		nDepth--;
		link = &object->above;
	}

	/*if(obj->character)
	{
		switch(obj->character->type)
		{
		case spriteChar:
		case spriteEmptyChar:
		case spriteExternalChar:
		case rootChar:
			 //this->AddThread(obj->thread);

			 break;
		default:
			 obj->thread=NULL;
		}
	}*/
	/*ScriptPlayer*player=obj->character->player;
	if(player->rootObject==obj)
	{
		player->m_pAVM2->PushObject(player->m_pLoader);
		//int v=0;
	}*/

	obj->parent = parent;
	if(!obj->thread)
		obj->thread = parent->thread;
	obj->above = *link;
	*link = obj;
	if(&root==parent||root.IsParentOf(parent))
		obj->AS3AddChild();
	obj->Modify();

	if(obj->thread&&obj->thread->m_bAS3Frame0)
	{
		//obj->thread->m_bAS3Frame0=false;
		//obj->CallFrame(obj->thread->curFrame);
	}

	CheckObject(parent);
	CacheText(obj);

	//Modify for REF
	//if(obj->thread&&obj->thread->rootObject&&obj->thread->rootObject->pASObject)
	//	player->m_pAVM2->PushObject(obj->thread->rootObject->pASObject);
	//if(obj->IsRootObject()&&obj->CreatePlayer)
	//{
		//if(strstr(obj->CreatePlayer->m_url.strTxt,"map.swf")!=NULL)
		//	int v=0;
	//	player->m_pAVM2->PushObject(obj->CreatePlayer->m_pLoaderInfo);
	//}
	//parent->Modify();

}

bool DisplayList::AS3RemoveObject(SObject*parent,SObject*obj)
{

	CheckObject(parent);

	obj->FreeCache();
	if(obj->parent!=parent)
		return false;
	
	if(obj==player->m_pUpObject||(player->m_pUpObject&&
	   obj->IsParentOf(player->m_pUpObject)))
		player->m_pUpRemovedObject=player->m_pUpObject;

	obj->AS3RemoveChild();
	obj->parent=NULL;
	//if(obj->pASObject)
	//{
	//	((DisplayObjectObject*)obj->pASObject)->m_bNewObject=true;
	//}
	//Modify for REF
	//if(obj->thread&&obj->thread->rootObject&&obj->thread->rootObject->pASObject)
	//	player->m_pAVM2->RemoveObject(obj->thread->rootObject->pASObject);
	//if(obj->IsRootObject()&&obj->CreatePlayer)
	//{
		//if(strstr(obj->CreatePlayer->m_url.strTxt,"map.swf")!=NULL)
		//	int v=0;
	//	player->m_pAVM2->RemoveObject(obj->CreatePlayer->m_pLoaderInfo);
	//}

	SObject** link = &parent->bottomChild;
	for (;;) {

		if ( !*link )
			return false;
	 	if ( obj==*link )
			break;
		link = &((*link)->above);
	}
	//obj->Modify
	//if ( obj->drawn )
	{
		SRECT rect;
		obj->GetDevBound(&rect);
		InvalidateRect(&rect);//&obj->devBounds);//&rect);
	}

	


	//obj->thread=NULL;
	if ( obj == button )
		button = 0;

	//else if(obj==button[1])
	//	button[1]=0;
	*link = obj->above;
	/*ScriptPlayer*player=obj->character->player;
	if(player->rootObject==obj)
	{
		player->m_pAVM2->RemoveObject(player->m_pLoader);
		//int v=0;
	}*/
	//CheckObject(parent);
	CheckObject(parent);

	//if(obj->IsRootObject()&&obj->pASObject)
	//	player->m_pAVM2->RemoveObject(obj->pASObject);
	
	return true;
	//parent->Modify();
	//FreeObject(obj);
}

SObject* DisplayList::AS3RemoveObject(SObject*parent,int i)
{

	CheckObject(parent);

	SObject** link = &parent->bottomChild;
	SObject* obj=*link;
	//while(i) 
	for(;;)
	{		
		if ( !obj )
			return XNULL;
		//if(obj->IsDisplayObject())
		{
			if(!i) break;
			i--;
		}
		link = &obj->above;
		obj = *link;
		//i--;
	}
	if(!obj) return obj;
	if(obj==player->m_pUpObject||(player->m_pUpObject&&
	   obj->IsParentOf(player->m_pUpObject)))
		player->m_pUpRemovedObject=player->m_pUpObject;
	//if(obj==player->m_pUpObject)
		
	//obj->AS3RemoveChild();
	obj->FreeCache();
	obj->AS3RemoveChild();
	obj->parent=NULL;
	if(obj->pASObject&&obj->IsDisplayObject())
	{
		//XXXXXXXXXXXXXXXXx
		((DisplayObjectObject*)obj->pASObject)->m_bNewObject=true;
		//obj->pASObject = (void*)0xfff0;
	}

	{
		SRECT rect;
		obj->GetDevBound(&rect);
		InvalidateRect(&rect);
	}

	if ( obj == button )
		button = 0;

	*link = obj->above;
	CheckObject(parent);

	return obj;

}

bool DisplayList::AS3SetChildIndex(SObject*parent,SObject*pObj,int i)
{


	CheckObject(parent);
	SObject** link = &parent->bottomChild;
	SObject* obj;
	int iCount=0;
	for(;;)
	{
		obj=*link;
		if(!obj) return false;
		//if(obj->IsDisplayObject())
		{
			if(obj==pObj)
			{
				if(iCount==i) return true;
				//else if(iCount<i) i--;
				*link=obj->above;
				break;
			}
			iCount++;
		}
		link=&obj->above;
	}
	link=&parent->bottomChild;
	for(;;)
	{
		obj=*link;
		//if(!obj) return false;
		//if(obj->IsDisplayObject())
		{
			if((!i/*&&obj&&obj->IsDisplayObject()*/)||!obj)
			{
				pObj->above=obj;
				*link=pObj;
				if(!pObj->IsCloneObject())
					pObj->oldDepth=pObj->depth;
				pObj->SetCloneFlag();
				pObj->depth=-1;
				//if(!obj)
					//return false;
				break;
			}
			//if(obj&&obj->IsDisplayObject())
			i--;
		}
		link=&obj->above;
	}
	CheckObject(parent);
	return true;
}

bool DisplayList::AS3SwapChild(SObject*parent,SObject*obj1,SObject*obj2)
{
	if(obj1==obj2) return true;
	if(obj1->parent!=parent||obj2->parent!=parent)
		return false;
	CheckObject(parent);
	SObject**l1=NULL,**l2=NULL,**link=&parent->bottomChild;
	SObject* obj;
	for(;;)
	{
		obj=*link;
		if(!obj) break;
		if(obj==obj1)
		{
			l1=link;
			if(l1&&l2) break;
		}
		else if(obj==obj2)
		{
			l2=link;
			if(l1&&l2) break;
		}
		link=&obj->above;
	}
	if(!l1||!l2)
		return false;
	if(obj1->above==obj2)
	{
		*l1=obj2;
		obj1->above=obj2->above;
		obj2->above=obj1;
	}
	else if(obj2->above==obj1)
	{
		*l2=obj1;
		obj2->above=obj1->above;
		obj1->above=obj2;
	}
	else
	{
		SObject*ln1=obj1->above;
		SObject*ln2=obj2->above;
		obj2->above=ln1;
		*l1=obj2;
		obj1->above=ln2;
		*l2=obj1;
	}
	int d=obj1->depth;
	if(!obj1->IsCloneObject())
		obj1->oldDepth=obj1->depth;
	if(!obj1->IsCloneObject())
		obj2->oldDepth=obj2->depth;
	obj1->SetCloneFlag();
	obj2->SetCloneFlag();

	obj1->depth=obj2->depth;
	obj2->depth=d;
	obj1->SetCloneFlag();
	obj2->SetCloneFlag();
	CheckObject(parent);
	return true;
}
bool DisplayList::AS3SwapChild(SObject*parent,int i1,int i2)
{
	if(i1==i2) return true;
	CheckObject(parent);
	SObject**l1=NULL,**l2=NULL,**link=&parent->bottomChild;
	SObject* obj,*obj1=XNULL,*obj2=XNULL;
	int iCount=0;
	for(;;)
	{
		obj=*link;
		if(!obj) break;
		if(iCount==i1)
		{
			l1=link;
			obj1=obj;
			if(l1&&l2) break;
		}
		else if(iCount==i2)
		{
			l2=link;
			obj2=obj;
			if(l1&&l2) break;
		}
		link=&obj->above;
		iCount++;
	}
	if(!l1||!l2) return false;
	if(obj1->above==obj2)
	{
		*l1=obj2;
		obj1->above=obj2->above;
		obj2->above=obj1;
	}
	else if(obj2->above==obj1)
	{
		*l2=obj1;
		obj2->above=obj1->above;
		obj1->above=obj2;
	}
	else
	{
		SObject*ln1=obj1->above;
		SObject*ln2=obj2->above;
		obj2->above=ln1;
		*l1=obj2;
		obj1->above=ln2;
		*l2=obj1;
	}
	int d=obj1->depth;
	obj1->depth=obj2->depth;
	obj2->depth=d;
	obj1->SetCloneFlag();
	obj2->SetCloneFlag();
	CheckObject(parent);
	return true;
}

void DisplayList::AS3SetStateObjects(SObject*obj,void*pInstance,int nState)
{
	SCharacter* ch = obj->character;
	if ( ch->type != buttonChar )
		return;
//	if(ch->tag==881)
//		int v=0;
	//obj->FreeChildren();
	U8 * cxData = ch->button.cxformData;

	ScriptPlayer* player = ch->splayer;
	//for ( int pass = 0; pass < 2; pass++ )
	{
		SParser parser;
		parser.Attach(ch->data, 0);
		//int nCode=0;
		if ( ch->tagCode == stagDefineButton2 )
			parser.GetWord();	// skip the action offset

		SParser cxParser;
		if ( cxData )
			cxParser.Attach(cxData, 0);

		for (;;) {
			U8 stateFlags = parser.GetByte();
			if ( !stateFlags ) break;

			PlaceInfo info;
			info.flags = splaceCharacter|splaceMatrix|splaceColorTransform;
			U16 tag = parser.GetWord();
			info.depth = obj->depth + parser.GetWord();
			parser.GetMatrix(&info.mat);
			XFilter*pFilterList=NULL,*pNextFilter=NULL;
			if ( ch->tagCode == stagDefineButton2 )
			{
				parser.GetColorTransform(&info.cxform, XTRUE);	// new style button
				if(stateFlags&sbtnFilterList)
				{
				   XU8 num=parser.GetByte();
				   //XFilter**root=&info.pFilter;
				   for(int i=0;i<num;i++)
				   {
					   XFilter*pFilter=XFilter::CreateFilter(&parser,this->player);
					   if(!pFilter) break;
					   pFilter->m_pNext=NULL;
					   //pFilter->pNext=XNULL;
					   //this->player->FreeFilter(pFilter);
					   if(pNextFilter==NULL)
					   {
						   pNextFilter=pFilter;
						   pFilterList=pFilter;
					   }
					   else
					   {
						   pNextFilter->m_pNext=pFilter;
						   pNextFilter=pFilter;
					   }
					   //delete pFilter;
				   }
				}
				if(stateFlags&sbtnBlendMode)
				{
					//parser.SkipBytes(1);
					info.blendMode=parser.GetByte();
					//if(!pStream-ReadByte(blendMode)) return XFALSE;
				}
			}
			else if ( cxData )
				cxParser.GetColorTransform(&info.cxform,XFALSE);// separate cxfrom data
			else
				info.cxform.Clear();							// no cxform data

			// Remove the objects first and then add in case we have conflicting layer/id combos
			//BOOL newVis = stateFlags & btnStateMap[newState];
			//BOOL oldVis = stateFlags & btnStateMap[oldState];
			//if ( pass == 0 && !newVis && oldVis ) {
				// Remove the child
			//	RemoveObject(obj, info.depth);
			//}
			//if((stateFlags&nState)!=0)
			//if ( pass == 1 && newVis && !oldVis )
			{
				// Add the child
				info.character = player->FindCharacter(tag);
				info.name = _strEmpty;
				info.pFilter=pFilterList;
				if ( info.character )
				{
					for(int i=0;i<4;i++)
					{
						int state=1<<i;
						if(!(stateFlags&state)) continue;
						//_XSObject*so=PlaceObject2(NULL,&info,NULL,true,obj->CreatePlayer);
						//if(so)
						//	player->m_pAVM2->SetStateObject(pInstance,so->pASObject,stateFlags);//SetButtonState(pInstance,obj->pASObject);
						SObject*parent=player->m_pAVM2->GetButtonState(pInstance,state);
						if(!parent) continue;
						PlaceObject2(parent,&info,NULL,true);
					}
					//SObject* sobj=AS3CreateObject(&info);//PlaceObject(obj, &info, XNULL,XTRUE);
					//if(sobj&&sobj->pASObject)
					//	player->m_pAVM2->SetStateObject(pInstance,sobj->pASObject,stateFlags);
				}
			}
			if(pFilterList)
			{
				XFilter*p=pFilterList;
				while(p)
				{
					XFilter*pNext=p->m_pNext;
					//delete p;
					this->player->FreeFilter(p);
					p=pNext;
				}
				pFilterList=XNULL;
			}
		}
	}
	player->m_pAVM2->FormatButtonState(pInstance);
	//((SimpleButtonObject*)pInstance)->FormatState();
}

SObject* DisplayList::AS3CreateObject(PlaceInfo*info)
{
	// Create a new object
	SObject* obj = CreateObject(info->character->splayer,player->m_nLoopCount);
	if ( !obj ) return 0;
	//obj->nCreateTime=player->m_nLoopCount;

	obj->parent = NULL;
	obj->above = NULL;//*link;
	obj->pFilter=info->pFilter;
	obj->clipObject=NULL;
	obj->pLink=XNULL;
	obj->pLinkBy=XNULL;
	obj->nForeColor=obj->nBackColor=0;
	//obj->blendMode=info->blendMode;
	//obj->cacheMode=info->cacheMode;
	obj->character = info->character;
	obj->xform = *info;
	obj->depth = info->depth;
	obj->ratio = info->ratio;
    obj->puppet = info->puppet;
	obj->drawn = XFALSE;
	obj->flags=SObject::OB_VISIABLE|SObject::OB_ENABLE;
	if(info->bClone)
		obj->flags|=SObject::OB_CLONEOBJ;
	obj->dragCenter = XFALSE;
	RectSetEmpty(&obj->devBounds);
	RectSetEmpty(&obj->dragConstraint);
	obj->dropTarget = NULL;
	obj->display = this;
	obj->bottomChild = 0;
//	obj->scrollObject = 0;
//	obj->edges = 0;
//	obj->colors = 0;
	obj->blendMode=0;
	obj->cacheMode=0;
	obj->state = 0;
	obj->pCacheMat=XNULL;
	obj->pCacheBmp=XNULL;

	obj->pObject=XNULL;
	obj->pASObject=XNULL;
	//if(bUpdate)
	obj->UpdateControl();
	obj->exflags=_XSObject::XEN_ALL;
	obj->tagType=info->character->type;
	obj->mpShapeList = XNULL;

	obj->SetInstance();
	//else
	//	int v=0;
#ifdef EDITTEXT
	obj->editText = 0;
#endif
	obj->name = info->name;//CreateStr(info->name);

	if (info->name.IsNull()) {
		/*if(info->character->className)
		{
			obj->name=info->character->className;
			//bInst=true;
		}
		else*/
		{
			XXVar var;
			var.SetString(_strInstance,32);
			//URLBuilder ub;
			//ub.AppendString("instance");
			//ub.AppendInt(++instanceNameCount);
			XXVar varc(++instanceNameCount);
			var.StringAdd(varc);
			//obj->strName=ub.buf;
			obj->name = var;//CreateStr(ub.buf);
		}
		obj->AS3SetNoName();
	}

	obj->clipDepth = info->flags & splaceDefineClip ? info->clipDepth : 0;//(info->flags & splaceDefineClip) != 0;
	obj->Modify();
	obj->thread=XNULL;
	void*pSuper=XNULL;
	bool bFrame=false;
	switch ( obj->character->type ) {

		/*case buttonChar: {
			// Set up the button state
			int state = bsIdle;
			if ( obj->depth == buttonDepth && obj->parent == buttonParent &&
				 buttonCharacter == obj->character && obj->ratio == buttonRatio ) {
				// The button is currently being tracked
				FLASHASSERT(obj->character->type == buttonChar);
				button = obj;
				state = buttonState;
			}
			obj->CreateObject(XNULL,pCnt);
			UpdateButton(obj, state);
		} break;*/

#ifdef EDITTEXT
		case textLineChar:
		case editTextChar:
			{
				//obj->editText = new EditText(obj);
				obj->editText = CreateEditText();//new EditText(obj);
				obj->editText->Init(obj);
				//obj->editText->editRect=obj->character->bounds;
				//obj->AS3CreateInstance(NULL,XTRUE);
			}
			break;
#endif
		case rootCloneChar:
		case rootChar:
			return obj;

  		case spriteChar: {


			// Create a sprite thread
			ScriptThread* thread = CreateThread();//new ScriptThread();
			thread->InitThread();
			if ( thread ) {
				thread->nEventFlags=info->nEventFlags;
				thread->pActions=info->pAction;
				thread->ClearState();
				obj->thread = thread;
				thread->player = obj->character->splayer;
				thread->splayer=this->player;
				thread->layerDepth = obj->depth;
				thread->rootObject = obj;
				thread->SetDisplay(obj->display,info->bAdd);
				thread->Attach(obj->character->data, 0);

				thread->startPos = 0;
				//if(obj->character->data)
				thread->len = obj->character->sprite.length;
				thread->numFrames = obj->character->sprite.numFrames;
				pSuper=obj->AS3CreateInst(XNULL,XTRUE);
				thread->DrawFrame(0, XFALSE,false);
				bFrame=true;
				//thread->DoEvent(CLIPLOAD,0);
				//if(info->character->tag==420)
				//	int v=0;
				//obj->AS3CreateInstance(NULL,XTRUE);

			}
		} break;
	}

	if(pSuper)
	{
		obj->AS3ConstructInst(pSuper);
		if(bFrame)
		{
			obj->thread->m_bAS3Frame0=false;
			obj->thread->m_bNeedCallFrame=false;
			obj->CallFrame(0);
		}
		//obj->AS3CreateAndConstruct(XNULL,XTRUE);

	}
		//obj->AS3CreateInstance(NULL,XTRUE);

	return obj;

}
