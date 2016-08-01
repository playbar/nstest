/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc.
****************************************************************************/

#include "StdAfxflash.h"

#include "sobject.h"

#include "stags.h"
#include "splay.h"
#include "splayer.h"
#include "edittext.h"
#include "curve.h"
#include "ObjectText.h"

#include "avmplus.h"  //modify by hgl,头锟侥硷拷锟斤拷锟侥硷拷锟叫硷拷位锟矫ｏ拷锟脚碉拷前锟斤拷
using namespace avmplus;

#include "AS3TextField.h"
#include "AS3StyleSheet.h"
#include "AS3TextFormat.h"

#include "XDom.h"
#include "XDomCard.h"
#include "XDomText.h"
#include "XWAPText.h"

#define _TOTALFONT "榛�浣�"

// now defined in sobject.cpp
void ResolveFontName(char *result, const char *fontName);
static inline void SETMATRIX(MATRIX&mat)
{
	mat.b=mat.c=0;
	if(mat.a<0) mat.a=-mat.a;
	if(mat.d<0) mat.d=-mat.d;
	if(mat.a>mat.d) mat.a=mat.d;
	else		    mat.d=mat.a;
}

enum
{
	kUnixSerifFont      = 1,
	kUnixSansFont       = 9,
	kUnixTypewriterFont = 17,
	kUnixBoldOffset     = 2,
	kUnixItalicOffset   = 2
};

#define BORDER_PAD 8

/*void AddRect(P_SRECT rect,
			 P_MATRIX mat, GLColor* color,
			 DisplayList *display, REdge** edgeList,
			 SStroker* stroker);*/

// INVALID_CODE is used to represent a "missing" glyph which is not present
// in an outline font.  Characters with missing glyphs are rendered zero-width
// and are ignored when typed by the user.
#define INVALID_CODE 0xFFFF

// The default fallback font when no device font is found
#define DEFAULT_EDITTEXT_FONT "Times New Roman"
#define DEFAULT_EDITTEXT_J_FONT_1 "MS UI Gothic"
#define DEFAULT_EDITTEXT_J_FONT_2 "MS P"
#define DEFAULT_EDITTEXT_HEIGHT 360

#ifdef DOTFONT

inline int WidthToDot(int w,int scale,int&nWidth,int&nAdjust)
{
	const int nDotWidth[]=
	{0,0,0,0,0,0,//<6
	 0,0,0,0,	 //6,7,8,9
	 0,12,12,12,//10,11,12,13,
	 12,16,16,16,16};//14,15,16,17,18

	const int nDotAdjust[]=
	{0,0,0,0,0,0,//<6
	 0,0,0,0,	 //6,7,8,9
	 0,-1,-1,0,//10,11,12,13,
	 -1,-1,0,0,1};//14,15,16,17,18

	w=(w*scale)>>16;
	nWidth=w;
	if(w>18) return 0;

	nAdjust=nDotAdjust[w];

	return nDotWidth[w];
}

/*inline FontCharInfo* GetFont(int w,XSWFPlayer*player,int&fontSize)
{
	switch(w)
	{
	//case 8:
	//	fontSize=8;
	//	return &player->dotFont8.font;break;
	case 12:
		fontSize=12;
		return &player->dotFont12.font;break;
	case 16:
		fontSize=16;
		return &player->dotFont16.font;break;
	}
	return NULL;
}*/
inline int charPixel(int nCode,FontCharInfo*pInfo)
{
	XU8* pData=(XU8*)(((XU16*)pInfo->infoData)+pInfo->nGlyphs);
	return pData[nCode];
	//return w;
}
/*inline int DotToWidth(int nCode,int w,int scale,XSWFPlayer*player)
{
	FontCharInfo*pInfo=GetFont(w,player);
	XU8* pData=(XU8*)*(((XU16*)pInfo->infoData)+pInfo->nGlyphs);
	w=pData[nCode];
	return (w*scale)>>16;
	//return (w*scale)>>16;
}*/
#endif
//#ifdef EDITTEXT

//------------------------------------------------------------------------
//
//    EditText
//
static _XTEXTFORMAT* FindFormat(_XTEXTFORMAT*fmt,XU32 iStart,XU32 iEnd)
{
	if(iStart<fmt->iStart||iEnd>fmt->iEnd)
		return XNULL;
	_XTEXTFORMAT* p=fmt->child;
	while(p)
	{
		_XTEXTFORMAT*find=FindFormat(p,iStart,iEnd);
		if(find) return find;
		p=p->next;
	}
	return fmt;
}

_XTEXTFORMAT* _FindFormat(_XTEXTFORMAT*fmt,const char*strID)
{
	while(fmt)
	{
		if(!fmt->strID.IsNull())
		{
			if(XString8::Compare(fmt->strID.strTxt,strID,0,true)==0)
				return fmt;
		}
		if(fmt->child)
		{
			_XTEXTFORMAT*c=_FindFormat(fmt->child,strID);
			if(c) return c;
		}
		fmt=fmt->next;
	}
	return NULL;
}

inline static _XTEXTFORMAT* _GetFormat(_XTEXTFORMAT*fmt,XU32 id)
{
	_XTEXTFORMAT*f=fmt->_GetFormat(id);
	if(f) return f;
	return fmt;
}

static _XTEXTFORMAT* DeleteFormat0(_XTEXTFORMAT*fmt,int b,int e,DisplayList*disp)
{
	//_XTEXTFORMAT*next=fmt->next;
	_XTEXTFORMAT*root=fmt,*prev=NULL;

	if(b<0) b=0;

	int delta=e-b;
	while(fmt)
	{
		_XTEXTFORMAT*next=fmt->next;
		if(fmt->iStart>=b&&fmt->iEnd<=e)//[]---------------[]
		{								//   []----------[]
			fmt->Free(disp);
			if(root==fmt)
				root=next;
			else
				prev->next=next;
			fmt=next;
			continue;			
		}
		if(fmt->iStart>=e)  // []-------[]
							//              []------------[]
		{
			fmt->iStart-=delta;
			if(fmt->iEnd<=0x7fffffff)
				fmt->iEnd-=delta;//
		}
		else if(fmt->iStart>=b)  //[]--------------------[]
		{				    //       []------------------|--[]
			fmt->iStart=b;
			if(fmt->iEnd<0x7fffffff)
				fmt->iEnd=b+(fmt->iEnd-e);
		}
		else if(fmt->iEnd>=b)
		{
			if(fmt->iEnd>e)//   []-------------[]
			{			   //[]--|------------------[]
				if(fmt->iEnd<0x7fffffff)
					fmt->iEnd=fmt->iStart+(b-fmt->iStart)+(fmt->iEnd-e);
			}
			else			//     []------------[]
			{				//[]-----------[]
				fmt->iEnd=fmt->iStart+(b-fmt->iStart);
			}
		}
		
		if(fmt->child)
		{
			_XTEXTFORMAT*child=DeleteFormat0(fmt->child,b,e,disp);
			fmt->child=child;
		}
		prev=fmt;
		fmt=next;
	}
	return root;
}

inline _XTEXTFORMAT* DeleteFormat(_XTEXTFORMAT*fmt,int b,int e,DisplayList*disp)
{
	if(b>e)
	{
		int v=e;
		e=b;
		b=v;
	}
	return DeleteFormat0(fmt,b,e,disp);
}

static _XTEXTFORMAT* ReplaceFormat0(_XTEXTFORMAT*fmt,int b,int e,int delta,DisplayList*disp)
{
	_XTEXTFORMAT*root=fmt,*prev=NULL;
	while(fmt)
	{
		_XTEXTFORMAT*next=fmt->next;
		if(delta<0)
		{
			if(fmt->iStart>=e+delta&&fmt->iEnd<=e)
			{
				fmt->Free(disp);
				if(root==fmt)
					root=next;
				else
					prev->next=next;
				fmt=next;
				continue;
			}
		}
		/*if(fmt->iStart>=e)   //[]-----------[]
							 //					[]-----------[]
		{
			fmt->iStart+=delta;
			if(fmt->iEnd<0x7fffffff)
				fmt->iEnd+=delta;
		}
		else if(fmt->iStart>=b)//[]-----------[]
		{					   //	[]--------|---[]
			if(fmt->iEnd>e&&fmt->iEnd<0x7fffffff)
			{
				fmt->iEnd+=delta;
			}
			
		}
		else if(fmt->iEnd>b)
		{

		}*/
		if(fmt->iStart>=e+delta)
		{
			fmt->iStart+=delta;
		}
		if(fmt->iEnd>=e+delta&&fmt->iEnd<0x7fffffff)
		{
			fmt->iEnd+=delta;
		}
		if(fmt->child)
		{
			_XTEXTFORMAT*child=ReplaceFormat0(fmt->child,b,e,delta,disp);
			fmt->child=child;
		}
		prev=fmt;
		fmt=next;
	}
	return root;
}

inline _XTEXTFORMAT* ReplaceFormat(_XTEXTFORMAT*fmt,int b,int e,int delta,DisplayList*disp)
{
	//return;	
	
	if(!delta) return NULL;
	if(b<e)
	{
		int v=e;
		e=b;
		b=v;
	}

	if(delta<0)
		return DeleteFormat0(fmt,e+delta,e,disp);
	else
		return ReplaceFormat0(fmt,b,e,delta,disp);
	/*if(delta==e-b)
	{
		return DeleteFormat(fmt,b,e,bNext,disp);
		//return;
	}
	if(delta<0)
	{
		if(fmt->iStart>=e+delta&&
		   fmt->iEnd<=e)
		{
			fmt->Free(disp);
			return;
		}
	}

	if(fmt->iStart>=e+delta)
	{
		fmt->iStart+=delta;
	}
	if(fmt->iEnd>=e+delta&&fmt->iEnd<0x7fffffff)
	{
		fmt->iEnd+=delta;
	}
	//if(fmt->iEnd<e)
	if(bNext)
	{
		_XTEXTFORMAT*pf=fmt->next;
		while(pf)
		//if(pf)
		{
			ReplaceFormat(pf,b,e,delta,false,disp);
			pf=pf->next;
		}
	}
	
	if(fmt->child)
		ReplaceFormat(fmt->child,b,e,delta,true,disp);*/
}





#ifndef min
	#define min(x, y) ((x)<(y)?(x):(y))
#endif

#ifndef max
	#define max(x, y) ((x)>(y)?(x):(y))
#endif

BOOL EditText::m_insertMode = XTRUE;

void EditText::SetFilterSize(_XTEXTFORMAT*fmt)
{
	XSWFPlayer*player=m_obj->display->player;
	//if(player->scaleMode!=DisplayList::showAll)
	//	return;

	if(!player->GetFilter()) return;
	XDOMFILTER*pFilter=player->GetFilter();
	if(pFilter->nFontZoom&&pFilter->nFontBase&&fmt->size<pFilter->nFontBase)
	{
		if(pFilter->nFontZoom<256)
			fmt->size=pFilter->nFontZoom*2000/256;
		else
			fmt->size=fmt->size*pFilter->nFontZoom/256;
	}
}

void EditText::Init(SObject*obj)
{

	m_bModifyHTML=false;
	m_nDeltaY=0;
	editRect=obj->character->rbounds;
/*#if (__CORE_VERSION__>=0x02074000)
	if(editRect.xmin<0)
		editRect.xmin=0;
	if(editRect.ymin<0)
		editRect.ymin=0;
#endif*/
	m_bNoSize=false;
	nTextWidth	  = 0;
	nTextHeight	  = 0;
	m_clickTime = 0;
	m_obj             = obj;
	m_autoMode		  = 0;
	this->m_variable.nType=0;
	this->m_initialText.nType=0;
	m_sharpness=0;
	m_pFormat=obj->display->CreateFormat();
	m_pFormat->Init();
	SParser parser;
	parser.Attach(obj->character->data, 0);
	m_bURL=false;
	m_flags = parser.GetWord();
	ScriptPlayer* player=obj->character?obj->character->splayer:NULL;
	//m_pFormat->font=("Times New Roman");
	if ( m_flags & seditTextFlagsHasFont )
	{
		//parser.SkipBytes(2);
		U16 id = parser.GetWord();
		if(player)
		{
			SCharacter*ch = player->FindCharacter(id);
			if(ch&&ch->type==fontChar)
				m_pFormat->font.SetString(ch->font.strFontName,ch->font.fLen);//=XXVar(ch->font.fontName;
		}
	}
	if(m_pFormat->font.IsNull())
	{
		XString8 str;
		str.ConvertFrom(_TOTALFONT,XCHARSET_GB2312);
		m_pFormat->font.SetString(str.GetData(),str.GetLength());
	}

	if( m_flags & seditTextHasFontClass)
	{
		m_pFormat->font=parser.GetStringP();
	}


	if ( m_flags & seditTextFlagsHasFont )
	{
		//m_height=parser.GetWord();
		m_pFormat->size=parser.GetWord();
		SetFilterSize(m_pFormat);
		m_flags &= ~seditTextFlagsHasFont;

		//parser.SkipBytes(2);
	}

	if ( m_flags & seditTextFlagsHasTextColor )
	{
		//m_textColor=
		m_pFormat->color=parser.GetColor(XTRUE).all;
	}
	//else
	//	m_textColor.all=SRGBBlack;

	if ( m_flags & seditTextFlagsHasMaxLength )
	{
		m_maxLength = parser.GetWord();
	}
	else
	{
		m_maxLength = 0;
	}

	if ( m_flags & seditTextFlagsHasLayout )
	{
		//m_align =
		m_pFormat->align=parser.GetByte();
		m_pFormat->leftMargin=parser.GetWord();
		m_pFormat->rightMargin=parser.GetWord();
		m_pFormat->indent=parser.GetWord();
		m_pFormat->leading=parser.GetWord();
		//parser.SkipBytes(8);
	}
	else
	{
		m_pFormat->align = stextAlignLeft;
	}

	m_variable = parser.GetStringP();
	//if(m_variable.strTxt[0]=='.')
	//	int v=0;

	if ( m_flags & seditTextFlagsHasText )
	{

		m_initialText = parser.GetStringP();
	}
	else
	{
		m_initialText = _strEmpty;
	}
	m_nAntiType		  = ANTITYPE_NORMAL;

	m_character       = obj->character;
	m_clickTime       = 0;
	m_selecting       = XFALSE;
	m_buffer          = (U16*)XXVar::_AllocData(sizeof(U16));
						//new U16[1];
	m_buffer[0]       = '\0';
	m_length          = 0;
	m_selectionStart  = 0;
	m_caretIndex	  = 0;
	m_selectionEnd    = 0;
	m_hscroll         = 0;
	m_vscroll         = 0;
	m_mouseIsDown     = XFALSE;
	m_doFindCursor    = XFALSE;
	m_mousePosition.x = 0;
	m_mousePosition.y = 0;
	//m_lineX           = XNULL;
	m_drawn           = XFALSE;
	//m_lineStarts      = XNULL;
	lineInfo		  = XNULL;
	charsInfo		  = XNULL;
	m_numLines        = 1;
	m_atLineEnd       = XFALSE;
	m_heldLeadByte    = -1;
	//devCharWidths     = XNULL;
	//devCharHeights	  = XNULL;
	//m_lineHeights	  = XNULL;
	//devLineSpacing    = 0;
	//devAscent         = 0;
	m_backColor.all	  = SRGBWhite;
	m_borderColor.all = SRGBBlack;
	m_flags			 |= (m_flags&seditTextFlagsBorder)?seditTextFlagsFillBackground:0;

	RectSetEmpty(&editBounds);


}
EditText::EditText(SObject* obj)
{
//	if(obj->character->tag==138)
//		int v=0;

	Init(obj);
	lineInfo = NULL;

}

void EditText::ClearFormat()
{
	_XTEXTFORMAT*fmt=m_pFormat->child;
	while(fmt)
	{
		_XTEXTFORMAT*next=fmt->next;
		fmt->Free(m_obj->display);
		fmt=next;
	}
	m_pFormat->child=NULL;
}

void EditText::Release()
{
	m_variable.Release();

	//ClearEditVariable();
	//delete [] m_buffer;

	XXVar::_FreeData(m_buffer);
	//delete [] m_lineStarts;
	//delete [] m_lineHeights;
	//delete [] m_lineX;
	if(lineInfo)
		XXVar::_FreeData(lineInfo);
	if(charsInfo)
		XXVar::_FreeData(charsInfo);
	lineInfo=NULL;
	charsInfo=NULL;
	//delete [] lineInfo;
	//delete [] charsInfo;
	//delete [] devCharWidths;
	//delete [] devCharHeights;
	if(m_pFormat)
		m_pFormat->Free(m_obj->display);
	m_pFormat=NULL;
	this->m_variable.Release();
	this->m_initialText.Release();
}

EditText::~EditText()
{
	ClearEditVariable();
	Release();
	//delete [] m_buffer;

	//if(m_initialText)
	//{
	//	if(m_flags1&seditTextDelInitText)
	//		delete m_initialText;
	//}
	/*if(m_variable)
	{
		if(m_flags1&seditTextDelVarText)
			delete m_variable;
	}*/

}

#ifdef EDITTEXT
BOOL PlayerIsFontAvailable(const char *fontName)
{
	return XFALSE;
}



int GetFontName(_XTEXTFORMAT*fmt,ScriptPlayer*player)
{
	if(!fmt->font.IsNull())
	{
		int nID=player->FindFont(fmt->font.strTxt);
		if(nID>0) return nID;
	}
	if(fmt->next)
	{
		int nID=GetFontName(fmt->next,player);
		if(nID) return nID;
	}
	if(fmt->child)
	{
		int nID=GetFontName(fmt->child,player);
		if(nID) return nID;
	}
	return 0;
}


void EditText::GetFontDesc(FontDesc& fontDesc,XU16* text)
{
	SParser parser;
	parser.Attach(m_character->data, 0);

	U16 flags = parser.GetWord();

	U16 fontTag = 0;

	bool bFontOK=false;
	if( (m_flags & seditTextFlagsHasFont)&&m_character->tag==0)
	{
		fontTag = parser.GetWord();
		fontDesc.height = parser.GetWord();
		fontTag=GetFontName(m_pFormat,m_character->splayer);
		if(fontTag)
			bFontOK=true;
	}
	else

	if ( flags & seditTextFlagsHasFont )
	{
		fontTag = parser.GetWord();
		fontDesc.height = parser.GetWord();
		//fontTag = 0;
	}

	ScriptPlayer* player = m_character->splayer;
	fontDesc.font=XNULL;

	// On Unix, if no font was specified, fall back on built-in Times Roman.
	if ( !fontTag )
	{
		fontTag = kUnixSerifFont;
#ifdef FULLFONT
		/*fontTag = kUnixSerifFont;
		fontDesc.font=&player->splayer->buildInFont;
		if(!player->splayer->LoadFont(text))
		{
			fontDesc.font=XNULL;
			return;
		}
		fontDesc.boldFlag = XFALSE;
		fontDesc.italicFlag = XFALSE;
		fontDesc.japaneseFlag = XFALSE;
		fontDesc.height = DEFAULT_EDITTEXT_HEIGHT;
		XU8* s = fontDesc.font->data + 4*fontDesc.font->font.nGlyphs;
		fontDesc.codeOffset = fontDesc.codeOffset = (U16)s[0] | ((U16)s[1]<<8);;
		return;*/
		fontDesc.font=&player->splayer->buildInFont;
		//if(!player->splayer->LoadFont(text))
		//	fontDesc.font=XNULL;
#endif

		//player = player->splayer->builtInFontsPlayer;
	}

	if ( fontTag )
	{
		// Find the font character
		SCharacter* font = fontDesc.font;
		if(font==XNULL)
		{
			font=player->FindCharacter(fontTag);

			if(font&&(font->font.flags&sfontFlagsSystem)!=0)
				font=XNULL;
			else if(font&&font->type==fontChar&&font->font.nGlyphs>0)
			{
				fontDesc.font=XNULL;
				strncpy(fontDesc.fontName,font->font.strFontName,font->font.fLen);
			}
			else
				font=XNULL;

		}
#ifdef FULLFONT
		if(font==XNULL)
		{
			fontTag=kUnixSerifFont;
			fontDesc.font=&player->splayer->buildInFont;
			if ( !player->splayer->LoadBuiltInFonts() )
			{
				return;
			}
			//if(!player->splayer->LoadFont(text))
			//	fontDesc.font=XNULL;
			font=fontDesc.font;
		}
#endif
		char fontName[256]="";

		if ( font )
		{
			// Get the font name
			if ( font->font.deviceState==textDync)
			{
				fontDesc.boldFlag = 0;
				fontDesc.italicFlag = 0;
				fontDesc.japaneseFlag = 0;
			}
			//	int v=0;
			else if ( font->tagCode == stagDefineFont2 )
			{
				parser.Attach(font->data - font->font.nDataOffset, 0);
				int len = parser.GetByte();
				parser.GetData(fontName, len);
				fontName[len] = 0;

				fontDesc.boldFlag = (font->font.flags & sfontFlagsBold) != 0;
				fontDesc.italicFlag = (font->font.flags & sfontFlagsItalic) != 0;
				fontDesc.japaneseFlag = (font->font.flags & sfontFlagsShiftJIS) != 0;

			}
			else if ( font->font.infoData )
			{
				parser.Attach(font->font.infoData, 0);
				int len = parser.GetByte();
				parser.GetData(fontName, len);
				fontName[len] = 0;

				// Get the font flags
				U8 flags = parser.GetByte();

				//if(m_pFormat->italic)flags|tfontItalic;
				//if(m_pFormat->Bold)flags|tfontBold;

				//font->font.flags = flags;

				fontDesc.boldFlag = (flags & tfontBold) != 0;
				fontDesc.italicFlag = (flags & tfontItalic) != 0;
				fontDesc.japaneseFlag = (flags & 0xf) == tfontShiftJIS;
			}

			BOOL useGlyphFont = IsFontIncluded();

			if(bFontOK)
				useGlyphFont=true;

			// If outlines aren't included, we need to fall back
			// on a built-in font

			if ( !useGlyphFont )
			{
#ifdef FULLFONT
				//if(player->splayer->LoadFont(text))
				{
					font = &player->splayer->buildInFont;
					useGlyphFont = XTRUE;
				}
#else
				if ( !strcmp(fontName, SANS_NAME) )
				{
					fontTag = kUnixSansFont;
				}
				else if ( !strcmp(fontName, TYPEWRITER_NAME) )
				{
					fontTag = kUnixTypewriterFont;
				}
				else
				{
					fontTag = kUnixSerifFont;
				}
				if ( fontDesc.boldFlag )
				{
					fontTag += kUnixBoldOffset;
				}
				if ( fontDesc.italicFlag )
				{
					fontTag += kUnixItalicOffset;
				}

				font = m_character->player->splayer->builtInFontsPlayer->FindCharacter(fontTag);
				useGlyphFont = XTRUE;
#endif
			}

			// Figure out the code table offset, if applicable
			if ( useGlyphFont )
			{
				if(font->font.deviceState==textDync)
				{
					fontDesc.codeOffset=0;
				}
				else
				{
					U16 g = font->font.nGlyphs;
					U8* s;
					if ( font->font.flags & sfontFlagsWideOffsets )
					{
						s = font->data + 4*g;
						fontDesc.codeOffset = (U32)s[0] | ((U32)s[1]<<8) | ((U32)s[2]<<16) | ((U32)s[3]<<24);
					}
					else
					{
						s = font->data + 2*g;
						fontDesc.codeOffset = (U16)s[0] | ((U16)s[1]<<8);
					}
				}
			}
			else
			{
				// Not using the outlines so ignore the code table
				fontDesc.codeOffset = -1;

				// We must determine if this font actually exists.
				// If it doesn't, fall back on a predetermined font.
				if ( !PlayerIsFontAvailable(fontDesc.fontName) )
				{
					if ( fontDesc.japaneseFlag )
					{
						// MS UI Gothic is preferred, but fallback on MS Gothic if not present
						if ( PlayerIsFontAvailable(DEFAULT_EDITTEXT_J_FONT_1) )
						{
							strcpy(fontDesc.fontName, DEFAULT_EDITTEXT_J_FONT_1);
						}
						else
						{
							strcpy(fontDesc.fontName, DEFAULT_EDITTEXT_J_FONT_2);
						}
					}
					else
					{
						strcpy(fontDesc.fontName, DEFAULT_EDITTEXT_FONT);
					}
				}
			}

			// Success!  Return this font information.
			fontDesc.font = font;
			return;
		}
	}

	// No font info in the text field tag, so we'll have to use defaults.
	strcpy(fontDesc.fontName, DEFAULT_EDITTEXT_FONT);
	fontDesc.boldFlag = XFALSE;
	fontDesc.italicFlag = XFALSE;
	fontDesc.japaneseFlag = XFALSE;
	fontDesc.height = DEFAULT_EDITTEXT_HEIGHT;
	fontDesc.font = NULL;
	fontDesc.codeOffset = -1;
}

