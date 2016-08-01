// ObjectText.cpp: implementation of the ObjectText class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "ObjectText.h"
#include "splayer.h"
#include "ObjectCreator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//DEL XXObjectText* XXObjectText::CreateClass(XXObject *pSuper, XDTDefineTextDisp *pDisp, XU8 bInit)
//DEL {
//DEL 	XXObjectText*pText=new XXObjectText(pDisp);
//DEL 	if(pText)
//DEL 	{
//DEL 		if(bInit)
//DEL 			pText->InitObject();
//DEL 		pText->m_bBase=bInit;
//DEL 		pText->SetSuper(pSuper,bInit);
//DEL 	}
//DEL 	return pText;
//DEL }
#define XNEEDTEXT XNEEDOBJ(m_pObject)
#define XNEEDTEXTR XNEEDOBJR(m_pObject,cnt)


XBOOL XXObjectText::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
		//XXVarvar=XXVar::CreateConst(XOCT_UNDEFINE);
	if(m_pObject)
	{
		ScriptThread*thread=m_pObject->thread;
		EditText*pEdit=m_pObject->GetEdit();
		switch(id)
		{		
		case _SYSID(addListener):
		case _SYSID(getDepth):
		case _SYSID(getFontList):
		case _SYSID(getTextFormat):
		case _SYSID(getNewTextFormat):
		case _SYSID(removeListener):
		case _SYSID(removeTextField):
		case _SYSID(replaceSel):
		case _SYSID(replaceText):
		case _SYSID(setNewTextFormat):
		case _SYSID(setTextFormat):
			 var=this;
			 var.ToFunction(id);
			 return XTRUE;
		case _SYSID(text)://XXTEXT_textGet:
			 m_pObject->GetText(var);//strTxt);
			 return XTRUE;
		case _SYSID(_alpha)://XXTEXT_alphaGet:
			 var=m_pObject->GetAlpha();
			 return XTRUE;	

		case _SYSID(_quality):
		case _SYSID(restrict):
		case _SYSID(menu):
		case _SYSID(mouseWheelEnabled):
		case _SYSID(_highquality):
		case _SYSID(gridFitType):
		case _SYSID(filters)://XXTEXT_filtersGet:
		case _SYSID(condenseWhite)://XSYSID_CONDENSEWHITE://XXTEXT_condenseWhiteGet:
		case _SYSID(antiAliasType)://XSYSID_ANTIALIASTYPE://XXTEXT_antiGet:			 
		case _SYSID(sharpness)://XSYSID_SHARPNESS:
		case _SYSID(styleSheet)://XSYSID_STYLESHEET:
		case _SYSID(textWidth)://XSYSID_TEXTWIDTH://XXTEXT_textWidthGet:
		case _SYSID(thickness)://XSYSID_THICKNESS:
		case _SYSID(type)://XSYSID_TYPE:
			 return XTRUE;
		case _SYSID(autoSize)://XSYSID_AUTOSIZE://XXTEXT_autoSizeGet:
			 if(pEdit)
			 {
				 if(!pEdit->IsAutoSize())
					 var=_strAligns[0];
				 else
				 {
					 switch(pEdit->m_autoMode)//m_pFormat->align)
					 {
						default:
							var=_strAligns[0];//XXSystemCall::StringOf(XSYSID_NONE);//_strAligns[0];
							break;
						case stextAlignLeft:
							var=_strAligns[1];//XXSystemCall::StringOf(XSYSID_LEFT);//_strAligns[1];
							break;
						case stextAlignRight:
							var=_strAligns[2];//XXSystemCall::StringOf(XSYSID_RIGHT);//_strAligns[2];break;
						case stextAlignCenter:
							var=_strAligns[3];//XXSystemCall::StringOf(XSYSID_CENTER);//_strAligns[3];break;
					 }
				 }
			 }
			 return XTRUE;

		case _SYSID(background)://XXTEXT_backgroundGet:
			 if(pEdit)
			 {
				 var=pEdit->IsBackground();
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(backgroundColor)://XXTEXT_backgroundColorGet:
			 if(pEdit)
			 {
				 var=pEdit->GetBackColor();
#ifdef _ARGB_PIXEL_
				 SWAPPIXELRB(var.iData32);
#endif
			 }
			 return XTRUE;
		case _SYSID(border)://XXTEXT_borderGet:
			 if(pEdit)
			 {
				 var=pEdit->IsBorder();
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(borderColor)://XXTEXT_borderColorGet:
			 if(pEdit)
			 {
				 var=pEdit->GetBorderColor();
#ifdef _ARGB_PIXEL_
				 SWAPPIXELRB(var.iData32);
#endif
			 }
			 return XTRUE;
		case _SYSID(bottomScroll)://XXTEXT_bottomScrollGet:
			 if(pEdit)
			 {
				 var=pEdit->CalcMaxVScroll()+1;
			 }
			 return XTRUE;
		case _SYSID(embedFonts)://XXTEXT_embedFontsGet:
			 if(pEdit)
			 {
				 var=pEdit->IsUseFont();
				 var.ToLogic();
			 }
			 return XTRUE;

		case _SYSID(_height)://XXTEXT_heightGet:
			 var=m_pObject->GetHeight();
			 return XTRUE;
		case _SYSID(hscroll)://XXTEXT_hscrollGet:
			 if(pEdit)
				 var=pEdit->m_hscroll;
			 return XTRUE;
		case _SYSID(html)://XXTEXT_htmlGet:
			 if(pEdit)
			 {
				 var=pEdit->IsHtml();
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(htmlText)://XXTEXT_htmlTextGet:
			 if(pEdit)
			 {
				 var=pEdit->m_initialText;
			 }
			 return XTRUE;
		case _SYSID(maxChars)://XXTEXT_maxCharsGet:
			 if(pEdit)
				 var=pEdit->m_maxLength;
			 return XTRUE;
		case _SYSID(maxhscroll)://XSYSID_MAXHSCROLL://XXTEXT_maxhscrollGet:
			 if(pEdit)
				var=pEdit->CalcMaxHScroll()+1;
			 return XTRUE;
		case _SYSID(maxscroll)://XXTEXT_maxscrollGet:
			 if(pEdit)
				 var=pEdit->CalcMaxVScroll()+1;
			 return XTRUE;
		case _SYSID(multiline)://XXTEXT_multilineGet:
			 if(pEdit)
			 {
				 var=pEdit->IsMultiLine();
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(_name)://XXTEXT_nameGet:
			 var=m_pObject->name;
			 return XTRUE;
		case _SYSID(_parent)://XXTEXT_parentGet:
			 var=GetParent();
			 return XTRUE;
		case _SYSID(password)://XXTEXT_passwordGet:
			 if(pEdit)
			 {
				 var=pEdit->m_flags&seditTextFlagsPassword;
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(_rotation):
			 var=m_pObject->GetRotate();
			 return XTRUE;
		case _SYSID(scroll)://XXTEXT_scrollGet:
			 if(pEdit)
				 var=pEdit->m_vscroll;
			 return XTRUE;
		case _SYSID(selectable)://XXTEXT_selectableGet:
			 if(pEdit)
			 {
				 var=!(pEdit->m_flags&seditTextFlagsNoSelect);
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(_soundbuftime)://XXTEXT_soundbuftimeGet:
			 var=pCnt->pPlayer->nSoundBufferTime;
			 return XTRUE;
		case _SYSID(tabEnabled)://XXTEXT_tabEnabledGet:
			 if(pEdit)
			 {
				 var=!(pEdit->m_flags&seditTextNoTabStop);
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(tabIndex)://XXTEXT_tabIndexGet:
			 var=pCnt->pPlayer->TableIndex(m_pObject);
			 return XTRUE;
		case _SYSID(_target)://XXTEXT_targetGet:
			 pCnt->pPlayer->GetTargetPath(m_pObject,var);
			 return XTRUE;
		case _SYSID(textColor)://XXTEXT_textColorGet:
			 if(pEdit)
			 {
				 var=pEdit->GetTextColor();
#ifdef _ARGB_PIXEL_
				 SWAPPIXELRB(var.iData32);
#endif
			 }
			 return XTRUE;
		case _SYSID(textHeight)://XXTEXT_textHeightGet:
			 if(pEdit)
				 var=pEdit->m_pFormat->size;//_height;
			 return XTRUE;
		case _SYSID(_url)://XXTEXT_urlGet:
			if(m_pObject->GetPlayer())
				var=m_pObject->GetPlayer()->m_url;
			 return XTRUE;
		case _SYSID(variable)://XXTEXT_variableGet:
			 if(pEdit)
			 {
				// if(pEdit->m_variable)
					 var=pEdit->m_variable;
				// else
				//	 var.ToConst(XOCT_NULL);
			 }
			 return XTRUE;
		case _SYSID(visible)://XXTEXT_visiableGet:
			 var=m_pObject->IsVisible();//visible;
			 var.ToLogic();
			 return XTRUE;
		case _SYSID(_width)://XXTEXT_widthGet:
			 var=m_pObject->GetWidth();
			 return XTRUE;
		case _SYSID(wordWrap)://XXTEXT_wordWrapGet:
			 if(pEdit)
			 {
				 var=pEdit->m_flags&seditTextFlagsWordWrap;
				 var.ToLogic();
			 }
			 return XTRUE;
		case _SYSID(_x)://XSYSID__X://XXTEXT_xGet:
			 var=m_pObject->GetPosX();
			 return XTRUE;
		case _SYSID(_y)://XSYSID__Y://XXTEXT_yGet:
			 var=m_pObject->GetPosY();
			 return XTRUE;
		case _SYSID(_yscale)://XSYSID__YSCALE://XXTEXT_yscaleGet:
			 var=m_pObject->GetScaleY();
			 return XTRUE;
		case _SYSID(_ymouse)://XSYSID__YMOUSE://XXTEXT_ymouseGet:
			 {
				double x,y;
				pCnt->pPlayer->GetMousePos(m_pObject,x,y);
				var=y;
			}return XTRUE;
		case _SYSID(_xscale)://XSYSID__XSCALE://XXTEXT_xscaleGet:
			 var=m_pObject->GetScaleX();
			 return XTRUE;
		case _SYSID(_xmouse)://XSYSID__XMOUSE://XXTEXT_xmouseGet:
			  {
				double x,y;
				pCnt->pPlayer->GetMousePos(m_pObject,x,y);
				var=x;
			}return XTRUE;
		case _SYSID(length)://XSYSID_LENGTH://XXTEXT_lengthGet:
			 if(pEdit)
				var=pEdit->m_length;
			 return XTRUE;		
		}
	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectText::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
		//XXVarvar=XXVar::CreateConst(XOCT_UNDEFINE);
	//XXVar var;
	//var.ToConst(XOCT_UNDEFINE);
	if(m_pObject)
	{
		ScriptThread*thread=m_pObject->thread;
		EditText*pEdit=m_pObject->GetEdit();
		int nSize=list.GetSize();
		switch(id)
		{
		case _SYSID(getDepth):
			 pCnt->pStack->Push(m_pObject->depth);
			 return XTRUE;
		case _SYSID(setNewTextFormat): 
		case _SYSID(setTextFormat):
		case _SYSID(getFontList):
		case _SYSID(getNewTextFormat):
		case _SYSID(getTextFormat):
			 pCnt->pStack->PushConst(XOCT_UNDEFINE);
			 return XTRUE;
		case _SYSID(removeTextField):
			 thread->RemoveDepth(m_pObject->depth);
			 pCnt->pStack->PushBool(XTRUE);
			 return XTRUE;
		case _SYSID(replaceSel):
			 if(pEdit&&nSize)
			 {
				 //pEdit->Delete();
				 list[0].ToString(XFALSE);
				 pEdit->ReplaceSel(list[0].strTxt);
				 //pEdit->Insert(list[0].strData,strlen(list[0].strData));
				 m_pObject->Modify();
			 }
			 pCnt->pStack->PushBool(XTRUE);
			 return XTRUE;
		case _SYSID(replaceText):
			if(pEdit&&nSize>=3)
			{
				int ns=list[0].ToInt();
				int ne=list[1].ToInt();
				list[2].ToString(XFALSE);
				pEdit->ReplaceText(ns,ne,list[2].strTxt);
				m_pObject->Modify();
				//XString16 str(pEdit->m_buffer);
				//str.RemoveAt(ns,ne-ns);
				//str.Insert(ns,XString16(list[2].strData));
			}
			 pCnt->pStack->PushBool(XTRUE);
			 return XTRUE;
		case _SYSID(addListener):
			 if(nSize&&list[0].IsObject())
			 	XXObject::AddListener(m_pObject->thread->rootObject->pObject,list[0].pObject);
			 pCnt->pStack->PushBool(XTRUE);
			 return XTRUE;
		case _SYSID(removeListener):
			 if(nSize&&list[0].IsObject())
			 	XXObject::RemoveListener(list[0].pObject);
			 pCnt->pStack->PushBool(XTRUE);
			 return XTRUE;
		}
	}
//	if(!bPush) delete pVar;
	//else 
	return XXObject::SysCallMethod(id,pCnt,list);
}
XBOOL XXObjectText::SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var)
{
		//XXVarvar=XXVar::CreateConst(XOCT_UNDEFINE);
	if(m_pObject)
	{
		ScriptThread*thread=m_pObject->thread;
		EditText*pEdit=m_pObject->GetEdit();
		switch(id)
		{
		case _SYSID(text)://XXTEXT_textSet:
			 var.ToString(XFALSE);
			 m_pObject->SetText(var.strTxt);
			 if(pEdit)
				pEdit->UpdateToVariable(*pCnt,var);
			 return XTRUE;
		case _SYSID(_alpha)://XXTEXT_alphaSet:
			 m_pObject->SetAlpha(var.ToInt());
			 break;		
		case _SYSID(autoSize)://XXTEXT_autoSizeSet:
			 if(pEdit)//nSize&&pEdit)
			 {
				var.ToString(XFALSE);
				switch(var.strTxt[0])
				{
				default:
				case 'n':
				case 'N':
					pEdit->AutoSize(XFALSE);break;
				case 'c':
				case 'C':
					pEdit->AutoSize(XTRUE);
					pEdit->m_autoMode=stextAlignCenter;break;
				case 't':
				case 'T':
				case 'l':
				case 'L':
					pEdit->AutoSize(XTRUE);
					pEdit->m_autoMode=stextAlignLeft;break;
				case 'r':
				case 'R':
					pEdit->AutoSize(XTRUE);
					pEdit->m_autoMode=stextAlignRight;break;
				}
				m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(background)://XXTEXT_backgroundSet:			 
			 if(pEdit)
			 {
				 pEdit->SetBackground(var.ToLogic());
			 }
			 return XTRUE;
		case _SYSID(backgroundColor)://XXTEXT_backgroundColorSet:
			 if(pEdit)
			 {
				 XU32 nColor=var.ToInt()|0xff000000;
#ifdef _ARGB_PIXEL_
				 SWAPPIXELRB(nColor);
#endif

				 pEdit->SetBackColor(nColor);
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(border)://XXTEXT_borderSet:
			 if(pEdit)
			 {
				 pEdit->SetBorder(var.ToLogic());
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(borderColor)://XXTEXT_borderColorSet:
			 if(pEdit)
			 {
				 XU32 nColor=var.ToInt()|0xff000000;
#ifdef _ARGB_PIXEL_
				 SWAPPIXELRB(nColor);
#endif
				 pEdit->SetBorderColor(nColor);//var.ToInt()|0xff000000);
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(bottomScroll)://XSYSID_BOTTOMSCROLL:
		case _SYSID(_quality):
		case _SYSID(restrict):
		case _SYSID(menu):
		case _SYSID(mouseWheelEnabled):
		case _SYSID(_highquality):
		case _SYSID(gridFitType):
		case _SYSID(filters)://XXTEXT_filtersGet:
		case _SYSID(condenseWhite)://XSYSID_CONDENSEWHITE://XXTEXT_condenseWhiteGet:
		case _SYSID(antiAliasType)://XSYSID_ANTIALIASTYPE://XXTEXT_antiGet:			 
		case _SYSID(sharpness)://XSYSID_SHARPNESS:
		case _SYSID(styleSheet)://XSYSID_STYLESHEET:
		case _SYSID(textWidth)://XSYSID_TEXTWIDTH://XXTEXT_textWidthGet:
		case _SYSID(thickness)://XSYSID_THICKNESS:
		case _SYSID(type)://XSYSID_TYPE:
			 return XTRUE;
			 //return XTRUE;		
		case _SYSID(embedFonts):
			 if(pEdit)
			 {
				 pEdit->SetUseFont(var.ToLogic());
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(_height):
			 m_pObject->SetHeight(var.ToFloat());
			 return XTRUE;
		case _SYSID(hscroll):
			 if(pEdit)
			 {
				 int v=var.ToInt(),max=pEdit->CalcMaxHScroll();
				 if(v<0) v=0;
				 else if(v>max) v=max;
				 pEdit->m_hscroll=v;
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(html):
			 if(pEdit)
			 {
				 pEdit->SetHtml(var.ToLogic());
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(htmlText):
			 if(pEdit)
			 {
				 var.ToString(XFALSE);
				 pEdit->SetHTMLText(pCnt,var.strTxt);
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(maxChars):
			 if(pEdit)
				 pEdit->m_maxLength=var.ToInt();
			 return XTRUE;

		case _SYSID(maxscroll):
			 return XTRUE;
			 
		case _SYSID(multiline):
			 //bPush=XFALSE;
			 if(pEdit)
			 {
				 pEdit->SetMultiLine(var.ToLogic());
			 }
			 return XTRUE;
		case _SYSID(_name):
			 {
				//var.ToString(XFALSE);
				//FreeStr(m_pObject->name);
				m_pObject->name=var;//CreateStr(var.strTxt);
				//list[0].nType=XODT_INT;
				//m_pDisp->SetName(list[0].strData);
			 }return XTRUE;
		case _SYSID(_parent):
			 return XTRUE;
		case _SYSID(password):
			 if(pEdit)
			 {
				 if(var.ToLogic())
					 pEdit->m_flags|=seditTextFlagsPassword;
				 else
					 pEdit->m_flags&=~seditTextFlagsPassword;
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(_rotation):
			 m_pObject->SetRotate(var.ToFloat());
			 return XTRUE;
		case _SYSID(scroll):
			 if(pEdit)
			 {
				 int max=pEdit->CalcMaxVScroll();
				 int v=var.ToInt();
				 if(v<0) v=0;
				 else if(v>max) v=max;
				 pEdit->m_vscroll=v;
				 m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(selectable):
			 if(pEdit)
			 {
				 if(var.ToLogic())
					 pEdit->m_flags&=~seditTextFlagsNoSelect;
				 else
					 pEdit->m_flags|=seditTextFlagsNoSelect;
			 }
			 return XTRUE;
		case _SYSID(_soundbuftime):
			 pCnt->pPlayer->nSoundBufferTime=var.ToInt();			 
			 return XTRUE;
		case _SYSID(tabEnabled):
			 if(pEdit)
			 {
				 if(var.ToLogic())
				    pEdit->m_flags&=~seditTextNoTabStop;
				 else
					pEdit->m_flags|=seditTextNoTabStop;
			 }
			 return XTRUE;
		case _SYSID(tabIndex):
		case _SYSID(_target):
			 return XTRUE;
		case _SYSID(textColor):
			 if(pEdit)
			 {
				XU32 nColor=var.ToInt()|0xff000000;
#ifdef _ARGB_PIXEL_
				 SWAPPIXELRB(nColor);
#endif
				 pEdit->SetTextColor(nColor);//var.ToInt()|0xff000000);
				 m_pObject->Modify();
			 }
			 return XTRUE;;
		case _SYSID(textHeight):
			 //bPush=XFALSE;
			 if(pEdit)
			 {
				pEdit->m_pFormat->size=var.ToInt();
				m_pObject->Modify();
			 }
			 return XTRUE;
		case _SYSID(_url):return XTRUE;
		case _SYSID(variable):
			 if(pEdit)
			 {
				 var.ToString(XFALSE);
				 pEdit->SetVariable(var);
			 }
			 return XTRUE;;
		case _SYSID(_visible):
			 m_pObject->SetVisible(var.ToLogic());
			 return XTRUE;;
		case _SYSID(_width):
			 m_pObject->SetWidth(var.ToFloat());
			 return XTRUE;;
		case _SYSID(wordWrap):
			 if(pEdit)
			 {
				 if(var.ToLogic())
					 pEdit->m_flags|=seditTextFlagsWordWrap;
				 else
					 pEdit->m_flags&=~seditTextFlagsWordWrap;
			 }
			 return XTRUE;;
		case _SYSID(_x):
			 m_pObject->SetPosX(var.ToFloat());
			 return XTRUE;;
		case _SYSID(_y):
			 m_pObject->SetPosY(var.ToFloat());
			 break;
		case _SYSID(_yscale)://XSYSID__YSCALE:
			 m_pObject->SetScaleY(var.ToFloat());
			 break;
		case _SYSID(_ymouse)://XSYSID__YMOUSE:
		case _SYSID(_xmouse)://XSYSID__XMOUSE:return XTRUE;
		case _SYSID(_xscale)://XSYSID__XSCALE://XXTEXT_xscaleSet:
			 m_pObject->SetScaleX(var.ToFloat());
			 break;
		case _SYSID(length)://XSYSID_LENGTH:
			 return XTRUE;
		}
	}
	return XXObject::SysSetMember(id,pCnt,var);
}

void XXObjectText::InitObject()
{
	_SYSCALL(addListener);//XXVar(XSYSID_);//(addListeners)));
	_SYSCALL(removeListener);//XXVar(XSYSID_);//(removeListener)));
	_SYSCALL(getDepth);//XXVar(XSYSID_);//(getDepth)));
	_SYSCALL(getFontList);//XXVar(XSYSID_);//(getFontList)));
	_SYSCALL(getNewTextFormat);//XXVar(XSYSID_);//(getNewTextFormat)));
	_SYSCALL(getTextFormat);//XXVar(XSYSID_);//(getTextFormat)));
	_SYSCALL(removeTextField);//XXVar(XSYSID_);//(removeTextfield)));
	_SYSCALL(replaceSel);//XXVar(XSYSID_);//(replaceSel)));
	_SYSCALL(replaceText);//XXVar(XSYSID_);//(replaceText)));
	_SYSCALL(setNewTextFormat);//XXVar(XSYSID_);//(setNewTextFormat)));
	_SYSCALL(setTextFormat);//XXVar(XSYSID_);//(setTextFormat)));

	_SYSCALL(text);//XSYSID_);//(textGet),XSYSID_);//(textSet));
	_SYSCALL(_alpha);//XSYSID_);//(alphaGet),XSYSID_);//(alphaSet));
	_SYSCALL(antiAliasType);//XSYSID_ANTIXSYSID_);//(antiGet),XSYSID_);//(antiSet));
	_SYSCALL(autoSize);//XSYSID_);//(autoSizeGet),XSYSID_);//(autoSizeSet));

	_SYSCALL(background);//XSYSID_);//(backgroundGet),XSYSID_);//(backgroundSet));
	_SYSCALL(backgroundColor);//XSYSID_);//(backgroundColorGet),XSYSID_);//(backgroundColorSet));
	_SYSCALL(border);//XSYSID_);//(borderGet),XSYSID_);//(borderSet));
	_SYSCALL(borderColor);//XSYSID_);//(borderColorGet),XSYSID_);//(borderColorSet));
	_SYSCALL(bottomScroll);//XSYSID_);//(bottomScrollGet),XNULL);
	_SYSCALL(condenseWhite);//XSYSID_);//(condenseWhiteGet),XSYSID_);//(condenseWhiteSet));
	_SYSCALL(embedFonts);//XSYSID_);//(embedFontsGet),XSYSID_);//(embedFontsSet));
	_SYSCALL(filters);//(filtersGet),XSYSID_);//(filtersSet));
	_SYSCALL(gridFitType);//(gridFitTypeGet),XSYSID_);//(gridFitTypeSet));
	_SYSCALL(_height);//(heightGet),XSYSID_);//(heightSet));
	_SYSCALL(_highquality);//(highqualityGet),XSYSID_);//(highqualitySet));
	_SYSCALL(hscroll);//(hscrollGet),XSYSID_);//(hscrollSet));
	
	_SYSCALL(html);//(htmlGet),XSYSID_);//(htmlSet));
	_SYSCALL(htmlText);//(htmlTextGet),XSYSID_);//(htmlTextSet));
	_SYSCALL(length);//(lengthGet),XNULL);
	_SYSCALL(maxChars);//(maxCharsGet),XSYSID_);//(maxCharsSet));
	_SYSCALL(maxhscroll);//(maxhscrollGet),XNULL);
	_SYSCALL(maxscroll);//(maxscrollGet),XNULL);
	_SYSCALL(menu);//(menuGet),XSYSID_);//(menuSet));
	_SYSCALL(mouseWheelEnabled);//(mouseWheelEnabledGet),XSYSID_);//(mouseWheelEnabledSet));
	_SYSCALL(multiline);//(multilineGet),XSYSID_);//(multilineSet));
	_SYSCALL(_name);//(nameGet),XSYSID_);//(nameSet));
	_SYSCALL(_parent);//(parentGet),XSYSID_);//(parentSet));
	_SYSCALL(password);//(passwordGet),XSYSID_);//(passwordSet));
	_SYSCALL(quality);//(qualityGet),XSYSID_);//(qualitySet));
	_SYSCALL(restrict);//(restrictGet),XSYSID_);//(restrictSet));
	_SYSCALL(_rotation);//(rotationGet),XSYSID_);//(rotationSet));
	_SYSCALL(scroll);//(scrollGet),XSYSID_);//(scrollSet));
	_SYSCALL(selectable);//(selectableGet),XSYSID_);//(selectableSet));
	_SYSCALL(sharpness);//(sharpnessGet),XSYSID_);//(sharpnessSet));
	_SYSCALL(_soundbuftime);//(soundbuftimeGet),XSYSID_);//(soundbuftimeSet));
	_SYSCALL(styleSheet);//(styleSheetGet),XSYSID_);//(styleSheetSet));
	_SYSCALL(tabEnabled);//(tabEnabledGet),XSYSID_);//(tabEnabledSet));
	_SYSCALL(tabIndex);//(tabIndexGet),XSYSID_);//(tabIndexSet));
	_SYSCALL(_target);//(targetGet),XNULL);
	//AddPropertySystem(_strText,XSYSID_);//(textGet),XSYSID_);//(textSet
	_SYSCALL(textColor);//(textColorGet),XSYSID_);//(textColorSet));
	_SYSCALL(textHeight);//(textHeightGet),XSYSID_);//(textHeightSet));
	_SYSCALL(textWidth);//(textWidthGet),XSYSID_);//(textWidthSet));
	_SYSCALL(thickness);//(thicknessGet),XSYSID_);//(thicknessSet));
	_SYSCALL(type);//(typeGet),XSYSID_);//(typeSet));
	_SYSCALL(_url);//(urlGet),XNULL);
	_SYSCALL(variable);//(variableGet),XSYSID_);//(variableSet));
	_SYSCALL(_visible);//(visiableGet),XSYSID_);//(visiableSet));
	_SYSCALL(_width);//(widthGet),XSYSID_);//(widthSet));
	_SYSCALL(wordWrap);//(wordWrapGet),XSYSID_);//(wordWrapSet));
	_SYSCALL(_x);//(xGet),XSYSID_);//(xSet));
	_SYSCALL(_xmouse);//(xmouseGet),XNULL);//XSYSID_);//(xmouseSet));
	_SYSCALL(_y);//(yGet),XSYSID_);//(ySet));
	_SYSCALL(_ymouse);//(ymouseGet),XNULL);
	_SYSCALL(_xscale);
	_SYSCALL(_yscale);


}


XXObject* XXObjectText::CreateObject(XSWFCONTEXT*pCnt,XXVARLIST&list,XBOOL bObj)
{
	pCnt->pStack->PushConst(XOCT_NULL);
	return XNULL;
}

//DEL void XXObjectText::DoCallback(XSWFCONTEXT&cnt,XXVARLIST&list,XU16 funID)
//DEL {
//DEL 	//XXVarvar=XXVar::CreateConst(XOCT_UNDEFINE);
//DEL 	XXVar var;
//DEL 	var.ToConst(XOCT_UNDEFINE);
//DEL 	//if(!pVar) return;
//DEL 	XBOOL bPush=XTRUE;
//DEL 	if(m_pObject)
//DEL 	{
//DEL 		//ScriptThread*thread=m_pObject->GetThread();
//DEL 		//XXObjectClip*thread=//m_pObject->GetTarget();
//DEL 		ScriptThread*thread=m_pObject->thread;
//DEL 		//XXObjectClip*pThread=(XXObjectClip*)m_pObject->thread->rootObject->pObject;
//DEL 
//DEL 		EditText*pEdit=m_pObject->GetEdit();
//DEL 		int nSize=list.GetSize();
//DEL 		switch(funID)
//DEL 		{
//DEL 		case XXTEXT_getDepth:
//DEL 			 var=m_pObject->depth;
//DEL 			 break;
//DEL 		case XXTEXT_getFontList:
//DEL 			 break;
//DEL 		case XXTEXT_getNewTextFormat:
//DEL 			 break;
//DEL 		case XXTEXT_getTextFormat:
//DEL 			 break;
//DEL 		case XXTEXT_removeTextfield:
//DEL 			 thread->RemoveDepth(m_pObject->depth);
//DEL 			 break;
//DEL 		case XXTEXT_replaceSel:
//DEL 			 if(pEdit&&nSize)
//DEL 			 {
//DEL 				 //pEdit->Delete();
//DEL 				 list[0].ToString(XFALSE);
//DEL 				 pEdit->ReplaceSel(list[0].strTxt);
//DEL 				 //pEdit->Insert(list[0].strData,strlen(list[0].strData));
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_replaceText:
//DEL 			if(pEdit&&nSize>=3)
//DEL 			{
//DEL 				int ns=list[0].ToInt();
//DEL 				int ne=list[1].ToInt();
//DEL 				list[2].ToString(XFALSE);
//DEL 				pEdit->ReplaceText(ns,ne,list[2].strTxt);
//DEL 				m_pObject->Modify();
//DEL 				//XString16 str(pEdit->m_buffer);
//DEL 				//str.RemoveAt(ns,ne-ns);
//DEL 				//str.Insert(ns,XString16(list[2].strData));
//DEL 			}
//DEL 			 break;
//DEL 		case XXTEXT_setNewTextFormat: 
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_setTextFormat:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		
//DEL 
//DEL 		case XXTEXT_textGet:
//DEL 			{
//DEL 				//XString8 strTxt;
//DEL 				m_pObject->GetText(var);//strTxt);
//DEL 				//var.SetString(strTxt.DataTo());
//DEL 			}
//DEL 			 break;
//DEL 		case XXTEXT_textSet:
//DEL 			bPush=XFALSE;
//DEL 			if(nSize)
//DEL 			{
//DEL 				list[0].ToString(XFALSE);
//DEL 				m_pObject->SetText(list[0].strTxt);
//DEL 				if(pEdit)
//DEL 					pEdit->UpdateToVariable(cnt,list[0]);
//DEL 			}
//DEL 			 break;
//DEL 		case XXTEXT_alphaGet:
//DEL 			 var=m_pObject->GetAlpha();
//DEL 			 break;
//DEL 		case XXTEXT_alphaSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 				m_pObject->SetAlpha(list[0].ToInt());
//DEL 			 break;
//DEL 		case XXTEXT_antiGet:
//DEL 			 
//DEL 			 break;
//DEL 		case XXTEXT_antiSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_addListeners:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&list[0].IsObject())
//DEL 			 	XXObject::AddListener(m_pObject->thread->rootObject->pObject,list[0].pObject);
//DEL 			 break;
//DEL 		case XXTEXT_autoSizeSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				list[0].ToString(XFALSE);
//DEL 				switch(list[0].strTxt[0])
//DEL 				{
//DEL 				default:
//DEL 				case 'n':
//DEL 				case 'N':
//DEL 					pEdit->AutoSize(XFALSE);break;
//DEL 				case 'c':
//DEL 				case 'C':
//DEL 					pEdit->AutoSize(XTRUE);
//DEL 					pEdit->m_align=stextAlignCenter;break;
//DEL 				case 't':
//DEL 				case 'T':
//DEL 				case 'l':
//DEL 				case 'L':
//DEL 					pEdit->AutoSize(XTRUE);
//DEL 					pEdit->m_align=stextAlignLeft;break;
//DEL 				case 'r':
//DEL 				case 'R':
//DEL 					pEdit->AutoSize(XTRUE);
//DEL 					pEdit->m_align=stextAlignRight;break;
//DEL 				}
//DEL 				m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_autoSizeGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 if(!pEdit->IsAutoSize())
//DEL 					 var=_strAlign[0];
//DEL 				 else
//DEL 				 {
//DEL 					 switch(pEdit->m_align)
//DEL 					 {
//DEL 						default:
//DEL 							var=_strAligns[0];break;
//DEL 						case stextAlignLeft:
//DEL 							var=_strAligns[1];break;
//DEL 						case stextAlignRight:
//DEL 							var=_strAligns[2];break;
//DEL 						case stextAlignCenter:
//DEL 							var=_strAligns[3];break;
//DEL 					 }
//DEL 				 }
//DEL 			 }
//DEL 			 break;
//DEL 
//DEL 		case XXTEXT_backgroundSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(pEdit&&nSize)
//DEL 			 {
//DEL 				 pEdit->SetBackground(list[0].ToLogic());
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_backgroundGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->IsBackground();
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_backgroundColorGet:
//DEL 			 if(pEdit)
//DEL 				 var=pEdit->GetBackColor();
//DEL 			 break;
//DEL 		case XXTEXT_backgroundColorSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 pEdit->SetBackColor(list[0].ToInt()|0xff000000);
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_borderGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->IsBorder();
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_borderSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 pEdit->SetBorder(list[0].ToLogic());
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_borderColorGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->GetBorderColor();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_borderColorSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 pEdit->SetBorderColor(list[0].ToInt()|0xff000000);
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_bottomScrollGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->CalcMaxVScroll()+1;
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_condenseWhiteGet:
//DEL 			 break;
//DEL 		case XXTEXT_condenseWhiteSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_embedFontsGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->IsUseFont();
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_embedFontsSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 pEdit->SetUseFont(list[0].ToLogic());
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_filtersGet:
//DEL 			 break;
//DEL 		case XXTEXT_filtersSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_gridFitTypeGet:
//DEL 			 break;
//DEL 		case XXTEXT_gridFitTypeSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 
//DEL 		case XXTEXT_heightGet:
//DEL 			 var=m_pObject->GetHeight();
//DEL 			 break;
//DEL 		case XXTEXT_heightSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 				m_pObject->SetHeight(list[0].ToFloat());
//DEL 			 break;
//DEL 		case XXTEXT_hscrollGet:
//DEL 			 if(pEdit)
//DEL 				 var=pEdit->m_hscroll;
//DEL 			 break;
//DEL 		case XXTEXT_hscrollSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 int v=list[0].ToInt(),max=pEdit->CalcMaxHScroll();
//DEL 				 if(v<0) v=0;
//DEL 				 else if(v>max) v=max;
//DEL 				 pEdit->m_hscroll=v;
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_htmlGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->IsHtml();
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_htmlSet:
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 pEdit->SetHtml(list[0].ToLogic());
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_htmlTextGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->m_initialText;
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_htmlTextSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(pEdit&&nSize)
//DEL 			 {
//DEL 				 list[0].ToString(XFALSE);
//DEL 				 pEdit->SetHTMLText(cnt,list[0].strTxt);
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_maxCharsGet:
//DEL 			 if(pEdit)
//DEL 				 var=pEdit->m_maxLength;
//DEL 			 break;
//DEL 		case XXTEXT_maxCharsSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 				 pEdit->m_maxLength=list[0].ToInt();
//DEL 			 break;
//DEL 
//DEL 		case XXTEXT_maxhscrollGet:
//DEL 			 if(pEdit)
//DEL 				var=pEdit->CalcMaxHScroll()+1;
//DEL 			 break;
//DEL 		case XXTEXT_maxscrollGet:
//DEL 			 if(pEdit)
//DEL 				 var=pEdit->CalcMaxVScroll()+1;
//DEL 			 break;
//DEL 		case XXTEXT_menuGet:
//DEL 			 break;
//DEL 		case XXTEXT_menuSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_mouseWheelEnabledGet:
//DEL 			 break;
//DEL 		case XXTEXT_mouseWheelEnabledSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_multilineSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 pEdit->SetMultiLine(list[0].ToLogic());
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_multilineGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->IsMultiLine();
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_nameGet:
//DEL 			 var=m_pObject->name;
//DEL 			 break;
//DEL 		case XXTEXT_nameSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 			 {
//DEL 				list[0].ToString(XFALSE);
//DEL 				//FreeStr(m_pObject->name);
//DEL 				m_pObject->name=list[0];//CreateStr(list[0].strTxt);
//DEL 				//list[0].nType=XODT_INT;
//DEL 				//m_pDisp->SetName(list[0].strData);
//DEL 			 }break;
//DEL 		case XXTEXT_parentGet:
//DEL 			 var=GetParent();
//DEL 			 break;
//DEL 		case XXTEXT_parentSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_passwordGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->m_flags&seditTextFlagsPassword;
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_passwordSet:
//DEL 			 if(pEdit&&nSize)
//DEL 			 {
//DEL 				 if(list[0].ToLogic())
//DEL 					 pEdit->m_flags|=seditTextFlagsPassword;
//DEL 				 else
//DEL 					 pEdit->m_flags&=~seditTextFlagsPassword;
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_qualityGet:
//DEL 			 break;
//DEL 		case XXTEXT_qualitySet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_restrictGet:
//DEL 			 break;
//DEL 		case XXTEXT_restrictSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_rotationGet:
//DEL 			 var=m_pObject->GetRotate();
//DEL 			 break;
//DEL 		case XXTEXT_rotationSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 				 m_pObject->SetRotate(list[0].ToFloat());
//DEL 			 break;
//DEL 		case XXTEXT_scrollGet:
//DEL 			 if(pEdit)
//DEL 				 var=pEdit->m_vscroll;
//DEL 			 break;
//DEL 		case XXTEXT_scrollSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 int max=pEdit->CalcMaxVScroll();
//DEL 				 int v=list[0].ToInt();
//DEL 				 if(v<0) v=0;
//DEL 				 else if(v>max) v=max;
//DEL 				 pEdit->m_vscroll=v;
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_selectableGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=!(pEdit->m_flags&seditTextFlagsNoSelect);
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_selectableSet:
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				 if(list[0].ToLogic())
//DEL 					 pEdit->m_flags&=~seditTextFlagsNoSelect;
//DEL 				 else
//DEL 					 pEdit->m_flags|=seditTextFlagsNoSelect;
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_sharpnessGet:
//DEL 			 //var=m_pObject->
//DEL 			 break;
//DEL 		case XXTEXT_sharpnessSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_soundbuftimeGet:
//DEL 			 break;
//DEL 		case XXTEXT_soundbuftimeSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_styleSheetGet:
//DEL 			 break;
//DEL 		case XXTEXT_styleSheetSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_tabEnabledGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=!(pEdit->m_flags1&seditTextNoTabStop);
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_tabEnabledSet:
//DEL 			 if(pEdit&&nSize)
//DEL 			 {
//DEL 				 if(list[0].ToLogic())\
//DEL 				    pEdit->m_flags1&=~seditTextNoTabStop;
//DEL 				 else
//DEL 					pEdit->m_flags|=seditTextNoTabStop;
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_tabIndexGet:
//DEL 			 var=cnt.pPlayer->TableIndex(m_pObject);
//DEL 			 break;
//DEL 		case XXTEXT_tabIndexSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_targetGet:
//DEL 			{
//DEL 				cnt.pPlayer->GetTargetPath(m_pObject,var);
//DEL 				//if(p)
//DEL 				  // var.SetString(p);
//DEL 			};
//DEL 			 break;
//DEL 		case XXTEXT_textColorGet:
//DEL 			 if(pEdit)
//DEL 				 var=pEdit->GetTextColor();
//DEL 			 break;
//DEL 		case XXTEXT_textColorSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 pEdit->SetTextColor(list[0].ToInt()|0xff000000);
//DEL 				 m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_textHeightGet:
//DEL 			 if(pEdit)
//DEL 				 var=pEdit->m_height;
//DEL 			 break;
//DEL 		case XXTEXT_textHeightSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize&&pEdit)
//DEL 			 {
//DEL 				pEdit->m_height=list[0].ToInt();
//DEL 				m_pObject->Modify();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_textWidthGet:
//DEL 			 break;
//DEL 		case XXTEXT_textWidthSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_thicknessGet:
//DEL 			 break;
//DEL 		case XXTEXT_thicknessSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_typeGet:
//DEL 			 break;
//DEL 		case XXTEXT_typeSet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 		case XXTEXT_urlGet:
//DEL 			 var=m_pObject->character->player->url;
//DEL 			 break;
//DEL 		case XXTEXT_variableSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 list[0].ToString(XFALSE);
//DEL 				 pEdit->SetVariable(list[0]);
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_variableGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 //if(pEdit->m_variable)
//DEL 				 var=pEdit->m_variable;
//DEL 				 //else
//DEL 				//	 var.ToConst(XOCT_NULL);
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_visiableGet:
//DEL 			 var=m_pObject->IsVisible();//visible;
//DEL 			 var.ToLogic();
//DEL 			 break;
//DEL 		case XXTEXT_visiableSet:
//DEL 			 if(nSize)
//DEL 				 m_pObject->SetVisible(list[0].ToLogic());
//DEL 			 break;
//DEL 		case XXTEXT_widthGet:
//DEL 			 var=m_pObject->GetWidth();
//DEL 			 break;
//DEL 		case XXTEXT_widthSet:
//DEL 			 if(nSize)
//DEL 				 m_pObject->SetWidth(list[0].ToFloat());
//DEL 			 break;
//DEL 		case XXTEXT_wordWrapGet:
//DEL 			 if(pEdit)
//DEL 			 {
//DEL 				 var=pEdit->m_flags&seditTextFlagsWordWrap;
//DEL 				 var.ToLogic();
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_wordWrapSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(pEdit&&nSize)
//DEL 			 {
//DEL 				 if(list[0].ToLogic())
//DEL 					 pEdit->m_flags|=seditTextFlagsWordWrap;
//DEL 				 else
//DEL 					 pEdit->m_flags&=~seditTextFlagsWordWrap;
//DEL 			 }
//DEL 			 break;
//DEL 		case XXTEXT_xGet:
//DEL 			 var=m_pObject->GetPosX();
//DEL 			 break;
//DEL 		case XXTEXT_xSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 				 m_pObject->SetPosX(list[0].ToFloat());
//DEL 			 break;
//DEL 		case XXTEXT_yGet:
//DEL 			 var=m_pObject->GetPosY();
//DEL 			 break;
//DEL 		case XXTEXT_ySet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 				 m_pObject->SetPosY(list[0].ToFloat());
//DEL 			 break;
//DEL 		case XXTEXT_highqualityGet:
//DEL 			 break;
//DEL 		case XXTEXT_highqualitySet:
//DEL 			 bPush=XFALSE;
//DEL 			 break;
//DEL 
//DEL 		case XXTEXT_yscaleGet:
//DEL 			 var=m_pObject->GetScaleY();
//DEL 			 break;
//DEL 		case XXTEXT_yscaleSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 				 m_pObject->SetScaleY(list[0].ToFloat());
//DEL 			 break;
//DEL 		case XXTEXT_ymouseGet:
//DEL 			 {
//DEL 				int x,y;
//DEL 				cnt.pPlayer->GetMousePos(m_pObject,x,y);
//DEL 				var=y;
//DEL 			}break;
//DEL 		case XXTEXT_xscaleGet:
//DEL 			 var=m_pObject->GetScaleX();
//DEL 			 break;
//DEL 		case XXTEXT_xscaleSet:
//DEL 			 bPush=XFALSE;
//DEL 			 if(nSize)
//DEL 				 m_pObject->SetScaleX(list[0].ToFloat());
//DEL 			 break;
//DEL 		case XXTEXT_xmouseGet:
//DEL 			  {
//DEL 				int x,y;
//DEL 				cnt.pPlayer->GetMousePos(m_pObject,x,y);
//DEL 				var=x;
//DEL 			}break;
//DEL 		case XXTEXT_lengthGet:
//DEL 			 if(pEdit)
//DEL 				var=pEdit->m_length;
//DEL 			 break;
//DEL 		}
//DEL 	}
//DEL //	if(!bPush) delete pVar;
//DEL 	//else 
//DEL 	if(bPush)
//DEL 		cnt.pStack->Push(var);
//DEL 	//cnt.pStack->Push(pVar);
//DEL }




XXObject* XXObjectText::GetParent()
{
	if(m_pObject==XNULL) return XNULL;///return m_pParent;
	//XDTSpriteDisp*p=m_pDisp->GetParent();
	SObject*p=m_pObject->thread->rootObject->parent;
	if(p) return p->pObject;
	else  return XNULL;//m_pParent;
}

void XXObjectText::GetLength(XXVar &var)
{
	if(m_pObject==XNULL) return;
	EditText*p=m_pObject->GetEdit();
	if(p)
	{
		var=p->m_length;
	}
}

//DEL ScriptThread* XXObjectText::GetTarget(XSWFCONTEXT *pCnt)
//DEL {
//DEL 	ScriptThread*pTarget=pCnt->GetTarget();
//DEL 	if(m_pObject)
//DEL 	{
//DEL 		pTarget=m_pObject->thread;
//DEL 		if(pTarget&&pTarget->rootObject)
//DEL 			pTarget=pTarget->rootObject->thread;
//DEL 	}
//DEL 	return pTarget;
//DEL }
