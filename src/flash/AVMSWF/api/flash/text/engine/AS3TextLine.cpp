#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "sobject.h"
#include "edittext.h"

#include "AS3TextLine.h"
#include "AS3TextLineValidity.h"
#include "AS3Rectangle.h"
#include "AS3GraphicElement.h"

namespace avmshell{
TextLineClass::TextLineClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextLineClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextLineObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextLineObject::TextLineObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectContainerObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nStartChar=0;
	m_nEndChar=0;
	m_bHasGraphic=false;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	m_strInvalid=top->getTextLineValidityClass()->getVALID();
	m_nAscent=0;
	m_nDescent=0;
	m_nTotalAscent=0;
	m_nTotalDescent=0;
	m_nTextWidth=0;
	m_nMaxWidth=500;
	m_nElementID=0;
}

void TextLineObject::SetBounds()
{
	//return;
	if(m_pObject)
	{
		int nDeltay=m_nTotalAscent;//*4/5;
		
		
		
		SObject*obj=m_pObject->bottomChild;
		while(obj)
		{
			obj->SetPosY(obj->GetPosY()-nDeltay*2/3);
			obj=obj->above;
		}

		m_pObject->editText->SetDeltay(0);//-nDeltay*20);

		//nDeltay=this->m_nAscent*4/5;

		SRECT&r=m_pObject->character->rbounds;
		r.xmax=(m_nTextWidth+8)*20;
		r.ymin=-nDeltay*20;
//#if (__CORE_VERSION__>=0x02070200)
		r.ymax=(m_nTotalAscent+m_nTotalDescent+2)*20;
		if(!m_pObject->bottomChild)
			nDeltay=nDeltay/8;
		else
			nDeltay=nDeltay*7/8;
//#else
//		r.ymax=(m_nTotalAscent+m_nTotalDescent+2-nDeltay*2/3)*20;
//#endif
		if(m_pObject->editText)
		{
			SRECT&r1=m_pObject->editText->editRect;
			r1.xmax=r.xmax;
			r1.ymin=-nDeltay*20;
			r1.ymax=r.ymax;
			//r1.ymin=0;
		}

		
	}
}

void TextLineObject::AppendDisplay(GraphicElementObject*gr)
{
	int nWidth=gr->m_nWidth,h=gr->m_nHeight;

	DisplayObjectObject*obj=gr->m_pGraphic;
	if(obj)
	{
		int ow=obj->AS3_width_get();
		int nDeltay=m_nTotalAscent;//*4/5;
		obj->AS3_x_set(gr->m_cacheX+(nWidth-ow)/2);
		obj->AS3_y_set(-nDeltay*2/3);
		this->AS3_addChild(obj);
		if(gr->m_cacheFormat)
		{
			_XTEXTFORMAT*pNew=(_XTEXTFORMAT*)gr->m_cacheFormat;
			pNew->size=nWidth*20;
			pNew->spaceHeight=h*20;
		}
	}
	m_pObject->Modify();
}

void TextLineObject::AppendObject(GraphicElementObject*gr)
{
	int nWidth=gr->m_nWidth,h=gr->m_nHeight;

	DisplayObjectObject*obj=gr->m_pGraphic;
	if(obj)
	{
		int ow=obj->AS3_width_get();
		obj->AS3_x_set(m_nTextWidth+(nWidth-ow)/2);
		obj->AS3_y_set(0);
		this->AS3_addChild(obj);
	}
	else
	{
		gr->m_cacheLine=this;
		gr->m_cacheX=m_nTextWidth;
	
	}
	/*else
	{
		nWidth=12;
		h=12;
	}*/
	m_nTextWidth+=nWidth;
	//int nHeight=gr->m_nHeight;
	int nHeight=this->m_nTotalAscent+this->m_nTotalDescent;
	if(nHeight<h)
		m_nTotalAscent=h-this->m_nTotalDescent;
	if(m_pObject&&m_pObject->editText)
	{
		void*pFmt=m_pObject->editText->AppendSpace(nWidth,h);
		if(!obj)
			gr->m_cacheFormat=pFmt;
	}
	m_nEndChar++;
	
}

