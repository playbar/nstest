//
//  inputViewController_iPhone.m
//  BaiWan
//
//  Created by handong on 11-6-13.
//  Copyright 2011 JQBar. All rights reserved.
//

#import "inputViewController_ios.h"
#import "BaiWanViewController_ios.h"
#import "historyViewController_ios.h"
#import "ClientView_ios.h"


#include "KeyBoardMiddleLayer.h"
#include "IPhoneDeviceData.h"
#include "XEdit.h"

#ifdef __USING_SPEECH_INPUT__
#import "GlobalPortingFunction.h"
#import "iflyMSC/iflySetting.h"
#import "RecognizerFactory.h"
#import "IFLYTEKDefinition.h"
#endif

#define degreeToRadians(x) (3.1415926 * (x) / 180.0)
#define CGRectEdit CGRectMake(0,0,700,30)

#define ZWidth      200
#define ZHeight     160
#define ZNoneLen    0

#define ZLeft       (170-ZWidth/2)
#define ZCenter     (512-ZWidth/2)
#define ZRight      (854-ZWidth/2)
const float iosversion = [[[UIDevice currentDevice] systemVersion] floatValue];

enum
{
    Eleft,
    Ecenter,
    Eright
};

extern BaiWanViewController_ios* g_explorerController_ios;
extern iOSDeviceRelateData g_deviceData;


@interface inputViewController_ios()

- (float)defaultFontSize;
- (float)offsetForButton;
- (void)setTextProperty:(unsigned int)prop;
- (void)doneEditing;

// notificationCenter
- (void)keyboardDidShow:(NSNotification *)notification;
- (void)keyboardWillHide:(NSNotification *)notification;
//监听键盘变换事件
- (void) keyBoardChangeRect:(NSNotification*)sender;

@end

@implementation inputViewController_ios
@synthesize popoverController;
@synthesize m_bIsShow;
@synthesize historyViewController;
@synthesize viewController;
//@synthesize singleTextView;

extern iOSDeviceRelateData g_deviceData;


