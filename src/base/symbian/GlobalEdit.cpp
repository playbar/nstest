/*
 ============================================================================
 Name		: GlobalEdit.cpp
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGlobalEdit implementation
 ============================================================================
 */

#include "GlobalEdit.h"
#include "Button.h"
#include "XWindow.h"
#include <eikon.hrh>
#include <eikenv.h>
#include <aknappui.h>
#include <aknutils.h>
#include <GGClient.mbg>
#include "GlobalObserver.h"


class ButtonCancel : public CButton
	{
public:
	void OnCreate(){}
	void OnClick();
	};
void ButtonCancel::OnClick()
	{
	CGlobalEdit *temp = (CGlobalEdit*)Parent();
	temp->onCancel();
	}
class ButtonOK : public CButton
	{
	void OnCreate(){}
	void OnClick();
	};
void ButtonOK::OnClick()
	{
	CGlobalEdit *temp = (CGlobalEdit*)Parent();
	temp->onOk();
	}

CGlobalEdit::CGlobalEdit()
	{
	// No implementation required
	m_hEdit = NULL;
	m_hCancel = NULL;
	m_hOK = NULL;
	m_hTitleBack = NULL;
	m_pObserver = NULL;
	m_pWinServer = NULL;
//	formatLayer = NULL;
	}

CGlobalEdit::~CGlobalEdit()
	{
	Release();
	}

CGlobalEdit* CGlobalEdit::NewLC(const TRect& aRect,const CCoeControl* aParent)
	{
	CGlobalEdit* self = new (ELeave) CGlobalEdit();
	CleanupStack::PushL(self);
	self->ConstructL(aRect,aParent);
	return self;
	}

CGlobalEdit* CGlobalEdit::NewL(const TRect& aRect,const CCoeControl* aParent)
	{
	CGlobalEdit* self = CGlobalEdit::NewLC(aRect,aParent);
	CleanupStack::Pop(); // self;
	return self;
	}

void CGlobalEdit::ConstructL(const TRect& aRect,const CCoeControl* aParent)
	{
	if(aParent)
		SetContainerWindowL(*aParent);
	else
		CreateWindowL();
	m_titleRect.SetRect(0,0,aRect.Width(),60);
	m_bodyRect.SetRect(0,65,aRect.Width(),aRect.Height());
	MakeVisible(false);
	SetRect(aRect);
	//����ʵЧ��ť
	m_hCancel = new ButtonCancel();
	CleanupStack::PushL(m_hCancel);
	m_hCancel->ConstructL(TRect(m_titleRect.iTl-TSize(5,5),TSize(90,m_titleRect.Height()-10)),this);
	CleanupStack::Pop(m_hCancel);
	//����ȷ����ť
	m_hOK = new ButtonOK();
	CleanupStack::PushL(m_hOK);
	m_hOK->ConstructL(TRect(TPoint(m_titleRect.Width()-95,m_titleRect.iTl.iY-5),TSize(90,m_titleRect.Height()-10)),this);
	CleanupStack::Pop(m_hOK);
	//����ͼƬ
	TFileName appPath;
	CompleteWithAppPath(appPath);
		TFileName fname;
	#ifdef __WINS__
		fname.Append(_L("z:\\resource\\apps\\GGClient.mbm"));
	#else
		fname.Append(appPath.PtrZ()[0]);
		fname.Append(_L(":\\resource\\apps\\GGClient.mbm"));
	#endif
	m_hTitleBack = new(ELeave) CFbsBitmap();
	m_hTitleBack->Load(fname,EMbmGgclientEdit);
	
	//�����ı������
	CCharFormatLayer* formatLayer;
	formatLayer=CEikonEnv::NewDefaultCharFormatLayerL();
	TCharFormat charFormat;
	TCharFormatMask charFormatMask;
	formatLayer->Sense(charFormat, charFormatMask);
	charFormat.iFontSpec.iHeight = 80;
	charFormat.iFontPresentation.iTextColor = TRgb(KRgbBlack);
	formatLayer->SetL(charFormat, charFormatMask);
	
	m_hEdit = new (ELeave)CEikEdwin;
	m_hEdit->SetContainerWindowL(*this);
	m_hEdit->ConstructL(EAknEditorFlagDefault , 150, 100, 100);

//	m_hEdit->SetAknEditorCase(  EAknEditorAllCaseModes );
//	
//	m_hEdit->SetAknEditorAllowedInputModes(EAknEditorAllInputModes);
//
//	m_hEdit->SetAknEditorInputMode(EAknEditorTextInputMode);
	m_hEdit->SetCharFormatLayer(formatLayer);
	m_hEdit->SetExtent(m_bodyRect.iTl+TSize(5,5),m_bodyRect.Size()-TSize(10,10));
	ActivateL();
	}

