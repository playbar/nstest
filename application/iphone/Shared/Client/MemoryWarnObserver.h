//
//  MemoryWarnObserver.h
//  BaiWan
//
//  Created by yock on 12-6-12.
//  Copyright (c) 2012年 JQBar. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol MemoryWarnObserver <NSObject>
@required

-(bool) onMemoryWarn:(int)bytecount;
-(void) onWarnBeProcess;

@end