#endif


#ifdef DOTFONT
static int GetDotFontID(_XTEXTFORMAT*fmt,int scale)
{
	int nSize=(fmt->size*scale)>>16;
}
#endif

XU32 EditText::GetEditStyle()
{
	XU32 nStyle=XEDS_ANY;
	if(m_flags&seditTextFlagsHasMaxLength)
		nStyle|=m_maxLength<<16;
	if(m_flags&seditTextFlagsPassword)//		= 0x0010,
		nStyle|=XEDS_PASSWORD;
	if(m_flags&seditTextFlagsMultiline)//		= 0x0020,
		nStyle|=XEDS_MULTILINE;
	return nStyle;
}

BOOL PlayerIsMultiByte();
BOOL PlayerIsLeadByte(U8 ch);

BOOL EditText::IsMultiByte()
{
	return PlayerIsMultiByte();
}

BOOL EditText::IsLeadByte(U8 ch)
{
	return PlayerIsLeadByte(ch);
}

U16 * EditText::MBCSToWide(char *buffer, int index, int count,bool bGB)
{
	/*U16 *result = new U16[count + 1];

	U16 *dst = result;
	U8 *src = (U8*)buffer + index;

	while ( count )
	{
		if ( IsLeadByte(*src) && count > 1 )
		{
			*dst++ = ((U16)*src<<8) | *(src+1);
			src += 2;
			count -= 2;
		}
		else
		{
			*dst++ = *src++;
			count--;
		}
	}
	*dst = 0;

	return result;*/

	U16* dst=(U16*)XXVar::_AllocData((count+1)*sizeof(U16));

	int l=bGB?XString16::GBToUCS(buffer+index,dst,count):XString16::UTF8ToUCS(buffer+index,dst,count);
	dst[l]=0;
	return dst;

	//XString16 str(buffer);
	//return str.DataTo();
}

void EditText::WideToMBCS(U16 *buffer, int index, int count,XXVar&var)
{
	//char *result = new char[count * 2 + 1];
	var.SetString(0,count*2+1);
	U16 *src = buffer + index;

	while ( count-- )
	{
		var.AddWChar(*src);
		/*if ( *src > 0xff )
		{
			*dst++ = (*src>>8)&0xff;
			*dst++ = *src&0xff;
		}
		else
		{
			*dst++ = (char)(*src);
		}*/
		src++;
	}

	//*dst = 0;
	//return result;
	//XString8 str(buffer);//(char*)buffer+index,count);
	//return str.DataTo();

}

int WideStrLen(const U16 *buf)
{
	int len = 0;
	while ( *buf++ )
	{
		len++;
	}
	return len;
}

void WideStrCopy(U16 *dst, const U16 *src)
{
	while ( *dst++ = *src++ );
}

/*int WideStrnCmp(U16* buf, const U16* dst, int len)
{
	int i=0;
	while (i<len)
	{
		if(buf[i]>dst[i])
			return 1;
		if(buf[i]<dst[i])
			return -1;
		i++;
	}
	return 0;
}
*/
void EditText::GetBuffer(XXVar&var)
{
	WideToMBCS(m_buffer,0,m_length,var);
	//var.AttachString(WideToMBCS(m_buffer, 0, m_length));
}

void EditText::SetBuffer(XPCTSTR text,bool bGB2312)
{
	//if(text[0]=='0')
	//	int v=0;
	U16 *newBuffer = MBCSToWide((char*)text, 0, strlen(text),bGB2312);
	if ( newBuffer )
	{
		XBOOL bChange=XString16::Compare(newBuffer,m_buffer);
		if(bChange)
		{
			m_bModifyHTML=true;
			//delete [] m_buffer;
			XXVar::_FreeData(m_buffer);
			m_buffer = newBuffer;
			m_length = WideStrLen(m_buffer);
			m_selectionStart = min(m_length, m_selectionStart);
			m_caretIndex	 = m_selectionStart;
			m_selectionEnd   = min(m_length, m_selectionEnd);
			//if(bChange)
			m_obj->Modify();
		}
		else
			XXVar::_FreeData(newBuffer);
			//delete newBuffer;
	}
}

void EditText::SetBuffer(XPCWSTR text,int l,bool bEvent)
{
	//if(text[0]=='0')
	//	int v=0;
	//U16 *newBuffer = new U16(l+
		//MBCSToWide((char*)text, 0, strlen(text));
	//return;
	//if(l==0)
	//	int v=0;
	if ( text )
	{
		U16* buffer=(U16*)XXVar::_AllocData((l+1)*sizeof(U16));
		int length = 0;
		if(this->IsCondenseWhite()&&m_pFormat->align==stextAlignCenter
			&&m_autoMode==stextAlignLeft&&IsMultiLine())
		{
			for(int i=0; i<l; i++)
			{
				/*if(text[i]==editTextSpace||text[i]==editTextNewline)
				{
					if(buffer[length-1]!=editTextNewline)
					{
						buffer[length++] = editTextNewline;
					}
					continue;
				}*/
				buffer[length++]=text[i];
			}
			l=length;
		}
		
		if(l==0) l=WideStrLen(text);
		XBOOL bChange=l!=m_length||XString16::Compare(text,m_buffer,0,l);
		if(bChange)
		{
			m_bModifyHTML=true;
			//delete [] m_buffer;
			XXVar::_FreeData(m_buffer);
			m_buffer = (U16*)XXVar::_AllocData((l+1)*sizeof(U16));
				//new U16[l+1];
			memcpy(m_buffer,length?buffer:text,l*sizeof(U16)+2);
			m_buffer[l]=0;
			m_length = l;
			m_selectionStart = min(m_length, m_selectionStart);
			m_caretIndex	 = m_selectionStart;
			m_selectionEnd   = min(m_length, m_selectionEnd);
			
			#if defined(__APPLE__) || defined(__ANDROID__)
			m_selectionStart = m_length;
			m_caretIndex	 = m_length;
			m_selectionEnd   = m_length;
			#endif
			//if(bChange)
			m_obj->Modify();
			if(bEvent)
			{
				XXVar varName  = this->m_variable;
				if(!varName.IsNull())
				//if (newValue)
				{
					XXVar newv;
					this->GetBuffer(newv);
					// oldValue may legitimately be NULL if variable isn't set
					//char* oldValue = GetVariable(focus->thread, varName);
					BOOL changed = this->UpdateVariable(newv);//XFALSE;
				}
				m_obj->AS3_Onchange();
			}
		}
		XXVar::_FreeData(buffer);
	}
}




void EditText::AutoScroll()
{
	if ( !m_mouseIsDown )
	{
		return;
	}

	int maxVScroll = CalcMaxVScroll();

	int ox=m_vscroll,oy=m_hscroll;

	SRECT bounds;
	SPOINT pt = m_mousePosition;

	{
		bounds = editRect;//m_character->bounds;
		RectInset(2*onePoint, &bounds);

		::MatrixTransformRect(&m_mat,&bounds,&bounds);
		// Transform the mouse position into the local
		// coordinate system of the edit text
		//MATRIX invmat;
		//MatrixInvert(&m_mat, &invmat);
		//DisplayList*disp=m_obj->display;//m_character->player->display;
		/*if ( disp->antialias_b )
		{
			disp->ToAnti(pt);
			//pt.x *= 4;
			//pt.y *= 4;
		}*/
		//MatrixTransformPoint(&invmat, &pt, &pt);
	}

	// if the selection does not extend to the start of the buffer
	// and the mouse is out of bounds on the top, dec vscroll
	if ( pt.y < bounds.ymin )
	{
		if ( m_selectionEnd > 0 && m_vscroll > 0 )
		{
			m_vscroll--;
		}
	}
	if ( pt.y > bounds.ymax )
	{
		if ( m_selectionEnd < m_length && m_vscroll < m_numLines-1 )
		{
			m_vscroll++;
		}
	}

	if ( m_vscroll > maxVScroll )
	{
		m_vscroll = maxVScroll;
	}

	// Find the line that the cursor is on
	int lineStart = FindLineStart(m_selectionEnd);

	// Find the length of the line the cursor is on
	int lineLength = FindLineEnd(m_selectionEnd) - lineStart;

	// if the selection does not extend to the end of the buffer
	// and the mouse is out of bounds ont he bottom, inc vscroll

	// if the selection does not extend to the end of line and
	// the mouse is out of bounds on the right, inc hscroll

	// if the selection does not extend to the start of the line and
	// the mouse is out of bounds on the left, dec hscroll

	if ( !(m_flags & seditTextFlagsWordWrap) && m_pFormat->align != stextAlignRight && m_pFormat->align != stextAlignCenter )
	{
		int lineSelectionIndex = m_selectionEnd - lineStart;
		if ( pt.x > bounds.xmax && lineSelectionIndex < lineLength && m_hscroll < lineLength )
		{
			m_hscroll++;
		}
		if ( pt.x < bounds.xmin && lineSelectionIndex > 0 && m_hscroll > 0 )
		{
			m_hscroll--;
		}
	}
	if(ox!=m_vscroll||oy!=m_hscroll)
	{
		if(m_obj->pASObject)
			m_obj->AS3_Onscroll();
		else
			m_obj->OnEvent(0,m_obj->display->player,XOBJEVENT_ONSCROLLER,XNULL);
	}
}

void EditText::FindCursor(SRECT* devBounds, _XCHARINFO*info)//int *devCharWidths)
{
	// First, find the line that the cursor is on.
	int lineno = FindLineNumber(m_selectionEnd);
	// Scroll, if necessary, to keep the line in view
	if ( m_vscroll > lineno )
	{
		m_vscroll = lineno;
	}
	else
	{
		int visibleLines = CalcVisibleLines(m_vscroll);
		if ( lineno-m_vscroll >= visibleLines )
		//if(lineno-m_vscroll>=visibleLines)
		{
			m_vscroll = lineno-visibleLines+1;
		}
	}

	// Scroll, if necessary, to keep m_selectionEnd in view
	// Find the position of the beginning of the line the
	// cursor is on
	int startLine = FindLineStart(m_selectionEnd);
	int lineSelectIndex = m_selectionEnd-startLine;

	// Find the x position of m_selectionEnd
	if ( !(m_flags & seditTextFlagsWordWrap) )
	{
		if ( m_hscroll >= lineSelectIndex )
		{
			m_hscroll = lineSelectIndex;
			// Scroll the cursor so that it's at
			// the 1/4 mark of the text field's width
			float x = devBounds->xmin;
			int target = devBounds->xmin + (devBounds->xmax - devBounds->xmin) / 4;
			while ( x < target && m_hscroll )
			{
				m_hscroll--;
				x += info[startLine+m_hscroll].charWidth;//devCharWidths[startLine+m_hscroll];
			}
		}
		else
		{
			float x = devBounds->xmin;
			for ( int i=m_hscroll; i<lineSelectIndex; i++ )
			{
				x += info[startLine+i].charWidth;//devCharWidths[startLine+i];
			}
			// If we're out of view on the right, scroll to the right
			while ( x > devBounds->xmax )
			{
				x -= info[startLine+m_hscroll].charWidth;//devCharWidths[startLine+m_hscroll];
				m_hscroll++;
			}
		}
	}
}

void EditText::ClearSelection()
{
	m_selectionStart = m_selectionEnd = 0;
}

void EditText::Forward(int wholeWord, int extend)
{
	if ( wholeWord )
	{
		// Forward one word
		// Skip initial whitespace
		while ( m_selectionEnd < m_length && LineBreakOK_Index(m_selectionEnd) )
		{
			m_selectionEnd++;
		}
		// Skip until whitespace
		while ( m_selectionEnd < m_length && !LineBreakOK_Index(m_selectionEnd) )
		{
			m_selectionEnd++;
		}
	}
	else
	{
		// Forward one character
		if ( m_selectionEnd < m_length )
		{
			m_selectionEnd++;
		}
	}
	if ( !extend )
	{
		m_selectionStart = m_selectionEnd;
		m_caretIndex	 = m_selectionStart;
	}
	m_atLineEnd = XFALSE;
}

void EditText::Backspace()
{
	
	if ( m_selectionStart == m_selectionEnd )
	{
		// No selection, delete character before the cursor.
		if ( m_selectionEnd > 0 )
		{
			DeleteFormat(m_pFormat,m_selectionEnd-1,m_selectionEnd,m_obj->display);
			WideStrCopy(m_buffer+m_selectionEnd-1, m_buffer+m_selectionEnd);
			m_selectionEnd--;
			m_selectionStart--;
			m_caretIndex = m_selectionStart;
			m_length--;
			m_atLineEnd = XFALSE;
		}
	}
	else
	{
		// There is a selection, delete it.
		DeleteFormat(m_pFormat,m_selectionStart,m_selectionEnd,m_obj->display);
		DeleteSelection();
	}
}

void EditText::Backward(int wholeWord, int extend)
{
	if ( wholeWord )
	{
		// By one word
		// Skip initial whitespace
		while ( m_selectionEnd > 0 && LineBreakOK_Index(m_selectionEnd-1) )
		{
			m_selectionEnd--;
		}
		// Skip until whitespace
		while ( m_selectionEnd > 0 && !LineBreakOK_Index(m_selectionEnd-1) )
		{
			m_selectionEnd--;
		}
	}
	else
	{
		// By one character
		if ( m_selectionEnd > 0 )
		{
			m_selectionEnd--;
		}
	}
	if ( !extend )
	{
		m_selectionStart = m_selectionEnd;
		m_caretIndex	 = m_selectionStart;
	}
	m_atLineEnd = XFALSE;
}

void EditText::DeleteSelection()
{
	if ( m_selectionStart != m_selectionEnd )
	{
		int A, B;
		if ( m_selectionStart < m_selectionEnd )
		{
			A = m_selectionStart;
			B = m_selectionEnd;
		}
		else
		{
			A = m_selectionEnd;
			B = m_selectionStart;
		}
		int count = B - A;
		WideStrCopy(m_buffer+A, m_buffer+B);
		m_selectionStart = m_selectionEnd = A;
		m_caretIndex = m_selectionStart;
		m_length -= count;
		m_atLineEnd = XFALSE;
	}
}

int EditText::CalcVisibleLines(int iStart)
{
	//if ( !devLineSpacing )
	//{
	//	return 0;
//	}
	if(!lineInfo)
		return 0;

	SRECT bounds = editRect;//m_character->bounds;
	::MatrixTransformRect(&m_mat,&bounds,&bounds);
	//RectInset(2*onePoint, &bounds);
	int i = 0;
	int h=lineInfo[iStart].nHeight;
	int maxh=bounds.ymax-bounds.ymin;
	maxh+=h>>2;
	for(i=iStart+1;i<m_numLines;i++)
	{
		h+=lineInfo[i].nHeight;
		if(h>=maxh) 
		{
			return i-iStart;
		}
	}
	return m_numLines-iStart;
	
	// Calculate the max value for vscroll based on # of lines
	//int visibleLines = (devBounds.ymax-devBounds.ymin-devAscent)/devLineSpacing;
	//if ( visibleLines < 0 )
	//{
	//	visibleLines = 0;
	//}
	//visibleLines++;
	//return visibleLines;
}

// Calculate the max value for vscroll based on # of lines

int EditText::CalcMaxHScroll()
{
	//int lineStart = FindLineStart(m_selectionEnd);

	// Find the length of the line the cursor is on
	//int lineLength = FindLineEnd(m_selectionEnd) - lineStart;
	if(!lineInfo) return 0;
	int nMax=0;
	for(int i=0;i<this->m_numLines;i++)
	{
		_XLINEINFO&info=this->lineInfo[i];
		if(nMax<info.nMaxScroll)
			nMax=info.nMaxScroll;
	}
	return nMax;
	//return lineLength;
}

int EditText::CalcMaxVScroll()
{
	if(!lineInfo) return 0;
	SRECT bounds = editRect;//m_character->bounds;
	RectInset(2*onePoint, &bounds);
	::MatrixTransformRect(&m_mat,&bounds,&bounds);
	int i,h=bounds.ymax;
	for(i=m_numLines-1;i>=0;i--)
	{

		if(i==m_numLines-1)
		{
			bool bEmpty=XTRUE;
			int nStart=lineInfo[i].nStart;
			while(nStart<lineInfo[i+1].nStart)
			{
				switch(m_buffer[nStart])
				{
				case '\r':
				case '\n':break;
				default:
					bEmpty=XFALSE;
					break;
				}
				nStart++;
			}
			if(bEmpty) continue;
		}

		h-=lineInfo[i].nHeight;
		if(h<=bounds.ymin)
		{
			int dh=-lineInfo[i].nHeight/2;
			if(h<dh) return i+1;
			return i;
		}
	}
	//int i,h=0,maxh=devBounds.ymax-devBounds.ymin;
	/*for(i=iStart;i<m_numLines;i++)
	{
		h+=lineInfo[i].nHeight;
		if(h>=maxh) return i;
	}*/
	return 0;
	/*int maxVScroll = m_numLines-CalcVisibleLines();
	if ( maxVScroll < 0 )
	{
		maxVScroll = 0;
	}
	return maxVScroll;*/
}

int EditText::FindLineNumber(int pos)
{
	// Search the line starts array.  Find a line
	// with an index greater than pos.
	int i;
	for ( i=0; i<m_numLines-1; i++ )
	{
		if ( pos < lineInfo[i+1].nStart)//m_lineStarts[i+1] )
		{
			break;
		}
	}
	return i;
}

int EditText::FindLineEnd(int pos)
{
	int lineno = FindLineNumber(pos);

	if ( lineno < m_numLines-1 )
	{
		lineno++;
	}
	else
	{
		return m_length;
	}

	int lineEnd = lineInfo[lineno].nStart;//m_lineStarts[lineno];

	if ( lineEnd != 0 )
	{
		// Not the beginning or end of the buffer
		// Try to back up
		lineEnd--;
	}

	return lineEnd;
}

