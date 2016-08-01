//
//  NetWorkDetector.h
//  BaiWan
//
//  Created by lining on 10-11-24.
//  Modified by handong on 11-05-10.
//  Copyright 2010 JQBar. All rights reserved.
//

#import <Foundation/Foundation.h>

@class Reachability;

@interface NetWorkDetector : NSObject {
	Reachability* m_reachability;
	NSURLConnection* m_connection;
}

+ (NetWorkDetector*) sharedInstance;

- (BOOL) canConnected;
- (BOOL) needConnected;

- (void) start;
- (void) stop;

@end
