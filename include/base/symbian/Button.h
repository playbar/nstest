/*
 ============================================================================
 Name		: Button.h
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CButton declaration
 ============================================================================
 */

#ifndef BUTTON_H
#define BUTTON_H

// INCLUDES

#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>
enum PROMPT_FROM{
	XPROMPT_EXIT,
	XPROMPT_REFRESH,
	XPROMPT_UPDATE,
	XPROMPT_SET,
	XPROMPT_CLOSE
};
class MTeleTimerObserver//�������
	{
	public:
	virtual void TelephoneStatusChanged()=0;
	};

class MBackTimerObserver
	{
	public:
	virtual void BackStatusChanged()=0;
	};
class MAdvanceTimerObserver
	{
	public:
	virtual void AdvanceStatusChanged()=0;
	};
// CLASS DECLARATION

/**
 *  CButton
 * 
 */

class CButton : public CCoeControl
	{
public:
	//����ת��
//	static  void ConvertToUnicode(TDes& aTarget, const TDesC8& aSource)
//		{
//
//		CCnvCharacterSetConverter* converter=CCnvCharacterSetConverter::NewLC();
//		if(converter->PrepareToConvertToOrFromL(KCharacterSetIdentifierGbk,CEikonEnv::Static()->FsSession())!=CCnvCharacterSetConverter::EAvailable)
//			{
//			User::Leave(KErrNotSupported);
//			}
//		TInt state= CCnvCharacterSetConverter::KStateDefault;
//		converter->ConvertToUnicode(aTarget,aSource,state);
//		
//		CleanupStack::PopAndDestroy();
//		}
	enum {EDefault,EClickDown,EClickUp};
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	virtual ~CButton();

public:
	//����Ĭ��ͼ��
	void SetDefaultImg(int Id);
	//���ð��º�ͼ��
	void SetDownImg(int Id);
	//����ʱ����
	virtual void OnCreate()=0;
	//���������Ӧ
	virtual void OnClick()=0;
public:
	//come from ccoecontrol
	void Draw(const TRect& aRect) const;
	
	void SizeChanged();
	
	TKeyResponse OfferKeyEventL(const TKeyEvent &aKeyEvent, TEventCode aType);
	
	void HandlePointerEventL(const TPointerEvent& aPointerEvent);

public:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CButton();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(const TRect& aRect,CCoeControl *parent);
public:
	void SetInvalid(bool aInvalid){isInvalid=aInvalid;}
	bool GetInvalid(){return isInvalid;}
	void SetAssInvalid(bool aInvalid){assIsInvalid=aInvalid;}
	bool GetAssInvalid(){return assIsInvalid;}
protected:
	CFbsBitmap	*m_hDefault;						//Ĭ��ͼ
	CFbsBitmap	*m_hDown;						//���º�ͼ
	unsigned char m_nState;						//��ť��ǰ״̬
	HBufC* m_hMsg;	
	bool isInvalid;
	bool assIsInvalid;
	};

#endif // BUTTON_H