bool EditText::HandleKeyDown(int key, int modifiers,bool bChar)//, NativePlayerWnd* native )
{
	if ( !m_drawn )
	{
		return false;
	}

	BOOL readOnly = ((m_flags & seditTextFlagsReadOnly) != 0);
	bool bChange=false;
	switch ( key )
	{
	case 20:return false;
	case Keyboard::ID_KEY_CUT:
		if ( !readOnly )
		{
			CopyToClipboard( );//native );
			if(m_selectionEnd>m_selectionStart)
				DeleteFormat(m_pFormat,m_selectionStart,m_selectionEnd,m_obj->display);
			DeleteSelection();
			m_obj->AS3_Onchange();
			bChange=true;
		}
		break;
	case Keyboard::ID_KEY_COPY:
		if ( m_selectionStart != m_selectionEnd )
		{
			CopyToClipboard( );//native );
		}
		break;
	case Keyboard::ID_KEY_PASTE:
		if ( !readOnly )
		{
			DeleteSelection();
			PasteFromClipboard( );//native );
			//m_obj->AS3Onchange();
		}
		break;
	case Keyboard::ID_KEY_BACKSPACE:
		if(!bChar) break;
		if ( !readOnly )
		{
			Backspace();
			m_obj->AS3_Onchange();
			bChange=true;
		}
		break;
	case Keyboard::ID_KEY_SELECT_ALL:
		m_selectionStart = m_length;
		m_caretIndex = m_selectionStart;
		m_selectionEnd = 0;
		m_hscroll = 0;
		break;
	case Keyboard::ID_KEY_UP:
		{
			if ( m_selectionEnd != 0 && m_atLineEnd )
			{
				m_selectionEnd--;
			}

			// Find our position on this line
			int lineStart = FindLineStart(m_selectionEnd);
			int index = m_selectionEnd - lineStart;

			// Find the beginning of the previous line
			int target = lineStart;
			if ( target )
			{
				target = FindLineStart(target-1);
			}

			// New position is index'th character of the line
			int lineEnd = FindLineEnd(target);
			target += index;
			if ( target > lineEnd )
			{
				target = lineEnd;
			}

			if ( modifiers & Keyboard::ID_KEY_SHIFT )
			{
				// If shift-home, extend selection to beginning of line
				m_selectionEnd = target;
			}
			else
			{
				// If home, de-select and move cursor to beginning of line
				m_selectionStart = m_selectionEnd = target;
				m_caretIndex = target;
			}

			m_atLineEnd = XFALSE;
		}
		break;
	case Keyboard::ID_KEY_DOWN:
		{
			if ( m_atLineEnd )
			{
				if ( m_selectionEnd )
				{
					m_selectionEnd--;
				}
			}

			// Find our position on this line
			int lineStart = FindLineStart(m_selectionEnd);
			int index = m_selectionEnd - lineStart;

			// Find the beginning of the next line
			int target = FindLineEnd(m_selectionEnd);
			if ( target < m_length )
			{
				target++;
			}

			// New position is index'th character of the line
			int lineEnd = FindLineEnd(target);
			target += index;
			if ( target > lineEnd )
			{
				target = lineEnd;
			}

			if ( modifiers & Keyboard::ID_KEY_SHIFT )
			{
				// If shift-home, extend selection to beginning of line
				m_selectionEnd = target;
			}
			else
			{
				// If home, de-select and move cursor to beginning of line
				m_selectionStart = m_selectionEnd = target;
				m_caretIndex = target;
			}

			m_atLineEnd = XFALSE;
		}
		break;
	case Keyboard::ID_KEY_LEFT:
		Backward((modifiers & Keyboard::ID_KEY_CTRL) != 0,
				 (modifiers & Keyboard::ID_KEY_SHIFT) != 0);
		break;
	case Keyboard::ID_KEY_RIGHT:
		Forward((modifiers & Keyboard::ID_KEY_CTRL) != 0,
				(modifiers & Keyboard::ID_KEY_SHIFT) != 0);
		break;

	case Keyboard::ID_KEY_PAGE_UP:

		return false;

		{
			if ( modifiers & Keyboard::ID_KEY_SHIFT )
			{
				// If shift-home, extend selection to beginning of line
				m_selectionEnd = 0;
			}
			else
			{
				// If home, de-select and move cursor to beginning of line
				m_selectionStart = m_selectionEnd = 0;
				m_caretIndex = 0;
			}
			m_atLineEnd = XFALSE;
		}
		break;

	case Keyboard::ID_KEY_PAGE_DOWN:
		{
			if ( modifiers & Keyboard::ID_KEY_SHIFT )
			{
				// If shift-home, extend selection to beginning of line
				m_selectionEnd = m_length;
			}
			else
			{
				// If home, de-select and move cursor to beginning of line
				m_selectionStart = m_selectionEnd = m_length;
				m_caretIndex = m_length;
			}
			m_atLineEnd = XFALSE;
		}
		break;
	case Keyboard::ID_KEY_HOME:
		{
			if ( m_selectionEnd != 0 && m_atLineEnd )
			{
				m_selectionEnd--;
			}

			int target;
			if ( modifiers & Keyboard::ID_KEY_CTRL )
			{
				// Jump to beginning of buffer
				target = 0;
			}
			else
			{
				// Jump to beginning of line
				target = FindLineStart(m_selectionEnd);
			}
			if ( modifiers & Keyboard::ID_KEY_SHIFT )
			{
				// If shift-home, extend selection to beginning of line
				m_selectionEnd = target;
			}
			else
			{
				// If home, de-select and move cursor to beginning of line
				m_selectionStart = m_selectionEnd = target;
				m_caretIndex = target;
			}
			m_atLineEnd = XFALSE;
		}
		break;
	case Keyboard::ID_KEY_END:
		{
			if ( !m_atLineEnd )
			{
				int target;
				if ( modifiers & Keyboard::ID_KEY_CTRL )
				{
					// Jump to end of buffer
					target = m_length;
				}
				else
				{
					// Jump to end of line
					target = FindLineEnd(m_selectionEnd);
					if ( LineBreakOK_Index(target) )
					{
						target++;
						m_atLineEnd = XTRUE;
					}
				}
				// If shift-end, extend selection to end of line
				if ( modifiers & Keyboard::ID_KEY_SHIFT )
				{
					m_selectionEnd = target;
				}
				else
				{
					// Move to end of line
					m_selectionStart = m_selectionEnd = target;
					m_caretIndex = target;
				}
			}
		}
		break;
	case Keyboard::ID_KEY_INSERT:
		m_insertMode = !m_insertMode;
		break;
	case Keyboard::ID_KEY_DELETE:
		if ( !readOnly&&m_length )
		{
			//if(m_selectionEnd>m_selectionStart)
			DeleteFormat(m_pFormat,m_selectionEnd,m_selectionEnd+1,m_obj->display);

			if ( (modifiers & Keyboard::ID_KEY_CTRL) != 0 )
			{
				// Ctrl-Shift-Delete means nothing
				if ( (modifiers & Keyboard::ID_KEY_SHIFT) == 0 )
				{
					// If there is a selection, do regular delete.
					if ( m_selectionStart == m_selectionEnd )
					{
						Forward(XTRUE, XTRUE);
						Delete();
					}
					else
					{
						Delete();
					}
				}
			}
			else
			{
				Delete();
			}

			m_atLineEnd = XFALSE;
			m_obj->AS3_Onchange();
			bChange=true;
		}
		break;
	case Keyboard::ID_KEY_CLEAR:
		if ( !readOnly&&m_length )
		{
			if(m_selectionEnd>m_selectionStart)
				DeleteFormat(m_pFormat,m_selectionStart,m_selectionEnd,m_obj->display);
			DeleteSelection();
			m_atLineEnd = XFALSE;
			m_obj->AS3_Onchange();
			bChange=true;
		}
		break;
	case Keyboard::ID_KEY_ENTER:
		if ( !readOnly && (m_flags & seditTextFlagsMultiline) != 0 )
		{
			ReplaceFormat0(m_pFormat,m_selectionStart,m_selectionEnd,1,m_obj->display);
			Insert(editTextNewline);
			m_obj->AS3_Onchange();
			bChange=true;
		}
		break;
	default:
		if ( !readOnly )
		{
			ReplaceFormat0(m_pFormat,m_selectionStart,m_selectionEnd,1,m_obj->display);
			Insert(key);
			
			U16 chars[]={key,0};
			m_obj->AS3_OnTextinput(chars,NULL);
			m_obj->AS3_Onchange();

			bChange=true;
		}
		break;
	}
	if(bChange)
	{
		//XString8 s(m_buffer,this->m_length);
		this->m_initialText=_strEmpty;
		this->m_initialText.ToString(XTRUE);
		for(int i=0;i<m_length;i++)
			m_initialText.AddWChar(m_buffer[i]);
		//this->m_initialText.ToString(XTRUE);
	}
	m_doFindCursor = XTRUE;
	return bChange;
}

void EditText::CopyToClipboard( )//NativePlayerWnd* native )
{
	int A, B;
	if ( m_selectionStart < m_selectionEnd )
	{
		A = m_selectionStart;
		B = m_selectionEnd;
	}
	else
	{
		A = m_selectionEnd;
		B = m_selectionStart;
	}
	int count = B - A;

	#ifdef EDITTEXT_CLIPBOARD_WIDE
		U16 *buffer = (U16*)XXVar::_AllocData(((count*2)+1)*sizeof(U16));
			//new U16[count * 2 + 1];
		U16 *src = m_buffer+A;
		U16 *dst = buffer;
		while ( count-- )
		{
			*dst++ = *src++;
		}
		*dst = 0;
		//native->SetStringToClipboard(buffer);
		XXVar::_FreeData(buffer);
		//delete [] buffer;

	#else
		// Translate linefeeds to carriage returns
		char *buffer = new char[count * 2 + 1];
		U16 *src = m_buffer+A;
		char *dst = buffer;
		while ( count-- )
		{
// 			if ( *src == editTextNewline )
// 			{
//  				*dst++ = '\n';
// 			}
// 			else
			if ( *src > 0xff )
			{
				*dst++ = (*src>>8)&0xff;
				*dst++ = *src&0xff;
			}
			else
			{
				*dst++ = *src;
			}
			src++;
		}
		*dst = 0;

		//native->SetStringToClipboard(buffer);
		delete [] buffer;
	#endif
}

void EditText::PasteFromClipboard( )//NativePlayerWnd* native)
{
	/*#ifdef EDITTEXT_CLIPBOARD_WIDE
		U16* clipboard;
	#else
		char* clipboard;
	#endif

	clipboard = native->GetStringFromClipboard();
	if ( clipboard )
	{
		#ifdef EDITTEXT_CLIPBOARD_WIDE
			InsertWideChars( clipboard, WideStrLen( clipboard) );
			delete [] clipboard;
		#else
			Insert( clipboard, strlen( clipboard ) );
			FreeStr( clipboard );
		#endif
	}*/
	m_atLineEnd = XFALSE;
}

void EditText::Insert(char *s, int len,bool bEvent)
{
	U16 *wideChars = MBCSToWide(s, 0, len,false);
	InsertWideChars(wideChars, WideStrLen(wideChars),bEvent);
	XXVar::_FreeData(wideChars);
	//delete [] wideChars;
}

void EditText::InsertWideChars(U16 *s, int insertLen,bool bEvent)
{
	U16 *buffer = s;
	int count;

	if ( !insertLen )
	{
		goto exit_gracefully;
	}

	// If using font outlines, purge any characters not in the font
	{
		buffer = (U16*)XXVar::_AllocData((insertLen+1)*sizeof(U16));
			//new U16[insertLen+1];
		if ( !buffer )
		{
			goto exit_gracefully;
		}
		memcpy(buffer, s, insertLen * sizeof(U16));
		buffer[insertLen]=0;
		FontDesc fontDesc;
		GetFontDesc(fontDesc,buffer);
		U16 *dst = buffer, *src = buffer;
		while ( insertLen-- )
		{
			U16 code = *src;
			if ( code == editTextNewline || code == editTextSoftNewline )
			{
				*dst++ = code;
			}
			else
			{
				U16 g = FindGlyph(code, fontDesc);
				if ( g == INVALID_CODE )
				{
					// If it's uppercase/lowercase, switch case and give
					// it another go.
					if ( code >= 'A' && code <= 'Z' )
					{
						code += ('a' - 'A');
						g = FindGlyph(code, fontDesc);
					}
					else if ( code >= 'a' && code <= 'z' )
					{
						code -= ('a' - 'A');
						g = FindGlyph(code, fontDesc);
					}
				}
				if ( g != INVALID_CODE )
				{
					*dst++ = code;
				}
			}
			src++;
		}
		insertLen = dst - buffer;
	}

	m_atLineEnd = XFALSE;

	// Delete any selection
	DeleteSelection();

	// Handle overwrite mode.
	if ( !m_insertMode )
	{
		int writeEnd = m_selectionEnd + insertLen;
		if ( writeEnd > m_length )
		{
			// If there is a maximum length setting, check
			// against it first.
			if ( m_maxLength != 0 && writeEnd > m_maxLength )
			{
				insertLen -= (writeEnd - m_maxLength);
				if ( insertLen <= 0 )
				{
					goto exit_gracefully;
				}
				writeEnd = m_maxLength;
			}
			Expand(writeEnd);
			m_length = writeEnd;
			m_buffer[m_length] = '\0';
		}

		memcpy(m_buffer+m_selectionEnd, buffer, sizeof(U16) * insertLen);
		m_selectionStart += insertLen;
		m_selectionEnd += insertLen;
		m_caretIndex = m_selectionStart;
		goto exit_gracefully;
	}

	// We're in insert mode.
	// If there is a maximum length setting, check
	// against it first.
	if ( m_maxLength != 0 && m_length + insertLen > m_maxLength )
	{
		// Too long, shorten the string
		insertLen = m_maxLength - m_length;
		if ( insertLen <= 0 )
		{
			goto exit_gracefully;
		}
	}

	// Expand the string
	Expand(m_length + insertLen);

	// Move all characters after the cursor.
	count = m_length - m_selectionEnd + 1;
	if ( count > 0 )
	{
		memmove(m_buffer + m_selectionEnd + insertLen,
				m_buffer + m_selectionEnd,
				sizeof(U16) * count);
	}

	// Bump up the length
	m_length += insertLen;

	// Place the new characters
	memcpy(m_buffer + m_selectionEnd, buffer, sizeof(U16) * insertLen);

	m_selectionEnd += insertLen;
	m_selectionStart += insertLen;
	m_caretIndex = m_selectionStart;

exit_gracefully:
	if ( buffer != s )
	{
		//delete [] buffer;
		XXVar::_FreeData(buffer);
	}
	if(bEvent)
	{
		m_obj->AS3_OnTextinput(s,XNULL);
		m_obj->AS3_Onchange();
	}
}

void EditText::Expand(int size)
{
	if ( size > m_length )
	{
		U16 *newBuffer = (U16*)XXVar::_AllocData((size+1)*sizeof(U16));
			//new U16[size + 1];
		memset(newBuffer, 0, sizeof(U16) * (size + 1));
		if ( m_buffer )
		{
			WideStrCopy(newBuffer, m_buffer);
			//delete [] m_buffer;
			XXVar::_FreeData(m_buffer);
		}
		m_buffer = newBuffer;
	}
}

void EditText::Delete()
{
	if ( m_selectionStart == m_selectionEnd )
	{
		// There is no selection, delete character under cursor
		if ( m_selectionEnd < m_length )
		{
			WideStrCopy(m_buffer+m_selectionEnd, m_buffer+m_selectionEnd+1);
			m_length--;
		}
	}
	else
	{
		// There is a selection, delete it.
		DeleteSelection();
	}
}


#define DOUBLE_CLICK_TIME 600


XBOOL EditText::IsEditAble(bool bDrawn)

{

	if(bDrawn&&!m_drawn)
		return XFALSE;


	if ( m_flags & (seditTextFlagsNoSelect|seditTextFlagsWasStatic|seditTextFlagsReadOnly) )
	{
		return XFALSE;
	}

	//if((m_flags & seditTextFlagsReadOnly)) return XFALSE;

	return XTRUE;
}

void EditText::DblClick(SPOINT*)
{
	//SelectAll();
	return;
}

static XBOOL _HasClickEvent(_XTEXTFORMAT*pFmt)
{	
	while(pFmt)
	{
		if(!pFmt->url.IsNull()||!pFmt->txtEvent.IsNull()) return XTRUE;
		if(pFmt->child)
		{
			if(_HasClickEvent(pFmt->child)) return XTRUE;
			}
		pFmt=pFmt->next;
	}
	return XFALSE;
}

XBOOL EditText::HasClickEvent()
{
	//_XTEXTFORMAT*pFmt=m_pFormat;
	//if(!m_pFormat) return NULL;
	//return _HasClickEvent(m_pFormat);
	return m_bURL;
}


static _XTEXTFORMAT* GetEventFormat(_XTEXTFORMAT*pFmt)
{
	if(!pFmt->url.IsNull()||!pFmt->txtEvent.IsNull())
	{
		if(!pFmt->txtEvent.IsNull())
		{
			return pFmt;
		}
		else if(XString8::Compare(pFmt->url.strTxt,"event:",6,XTRUE)==0)
			return pFmt;
		_XTEXTFORMAT*p=pFmt->next;
		while(p)
		{
			_XTEXTFORMAT*pGet=GetEventFormat(p);
			if(pGet) return pGet;
			p=p->next;
		}
	}
	if(pFmt->child)
		return GetEventFormat(pFmt->child);
	return NULL;
}

void EditText::DoClick(int x,int y)
{
	if(!m_selecting)
	{
		//m_clickTime = XTime::GetTimeMSec();
		SPOINT pt={x,y};

		this->m_obj->GetTotalMatrix(m_mat,XTRUE);

		DoMouse(&pt,XTRUE);
		m_selecting = XFALSE;
	}
	_XTEXTFORMAT*pFmt=m_pFormat->_GetFormat(m_selectionStart);
	if(m_bURL&&(!pFmt||pFmt->url.IsNull()||!pFmt->txtEvent.IsNull()))
	{
		pFmt=GetEventFormat(m_pFormat);
	}
	//m_obj->display->player->Trace(XString16("Text Click"));
	if(pFmt&&!(pFmt->url.IsNull()||!pFmt->txtEvent.IsNull()))
	{
		XSWFPlayer*player=m_obj->display->player;
		
		//XString16 strMsg(pFmt->url.strTxt);
		//m_obj->display->player->Trace(strMsg);
		if(!pFmt->txtEvent.IsNull())
		{
			if(m_obj->pASObject)
			{
				m_obj->AS3_OnLink(XNULL,(U8*)pFmt->txtEvent.strTxt);
			}
		}
		else if(XString8::Compare(pFmt->url.strTxt,"event:",6,XTRUE)==0)
		{
			
			if(m_obj->pASObject)
			{
				//m_obj->display->player->Trace(XString16("Text Event Ok"));
				m_obj->AS3_OnLink(XNULL,(U8*)pFmt->url.strTxt+6);
			}
		}
		else
		{
			player->AddLoader(pFmt->url.strTxt,pFmt->target.strTxt);
		}
	}
}

void EditText::DoMouse(SPOINT* pt, BOOL mouseIsDown)
{
	if ( !m_drawn || !lineInfo)
	{
		return;
	}

	bool bSelect=(m_flags & seditTextFlagsNoSelect)==0;

	if(!bSelect&&(!m_bURL||!mouseIsDown)) return;
	/*if ( m_flags & seditTextFlagsNoSelect )
	{
		return;
	}*/

	SRECT bounds;
// #ifndef _UNIX
// 	if (IsFontIncluded())
// #endif
	{

		bounds = editRect;//m_character->bounds;
		RectInset(2*onePoint, &bounds);
		::MatrixTransformRect(&m_mat,&bounds,&bounds);
	}

	BOOL shiftKey;
	shiftKey = XFALSE;

	m_atLineEnd = XFALSE;

	// Check for double-click
	/*if ( bSelect&&mouseIsDown && m_mousePosition.x == pt->x && m_mousePosition.y == pt->y )
	{
	#ifndef SPRITE_XTRA
		// Check if less than DOUBLE_CLICK_TIME ms has elapsed
		if ( XTime::GetTimeMSec() - m_clickTime < DOUBLE_CLICK_TIME )
		{
			// Select entire word
			SelectAll();
			return;
		}
	#endif
	}*/

	/*if(mouseIsDown)
	{
		if(!m_clickTime)
			m_clickTime=XTime::GetTimeMSec();
	}
	else
		m_clickTime=0;*/

	m_mouseIsDown = mouseIsDown;
	m_mousePosition = *pt;
	
	

	SPOINT localpt = *pt;

	{
		// Transform the mouse position into local coordinates
		//MATRIX invmat;
		//MatrixInvert(&m_mat, &invmat);

		//DisplayList*disp=m_obj->display;//m_character->player->display;
	
		
		
		//MatrixTransformPoint(&invmat, &localpt, &localpt);
		//localpt.x/=20;
		//localpt.y/=20;
	}

	//int visibleLines = CalcVisibleLines();
	//if ( !visibleLines )
	//{
	//	return;
	//}
	int endLine=CalcVisibleLines(m_vscroll)+m_vscroll;

	// Find the line that the mouse is over
	int y=bounds.ymin,lineno = m_vscroll;//(localpt.y-bounds.ymin)/devLineSpacing;
	//lineno = max(0, min(lineno, visibleLines-1));
	while(lineno<endLine)
	{
		y+=lineInfo[lineno].nHeight;
		if(y>localpt.y) break;
		lineno++;
	}
	//lineno += m_vscroll;
	if ( lineno > m_numLines )
	{
		lineno = m_numLines;
	}

	int index = lineInfo[lineno].nStart;//m_lineStarts[lineno];
	int lineEnd = FindLineEnd(index);

	index += m_hscroll;
	if ( index > lineEnd )
	{
		index = lineEnd;
	}

	// Find the character in the text right before
	// the position of the mouse click
	float x = lineInfo[lineno].x;//m_lineX[lineno];
	for ( ; m_buffer[index]; index++ )
	{
		if ( index >= lineEnd )
		{
			if ( LineBreakOK_Index(index) )
			{
				index++;
				m_atLineEnd = XTRUE;
			}
			break;
		}
		if ( localpt.x < x+charsInfo[index].charWidth/2)//devCharWidths[index]/2 )
		{
			break;
		}
		x += charsInfo[index].charWidth;//devCharWidths[index];
		if ( x > bounds.xmax )
		{
			break;
		}
	}

	if(bSelect)
		m_selectionEnd = index;
	if ( mouseIsDown&&(!m_selecting || !bSelect))
	{

		if(m_bURL)
			m_selecting=XFALSE;
		else
			//bSelect=false;

		m_selecting = XTRUE;
		if ( !shiftKey )
		{
			if(bSelect)
				m_selectionStart = index;
			else
			{
				m_selectionStart=m_selectionEnd=index;
			}
			m_caretIndex = m_selectionStart;
			/*_XTEXTFORMAT*pFmt=m_pFormat->_GetFormat(index);
			if(mouseIsDown&&pFmt->url.GetLength()&&m_obj->pASObject)
			{
				if(XString8::Compare(pFmt->url.strTxt,"event:",6,XTRUE)==0)
				{
					m_obj->AS3_OnLink(XNULL,(U8*)pFmt->url.strTxt+6);
						//(U16*)(pFmt->url.strTxt+6),0);
				}
			}*/
		}
	}
	else
	{
		if ( bSelect&&!mouseIsDown )
		{
			m_selecting = XFALSE;
		}
	}
}


_XTEXTFORMAT* EditText::GetFormat(int x,int y)
{
	if ( !m_drawn || !lineInfo)
	{
		return NULL;
	}

	return FindFormat(m_pFormat,this->m_selectionStart,this->m_selectionEnd);
}

BOOL EditText::BreakEnglish(U16 ch1, U16 ch2)
{
	//return ch1 == ' ' || ch1 == '-' || ch1==0x1030;

	switch(ch1)
	{
	default:
		if(ch1>0) break;
	case ' ':
	case '-':
		{
			switch(ch2)
			{
			case 0x3010:
			case '(':
			case '[':
				return XTRUE;
			default:
				if(ch2>='0'&&ch2<='9') return XTRUE;
				if(ch2>='A'&&ch2<='Z') return XTRUE;
				if(ch2>='a'&&ch2<='z') return XTRUE;
				break;
			}
		}break;

	/*case '(':
	case '[':
	case 0x3010:
		if(ch2>='0'&&ch2<='9') return XTRUE;
		if(ch2>='A'&&ch2<='Z') return XTRUE;
		if(ch2>='a'&&ch2<='z') return XTRUE;
		break;*/
	}

	return XFALSE;
}

enum
{
	J_END   = 0x00,
	J_SETHI = 0x01,
	J_RANGE = 0x02
};

static BOOL JCheckTable(U8 *table, U16 ch)
{
	U8 ch_hi = (ch >> 8) & 0xFF;
	U8 ch_lo = (ch & 0xFF);

	U8 table_hi = 0;

	while ( *table )
	{
		if ( *table == 1 )
		{
			// Select a new high byte
			table_hi = *++table;
		}
		else if ( *table == 2 )
		{
			// Expect a range
			U8 range_lo = *++table;
			U8 range_hi = *++table;
			if ( ch_hi == table_hi && ch_lo >= range_lo && ch_lo <= range_hi )
			{
				return XTRUE;
			}
		}
		else
		{
			// Regular character
			if ( ch_hi == table_hi && ch_lo == *table )
			{
				return XTRUE;
			}
		}
		table++;
	}

	return XFALSE;
}

