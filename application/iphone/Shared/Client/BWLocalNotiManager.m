//
//  BWLocalNotiManager.m
//  BaiWan
//
//  Created by xiepf on 14-9-15.
//  Copyright (c) 2014年 JQBar. All rights reserved.
//

#import "BWLocalNotiManager.h"

// 1年，2月，3日，4小时，5分钟，6周
static int numToScheduleInterval[6] =
{
    NSCalendarUnitYear,
    NSCalendarUnitMonth,
    NSCalendarUnitDay,
    NSCalendarUnitHour,
    NSCalendarUnitMinute,
    NSCalendarUnitWeekday
};

static BWLocalNotiManager* bwLocalNotiManager = nil;

@interface BWLocalNotiManager()
{
    BOOL _loadSuceed;
}
@end

@implementation BWLocalNotiManager
+(BWLocalNotiManager *)sharedInstance
{
    
    if (!bwLocalNotiManager)
    {
        bwLocalNotiManager = [[BWLocalNotiManager alloc]init];
    }
    return bwLocalNotiManager;
}

-(id)init
{
    id returned = [super init];
    if (returned)
    {
        _loadSuceed = NO;
    }
    return returned;
}

-(void)dealloc
{
    self.requestData = nil;
    self.requestConnection = nil;
    [super dealloc];
}

-(void)startLoadingNotificationMessage
{
    [self loadRequest];
}

- (void)loadRequest
{
    if (_loadSuceed)
        return;
    NSURL *url = [NSURL URLWithString:@"http://bwtest.jqbar.com/iospush/iospush.aspx"];
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    self.requestConnection=[NSURLConnection connectionWithRequest:request delegate:self];
}

-(void)startNotification:(NSDictionary *)paramDic
{
    //是否取消之前所有的本地通知
    if ([[[[paramDic objectForKey:@"CancelFlag"] objectAtIndex:0]objectForKey:@"flag"]intValue])
    {
        [[UIApplication sharedApplication] cancelAllLocalNotifications];
        //清空 icon数量
        [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
    }
    
    NSArray *pushArray = [paramDic objectForKey:@"TableInfo"];
    for (id pushTemp in pushArray)
    {
        //启动本地通知
        UILocalNotification *notification=[[UILocalNotification alloc] init];
        if (notification!=nil)
        {
            //现在的时间
            NSDate *now=[NSDate date];
            //获得push生效的时间
            NSString  * pushDateString =  [pushTemp objectForKey:@"time"];
            //根据时间字符串获得NSDate
            NSDateFormatter  *dateformatter=[[NSDateFormatter alloc] init];
            [dateformatter setDateFormat:@"YYYY-MM-dd HH:mm"];
            NSDate  * pushDate=[dateformatter dateFromString:pushDateString];
            [dateformatter release];
            //然后比较  now跟  todayTwelve那个大，如果已经过了12点，那就设置明天12点
            NSComparisonResult   dateResult =  [now  compare:pushDate ];
            if ((dateResult ==  NSOrderedDescending)&&([[pushTemp objectForKey:@"repeat"]intValue]==0))
            {
                continue;
            }
            
            if (dateResult ==  NSOrderedDescending)
            {
                NSDate  *  tomorrowPushDate =  [pushDate  dateByAddingTimeInterval: 24 * 60 * 60];
                
                notification.fireDate =  tomorrowPushDate;
            }
            else
            {
                notification.fireDate= pushDate;
            }
            
            // 1年，2月，3日，4小时，5分钟，6周
            notification.repeatInterval = numToScheduleInterval[[[pushTemp objectForKey:@"repeat"]intValue]-1];
            notification.timeZone=[NSTimeZone defaultTimeZone];
            notification.applicationIconBadgeNumber ++;
            notification.alertBody=[pushTemp objectForKey:@"content"];
            notification.alertAction = [pushTemp objectForKey:@"action"];
            [[UIApplication sharedApplication] scheduleLocalNotification:notification];
            NSLog(@"%@",notification);
            [notification release];
        }
    }
}

// 网络请求代理方法
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
	NSMutableData *temp = [[NSMutableData alloc] init];
    self.requestData = temp;
    [temp release];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    [self.requestData appendData:data];
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    if (!_loadSuceed)
    {
        [self performSelector:@selector(loadRequest) withObject:nil afterDelay:20];
    }
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    _loadSuceed = YES;
    //NSDictionary *dicTemp = [NSJSONSerialization JSONObjectWithData:self.requestData options:NSJSONReadingMutableLeaves error:nil];
        NSString *str1 = @"{\"TableInfo\":[{\"time\":\"2014-09-23 14:55\",\"repeat\":\"1\",\"content\":\"年push内容\",\"action\":\"打开年push\"}, { \"time\":\"2014-09-23 16:00\",\"repeat\":\"2\",\"content\":\"月push内容\",\"action\":\"打开月push\"}, { \"time\":\"2014-09-23 17:00\",\"repeat\":\"3\",\"content\":\"日push内容\",\"action\":\"打开日push\"}, { \"time\":\"2014-09-23 18:00\",\"repeat\":\"4\",\"content\":\"小时push内容\",\"action\":\"打开小时push\"}, { \"time\":\"2014-09-23 19:00\",\"repeat\":\"6\",\"content\":\"周push内容\",\"action\":\"打开周push\"}],\"CancelFlag\":[{\"flag\":\"1\"}]}";
    NSDictionary *dicTemp = [NSJSONSerialization JSONObjectWithData:[str1 dataUsingEncoding:NSUTF8StringEncoding] options:NSJSONReadingMutableLeaves error:nil];
    [self startNotification:dicTemp];
}

@end
