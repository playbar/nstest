//
//  UIDeviceAdvance.h
//  BaiWan
//
//  Created by handong on 11-6-21.
//  Copyright 2011 JQBar. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIDevice (machine) 
- (NSString*)machine;
- (NSString*)MACAddress;
- (NSString*)WIFiIpAddress;
-(NSString *)idfvUDID;
@end

@interface UIDevice (memory)
- (unsigned int)freeMemory;
- (unsigned int)activeMemory;
- (unsigned int)inactiveMemory;
- (unsigned int)totalMemory;
- (unsigned int)usedMemory;
@end


@interface NSData (NSData_Convert)

- (NSString*)stringWithHexData;

@end