BOOL EditText::LineBreakOK_Index(int index)
{
	if ( index < 0 || index >= m_length )
	{
		return XFALSE;
	}
	if ( index == m_length-1 )
	{
		return LineBreakOK(m_buffer[index], 0);
	}
	return LineBreakOK(m_buffer[index], m_buffer[index+1]);
}

BOOL EditText::LineBreakOK(U16 ch1, U16 ch2)
// ch1: Last character of current line
// ch2: First character of next line
// return:
// 	XTRUE  if it is ok to break this two characters
// 	XFALSE do not break the characters
{
	return XFALSE;
	return BreakEnglish(ch1, ch2);
}

void EditText::CalculateLineStarts(MATRIX&mat,_XCHARINFO*info, int wi,SFIXED a,int iAscent,int iDescent)//, int indent)
{

	U16 *ptr = m_buffer;
	int maxLineStarts = 32;
	BOOL done = XFALSE;
	U16 *currentBreak = NULL;

	//bool bReturn=true;

	//delete [] m_lineStarts;
	//delete lineInfo;
	XXVar::_FreeData(lineInfo);
	lineInfo= (_XLINEINFO*)XXVar::_AllocData(maxLineStarts*sizeof(_XLINEINFO));
		//new _XLINEINFO[maxLineStarts];
	//m_lineStarts = new int[maxLineStarts];
	m_numLines = 0;

	_XTEXTFORMAT*fmt=_GetFormat(m_pFormat,0);//m_pFormat->_GetFormat(0);
	//int maxWidth=0;

	if(a)
	{
		lineInfo[0].lMargin= FixedMul(fmt->leftMargin<< 16,a) / fixed_1;
		lineInfo[0].rMargin= FixedMul(fmt->rightMargin<< 16,a) / fixed_1;
		lineInfo[0].indent= FixedMul(fmt->indent<< 16,a) / fixed_1;
	}
	else
	{
		lineInfo[0].lMargin=fmt->leftMargin;
		lineInfo[0].rMargin=fmt->rightMargin;
		lineInfo[0].indent=fmt->indent;
	}
	//m_lineStarts[0] = 0;
	lineInfo[0].nStart=0;
	lineInfo[0].nAdjust=0;
	lineInfo[0].nHeight=0;
	lineInfo[0].nMaxScroll=0;
	lineInfo[0].nAscent=iAscent*fmt->size/1024;
	lineInfo[0].nDescent=iDescent*fmt->size/1024;
	lineInfo[0].nLeading=fmt->leading;
	lineInfo[0].nWidth=0;

	//lineInfo[0].xPixelStart=0;
	lineInfo[0].x=0;


	float x = fmt->indent;//indent;

	nTextWidth = 0;
	nTextHeight = 0;
	int nLineWidth = 0;
	//bool bLine = false;

	//if((IsAutoSize()&&(((this->m_flags&seditTextFlagsWordWrap)==0&&(m_flags&seditTextLockHTML)==0)))||m_bNoSize)
	if(IsAutoSize()&&(((this->m_flags&seditTextFlagsWordWrap)==0&&(m_flags&seditTextLockHTML)==0)||m_bNoSize))
	{
		//MATRIX invMat;
		//::MatrixInvert(&mat,&invMat);
		SRECT ra=m_obj->display->player->player->frame;
		//SRECT r={0,ra.xmax/20,0,ra.ymax/20};//m_obj->display->player->player->frame;
		//m_obj->display->ToAnti(r);
		//::MatrixTransformRect(&invMat,&r,&r);
		int stagew=RectWidth(&ra)-1200;
		stageW=ra.xmax-1200;
		stageH=ra.ymax-1200;
		//if(fmt->align==0)
		//int left=m_obj->character->bounds.xmin;
		//int right=m_obj->character->bounds.xmax;
		//int cx=(left+right)>>1;
		int offx=(mat.tx*20);//>>m_obj->display->antialias_b;
		ra.xmax=ra.xmax*3/4;
		switch(this->m_nAntiType)
		{
			case stextAlignCenter:
				 //wi=stagew;
				 //if(wi>cx) wi=cx;
				 //maxWidth=stagex-left;
				 //wi=r.xmax+r.xmin/2-(r.xmax-ra.xmax)/2;
				 if(offx<0)
					 wi=ra.xmax+offx;
				 else
					 wi=ra.xmax-offx;
				 break;
			case stextAlignRight:
				 //wi=right;
				 //wi=r.xmax-(r.xmax-ra.xmax);
				 //maxx=stagex
				 if(offx>0)
					wi=ra.xmax;//ra.xmax-offx;
				 else
				    wi=ra.xmax+offx;
				 break;
			case stextAlignLeft:
				 //wi=r.xmax+r.xmin;
					 //stagew-left;
				 //maxx=stagex;
				 if(offx>0)
					wi=ra.xmax-offx;
				 else
					wi=ra.xmax;
				 break;

		}
		//if(maxWidth>right) bReturn=false;
		//bReturn=false;

	}
	int width=wi-lineInfo[0].lMargin-lineInfo[0].rMargin;
	BOOL breakHere = XFALSE,bCnt=XFALSE;
//	int nextIndent = 0;

	while ( !done )
	{
		switch ( *ptr )
		{
		case 0:
			breakHere = XTRUE;
			done = XTRUE;
			break;
		case editTextNewHTMLLine:
			ptr++;
			if((this->m_flags & seditTextFlagsMultiline)==0) break;
			bCnt=XTRUE;
			//lineInfo[m_numLines].nHeight=XMAX(lineInfo[m_numLines].nHeight,info[ptr-m_buffer].lineSpace);
			breakHere = XTRUE;
			{
				fmt=_GetFormat(m_pFormat,ptr-m_buffer);//
					//m_pFormat->_GetFormat(ptr-m_buffer);
				x = fmt->indent;//m_pFormat->_GetFormat(ptr-m_buffer)->indent;
			}
			break;
		case editTextNewline:
			ptr++;
			//if((this->m_flags & seditTextFlagsMultiline)==0&&!m_bNoSize) break;
			bCnt=XTRUE;
			//lineInfo[m_numLines].nHeight=XMAX(lineInfo[m_numLines].nHeight,info[ptr-m_buffer].lineSpace);
			breakHere = XTRUE;
			{
				fmt=_GetFormat(m_pFormat,ptr-m_buffer);//m_pFormat->_GetFormat(ptr-m_buffer);
				x = fmt->indent;//m_pFormat->_GetFormat(ptr-m_buffer)->indent;
			}
			break;

		case editTextSoftNewline:

			ptr++;
			//if((this->m_flags & seditTextFlagsMultiline)==0&&!m_bNoSize) break;
			bCnt=XTRUE;
			//if((this->m_flags & seditTextFlagsMultiline)==0) break;
			//if(lineInfo[m_numLines].nWidth)
			breakHere = XTRUE;
			x = 0;
			if(nTextWidth<nLineWidth)
				nTextWidth=nLineWidth;
			nLineWidth = 0;
			break;

		default:
			// Regular character
			x += info[ptr-m_buffer].charWidth+info[ptr-m_buffer].letterSpace;//fmt->letterSpacing;
		
			if ( ((m_flags & seditTextFlagsWordWrap) != 0||
				(m_flags&seditTextLockHTML)!=0) && x > width)
				//&&(!IsAutoSize()||x>stagex) )
			{
				if((m_flags & seditTextFlagsWordWrap) != 0)
				{
					if(nTextWidth<nLineWidth)
						nTextWidth=nLineWidth;
					nLineWidth=0;
				}
				breakHere = XTRUE;
				x = 0;
				if ( currentBreak )
				{
					ptr = currentBreak + 1;
				}
				if(!bCnt)
				{
					ptr++;
					//fmt=fmt->_NextFormat(ptr-m_buffer,m_pFormat);
				}
			}
			else
			{

				lineInfo[m_numLines].nWidth+=info[ptr-m_buffer].charWidth+info[ptr-m_buffer].letterSpace;//fmt->letterSpacing;
				//lineInfo[m_numLines].nAdjust+=info[ptr-m_buffer].adjust;
				lineInfo[m_numLines].nHeight=XMAX(lineInfo[m_numLines].nHeight,info[ptr-m_buffer].lineSpace);
				nLineWidth += info[ptr-m_buffer].charWidth+info[ptr-m_buffer].letterSpace;//fmt->letterSpacing;

				// Is this character a potential break position?
				if ( LineBreakOK(ptr[0], ptr[1]) )
				{
					currentBreak = ptr;
				}
				bCnt=XTRUE;
				ptr++;
				//fmt=fmt->_NextFormat(ptr-m_buffer,m_pFormat);
			}

			if(!breakHere&&x>width)
				lineInfo[m_numLines].nMaxScroll++;
		}

		if ( breakHere )												{

			if ( m_numLines >= maxLineStarts-1 )
			{
				maxLineStarts *= 2;
				//int *newLineStarts = new int[maxLineStarts];
				_XLINEINFO*newLineInfo = (_XLINEINFO*)XXVar::_AllocData(sizeof(_XLINEINFO)*maxLineStarts);
					//new _XLINEINFO[maxLineStarts];
				memcpy(newLineInfo,lineInfo,sizeof(_XLINEINFO)*(m_numLines+1));
				//delete [] lineInfo;
				XXVar::_FreeData(lineInfo);
				lineInfo = newLineInfo;
				//memcpy(newLineStarts, m_lineStarts, sizeof(int) * (m_numLines+1));
				//delete [] m_lineStarts;
				//m_lineStarts = newLineStarts;
			}

			//m_lineStarts[++m_numLines] = ptr - m_buffer;
			lineInfo[++m_numLines].nStart=ptr-m_buffer;

			if(a)
			{
				lineInfo[m_numLines].lMargin= FixedMul(fmt->leftMargin<< 16,a) / fixed_1;
				lineInfo[m_numLines].rMargin= FixedMul(fmt->rightMargin<< 16,a) / fixed_1;
				lineInfo[m_numLines].indent= FixedMul(fmt->indent<< 16,a) / fixed_1;
			}
			else
			{
				lineInfo[m_numLines].lMargin=fmt->leftMargin;
				lineInfo[m_numLines].rMargin=fmt->rightMargin;
				lineInfo[m_numLines].indent=fmt->indent;
			}
			//m_lineStarts[0] = 0;
			//width=w-lineInfo[0].lMargin-lineInfo[0].rMargin;
			lineInfo[m_numLines].nMaxScroll=0;
			lineInfo[m_numLines].nAdjust=0;
			lineInfo[m_numLines].nHeight=ptr-m_buffer<m_length?info[ptr-m_buffer].lineSpace:0;
			lineInfo[m_numLines].nAscent=iAscent*fmt->size/1024;
			lineInfo[m_numLines].nDescent=iDescent*fmt->size/1024;
			lineInfo[m_numLines].nLeading=fmt->leading;
			lineInfo[m_numLines].nWidth=0;

			//lineInfo[m_numLines].xPixelStart=0;
			lineInfo[m_numLines].x=0;

			currentBreak = NULL;
			breakHere    = XFALSE;
			bCnt=XFALSE;
		}
	}
	if(nTextWidth<nLineWidth)
		nTextWidth=nLineWidth;
	int nMinHeight=0,i;
	for(i=0;i<this->m_numLines-1;i++)
	{
		if(!this->lineInfo[i].nHeight) continue;
		if(!nMinHeight||nMinHeight>lineInfo[i].nHeight)
			nMinHeight=lineInfo[i].nHeight;
	}
	nTextHeight=0;
	if(nMinHeight<=0&&m_pFormat)
	{
		nMinHeight=m_pFormat->size;
	}
	for(i=0;i<this->m_numLines;i++)
	{
		if(!this->lineInfo[i].nHeight)
			lineInfo[i].nHeight=nMinHeight;
		nTextHeight+=lineInfo[i].nHeight;
	}
	MATRIX lmat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(lmat);
	nTextHeight = (float)nTextHeight*fixed_1/lmat.d;
	nTextWidth = (float)nTextWidth*fixed_1/lmat.a;
}



//extern RColor* CreateClipColor(DisplayList* display, RColor** colorList);
// Helper for BuildEdges
/*RColor* CreateEditTextColor(int red,
							int green,
							int blue,
							int alpha,
							int layer,
							STransform* x,
							SObject* obj,
							EditText*edit)
{
	if ( obj->clipDepth )
	{
		RColor* clipColor = XNULL;
		RColor**colorList=&obj->colors;
		if(*colorList&&(*colorList)->colorType == colorClip)
			clipColor=*colorList;
		else clipColor=CreateClipColor(obj->display, colorList);


		return clipColor;
		//return NULL;
	}

	RColor* color = obj->display->CreateColor();

	if ( !color )
	{
		return NULL;
	}

	color->SetUp(obj->display->pRaster);//&obj->display->raster);
	color->nextColor = obj->colors;
	obj->colors = color;

	color->order = layer;
	FLASHASSERT(color->order < 0x10000);

	color->rgb.alpha = alpha;
	color->rgb.red = red;
	color->rgb.green = green;
	color->rgb.blue = blue;

	if ( x->cxform.HasTransform() )
	{
		x->cxform.Apply(&color->rgb);
		if(edit&&edit->IsEditAble())
			color->rgb.alpha=0xff;
	}

	color->transparent = PreMulAlpha(&color->rgb);

	if ( obj->display->pRaster->bits)//obj->display->raster.bits )
	{
		color->BuildCache();
	}

	return color;
}*/

// Helper for BuildEdges
/*void StrokeCursor(SObject* obj,
				  P_MATRIX mat,
				  int cursorX,
				  int cursorTop,
				  int cursorBottom,
				  RColor* color,
				  SStroker* stroker)
{
	// Transform cursorTop and cursorBottom into global space
	SPOINT topLeft;
	topLeft.x = cursorX;
	topLeft.y = cursorTop;
	MatrixTransformPoint(mat, &topLeft, &topLeft);

	SPOINT bottomRight;
	bottomRight.x = cursorX;
	bottomRight.y = cursorBottom;
	MatrixTransformPoint(mat, &bottomRight, &bottomRight);

	stroker->BeginStroke(1, color);

	CURVE c;
	CurveSetLine(&topLeft, &bottomRight, &c);
	stroker->AddStrokeCurve(&c);
	stroker->EndStroke();
}*/

// FindGlyph locates the glyph for a particular character code in a font
// and returns the glyph index
U16 EditText::FindGlyph(U16 targetCode, FontDesc& fontDesc)
{
	SCharacter* font = fontDesc.font;

	//if(!font->data) 
	//	return INVALID_CODE;
	if(font->font.deviceState==textDync)
	{
		//XU16*codeTable=font->font.deviceState==textDync?font->font.GetCodes():(XU16*)(font->data + fontDesc.codeOffset);
		//XSortConstU16 sort(codeTable,font->font.nGlyphs);
		//int id=sort.Index((void*)(XU32)targetCode);
		//if(id>=0) return id;
		return targetCode;
		//return INVALID_CODE;
	}	
	//return -1;
	//XU16*codeTable=font->font.deviceState==textDync?font->font.GetCodes():(XU16*)(font->data + fontDesc.codeOffset);
	
	U8 *codeTable = font->data + fontDesc.codeOffset;
	if(!codeTable) return INVALID_CODE;
	// Find the glyph for this character using binary search
	int lo = 0;
	int hi = font->font.nGlyphs-1;
	if ( font->font.flags & sfontFlagsWideCodes )
	{
		while ( lo <= hi )
		{
			int pivot = (lo+hi)>>1;
			U16 testCode = (U16)codeTable[pivot*2] | ((U16)codeTable[pivot*2+1]<<8);
			if ( testCode == targetCode )
			{
				return pivot;
			}
			else if ( targetCode < testCode )
			{
				hi = pivot-1;
			}
			else
			{
				lo = pivot+1;
			}
		}
	}
	else
	{
		while ( lo <= hi )
		{
			int pivot = (lo+hi)>>1;
			if ( codeTable[pivot] == targetCode )
			{
				return pivot;
			}
			else if ( targetCode < codeTable[pivot] )
			{
				hi = pivot-1;
			}
			else
			{
				lo = pivot+1;
			}
		}
	}
	return INVALID_CODE;
}

// Converts an array of character codes to an array of glyph indices into
// a particular font.  Missing characters are returned as INVALID_CODE.
U16 * EditText::FindGlyphs(U16 *buffer, int length, FontDesc& fontDesc)
{
	if ( !length )
	{
		return NULL;
	}
	// Convert character codes to glyph indices
	U16* glyphs = (U16*)XXVar::_AllocData(length*sizeof(U16));
		//new U16[length];
	U16* dst = glyphs;
	while ( length-- )
	{
		*dst++ = FindGlyph(*buffer++, fontDesc);
	}
	return glyphs;
}

static int MakeFontID(char*fontName)
{
	int id=100;
	if(!fontName) return id;
	while(*fontName)
	{
		id++;
		id+=*fontName;
		fontName++;
	}
	return id;
}

