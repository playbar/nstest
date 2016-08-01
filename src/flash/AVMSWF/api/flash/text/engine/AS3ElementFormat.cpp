#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ElementFormat.h"
#include "AS3TextBaseline.h"
#include "AS3BreakOpportunity.h"
#include "AS3DigitCase.h"
#include "AS3Kerning.h"
#include "AS3LigatureLevel.h"
#include "AS3TextRotation.h"
#include "AS3TypographicCase.h"

namespace avmshell{
ElementFormatClass::ElementFormatClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ElementFormatClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ElementFormatObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ElementFormatObject* ElementFormatClass::CreateObject()
{
	ElementFormatObject*pObj=(ElementFormatObject*)createInstance(ivtable(),prototype);
	pObj->m_nFontSize=16;
	return pObj;
}

ElementFormatObject::ElementFormatObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_nBaseLine=TextBaselineClass::USE_DOMINANT_BASELINE;
	m_nDomainBaseLine=TextBaselineClass::USE_DOMINANT_BASELINE;
	m_alpha=1.0;
	m_baselineShift=0.0;
	m_nColor=0xff000000;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	DigitCaseClass*pClass=top->getDigitCaseClass();
	m_strDigitCase=pClass->getDEFAULT();
	m_strDigitWidth=m_strDigitWidth;
	m_nFontSize=12.0;
	
	KerningClass*pKerning=top->getKerningClass();
	m_strKerning=pKerning->getON();

	LigatureLevelClass*pLigatureLevel=top->getLigatureLevelClass();
	m_strLigatureLevel=pLigatureLevel->getCOMMON();

	m_strLocal=core()->newConstantStringLatin1("en");
	m_bLocked=false;


	TextRotationClass*pRotation=top->getTextRotationClass();
	m_strRotation=pRotation->getROTATE_0();

	m_nTrackingLeft=0.0;
	m_nTrackingRight=0.0;