- (void)viewDidLoad 
{
    [super viewDidLoad];
    
#ifdef __USING_SPEECH_INPUT__
    [self.view setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, 33)];
    
    if (g_deviceData.m_isPad)
    {
        blackLineView = [[UIView alloc]init];
        blackLineView.backgroundColor = [UIColor lightGrayColor];
        [self.view addSubview:blackLineView];
        [self.view setBackgroundColor:[UIColor whiteColor]];
        singleTextView = [[UITextField alloc]initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth - 147, 31)];
        singleTextView.borderStyle = UITextBorderStyleRoundedRect;
        singleTextView.autocapitalizationType = UITextAutocapitalizationTypeNone;
        singleTextView.returnKeyType = UIReturnKeyDone;
        historyButton = [UIButton buttonWithType: UIButtonTypeCustom];
        historyButton.frame = CGRectMake(g_deviceData.m_screenWidth - 70, 0, 60, 33);
        [historyButton setTitle:@"历史" forState:UIControlStateNormal];
        [historyButton setTitleColor:[UIColor colorWithRed:28.0/255 green:157.0/255 blue:238.0/255 alpha:1] forState:UIControlStateNormal];
        [historyButton setTitle:@"历史" forState:UIControlStateHighlighted];
        [historyButton setTitleColor:[UIColor colorWithRed:28.0/255 green:157.0/255 blue:238.0/255 alpha:1] forState:UIControlStateHighlighted];
        [historyButton addTarget:self action:@selector(onHistory:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:historyButton];
        micButton = [UIButton buttonWithType:UIButtonTypeCustom];
        micButton.frame = CGRectMake(g_deviceData.m_screenWidth - 128, 0, 48, 33);
        [micButton setBackgroundImage:[UIImage imageNamed:@"speechRecognizeButton.png"] forState:UIControlStateNormal];
        [micButton addTarget:self action:@selector(micPressed:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:micButton];
        
        keyboardButton = [UIButton buttonWithType:UIButtonTypeCustom];
        keyboardButton.frame = CGRectMake(g_deviceData.m_screenWidth - 128, 0, 48, 33);
        [keyboardButton setBackgroundImage:[UIImage imageNamed:@"keyboardButton.png"] forState:UIControlStateNormal];
        [keyboardButton addTarget:self action:@selector(keyboardButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:keyboardButton];
        keyboardButton.hidden = YES;
    }
    else
    {
        blackLineView = [[UIView alloc]init];
        blackLineView.backgroundColor = [UIColor lightGrayColor];
        [self.view addSubview:blackLineView];
        [self.view setBackgroundColor:[UIColor whiteColor]];
        singleTextView = [[UITextField alloc]initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth - 48, 31)];
        singleTextView.borderStyle = UITextBorderStyleLine;
        singleTextView.layer.borderColor = [[UIColor lightGrayColor] CGColor];
        singleTextView.autocapitalizationType = UITextAutocapitalizationTypeNone;
        singleTextView.returnKeyType = UIReturnKeyDone;
        micButton = [UIButton buttonWithType:UIButtonTypeCustom];
        micButton.frame = CGRectMake(g_deviceData.m_screenWidth - 48, 0, 48, 33);
        [micButton setBackgroundImage:[UIImage imageNamed:@"speechRecognizeButton.png"] forState:UIControlStateNormal];
        [micButton addTarget:self action:@selector(micPressed:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:micButton];
        
        keyboardButton = [UIButton buttonWithType:UIButtonTypeCustom];
        keyboardButton.frame = CGRectMake(g_deviceData.m_screenWidth - 48, 0, 48, 33);
        [keyboardButton setBackgroundImage:[UIImage imageNamed:@"keyboardButton.png"] forState:UIControlStateNormal];
        [keyboardButton addTarget:self action:@selector(keyboardButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:keyboardButton];
        keyboardButton.hidden = YES;
    }
    singleTextView.backgroundColor = [UIColor whiteColor];
    singleTextView.delegate = self;
    
    [self.view addSubview:singleTextView];
    
    historyArray = [[NSMutableArray alloc] init];
    historyViewController = [[historyViewController_ios alloc] init];
    [(UITableView*)historyViewController.view setDataSource:self];
    [(UITableView*)historyViewController.view setDelegate:self];
    
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardDidShow:) name:UIKeyboardDidShowNotification object:nil];
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
//    //设置输入框监听
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onTextChanged:) name:UITextFieldTextDidChangeNotification object:NULL];
//    [[UIDevice currentDevice] systemVersion];
//    if(iosversion>4.9)
//        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyBoardChangeRect:) name:UIKeyboardDidChangeFrameNotification object:NULL];
    m_hDb = NULL;
    m_nSLen = ZNoneLen;
    popoverController = NULL;
    m_iLastPoint.x=0;
    m_iLastPoint.y=0;
    m_bNeedShow = false;
    m_bNeedHide = false;
    m_bIsShow = false;
    if (!speechRecognizerView)
    {
        speechRecognizerView = [[SpeechRecognizerView alloc]initWithFrame:CGRectMake(0, 0, 0, 0)];
    }
    
#else
    [self.view setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, 31)];
    [self.view setBackgroundColor:[UIColor lightGrayColor]];
    if (g_deviceData.m_isPad)
    {
        singleTextView = [[UITextField alloc]initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth - 147, 31)];
        singleTextView.borderStyle = UITextBorderStyleRoundedRect;
        singleTextView.autocapitalizationType = UITextAutocapitalizationTypeNone;
        singleTextView.returnKeyType = UIReturnKeyDone;
        historyButton = [UIButton buttonWithType: UIButtonTypeRoundedRect];
        historyButton.frame = CGRectMake(g_deviceData.m_screenWidth - 88, 0, 72, 33);
        [historyButton setTitle:@"历史" forState:UIControlStateNormal];
        [historyButton setTitle:@"历史" forState:UIControlStateHighlighted];
        [historyButton addTarget:self action:@selector(onHistory:) forControlEvents:UIControlEventTouchUpInside];
        [self.view addSubview:historyButton];
    }
    else
    {
        singleTextView = [[UITextField alloc]initWithFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, 31)];
        singleTextView.borderStyle = UITextBorderStyleLine;
        singleTextView.autocapitalizationType = UITextAutocapitalizationTypeNone;
        singleTextView.returnKeyType = UIReturnKeyDone;
    }
    singleTextView.backgroundColor = [UIColor whiteColor];
    singleTextView.delegate = self;
    
    [self.view addSubview:singleTextView];
    
    historyArray = [[NSMutableArray alloc] init];
    historyViewController = [[historyViewController_ios alloc] init];
    [(UITableView*)historyViewController.view setDataSource:self];
    [(UITableView*)historyViewController.view setDelegate:self];
    
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardDidShow:) name:UIKeyboardDidShowNotification object:nil];
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
//    //设置输入框监听
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onTextChanged:) name:UITextFieldTextDidChangeNotification object:NULL];
//    [[UIDevice currentDevice] systemVersion];
//    if(iosversion>4.9)
//        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyBoardChangeRect:) name:UIKeyboardDidChangeFrameNotification object:NULL];
    m_hDb = NULL;
    m_nSLen = ZNoneLen;
    popoverController = NULL;
    m_iLastPoint.x=0;
    m_iLastPoint.y=0;
    m_bNeedShow = false;
    m_bNeedHide = false;
    m_bIsShow = false;