void EditText::CalcLineInfo(STransform*xt)
{
	//SETMATRIX(xt->mat);
	//return XTRUE;
	//MATRIX cameraMat;
	//XSWFPlayer::m_pInstance->display.GetCameraMatrix(cameraMat);
	//cameraMat.tx = cameraMat.ty = 0;
	//::MatrixInvert(&cameraMat,&cameraMat);

//	BwShapeRecord*pRec = &charObj->character->record;
//	pRec->ReleaseAll();
	//BwShapeRecord::ReleaseShapeList(pRec->pFirst);
	//pRec->pFirst = NULL;

//	GLShapeListData* pList = _XSObject::CreateShapeList(xt,clipObj,true);
//	GLShapeData* pLast = NULL;

	ScriptPlayer* player = m_character->splayer;
	XSWFPlayer* splayer = XSWFPlayer::m_pInstance;

	if ( splayer )
	{
		if ( !splayer->LoadBuiltInFonts() )
		{
			return;
		}
	}

	int startLine, nextLine, visibleLines;
	BOOL retval = XFALSE;
	U8 *advanceTable;
	SCharacter* font;
	//SRGB rgb;
	int i, lineno, y;
	//int devHeight, screenHeight;
	//RColor* whiteColor, *blackColor, *color, *backRColor,*filterColor;
	U8 *s;
	U16 *glyphs = NULL;
	U16 *buffer = NULL;
	MATRIX mat, screenmat;
	BOOL mayDrawCursor;
	int nRedF=0,nGreenF=0,nBlueF=0;
	int nFontID = 0;

	//if(this->m_length==7)
		//return 0;
	//	int vv=0;
	//XString8 tmp(_T("【"));
	//XString16 tmp16(tmp);
	//U16 lMargin=0, rMargin=0, indent=0, leading=0;
	//int devLMargin, devRMargin, devLeading, devCurrIndent;

	// Set up a stroker for any lines
	//SStroker sstroker(clipObj->display,&clipObj->edges);
	//SStroker* stroker = NULL;

	BOOL isFocus = player->splayer &&
				   player->splayer->focus &&
				   player->splayer->focus->character == m_character;

	mayDrawCursor = isFocus &&
					player->splayer->cursorBlink &&
					!(m_flags & seditTextFlagsReadOnly);

	mat = xt->mat;
	m_mat = xt->mat;
	MATRIX cameraMat,invMat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(cameraMat);
	//cameraMat.tx = cameraMat.ty = 0;
	::MatrixInvert(&cameraMat,&invMat);
	//invMat.tx = invMat.ty = 0;
	::MatrixConcat(&mat,&cameraMat,&mat);
	if(m_nDeltaY)
	{
		MATRIX matoff;
		::MatrixIdentity(&matoff);
		//mat.ty+=m_nDeltaY;
		matoff.ty=m_nDeltaY;
		::MatrixConcat(&matoff,&mat,&mat);
		//::MatrixConcat(&matoff,&m_mat,&m_mat);
		//m_mat.ty+=m_nDeltaY;
	}
	m_mat = mat;

	// Device font
	screenmat = mat;
	

	buffer = (U16*)XXVar::_AllocData((m_length+1)*sizeof(U16));
		//new U16[m_length + 1];

	if ( m_flags & seditTextFlagsPassword )
	{
		for ( i=0; i<m_length; i++ )
		{
			buffer[i] = PASSWORD_CHAR;
		}
		buffer[m_length] = 0;
	}
	else
	{
		if ( m_buffer )
		{
			WideStrCopy(buffer, m_buffer);
		}
		else
		{
			buffer[0] = 0;
		}
	}
	// Get font information
	FontDesc fontDesc;
	XGlobal::Memset(&fontDesc,0,sizeof(fontDesc));
	GetFontDesc(fontDesc,buffer);
	//m_height = fontDesc.height;
	font = fontDesc.font;
	XBOOL bFont3=(font&&font->tagCode==stagDefineFont3);
	SParser parser;
	parser.Attach(m_character->data, 0);

	// Skip flags
	parser.SkipBytes(2);

	_XTEXTFORMAT*pFormat=_GetFormat(m_pFormat,0);//m_pFormat->_GetFormat(0);
	XString8 strFont(_TOTALFONT);
	////strFont.ConvertFrom(_TOTALFONT,XCHARSET_GB2312),
	nFontID = MakeFontID(pFormat->font.strTxt);//strFont.GetData());//pFormat->font.strTxt);
	//if(!pFormat)
	//	pFormat=m_pFormat->_GetFormat(0);
	//if ( m_flags & seditTextFlagsHasFont )
	//{
	//	parser.SkipBytes(4);
	//}
	if ( m_flags & seditTextFlagsHasFont )
	{
		parser.SkipBytes(2);
	}

	if( m_flags & seditTextHasFontClass)
	{
		parser.GetStringP();
	}


	if ( m_flags & seditTextFlagsHasFont )
	{
		parser.SkipBytes(2);
	}

	if ( m_flags & seditTextFlagsHasTextColor )
	{
		//rgb =
		parser.GetColor(XTRUE);
	}

	//SRGB rgb=m_textColor;

	if ( m_flags & seditTextFlagsHasMaxLength )
	{
		parser.SkipBytes(2);
	}

	if ( m_flags & seditTextFlagsHasLayout )
	{
		parser.GetByte();
		//lMargin = parser.GetWord();
		//rMargin = parser.GetWord();
		//indent = parser.GetWord();
		//leading = parser.GetWord();
		parser.SkipBytes(8);
	}

	// Calculate widths of characters
	//delete [] devCharWidths;
	//delete [] devCharHeights;
	//devCharWidths = new int[m_length+1];
	//devCharHeights= new int[m_length+1];
	//delete charsInfo;
	XXVar::_FreeData(charsInfo);
	charsInfo= (_XCHARINFO*)XXVar::_AllocData((m_length+1)*sizeof(_XCHARINFO));
		//new _XCHARINFO[m_length+1];

	int iDevAscent;
	int iDescent,iLeading;
	//int layer;
	int screenHeight;
	int devAscent,devLineSpacing,nFilterFrame=0;
	SRECT devBounds;
//	SRGB rgb;
//
//	rgb.all=pFormat->color;
//	float color[]={rgb.rgb.red/255.0,
//				   rgb.rgb.green/255.0,
//				   rgb.rgb.blue/255.0,
//				   rgb.rgb.transparency/255.0};
//// Set up a new color
//	float backRColor[]={m_backColor.rgb.red/255.0,
//						m_backColor.rgb.green/255.0,
//						m_backColor.rgb.blue/255.0,
//						m_backColor.rgb.transparency/255.0};
//	float blackColor[] = {m_borderColor.rgb.red/255.0,
//						  m_borderColor.rgb.green/255.0,
//						  m_borderColor.rgb.blue/255.0,
//						  m_borderColor.rgb.transparency/255.0};
//		//0, 0, 0, 255, 2, x, m_obj);
//	float whiteColor[] = {(0xFF ^ m_borderColor.rgb.red)/255.0,
//						  (0xFF ^ m_borderColor.rgb.green)/255.0,
//						  (0xFF ^ m_borderColor.rgb.blue)/255.0,
//						  m_borderColor.rgb.transparency/255.0};
	screenHeight = FixedMul(pFormat->size<<16, screenmat.d) / fixed_1;
    

	int nScale=fixed_1;//,iFirst=0;
	if(screenmat.b==0&&
		   screenmat.c==0&&
		   screenmat.a>0&&
		   screenmat.d>0)
		   nScale=screenmat.a>screenmat.d?screenmat.d:screenmat.a;
   // MATRIX cmat;
	//splayer->display.GetCameraMatrix(cmat);
	//nScale = ((nScale>>8)*cmat.a)>>8;
    //screenHeight = FixedMul(pFormat->size << 16, screenmat.d) / fixed_1;
    //if ( !buildEdges )//&& !screenHeight )
    //{
    //    goto exit_gracefully;
    //}
    //layer=5;
//	int iLeading = 0;
	if ( font )
	{
		// Glyph outline font

		if(font->font.deviceState==textDync)
		{
			iDevAscent=800;//912;
			iDescent=112;
			iLeading=112;
			devAscent=iDevAscent*pFormat->size/1024;
			int descent=iDescent*pFormat->size/1024;
			int leading=iLeading*((int)pFormat->size)/1024;
			devLineSpacing = devAscent + descent + leading;//ADDLEAD// + (short)pFormat->leading;
			advanceTable=NULL;//(XU8*)font->font.GetAdvance();
		}
		else
		{
			parser.Attach(font->data, fontDesc.codeOffset);

			// Skip code tables
			if ( font->font.flags & sfontFlagsWideCodes )
			{
				parser.SkipBytes(font->font.nGlyphs * 2);
			}
			else
			{
				parser.SkipBytes(font->font.nGlyphs);
			}
			iDevAscent=parser.GetWord();
			iDescent=parser.GetWord();
			iLeading=(short)parser.GetWord();
			if(bFont3)
			{
				iDevAscent/=20;
				iDescent/=20;
				iLeading/=20;
			}
			
			if(m_character->type!=textChar)
					iLeading=0;

			devAscent = iDevAscent * pFormat->size / 1024;
			int descent = iDescent * pFormat->size / 1024;


			int leading = iLeading * ((int)pFormat->size) / 1024;//ADDLEAD
			devLineSpacing = devAscent + descent + leading;// +(short)pFormat->leading;
			// Skip leading
			//parser.SkipBytes(2);
			advanceTable = (U8*) (parser.script+parser.pos);
		}


	
		

		//devBounds = editRect;//m_character->bounds;
		MatrixTransformRect(&mat, &editRect, &devBounds);
		RectInset(2*onePoint, &devBounds);

		// Copy left, right margin, indent, leading.. no need to transform
		//devLMargin = pFormat->leftMargin;
		//devRMargin = pFormat->rightMargin;
		//devIndent  = pFormat->indent;
		//devLeading = pFormat->leading;

	}
	else
	{
		// Device font
		mat = screenmat;

		if ( !MatrixIsScaleOnly(&mat) )
		{
			goto exit_gracefully;
		}

		MatrixTransformRect(&mat, &editRect, &devBounds);
		//rgb = xt->cxform.Apply(rgb);

		devBounds = editRect;//m_character->bounds;
		RectInset(2*onePoint, &devBounds);
		//MatrixTransformRect(&mat, &devBounds, &devBounds);
	}


	//if ( buildEdges )
	{
		glyphs = FindGlyphs(buffer, m_length, fontDesc);

		for ( i=0; i<m_length; i++ )
		{
			//if(i==11)
			//	int v=0;
			//charsInfo[i].charPixel=0;
			if(i)
			{
				_XTEXTFORMAT*pNew=pFormat->_NextFormat(i,m_pFormat);
				if(pNew&&pFormat!=pNew)
				{
					pFormat=pNew;
					nFontID = MakeFontID(pFormat->font.strTxt);
					screenHeight = FixedMul(pFormat->size << 16, screenmat.d) / fixed_1;
					//if(font->font.deviceState==textDync)
					if(pFormat->spaceHeight)
					{
						devLineSpacing = pFormat->spaceHeight;
						devAscent	   = pFormat->spaceHeight;
					}
					else
					{
						devAscent=iDevAscent*pFormat->size/1024;
						int descent=iDescent*pFormat->size/1024;
						int leading = iLeading * ((int)pFormat->size) / 1024;//ADDLEAD
						devLineSpacing = devAscent + descent + leading;// + (short)pFormat->leading;
					}
					
				}
			}
			U16 code = glyphs[i];
			//U16 advance=0;
			float advance=0;
			float ns=0;
			if ( code == INVALID_CODE )
			{
				advance = 0;
			}
			else
			{
				//advance = (XU16)advanceTable[code*2] | ((XU16)advanceTable[code*2+1]<<8);
				//advance = advance * fontDesc.height / 1024;
				ns = (float)((pFormat->size*nScale)/fixed_1)/20;
				//if(ns<12) ns=12;
				XString8 strName;
				charsInfo[i].pTL = _XSObject::BuildFontChar(font,code,pFormat->font.strTxt/*strFont.GetData()*/,nFontID,ns);
				if(pFormat->spaceHeight)
				{
					advance=(float)pFormat->size*nScale/fixed_1;
				}
				else
				{
					if(!advanceTable)
					{
						if(charsInfo[i].pTL)
						{
							GLTranglesOrLines*pTL = (GLTranglesOrLines*)charsInfo[i].pTL;
							advance = (float)pTL->pChar->fw*ns*20/pTL->pChar->h;
						}
						else advance=(float)pFormat->fontSize/2*nScale/fixed_1;
					}
					else 
					{
						advance = (U16)advanceTable[code*2] | ((U16)advanceTable[code*2+1]<<8);
						if(bFont3)
							advance = (float)advance * pFormat->size / (1024*20);
						else
							advance = (float)advance * pFormat->size / 1024;
						advance=(float)advance*nScale/fixed_1;
					}
				}
			}
			charsInfo[i].charSize=ns*20;
			charsInfo[i].charWidth=advance;
			charsInfo[i].letterSpace=pFormat->letterSpacing;
			charsInfo[i].charCode=code;
			//charsInfo[i].charType=0;
			//charsInfo[i].charHeight=screenHeight;
			charsInfo[i].lineSpace=(float)devLineSpacing*nScale/fixed_1;
			charsInfo[i].pFormat=pFormat;
			
#ifdef DOTFONT
			//if(isDotShow)
			/*{
				int nFontWidth=0,nAdjust=0;
				int nPixel=WidthToDot(pFormat->size,nScale,nFontWidth,nAdjust);
				if(nPixel==12)
					charsInfo[i].charType=GLT_FONT12;
				else if(nPixel==16)
					charsInfo[i].charType=GLT_FONT16;
			}*/
#endif
			//devCharWidths[i] = advance;
			//devCharHeights[i]=

		}
	}
	


	pFormat=_GetFormat(m_pFormat,0);//m_pFormat->_GetFormat(0);
	//nFontID = GetFontID(pFormat->font.strTxt);
	iDevAscent = (float)iDevAscent*nScale/fixed_1;
	iDescent = (float)iDescent*nScale/fixed_1;


	CalculateLineStarts(mat,charsInfo, RectWidth(&devBounds),font?0:mat.a,iDevAscent,iDescent);//, pFormat->indent);


exit_gracefully:
	//delete [] glyphs;
	//delete [] buffer;
	//delete stroker;
	XXVar::_FreeData(glyphs);
	XXVar::_FreeData(buffer);
	editBounds = devBounds;
	//m_drawn = retval;

}