void TextLineObject::AppendText(XU16*text,int length,void*pFmt0,int nWidth)
{	
	_XTEXTFORMAT*pFmt=(_XTEXTFORMAT*)pFmt0;
	int nHeight=this->m_nAscent+this->m_nDescent;
	m_nEndChar+=length;
	if(nHeight<pFmt->size/20)
		this->m_nAscent=pFmt->size/20-this->m_nDescent;
	if(m_nTotalAscent<m_nAscent)
		m_nTotalAscent=m_nAscent;
	m_nTextWidth+=nWidth;
	if(m_pObject&&m_pObject->editText)
		m_pObject->editText->AppendText(text,length,pFmt);
}

void TextLineObject::InitObject()
{
	if(m_pObject==NULL)
	{
		DisplayObjectObject::CreateNewObject(textLineChar);//CreateNewObject(
		if(m_pObject)
		{
			EditText*pEdit=m_pObject->editText;
			pEdit->m_flags&=~seditTextFlagsMultiline;
			pEdit->m_flags&=~seditTextFlagsWordWrap;
		}
	}
}

TextLineObject* TextLineClass::CreateObject()
{
	TextLineObject*pObj=(TextLineObject*)createInstance(ivtable(),prototype);
	pObj->InitObject();
	return pObj;
}

