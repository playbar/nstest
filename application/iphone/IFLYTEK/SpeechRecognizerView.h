//
//  SpeechAlertView.h
//  rhljiayouTest
//
//  Created by iHope on 13-8-7.
//  Copyright (c) 2013年 iHope. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PopupView.h"
#import "iflyMSC/IFlySpeechRecognizerDelegate.h"

@protocol SpeechRecognizerViewDelegate <NSObject>
@optional
- (void)getResultText:(NSString *)text;
-(void)keyboardButtonPressed:(id)sender;
@end
@class IFlySpeechRecognizer;

@interface SpeechRecognizerView : UIView<IFlySpeechRecognizerDelegate>
{
    UIImageView *speechImage;
    UIView *blackLineView;
    PopupView * _popUpView;
    IFlySpeechRecognizer * _iFlySpeechRecognizer;
    NSString * _result;
}
@property (assign, nonatomic)id<SpeechRecognizerViewDelegate> speechDelegate;
-(void)showInTextView:(UITextField *)textView;
-(void)onSelfDisAppear;
-(void)changeToKeyboard;
@end