BOOL EditText::Draw(STransform* xt, BOOL buildEdges,_XSObject*clipObj,_XSObject*charObj,bool justUpdate)//bool bFilter)
{
	//return XTRUE;
	//MATRIX cameraMat;
	//XSWFPlayer::m_pInstance->display.GetCameraMatrix(cameraMat);
	//cameraMat.tx = cameraMat.ty = 0;
	//::MatrixInvert(&cameraMat,&cameraMat);
	//SETMATRIX(xt->mat);
	BwShapeRecord*pRec = &charObj->character->record;
	pRec->ReleaseAll();
	//BwShapeRecord::ReleaseShapeList(pRec->pFirst);
	//pRec->pFirst = NULL;

	GLShapeListData* pList = _XSObject::CreateShapeList(xt,clipObj,true);
	GLShapeData* pLast = NULL;

	ScriptPlayer* player = m_character->splayer;
	XSWFPlayer* splayer = XSWFPlayer::m_pInstance;

	if ( splayer )
	{
		if ( !splayer->LoadBuiltInFonts() )
		{
			return XFALSE;
		}
	}

	int startLine, nextLine, visibleLines;
	BOOL retval = XFALSE;
	U8 *advanceTable;
	SCharacter* font;
	//SRGB rgb;
	int i, lineno, y;
	//int devHeight, screenHeight;
	//RColor* whiteColor, *blackColor, *color, *backRColor,*filterColor;
	U8 *s;
	U16 *glyphs = NULL;
	U16 *buffer = NULL;
	MATRIX mat, screenmat;
	BOOL mayDrawCursor;
	int nRedF=0,nGreenF=0,nBlueF=0;
	int nFontID = 0;

	//if(this->m_length==7)
		//return 0;
	//	int vv=0;
	//XString8 tmp("赵子龙");
	//XString8 buf(m_buffer);
	//XString16 tmp16(tmp);
	//U16 lMargin=0, rMargin=0, indent=0, leading=0;
	//int devLMargin, devRMargin, devLeading, devCurrIndent;

	// Set up a stroker for any lines
	//SStroker sstroker(clipObj->display,&clipObj->edges);
	//SStroker* stroker = NULL;
	//if(m_length==4&&m_buffer[0]==36213)
	//	int v=0;
	BOOL isFocus = player->splayer &&
				   player->splayer->focus &&
				   player->splayer->focus->character == m_character;

	mayDrawCursor = isFocus &&
					player->splayer->cursorBlink &&
					!(m_flags & seditTextFlagsReadOnly);

	mat = xt->mat;
	m_mat = xt->mat;
	MATRIX cameraMat,invMat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(cameraMat);
	//cameraMat.tx = cameraMat.ty = 0;
	::MatrixInvert(&cameraMat,&invMat);
	//invMat.tx = invMat.ty = 0;
	::MatrixConcat(&mat,&cameraMat,&mat);
	if(m_nDeltaY)
	{
		MATRIX matoff;
		::MatrixIdentity(&matoff);
		//mat.ty+=m_nDeltaY;
		matoff.ty=m_nDeltaY;
		::MatrixConcat(&matoff,&mat,&mat);
		//::MatrixConcat(&matoff,&m_mat,&m_mat);
		//m_mat.ty+=m_nDeltaY;
	}
	m_mat = mat;

	// Device font
	screenmat = mat;
	

	buffer = (U16*)XXVar::_AllocData((m_length+1)*sizeof(U16));
		//new U16[m_length + 1];

	if ( m_flags & seditTextFlagsPassword )
	{
		for ( i=0; i<m_length; i++ )
		{
			buffer[i] = PASSWORD_CHAR;
		}
		buffer[m_length] = 0;
	}
	else
	{
		if ( m_buffer )
		{
			WideStrCopy(buffer, m_buffer);
		}
		else
		{
			buffer[0] = 0;
		}
	}
	// Get font information
	FontDesc fontDesc;
	XGlobal::Memset(&fontDesc,0,sizeof(fontDesc));
	GetFontDesc(fontDesc,buffer);
	//m_height = fontDesc.height;
	font = fontDesc.font;
	XBOOL bFont3=(font&&font->tagCode==stagDefineFont3);
	SParser parser;
	parser.Attach(m_character->data, 0);

	// Skip flags
	parser.SkipBytes(2);

	_XTEXTFORMAT*pFormat=_GetFormat(m_pFormat,0);//m_pFormat->_GetFormat(0);
	XString8 strFont(_TOTALFONT);
	////strFont.ConvertFrom(_TOTALFONT,XCHARSET_GB2312),
	nFontID = MakeFontID(pFormat->font.strTxt);//strFont.GetData());//pFormat->font.strTxt);
	//if(!pFormat)
	//	pFormat=m_pFormat->_GetFormat(0);
	//if ( m_flags & seditTextFlagsHasFont )
	//{
	//	parser.SkipBytes(4);
	//}
	if ( m_flags & seditTextFlagsHasFont )
	{
		parser.SkipBytes(2);
	}

	if( m_flags & seditTextHasFontClass)
	{
		parser.GetStringP();
	}


	if ( m_flags & seditTextFlagsHasFont )
	{
		parser.SkipBytes(2);
	}

	if ( m_flags & seditTextFlagsHasTextColor )
	{
		//rgb =
		parser.GetColor(XTRUE);
	}

	//SRGB rgb=m_textColor;

	if ( m_flags & seditTextFlagsHasMaxLength )
	{
		parser.SkipBytes(2);
	}

	if ( m_flags & seditTextFlagsHasLayout )
	{
		parser.GetByte();
		//lMargin = parser.GetWord();
		//rMargin = parser.GetWord();
		//indent = parser.GetWord();
		//leading = parser.GetWord();
		parser.SkipBytes(8);
	}

	// Calculate widths of characters
	//delete [] devCharWidths;
	//delete [] devCharHeights;
	//devCharWidths = new int[m_length+1];
	//devCharHeights= new int[m_length+1];
	//delete charsInfo;
	XXVar::_FreeData(charsInfo);
	charsInfo= (_XCHARINFO*)XXVar::_AllocData((m_length+1)*sizeof(_XCHARINFO));
		//new _XCHARINFO[m_length+1];

	int iDevAscent;
	int iDescent,iLeading;
	//int layer;
	int screenHeight;
	int devAscent,devLineSpacing,nFilterFrame=0;
	SRECT devBounds;
	SRGB rgb;

	rgb.all=pFormat->color;
	float color[]={rgb.rgb.red/255.0,
				   rgb.rgb.green/255.0,
				   rgb.rgb.blue/255.0,
				   rgb.rgb.transparency/255.0};
// Set up a new color
	float backRColor[]={m_backColor.rgb.red/255.0,
						m_backColor.rgb.green/255.0,
						m_backColor.rgb.blue/255.0,
						m_backColor.rgb.transparency/255.0};
	float blackColor[] = {m_borderColor.rgb.red/255.0,
						  m_borderColor.rgb.green/255.0,
						  m_borderColor.rgb.blue/255.0,
						  m_borderColor.rgb.transparency/255.0};
		//0, 0, 0, 255, 2, x, m_obj);
	float whiteColor[] = {(0xFF ^ m_borderColor.rgb.red)/255.0,
						  (0xFF ^ m_borderColor.rgb.green)/255.0,
						  (0xFF ^ m_borderColor.rgb.blue)/255.0,
						  m_borderColor.rgb.transparency/255.0};
	screenHeight = FixedMul(pFormat->size<<16, screenmat.d) / fixed_1;
    

	int nScale=fixed_1;//,iFirst=0;
	if(screenmat.b==0&&
		   screenmat.c==0&&
		   screenmat.a>0&&
		   screenmat.d>0)
			nScale=screenmat.a>screenmat.d?screenmat.d:screenmat.a;
   // MATRIX cmat;
	//splayer->display.GetCameraMatrix(cmat);
	//nScale = ((nScale>>8)*cmat.a)>>8;
    //screenHeight = FixedMul(pFormat->size << 16, screenmat.d) / fixed_1;
    if ( !buildEdges )//&& !screenHeight )
    {
        goto exit_gracefully;
    }
    //layer=5;
//	int iLeading = 0;
	if ( buildEdges&&font )
	{
		// Glyph outline font

		if(font->font.deviceState==textDync)
		{
			iDevAscent=1024;//912;
			iDescent=256;
			iLeading=256;
			devAscent=iDevAscent*pFormat->size/1024;
			int descent=iDescent*pFormat->size/1024;
			int leading=iLeading*((int)pFormat->size)/1024;
			devLineSpacing = devAscent + descent + leading;//ADDLEAD// + (short)pFormat->leading;
			advanceTable=NULL;//(XU8*)font->font.GetAdvance();
		}
		else
		{
			parser.Attach(font->data, fontDesc.codeOffset);

			// Skip code tables
			if ( font->font.flags & sfontFlagsWideCodes )
			{
				parser.SkipBytes(font->font.nGlyphs * 2);
			}
			else
			{
				parser.SkipBytes(font->font.nGlyphs);
			}
			iDevAscent=parser.GetWord();
			iDescent=parser.GetWord();
			iLeading=(short)parser.GetWord();
			if(bFont3)
			{
				iDevAscent/=20;
				iDescent/=20;
				iLeading/=20;
			}
			
			if(m_character->type!=textChar)
					iLeading=0;

			devAscent = iDevAscent * pFormat->size / 1024;
			int descent = iDescent * pFormat->size / 1024;


			int leading = iLeading * ((int)pFormat->size) / 1024;//ADDLEAD
			devLineSpacing = devAscent + descent + leading;// +(short)pFormat->leading;
			// Skip leading
			//parser.SkipBytes(2);
			advanceTable = (U8*) (parser.script+parser.pos);
		}


		if( m_flags&seditTextFlagsFillBackground)
		{
			_XSObject::AddFrame(editRect,xt->mat,backRColor,pList,pRec,true,&pLast);
			//AddRect(editRect,m_mat,backRColor,pList,pRec);
			//AddRect(&editRect, &m_mat, backRColor,
			//		clipObj->display, &clipObj->edges, NULL);
		}
		if ( m_flags & seditTextFlagsBorder )
		{
			//AddFrame(editRect,m_mat,blackColor,pList,pRec);
			_XSObject::AddFrame(editRect,xt->mat,blackColor,pList,pRec,false,&pLast);
			//AddRect(&editRect, &m_mat, blackColor,
			//		clipObj->display, &clipObj->edges, stroker);
		}
		

		//devBounds = editRect;//m_character->bounds;
		MatrixTransformRect(&mat, &editRect, &devBounds);
		RectInset(2*onePoint, &devBounds);

		// Copy left, right margin, indent, leading.. no need to transform
		//devLMargin = pFormat->leftMargin;
		//devRMargin = pFormat->rightMargin;
		//devIndent  = pFormat->indent;
		//devLeading = pFormat->leading;

	}
	else
	{
		// Device font
		mat = screenmat;

		if ( !MatrixIsScaleOnly(&mat) )
		{
			goto exit_gracefully;
		}

		MatrixTransformRect(&mat, &editRect, &devBounds);
		rgb = xt->cxform.Apply(rgb);

		devBounds = editRect;//m_character->bounds;
		RectInset(2*onePoint, &devBounds);
		//MatrixTransformRect(&mat, &devBounds, &devBounds);
	}


	if ( buildEdges )
	{
		glyphs = FindGlyphs(buffer, m_length, fontDesc);

		for ( i=0; i<m_length; i++ )
		{
			//if(i==11)
			//	int v=0;
			//charsInfo[i].charPixel=0;
			if(i)
			{
				_XTEXTFORMAT*pNew=pFormat->_NextFormat(i,m_pFormat);
				if(pNew&&pFormat!=pNew)
				{
					pFormat=pNew;
					nFontID = MakeFontID(pFormat->font.strTxt);
					screenHeight = FixedMul(pFormat->size << 16, screenmat.d) / fixed_1;
					//if(font->font.deviceState==textDync)
					if(pFormat->spaceHeight)
					{
						devLineSpacing = pFormat->spaceHeight;
						devAscent	   = pFormat->spaceHeight;
					}
					else
					{
						devAscent=iDevAscent*pFormat->size/1024;
						int descent=iDescent*pFormat->size/1024;
						int leading = iLeading * ((int)pFormat->size) / 1024;//ADDLEAD
						devLineSpacing = devAscent + descent + leading;// + (short)pFormat->leading;
					}
					
				}
			}
			U16 code = glyphs[i];
			//U16 advance=0;
			float advance=0;
			float ns=0;
			if ( code == INVALID_CODE )
			{
				advance = 0;
			}
			else
			{
				//advance = (XU16)advanceTable[code*2] | ((XU16)advanceTable[code*2+1]<<8);
				//advance = advance * fontDesc.height / 1024;
				ns = (float)((pFormat->size*nScale)/fixed_1)/20;
				//if(ns<12) ns=12;
				XString8 strName;
				charsInfo[i].pTL = _XSObject::BuildFontChar(font,code,pFormat->font.strTxt/*strFont.GetData()*/,nFontID,ns);
				if(pFormat->spaceHeight)
				{
					advance=(float)pFormat->size*nScale/fixed_1;
				}
				else
				{
					if(!advanceTable)
					{
						if(charsInfo[i].pTL)
						{
							GLTranglesOrLines*pTL = (GLTranglesOrLines*)charsInfo[i].pTL;
							advance = (float)pTL->pChar->fw*ns*20/pTL->pChar->h;
						}
						else advance=(float)pFormat->fontSize/2*nScale/fixed_1;
					}
					else 
					{
						advance = (U16)advanceTable[code*2] | ((U16)advanceTable[code*2+1]<<8);
						if(bFont3)
							advance = (float)advance * pFormat->size / (1024*20);
						else
							advance = (float)advance * pFormat->size / 1024;
						advance=(float)advance*nScale/fixed_1;
					}
				}
			}
			charsInfo[i].charSize=ns*20;
			charsInfo[i].charWidth=advance;
			charsInfo[i].letterSpace=pFormat->letterSpacing;
			charsInfo[i].charCode=code;
			//charsInfo[i].charType=0;
			//charsInfo[i].charHeight=screenHeight;
			charsInfo[i].lineSpace=(float)devLineSpacing*nScale/fixed_1;
			charsInfo[i].pFormat=pFormat;
			
#ifdef DOTFONT
			//if(isDotShow)
			/*{
				int nFontWidth=0,nAdjust=0;
				int nPixel=WidthToDot(pFormat->size,nScale,nFontWidth,nAdjust);
				if(nPixel==12)
					charsInfo[i].charType=GLT_FONT12;
				else if(nPixel==16)
					charsInfo[i].charType=GLT_FONT16;
			}*/
#endif
			//devCharWidths[i] = advance;
			//devCharHeights[i]=

		}
	}
	else
	{
		// Calculate device character widths
		int oldWidth = 0;
		for(i=0;i<m_length;i++)
		{
			charsInfo[i].charWidth=0;
			//charsInfo[i].charHeight=0;
			charsInfo[i].letterSpace=0;
			charsInfo[i].lineSpace=0;
			charsInfo[i].pFormat=0;
			//if((XU16)buffer[i]<0x80)
			//	devCharWidths[i]=0;//

		}
	}


	pFormat=_GetFormat(m_pFormat,0);//m_pFormat->_GetFormat(0);
	//nFontID = GetFontID(pFormat->font.strTxt);
	iDevAscent = (float)iDevAscent*nScale/fixed_1;
	iDescent = (float)iDescent*nScale/fixed_1;

	CalculateLineStarts(mat,charsInfo, RectWidth(&devBounds),buildEdges&&font?0:mat.a,iDevAscent,iDescent);//, pFormat->indent);
	if(justUpdate)
		goto exit_gracefully;
	visibleLines = CalcVisibleLines(m_vscroll);


	if ( m_doFindCursor&&!clipObj->clipDepth )
	{
		int ox=m_vscroll,oy=m_hscroll;
		FindCursor(&devBounds, charsInfo);//devCharWidths);
		m_doFindCursor = XFALSE;
		if(ox!=m_vscroll||oy!=m_hscroll)
		{
			if(m_obj->pASObject)
				m_obj->AS3_Onscroll();
			else
				m_obj->OnEvent(0,m_obj->display->player,XOBJEVENT_ONSCROLLER,XNULL);
		}
	}

	{
		y = devBounds.ymin;

		// Skip any lines that are scrolled out of view
		lineno = m_vscroll;
		if ( lineno > m_numLines )
		{
			lineno = m_numLines - 1;
		}

		startLine = lineInfo[lineno].nStart;//m_lineStarts[lineno];

		MATRIX charMat;
		MatrixIdentity(&charMat);

		if(bFont3)//font&&font->tagCode==stagDefineFont3)
		{
			//charMat.ty = devBounds.ymin + lineInfo[lineno].nHeight;//devAscent/20;
			charMat.a = charMat.d = pFormat->size * (fixed_1/(1024*20));
		}
		else
		{
			//charMat.ty = devBounds.ymin + lineInfo[lineno].nHeight;//devAscent;
			charMat.a = charMat.d = pFormat->size * (fixed_1/1024);
		}

		//delete [] m_lineX;
		//m_lineX = new int[m_numLines+1];
		for ( i=0; i<=m_numLines; i++ )
		{
			lineInfo[i].x=0;
		}

		//devCurrIndent = devIndent;

		//int nexty;
		pFormat=_GetFormat(m_pFormat,0);//m_pFormat->_GetFormat(0);
//		nFontID = GetFontID(pFormat->font.strTxt);
		while ( --visibleLines >= 0 )
		{
			devLineSpacing=lineInfo[lineno].nHeight;

			int dh=charsInfo[startLine].lineSpace;

			if(dh<devLineSpacing)
			{
					charMat.ty=y+devLineSpacing-(devLineSpacing-dh)*iDescent/1024;
						//(iDescent*pFmt->size iDescent*pFmt->size;
			}
			else charMat.ty=y+devLineSpacing;//-lineInfo[lineno].nDescent;//charsInfo[startLine].lineSpace;
			_XLINEINFO&info=lineInfo[lineno];
			//int iOff=info.nStart;
			//while(glyphs[startLine+iOff]INVALID_CODE
			//for(int iOff=0;iOff<info.l
			//_XTEXTFORMAT*pFmt=m_pFormat->_GetFormat(info.nStart);
			
			int cursorTop = y;
			int cursorBottom = y + info.nHeight;//devHeight;
			int selectTop = y;
			int selectBottom = y + info.nHeight;//devHeight;

			// Find the next line
			nextLine = lineInfo[min(m_numLines, lineno+1)].nStart;

			float x = devBounds.xmin + info.lMargin+info.indent;//devLMargin + devCurrIndent;
			//devCurrIndent = 0;

			int len = nextLine - startLine;

			for ( i=0;i<len;i++)
			{
				if(glyphs[startLine+i]!=INVALID_CODE)
				{
					_XTEXTFORMAT*pFmt=charsInfo[info.nStart+i].pFormat;//m_pFormat->_GetFormat(info.nStart+i);
					if(pFmt!=pFormat)
					{
						if(pFmt->color!=pFormat->color)
						{
							rgb.all=pFmt->color;
							color[0] = rgb.rgb.red/255.0;
							color[1] = rgb.rgb.green/255.0;
							color[2] = rgb.rgb.blue/255.0;
							color[3] = rgb.rgb.transparency/255.0;
						}
						if(bFont3)//font&&font->tagCode==stagDefineFont3)
						{
							//charMat.ty = devBounds.ymin + lineInfo[lineno].nHeight;//devAscent/20;
							charMat.a = charMat.d = pFmt->size * (fixed_1/(1024*20));
						}
						else
						{
							//charMat.ty = devBounds.ymin + lineInfo[lineno].nHeight;//devAscent;
							charMat.a = charMat.d = pFmt->size * (fixed_1/1024);
						}
						pFormat=pFmt;
//						nFontID = GetFontID(pFormat->font.strTxt);
					}
					break;
				}
			}
			// Don't render the newline
			if ( len )
			{
				U16 endChar = buffer[startLine+len-1];
				if ( endChar == editTextNewline || endChar == editTextSoftNewline )
				{
					len--;
				}
				//if ( endChar == editTextNewline )
				//{
				//	devCurrIndent = devIndent;
				//}
			}

			if ( m_hscroll > len )
			{
				len = 0;
			}
			else
			{
				len -= m_hscroll;
				startLine += m_hscroll;
			}

			// Compute the X coordinate of each character
			float *charX = (float*)XXVar::_AllocData((len+1)*sizeof(float));
				//new int[len + 1];



			for ( i=0; i<len; i++ )
			{
				charX[i] = x;
				float charWidth = charsInfo[startLine+i].charWidth+charsInfo[startLine+i].letterSpace;//pFormat->letterSpacing;//devCharWidths[startLine+i];
				if ( !(m_flags & seditTextFlagsWordWrap) )
				{
					// If word wrap is on then let CalculateLineStarts
					// decide on line breaks
					if ( i&&x /*+ charWidth*/ > devBounds.xmax - info.rMargin+onePoint*2+10 )
					{
						break;
					}
				}
				x += charWidth;
			}
			charX[i] = x;
			len = i;

			float xOffset = 0;
			float nCenterX = devBounds.xmin,nCenterY=devBounds.ymin;
			switch ( pFormat->align )
			{
			case stextAlignCenter:
				xOffset = (devBounds.xmax - devBounds.xmin - (charX[len] - charX[0]))/2;
				nCenterX = (devBounds.xmin+devBounds.xmax)/2.0f;
				nCenterY = (devBounds.ymin+devBounds.ymax)/2.0f;
				break;
			case stextAlignRight:
				xOffset = devBounds.xmax - devBounds.xmin - (charX[len] - charX[0]);
				nCenterX = (devBounds.xmax);
				nCenterY = (devBounds.ymax);
				break;
			}
			if ( xOffset )
			{
				for ( i=0; i<=len; i++ )
				{
					charX[i] += xOffset;
				}
			}

			//m_lineX[lineno] = charX[0];
			info.x=charX[0];

			lineno++;

			int A, B;
			if ( m_selectionStart < m_selectionEnd )
			{
				A = m_selectionStart;
				B = m_selectionEnd;
			}
			else
			{
				A = m_selectionEnd;
				B = m_selectionStart;
			}
			A -= startLine;
			B -= startLine;

			if ( B > len )
			{
				// Selection extends beyond this line
				selectBottom = y + devLineSpacing;
			}

			A = max(0, min(A, len));
			B = max(0, min(B, len));
			MATRIX m;
			::MatrixIdentity(&m);
			//m.ty-=(devLineSpacing*4096)/(cameraMat.d>>4);
			if ( buildEdges )
			{
				
				if ( A != B )
				{
					
					SRECT rect;
					rect.xmin = (int)(charX[A]+0.5);
					rect.ymin = selectTop+80;
					rect.xmax = (int)(charX[B]+0.5);
					rect.ymax = selectBottom+80;
					_XSObject::AddFrame(rect,invMat,blackColor,pList,pRec,true,&pLast);
					/*AddRect(&rect, &m_mat, blackColor,
							m_obj->display, &clipObj->edges, NULL);*/
				}
				for ( i=0; i<len; i++ )
				{
					_XTEXTFORMAT*pFmt=charsInfo[startLine+i].pFormat;
						//i?pFormat->_NextFormat(startLine+i,m_pFormat):m_pFormat->_GetFormat(startLine+i);
					if(pFmt!=pFormat)
					{
						if(pFmt->size!=pFormat->size)
						{

							int dh=charsInfo[startLine+i].lineSpace;

							if(dh<devLineSpacing)
							{
								charMat.ty=y+devLineSpacing-(devLineSpacing-dh)*iDescent/1024;//-lineInfo[startLine+1].nDescent;
									//(iDescent*pFmt->size iDescent*pFmt->size;
							}
							else
								charMat.ty=y+devLineSpacing;//-lineInfo[lineno].nDescent;
								//charMat.ty=y+dh;//(dh+devLineSpacing)/2;//nHeiht;
							charMat.a=charMat.d=charsInfo[startLine+i].charWidth;
							if(bFont3)
							{
								charMat.a = charMat.d = pFmt->size * (fixed_1/(1024*20));
								//charMat.ty = devBounds.ymin + devAscent/20;

							}
							else
							{
								//charMat.ty = devBounds.ymin + devAscent;
								charMat.a = charMat.d = pFmt->size * (fixed_1/(1024));
							}
						}
						if(pFmt->color!=pFormat->color)
						{
							rgb.all=pFmt->color;
							color[0] = rgb.rgb.red/255.0;
							color[1] = rgb.rgb.green/255.0;
							color[2] = rgb.rgb.blue/255.0;
							color[3] = rgb.rgb.transparency/255.0;
							//rgb = xt->cxform.Apply(rgb);
						}
						pFormat=pFmt;
//						nFontID = GetFontID(pFormat->font.strTxt);
					}
					U16 g = glyphs[startLine+i];
					if ( g != INVALID_CODE )
					{
						
#ifdef DOTFONT
						/*if(font->font.deviceState==textDync)
						{
							if(charsInfo[startLine+i].charType==GLT_FONT12)
								cf = &XSWFPlayer::m_pInstance->dotFont12;
							else if(charsInfo[startLine+i].charType==GLT_FONT12)
								cf = &XSWFPlayer::m_pInstance->dotFont16;
						}*/
#endif
						GLTranglesOrLines*pTL = (GLTranglesOrLines*)charsInfo[startLine+i].pTL;
							//_XSObject::BuildFontChar(cf,g,pFormat->font.strTxt,charsInfo[startLine+i].pixelWidth);
						if(pTL)
						{
							
							if(pTL->type==GLT_FONT)
							{	
								if(font->font.deviceState==textDync)
								{
									m.b=m.c=0;
									m.tx = (int)(charX[i]+0.5);//+m_mat.tx;
									m.ty = charMat.ty;//+m_mat.ty;
									m.ty -= charsInfo[startLine+i].lineSpace;//*charsInfo[startLine+i].charWidth/pTL->pChar->fw;
									m.a = m.d = (float)fixed_1*charsInfo[startLine+i].charWidth/pTL->pChar->fw;//((fixed_1<<14)/(cameraMat.a>>2));
								}
								else
								{
									//charMat.tx = (float)charX[i];
									m.b=m.c=0;
									m.tx = (int)(charX[i]+0.5);//+m_mat.tx;
									m.ty = charMat.ty;//+m_mat.ty;
									m.ty-=lineInfo[lineno].nDescent;
									//float sc=(float)charsInfo[startLine+i].charWidth*nScale/fixed_1/pTL->pChar->fw;
									float sc= (float)charsInfo[startLine+i].charSize/pTL->pChar->fw;
									m.a = m.d = (float)fixed_1*sc;//charsInfo[startLine+i].charSize/pTL->pChar->fw;//(float)fixed_1;
									//m=charMat;
									//m.a = (float)m.a*nScale/fixed_1;
									//m.d = m.a;
									//m.tx-=pTL->pChar->offx/20;
									//m.ty-=pTL->pChar->offy/20;
									//m.tx = (float)(m.tx-nCenterX)*nScale/fixed_1+nCenterX;
									//m.ty = m.ty+lineInfo[lineno].nDescent-iDescent*sc;
									float fs=(float)charsInfo[startLine+i].charSize/1024;
									if(font->tagCode==stagDefineFont3)
									{
										//m.ty = (float)(m.ty-nCenterY-charsInfo[i].lineSpace/2)*nScale/fixed_1+nCenterY+charsInfo[i].lineSpace/2;
										m.ty += (float)pTL->pChar->offy/20*fs;
										//m.ty -= charsInfo[startLine+i].lineSpace;
										//m.ty -= (float)pTL->pChar->offy*fs;
										m.tx += (float)pTL->pChar->offx/20*fs;//*fixed_1/nScale/20*sc;
									}
									else
									{
										//m.ty = (float)(m.ty-nCenterY-charsInfo[i].lineSpace)*nScale/fixed_1+nCenterY+charsInfo[i].lineSpace;
										//m.ty -= charsInfo[startLine+i].lineSpace;
										m.ty += (float)pTL->pChar->offy*fs;//fixed_1/nScale*sc;
										m.tx += (float)pTL->pChar->offx*fs;//fixed_1/nScale*sc;
									}
								}

							}
							else
							{
								charMat.tx = (int)(charX[i]+0.5);
								//::MatrixConcat(&charMat,&xt->mat,&m);
								//MATRIX mm=m_mat;
								//mm.tx = 0;
								//mm.ty = 0;
								//::MatrixConcat(&charMat,&mat,&m);
								m=charMat;
								m.a = (float)m.a*nScale/fixed_1;
								m.d = m.a;
								//m.tx = (float)(m.tx-nCenterX)*nScale/fixed_1+nCenterX;
								//m.ty = (float)(m.ty-nCenterY-charsInfo[i].lineSpace)*nScale/fixed_1+nCenterY+charsInfo[i].lineSpace;
								//m=xt->mat;
							}
							::MatrixConcat(&m,&invMat,&m);
							GLShapeData* pData = _XSObject::CreateShapeData(&m,( i >= A && i < B )?whiteColor:color,pTL);
							//cf->font.shapeData[g]->pChar->nCount++;
							_XSObject::GLUseChar(pTL->pChar);
							
							_XSObject::GLSetShapeListData(pList,pData,&pLast);
						}
					}
				}

				
			}
			

			// If edit text has the focus, then maybe draw the cursor
			if ( mayDrawCursor )
			{
				int cursorPos = m_selectionEnd-startLine;
				BOOL drawIt = (cursorPos >= 0 && cursorPos < len);
				if ( cursorPos == len && (m_atLineEnd || m_selectionEnd >= m_length-1 || len == 0 || buffer[startLine+len] == editTextNewline) )
				{
					drawIt = XTRUE;
				}
				if ( drawIt )
				{
					int cursorX = (int)(charX[cursorPos]+0.5);
					SPOINT screenPt;
					if ( buildEdges )
					{
						//StrokeCursor(clipObj, &m_mat, cursorX, cursorTop, cursorBottom, color, stroker);
						SRECT c={cursorX,cursorX+onePoint,cursorTop,cursorBottom };
								
						_XSObject::AddFrame(c,invMat,color,pList,pRec,true,&pLast);
						screenPt.x = cursorX;
						screenPt.y = cursorTop;
						//MatrixTransformPoint(&screenmat, &screenPt, &screenPt);
					}
					else
					{
						screenPt.x = cursorX;
						screenPt.y = cursorTop;
					}
					mayDrawCursor = XFALSE;
				}
			}

			//delete [] charX;
			XXVar::_FreeData(charX);

			if ( !buffer[startLine] )
			{
				break;
			}

			startLine = nextLine;
			y += devLineSpacing;
			//charMat.ty += devLineSpacing;
		}
	}

	if ( !(m_flags & seditTextFlagsNoSelect) )
	{
		// Auto-scroll
		AutoScroll();
	}

	// Signal success!
	retval = XTRUE;

exit_gracefully:
	//delete [] glyphs;
	//delete [] buffer;
	//delete stroker;
	XXVar::_FreeData(glyphs);
	XXVar::_FreeData(buffer);

	m_drawn = retval;
	editBounds = devBounds;
	return retval;
	//return XTRUE;
}

void EditText::TranslatePoint(int&lx,int&ly)
{
	MATRIX mat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(mat);
	/*mat.tx = 0;
	mat.ty = 0;
	SPOINT pt={lx,ly};
	::MatrixTransformPoint(&mat,&pt,&pt);
	lx = pt.x;
	ly = pt.y;*/
}

void EditText::GetEnabledCommands(int& flags)
{
	if ( m_flags & seditTextFlagsNoSelect )
	{
		// No commands permitted at all
		flags = 0;
	}
	else
	{
		// Select All is always permitted if selection isn't disabled
		flags = kSelectAllEnabled;

		BOOL selection = (m_selectionStart != m_selectionEnd);

		// Copy is permitted if a selection exists and not in password mode
		if ( selection && ((m_flags & seditTextFlagsPassword) == 0) )
		{
			flags |= kCopyEnabled;
		}

		if ( (m_flags & seditTextFlagsReadOnly) == 0 )
		{
			// Cut is enabled if Copy is enabled and not read only
			if ( flags & kCopyEnabled )
			{
				flags |= kCutEnabled;
			}
			// Clear is enabled if a selection exists and not read only
			if ( selection )
			{
				flags |= kClearEnabled;
			}
			// Paste is enabled if not read only and clipboard data exists
			// For simplicity, Paste is always enabled on UNIX
			flags |= kPasteEnabled;

		}
	}
}


XBOOL EditText::UpdateVariable(XXVar&v)//XPCTSTR v)
{
	XBOOL bChange=XFALSE;
	XSWFPlayer* splayer = m_obj->display->player;//m_character->player->splayer;
	//XSWFPlayer*player=s
	if ( splayer&&!m_variable.IsNull() )
	{
		//XXVar var;
		//XSWFPlayer*player=splayer->splayer;
		XSWFCONTEXT cnt;
		splayer->InitContext(cnt);
		XXVar var;
		//ScriptThread*thread=m_obj->thread->rootObject->thread;
		XXObject*obj=m_obj->thread->rootObject->pObject;
		cnt.InitContextA(XNULL,obj,obj,m_obj->pObject,m_obj->pObject,XNULL);
		if(splayer->GetVariable(cnt,m_variable.strTxt,var,XFALSE))
		{
			var.ToString(XFALSE);
			bChange=strcmp(v.strTxt,var.strTxt)!=0;
			if(bChange)
			{
				//var=v;
				splayer->SetVariable(cnt,m_variable,v,this);
			}
		}
		else bChange=XTRUE;

		splayer->ReleaseContext(cnt);

	}
	return bChange;
}

void EditText::UpdateToVariable(XSWFCONTEXT&cnt,XXVar&var)
{
	XSWFPlayer* splayer = m_obj->display->player;//m_character->player->splayer;
	//XSWFPlayer*player=s
	if ( splayer &&! m_variable.IsNull())
	{
		//XXVar var;
		//XSWFPlayer*player=splayer->splayer;
		//XSWFCONTEXT cnt;
		//splayer->InitContext(cnt);
		//XXVar var;
		//var.SetString((char*)strTxt);
		XXObject*obj=m_obj->thread->rootObject->pObject;
		cnt.InitContextA(cnt.pCaller,obj,obj,m_obj->pObject,m_obj->pObject,cnt.pWith);
		splayer->SetVariable(cnt,m_variable,var,XNULL);//XFALSE,obj);
		//var.nType=XODT_INT;
	}
}


void EditText::ClearEditVariable()
{
	if(m_obj->thread&&!m_variable.IsNull())
	{
		//if(strcmp(m_variable,"_parent._parent.percent")==0)
		//	int v=0;
		m_variable.Release();		
		XSWFPlayer* splayer = m_obj->display->player;//m_character->player->splayer;
		if(!splayer||!m_obj||!m_obj->thread||!m_obj->thread->rootObject) return;
		//if(m_obj->thread)
		{
			XXObject*thread=m_obj->thread->rootObject->pObject;
			XSWFCONTEXT cnt;
			splayer->InitContext(cnt);
			//cnt.contexts.Add(thread->thread);
			//cnt.thises.Add(thread->pObject);
			cnt.InitContextA(XNULL,thread,thread,m_obj->pObject,m_obj->pObject,XNULL);
			splayer->RemoveEditText(cnt,m_variable.strTxt,this);
			splayer->ReleaseContext(cnt);
		}
	}
}