#endif
    
}

-(void)viewDidDisappear:(BOOL)animated
{
    [super viewWillDisappear:YES];
//    [_iFlySpeechUnderstander cancel];
//    [_iFlySpeechUnderstander setDelegate: nil];
    
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    // 转屏时需要修改工具栏大小
#ifdef __USING_SPEECH_INPUT__
    [self.view setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, 33)];
    if (g_deviceData.m_isPad)
    {
        singleTextView.frame = CGRectMake(0, 0, g_deviceData.m_screenWidth - 147, 33);
        historyButton.frame = CGRectMake(g_deviceData.m_screenWidth - 70, 0, 60, 33);
        micButton.frame = CGRectMake(g_deviceData.m_screenWidth - 128, 0, 48, 33);
        keyboardButton.frame = CGRectMake(g_deviceData.m_screenWidth - 128, 0, 48, 33);
    }
    else
    {
        singleTextView.frame = CGRectMake(0, 0, g_deviceData.m_screenWidth - 48, 33);
        micButton.frame = CGRectMake(g_deviceData.m_screenWidth - 48, 0, 48, 33);
        keyboardButton.frame = CGRectMake(g_deviceData.m_screenWidth - 48, 0, 48, 33);
    }
    blackLineView.frame = CGRectMake(0, 0, g_deviceData.m_screenWidth, 1);
#else
    [self.view setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, 31)];
    if (g_deviceData.m_isPad)
    {
        singleTextView.frame = CGRectMake(0, 0, g_deviceData.m_screenWidth - 147, 31);
        historyButton.frame = CGRectMake(g_deviceData.m_screenWidth - 88, 0, 72, 33);
    }
    else
    {
        singleTextView.frame = CGRectMake(0, 0, g_deviceData.m_screenWidth, 31);
    }
#endif
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (interfaceOrientation==UIInterfaceOrientationLandscapeRight) {
        return YES;
    }else if (interfaceOrientation==UIInterfaceOrientationLandscapeLeft) {
        return YES;
    }else
        return NO;
}

- (float)defaultFontSize
{
    return 25;
}

- (float)offsetForButton
{
    return 0;
}

- (void)showKeyBoard
{
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardDidShow:) name:UIKeyboardDidShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
    //设置输入框监听
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onTextChanged:) name:UITextFieldTextDidChangeNotification object:NULL];
    [[UIDevice currentDevice] systemVersion];
    if(iosversion>4.9)
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyBoardChangeRect:) name:UIKeyboardDidChangeFrameNotification object:NULL];
    m_bIsShow = true;
    //clientview can not touch
    [viewController setNoneTouch:true];
#if (BWSHOWBARMODE == BWON)
    if(!viewController.m_bIsSubview && viewController.showBarModeEnable)
#else//BWSHOWBARMODE
        if(!viewController.m_bIsSubview)
#endif//BWSHOWBARMODE
        {
            switch (viewController.currentMode) {
                case EMODE_BAR:
                    [viewController showBar:false];
                    break;
                case EMODE_WIDGET:
                    [viewController showWidget:false];
                    break;
                default:
                    break;
            }
        }
    [viewController needScrollV:true];
    
    
    doneEdit = NO;
    [singleTextView becomeFirstResponder];
    m_hDb = [[DBEngine alloc] init];
    //zhangchen 2012,6,8 changed db path,take the db out from documents, in put it to library/documention
    NSArray* docpath = NSSearchPathForDirectoriesInDomains(NSDocumentationDirectory, NSUserDomainMask, YES);
    NSString* path = [[docpath objectAtIndex:0] stringByAppendingPathComponent:@"jqbardb"];
    [m_hDb OpenOrCreate:path];
    if(g_explorerController_ios)
    {
        m_iLastPoint = g_explorerController_ios.clientView.lastPoint;
    }
    int area = g_deviceData.m_screenWidth/3;
    switch ((int)m_iLastPoint.x / area) {
        case Eleft:
            m_nPosition = ZRight;
            break;
        case Ecenter:
            m_nPosition = ZRight;
            break;
        default:
        case Eright:
            m_nPosition = ZLeft;
            break;
    }
}

