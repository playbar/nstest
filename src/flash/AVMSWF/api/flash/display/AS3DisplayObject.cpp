
#include "StdAfxflash.h"
#include "splayer.h"
#include "ShellCore.h"
#include "sobject.h"

//extern _XSObject* __obj;

using namespace avmplus;
#include "AS3LoaderInfo.h"
#include "AS3DisplayObject.h"
#include "AS3BlendMode.h"
#include "AS3Point.h"
#include "AS3Rectangle.h"
#include "AS3Stage.h"
#include "AS3Transform.h"
#include "AS3Event.h"
#include "AS3BitmapData.h"
#include "AS3Matrix.h"
#include "AS3ColorTransform.h"
#include "AS3Graphics.h"
#include "AS3BitmapFilter.h"
#include "AS3Loader.h"
#include "AS3DeviceBitmapData.h"
#include "XFilter.h"
#include "avm2.h"


/*void* _DebugWindow=0;

void _FilterValue(const void*container,const void*address,const void*value)
{
	if(!container) return;
	if(_DebugWindow)
	{
		if((int)value==(int)_DebugWindow)
		{
			//if((int)address<0xf000000)
			//	int v=0;
			XString16 s("Container:"),si;
			si.FromInt((int)container,true);
			s+=si;
			s+=XString16(" Address:");
			si.FromInt((int)address,true);
			s+=si;
			XSWFPlayer::m_pInstance->Trace(s);
			int v=0;
		}
		else if(*((int*)address)==(int)_DebugWindow)
		{
			XString16 s("Clear Container:"),si;
			si.FromInt((int)container,true);
			s+=si;
			s+=XString16(" Address:");
			si.FromInt((int)address,true);
			s+=si;
			XSWFPlayer::m_pInstance->Trace(s);
		}
	}
}*/

namespace avmshell{
DisplayObjectClass::DisplayObjectClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())DisplayObjectObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DisplayObjectClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DisplayObjectObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DisplayObjectObject::DisplayObjectObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	
	m_pFreePlayer = NULL;
		//int v=0;
	m_pObject=NULL;
	m_bNewObject=false;
	//m_pRotateData=NULL;
	m_pAccessibilityProp=NULL;
	m_rotationX=0;
	m_rotationY=0;
	m_rotationZ=0;
	m_scaleZ=0;
	m_pMetaData = NULL;

	//ScriptObject*pArray=topcore()->arrayClass->newArray(10);
	//pArray->setStringsetStringProperty("test",100);
	//Add your construct code here...
}



//////////////////////////////////////////////////////////
//Native Method start...
ScriptObject* DisplayObjectObject::AS3_accessibilityProperties_get()
{
	return m_pAccessibilityProp;//->atom();
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_accessibilityProperties_set(ScriptObject*pAcc)
{
	//Add your act code here...	
	m_pAccessibilityProp=(AccessibilityPropertiesObject*)pAcc;
	m_pFilters=NULL;
}

double DisplayObjectObject::AS3_alpha_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		return m_pObject->AS3GetAlpha();
	}
	return 0; 
		//kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_alpha_set(double alpha)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(MathUtils::isNaN(alpha))
		alpha=0;
		//return;
	if(m_pObject)
	{
		m_pObject->AS3SetAlpha(alpha);
	}
}

Stringp DisplayObjectObject::AS3_blendMode_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		BlendModeClass*pClass=((ShellToplevel*)toplevel())->getBlendModeClass();
		Stringp strMode=pClass->getModeNameOf(m_pObject->blendMode);//core()->kEmptyString;
		
		/*if(m_pObject->blendMode<=BlendModeClass::SUBTRACT)
		{
			Stringp* slots=pClass->getSlots();
			strMode=slots[m_pObject->blendMode];
				//pClass->getSlotADD()+m_pObject->blendMode;
		}*/
		/*switch(m_pObject->blendMode)
		{//more Safely
		case drawBlendAdd:
			 strMode=pClass->getSlotADD();break;
		case drawBlendAlpha://BlendModeClass::ALPHA:
			 strMode=pClass->getSlotALPHA();break;
		case drawBlendDarken://BlendModeClass::DARKEN:
			 strMode=pClass->getSlotDARKEN();break;
		case drawBlendDefrence://BlendModeClass::DIFFERENCE_FORSPACE:
			 strMode=pClass->getSlotDIFFERENCE_FORSPACE();break;
		case drawBlendErease://BlendModeClass::ERASE:
			 strMode=pClass->getSlotERASE();break;
		case drawBlendHardLight://BlendModeClass::HARDLIGHT:
			 strMode=pClass->getSlotHARDLIGHT();break;
		case drawBlendInvert://BlendModeClass::INVERT:
			 strMode=pClass->getSlotINVERT();break;
		case drawBlendLayer://BlendModeClass::LAYER:
			 strMode=pClass->getSlotLAYER();break;
		case drawBlendLighten://BlendModeClass::LIGHTEN:
			 strMode=pClass->getSlotLIGHTEN();break;
		case drawBlendMulty://BlendModeClass::MULTIPLY:
			 strMode=pClass->getSlotMULTIPLY();break;
		case drawBlendNormal1:
		case drawBlendNormal0://BlendModeClass::NORMAL:
			 strMode=pClass->getSlotNORMAL();break;
		case drawBlendOverlay://BlendModeClass::OVERLAY:
			 strMode=pClass->getSlotOVERLAY();break;
		case drawBlendScreen://BlendModeClass::SCREEN:
			 strMode=pClass->getSlotSCREEN();break;
		case drawBlendSubtrace://BlendModeClass::SUBTRACT:
			 strMode=pClass->getSlotSUBTRACT();break;
			 break;
		}*/
		return strMode;
	}
	return core()->kEmptyString; 
		//kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_blendMode_set(Stringp strMode)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//return;
	if(m_pObject)
	{
		BlendModeClass*pClass=((ShellToplevel*)toplevel())->getBlendModeClass();
	
		m_pObject->AS3SetBlendMode(pClass->getModeIDOf(strMode));
		//m_pObject->Modify();
		/*int nMode=strMode-slots[0];
		if(nMode>=0&&nMode<=BlendModeClass::SUBTRACT)
		{

		}*/
	}
//return kAvmThunkUndefined;//Modify this please!
}

