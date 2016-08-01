//
//  ResMakerAppDelegate.h
//  ResMaker
//
//  Created by JQBar on 11-10-8.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ResMaker;

@interface ResMakerAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *window;
	ResMaker* resMaker;
}

@property (assign) IBOutlet NSWindow *window;
@property (nonatomic, retain) IBOutlet ResMaker* resMaker;

@end