void EditText::UpdateFromVariable(XBOOL bInit)//XSWFCONTEXT&cnt
{
	XSWFPlayer* splayer = m_obj->display->player;//m_character->splayer->splayer;
	//XSWFPlayer*player=s
	if ( splayer )
	{
		//XXVar var;
		//XSWFPlayer*player=splayer->splayer;
		if(!m_variable.IsNull())//m_variable&&m_variable[0])
		{

			XSWFCONTEXT cnt;
			splayer->InitContext(cnt);
			XXVar var;
			//ScriptThread*thread=m_obj->thread->rootObject->thread;
			XXObject*obj=m_obj->thread->rootObject->pObject;
			//cnt.contexts.Add(obj->thread);
			//cnt.thises.Add(obj->pObject);
			cnt.InitContextA(cnt.pCaller,obj,obj,m_obj->pObject,m_obj->pObject,cnt.pWith);
			//if(m_variable.strTxt[0]=='.')
			//	int v=0;
			//if(strcmp(m_variable,"_parent._parent.percent")==0)
			//		int v=0;
			if(splayer->GetVariable(cnt,m_variable.strTxt,var,XFALSE))
			{
				if(bInit)
				{
					cnt.InitContextA(cnt.pCaller,obj,obj,m_obj->pObject,m_obj->pObject,cnt.pWith);
					splayer->SetVariable(cnt,m_variable,var,this);
				}
				var.ToString(XFALSE);
				if(m_obj->GetPlayer()->version<=5)
				{
					XString8 str;
					str.ConvertFrom(var.strTxt,XCHARSET_GB2312);
					var.SetString(str.GetData(),str.GetLength());
				}
				SetBuffer(var.strTxt,false);
			}
			else if ( bInit )
			{
				// The variable is not yet set, set it from the initial value
				// in the edit text field.
				//char* txt="";
				//XString8 strTxt;
				XXVar strTxt=_strEmpty;
				if(!m_initialText.IsNull())//m_initialText)
				{  // strTxt=m_initialText;
					if(m_flags&seditTextFlagsHTML)
					{
						//TransHTMLText(m_initialText.strTxt,strTxt);
						//txt=strTxt;
						InitHTMLText(m_initialText.strTxt,m_initialText.GetLength());
					}
					else
					{
						SetBuffer(m_initialText.strTxt,false);
						strTxt=m_initialText;
					}
						//strTxt=m_initialText;
				}
				else
					SetBuffer(_strEmpty,false);//strTxt.strTxt);
				//XXVar var(txt);
				//var.SetString(txt);
				cnt.InitContextA(cnt.pCaller,obj,obj,m_obj->pObject,m_obj->pObject,cnt.pWith);

				splayer->SetVariable(cnt,m_variable,strTxt,this);
				//var.nType=XODT_INT;
				//thread->AddVariable(varName, m_initialText, XTRUE);
			}
			splayer->ReleaseContext(cnt);
		}
		else if ( !m_initialText.IsNull()&&bInit )
		{
			// The variable is not yet set, set it from the initial value
			// in the edit text field.
			//char* txt=m_initialText;
			//XString8 strTxt;
			XXVar strTxt=m_initialText;//(_strEmpty);
			if(m_flags&seditTextFlagsHTML)
			{
				//TransHTMLText(m_initialText.strTxt,strTxt);
				InitHTMLText(m_initialText.strTxt,m_initialText.GetLength());
				//txt=strTxt;
			}
			else SetBuffer(strTxt.strTxt,false);//txt);
			//thread->AddVariable(varName, m_initialText, XTRUE);
		}

		SaveFocus* saveFocus = &splayer->saveFocus;
		if ( !saveFocus->variable.IsNull() && m_obj->depth == saveFocus->depth &&
			 StrEqual(m_variable.strTxt, saveFocus->variable.strTxt) )
		{
			m_selectionStart = saveFocus->selectionStart;
			m_selectionEnd   = saveFocus->selectionEnd;
			m_selecting      = saveFocus->selecting;
			splayer->focus   = m_obj;
			m_caretIndex	 = m_selectionStart;
		}
	}
}

//
// SaveFocus object
//
void SaveFocus::SaveFocus()
{
	Clear();
}

void SaveFocus::Clear()
{
	variable        = 0;
	depth           = 0;
	selectionStart  = 0;
	selectionEnd    = 0;
	selecting       = XFALSE;
}

void SaveFocus::Save(SPlayer* splayer)
{
	if ( splayer && splayer->focus )
	{
		variable        = splayer->focus->editText->m_variable;
		depth           = splayer->focus->depth;
		selectionStart  = splayer->focus->editText->m_selectionStart;
		selectionEnd    = splayer->focus->editText->m_selectionEnd;
		selecting       = splayer->focus->editText->m_selecting;
	}
	else
	{
		Clear();
	}
}

void EditText::ReplaceSel(XPCTSTR strTxt)
{

	if(strTxt)
		Insert((char*)strTxt,strlen(strTxt));
	else
		DeleteSelection();
}

void EditText::ReplaceText(int ns, int ne, XPCTSTR strTxt)
{
	int start=m_selectionStart;
	int end=m_selectionEnd;
	if(ns<0) ns=0;
	if(ns>m_length) ns=m_length;
	if(ne<ns) ne=ns;
	if(ne>m_length) ne=m_length;
	m_selectionStart=ns;
	m_selectionEnd=ne;

	ReplaceSel(strTxt);

	m_selectionStart=min(start,m_length);
	m_selectionEnd=min(end,m_length);
	m_caretIndex = m_selectionStart;

}

void EditText::AutoSize(XBOOL b)
{
	if(b)
		m_flags|=seditTextAutoSize;
	else
		m_flags&=~seditTextAutoSize;
}

XBOOL EditText::TransHTMLText(XPCTSTR strTxt, XXVar &dst)
{
	if(m_obj)
	{
		ScriptPlayer*p=m_obj->GetPlayer();//character->player;
		if(p&&p->splayer)
		{
			XWindow*pWin=p->splayer->GetWindow();
			if(pWin->Handle(XCMD_PROCFLASHTEXT,(XU32)strTxt,(XU32)&dst))
			{
				//dst.ConvertToGB();
				return XTRUE;
			}
		}
		//XWindow*pWin=m_obj->character->player;
	}
	return XFALSE;
}

void EditText::SetHTMLText(XSWFCONTEXT*pCnt,XPCTSTR strTxt)
{
	/*if(m_initialText.IsNull())
	{
		if(m_flags1&seditTextDelInitText)
			delete m_initialText;
		m_initialText=XNULL;
	}*/
//#if (__CORE_VERSION__>=0x02079000)
	this->SetHtml(true);
//#endif
	m_initialText=_strEmpty;
	m_flags|=seditTextFlagsHTML;
	m_flags|=seditTextDelInitText;
	m_initialText=strTxt;//::CreateStr(strTxt);
	m_initialText.ToString(XTRUE);
	//if((strTxt[0]=='5'||strTxt[0]=='6')&&strTxt[1]=='0')
	//	int a = 0;
	//XXVar str;
	//TransHTMLText(strTxt,str);
	//SetBuffer(str.strTxt);
	InitHTMLText(m_initialText.strTxt,m_initialText.GetLength());
//#if (__CORE_VERSION__>=0x02077000)
	m_bModifyHTML=true;
//#else
//	m_bModifyHTML=false;
//#endif
	//XXVar var;
	//var.AttachString(str.DataTo());
	//var.SetString(str,str.GetLength());//,str.GetLength());
	//UpdateToVariable(cnt,str);
	//var.nType=XODT_INT;
}


void EditText::SetVariable(XXVar&var)//XPCTSTR str)
{
	m_variable=var;
	/*if(!m_variable.IsNull())
	{
		//if(m_flags1&seditTextDelVarText)
		//	delete m_variable;
		m_initialText=XNULL;
	}*/
	/*if(str&&str[0])
	{
		m_variable=CreateStr(str);
		UpdateFromVariable(XTRUE);
	}
	else
		m_variable=XNULL;*/
}


void EditText::SelectAll() {
	m_selectionStart = m_selectionEnd;
	// Bump selection end up to nearest word boundary
	U16 *ptr = m_buffer + m_selectionEnd;
	while (m_selectionEnd < m_length) {
		if (LineBreakOK_Index(m_selectionEnd) || *ptr == editTextNewline
				|| *ptr == editTextSoftNewline) {
			break;
		}
		ptr++;
		m_selectionEnd++;
	}
	// Back selection start down to nearest word boundary
	ptr = m_buffer + m_selectionStart;
	while (m_selectionStart > 0) {
		if (LineBreakOK_Index(m_selectionStart - 1) || *(ptr - 1)
				== editTextNewline || *(ptr - 1) == editTextSoftNewline) {
			break;
		}
		ptr--;
		m_selectionStart--;
	}
	m_caretIndex = m_selectionStart;
}


static void ProcItem(XDomItem*pItem,XString16&strOut,_XTEXTFORMAT*pFormat,DisplayList*disp,EditText*pEdit,XDom*pDom)
{
	int iType=pItem->GetID();
	//XBOOL bReturn=XFALSE;
	int iCount=pItem->GetChildCount();
	bool bRoot=pFormat->parent==XNULL;
//#if (__CORE_VERSION__>=0x02077000)
//#else
//	//[Note X]
//	{
//		switch(iType)
//		{
//		case XDOM_LI:
//		case XDOM_P:
//		case XDOM_DIV:
//		//case XDOM_SPAN:
//		case XDOM_H1:
//		case XDOM_H2:
//		case XDOM_H3:
//		case XDOM_H4:
//		case XDOM_H5:
//		case XDOM_H6:
//		//case XDOM_FONT:
//			 //if(!strOut.IsEmpty())
//			 if(!strOut.IsEmpty()&&iCount)
//				strOut+=editTextNewline;
//			// strOut+='\n';
//			 break;
//		}
//	}
//#endif
	if(iCount)
	{
		pFormat=pFormat->_NewChild(strOut.GetLength(),disp);
		if(bRoot)
			pFormat->leading=0;
		int v;
		if (pItem->GetAttrib(XEAB::ALIGN, v)||pItem->GetAttrib(XEAB::TEXT_ALIGN,v)) {
			switch (v) {
			case XEA::CENTER:
				pFormat->align = stextAlignCenter;
				break;
			case XEA::LEFT:
				pFormat->align = stextAlignLeft;
				break;
			case XEA::RIGHT:
				pFormat->align = stextAlignRight;
				break;
			default:
				pFormat->align = stextAlignJustify;
				break;
			}
		}
		if(pItem->GetAttrib(XEAB::COLOR,v))
		{
			U8 *p=(U8*)&v;
			//if(!v)
			//	pFormat->color=0xff000000;
			//else

			if(v==0)
			{
				pFormat->color=pEdit->m_pFormat->color;
			}
			else if(v!=0x00FFFFFF)
#ifdef _ARGB_PIXEL_
				pFormat->color=(p[2]<<16)|(p[1]<<8)|p[0]|(p[3]<<24);//|0xff000000;
#else
				pFormat->color=(p[0]<<16)|(p[1]<<8)|p[2]|(p[3]<<24);//|0xff000000;
#endif
		}
		if(pItem->GetAttrib(XEAB::SIZE,v)||pItem->GetAttrib(XEAB::FONT_SIZE,v))
		{
			if(v)
			{
				pFormat->size=v*20;
				//if(pFormat->size==0)
				//int v=0;
			}
		}
		XString8 strFace;
		if(pItem->GetAttrib(XEAB::FACE,strFace))
		{
			pFormat->font=strFace;
			pFormat->font.ToString(XTRUE);
		}
		if(pItem->GetAttrib(XEAB::HREF,strFace))
		{
			pFormat->url=strFace;
			pFormat->url.ToString(XTRUE);
		}
		if(pItem->GetAttrib(XEAB::XEVENT,strFace))
		{
			pFormat->txtEvent=strFace;
			pFormat->txtEvent.ToString(XTRUE);
		}
		if(pItem->GetAttrib(XEAB::TARGET,strFace))
		{
			pFormat->target=strFace;
			pFormat->target.ToString(XTRUE);
		}
		if(pItem->GetAttrib(XEAB::LETTERSPACING,v))
		{
			pFormat->letterSpacing=v*20;
		}
		//if (pItem->GetAttrib( XEAB::KERNING, v ) )
		//{
		//	pFormat->kerning = v * 20;
		//}
		if(pItem->GetAttrib(XEAB::BLOCKINDENT,v))
		{
			pFormat->blockIndent=v*20;
		}
		if(pItem->GetAttrib(XEAB::INDENT,v))
		{
			pFormat->indent=v*20;
		}
		if(pItem->GetAttrib(XEAB::LEADING,v))
		{
			pFormat->leading=v*20;
		}
		if(pItem->GetAttrib(XEAB::LEFTMARGIN,v))
		{
			pFormat->leftMargin=v*20;
		}
		if(pItem->GetAttrib(XEAB::RIGHTMARGIN,v))
		{
			pFormat->rightMargin=v*20;
		}
		//if(pItem->GetAttrib(XEAB::KER,
	}
	XString8 strFace;
	int		 nID=0;
	switch(iType)
	{
	case XDOM_IMG:
		 if(pItem->GetAttrib(XEAB::ID,nID))
		 {
			XCSS*pCss=pDom->m_pCSS?pDom->m_pCSS:&pDom->m_css;
		    const char*strID=pCss->GetIDName(nID);
			if(strID)
			{
				pFormat->strID=strID;
				pFormat->strID.ToString(XTRUE);
			}
		 }
		 if(pItem->GetAttrib(XEAB::SRC,strFace))
		 {
			 pFormat->strSrc=strFace;
			 pFormat->strSrc.ToString(XTRUE);
		 }
		 break;
	case XDOM_U:
		 pFormat->underline=true;
		 break;
	case XDOM_B:
		 pFormat->Bold=true;
		 break;
	case XDOM_I:
		 pFormat->italic=true;
		 break;
	case XDOM_P:
		pEdit->SetCondenseWhite(true);
		break;
	//case XDOM_TEXTAREA:
	//	 if(pEdit->IsMultiLine())
	//		strOut+=editTextNewline;
	//	 break;
//#if (__CORE_VERSION__>=0x02077000)
	case XDOM_SBR:
		 strOut+=editTextNewline;
		 return;
//#endif
	case XDOM_BR:
		 //strOut+='\r';
		 if(pEdit->IsMultiLine())
			strOut+=editTextNewline;
		 return;
		 break;
	case XDOM_TEXT:
		{
			XDomText*pText=(XDomText*)pItem;
			strOut+=pText->m_strTxt;
		}
		break;
//#if (__CORE_VERSION__>=0x02074000)
	case XDOM_SPACE:
		strOut+=XString16(" ");
		break;
//#endif
	}
	for(int i=0;i<iCount;i++)
	{
		XDomItem*pChild=pItem->GetChild(i);
		ProcItem(pChild,strOut,pFormat,disp,pEdit,pDom);
	}
	//if(pEdit->IsMultiLine())
	
	if(pFormat->parent)
		pFormat->iEnd=strOut.GetLength();

//#if (__CORE_VERSION__>=0x02073000)
	switch(iType)
	{
	case XDOM_P:
	case XDOM_TEXTAREA:
		 if(pEdit->IsMultiLine())
			strOut+=editTextNewline;
		 break;
	}
//#endif
	//if(bReturn)
	//	strOut+='\n';
}

inline static void AddText(XWAPText&wap,int iStart,int iEnd,EditText*edit)
{
	if(iEnd<=iStart) return;
//#if (__CORE_VERSION__>=0x02079000)
	if(iEnd-iStart==1&&iStart==edit->m_length-1)
	{
		if(edit->m_buffer[iStart]=='\r'||
		   edit->m_buffer[iStart]=='\n') return;
	}
	while(iStart<iEnd)
	{
		if(edit->m_buffer[iStart]!='\r'||
			edit->m_buffer[iStart]!='\n') break;
		iStart++;
	}
	if(iEnd<=iStart) return;
//#endif
	XString16 s16(edit->m_buffer+iStart,iEnd-iStart);
	XString8  s8(s16);
	wap.AddText(s8);
}

#define DIFF(FMT,PARENT,DATA)\
	(PARENT?(FMT->DATA!=PARENT->DATA):FMT->DATA)

static int AddFormat(_XTEXTFORMAT*pFmt,EditText*edit,XWAPText&wap,int iStart,_XTEXTFORMAT*parent)
{
	XU32 iEnd=pFmt->iEnd;
	if(iEnd>=edit->m_length)
		iEnd=edit->m_length;
	if(iStart<pFmt->iStart)
	{
		AddText(wap,iStart,pFmt->iStart,edit);
		iStart=pFmt->iStart;
	}
	if(iEnd>iStart)
	{
		int iTokens=0;
//#if (__CORE_VERSION__>=0x02077000)
		if(DIFF(pFmt,parent,align)||edit->m_buffer[iEnd]=='\r'||edit->m_buffer[iEnd]=='\n')//||parent==NULL)
			//pFmt->align!=stextAlignLeft||(parent&&pFmt->align!=parent->align))
		{
			//if(!parent||pFmt->align!=parent->align)
			{
				iTokens++;
				wap.AddToken(XDOM_P);
				switch(pFmt->align)
				{
				case stextAlignLeft:
					 wap.AddAttrib(XEAB::ALIGN,"left");
					 break;
				case stextAlignRight:
					 wap.AddAttrib(XEAB::ALIGN,"right");
					 break;
				case stextAlignCenter:
					 wap.AddAttrib(XEAB::ALIGN,"center");
					 break;
				}
			}
		}
//#endif
		if(DIFF(pFmt,parent,indent)||
		   DIFF(pFmt,parent,leading)||
		   DIFF(pFmt,parent,blockIndent)||
		   DIFF(pFmt,parent,leftMargin)||
		   DIFF(pFmt,parent,rightMargin))
		{
			{
				iTokens++;
				wap.AddToken(XDOM_TEXTFORMAT);
				if(DIFF(pFmt,parent,indent))
					wap.AddAttrib(XEAB::INDENT,pFmt->indent/20);
				if(DIFF(pFmt,parent,leading))
					wap.AddAttrib(XEAB::LEADING,pFmt->leading/20);
				if(DIFF(pFmt,parent,blockIndent))
					wap.AddAttrib(XEAB::BLOCKINDENT,pFmt->blockIndent/20);
				if(DIFF(pFmt,parent,leftMargin))
					wap.AddAttrib(XEAB::LEFTMARGIN,pFmt->leftMargin/20);
				if(DIFF(pFmt,parent,rightMargin))
					wap.AddAttrib(XEAB::RIGHTMARGIN,pFmt->rightMargin/20);
			}
		}
//#if (__CORE_VERSION__>=0x02077000)
//#else
//		if(DIFF(pFmt,parent,align))
//			//pFmt->align!=stextAlignLeft||(parent&&pFmt->align!=parent->align))
//		{
//			//if(!parent||pFmt->align!=parent->align)
//			{
//				iTokens++;
//				wap.AddToken(XDOM_P);
//				switch(pFmt->align)
//				{
//				case stextAlignLeft:
//					 wap.AddAttrib(XEAB::ALIGN,"left");
//					 break;
//				case stextAlignRight:
//					 wap.AddAttrib(XEAB::ALIGN,"right");
//					 break;
//				case stextAlignCenter:
//					 wap.AddAttrib(XEAB::ALIGN,"center");
//					 break;
//				}
//			}
//		}
//#endif
		if(DIFF(pFmt,parent,size)||
		   DIFF(pFmt,parent,color)||
		   DIFF(pFmt,parent,letterSpacing)/*||
		   DIFF(pFmt,parent,kerning)*/)
			//pFmt->size||pFmt->color||(pFmt->letterSpacing||(parent&&pFmt->letterSpacing!=parent->letterSpacing)))
		{
			//if(!parent||pFmt->size!=parent->size||pFmt->color!=parent->color||pFmt->letterSpacing!=pFmt->letterSpacing)
			iTokens++;
				wap.AddToken(XDOM_FONT);
			if(DIFF(pFmt,parent,color))
			{
				
					XString8 strColor="#";
					XU32 nColor=0;
					XU8*p=(XU8*)&pFmt->color;
					#ifdef _ARGB_PIXEL_
						nColor=(p[2]<<16)|(p[1]<<8)|p[0];//|(p[3]<<24);//|0xff000000;
					#else
						nColor=(p[0]<<16)|(p[1]<<8)|p[2];//|(p[3]<<24);//|0xff000000;
					#endif
					for(int i=0;i<6;i++)
					{
						int v=(nColor>>((5-i)*4))&0xf;
						if(v<10)
							strColor+=(char)('0'+v);
						else
							strColor+=(char)('A'+v-10);
					}
					//strColor.FromInt((pFmt->color&0xffffff),XTRUE);
					//strColor[0]='#';
					//strColor[1]='0';
					wap.AddAttrib(XEAB::COLOR,strColor);
				}
				if(DIFF(pFmt,parent,size))
				{
					/*if ()
						wap.AddAttrib(XEAB::SIZE,12);
					else
						wap.AddAttrib(XEAB::SIZE,pFmt->size/20);*/
					wap.AddAttrib(XEAB::SIZE,pFmt->size/20);
				}
				//if(pFmt->letterSpacing)
				//if(DIFF(pFmt,parent,letterSpacing))
					wap.AddAttrib(XEAB::LETTERSPACING,pFmt->letterSpacing/20);
				//if(!DIFF(pFmt,parent,kerning))
					//wap.AddAttrib(XEAB::KERNING,NULL);
			
		}
		if(!pFmt->url.IsNull()||!pFmt->txtEvent.IsNull())
		{
			if(!parent||pFmt->url.strTxt!=parent->url.strTxt||pFmt->txtEvent.strTxt!=pFmt->txtEvent.strTxt)
			{
				iTokens++;
				wap.AddToken(XDOM_A);
				if(!pFmt->url.IsNull())
					wap.AddAttrib(XEAB::HREF,pFmt->url.strTxt);
				if(!pFmt->txtEvent.IsNull())
					wap.AddAttrib(XEAB::XEVENT,pFmt->txtEvent.strTxt);
				if(!pFmt->target.IsNull())
					wap.AddAttrib(XEAB::TARGET,pFmt->target.strTxt);
			}

			//if(!pFmt->url.IsNull())
			//	wap.AddAttrib(XEAB::
		}
		if(DIFF(pFmt,parent,Bold))//pFmt->Bold)
		{
			//if(!parent||pFmt->Bold!=parent->Bold)
			{
				iTokens++;
				wap.AddToken(XDOM_B);
			}
		}
		if(DIFF(pFmt,parent,underline))//pFmt->underline)
		{
			//if(!parent||pFmt->underline!=parent->underline)
			{
				iTokens++;
				wap.AddToken(XDOM_U);
			}
		}

		if(iEnd>iStart)
		{
			if(!pFmt->child)
				AddText(wap,iStart,iEnd,edit);
			else
			{
				iStart=AddFormat(pFmt->child,edit,wap,iStart,pFmt);
				if(iStart<iEnd)
					AddText(wap,iStart,iEnd,edit);
			}
			iStart=iEnd;
		}

		for(int i=0;i<iTokens;i++)
			wap.EndToken();
	}
	   
	//pFmt=pFmt->next;
	//while(pFmt)
	//{
	//	iStart=AddFormat(pFmt,edit,wap,iStart,parent);
	//	pFmt=pFmt->next;
	//}
	if(pFmt->next)
		iStart=AddFormat(pFmt->next,edit,wap,iStart,parent);

	/*if(iStart<pFmt->iStart)
	{
		XString16 strTxt(edit->m_buffer+iStart,pFmt->iStart-iStart);
		XString8  s8(strTxt);
		wap.AddText(s8);
	}
	if(pFmt->iEnd>pFmt->iStart)
	{
		int iEnd=pFmt->iEnd;
		
		if(pFmt->child)
		{
			int iEnd=AddFormat(pFmt->child,edit,wap,iStart);
			if(iEnd<pFmt->iEnd)
			{
				XString16 strTxt(edit->m_buffer+iEnd,pFmt->iStart-iEnd);
				XString8  s8(strTxt);
				wap.AddText(s8);
			}
		}
		iStart=pFmt->iEnd;
	}*/
	return iStart;	
}

