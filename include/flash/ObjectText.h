// ObjectText.h: interface for the ObjectText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTTEXT_H__C0EB9E23_C8A5_4681_B042_8F01A03AB535__INCLUDED_)
#define AFX_OBJECTTEXT_H__C0EB9E23_C8A5_4681_B042_8F01A03AB535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"


class XXObjectText:public XXObject//Event
{
	XDEC_XOBJECT(XXObjectText)
protected:
	void GetLength(XXVar&var);
	void InitObject();
//	SObject*m_pObject;
public:
	virtual XBOOL SysGetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysSetMember(int id,XSWFCONTEXT*pCnt,XXVar&var);
	virtual XBOOL SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list);

	/*void addListeners(XSWFCONTEXT&cnt,XXVARLIST&list);
	void alphaGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void alphaSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void autoSizeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void autoSizeSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void antiGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void antiSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	
	void backgroundSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void backgroundGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void backgroundColorGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void backgroundColorSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void borderGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void borderSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void borderColorGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void borderColorSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void bottomScrollGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void condenseWhiteGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void condenseWhiteSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void embedFontsGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void embedFontsSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void filtersGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void filtersSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void gridFitTypeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void gridFitTypeSet(XSWFCONTEXT&cnt,XXVARLIST&list);

	void heightGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void heightSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void hscrollGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void hscrollSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void htmlGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void htmlSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void htmlTextGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void htmlTextSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void maxCharsGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void maxCharsSet(XSWFCONTEXT&cnt,XXVARLIST&list);

	void maxhscrollGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void maxscrollGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void menuGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void menuSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void mouseWheelEnabledGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void mouseWheelEnabledSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void multilineSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void multilineGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void nameGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void nameSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void parentGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void parentSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void passwordGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void passwordSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void qualityGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void qualitySet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void restrictGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void restrictSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void rotationGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void rotationSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void scrollGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void scrollSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void selectableGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void selectableSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void sharpnessGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void sharpnessSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void soundbuftimeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void soundbuftimeSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void styleSheetGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void styleSheetSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void tabEnabledGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void tabEnabledSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void tabIndexGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void tabIndexSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void targetGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textColorGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textColorSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textHeightGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textHeightSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textWidthGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void textWidthSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void thicknessGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void thicknessSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void typeGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void typeSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void urlGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void visiableGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void visiableSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void widthGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void widthSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void wordWrapGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void wordWrapSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void xGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void xSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void yGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void ySet(XSWFCONTEXT&cnt,XXVARLIST&list);


	void yscaleGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void yscaleSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void ymouseGet(XSWFCONTEXT&cnt,XXVARLIST&list);	
	void xscaleGet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void xscaleSet(XSWFCONTEXT&cnt,XXVARLIST&list);
	void xmouseGet(XSWFCONTEXT&cnt,XXVARLIST&list);*/
	
public:
	virtual XXObject* GetParent();
	void SetObject(SObject*p){m_pObject=p;}
	virtual XU32 GetObjectType()
	{
		return XXOBJ_TEXT;
		//return Handle(XNULL,XNULL,XNULL,XOH_GETTYPE);
	}
	
	XXObjectText(ScriptPlayer*pRoot):XXObject(pRoot)
	{
		m_pObject=XNULL;
		#ifdef _DEBUG
			nObjCount[XXOBJ_STRING]++;
		#endif
	}
	~XXObjectText()
	{
		#ifdef _DEBUG
			nObjCount[XXOBJ_STRING]--;
		#endif	
	}
};

#endif // !defined(AFX_OBJECTTEXT_H__C0EB9E23_C8A5_4681_B042_8F01A03AB535__INCLUDED_)