bool DisplayObjectObject::AS3_cacheAsBitmap_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->cacheMode!=0;
	return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_cacheAsBitmap_set(bool bCache)
{
	//Add your act code here...	
	if(m_pObject)
		m_pObject->cacheMode=bCache;
	if(!bCache)
		m_pObject->FreeCacheBmp();
}
ArrayObject* DisplayObjectObject::AS3_filters_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pFilters==XNULL)
	{
		m_pFilters=toplevel()->arrayClass->newArray(0);
		BitmapFilterClass*pClass=((ShellToplevel*)toplevel())->getBitmapFilterClass();
		if(m_pObject&&m_pObject->pFilter)
		{
			_XFilter*pFilter=m_pObject->pFilter;
			while(pFilter)
			{
				BitmapFilterObject*pf=pClass->CreateFilter(pFilter);
				Atom atom=pf->atom();
				m_pFilters->push(&atom,1);
				pFilter=pFilter->m_pNext;
			}
		}
	}
	return m_pFilters;//->atom();
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_filters_set(ArrayObject*pFilters)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//return;
	m_pFilters=pFilters;

	if(m_pObject)
	{
		//if(m_pObject->xform.mat.ty==260)
		//	int vv=0;
		m_pObject->FreeFilter();
		if(pFilters)
		{
			int i,l=pFilters->getDenseLength();//get_length();
			_XFilter*pRoot=NULL;
			for(i=0;i<l;i++)
			{
				BitmapFilterObject*pObj=(BitmapFilterObject*)AvmCore::atomToScriptObject(pFilters->getIntProperty(i));
				if(pObj==NULL) continue;
				_XFilter*pFilter=pObj->CreateFilter();
				if(pFilter)
				{					
					pFilter->m_pNext=NULL;
					if(pRoot==NULL)
					{
						pRoot=pFilter;
						m_pObject->pFilter=pRoot;
					}
					else
					{
						pRoot->m_pNext=pFilter;
					}
				}
			}
//#if (__CORE_VERSION__>=0x02076000)
			m_pObject->Modify();
//#endif
		}
//#if (__CORE_VERSION__>=0x02079000)
		else 
			m_pObject->Modify();
//#endif
	}
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

double DisplayObjectObject::AS3_width_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
//#ifdef _WINEMU
//	StUTF8String ss(vtable->traits->name());
//	if(strcmp(ss.c_str(),"HeroSoulChangeItemCard")==0)
//	{
//		//return 102;
//		int vv=0;
//	}
//#endif
	if(m_pObject)
	{
		//if(m_pObject->character&&m_pObject->character->tag==141)
		//	int v=0;
		return m_pObject->GetWidth();
	}
	else return 0;
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_width_set(double w)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//StUTF8String s(vtable->traits->name());
	//if(strcmp(s.c_str(),"ToolTip")==0)
	//	int vv=0;
//#if (__CORE_VERSION__>=0x02076000)
//	if(w==295)
	//	int vv=0;
	if(w<0) return;
//#endif
	if(MathUtils::isNaN(w))
		return;
	if(m_pObject)
	{
		//if(m_pObject->character->tag==22)
		//	int v=0;
		//if(w<0) return;
		m_pObject->SetWidth(w);
	}
	//return kAvmThunkUndefined;//Modify this please!
}


double DisplayObjectObject::AS3_height_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
#ifdef _WINEMU
	/*StUTF8String ss(vtable->traits->name());
	if(strcmp(ss.c_str(),"XInfoView")==0)
	{
		
		int vv=0;
	}*/
#endif
	if(m_pObject)
		return m_pObject->GetHeight();
	return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_height_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(MathUtils::isNaN(v))
		return;
//#if (__CORE_VERSION__>=0x02075000)
	if(v<0) return;
//#endif
	if(m_pObject)
	{
		//if(v<0) 
		//	return;
		m_pObject->SetHeight(v);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

LoaderInfoObject* DisplayObjectObject::AS3_loaderInfo_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...

	//ShellCodeContext*codeContext=(ShellCodeContext*)core()->codeContext();
	//if(codeContext!=m_codeContext)
	//{
	//	int v=0;
	//}

	if(m_pObject)//&&m_pObject->rootPlayer)
	{
		_XSObject*obj=m_pObject;
		bool bInDisplay=true;
		while(obj)
		{			
			if(!obj->character)
				return obj->CreatePlayer->m_pLoaderInfo;
			//	return AS3_stage_get();		
			if(obj->IsRootObject())
				return obj->character->splayer->m_pLoaderInfo;
				//return (DisplayObjectObject*)obj->pASObject;
			obj=obj->parent;
		}
	}
	//else 
	if(m_codeContext)
	{
		//_XSObject*root=m_codeContext->m_pRoot;//->rootObject;
		//if(root)
		//	return root->pASObject;
		return m_codeContext->m_pRoot->m_pLoaderInfo;
	}
	return NULL;
	//return m_pLoadInfo;
	//ShellCodeContext*pContext=(ShellCodeContext*)core()->codeContext();
	//return pContext->
	/*if(m_pCodeContext)//m_pObject)
	{
		//if(m_pObject->character)
		//	LoaderInfoObject*pObj=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
		//else
		//{

		//}
		if(!m_pCodeContext->m_pRoot)
			return ((ShellCore*)core())->GetPlayer()->player->m_pLoaderInfo;
		else
			return m_pCodeContext->m_pRoot->m_pLoaderInfo;//m_pLoaderInfo;//m_pLoader->m_pLoaderInfo;//m_pLoaderInfo;
		//return pObj;
	}
	return NULL;//Undefined;*/
	//return m_pLoaderInfo;
	/*ShellToplevel*t=(ShellToplevel*)toplevel();
	LoaderInfoClass*pClass=t->getLoaderInfoClass();

	Atom atom=pClass->atom();//{pClass->atom(),atom()};
	LoaderInfoObject*pInfo=(LoaderInfoObject*)core()->atomToScriptObject(pClass->construct(0,&atom));
	//pInfo->m_pEventObject=this;
	//pInfo->m_pEventObject=NULL;
	//pInfo->
	return pInfo->atom();
	//loaderInfo
	//return kAvmThunkUndefined;//Modify this please!*/
}

ScriptObject* DisplayObjectObject::AS3_mask_get()
{
	//Add your act code here...	
	if(m_pObject)
	{
		SObject*pMask=m_pObject->AS3GetMask();
		if(pMask&&pMask->pASObject)
		{
			ScriptObject*p=(ScriptObject*)pMask->pASObject;
			m_pMask=(DisplayObjectObject*)p;
			return p;
		}
	}
	return NULL;//Modify this please!
}

void DisplayObjectObject::AS3_mask_set(DisplayObjectObject*pObj)
{
	//Add your act code here...
	m_pMask=pObj;
	if(m_pObject)
	{
		if(pObj&&pObj->m_pObject)
		{
			//if(pObj->m_pObject->character->type==19)
			//	int v=0;			
			m_pObject->SetMask(pObj->m_pObject);
		}
		else
			m_pObject->SetMask(NULL);
//#if (__CORE_VERSION__>=0x02077000)
		m_pObject->Modify();
//#endif
	}
	
	//return kAvmThunkUndefined;//Modify this please!
}

double DisplayObjectObject::AS3_mouseX_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	if(m_pObject)
	{
		//((ShellCore*)core())->
		XSWFPlayer*pl=((ShellCore*)core())->GetPlayer();
		//ShellCodeContext*pContext=(ShellCodeContext*)core()->codeContext();		
		double x,y;
		pl->GetMousePos(m_pObject,x,y);
		return x;
	}
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

double DisplayObjectObject::AS3_mouseY_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{

		//((ShellCore*)core())->
		//ScriptPlayer*pl=((ShellCore*)core())->GetPlayer();
		XSWFPlayer*pl=((ShellCore*)core())->GetPlayer();
		double x,y;
		pl->GetMousePos(m_pObject,x,y);
		return y;
	}
	return kAvmThunkUndefined;//Modify this please!
}

Stringp DisplayObjectObject::AS3_name_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	if(m_pObject)
	{
		Stringp str=core()->internStringUTF8(m_pObject->name.strTxt);
		return str;
	}
	return core()->kEmptyString;//->atom();
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_name_set(Stringp strName)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		StUTF8String s(strName);
		m_pObject->name=s.c_str();
		m_pObject->name.ToString(XTRUE);
		//m_pObject->SetName(strName);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

