//
//  LocalViewController_iPhone.h
//  BaiWan
//
//  Created by JQBar on 11-7-15.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@class BaiWanViewController_ios;

@interface web_localViewController_ios : UIViewController<UITableViewDelegate, UITableViewDataSource> {

	UITableView* fileList;
	UIBarButtonItem* editButton;
	UIBarButtonItem* completeButton;
    
	NSMutableArray* fileLocationArray;
	NSMutableArray* fileNameArray;

	void* mobileView;
	BaiWanViewController_ios* mainViewController;
}

//@property (nonatomic, retain)IBOutlet UITableView* fileList;
//@property (nonatomic, retain)IBOutlet UIBarButtonItem* editButton;
//@property (nonatomic, retain)IBOutlet UIBarButtonItem* completeButton;

- (void)initData;

- (void)onEditButton;
- (void)onDoneButton;

- (void)setMobileView:(void*)m;
- (void*)mobileView;

@end
