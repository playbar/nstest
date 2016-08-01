//
//  SendTouchButton.m
//  ExpandingBarExample
//
//  Created by xiepf on 14-7-23.
//  Copyright (c) 2014年 Topic Design. All rights reserved.
//

#import "SendTouchButton.h"

@implementation SendTouchButton

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        
       // NSBundle *bundle = [NSBundle bundleWithURL:[[NSBundle mainBundle] URLForResource:@"BaiWanPlatformKit" withExtension:@"bundle"]];
        // Initialization code
      //  NSString *image_suffix;
//        if ([BWDeviceManager isiphone])
//        {
//            image_suffix = @"_phone";
//        }
//        else
//        {
//            image_suffix = @"_pad";
//        }
        
       

        // Initialization code
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/
-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
   
    if (timer)
    {
        [timer invalidate];
        timer=nil;
    }
    [self.delegate SenTouchButtonTouchBegain];
    [[self superview] touchesBegan:touches withEvent:event];
     [self setImage :[UIImage imageNamed:@"BWFloatToolButtonHight.png"]forState:UIControlStateNormal];
    [self.delegate BarAlphaChangeIncrease:YES];
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    [[self superview] touchesMoved:touches withEvent:event];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    //if (self.buttonIsShow>0) {
        [[self superview] touchesEnded:touches withEvent:event];
        [self setImage:[UIImage  imageNamed:@"BWFloatToolButton.png"] forState:UIControlStateNormal];
        timer=[NSTimer scheduledTimerWithTimeInterval:4 target:self selector:@selector(barAlpha) userInfo:nil repeats:NO];
    //}
    
    
}

-(void)barAlpha
{
    if (!_buttonIsShow)
    {
         [self.delegate BarAlphaChangeIncrease:NO];
    }
    timer=nil;
}
@end