AvmBox DisplayObjectObject::AS3_opaqueBackground_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{	//atomKind
		//double d=(m_pObject->nBackColor)&0xffff;
		//return d;
		XU32 nColor=m_pObject->nBackColor;
		if(!nColor) return nColor;
		return core()->uintToAtom(nColor&0xffffff);
		//return m_pObject->nBackColor&0xffffff;
	}
	return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_opaqueBackground_set(Atom pObject)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	if(m_pObject)
	{
		//atomKind(
		Atom n=core()->numberAtom(pObject);
		XU32 nColor=(XU32)core()->number(n);
		if(nColor) nColor|=0xff000000;
		if(nColor!=m_pObject->nBackColor)
		{
			m_pObject->SetBackColor(nColor);//nBkColor=nColor;
			//m_pObject->Modify();
		}
		//core()->atomToDouble(pObject);
	}
	//Add your act code here...
	//core()->atomToDouble(
	//return kAvmThunkUndefined;//Modify this please!
}

DisplayObjectObject* DisplayObjectObject::AS3_parent_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	/*if(m_pObject&&m_pObject->parent)
	{
		ScriptObject*pObject=(ScriptObject*)m_pObject->parent->pASObject;
		if(pObject)
			return pObject;
	}*/
	DisplayObjectObject*pObj=(DisplayObjectObject*)this->GetParentObject(false);
	//if(pObj==NULL)
	//	int v=0;
	//if(pObj==NULL&&m_pObject)
	//{
	//	if(m_pObject->character->tag==268)
	//		pObj=this->GetParentObject(false);
	//}
	return pObj;
	//return NULL;//kAvmThunkUndefined;//Modify this please!
}

DisplayObjectObject* DisplayObjectObject::AS3_root_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//ScriptPlayer*p=((ShellCore*)core())->GetRootPlayer();
	//ShellCodeContext*pContext=(ShellCodeContext*)core()->codeContext();
	/*ScriptPlayer*p=XNULL;
	if(m_pObject)
		p=m_pObject->character->player;
	if(!p)
		p=((ShellCore*)core())->GetScriptPlayer();
		//pContext->m_pRoot;
	if(p&&p->rootObject->pASObject)
	{
		ScriptObject*pObj=(ScriptObject*)p->rootObject->pASObject;
		return pObj;
	}*/
	if(m_pObject)//&&m_pObject->rootPlayer)
	{
		_XSObject*obj=m_pObject;
		bool bInDisplay=true;
		while(obj)
		{			
			if(!obj->character)
				return AS3_stage_get();		
			if(obj->IsRootObject())
				return (DisplayObjectObject*)obj->pASObject;
			obj=obj->parent;
		}
		//if(m_pObject->thread&&m_pObject->thread->player)
		//	return (DisplayObjectObject*)m_pObject->thread->player->rootObject->pASObject;
		//return (DisplayObjectObject*)m_pObject->rootPlayer->rootObject->pASObject;
		/*_XSObject*parent=m_pObject->parent;
		ScriptPlayer*p=NULL;
		if(!parent||parent==&m_pObject->display->root)
		{
			if(m_pObject->character)
				p=m_pObject->character->player;
		}
		else if(parent->character)
			p=parent->character->player;
		else if(m_pObject->character)
			p=m_pObject->character->player;
		if(p) return (ScriptObject*)p->rootObject->pASObject;*/
		/*ScriptPlayer*p=NULL;
		if(m_pObject->player)//character)
			p=m_pObject->player;//character->player;
		else
			return this->AS3_stage_get();
		//else p=((ShellCore*)core())->GetPlayer()->player;
		if(p) return (DisplayObjectObject*)p->rootObject->pASObject;*/

	}
//#if (__CORE_VERSION__>=0x02070100)
	else 
//#endif
	if(m_codeContext)
	{
		return (DisplayObjectObject*)m_codeContext->m_pRoot->rootObject->pASObject;
	}
	return NULL;//kAvmThunkUndefined;//Modify this please!
}

double DisplayObjectObject::AS3_rotation_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		return m_pObject->GetRotate();
	}
	return 0;
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_rotation_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(MathUtils::isNaN(v))
		v=0;
	if(m_pObject)
	{
		while(v>180) v-=360;
		while(v<-180) v+=360;
		return m_pObject->SetRotate(v);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

RectangleObject* DisplayObjectObject::AS3_scale9Grid_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(!m_scale9grid&&m_pObject)
	{
		SRECT*r=m_pObject->GetGridRect();
		if(r)
		{
			RectangleClass*pClass=((ShellToplevel*)toplevel())->getRectangleClass();
			m_scale9grid=pClass->CreateRectangle(r->xmin,r->ymin,(r->xmax-r->xmin),(r->ymax-r->ymin));
		}
	}
	return m_scale9grid;//AS3_getBounds(NULL);
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_scale9Grid_set(RectangleObject*pRect)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!

	if(m_pObject)
	{
		if(!pRect)
			m_pObject->SetGridRect(NULL);
		else
		{
			SRECT rect;
			AS3RECTANGLEDATA&r=pRect->GetData();
			rect.xmin=(int)(r.left);
			rect.ymin=(int)(r.top);
			rect.xmax=(int)((r.left+r.width));
			rect.ymax=(int)((r.top+r.height));
			m_pObject->SetGridRect(&rect);
		}
	}

	m_scale9grid=pRect;
}

double DisplayObjectObject::AS3_scaleX_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->GetScaleX()/100;
	return 1;
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_scaleX_set(double s)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(MathUtils::isNaN(s))
		s=0;
	if(m_pObject)
	{
		m_pObject->SetScaleX(s*100);
//#ifdef _WINEMU
//		if(m_pObject->xform.mat.a<0)
//			int vv=0;
//#endif
	}
	//return kAvmThunkUndefined;//Modify this please!
}

double DisplayObjectObject::AS3_scaleY_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->GetScaleY()/100;
	return 1;
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_scaleY_set(double s)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...


	if(MathUtils::isNaN(s))
		s=0;
	if(m_pObject)
	{

		m_pObject->SetScaleY(s*100);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

RectangleObject* DisplayObjectObject::AS3_scrollRect_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{

		SRECT* rect=m_pObject->AS3GetScrollRect();
		if(!rect) return NULL;
#ifdef _WINEMU
		//if(rect->xmin>20000)
		//	int v=0;
#endif
		//m_pObject->AS3GetScrollRect(rect);
		ShellToplevel* t=(ShellToplevel*)toplevel();
		return t->getRectangleClass()->CreateRectangle(
			rect->xmin/20.0,
			rect->ymin/20.0,
			(rect->xmax-rect->xmin)/20.0,
			(rect->ymax-rect->ymin)/20.0);
	}
	return NULL;//Modify this please!
}