- (void)hideKeyBoard
{
    m_bIsShow = false;
    
    [viewController setNoneTouch:false];
    [viewController needScrollV:false];
#if (BWSHOWBARMODE == BWON)
    if(!viewController.m_bIsSubview && viewController.showBarModeEnable)
#else//BWSHOWBARMODE
        if(!viewController.m_bIsSubview)
#endif//BWSHOWBARMODE
        {
            switch (viewController.currentMode) {
                case EMODE_BAR:
                    [viewController showBar:true];
                    break;
                case EMODE_WIDGET:
                    [viewController showWidget:true];
                    [viewController.clientView setFrame:CGRectMake(0, 0, g_deviceData.m_screenWidth, g_deviceData.m_screenHeight)];
                    break;
                default:
                    break;
            }
        }
    
    
    
    [singleTextView resignFirstResponder];
    if(m_hDb != NULL)
    {
        [m_hDb Close];
        [m_hDb release];
        m_hDb=NULL;
    }
    m_nSLen = ZNoneLen;
}

//监听键盘变换事件
- (void) keyBoardChangeRect:(NSNotification*)sender
{
//    CGPoint beginCenterP = [[[sender userInfo] valueForKey:UIKeyboardCenterBeginUserInfoKey] CGPointValue];
//    CGPoint endCenterP = [[[sender userInfo] valueForKey:UIKeyboardCenterEndUserInfoKey] CGPointValue];
//    CGRect  keyBoardBoundR = [[[sender userInfo] valueForKey:UIKeyboardBoundsUserInfoKey] CGRectValue];
    CGRect  keyBoardFrameR = [[[sender userInfo] valueForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    BWLOG(@"orgin x=%f y=%f size w=%f h=%f",keyBoardFrameR.origin.x,keyBoardFrameR.origin.y,keyBoardFrameR.size.width,keyBoardFrameR.size.height);
    UIViewAnimationCurve animCur = (UIViewAnimationCurve)[[[sender userInfo] valueForKey:UIKeyboardAnimationCurveUserInfoKey] intValue];
    NSTimeInterval animDuration = [[[sender userInfo] valueForKey:UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    
    
    if(m_bIsShow)
    {
        keyBoardFrameR = [viewController.view convertRect:keyBoardFrameR fromView:nil];
        [viewController setKeyBoardFrame:keyBoardFrameR withDuration:animDuration isKeyBoard:ESplite];
    }
    
}

//监听输入
- (IBAction)onTextChanged:(id)sender
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];
    NSString *str = [[singleTextView text] stringByAppendingString:@"%"];
    int len = str.length;
    int position;
    
    if(len<=1)
    {
        if([popoverController isPopoverVisible])
            [popoverController dismissPopoverAnimated:YES];
        [pool release];
        return;
    }
    if(m_nSLen>len-1 || m_nSLen<=ZNoneLen)
    {
        [m_hDb Select:str UseCount:0 LastTime:NULL];
        
        [historyArray removeAllObjects];
        int index;
        NSString *temp;
        while([m_hDb Get:&index UseCount:NULL LastTime:NULL Data:&temp])
        {
            [historyArray addObject:temp];
        }
        if([historyArray count]==0)
        {
            m_nSLen = [str length]-1;
            if(m_nSLen <= 5) m_nSLen = ZNoneLen;
            //无可匹配历史消息
            if(popoverController!=NULL && popoverController.popoverVisible)
            {
                //隐藏
                [popoverController dismissPopoverAnimated:YES];
            }
        }
        else
        {
            //有可匹配历史消息
            if(g_deviceData.m_isPad)
            {
                    if(popoverController==NULL)
                    {
                        CGRect rt = [singleTextView accessibilityFrame];
                        CGRect grt = CGRectMake(m_nPosition, rt.origin.y+40, ZWidth, ZHeight);
                        popoverController = [[UIPopoverController alloc] initWithContentViewController:historyViewController];
                        [popoverController setDelegate:self];
                        [popoverController setPopoverContentSize:[historyViewController.view frame].size];
                        [popoverController presentPopoverFromRect:grt inView:self.view permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
                    }
                    else
                    {
                        [(UITableView*)(historyViewController.view) reloadData];
                        if(![popoverController isPopoverVisible])
                        {
                            CGRect rt = [singleTextView accessibilityFrame];
                            CGRect grt = CGRectMake(m_nPosition, rt.origin.y+40, 200, 160);
                            [popoverController presentPopoverFromRect:grt inView:self.view permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
                        }
                    }
            }
            m_nSLen = ZNoneLen;
        }
    }
    [pool release];
    
}

- (void)setText:(NSString*)text maxLen:(int)max_len property:(unsigned int)prop;
{
    [self setTextProperty:prop];
    singleTextView.text = text;
}

- (void)setTextProperty:(unsigned int)prop
{
    if ((prop & XEDS_NUMBER) && (prop & XEDS_SIGN))
        singleTextView.keyboardType = UIKeyboardTypeDefault;
    else if (prop & XEDS_NUMBER)
        singleTextView.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
    else if (prop & XEDS_SIGN)
        singleTextView.keyboardType = UIKeyboardTypeASCIICapable;
    else
        singleTextView.keyboardType = UIKeyboardTypeDefault;
    
    if (prop & XEDS_PASSWORD)
        singleTextView.secureTextEntry = YES;
    else
        singleTextView.secureTextEntry = NO;
}
- (void)onHistory:(UIButton*)aButton
{
    /*
    CGRect buttonFrame = [aButton frame];
    popoverController = [[UIPopoverController alloc] initWithContentViewController:historyViewController];
    [popoverController setPopoverContentSize:[historyViewController.view frame].size];
    [popoverController presentPopoverFromRect:buttonFrame inView:self.view permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
     */
    if(popoverController == NULL)
    {
        CGRect rt = [singleTextView accessibilityFrame];
        CGRect grt = CGRectMake(m_nPosition, rt.origin.y+40, ZWidth, ZHeight);
        popoverController = [[UIPopoverController alloc] initWithContentViewController:historyViewController];
        [popoverController setDelegate:self];
        [popoverController setPopoverContentSize:[historyViewController.view frame].size];
        [m_hDb Select:@"%" UseCount:0 LastTime:NULL];
        
        [historyArray removeAllObjects];
        NSString *temp;
        while([m_hDb Get:NULL UseCount:NULL LastTime:NULL Data:&temp])
        {
            [historyArray addObject:temp];
        }
        [(UITableView*)(historyViewController.view) reloadData];
        [popoverController presentPopoverFromRect:grt inView:self.view permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
    }
    else
    {
        if([popoverController isPopoverVisible])
            [popoverController dismissPopoverAnimated:YES];
        else
        {
            [m_hDb Select:@"%" UseCount:0 LastTime:NULL];
            
            [historyArray removeAllObjects];
            NSString *temp;
            while([m_hDb Get:NULL UseCount:NULL LastTime:NULL Data:&temp])
            {
                [historyArray addObject:temp];
            }
            [(UITableView*)(historyViewController.view) reloadData];
            CGRect rt = [singleTextView accessibilityFrame];
            CGRect grt = CGRectMake(m_nPosition, rt.origin.y+40, ZWidth, ZHeight);
            [popoverController presentPopoverFromRect:grt inView:self.view permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
            
        }
    }
}

//- (IBAction)onDone:(UIButton*)aButton
//{
//    [self doneEditing];
//}

- (void)doneEditing
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    if (doneEdit)
        return;
    doneEdit = YES;
    
    NSString* text = [singleTextView text];
    //新的存储
    if(text.length>0)
        [m_hDb Add:text];
    KeyBoardMiddleLayer::getInstance()->endInput(singleTextView);
    
    [self hideKeyBoard];
	
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
}

- (void)viewDidUnload {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardDidShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardWillHideNotification object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:UITextFieldTextDidChangeNotification object:nil];
    if (iosversion>4.9) {
        [[NSNotificationCenter defaultCenter] removeObserver:self name:UIKeyboardDidChangeFrameNotification object:nil];
    }
    if(popoverController!=NULL)
    {
        [popoverController release];
        popoverController=NULL;
    }
    if(m_hDb!=NULL)
    {
        [m_hDb release];
        m_hDb=NULL;
    }
    if(historyArray!=NULL)
        [historyArray release];
    historyArray=NULL;
    [super viewDidUnload];
}


- (void)dealloc 
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	[singleTextView release];
#ifdef __USING_SPEECH_INPUT__
    [speechRecognizerView release];
    [blackLineView release];
#endif
    [super dealloc];
}

#pragma mark -
#pragma mark notificationCenter
- (void)keyboardDidShow:(NSNotification *)notification
{
    NSDictionary *userInfo = [notification userInfo];
    
    // Get the origin of the keyboard when it's displayed.
    NSValue* aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
    NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
    NSTimeInterval animationDuration;
    [animationDurationValue getValue:&animationDuration];
	
    // Get the top of the keyboard as the y coordinate of its origin in self's view's coordinate system. The bottom of the text view's frame should align with the top of the keyboard's final position.
    CGRect keyboardRect = [aValue CGRectValue];
    
    keyboardRect = [viewController.view convertRect:keyboardRect fromView:nil];
#ifdef __USING_SPEECH_INPUT__
    inputKeyboardRect = CGRectMake(0, 0, keyboardRect.size.width, keyboardRect.size.height);
#endif
    [viewController setKeyBoardFrame:keyboardRect withDuration:animationDuration isKeyBoard:EShow];
}

- (void)keyboardWillHide:(NSNotification *)notification
{
    NSDictionary *userInfo = [notification userInfo];
    NSValue* aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
    CGRect keyboardRect = [aValue CGRectValue];
    keyboardRect = [viewController.view convertRect:keyboardRect fromView:nil];
    keyboardRect.origin.y = 768;
    NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
    NSTimeInterval animationDuration;
    [animationDurationValue getValue:&animationDuration];
    [viewController setKeyBoardFrame:keyboardRect withDuration:animationDuration isKeyBoard:EHide];
}

#pragma mark -
#pragma mark UITextField Delegate
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
#ifdef __USING_SPEECH_INPUT__
    if (!g_deviceData.m_isPad)
    {
        textField.layer.borderColor = [[UIColor lightGrayColor] CGColor];
        textField.layer.borderWidth = 1.0;
    }
#endif
    return YES;
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    [popoverController dismissPopoverAnimated:YES];
    [self doneEditing];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    
    return YES;
}

#pragma mark -
#pragma mark UITextView Delegate
- (BOOL)textViewShouldBeginEditing:(UITextView *)textView
{
    return YES;
}

- (BOOL)textViewShouldEndEditing:(UITextView *)aTextView
{
    [aTextView resignFirstResponder];
    return YES;
}

- (void)textViewDidEndEditing:(UITextView *)textView
{
    
    [self doneEditing];
}

#pragma mark -
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSUInteger row = [indexPath row];
	UITableViewCell* cell = nil;
	
	static NSString* id_str = @"history";
	cell = [(UITableView*)historyViewController.view dequeueReusableCellWithIdentifier:id_str];
	if (cell == nil)
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:id_str] autorelease];

    int rowint = row;
    int countint = [historyArray count];
    
    if (rowint>=countint)
        return cell;
    //	cell.textLabel.text = [historyArray objectAtIndex:[historyArray count]-row-1];
    cell.textLabel.text = [historyArray objectAtIndex:row];
	cell.textLabel.font = [UIFont boldSystemFontOfSize:25];
	
	return cell;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [historyArray count];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    NSString* string = [historyArray objectAtIndex:[indexPath row]];
    
    //    NSString* oldString = [singleTextView text];
    //    [singleTextView setText:[NSString stringWithFormat:@"%@%@", oldString, string]];
    [singleTextView setText:string];
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    [popoverController dismissPopoverAnimated:YES];
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popover
{
    
}
#ifdef __USING_SPEECH_INPUT__
-(void)keyboardButtonPressed:(id)sender
{
    singleTextView.inputView = nil;
    [singleTextView reloadInputViews];
    keyboardButton.hidden = YES;
    micButton.hidden = NO;
    [speechRecognizerView onSelfDisAppear];
    void* mobileView = g_explorerController_ios.clientView.mobileView;
    g_setSoundMixSilence(mobileView, false);
}

-(void)micPressed:(id)sender
{
    void* mobileView = g_explorerController_ios.clientView.mobileView;
    g_setSoundMixSilence(mobileView, true);
    speechRecognizerView.speechDelegate = self;
    speechRecognizerView.frame = inputKeyboardRect;
    [speechRecognizerView showInTextView:singleTextView];
    keyboardButton.hidden = NO;
    micButton.hidden = YES;
}

- (void)getResultText:(NSString *)text
{
    [singleTextView setText:[NSString stringWithFormat:@"%@%@", singleTextView.text,text]];
}
#endif
@end