XBOOL EditText::UpdateHTMLText()
{
	if(!m_bModifyHTML) return XFALSE;
	XWAPText wap;
	wap.SetMode(XWAPText::XW_TEXT);
	int iStart=0;
	AddFormat(m_pFormat,this,wap,iStart,NULL);
	m_initialText=wap;
	m_initialText.ToString(XTRUE);
	//wap.m_pData
	m_bModifyHTML=false;
	return XTRUE;
}

static void OptFormat(_XTEXTFORMAT*&pFmt,DisplayList*display)
{
	//return;
	_XTEXTFORMAT*child=pFmt->child;
	if(child)
	{
		while(pFmt->iStart==child->iStart&&pFmt->iEnd==child->iEnd)
		{
			_XTEXTFORMAT*next=child->next;
			while(next)
			{
				_XTEXTFORMAT*pn=next->next;
				next->Free(display);
				//display->FreeFormat(next);
				next=pn;
			}
			child->next=pFmt->next;
			child->parent=pFmt->parent;
			pFmt->color=123;
			display->FreeFormat(pFmt);
			pFmt=pFmt->child;
			child=pFmt->child;
			if(!child) break;
		}
		if(child)
			OptFormat(pFmt->child,display);
	}
	if(pFmt->next)
		OptFormat(pFmt->next,display);
}

void EditText::InitHTMLText(const char*strTxt,int l)
{
	_XTEXTFORMAT*p=m_pFormat->child;
	while(p)
	{
		p->Free(m_obj->display);
		p=p->next;
	}
	if(m_pFormat->underline)
		int a = 0;
	if(m_pFormat->child&&m_pFormat->child->underline)
		int a = 0;
	m_pFormat->child=NULL;
	XDom dom;
	XCSS*pCSS=XNULL;
	if(m_obj->pASObject)
	{
		TextFieldObject*p=(TextFieldObject*)m_obj->pASObject;
		if(p->m_pSheet)
		{
			pCSS=&p->m_pSheet->m_css;
			dom.m_pCSS=pCSS;
		}
	}
	
    dom.Reset(XCHARSET_UTF8,XCNT::CTEXT,XCNT::TVND_WAP_WML,XNULL);
    dom.AddTag(new XDomNode(XDOM_WML));
    dom.AddTag(new XDomCard());
	//if(strstr(strTxt,"&lt;xml")!=NULL)
	//	int v=0;
    XBOOL bOK=dom.PhaseXML((XU8*)strTxt,l,true,true, IsCondenseWhite() );
	if(pCSS)
		dom.GetActiveCard()->SetCSS(pCSS);
	XDomItem*pItem=dom.GetActiveCard();
	XString16 strOut;
	int i,iCount=pItem->GetChildCount();
	for(i=0;i<iCount;i++)
	{
		XDomItem*pc=pItem->GetChild(i);
		if(pc)
			ProcItem(pc,strOut,m_pFormat,m_obj->display,this,&dom);
	}
	/*if(!IsMultiLine())
	{
		int l=strOut.GetLength()-1;
		XPCTSTR str=strOut;
		if(l>=0)
		{
			if(str[l]==editTextNewline)
				strOut[l]=0;
		}
	}*/
	SetBuffer(strOut,strOut.GetLength());
	/*if(m_pFormat->child&&!m_pFormat->child->next)
	{
		if(m_pFormat->child->iStart<=0&&
			m_pFormat->child->iEnd>=(XU32)m_length)
		{
			m_pFormat->Copy(*m_pFormat->child);
			m_pFormat->iStart=0;
			m_pFormat->iEnd=0xffffffff;
			m_obj->display->FreeFormat(m_pFormat->child);
			m_pFormat->child=m_pFormat->child->child;

		}
	}*/
	//if(m_length>8&&m_buffer[0]==91)
	//	int v=0;
	m_pFormat->iEnd=m_length;
	OptFormat(m_pFormat,m_obj->display);
	m_pFormat->iEnd=0xffffffff;
	/*l=strOut.GetLength();
	XU16*pData=(XU16*)strOut.GetData();
	while(l)
	{
		if(pData[l-1]==editTextNewHTMLLine)
			l--;
		else
		{
			pData[l]=0;
			strOut.SetSize(l+1);
			break;
		}
	}*/
	/*if(l>=2)
	{
		XU16*pData=strOut.GetData();
		if(pData[l-2]=='\n'&&
           pData[l-1]=='\n')
		{
			pData[l-2]=0;
			strOut.SetSize(l-1);
		}
	}*/
	
	SetURLFlag();
	m_bModifyHTML=false;
}


void EditText::UpdateIfNeed()
{
	if(lineInfo==NULL||m_obj->dirty)
	{
		STransform xx;
		if(m_obj->parent)
		{
			m_obj->parent->GetTotalMatrix(xx.mat,XFALSE);
			m_obj->parent->GetTotalCXForm(xx.cxform);
		}
		else
		{
			m_obj->GetTotalMatrix(xx.mat,XFALSE);
			m_obj->GetTotalCXForm(xx.cxform);
		}
		/*_XSObject*obj=NULL;
		_XSObject*p=m_obj->parent;
		while(p)
		{
			if(p->clipDepth)
			{
				obj=p;
				break;
			}
			p=p->parent;
		}*/
		m_obj->FreeCache();
		CalcLineInfo(&xx);
		//m_obj->BuildEdges(&xx,obj);

	}
}



void EditText::SetFormat(void*fmt,XU32 iStart,XU32 iEnd)
{
	if(iStart<0) iStart=0;
	if(iEnd<=iStart) iEnd=m_length;
	_XTEXTFORMAT*pNew=FindFormat(m_pFormat,iStart,iEnd);

	if(pNew->iStart==iStart&&pNew->iEnd==iEnd)
	{
		((TextFormatObject*)fmt)->GetFormat(pNew);
		SetFilterSize(pNew);
	}
	else
	{
		_XTEXTFORMAT**pp=&pNew->child;
		TextFormatObject*obj=(TextFormatObject*)fmt;
		while(iStart<iEnd)
		{
			_XTEXTFORMAT*p=*pp;
			if(!p||iEnd<p->iStart)
			{  //befor the start,create new format
				_XTEXTFORMAT*pCopy=m_obj->display->CreateFormat();
				pCopy->Copy(*pNew);
				obj->GetFormat(pCopy);
				pCopy->iStart=iStart;
				pCopy->iEnd=iEnd;
				pCopy->parent=pNew;
				pCopy->child=NULL;
				pCopy->next=p;
				*pp=pCopy;
				iStart=iEnd;
				break;
			}			
			if(p->iStart==iStart&&p->iEnd==iEnd)
			{   //equal and copy data
				obj->GetFormat(p);
				iStart=iEnd;
				break;
			}
			else if(p->iEnd>iStart)
			{   //has common area
				
				if(iStart<p->iStart)
				{   // like...
					// [.._____ new
					//    [______old,create new format before start
					_XTEXTFORMAT*pCopy=m_obj->display->CreateFormat();
					pCopy->Copy(*pNew);
					obj->GetFormat(pCopy);
					pCopy->iStart=iStart;
					pCopy->iEnd=p->iStart;
					pCopy->parent=pNew;
					pCopy->child=NULL;
					pCopy->next=p;
					*pp=pCopy;
				}
				else if(iStart>p->iStart)
				{
					//like ...
					//    [_______new
					// [..._______old,create new format befor start
					_XTEXTFORMAT*pCopy=m_obj->display->CreateFormat();
					pCopy->Copy(*p);
					pCopy->iStart=p->iStart;
					pCopy->iEnd=iStart;
					p->iStart=iStart;
					pCopy->parent=pNew;
					pCopy->child=NULL;
					pCopy->next=p;
					*pp=pCopy;
				}
				if(p->iEnd>iEnd)
				{
					//like ...
					// _____]		new
					// ______...]	old,create new format before end,finished
					_XTEXTFORMAT*pCopy=m_obj->display->CreateFormat();
					pCopy->Copy(*p);
					obj->GetFormat(pCopy);
					pCopy->iStart=iStart;
					pCopy->iEnd=iEnd;
					p->iStart=iEnd;
					pCopy->parent=pNew;
					pCopy->child=NULL;
					pCopy->next=p;
					*pp=pCopy;
					break;
				}
				iStart=p->iEnd;
			}
			//obj->GetFormat(p);
			pp=&p->next;
		}
		/*_XTEXTFORMAT*pCopy=m_obj->display->CreateFormat();//new _XTEXTFORMAT;
		pCopy->Copy(*pNew);
		((TextFormatObject*)fmt)->GetFormat(pCopy);
		SetFilterSize(pCopy);
		pCopy->iStart=iStart;
		pCopy->iEnd=iEnd;
		pCopy->parent=pNew;
		pCopy->next=XNULL;
		pCopy->child=XNULL;
		_XTEXTFORMAT**pp=&pNew->child;
		for(;;)
		{
			_XTEXTFORMAT*p=*pp;
			if(!p&&pCopy)
			{
				*pp=pCopy;
				break;
			}
			else
			{
				if(p->iStart>=iStart&&p->iEnd<=iEnd)
				{
					*pp=p->next;
					m_obj->display->FreeFormat(p);
					continue;
				}
				else if(p->iStart<iStart&&p->iEnd>iStart)
				{
					p->iEnd=iStart;
					if(pCopy)
					{
						pCopy->next=p->next;
						p->next=pCopy;
						pCopy=NULL;
						return;
					}
				}
				else if(p->iStart<iEnd&&p->iEnd>iEnd)
				{
					p->iStart=iEnd;
					if(pCopy)
					{
						*pp=pCopy;
						pCopy->next=p;
						pCopy=NULL;
						return;
					}
				}
			}
			pp=&p->next;
		}*/
	}
	m_obj->Modify();
	m_bModifyHTML=true;
}


void EditText::GetTextSize(double&w,double&h,bool bTrans)
{
	//LOGWHERE();
	UpdateIfNeed();
	//LOGWHERE();
	int i;//,iCount=this->CalcVisibleLines(m_vscroll);
	h=0,w=0;
	MATRIX mat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(mat);
	float scaley=(float)fixed_1/mat.d;
	float scalex=(float)fixed_1/mat.a;
	//LOGWHEREVAL(m_numLines);
	if( lineInfo != NULL )
	
	for(i=0;i<m_numLines;i++)
	{
		//LOGWHEREVAL(i);
		if(i==m_numLines-1)
		{
			if(lineInfo[i].nWidth<=0)
				break;
		}
		h+=lineInfo[i].nHeight*scaley;
		float wi=lineInfo[i].nWidth*scalex;
		if(w<wi)
			w=wi;
	}
	//LOGWHERE();

	//SRECT devBounds=editRect;
	if(h<0&&m_pFormat)
	{
		h=m_pFormat->size;
	}
	if(bTrans)
	{
		/*SRECT r={0,w,0,h};
		MATRIX mat;
		m_obj->GetTotalMatrix(mat,XFALSE);
		::MatrixTransformRect(&mat,&r,&r);
		w=RectWidth(&r)/20.0;
		h=RectHeight(&r)/20.0;*/
		w=w/20;
		h=h/20;
	}
	//LOGWHERE();
}

void EditText::GetCharBoundaries(int id,SRECT&rect)
{
	UpdateIfNeed();
	SRECT bounds=editRect;//m_character->bounds;
	int i,line=GetLineIndexOfChar(id);
	
	rect.ymin=0;//m_pFormat->leading;//bounds.xmin;
	rect.xmin=lineInfo[line].x;//bounds.ymin;
	
	{
		int nHeight=0,nWidth=0,nSpace=0;
		for(i=0;i<line;i++)
		{
			nHeight+=lineInfo[i].nHeight;//lineInfo[i].nPixelHeight;
		}
		rect.ymin+=nHeight;//(nHeight*m_nRevScale)>>16;
		rect.ymax=rect.ymin+lineInfo[line].nHeight;
		rect.ymin=rect.ymax-charsInfo[id].lineSpace;
		for(i=lineInfo[line].nStart;i<id;i++)
		{
			nWidth+=charsInfo[i].charWidth;//charsInfo[i].charPixel;
			//nSpace+=charsInfo[i].letterSpace;
			//if(charsInfo[i].charPixel)
			//	rect.xmin+=charsInfo[i].charPixel*20+charsInfo[i].letterSpace;
			//else
			//	rect.xmin+=charsInfo[i].charWidth+charsInfo[i].letterSpace;
		}
		rect.xmin+=nWidth+nSpace;//;((nWidth*m_nRevScale)>>16)+nSpace;
		rect.xmax=rect.xmin+charsInfo[i].charWidth+charsInfo[i].letterSpace;
	}

	rect.xmax/=20;
	rect.ymax/=20;
	rect.xmin/=20;
	rect.ymin/=20;
	//MATRIX mat,mat20;
	//m_obj->GetTotalMatrix(mat,FALSE);
	//::MatrixTransformRect(&mat,&rect,&rect);
	
}

int  EditText::GetLineIndexAtPoint(int lx,int ly)
{
	//lx=lx*onePoint;
	//ly=ly*onePoint;
	SRECT bounds;
	{
		bounds = editRect;//m_character->bounds;
		//RectInset(2*onePoint, &bounds);
	}

	if(lx<bounds.xmin||lx>=bounds.xmax||
		ly<bounds.ymin||ly>=bounds.ymax) return -1;
	UpdateIfNeed();
	int endLine=CalcVisibleLines(m_vscroll)+m_vscroll;

	// Find the line that the mouse is over
	int y=bounds.ymin,lineno = m_vscroll;//(localpt.y-bounds.ymin)/devLineSpacing;
	//lineno = max(0, min(lineno, visibleLines-1));
	MATRIX mat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(mat);
	float scale=(float)fixed_1/mat.d;
	while(lineno<endLine)
	{
		y+=lineInfo[lineno].nHeight*scale;
		if(y>ly) break;
		lineno++;
	}
	//lineno += m_vscroll;
	if ( lineno >= m_numLines )
	{
		return -1;
		//lineno = m_numLines;
	}
	return lineno;
}

int  EditText::GetCharIndextAtPoint(int lx,int ly)
{
    if ( lineInfo == NULL ) {
        return 0;
    }
	int lineno=GetLineIndexAtPoint(lx,ly);
	if(lineno<0)
        return lineno;
	//lx=lx*onePoint;
	//ly=ly*onePoint;
	SRECT bounds;
	{

		bounds = editRect;//m_character->bounds;
		RectInset(2*onePoint, &bounds);
	}

	int endLine=CalcVisibleLines(m_vscroll)+m_vscroll;

	// Find the line that the mouse is over
	int y=bounds.ymin;//(localpt.y-bounds.ymin)/devLineSpacing;

    
	int index = lineInfo[lineno].nStart;//m_lineStarts[lineno];
	int lineEnd = FindLineEnd(index);

	index += m_hscroll;
	if ( index > lineEnd )
	{
		index = lineEnd;
	}

	// Find the character in the text right before
	// the position of the mouse click
	MATRIX mat;
	XSWFPlayer::m_pInstance->display.GetCameraMatrix(mat);
	float scale=(float)fixed_1/mat.a;
	int x = lineInfo[lineno].x*scale;//m_lineX[lineno];
	for ( ; m_buffer[index]; index++ )
	{
		if ( index >= lineEnd )
		{
			return -1;
		}
		if ( lx < x+charsInfo[index].charWidth*scale/2)//devCharWidths[index]/2 )
		{
			break;
		}
		x += charsInfo[index].charWidth*scale;//devCharWidths[index];
		if ( x > bounds.xmax )
		{
			return -1;
		}
	}
	if(index>=m_length) return m_length-1;
	return index;
}
int  EditText::GetFirstCharInParagraph(int id)
{
	while(id>0)
	{
		id--;
		if(m_buffer[id]==editTextNewline||
			m_buffer[id]==editTextSoftNewline)
			return id+1;
	}
	return 0;
}
const char* EditText::GetImageRefrence(char* url)
{
	_XTEXTFORMAT*fmt=_FindFormat(m_pFormat,url);
	if(fmt) return fmt->strSrc.strTxt;
	return NULL;
	//return NULL;
}

int  EditText::GetLineIndexOfChar(int id)
{
	UpdateIfNeed();
	for(int i=0;i<m_numLines;i++)
	{
		if(id>=lineInfo[i].nStart&&
		   id<lineInfo[i+1].nStart) return i;
	}
	return 0;
}
int  EditText::GetLineLength(int i)
{
	UpdateIfNeed();
	if(i<0||i>=m_numLines) return -1;

	return lineInfo[i+1].nStart-lineInfo[i].nStart;
}
int	 EditText::GetLineOffset(int i)
{
	UpdateIfNeed();
	if(i<0||i>=m_numLines) return -1;
	return lineInfo[i].nStart;
}



U16* EditText::GetLineText(int&n)
{
	UpdateIfNeed();
	if(n<0||n>m_numLines) return XNULL;
	int l=n;
	n=lineInfo[n+1].nStart-lineInfo[n].nStart;
	return m_buffer+lineInfo[l].nStart;
}
int  EditText::GetPararaphLength(int id)
{
	if(id<0||id>=m_length) return -1;
	while(id)
	{
		if(m_buffer[id]==editTextNewline|| m_buffer[id]==editTextSoftNewline)
			break;
//#if (__CORE_VERSION__>=0x02081000)
		id++;
//#endif
	}
	int is=id+1;
	while(id<m_length)
	{
		if(m_buffer[id]==editTextNewline||
			m_buffer[id]==editTextSoftNewline)
		{
			break;
		}
		id++;
	}
	return id-is;
}
_XTEXTFORMAT* EditText::GetTextFormat(int b,int e)
{
	return _GetFormat(m_pFormat,b);//m_pFormat->_GetFormat(b);
	//return 0;
}
void EditText::replaceSelectText(U16* pText,int l)
{
	int b=m_selectionStart;
	int e=m_selectionEnd;
	int delta=l-(e-b);
	InsertWideChars(pText,l,false);
	ReplaceFormat(m_pFormat,b,e,delta,m_obj->display);
	m_obj->Modify();
	m_bModifyHTML=true;
}


void EditText::setSelection(int b,int e)
{
	m_selectionStart=b;
	m_selectionEnd=e;
	m_caretIndex = m_selectionStart;
	m_obj->Modify();
}

void EditText::AppendText(U16*pText,int l,_XTEXTFORMAT*pFmt)
{
	if(l<=0) return;
	_XTEXTFORMAT*pNew=m_obj->display->CreateFormat();
	pNew->child=NULL;
	pNew->Copy(*pFmt);
	pNew->iStart=m_length;
	pNew->iEnd=m_length+l;
	pNew->next=NULL;
	_XTEXTFORMAT*pChild=m_pFormat->child;
	if(pChild==NULL)
		m_pFormat->child=pNew;
	else
	{
		for(;;)
		{
			if(pChild->next==NULL)
			{
				pChild->next=pNew;
				break;
			}
			pChild=pChild->next;
		}
	}
	AppendText(pText,l);
}

_XTEXTFORMAT* EditText::AppendSpace(int nWidth,int nHeight)
{
	_XTEXTFORMAT*pNew=m_obj->display->CreateFormat();	
	pNew->iStart=m_length;
	pNew->iEnd=m_length+1;
	pNew->color=0;
	pNew->size=nWidth*20;
	pNew->spaceHeight=nHeight*20;
	pNew->child=NULL;
	pNew->next=NULL;
	_XTEXTFORMAT*pChild=m_pFormat->child;
	if(pChild==NULL)
		m_pFormat->child=pNew;
	else
	{
		for(;;)
		{
			if(pChild->next==NULL)
			{
				pChild->next=pNew;
				break;
			}
			pChild=pChild->next;
		}
	}
	XU16 chars[]={' ',0};
	AppendText(chars,1);
	return pNew;
}

void EditText::AppendText(U16*pText,int insertLen)
{
	/*int start=m_selectionStart;
	int end=m_selectionEnd;
	m_selectionStart=m_length;
	m_selectionEnd=m_length;

	InsertWideChars(pText,l);

	m_selectionStart=XMIN(start,m_length);
	m_selectionEnd=XMIN(end,m_length);
	m_caretIndex = m_selectionStart;*/
	//if(this->m_length>=24)
	//	int v=0;

	U16 *buffer = pText;
	int count;

	if ( !insertLen )
	{
		goto exit_gracefully;
	}

	// If using font outlines, purge any characters not in the font
	{
		buffer = (U16*)XXVar::_AllocData((m_length+insertLen+1)*sizeof(U16));
			//new U16[insertLen+1];
		if ( !buffer )
		{
			goto exit_gracefully;
		}
		if(m_length)
			memcpy(buffer, m_buffer,m_length*sizeof(U16));
		memcpy(buffer+m_length, pText, insertLen * sizeof(U16));
		buffer[insertLen+m_length]=0;
	}
exit_gracefully:
	if ( buffer != pText )
	{
		//delete [] buffer;
		XXVar::_FreeData(m_buffer);
		m_buffer=buffer;
		m_length+=insertLen;
	}
	/*else
	{
		m_buffer=buffer;
		m_length+=insertLen;
	}*/
	
	m_obj->Modify();
}


//#if (__CORE_VERSION__>=0x02079000)
void EditText::replaceWideText(U16*pText,int l,int b,int e,bool bFormat)
//#else
//void EditText::replaceWideText(U16*pText,int l,int b,int e)
//#endif
{
	int start=m_selectionStart;
	int end=m_selectionEnd;
	if(b<0) b=0;
	if(e>m_length) e=m_length;
	if(e<b) e=b;
	int delta=l-(e-b);
	//ReplaceFormat(m_pFormat,b,e,delta,m_obj->display);
	m_selectionStart=b;
	m_selectionEnd=e;

//#if (__CORE_VERSION__>=0x02073000)
	if(l<=0)
		this->DeleteSelection();
	else
//#endif
	InsertWideChars(pText,l);
//#if (__CORE_VERSION__>=0x02079000)
	if(bFormat)
		ReplaceFormat(m_pFormat,b,e,delta,m_obj->display);
//#endif
	m_selectionStart=XMIN(start,m_length);
	m_selectionEnd=XMIN(end,m_length);
	m_caretIndex = m_selectionStart;
	m_obj->Modify();
	m_bModifyHTML=true;
}

static bool _IsURLTextFormat(_XTEXTFORMAT*fmt)
{
	while(fmt)
	{
		if(!fmt->url.IsNull()||!fmt->txtEvent.IsNull()) return true;
		if(_IsURLTextFormat(fmt->child)) return true;
		fmt=fmt->next;
	}
	return false;
}

void EditText::SetURLFlag()
{
	m_bURL=_IsURLTextFormat(m_pFormat);
}