void DisplayObjectObject::AS3_scrollRect_set(RectangleObject*pObj)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		if(pObj)
		{
			SRECT rect;
			AS3RECTANGLEDATA&r=pObj->GetData();
#ifdef _WINEMU
			//if(r.left>20000)
			//	int v=0;
#endif
			rect.xmin=(int)(r.left*20);
			rect.ymin=(int)(r.top*20);
			rect.xmax=(int)((r.left+r.width)*20);
			rect.ymax=(int)((r.top+r.height)*20);
			m_pObject->AS3SetScrollRect(&rect);
		}
		else
			m_pObject->AS3SetScrollRect(NULL);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

StageObject* DisplayObjectObject::AS3_stage_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	
	ShellCore*pCore=(ShellCore*)core();
	XSWFPlayer*player=pCore->GetPlayer();
	if(!m_pObject) 
		return NULL;
	//if(!m_pObject->parent&&!m_pObject->IsRootObject())
	//	return NULL;
	//if(!m_pObject->IsInDisplay())//
	/*_XSObject*obj=m_pObject;
	bool bInDisplay=false;
	while(obj)
	{
		if(obj->IsRootObject())
		{
			bInDisplay=true;
			break;
		}
		obj=obj->parent;
	}
	if(!bInDisplay) return NULL;*/
	
	//if(m_pObject->IsRootObject())
	//	int v=0;

	//TDXY need return a not null pointer,but is not ...I don't konw reason

	if(m_pObject->thread&&!m_pObject->thread->m_bInDisplay)
	{	//return NULL;
		_XSObject*obj=m_pObject->parent;
		bool bInDisplay=false;
		while(obj)
		{			
			if(!obj->character)
				return NULL;
				//return AS3_stage_get();		
			if(obj->IsRootObject())
				//return (DisplayObjectObject*)obj->pASObject;
			{
				bInDisplay=true;
				break;
			}
			obj=obj->parent;
		}
		if(!bInDisplay) 
			return NULL;
	}
//	if(!player->display.root.IsParentOf(m_pObject)&&m_pObject!=&player->display.root)
//		return NULL;

	StageObject*pStage=(StageObject*)player->display.root.pASObject;
	if(pStage==XNULL)
	{
		ShellToplevel*t=(ShellToplevel*)toplevel();
		StageObject*p=t->getStageClass()->CreateStage(XNULL);
		//p->IncrementRef();
		((ShellToplevel*)toplevel())->PushObject(p);
		if(player->display.root.pASObject==NULL)
		{
			player->display.root.pASObject=p;
			((ShellToplevel*)toplevel())->PushObject(p);
		}
		player->display.root.pASObject=pStage=p;
	}
	return pStage;

	//ShellToplevel*t=(ShellToplevel*)toplevel();
	//ShellCore*pCore=(ShellCore*)core();
	//return t->getStageClass()->CreateStage(this);
	//return kAvmThunkUndefined;//Modify this please!
}

TransformObject* DisplayObjectObject::AS3_transform_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	ShellToplevel*t=(ShellToplevel*)toplevel();
	return (TransformObject*)AvmCore::atomToScriptObject(t->getTransformClass()->CreateTransform(this));
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_transform_set(TransformObject*pTrans)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		DisplayObjectObject*ps=pTrans->GetDisplayObject();
		if(ps)
		{
			MATRIX mat;
			ps->GetMatrix(mat);
			SetMatrix(mat);
			XCXForm form;
			form.Clear();
			ps->GetCXForm(form);
			SetCXForm(form);
		}
	}
	//return kAvmThunkUndefined;//Modify this please!
}

bool DisplayObjectObject::AS3_visible_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsVisible()!=0;
	return false;
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_visible_set(bool b)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	/*if(b&&1)
	{
		StUTF8String s(vtable->traits->name());
		if(strcmp(s.c_str(),"Dlg_Loading")==0)
			int v=0;
	}*/
	/*if(b)
	{
		StUTF8String s(this->vtable->traits->name());
		if(strcmp(s.c_str(),"ToolTip")==0)
			int v=0;
	}*/

	if(m_pObject)
	{

		/*if(m_pObject->name.strTxt&&strcmp(m_pObject->name.strTxt,"btnMC")==0)
		{
			XString8 str;
			str.ConvertFrom("聂",XCHARSET_GB2312);
			if(m_pObject->parent&&m_pObject->parent->name.strTxt)
			{
				if(strstr(m_pObject->parent->name.strTxt,str.GetData())!=NULL)
					int v=0;
			}
		}*/
		//StUTF8String s(vtable->traits->name());
		//if(strcmp(s.c_str(),"TipsView")==0)
		//	int v=0;

		m_pObject->SetVisible(b);
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(b&&player->m_nLockOpt)
		{
			player->AddOptObject(m_pObject);
		}
		//m_pObject->parent->Modify();
	}
	//return kAvmThunkUndefined;//Modify this please!
}


double DisplayObjectObject::AS3_x_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		//double v=m_pObject->GetPosX();
		//if(v<-10000)
		//	int v=0;
		//if(m_pObject->character->tag==7)
		//	int v=0;
		return m_pObject->GetPosX();
	}
	return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_x_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	//v=0;
/*#ifdef _WINEMU
	StUTF8String ss(vtable->traits->name());
	if(strcmp(ss.c_str(),"ItemNameRenderer")==0)
	{
		
		int vv=0;
	}
#endif*/

	if(MathUtils::isNaN(v))
		v=0;
	if(m_pObject)
	{
		m_pObject->SetPosX(v);
	}
//return kAvmThunkUndefined;//Modify this please!
}

double DisplayObjectObject::AS3_y_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	
	{		
		//if(m_pObject->character->tag==1287)
		//	int v=0;
		return m_pObject->GetPosY();
	}
	return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_y_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
#ifdef _WINEMU
	/*StUTF8String ss(vtable->traits->name());
	if(strstr(ss.c_str(),"FreeGuideArrow")!=NULL)
	{
		
		int vv=0;
	}*/
#endif
/*#ifdef _WINEMU
	StUTF8String ss(vtable->traits->name());
	if(strcmp(ss.c_str(),"ToggleButton")==0)
	{
		if(m_pObject&&m_pObject->parent)
		{
			int iCount=0;
			_XSObject*child=m_pObject->parent->bottomChild;
			while(child)
			{
				iCount++;
				child=child->above;
			}
			if(iCount==3)
				int vv=0;
		}
	}
#endif*/

	if(m_pObject)
	if(MathUtils::isNaN(v))
		v=0;
	if(m_pObject)
	{
		
		/*if(m_pObject->name.strTxt)
		{
			//StUTF8String s(vtable->traits->name());
			if(strcmp(m_pObject->name.strTxt,"hb")==0)
				int v=0;
		}*/
		//if(v>1000)
		//	int vv=0;
		m_pObject->SetPosY(v);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

double DisplayObjectObject::AS3_z_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{	
		if(!m_pObject->parent) return 0;
		int iSet=0,iMax=0;
		bool bSet=false;
		_XSObject*child=m_pObject->parent->bottomChild;
		while(child)
		{
			if(child==m_pObject)
				bSet=true;
			iMax++;
			if(!bSet)
				iSet++;
			child=child->above;
		}
		return iMax-iSet;
		//if(m_pObject->character->tag==1287)
		//	int v=0;
		//return 0;//m_pObject->GetPosY();
	}
	return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::AS3_z_set(double v)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(MathUtils::isNaN(v))
		return;
	if(m_pObject)
	{

		if(!m_pObject->parent) return;

		int iSet=0,iMax=0;
		bool bSet=false;
		_XSObject*child=m_pObject->parent->bottomChild;
		while(child)
		{
			if(child==m_pObject)
				bSet=true;
			iMax++;
			if(!bSet)
				iSet++;
			child=child->above;
		}
		
		_XSObject*parent=m_pObject->parent;
		parent->display->AS3SetChildIndex(parent,m_pObject,iMax-v);
		//m_pObject->SetPosY(v);
	}
	//return kAvmThunkUndefined;//Modify this please!
}

