/*
 ============================================================================
 Name		: Button.cpp
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CButton implementation
 ============================================================================
 */

#include "Button.h"
#include <aknutils.h>
#include <GGClient.mbg>				//������Դ���е�ͼƬ

#include<UTF.H>
_LIT(_strFont,"Times New Roman");
CButton::CButton()
	{
	// No implementation required
	m_hDefault = NULL;
	m_hDown = NULL;
	m_nState = 0;
	m_hMsg=NULL;
	isInvalid=false;
	}

CButton::~CButton()
	{
	if(m_hDefault) delete m_hDefault;
	if(m_hDown) delete m_hDown;
	if(m_hMsg) delete m_hMsg;
	}

void CButton::ConstructL(const TRect& aRect,CCoeControl *parent)
	{
	if(parent == NULL)
		CreateWindowL();
	else
		SetContainerWindowL(*parent);
	OnCreate();
	SetRect(aRect);
    ActivateL();
	}

void CButton::Draw(const TRect& aRect) const
	{
	CWindowGc &gc = SystemGc();
	
	switch(m_nState)
		{
		case EClickDown:
			{
			if(m_hDown)
				gc.BitBlt(aRect.iTl,m_hDown);
			}
			break;
		default:
			{
			if(m_hDefault)
				gc.BitBlt(aRect.iTl,m_hDefault);
			}
		}
		
		if(m_hMsg)
		{
//			TRect rect=Rect();
			gc.SetPenColor(0x000000);
			TInt h=18;
			CFont* font;
			TFontSpec zcFontSpec(_strFont,h);  
			CCoeEnv::Static()->ScreenDevice()->GetNearestFontInPixels(font,zcFontSpec);
			gc.UseFont(font);
			TPoint point;
			TInt widDis=aRect.iBr.iX-aRect.iTl.iX;
			TInt fontWidDis=font->TextWidthInPixels(*m_hMsg);
			TInt heiDis=aRect.iBr.iY-aRect.iTl.iY;
			TInt fontHeiDis=font->FontMaxHeight();
			point.SetXY(aRect.iTl.iX+(widDis-fontWidDis)/2,aRect.iTl.iY+(widDis-fontHeiDis)/2);
			gc.DrawText(*m_hMsg,point);
			gc.DiscardFont();
		}	
		
	
	
	}

void CButton::SizeChanged()
	{
	DrawNow();
	}

TKeyResponse CButton::OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType)
	{
	
	}

void CButton::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
	if(isInvalid) return;
	switch(aPointerEvent.iType)
		{
		case TPointerEvent::EButton1Down:
			{
			m_nState = EClickDown;
			DrawNow();
			}
			break;
		case TPointerEvent::EButton1Up:
			{
			m_nState = EClickUp;
			OnClick();
			DrawNow();
			}
		}
	}

//����Ĭ��ͼ��
void CButton::SetDefaultImg(int Id)
	{
	if(!m_hDefault)
		m_hDefault = new(ELeave) CFbsBitmap();
	TFileName appPath;
	CompleteWithAppPath(appPath);
		TFileName fname;
	#ifdef __WINS__
		fname.Append(_L("z:\\resource\\apps\\GGClient.mbm"));
	#else
		fname.Append(appPath.PtrZ()[0]);
		fname.Append(_L(":\\resource\\apps\\GGClient.mbm"));
	#endif

	
	
	m_hDefault->Load(fname,Id);
	}

//���ð��º�ͼ��
void CButton::SetDownImg(int Id)
	{
	if(!m_hDown)
		m_hDown = new(ELeave) CFbsBitmap();
	TFileName appPath;
	CompleteWithAppPath(appPath);
	TFileName fname;
	#ifdef __WINS__
		fname.Append(_L("z:\\resource\\apps\\GGClient.mbm"));
	#else
		fname.Append(appPath.PtrZ()[0]);
		fname.Append(_L(":\\resource\\apps\\GGClient.mbm"));
	#endif
	
	
		m_hDown->Load(fname,Id);
	}

