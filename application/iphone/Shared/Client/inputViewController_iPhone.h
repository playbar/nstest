//
//  inputViewController_iPhone.h
//  BaiWan
//
//  Created by handong on 11-6-13.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DBEngine.h"

@class BaiWanViewController_iPhone;
@class historyViewController_iPhone;

@interface inputViewController_iPhone : UIViewController <UITextViewDelegate, UITextFieldDelegate,UITableViewDataSource, UITableViewDelegate, UIPopoverControllerDelegate> {
    
    UITextField* singleTextView;
    DBEngine    *m_hDb;     //数据库支持
    int          m_nSLen;   //终结串长度纪录
    CGPoint     m_iLastPoint;   //最后屏幕点击位置
    int         m_nPosition;       //X轴区域
    bool        m_bIsShow;         //是否在显示状态
    bool        m_bNeedShow;       //需要显示
    bool        m_bNeedHide;       //需要隐藏
    
    BOOL doneEdit;
    
    BaiWanViewController_iPhone* viewController;
    historyViewController_iPhone* historyViewController;
    NSMutableArray* historyArray;
    UIPopoverController* popoverController;
}
@property (readonly) UIPopoverController* popoverController;
@property (readonly) bool m_bIsShow;
@property historyViewController_iPhone* historyViewController;
@property (nonatomic, assign) BaiWanViewController_iPhone* viewController;
@property (nonatomic, assign) IBOutlet UITextField* singleTextView;

- (void)setText:(NSString*)text maxLen:(int)max_len property:(unsigned int)prop;
- (IBAction)onHistory:(UIButton*)aButton;
- (IBAction)onDone:(UIButton*)aButton;
- (void)showKeyBoard;
- (void)hideKeyBoard;
//监听输入
- (IBAction)onTextChanged:(id)sender;

@end