RectangleObject* DisplayObjectObject::AS3_getBounds(DisplayObjectObject*pClip)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	SRECT rect;
	if(m_pObject)
	{
		//if(m_pObject->character&&m_pObject->character->tag==86)
		//	int v=0;
		MATRIX*pMatrix=XNULL;
		MATRIX mat;
		if(pClip&&pClip->m_pObject)//&&pClip->m_pObject!=m_pObject->parent)//m_pDisp)
		{
			::MatrixIdentity(&mat);
			SObject*p=m_pObject;//->parent;//->parent;
			while(p)
			{
				if(p==pClip->m_pObject||p->character==XNULL||
					p->character->type==rootChar) 
				{
					//if(p->parent)
					if(p!=pClip->m_pObject)
						::MatrixConcat(&mat,&p->xform.mat,&mat);
					break;
				}
				::MatrixConcat(&mat,&p->xform.mat,&mat);
				p=p->parent;
			}
			//::MatrixConcat(&mat,&m_pRoot->display->camera.mat,&mat);
			//pClip->m_pObject->GetTotalMatrix(mat,XFALSE);
			//::MatrixInvert(&mat,&mat);
			pMatrix=&mat;
		}	
		m_pObject->GetBoundBox(rect,pMatrix,false);
	}
	else
	{
		::RectSetEmpty(&rect);
	}

	ShellToplevel*t=(ShellToplevel*)toplevel();
	if(RectIsEmpty(&rect))
	{
		rect.xmin=0;
		rect.xmax=0;
		rect.ymin=0;
		rect.ymax=0;
	}
		//int v=0;
	return (RectangleObject*)AvmCore::atomToScriptObject(t->getRectangleClass()->CreateRectangle(
		rect.xmin/20.0,
		rect.ymin/20.0,
		(rect.xmax-rect.xmin)/20.0,
		(rect.ymax-rect.ymin)/20.0)->atom());
	//RectangleObject*pRt=(RectangleObject*)t->getRectangleClass()->CreateRectangle(rect);

	//return pRt->atom();

		//core()->atomToScriptObject();
	//return kAvmThunkUndefined;//Modify this please!	
	//return AS3_getRect(pClip);
	//return kAvmThunkUndefined;//Modify this please!
}

RectangleObject* DisplayObjectObject::AS3_getRect(DisplayObjectObject*pClip)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	SRECT rect;
	if(m_pObject)
	{
		//if(m_pObject->character&&m_pObject->character->tag==86)
		//	int v=0;
		MATRIX*pMatrix=XNULL;
		MATRIX mat;
		if(pClip&&pClip->m_pObject)//&&pClip->m_pObject!=m_pObject->parent)//m_pDisp)
		{
			::MatrixIdentity(&mat);
			SObject*p=m_pObject;//->parent;//->parent;
			while(p)
			{
				if(p==pClip->m_pObject||p->character==XNULL||
					p->character->type==rootChar) 
				{
					//if(p->parent)
					if(p!=pClip->m_pObject)
						::MatrixConcat(&mat,&p->xform.mat,&mat);
					break;
				}
				::MatrixConcat(&mat,&p->xform.mat,&mat);
				
				//if(p->character==XNULL||p->character->type==rootChar) break;
				p=p->parent;
			}
			//::MatrixConcat(&mat,&m_pRoot->display->camera.mat,&mat);
			//pClip->m_pObject->GetTotalMatrix(mat,XFALSE);
			//::MatrixInvert(&mat,&mat);
			pMatrix=&mat;
		}
		
	
		m_pObject->GetBoundRect(rect,pMatrix,false);
	}
	else
	{
		::RectSetEmpty(&rect);
	}


	ShellToplevel*t=(ShellToplevel*)toplevel();
//#if (__CORE_VERSION__>=0x02077000)
	if(RectIsEmpty(&rect))
	{
		rect.xmin=0;
		rect.xmax=0;
		rect.ymin=0;
		rect.ymax=0;
	}
		//int v=0;
//#endif
	return (RectangleObject*)AvmCore::atomToScriptObject(t->getRectangleClass()->CreateRectangle(
		rect.xmin/20.0,
		rect.ymin/20.0,
		(rect.xmax-rect.xmin)/20.0,
		(rect.ymax-rect.ymin)/20.0)->atom());
	//RectangleObject*pRt=(RectangleObject*)t->getRectangleClass()->CreateRectangle(rect);

	//return pRt->atom();

		//core()->atomToScriptObject();
	//return kAvmThunkUndefined;//Modify this please!
}

PointObject* DisplayObjectObject::AS3_globalToLocal(PointObject*pt)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	PointObject*ptNew=pt->clone();

	if(m_pObject)
	{
		AS3POINTDATA&pt=ptNew->GetData();
		m_pObject->GlobalToLocal(pt.x,pt.y);
		ptNew->Modify();
		//ptNew->Modify();
		
	}
	return ptNew;
	//return kAvmThunkUndefined;//Modify this please!
}

bool DisplayObjectObject::AS3_hitTestObject(DisplayObjectObject*pObj)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject&&pObj->m_pObject)
	{
		return m_pObject->HitObject(pObj->m_pObject)!=0;
	}
	else
	{
		return false;
	}
	//return kAvmThunkUndefined;//Modify this please!
}

bool DisplayObjectObject::AS3_hitTestPoint(double x,double y,bool b)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pObject)
	{
		if(this->m_bNewObject||!m_pObject->parent) return false;
		bool ret=m_pObject->AS3HitTest(x,y,b);
		//if(!ret)
		//{
		//	ret=m_pObject->AS3HitTest(x,y,b);
		//}

		//if(!ret)
		//	ret=m_pObject->AS3HitTest(x,y,b);

		return ret;
			//HitTest(x,y,b)!=0;
	}
	else
		return false;
	//return kAvmThunkUndefined;//Modify this please!
}

PointObject* DisplayObjectObject::AS3_localToGlobal(PointObject*pt)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	PointObject*ptNew=pt->clone();
	if(m_pObject)
	{
		AS3POINTDATA&data=ptNew->GetData();
		m_pObject->LocalToGlobal(data.x,data.y);
		ptNew->Modify();
	}
	return ptNew;
	//return kAvmThunkUndefined;//Modify this please!
}

void DisplayObjectObject::GetMatrix(MATRIX&m)
{
	if(m_pObject)
		m_pObject->GetMatrix(m);
}
void DisplayObjectObject::SetMatrix(MATRIX&m)
{
	if(m_pObject)
		m_pObject->SetMatrix(m);
}

void DisplayObjectObject::GetCXForm(XCXForm&m)
{
	if(m_pObject)
		m_pObject->GetCXForm(m);
}
void DisplayObjectObject::SetCXForm(XCXForm&m)
{
	if(m_pObject)
		m_pObject->SetCXForm(m);
}