	TypographicCaseClass*pTypo=top->getTypographicCaseClass();
	m_strTypographicCase=pTypo->getDEFAULT();
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp ElementFormatObject::AS3_alignmentBaseline_get()
{
	//Add your act code here...
	TextBaselineClass*pClass=((ShellToplevel*)toplevel())->getTextBaselineClass();
	return pClass->GetBaseName(m_nBaseLine);

	//return m_strBaseLine;//Modify this please!
}

void ElementFormatObject::AS3_alignmentBaseline_set(Stringp alignmentBaseline)
{
	//Add your act code here...
	TextBaselineClass*pClass=((ShellToplevel*)toplevel())->getTextBaselineClass();
	m_nBaseLine=pClass->GetBaseMode(alignmentBaseline);
}

double ElementFormatObject::AS3_alpha_get()
{
	//Add your act code here...
	return m_alpha;//Modify this please!
}

void ElementFormatObject::AS3_alpha_set(double alpha)
{
	//Add your act code here...
	m_alpha=alpha;
	int nAlpha=(int)(alpha*255);
	m_nColor|=nAlpha<<24;
}

double ElementFormatObject::AS3_baselineShift_get()
{
	//Add your act code here...
	return m_baselineShift;//Modify this please!
}

void ElementFormatObject::AS3_baselineShift_set(double baselineShift)
{
	//Add your act code here...
	m_baselineShift=baselineShift;
}

Stringp ElementFormatObject::AS3_breakOpportunity_get()
{
	//Add your act code here...
	ShellToplevel*top=(ShellToplevel*)toplevel();
	BreakOpportunityClass*pClass=top->getBreakOpportunityClass();
	return pClass->GetBreakName(m_nBreakMode);
	//return NULL;//Modify this please!
}

void ElementFormatObject::AS3_breakOpportunity_set(Stringp breakOpportunity)
{
	//Add your act code here...
	ShellToplevel*top=(ShellToplevel*)toplevel();
	BreakOpportunityClass*pClass=top->getBreakOpportunityClass();
	m_nBreakMode=pClass->GetBreakMode(breakOpportunity);
}

uint32_t ElementFormatObject::AS3_color_get()
{
	//Add your act code here...
	return m_nColor&0xffffff;//Modify this please!
}

void ElementFormatObject::AS3_color_set(uint32_t color)
{
	//Add your act code here...
	m_nColor&=0xff000000;
	m_nColor|=(color&0xffffff);
}

Stringp ElementFormatObject::AS3_digitCase_get()
{
	//Add your act code here...
	return m_strDigitCase;//Modify this please!
}

void ElementFormatObject::AS3_digitCase_set(Stringp digitCase)
{
	//Add your act code here...
	m_strDigitCase=digitCase;
}

Stringp ElementFormatObject::AS3_digitWidth_get()
{
	//Add your act code here...
	return m_strDigitWidth;//Modify this please!
}

void ElementFormatObject::AS3_digitWidth_set(Stringp digitWidth)
{
	//Add your act code here...
	m_strDigitWidth=digitWidth;
}

Stringp ElementFormatObject::AS3_dominantBaseline_get()
{
	//Add your act code here...
	TextBaselineClass*pClass=((ShellToplevel*)toplevel())->getTextBaselineClass();
	return pClass->GetBaseName(m_nDomainBaseLine);
	//return NULL;//Modify this please!
}

void ElementFormatObject::AS3_dominantBaseline_set(Stringp dominantBaseline)
{
	//Add your act code here...
	TextBaselineClass*pClass=((ShellToplevel*)toplevel())->getTextBaselineClass();
	m_nDomainBaseLine=pClass->GetBaseMode(dominantBaseline);
}

FontDescriptionObject* ElementFormatObject::AS3_fontDescription_get()
{
	//Add your act code here...
	return m_pFontDesc;//Modify this please!
}

void ElementFormatObject::AS3_fontDescription_set(FontDescriptionObject *pFontDescription)
{
	//Add your act code here...
	m_pFontDesc=pFontDescription;
}

double ElementFormatObject::AS3_fontSize_get()
{
	//Add your act code here...
	return m_nFontSize;//Modify this please!
}

void ElementFormatObject::AS3_fontSize_set(double fontSize)
{
	//Add your act code here...
	m_nFontSize=fontSize;
}

Stringp ElementFormatObject::AS3_kerning_get()
{
	//Add your act code here...
	return m_strKerning;//Modify this please!
}

void ElementFormatObject::AS3_kerning_set(Stringp kerning)
{
	//Add your act code here...
	m_strKerning=kerning;
}

Stringp ElementFormatObject::AS3_ligatureLevel_get()
{
	//Add your act code here...
	return m_strLigatureLevel;//Modify this please!
}

void ElementFormatObject::AS3_ligatureLevel_set(Stringp ligatureLevel)
{
	//Add your act code here...
	m_strLigatureLevel=ligatureLevel;
}

Stringp ElementFormatObject::AS3_locale_get()
{
	//Add your act code here...
	return m_strLocal;//Modify this please!
}

void ElementFormatObject::AS3_locale_set(Stringp locale)
{
	//Add your act code here...
	m_strLocal=locale;
}

bool ElementFormatObject::AS3_locked_get()
{
	//Add your act code here...
	return m_bLocked;//Modify this please!
}

void ElementFormatObject::AS3_locked_set(bool locked)
{
	//Add your act code here...
	m_bLocked=locked;
}

Stringp ElementFormatObject::AS3_textRotation_get()
{
	//Add your act code here...
	return m_strRotation;//Modify this please!
}

void ElementFormatObject::AS3_textRotation_set(Stringp pTextRotation)
{
	//Add your act code here...
	m_strRotation=pTextRotation;
}

double ElementFormatObject::AS3_trackingLeft_get()
{
	//Add your act code here...
	return m_nTrackingLeft;//Modify this please!
}

void ElementFormatObject::AS3_trackingLeft_set(double trackingLeft)
{
	//Add your act code here...
	m_nTrackingLeft=trackingLeft;
}

double ElementFormatObject::AS3_trackingRight_get()
{
	//Add your act code here...
	return m_nTrackingRight;//Modify this please!
}

void ElementFormatObject::AS3_trackingRight_set(double trackingRight)
{
	//Add your act code here...
	m_nTrackingRight=trackingRight;
}

Stringp ElementFormatObject::AS3_typographicCase_get()
{
	//Add your act code here...
	return m_strTypographicCase;//Modify this please!
}

void ElementFormatObject::AS3_typographicCase_set(Stringp typographicCase)
{
	//Add your act code here...
	m_strTypographicCase=typographicCase;
}

//AS3 contructor function..
//AvmBox ElementFormatObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox ElementFormatObject::AS3_clone(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

FontMetricsObject* ElementFormatObject::AS3_getFontMetrics()
{
	//Add your act code here...
	if(m_pMetrics==NULL)
	{
		FontMetricsClass*pClass=((ShellToplevel*)toplevel())->getFontMetricsClass();
		m_pMetrics=pClass->CreateObject(this->m_nFontSize);
	}
	else
		m_pMetrics->SetFontSize(m_nFontSize);
	return m_pMetrics;
}

}
