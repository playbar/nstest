//
//  ResMaker.h
//  ResMaker
//
//  Created by JQBar on 11-10-8.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface ResMaker : NSObject {

	NSThread* workThread;
	
	NSButton* startButton;
	NSTextField* srsPathField;
	NSTextField* dstPathField;
	NSTextField* msgBoxField;
	NSProgressIndicator* progressIndicator;
	
	BOOL isRunning;
	
	NSAutoreleasePool* pool;
}

@property (nonatomic, retain) IBOutlet NSButton* startButton;
@property (nonatomic, retain) IBOutlet NSTextField* srsPathField;
@property (nonatomic, retain) IBOutlet NSTextField* dstPathField;
@property (nonatomic, retain) IBOutlet NSTextField* msgBoxField;
@property (nonatomic, retain) IBOutlet NSProgressIndicator* progressIndicator;

- (void)start;
- (void)stop;
- (void)trace:(NSString*)msg;
- (void)setPercent:(int)percent;

- (IBAction)clickStartButton:(id)sender;

@end