/*int DisplayObjectObject::MapSystemEvent(Stringp strName,int&nMax)
{
	nMax=XAS3_EVENT_MAX;
	EventClass*pClass=(EventClass*)(toplevel()->getBuiltinExtensionClass(abcclass_flash_events_Event));
	// added  将显示对象添加到显示列表中时调度。 DisplayObject 
    //addedToStage  在将显示对象直接添加到舞台显示列表或将包含显示对象的子树添加至舞台显示列表中时调度。 DisplayObject 
    //deactivate  Flash Player 失去操作系统焦点并变为非活动状态时调度。 EventDispatcher 
    //enterFrame  播放头进入新帧时调度。 DisplayObject 
    //removed  将要从显示列表中删除显示对象时调度。 DisplayObject 
    //removedFromStage  在从显示列表中直接删除显示对象或删除包含显示对象的子树时调度。 DisplayObject 
    //render 

	if(strName==pClass->getSlotADDED())
		return XAS3_EVENT_added;
	else if(strName==pClass->getSlotADDED_TO_STAGE())
		return XAS3_EVENT_addedToStage;
	else if(strName==pClass->getSlotENTER_FRAME())
		return XAS3_EVENT_enterFrame;
	else if(strName==pClass->getSlotREMOVED())
		return XAS3_EVENT_removed;
	else if(strName==pClass->getSlotREMOVED_FROM_STAGE())
		return XAS3_EVENT_removedFromStage;
	else if(strName==pClass->getSlotRENDER())
		return XAS3_EVENT_render;
	return EventDispatcherObject::MapSystemEvent(strName,nMax);
	//return idd;
}*/

void DisplayObjectObject::AS3_constructor()
{
/*#ifdef _WINEMU
	StUTF8String ss(vtable->traits->name());
	if(strcmp(ss.c_str(),"ItemNameRenderer")==0)
	{
		
		int vv=0;
	}
#endif*/
	//if(m_pLoadInfo==NULL&&m_pCodeContext!=NULL)
	{
		//m_pLoadInfo=m_pCodeContext->m_pLoaderInfo;
		//ShellCodeContext*p=(ShellCodeContext*)core()->codeContext();
		//m_pObject=
	}
	
	//else type=m_pObject->character->tag;
}


void DisplayObjectObject::ReleaseObject()
{
	//if(this->GetRefBitmap()!=XNULL)
	//	int v=0;
	if(m_pObject)
	{
		
		if(IsRunning())
			((ShellCore*)core())->GetPlayer()->ClearOpt(m_pObject);
		m_pObject=NULL;
		//m_pLoaderInfo=NULL;
	}
	EventDispatcherObject::ReleaseObject();
}



void DisplayObjectObject::Release()
{
//	StUTF8String s(vtable->traits->name());
//	if(strcmp(s.c_str(),"BattleArmy")==0)
//		int v=0;

	if(IsRunning())
	{
		//m_codeContext->m_eventsBinding->remove(this,true);
		if(m_pFreePlayer)
		{
			((ShellCore*)core())->GetPlayer()->AS3ReleasePlayer(m_pFreePlayer);
			m_pFreePlayer = NULL;
		}
		if(m_pObject)
		{
			//if(m_pObject&&m_pObject->character&&m_pObject->character->tag==65534)
			//	int v=0;
			m_pObject->pASObject=NULL;
			//m_pObject->thread=NULL;
			/*if(m_pObject->character->type==rootChar||
				(m_pObject->character->type==bitmapEmptyChar&&m_pObject->character->tag==ctagBitmapRoot))
			{
				m_pObject->character->player->AS3Unload(false);
				EventDispatcherObject::Release();
				//delete m_pObject;
				m_pObject=XNULL;
				return;
			}*/
				//int v=0;
		}
	}
	if(m_bNewObject&&m_pObject)
	{
		//if(m_pObject->character->tag==81)
		//	int v=0;
		//if(IsRunning())
		//	((ShellCore*)core())->GetPlayer()->AS3RemoveAndClear(m_pObject);
		//if(m_pObject->character->tag==131)
		//	int v=0;
		
			//int v=0;

		SObject*p=m_pObject;
		m_pObject=XNULL;		
		if(IsRunning())
		{
			//if(p->rootPlayer&&p==p->rootPlayer->rootObject)
			//	p->player->rootObject=XNULL;
			//if(p->thread&&p->thread->player->rootObject==p)
			//	p->thread->player->rootObject=XNULL;
			//if(p==__obj)
			//	int v=0;
			bool bRoot=p->IsRootObject();
			//if(!p->character||p->character->type==loaderChar)
			//	bRoot=true;
			ShellCore*c=(ShellCore*)core();
			//p->thread=NULL;
			p->pASObject=NULL;
			p->FreeChildren();
			p->FreeContext();
			if(!bRoot)
				c->GetPlayer()->display.FreeObject(p);
		}
		//else 
		//	p->Release();
		/*else
		{
			switch(type)
			{
				case 
			}
			//p->character->player=NULL;
			//p->pASObject=NULL;
			//p->FreeContext();
		}*/
		
		//ShellCodeContext*pContext=(ShellCodeContext*)core()->codeContext();
		
	}
	this->m_pLoaderInfo = NULL;
	EventDispatcherObject::Release();
		//delete m_pObject;
	m_pObject=XNULL;
}

bool DisplayObjectObject::CreateNewObject(int type)
{
	/*StUTF8String s(vtable->traits->name());
	StUTF8String s1(vtable->traits->ns()->getURI());
	if(strcmp(s.c_str(),"MainTimeline")==0&&strcmp(s1.c_str(),"cloud_fla")==0)
		int v=0;*/
	if(m_pObject) return true;
	//CodeContext*cd=this->vtable->init->codeContext();
	//ShellCodeContext*pContext=(ShellCodeContext*)core()->codeContext();
	//if(pContext==NULL) return false;
	ScriptPlayer*player=m_codeContext->m_pRoot;//((ShellCore*)core())->GetScriptPlayer();
	if(!player) return false;
	((ShellCore*)core())->GetPlayer()->AS3CreateSObject(m_pObject,type,this,(SCharacter*)this->vtable->GetInitData(),player);
	m_bNewObject=true;
	if(m_pObject)
	{
		ScriptPlayer*p=m_pObject->GetPlayer();
		if(p)
			m_pLoaderInfo=p->m_pLoaderInfo;
	}
	//((ShellCore*)core())->GetPlayer()->AS3Push(m_pObject);
	//type=m_pObject->character->tag;
	/*if(m_pObject->character->className==XNULL)//.IsNull())
	{
		Traits*t=traits();
		Stringp s=t->name();
		StUTF8String u(s);
		//name=u.c_str();
		m_pObject->character->className=(char*)u.c_str();//.SetString(u.c_str(),u.length());
		//m_pObject->character->className.ToString(XTRUE);
	}*/
	return true;
}

EventDispatcherObject* DisplayObjectObject::GetParentObject(bool bSeek)
{
	if(!m_pObject||!m_pObject->parent) return NULL;
	SObject*p=m_pObject->parent;
	if(p==&p->display->root)
		return AS3_stage_get();
	if(!bSeek) 
	{
		if(!p->pASObject)
			p->AS3CreateAndConstruct(XNULL,XTRUE);
		return (EventDispatcherObject*)p->pASObject;
	}
	while(p)
	{
		if(p->pASObject)
			return (EventDispatcherObject*)p->pASObject;
		p=p->parent;
	}
	return NULL;
}

