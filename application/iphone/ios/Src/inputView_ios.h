//
//  inputView_iPhone.h
//  BaiWan
//
//  Created by handong on 11-6-13.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DBEngine.h"

#ifdef __USING_SPEECH_INPUT__
#import "SpeechRecognizerView.h"
#endif

@class BaiWanViewController_ios;
@class historyViewController_ios;

@interface inputView_ios : UIView <UITextViewDelegate, UITextFieldDelegate,UITableViewDataSource, UITableViewDelegate, UIPopoverControllerDelegate
#ifdef __USING_SPEECH_INPUT__
,SpeechRecognizerViewDelegate
#endif
>
{
    
    UITextField* singleTextView;
    UIButton *historyButton;
    DBEngine    *m_hDb;     //数据库支持
    int          m_nSLen;   //终结串长度纪录
    CGPoint     m_iLastPoint;   //最后屏幕点击位置
    int         m_nPosition;       //X轴区域
    bool        m_bIsShow;         //是否在显示状态
    bool        m_bNeedShow;       //需要显示
    bool        m_bNeedHide;       //需要隐藏
    
    BOOL doneEdit;
    
    BaiWanViewController_ios* viewController;
    historyViewController_ios* historyViewController;
    NSMutableArray* historyArray;
    UIPopoverController* popoverController;
#ifdef __USING_SPEECH_INPUT__
    UIButton *micButton;
    UIButton *keyboardButton;
    SpeechRecognizerView * speechRecognizerView;
    CGRect inputKeyboardRect;
    UIView *blackLineView;
#endif
}
@property (readonly) UIPopoverController* popoverController;
@property (readonly) bool m_bIsShow;
@property historyViewController_ios* historyViewController;
@property (nonatomic, assign) BaiWanViewController_ios* viewController;
//@property (nonatomic, assign) IBOutlet UITextField* singleTextView;

- (void)setText:(NSString*)text maxLen:(int)max_len property:(unsigned int)prop;
- (void)onHistory:(UIButton*)aButton;
//- (IBAction)onDone:(UIButton*)aButton;
- (void)showKeyBoard;
- (void)hideKeyBoard;
//监听输入
- (void)onTextChanged:(id)sender;
- (void)adjustView;
@end