//////////////////////////////////////////////////////////
//Native Method start...
TextBlockObject*	TextLineObject::AS3_textBlock_get()
{
	return m_pBlock;
}
bool	TextLineObject::AS3_hasGraphicElement_get()
{
	return m_bHasGraphic;
}
bool	TextLineObject::AS3_hasTabs_get()
{
	return false;
}
TextLineObject*	TextLineObject::AS3_nextLine_get()
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	if(m_strInvalid->equals(top->getTextLineValidityClass()->getSTATIC()))
		return NULL;
	return m_pNext;
}
TextLineObject*	TextLineObject::AS3_previousLine_get()
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	if(m_strInvalid->equals(top->getTextLineValidityClass()->getSTATIC()))
		return NULL;
	return m_pPrev;
}
double	TextLineObject::AS3_ascent_get()
{
	return m_nAscent;
}
double	TextLineObject::AS3_descent_get()
{
	return m_nDescent;
}
double	TextLineObject::AS3_textHeight_get()
{
	return m_nAscent+m_nDescent;
}
double	TextLineObject::AS3_textWidth_get()
{
	return m_nTextWidth;
}
double	TextLineObject::AS3_totalAscent_get()
{
	return m_nTotalAscent;
}
double	TextLineObject::AS3_totalDescent_get()
{
	return m_nTotalDescent;
}
double	TextLineObject::AS3_totalHeight_get()
{
	return m_nTotalAscent+m_nTotalDescent;
}
int		TextLineObject::AS3_textBlockBeginIndex_get()
{
	return m_nStartChar;
}
int		TextLineObject::AS3_rawTextLength_get()
{
	return m_nEndChar-m_nStartChar;
}
double	TextLineObject::AS3_specifiedWidth_get()
{
	return m_nMaxWidth;
}
double	TextLineObject::AS3_unjustifiedTextWidth_get()
{
	return m_nTextWidth;
}
Stringp	TextLineObject::AS3_validity_get()
{
	return m_strInvalid;
}
void	TextLineObject::AS3_validity_set(Stringp value)
{
	m_strInvalid=value;
}
int		TextLineObject::AS3_atomCount_get()
{
	return m_nEndChar-m_nStartChar;
}
ObjectVectorObject*	TextLineObject::AS3_mirrorRegions_get()
{   //[Note X]
	return NULL;
}
int		TextLineObject::AS3_getAtomIndexAtPoint(double stageX, double stageY)
{  //Wait
	if(m_pObject)
	{
		MATRIX mat;
		m_pObject->GetTotalMatrix(mat,XFALSE);
		//int nDelay=m_nTotalAscent*4/5;
		if(m_pObject->editText->m_nDeltaY)
		{
			MATRIX matoff;
			::MatrixIdentity(&matoff);
			//mat.ty+=m_nDeltaY;
			matoff.ty=m_pObject->editText->m_nDeltaY;
			::MatrixConcat(&matoff,&mat,&mat);
		}
		//stageY-=m_pObject->editText->m_nDeltaY;
		::MatrixInvert(&mat,&mat);
		SPOINT pt={stageX*20,(stageY)*20};
		::MatrixTransformPoint(&mat,&pt,&pt);
		return m_pObject->editText->GetCharIndextAtPoint(pt.x/20,pt.y/20);//stageX,stageY);
	}
	//return 0;
	return 0;
}
int		TextLineObject::AS3_getAtomIndexAtCharIndex(int charIndex)
{
	if(!m_pObject) return -1;
	if(charIndex<0||charIndex>=m_pObject->editText->m_length) return -1;
	return charIndex;
}
RectangleObject*	TextLineObject::AS3_getAtomBounds(int i)
{
	SRECT rect;
	
	if(i<0) i=0;
	
	if(m_pObject)
	{
		if(i>=m_pObject->editText->m_length)
			i=m_pObject->editText->m_length-1;
		//CheckID(i);
		if(i>=m_pObject->editText->m_length||i<0)
			toplevel()->throwRangeError(kInvalidRangeError,INTString(i),INTString(m_pObject->editText->m_length));
			//return NULL;
		m_pObject->editText->GetCharBoundaries(i,rect);
		rect.ymin-=this->m_nTotalAscent;
		rect.ymax-=this->m_nTotalAscent;
		int dh=(rect.ymax-rect.ymin)/5;
		rect.ymin+=dh;
		rect.ymax+=dh;
	}
	RectangleObject*pObj=(RectangleObject*)((ShellToplevel*)toplevel())->getRectangleClass()->
		CreateRectangle(rect.xmin,rect.ymin,rect.xmax-rect.xmin,rect.ymax-rect.ymin);
	
	return pObj;
	//return NULL;
}
int		TextLineObject::AS3_getAtomBidiLevel(int atomIndex)
{
	return 0;
}
Stringp	TextLineObject::AS3_getAtomTextRotation(int atomIndex)
{
	return NULL;
}
int		TextLineObject::AS3_getAtomTextBlockBeginIndex(int atomIndex)
{
	return atomIndex+m_nStartChar;
}
int		TextLineObject::AS3_getAtomTextBlockEndIndex(int atomIndex)
{
	return atomIndex+m_nStartChar+1;
}
double	TextLineObject::AS3_getAtomCenter(int i)
{
	SRECT rect;
	if(i<0) return NULL;
	if(m_pObject)
	{
		//CheckID(i);
		if(i>=m_pObject->editText->m_length||i<0)
			toplevel()->throwRangeError(kInvalidRangeError,INTString(i),INTString(m_pObject->editText->m_length));
			//return NULL;
		m_pObject->editText->GetCharBoundaries(i,rect);
		//rect.ymin-=this->m_nTotalAscent;
		//rect.ymax-=this->m_nTotalAscent;
		//int dh=(rect.ymax-rect.ymin)/5;
		//rect.ymin+=dh;
		//rect.ymax+=dh;
		return (rect.xmin+rect.xmax)/2;
	}
	
	//RectangleObject*pObj=(RectangleObject*)((ShellToplevel*)toplevel())->getRectangleClass()->
		//CreateRectangle(rect.xmin,rect.ymin,rect.xmax-rect.xmin,rect.ymax-rect.ymin);
	return 0;
}
bool	TextLineObject::AS3_getAtomWordBoundaryOnLeft(int i)
{
	if(m_pObject)
	{
		if(i>=m_pObject->editText->m_length||i<0)
			toplevel()->throwRangeError(kInvalidRangeError,INTString(i),INTString(m_pObject->editText->m_length));
		if(i==0) return true;
		XU16*pBuffer=m_pObject->editText->m_buffer;
		int iType=TextBlockObject::TypeOfChar(pBuffer[i]);
		switch(iType)
		{
		case TextBlockObject::CHAR_NUM:
		case TextBlockObject::CHAR_ALPHA:
			return TextBlockObject::TypeOfChar(pBuffer[i-1])==iType;
			 break;
		case TextBlockObject::CHAR_SIGN:
			 return true;
		}
	}
	return false;
	//return false;
}
DisplayObjectObject*	TextLineObject::AS3_getAtomGraphic(int atomIndex)
{
	return NULL;
}
double	TextLineObject::AS3_getBaselinePosition(Stringp baseline)
{
	return 0.0;
}
Stringp	TextLineObject::AS3_dump()
{
	Stringp strTxt=core()->newConstantStringLatin1("<line>");	

	strTxt=String::concatStrings(strTxt,core()->newConstantStringLatin1("</line>"));
	return strTxt;
}

}
