//
//  SDKInstance.m
//  BaiWan
//
//  Created by imac1 on 13-9-25.
//  Copyright (c) 2013年 JQBar. All rights reserved.
//
//#if BWMBSDKMODE
#import "MobisageSDKInstance.h"
#import <objc/runtime.h>

@implementation MobisageSDKInstance

-(void)dealloc
{
    [super dealloc];
}
-(void)initOfferWallWithDelegate:(id)dele
{
    MobiSageOfferWallViewController *owViewController=[[MobiSageOfferWallViewController  alloc ] initWithPublisherID:@"a8300de238a94ae0a7788e75b56d938b"];
    self.delegate = dele;
    
}
-(void)showOfferWallWithAppID:(NSString *)appID andAppSec:(NSString *)appSec andUserID:(NSString *)userID
{
    //1.PublisherID的审核需要时间(未审核的pid是不显示广告的),测试请用当前的ID(a8300de238a94ae0a7788e75b56d938b).测试完成后更换自己应用申请的PID.
    //2.实例化对象一定要在程序开始时初始化,并且网络畅通.因为需要读取网络配置开关.否则加载广告会失败!
    MobiSageOfferWallViewController *owViewController=[[MobiSageOfferWallViewController  alloc ] initWithPublisherID:@"a8300de238a94ae0a7788e75b56d938b"];
    [owViewController setURLScheme:@"sample"];
    ((UIViewController *)self.delegate).view.transform = CGAffineTransformMakeRotation(0);
    [owViewController presentOfferWallWithViewController:self.delegate];

//    NSString *className = NSStringFromClass([MobiSageOfferWallViewController class]);
//    const char *cClassName = [className UTF8String];
//    
//    id theClass = objc_getClass(cClassName);
//    
//    unsigned int outCount;
//    
//    
//    Method *m =  class_copyMethodList(theClass,&outCount);
//    
//    NSLog(@"函数个数：%d",outCount);
//    for (int i = 0; i<outCount; i++) {
//        SEL a = method_getName(*(m+i));
//        NSString *sn = NSStringFromSelector(a);
//        NSLog(@"%@",sn);
//    }
}

-(void)offerWallDidFinishLoad:(MobiSageOfferWallViewController *)owInterstitial
{
    
}
- (void)OfferWallInterstitialWillPresentScreen:(MobiSageOfferWallViewController *)owInterstitial
{
    
}
-(void)checkPointWithUserID:(NSString *)userID
{
    
}
@end
//#endif