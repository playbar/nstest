//
//  main.m
//  BaiWan
//
//  Created by handong on 11-5-9.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AppDelegate_ios.h"
#if BWPLAYNOTFLASHGAME
#import "ShareGameInstance.h"
#endif
int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
#if BWPLAYNOTFLASHGAME
    [[ShareGameInstance sharedGame] initInMainFunction];
#endif
     int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate_ios");
    [pool release];
    return retVal;
}
