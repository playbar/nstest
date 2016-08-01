    //
//  inputViewController_iPad.m
//  BaiWan
//
//  Created by handong on 11-6-13.
//  Copyright 2011 JQBar. All rights reserved.
//

#import "inputViewController_iPad.h"
#import "BaiWanViewController_iPad.h"
#import "historyViewController_iPad.h"
#import "ClientView_iPad.h"


#include "KeyBoardMiddleLayer.h"
#include "IPhoneDeviceData.h"
#include "XEdit.h"

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

extern BaiWanViewController_iPad* g_explorerController_iPad;
extern iOSDeviceRelateData g_deviceData;


@interface inputViewController_iPad()

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

@implementation inputViewController_iPad
@synthesize popoverController;
@synthesize m_bIsShow;
@synthesize historyViewController;
@synthesize viewController;
@synthesize singleTextView;

extern iOSDeviceRelateData g_deviceData;

- (void)viewDidLoad 
{
    [super viewDidLoad];
    
    historyArray = [[NSMutableArray alloc] init];
    historyViewController = [[historyViewController_iPad alloc] initWithNibName:@"historyViewController_iPad" bundle:nil];
    [(UITableView*)historyViewController.view setDataSource:self];
    [(UITableView*)historyViewController.view setDelegate:self];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardDidShow:) name:UIKeyboardDidShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
    //设置输入框监听
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onTextChanged:) name:UITextFieldTextDidChangeNotification object:NULL];
    [[UIDevice currentDevice] systemVersion];
    if(iosversion>4.9)
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyBoardChangeRect:) name:UIKeyboardDidChangeFrameNotification object:NULL];
    m_hDb = NULL;
    m_nSLen = ZNoneLen;
    popoverController = NULL;
    m_iLastPoint.x=0;
    m_iLastPoint.y=0;
    m_bNeedShow = false;
    m_bNeedHide = false;
    m_bIsShow = false;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
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
    if(g_explorerController_iPad)
    {
        m_iLastPoint = g_explorerController_iPad.clientView.lastPoint;
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
    CGPoint beginCenterP = [[[sender userInfo] valueForKey:UIKeyboardCenterBeginUserInfoKey] CGPointValue];
    CGPoint endCenterP = [[[sender userInfo] valueForKey:UIKeyboardCenterEndUserInfoKey] CGPointValue];
    CGRect  keyBoardBoundR = [[[sender userInfo] valueForKey:UIKeyboardBoundsUserInfoKey] CGRectValue];
    CGRect  keyBoardFrameR = [[[sender userInfo] valueForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    NSLog(@"orgin x=%f y=%f size w=%f h=%f",keyBoardFrameR.origin.x,keyBoardFrameR.origin.y,keyBoardFrameR.size.width,keyBoardFrameR.size.height);
    UIViewAnimationCurve animCur = (UIViewAnimationCurve)[[[sender userInfo] valueForKey:UIKeyboardAnimationCurveUserInfoKey] intValue];
    NSTimeInterval animDuration = [[[sender userInfo] valueForKey:UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    

    if(m_bIsShow)
    {
        keyBoardFrameR = [viewController.view convertRect:keyBoardFrameR fromView:nil];
        [viewController setKeyBoardFrame:keyBoardFrameR withDuration:animDuration isKeyBoard:ESplite];
    }
    
    
    
//    NSDictionary *userInfo = [notification userInfo];
//    NSValue* aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
//    CGRect keyboardRect = [aValue CGRectValue];
//    keyboardRect = [viewController.view convertRect:keyboardRect fromView:nil];
//    keyboardRect.origin.y = 768;
//    NSValue *animationDurationValue = [userInfo objectForKey:UIKeyboardAnimationDurationUserInfoKey];
//    NSTimeInterval animationDuration;
//    [animationDurationValue getValue:&animationDuration];
//    [viewController setKeyBoardFrame:keyboardRect withDuration:animationDuration isKeyBoard:NO];
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

- (IBAction)onHistory:(UIButton*)aButton
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

- (IBAction)onDone:(UIButton*)aButton
{
    [self doneEditing];
}

- (void)doneEditing
{
    if (doneEdit)
        return;
    doneEdit = YES;
    
    NSString* text = [singleTextView text];
    /*
    if (![historyArray containsObject:text])
    {
        if ([historyArray count]<20)
            [historyArray addObject:[NSString stringWithString:text]];
        else
        {
            [historyArray removeObjectAtIndex:0];
            [historyArray addObject:[NSString stringWithString:text]];
        }
            
        [(UITableView*)historyViewController.view reloadData];
    }
     */
    //新的存储
    if(text.length>0)
        [m_hDb Add:text];
    KeyBoardMiddleLayer::getInstance()->endInput(singleTextView);
        
    [self hideKeyBoard];
	
//	[viewController.clientView cleanupMultiTouchArray];
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
    [[NSNotificationCenter defaultCenter] removeObserver:self name:nil object:nil];
	[singleTextView release];
    
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
	
//	cell.textLabel.text = [historyArray objectAtIndex:[historyArray count]-row-1];

    int rowint = row;
    int countint = [historyArray count];
    
    if (rowint>=countint)
        return cell;
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

@end