void DisplayObjectObject::SetObject(void*pObject)
{
	m_pObject=(_XSObject*)pObject;
	if(m_pObject)
	{
		ScriptPlayer*p=m_pObject->GetPlayer();
		if(p)
			m_pLoaderInfo=p->m_pLoaderInfo;
	}
	else
		m_pLoaderInfo=NULL;
}

void DisplayObjectObject::DrawDevice(BXDeviceBitmapDataObject*pDst,MATRIX&mat0,ColorTransformObject*pTransform,Stringp blendMode,XRect&rect,bool sm,XRect&rc)
{
	
	/*if(m_pObject)
	{
		DisplayList*disp=m_pObject->display;
		SObject*pObj=m_pObject;
		XRaster*pRaster=pDst->m_pRaster;// raster;
		//XBitBuffer bits;
		//bits.bitmap=pDib;
		//SRECT rect={pDst->m_nOffX,pDst->m_nOffY,pDst->m_nWidth,pDst->m_nHeight};//0,pDib->Width(),0,pDib->Height()};//rc.left,rc.right,rc.top,rc.bottom};

		ScriptPlayer*player=m_pObject->thread?m_pObject->thread->player:NULL;
		if(player)
			player->doCommand(false);
		XBOOL bVisible=m_pObject->IsVisible();
		m_pObject->SetVisible(XTRUE);


		STransform x;
		//if(pMatrix)
		//	pMatrix->GetMatrix(&x.mat,true);
		//else
		//	::MatrixIdentity(&x.mat);
		x.mat=mat0;
		if(pTransform)
			pTransform->GetCXForm(x.cxform);
		else
			x.cxform.Clear();
		MATRIX mat;
		::MatrixIdentity(&mat);
		mat.a=mat.d=(fixed_1/20);//<<disp->antiShift;
		//pRaster->bits->bitmap->FillRect(XRect(0,0,600,900),0xffff0000,false);
		//mat.tx=pDst->m_nOffX;
		//mat.ty=pDst->m_nOffY;
		::MatrixConcat(&x.mat,&mat,&x.mat);
		x.mat.tx+=pDst->m_nAntiX>>4;//(4-disp->antiShift);//<<disp->antiShift;
		x.mat.ty+=pDst->m_nAntiY>>4;//(4-disp->antiShift);//<<disp->antiShift;
		//::MatrixConcat(&x.mat,&disp->camera.mat,&x.mat);
		//x.mat.a/=20;
		//x.mat.d/=20;
		//pDib->Reset();//Clear(XRect(rect.xmin,rect.ymin,rect.xmax,rect.ymax),0xff000000);
		//pDst->Clear();
		SRECT sr={rect.left,rect.right,rect.top,rect.bottom};
		pRaster->Attach(pRaster->bits,&sr,disp->antialias_b,disp->antiShift);
		//SPOINT pt={0,0};
		//::MatrixTransformPoint(&x.mat,
		pRaster->BeginPaint();
		pObj->Modify();
		pObj->CalcDrawUpdate(&x.mat,NULL,XFALSE);


   		//pObj->DrawDraw(pRaster,x,XNULL,XNULL,XFALSE,0);
		
		pRaster->PaintBits();
		//pObj->FreeCache();
		::FreeCache(pObj);
		
		m_pObject->SetVisible(bVisible);

		//MATRIX mat;
		::MatrixIdentity(&mat);
		pObj->GetTotalMatrix(mat,XTRUE);
		pObj->CalcDrawUpdate(&mat,NULL,XFALSE);
	}*/
}


