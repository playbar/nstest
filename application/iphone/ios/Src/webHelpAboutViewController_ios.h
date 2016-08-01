//
//  webHelpAboutViewController.h
//  BaiWan
//
//  Created by imac1 on 13-10-23.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>
typedef enum{WEB_PAGE_TYPE_HELP = 1, WEB_PAGE_TYPE_ABOUT } WEB_PAGE_TYPE;

@interface webHelpAboutViewController_ios : UIViewController
{
    BOOL baiwanInfoViewIsShow;
    UITextView* baiwanInfoView;
    WEB_PAGE_TYPE m_pageType;
}
-(id)initWithStyle:(WEB_PAGE_TYPE)pagetype;
@end