void CGlobalEdit::Draw(const TRect& aRect) const
	{
	CWindowGc &gc = SystemGc();
	gc.Clear();
//	gc.DrawRoundRect(m_titleRect,TSize(5,5));
	gc.BitBlt(TPoint(0,0),m_hTitleBack);
	gc.DrawRoundRect(m_bodyRect,TSize(5,5));
	}
TKeyResponse CGlobalEdit::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	
	}
void CGlobalEdit::SizeChanged()
	{
	DrawNow();
	}
void CGlobalEdit::HandlePointerEventL(const TPointerEvent& aPointerEvent)
	{
	TPoint pt = aPointerEvent.iParentPosition;
	if(m_bodyRect.Contains(pt))
		{
		m_hEdit->HandlePointerEventL(aPointerEvent);
		return;
		}
	TRect rt = m_hCancel->Rect();
	if(rt.Contains(pt))
		{
		m_hCancel->HandlePointerEventL(aPointerEvent);
		return;
		}
	rt = m_hOK->Rect();
	if(rt.Contains(pt))
		{
		m_hOK->HandlePointerEventL(aPointerEvent);
		return;
		}
	}
TInt CGlobalEdit::CountComponentControls() const
	{
	return 3;
	}
CCoeControl* CGlobalEdit::ComponentControl(TInt aIndex) const
	{
	switch(aIndex)
		{
		case 0:
			return m_hEdit;
		case 1:
			return m_hCancel;
		case 2:
			return m_hOK;
		}
	}

//����������е�����
void CGlobalEdit::setEditText(const TDesC& text)
	{
	m_hEdit->SetTextL(&text);
	}
//��ȡ������е�����
HBufC* CGlobalEdit::getEditText()
	{
	return m_hEdit->GetTextInHBufL();
	}
//���������е��ַ�
void CGlobalEdit::clean()
	{
	m_hEdit->SelectAllL();
	m_hEdit->ClearSelectionL();
	}
//������򽹵�
void CGlobalEdit::setEditFocus(bool focus)
	{
	CCoeAppUi *ui = CEikonEnv::Static()->AppUi();
	if(focus)
		CEikonEnv::Static()->AppUi()->AddToStackL(m_hEdit);
	else
		CEikonEnv::Static()->AppUi()->RemoveFromStack(m_hEdit);
	m_hEdit->SetFocus(focus);
	if(m_pWinServer)
		m_pWinServer->Handle(XWM_SYMBIAN_COMMAND,XCMD_MAKE_VISIBLE,(XU32)!focus);
	m_hEdit->DrawNow();
	}
//���OK
void CGlobalEdit::onOk()
	{
	if(m_pObserver == NULL) return;
	HBufC *buf = getEditText(); 
	if(buf)
		{
		unsigned short* temp = new unsigned short[buf->Length()+1];
		memmove(temp,buf->Des().Ptr(),buf->Length()*2);
		temp[buf->Length()] = 0x0000;
		m_pObserver->SetText(temp);
		delete buf;
//		delete temp;
		}
	m_pObserver->OnGlobalEvent((XU32)this,0);
	}
//���Cancel
void CGlobalEdit::onCancel()
	{
	clean();
	setEditFocus(false);
	m_pObserver->OnGlobalEvent((XU32)this,0);
	}
//�ͷ��ڴ���Դ
void CGlobalEdit::Release()
	{
	#define R(M) {if(M) {delete M; M=NULL;}}
	R(m_hEdit);
	R(m_hCancel);
	R(m_hOK);
	R(m_hTitleBack);
//	R(formatLayer);
	#undef R
	m_pObserver = NULL;
	m_pWinServer = NULL;
	}