void DisplayObjectObject::Draw(BitmapDataObject*pDst,MatrixObject*pMatrix,ColorTransformObject*pTransform,Stringp blendMode,RectangleObject*clip,bool sm)
{
	
	if(m_pObject)
	{
		pDst->LockBitmap(true);
		if(pDst->IsEmptyBits()) return;
		SBitmapCore* bm=&pDst->m_pChar->bits;
		bm->preAlpha = true;
		int w=bm->width,h=bm->height;
		FrameTexture*pFrame=FrameTexture::GetFrameTexture(w,h);//pDst->m_pChar->frame;
		if(!pFrame)
			return;
		//RasterGL::sharedRasterGL()->UpdateTexture(pFrame->mTexture->id,bm->baseAddr,Texture2DPixelFormat_RGBA8888,0,0,w,h);
		//if(m_pObject->character->tag==288)
		//	int v=0;
		//if(pDib->Width()==83&&pDib->Height()==24)
		//	int v=0;

		SObject*pObj=m_pObject;
		SRECT rect,*clipRect=NULL;

		ScriptPlayer*player=m_pObject->thread?m_pObject->thread->player:NULL;
		if(player)
			player->doCommand(false);
		XBOOL bVisible=m_pObject->IsVisible();
		m_pObject->SetVisible(XTRUE);
		
		int mode =drawBlendNormal0;
		if(blendMode!=NULL)
		{
			ShellToplevel*top=(ShellToplevel*)toplevel();
			BlendModeClass*pClass=top->getBlendModeClass();
			mode=pClass->getModeIDOf(blendMode);
			
		}
		if(clip)
		{
			AS3RECTANGLEDATA&data=clip->GetData();
			rect.xmin=data.left*onePoint;
			rect.ymin=data.top*onePoint;
			rect.xmax=data.left*onePoint+data.width*onePoint;
			rect.ymax=data.top*onePoint+data.height*onePoint;
			clipRect=&rect;
		}
		MATRIX cMat,invMat;
		XSWFPlayer::m_pInstance->display.GetCameraMatrix(cMat);
		::MatrixInvert(&cMat,&invMat);
		STransform x;
		if(pMatrix)
			pMatrix->GetMatrix(&x.mat,true);
		else
			::MatrixIdentity(&x.mat);
		if(pTransform)
			pTransform->GetCXForm(x.cxform);
		else
			x.cxform.Clear();
		x.cxform.nBlendMode = mode;
		//MATRIX mat;
		//::MatrixIdentity(&mat);
		::MatrixConcat(&x.mat,&invMat,&x.mat);
		//::MatrixConcat(&x.mat,&disp->camera.mat,&x.mat);
		//x.mat.a/=20;
		//x.mat.d/=20;
		//pDib->Reset();//Clear(XRect(rect.xmin,rect.ymin,rect.xmax,rect.ymax),0xff000000);
		if(pFrame->mHeight>=bm->height)
		{
			//pFrame->clear();
			RasterGL::sharedRasterGL()->UpdateTexture(pFrame->mTexture->id,bm->baseAddr,Texture2DPixelFormat_RGBA8888,0,0,w,h);
			pFrame->beginPaint(true,true);
			//SRECT* old=pObj->scrollRect;
			//pObj->scrollRect = clipRect;
			pObj->Modify();
			pObj->CalcDrawUpdate(&x.mat,NULL,XFALSE);
			if(clipRect==NULL)
			//{
				pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
			//}
			else
			{
				ClipMask mask;
				ClipMask* pMask = &mask;
				pMask = new ClipMask();
				pMask->beginPaint();
				pMask->setInverted( true );
				pMask->setupClip();
				float color[]={1,1,1,1};
				//SRECT rr={0,RectWidth(clipRect),0,RectHeight(clipRect)};
				//MATRIX idMat;
				//::MatrixIdentity(&idMat);
				RasterGL::sharedRasterGL()->DrawRectangle(*clipRect,invMat,color);//DrawShapeList(clipData,true);
				pMask->setupDraw();
				pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
				pMask->endPaint();
				pFrame->endPaint();
				
			}
			pFrame->GetPixels(0,0,w,h,(XU8*)bm->baseAddr,false);
			//pObj->scrollRect=old;
		}
		else
		{
			MATRIX mat=x.mat;
			MATRIX tMat;
			::MatrixIdentity(&tMat);
			//::MatrixIdentity(&idMat);
			int dy;
			int cw=bm->width;
			
			for(dy=0;dy<bm->height;dy+=pFrame->mHeight)
			{
				//for(dx=0;dx<bm->height;dx+=pFrame->mWidth)
				{
					//tMat.tx = -dx*20;
					tMat.ty = -dy*20;
					::MatrixConcat(&mat,&tMat,&x.mat);
					//int cw=pFrame->mWidth;
					int ch=pFrame->mHeight;
					//if(dx+cw>bm->width) cw=bm->width-dx;
					if(dy+ch>bm->height) ch=bm->height-dy;
					SRECT cr={0,cw,0,ch};
					XU8* dst=(XU8*)bm->baseAddr+bm->rowBytes*dy;//+dx*4;
					RasterGL::sharedRasterGL()->UpdateTexture(pFrame->mTexture->id,dst,Texture2DPixelFormat_RGBA8888,0,0,cw,ch);
					pFrame->beginPaint(true,true);
					pObj->Modify();
					pObj->CalcDrawUpdate(&x.mat,NULL,XFALSE);
					if(clipRect)
					{
						SRECT clip={clipRect->xmin/20,clipRect->xmax/20,clipRect->xmin/20+tMat.ty/20,clipRect->xmax+tMat.ty/20};
						::RectIntersect(&cr,&clip,&cr);
						if(RectIsEmpty(&cr)) {pFrame->endPaint();continue;}

						ClipMask mask;
						ClipMask* pMask = &mask;
						pMask = new ClipMask();
						pMask->beginPaint();
						pMask->setInverted( true );
						pMask->setupClip();
						float color[]={1,1,1,1};
						//SRECT rr={0,RectWidth(clipRect),0,RectHeight(clipRect)};
						//MATRIX mat;
						//::MatrixIdentity(&mat);
						RasterGL::sharedRasterGL()->DrawRectangle(cr,invMat,color);//DrawShapeList(clipData,true);
						pMask->setupDraw();
						pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
						pMask->endPaint();
					}
					else
					{
						pObj->Draw(RasterGL::sharedRasterGL(),x,true,NULL);
					}
					pFrame->endPaint();					
					pFrame->GetPixels(0,0,cw,ch,dst,false);
				}
			}
		}
			//pObj->scrollRect=old;
			//pObj->FreeCache();
		::FreeCache(pObj);
		m_pObject->SetVisible(bVisible);
		pObj->Modify();
		//MATRIX mat;
		::MatrixIdentity(&cMat);
		pObj->GetTotalMatrix(cMat,XTRUE);
		pObj->CalcDrawUpdate(&cMat,NULL,XFALSE);
		
		pDst->SetModify();
	}
}


GraphicsObject* DisplayObjectObject::CreateGraphics()
{
	AvmAssert(m_pObject!=NULL);
	GraphicsClass*pClass=((ShellToplevel*)this->toplevel())->getGraphicsClass();
	return pClass->CreateGraphics(this);
}


void DisplayObjectObject::AS3_drawNow()
{

}
/*bool DisplayObjectObject::CheckEventMask(int nCmd)
{
	if(!m_pObject||nCmd!=XAS3_EVENT_enterFrame||!m_pObject->character) return false;
	switch(m_pObject->character->type)
	{
	case rootChar:
	case spriteExternalChar:
	case spriteChar:
	case spriteEmptyChar:
	case playerChar:
		 return false;
	}
	return true;
}*/

//new adds
void DisplayObjectObject::AS3_blendShader_set(ShaderObject* pShader)
{

}
MatrixObject* DisplayObjectObject::AS3_cacheAsBitmapMatrix_get()
{
	return NULL;
}
void DisplayObjectObject::AS3_cacheAsBitmapMatrix_set(MatrixObject *pMatrix)
{

}
double DisplayObjectObject::AS3_rotationX_get()
{
	//if(m_pRotateData)
	//	return m_pRotateData->rx;
	//return 0;
	return m_rotationX;
}
void DisplayObjectObject::AS3_rotationX_set(double rotationX)
{
	//if(!m_pRotateData)
	//	m_pRotateData=new ROTATEDATA;
	//if(m_pRotateData)
	//	m_pRotateData->rx=rotationX;
	m_rotationX=rotationX;
}
double DisplayObjectObject::AS3_rotationY_get()
{
	//if(m_pRotateData)
	//	return m_pRotateData->ry;
	//return 0;
	return m_rotationY;
}
void DisplayObjectObject::AS3_rotationY_set(double rotationY)
{
	//if(!m_pRotateData)
	//	m_pRotateData=new ROTATEDATA;
	//if(m_pRotateData)
	//	m_pRotateData->ry=rotationY;
	m_rotationY=rotationY;
}
double DisplayObjectObject::AS3_rotationZ_get()
{
	//if(m_pRotateData)
	//	return m_pRotateData->rz;
	//return 0;
	return m_rotationZ;
}
void DisplayObjectObject::AS3_rotationZ_set(double rotationZ)
{
	//if(!m_pRotateData)
	//	m_pRotateData=new ROTATEDATA;
	//if(m_pRotateData)
	//	m_pRotateData->rz=rotationZ;
	if(MathUtils::isNaN(rotationZ))
		return;
	m_rotationZ=rotationZ;
}
double DisplayObjectObject::AS3_scaleZ_get()
{
	//if(m_pRotateData)
	//	return m_pRotateData->sz;
	//return 0;
	return m_scaleZ;
}
void DisplayObjectObject::AS3_scaleZ_set(double scaleZ)
{
	///if(!m_pRotateData)
	//	m_pRotateData=new ROTATEDATA;
	//if(m_pRotateData)
	//	m_pRotateData->sz=scaleZ;
	if(MathUtils::isNaN(scaleZ))
		return;
	m_scaleZ=scaleZ;
}

Vector3DObject* DisplayObjectObject::AS3_globalToLocal3D(PointObject *pPoint)
{
	return NULL;
}
PointObject* DisplayObjectObject::AS3_local3DToGlobal(Vector3DObject *pPoint3D)
{
	return NULL;
}
//new adds end

ScriptObject* DisplayObjectObject::AS3_metaData_get()
{
	//LOGWHERE();
	return m_pMetaData;
}
void DisplayObjectObject::AS3_metaData_set(ScriptObject* metaData)
{
	//LOGWHERE();
	m_pMetaData = metaData;
}
}
