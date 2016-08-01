//
//  SpeechAlertView.m
//  rhljiayouTest
//
//  Created by iHope on 13-8-7.
//  Copyright (c) 2013年 iHope. All rights reserved.
//

#import "SpeechRecognizerView.h"
#import "IFLYTEKDefinition.h"
#import "RecognizerFactory.h"
#import "iflyMSC/IFlySpeechUtility.h"
#import "iflyMSC/IFlySpeechRecognizer.h"
// timeout      连接超时的时间，以ms为单位，毫秒，符号ms ，1000 毫秒 = 1秒，30000=30秒
//timeout:网络超时时间,单位:ms,默认为20000,范围0-30000
@implementation SpeechRecognizerView

-(id)initWithFrame:(CGRect)frame
{
    id returned = [super initWithFrame:frame];
    if(returned)
    {
        self.backgroundColor = [UIColor whiteColor];
        speechImage = [[UIImageView alloc]initWithFrame:CGRectMake((frame.size.width-291)/2, (frame.size.height-291)/2, 291, 218)];
        speechImage.image = [UIImage imageNamed:@"0_mic.png"];
        [self addSubview:speechImage];
        blackLineView = [[UIView alloc]init];
        blackLineView.backgroundColor = [UIColor blackColor];
        [self addSubview:blackLineView];
        
        // 创建识别对象
        //创建语音配置
        NSString *initString = [[NSString alloc] initWithFormat:@"appid=%@,timeout=%@",APP_ID,TIMEOUT];
        
        //所有服务启动前，需要确保执行createUtility
        [IFlySpeechUtility createUtility:initString];
        [initString release];
        _iFlySpeechRecognizer = [RecognizerFactory CreateRecognizer:self Domain:@"iat"];
        
        _popUpView = [[PopupView alloc]initWithFrame:CGRectMake(100, 100, 0, 0)];
        _popUpView.ParentView = self;
    }
    return returned;
}
//重新写界面内容
- (void) layoutSubviews
{
    CGFloat width = self.frame.size.width;
    CGFloat height = self.frame.size.height;
    
    if ((width < 291)||(height < 218))
    {
        width = 291/2;
        height = 218/2;
    }
    else
    {
        width = 291;
        height = 218;
    }
    
    speechImage.frame = CGRectMake((self.frame.size.width-width)/2, (self.frame.size.height-height)/2, width, height);
    blackLineView.frame = CGRectMake(0, 0, self.frame.size.width, 1);
}

//显示
-(void)showInTextView:(UITextField *)textView
{
    speechImage.image = [UIImage imageNamed:@"0_mic.png"];
    textView.inputView = self;
    [textView reloadInputViews];
    [_iFlySpeechRecognizer setDelegate: self];
    bool ret = [_iFlySpeechRecognizer startListening];
    if (ret)
    {
    }
    else
    {
        [_popUpView setText: @"启动识别服务失败，请稍后重试" andNeedToReturnToKeyBoard:true];//可能是上次请求未结束
        [self addSubview:_popUpView];
    }
}

-(void)onSelfDisAppear
{
    [_iFlySpeechRecognizer stopListening];
    [_iFlySpeechRecognizer setDelegate: nil];
}

-(void)dealloc
{
    [_popUpView release];
    [speechImage release];
    [blackLineView release];
    [_iFlySpeechRecognizer setDelegate: nil];
    //[_iFlySpeechRecognizer release];
    [super dealloc];
}
#pragma mark - IFlySpeechRecognizerDelegate
/**
 * @fn      onVolumeChanged
 * @brief   音量变化回调
 *
 * @param   volume      -[in] 录音的音量，音量范围1~100
 * @see
 */
- (void) onVolumeChanged: (int)volume
{
    //录音的音量，音量范围1~100
    if (volume>=0 &&volume<=5)
        speechImage.image = [UIImage imageNamed:@"0_mic.png"];
    else if(volume>5 && volume<=10)
        speechImage.image = [UIImage imageNamed:@"1_mic.png"];
    else if(volume>10 && volume<=15)
        speechImage.image = [UIImage imageNamed:@"2_mic.png"];
    else if(volume>15 && volume<=25)
        speechImage.image = [UIImage imageNamed:@"3_mic.png"];
    else
        speechImage.image = [UIImage imageNamed:@"4_mic.png"];
}

/**
 * @fn      onBeginOfSpeech
 * @brief   开始识别回调
 *
 * @see
 */
- (void) onBeginOfSpeech
{
    [_popUpView setText: @"开始录音" andNeedToReturnToKeyBoard:false];
    [self addSubview:_popUpView];
}

/**
 * @fn      onEndOfSpeech
 * @brief   停止录音回调
 *
 * @see
 */
- (void) onEndOfSpeech
{
    speechImage.image = [UIImage imageNamed:@"0_mic.png"];
    [_popUpView setText: @"停止录音" andNeedToReturnToKeyBoard:true];
    [self addSubview:_popUpView];
}


/**
 * @fn      onError
 * @brief   识别结束回调
 *
 * @param   errorCode   -[out] 错误类，具体用法见IFlySpeechError
 */
- (void) onError:(IFlySpeechError *) error
{
    speechImage.image = [UIImage imageNamed:@"0_mic.png"];
    NSString *text ;

    if (error.errorCode ==0 )
    {
        if (_result.length==0)
        {
            text = @"无识别结果";
        }
        else
        {
            text = @"识别成功";
        }
    }
    else
    {
        text = [NSString stringWithFormat:@"发生错误：%@",error.errorDesc];
        [_popUpView setText: text andNeedToReturnToKeyBoard:true];
        [self addSubview:_popUpView];
    }
}

/**
 * @fn      onResults
 * @brief   识别结果回调
 *
 * @param   result      -[out] 识别结果，NSArray的第一个元素为NSDictionary，NSDictionary的key为识别结果，value为置信度
 * @see
 */
- (void) onResults:(NSArray *) results isLast:(BOOL)isLast
{
    NSMutableString *resultString = [[NSMutableString alloc] init];
    NSDictionary *result = results[0];
    for (NSString *key in result) {
        [resultString appendFormat:@"%@",key];
    }
    NSMutableString *showString = [[NSMutableString alloc] init];
    NSError *error;
    NSDictionary *dicData = [NSJSONSerialization JSONObjectWithData:[resultString dataUsingEncoding:NSUTF8StringEncoding] options:0 error:&error];
    NSArray *ws = [dicData objectForKey:@"ws"];
    for (NSDictionary *dic in ws)
    {
        NSArray * cw = [dic objectForKey:@"cw"];
        for (NSDictionary *w in cw)
        {
            [showString appendFormat:@"%@",[w objectForKey:@"w"]];
        }
    }
    
    [resultString release];
    if (_result)
    {
        [_result release];
        _result = nil;
    }
    _result = showString;
    [_speechDelegate getResultText:showString];
}

/**
 * @fn      onCancel
 * @brief   取消识别回调
 * 当调用了`cancel`函数之后，会回调此函数，在调用了cancel函数和回调onError之前会有一个短暂时间，您可以在此函数中实现对这段时间的界面显示。
 * @param
 * @see
 */
- (void) onCancel
{
    speechImage.image = [UIImage imageNamed:@"0_mic.png"];
}
-(void)changeToKeyboard
{
    [_speechDelegate keyboardButtonPressed:nil];
}
@end
