/*
 ============================================================================
 Name		: GlobalEdit.h
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CGlobalEdit declaration
 ============================================================================
 */

#ifndef GLOBALEDIT_H
#define GLOBALEDIT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>
#include <eikedwin.h>

// CLASS DECLARATION

class ButtonCancel;
class ButtonOK;
class XWindow;
class GlobalEditObserver;

/**
 *  CGlobalEdit
 * 
 */
class CGlobalEdit : public CCoeControl
	{
public:
	static CGlobalEdit* GetInstance()
	{
	static CGlobalEdit* instance = NULL;
	if(!instance)
		instance = NewL(TRect(0,0,640,360));
	return instance;
	}
public:
//	class MEditObserver
//		{
//		public:
//		virtual void OnGlobalEvent(unsigned long data1,unsigned long data2)=0;
//		virtual void SetText(unsigned short *text)=0;
//		};
public:
	// Constructors and destructor

	/**
	 * Destructor.
	 */
	~CGlobalEdit();

	/**
	 * Two-phased constructor.
	 */
	static CGlobalEdit* NewL(const TRect& aRect,const CCoeControl* aParent=NULL);

	/**
	 * Two-phased constructor.
	 */
	static CGlobalEdit* NewLC(const TRect& aRect,const CCoeControl* aParent=NULL);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CGlobalEdit();

	/**
	 * EPOC default constructor for performing 2nd stage construction
	 */
	void ConstructL(const TRect& aRect,const CCoeControl* aParent);
public:
	void Draw(const TRect& aRect) const;										//come from CCoeControl
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);	//come from CCoeControl
	void SizeChanged();															//come from CCoeControl
	void HandlePointerEventL(const TPointerEvent& aPointerEvent);
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
public:
	//����������е�����
	void setEditText(const TDesC& text);
	//��ȡ������е�����
	HBufC* getEditText();
	//���������е��ַ�
	void clean();
	//������򽹵�
	void setEditFocus(bool focus);
	//��������
	void setEditObserver(GlobalEditObserver* observer){m_pObserver = observer;}
	//���ô�����Ϣ����
	void setWinServer(XWindow* winServer){m_pWinServer = winServer;}
	//���OK
	void onOk();
	//���Cancel
	void onCancel();
	//�ͷ��ڴ���Դ
	void Release();

protected:
	//�����
	CEikEdwin		*m_hEdit;
	//��ť������һ��ȡ��һ��ȷ��
	ButtonCancel	*m_hCancel;
	ButtonOK		*m_hOK;
	//ͷ������ͼ
	CFbsBitmap		*m_hTitleBack;
	//ͷ��
	TRect			 m_titleRect;
	//�ı�
	TRect			 m_bodyRect;
	//֪ͨ����
	GlobalEditObserver		*m_pObserver;
	//������Ϣ֪ͨ��
	XWindow			*m_pWinServer;
	};

#endif // GLOBALEDIT_H
