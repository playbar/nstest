
#import "GetStatusBarPara.h"
#import "RNExpandingButtonBar.h"

#define min(x, y) ((x) < (y)? (x): (y))
#define max(x, y) ((x) > (y)? (x): (y))
#define CONSTRAINTDEMI(x, minArg, maxArg) min(max((x),(minArg)), (maxArg))
//#define STARTING_POINT CGPointMake(40, 40)
@interface RNExpandingButtonBar ()
- (void) _expand:(NSDictionary*)properties;
- (void) _close:(NSDictionary*)properties;
typedef enum
{
    LEFTUP=1,
    RIGHTUP,
    LEFTDOWN,
    RIGHTDOWN,
    
    
}POSTION;

@end

@implementation RNExpandingButtonBar
{
    int _cnt;//调用次数
    UIButton *_buttonCorner;
    NSMutableArray *_arrButtons;
    NSUInteger _iNumberOfButtons;
    float _fButtonSize;
    float _fInnerRadius;
    POSTION SelfPostion;
    CGPoint STARTING_POINT;
    NSArray *titileArray;
    NSArray *buttonImage;
    NSArray *buttonHightImage;
    SendTouchButton *defaultButton;
    NSTimer *timer;
}
- (id) initWithImage:(UIImage*)image selectedImage:(UIImage*)selectedImage toggledImage:(UIImage*)toggledImage toggledSelectedImage:(UIImage*)toggledSelectedImage buttons:(NSArray*)buttons center:(CGPoint)center;
{
    if (self = [super init]) {
        [self setDefaults];
        
        // Reverse buttons so it makes since for top/bottom
        NSArray *reversedButtons = [[buttons reverseObjectEnumerator] allObjects];
        [self setButtons:reversedButtons];
       buttonHightImage=[[NSArray alloc]initWithObjects:@"BWFloatToolRefreshButton",@"BWFloatToolpayButon",@"BWFloatToolExitButton",nil];
         buttonImage=[[NSArray alloc]initWithObjects:@"BWFloatToolRefreshButtonHight",@"BWFloatToolpayButonHight",@"BWFloatToolExitButtonHight",nil];
        // Button location/size settings
       // CGRect buttonFrame = CGRectMake(0, 0, [image size].width, [image size].height);
        CGRect buttonFrame = CGRectMake(0, 0, 70,70);
        //CGPoint buttonCenter = CGPointMake([image size].width / 2.0f, [image size].height / 2.0f);
        CGPoint buttonCenter = CGPointMake(35, 35);
        defaultButton = [[SendTouchButton alloc] initWithFrame:buttonFrame];
        defaultButton.delegate=self;
        [defaultButton setCenter:buttonCenter];
        [defaultButton setImage:image forState:UIControlStateNormal];
       // [defaultButton setImage:selectedImage forState:UIControlStateHighlighted];
        [defaultButton addTarget:self action:@selector(onButton:) forControlEvents:UIControlEventTouchUpInside];
        [self setButton:defaultButton];
        UIButton *toggledButton = [[UIButton alloc] initWithFrame:buttonFrame];
        toggledButton.tag=500;
        [toggledButton setCenter:buttonCenter];
        [toggledButton setImage:toggledSelectedImage forState:UIControlStateNormal];
        [toggledButton addTarget:self action:@selector(onToggledButton:) forControlEvents:UIControlEventTouchUpInside];
        // Init invisible
        [toggledButton setAlpha:0.0f];
        [self setToggledButton:toggledButton];
        //
        //        for (int i = 0; i < [buttons count]; ++i) {
        //            UIButton *button = (UIButton*)[buttons objectAtIndex:i];
        //            [button addTarget:self action:@selector(explode:) forControlEvents:UIControlEventTouchUpInside];
        //            [button setCenter:buttonCenter];
        //            [button setAlpha:0.0f];
        //            [self addSubview:button];
        //        }
        _iNumberOfButtons = buttons.count ;
        _buttonCorner = [UIButton buttonWithType:UIButtonTypeCustom];
        // [_buttonCorner setImage:[UIImage imageNamed:_strCornerButtonImageName] forState:UIControlStateNormal];
        [_buttonCorner addTarget:self action:@selector(hideMenu:) forControlEvents:UIControlEventTouchUpInside];
        [_buttonCorner setFrame:CGRectMake(0, 20, 40, 40)];
        
        //Circular menu buttons
        _arrButtons = [[NSMutableArray alloc] init];
        for (int i = 0; i < _iNumberOfButtons; i++)
        {
            UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
            [button setTag:i];
            [button addTarget:self action:@selector(hideMenu:) forControlEvents:UIControlEventTouchUpInside];
            // [button setImage:[UIImage imageNamed:[_arrButtonImageName objectAtIndex:i]] forState:UIControlStateNormal];
            [button setFrame:CGRectMake(0, 0,50, 50)];
            button.layer.cornerRadius=25;
            [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:i]] forState:UIControlStateNormal];
            [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:i]] forState:UIControlStateHighlighted];
            [_arrButtons addObject:button];
        }
        
        
        // Container view settings
       // [self setBackgroundColor:[UIColor greenColor]];
        [self setFrame:buttonFrame];
        [self setCenter:center];
        
        [self addSubview:[self button]];
        [self addSubview:[self toggledButton]];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(statusBarFrameOrOrientationChanged:) name:UIApplicationDidChangeStatusBarOrientationNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(statusBarFrameOrOrientationChanged:) name:UIApplicationDidChangeStatusBarFrameNotification object:nil];
        windowBounds = [GetStatusBarPara gainWindowBoundsWithoutStatusBar];
        [self statusBarFrameOrOrientationChanged:nil];
    }
    SelfPostion=LEFTUP;
    return self;
}

- (void) setDefaults
{
    STARTING_POINT=CGPointMake(35, 35);
    _fButtonSize = 35;//circular button width/height
    _fInnerRadius = 90;//1st circle boundary
    _fadeTime = 0.2f;
    _animationTime = 0.4f;
    _padding = 15.0f;
    _far = 15.0f;
    _near = 7.0f;
    _delay = 0.1f;
    
    _toggled = NO;
    _spin = NO;
    _horizontal = NO;
    _animated = YES;
}
#pragma mark-SendTouchButtonDelegate
-(void)BarAlphaChangeIncrease:(BOOL)isIncrese
{
    
   
    if (isIncrese)
    {
        [UIView setAnimationDuration:2];
        [UIView beginAnimations:nil context:nil];
        self.alpha=1;
        [UIView commitAnimations];
    }
    else if(!defaultButton.buttonIsShow)
    {
        [UIView setAnimationDuration:2];
        [UIView beginAnimations:nil context:nil];
        self.alpha=0.5;
        [UIView commitAnimations];
    }
}
-(void)SenTouchButtonTouchBegain
{
    if (timer)
    {
        [timer invalidate];
        timer=nil;
    }
    [self.delegate BarTouchBegain];
}
- (void) onButton:(UIButton *)sender
{
    defaultButton.buttonIsShow=YES;
    [self showButtonsAnimated:_animated];
}

- (void) onToggledButton:(id)sender
{
    [self hideButtonsAnimated:_animated];
}

- (void) showButtons
{
   
    [self showButtonsAnimated:NO];
}

- (void) hideButtons
{
    [self hideButtonsAnimated:NO];
    defaultButton.buttonIsShow=NO;
}

- (void) toggleMainButton
{
    UIButton *animateTo;
    UIButton *animateFrom;
    if (_toggled) {
        animateTo = [self button];
        animateFrom = [self toggledButton];
    }
    else {
        animateTo = [self toggledButton];
        animateFrom = [self button];
    }
    [UIView animateWithDuration:_fadeTime animations:^{
        [animateTo setAlpha:1.0f];
        [animateFrom setAlpha:0.0f];
    }];
}

//- (void) explode:(id)sender
//{
//    if (! _explode) return;
//    UIView *view = (UIView*)sender;
//    CGAffineTransform scale = CGAffineTransformMakeScale(5.0f, 5.0f);
//    CGAffineTransform unScale = CGAffineTransformMakeScale(1.0f, 1.0f);
//    [UIView animateWithDuration:0.3 animations:^{
//        [view setAlpha:0.0f];
//        [view setTransform:scale];
//    } completion:^(BOOL finished){
//        [view setAlpha:1.0f];
//        [view setTransform:unScale];
//    }];
//}
- (void) showButtonsAnimated:(BOOL)animated
{
    if ([self delegate] && [[self delegate] respondsToSelector:@selector(expandingBarWillAppear:)]) {
        [[self delegate] expandingBarWillAppear:self];
    }
    [self showButtonsXIN];
    defaultButton.buttonIsShow=YES;
    [self.delegate buttonsShow:YES];
}
-(void)BarAlphaChange
{
    timer=nil;
    [UIView setAnimationDuration:2];
    [UIView beginAnimations:nil context:nil];
    self.alpha=0.5;
    [UIView commitAnimations];
}
- (void)showButtonsXIN
{
    // [self.view addSubview:_buttonCorner];
    
    for (int index = 0; index < _iNumberOfButtons; index++)
    {
        UIButton *button = [_arrButtons objectAtIndex:index];
        button.center = STARTING_POINT;
        button.alpha=1;
        [self.superview addSubview:button];
    }
    
    [self.superview layoutIfNeeded]; // Ensures that all pending layout operations have been completed
    
    [UIView animateWithDuration:0.25 animations:
     ^{
         
         
         [self setButtonFrames];
         [self.superview layoutIfNeeded]; // Forces the layout of the subtree animation block and then captures all of the frame changes
     }];
    _toggled = NO;
    [self toggleMainButton];
}

- (void)setButtonFrames
{
    CGPoint circleCenter = STARTING_POINT;
    
    /*
     Logic : Use parametric equations to set point along circumference of circle
     
     These formulae will give point(x,y) along circumference
     
     x = cx + r * cos(a)
     y = cy + r * sin(a)
     
     Where,
     r is the radius,
     cx,cy the origin,
     and a the angle from 0..2PI radians or 0..360 degrees.
     */
    
    //1st circle initialization
    float incAngle = ( 117/3 )*M_PI/180.0 ;
    float curAngle = 0.1;//more value more to left;
    float circleRadius = _fInnerRadius;
    
    for (int i = 0; i < _iNumberOfButtons; i++)
    {
        if(i == 3)//2nd circle
        {
            curAngle = 0.09;
           // incAngle = ( 115/4 )*M_PI/180.0;
           // circleRadius = _fInnerRadius +65;
        }
        else if(i == 7)//3rd circle
        {
            curAngle = 0.04;
            incAngle = ( 113/5 )*M_PI/180.0;
            circleRadius = _fInnerRadius +(65*2);
        }
        
        CGPoint buttonCenter;
        switch (SelfPostion) {
            case LEFTUP:{

                   buttonCenter.x = circleCenter.x + cos(curAngle)*circleRadius;
                   buttonCenter.y = circleCenter.y + sin(curAngle)*circleRadius;
           

                UIButton *button = [_arrButtons objectAtIndex:i];
                button.center = buttonCenter;
                curAngle += incAngle;
                break;}
            case  LEFTDOWN:
            {
                
                    buttonCenter.x = circleCenter.x + cos(curAngle)*circleRadius;
                    buttonCenter.y = circleCenter.y - sin(curAngle)*circleRadius;
                              UIButton *button = [_arrButtons objectAtIndex:i];
                button.center = buttonCenter;
                curAngle += incAngle;
                break;}
                
            case RIGHTUP:
            {
                                    buttonCenter.x = circleCenter.x - cos(curAngle)*circleRadius;
                
                    buttonCenter.y = circleCenter.y + sin(curAngle)*circleRadius;
               
                UIButton *button = [_arrButtons objectAtIndex:i];
                button.center = buttonCenter;
                curAngle += incAngle;
                break;}
            case RIGHTDOWN:
            {
                
                    buttonCenter.x = circleCenter.x - cos(curAngle)*circleRadius;
                
                    buttonCenter.y = circleCenter.y - sin(curAngle)*circleRadius;
                
                UIButton *button = [_arrButtons objectAtIndex:i];
                button.center = buttonCenter;
                curAngle += incAngle;
                break;}
            default:
                break;
        }
        
        //        buttonCenter.x = circleCenter.x + cos(curAngle)*circleRadius;
        //        buttonCenter.y = circleCenter.y + sin(curAngle)*circleRadius;
        //        UIButton *button = [_arrButtons objectAtIndex:i];
        //        button.center = buttonCenter;
        //        curAngle += incAngle;
    }
}





//- (void) showButtonsAnimated1:(BOOL)animated
//{
//    if ([self delegate] && [[self delegate] respondsToSelector:@selector(expandingBarWillAppear:)]) {
//        [[self delegate] expandingBarWillAppear:self];
//    }
//    float y = [[self button] center].y;
//    float x = [[self button] center].x;
//    float endY = y;
//    float endX = x;
//    for (int i = 0; i < [[self buttons] count]; ++i) {
//        UIButton *button = [[self buttons] objectAtIndex:i];
//        endY -= [self getYoffset:button];
//        endX += [self getXoffset:button];
//        float farY = endY - ( ! _horizontal ? _far : 0.0f);
//        float farX = endX - (_horizontal ? _far : 0.0f);
//        float nearY = endY + ( ! _horizontal ? _near : 0.0f);
//        float nearX = endX + (_horizontal ? _near : 0.0f);
//        if (animated) {
//            NSMutableArray *animationOptions = [NSMutableArray array];
//            if (_spin) {
//                CAKeyframeAnimation *rotateAnimation = [CAKeyframeAnimation animationWithKeyPath:@"transform.rotation.z"];
//                [rotateAnimation setValues:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f],[NSNumber numberWithFloat:M_PI * 2], nil]];
//                [rotateAnimation setDuration:_animationTime];
//                [rotateAnimation setKeyTimes:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f], [NSNumber numberWithFloat:1.0f], nil]];
//                [animationOptions addObject:rotateAnimation];
//            }
//            
//            CAKeyframeAnimation *positionAnimation = [CAKeyframeAnimation animationWithKeyPath:@"position"];
//            [positionAnimation setDuration:_animationTime];
//            CGMutablePathRef path = CGPathCreateMutable();
//            CGPathMoveToPoint(path, NULL, x, y);
//            CGPathAddLineToPoint(path, NULL, farX, farY);
//            CGPathAddLineToPoint(path, NULL, nearX, nearY);
//            CGPathAddLineToPoint(path, NULL, endX, endY);
//            [positionAnimation setPath: path];
//            CGPathRelease(path);
//            
//            [animationOptions addObject:positionAnimation];
//            
//            CAAnimationGroup *animationGroup = [CAAnimationGroup animation];
//            [animationGroup setAnimations: animationOptions];
//            [animationGroup setDuration:_animationTime];
//            [animationGroup setFillMode: kCAFillModeForwards];
//            [animationGroup setTimingFunction: [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseIn]];
//            
//            NSDictionary *properties = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:button, [NSValue valueWithCGPoint:CGPointMake(endX, endY)], animationGroup, nil] forKeys:[NSArray arrayWithObjects:@"view", @"center", @"animation", nil]];
//            [self performSelector:@selector(_expand:) withObject:properties afterDelay:_delay * ([[self buttons] count] - i)];
//        }
//        else {
//            [button setCenter:CGPointMake(x, y)];
//            [button setAlpha:1.0f];
//        }
//        
//    }
//    _toggled = NO;
//    [self toggleMainButton];
//    float delegateDelay = _animated ? [[self buttons] count] * _delay + _animationTime : 0.0f;
//    if ([self delegate] && [[self delegate] respondsToSelector:@selector(expandingBarDidAppear:)]) {
//        [[self delegate] performSelector:@selector(expandingBarDidAppear:) withObject:self afterDelay:delegateDelay];
//    }
//    //    self.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y, endX, self.frame.size.height);
//}
- (void) hideButtonsAnimated:(BOOL)animated
{
    if ([self delegate] && [[self delegate] respondsToSelector:@selector(expandingBarWillDisappear:)]) {
        [[self delegate] performSelector:@selector(expandingBarWillDisappear:) withObject:self];
    }
      defaultButton.buttonIsShow=NO;
  timer= [NSTimer scheduledTimerWithTimeInterval:4 target:self selector:@selector(BarAlphaChange) userInfo:nil repeats:NO];
    [self.delegate BarTouchBegain];
    [self.delegate buttonsShow:NO];
    [self removeViewWithAnimation];
    
}
//-(void)setCenter:(CGPoint)center
//{
//    //center = CGPointMake(35, 35);
//    [super setCenter:center];
//}
//-(void)setFrame:(CGRect)frame
//{
//    frame = CGRectMake(0, 0, frame.size.width, frame.size.height);
//    [super setFrame:frame];
//}
//- (void) hideButtonsAnimatedTwo:(BOOL)animated
//{
//    if ([self delegate] && [[self delegate] respondsToSelector:@selector(expandingBarWillDisappear:)]) {
//        [[self delegate] performSelector:@selector(expandingBarWillDisappear:) withObject:self];
//    }
//    CGPoint center = [[self button] center];
//    float endY = center.y;
//    float endX = center.x;
//    for (int i = 0; i < [[self buttons] count]; ++i) {
//        UIButton *button = [[self buttons] objectAtIndex:i];
//        if (animated) {
//            NSMutableArray *animationOptions = [NSMutableArray array];
//            if (_spin) {
//                CAKeyframeAnimation *rotateAnimation = [CAKeyframeAnimation animationWithKeyPath:@"transform.rotation.z"];
//                [rotateAnimation setValues:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f],[NSNumber numberWithFloat:M_PI * -2], nil]];
//                [rotateAnimation setDuration:_animationTime];
//                [rotateAnimation setKeyTimes:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f], [NSNumber numberWithFloat:1.0f], nil]];
//                [animationOptions addObject:rotateAnimation];
//            }
//            
//            CAKeyframeAnimation *opacityAnimation = [CAKeyframeAnimation animationWithKeyPath:@"opacity"];
//            [opacityAnimation setValues:[NSArray arrayWithObjects:[NSNumber numberWithFloat:1.0f], [NSNumber numberWithFloat:0.0f], nil]];
//            [opacityAnimation setDuration:_animationTime];
//            [animationOptions addObject:opacityAnimation];
//            
//            float y = [button center].y;
//            float x = [button center].x;
//            CAKeyframeAnimation *positionAnimation = [CAKeyframeAnimation animationWithKeyPath:@"position"];
//            [positionAnimation setDuration:_animationTime];
//            CGMutablePathRef path = CGPathCreateMutable();
//            CGPathMoveToPoint(path, NULL, x, y);
//            CGPathAddLineToPoint(path, NULL, endX, endY);
//            [positionAnimation setPath: path];
//            CGPathRelease(path);
//            
//            [animationOptions addObject:positionAnimation];
//            
//            CAAnimationGroup *animationGroup = [CAAnimationGroup animation];
//            [animationGroup setAnimations: animationOptions];
//            [animationGroup setDuration:_animationTime];
//            [animationGroup setFillMode: kCAFillModeForwards];
//            [animationGroup setTimingFunction: [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseIn]];
//            
//            NSDictionary *properties = [NSDictionary dictionaryWithObjects:[NSArray arrayWithObjects:button, animationGroup, nil] forKeys:[NSArray arrayWithObjects:@"view", @"animation", nil]];
//            [self performSelector:@selector(_close:) withObject:properties afterDelay:_delay * ([[self buttons] count] - i)];
//        }
//        else {
//            [button setCenter:center];
//            [button setAlpha:0.0f];
//        }
//    }
//    float delegateDelay = _animated ? [[self buttons] count] * _delay + _animationTime: 0.0f;
//    if ([self delegate] && [[self delegate] respondsToSelector:@selector(expandingBarDidDisappear:)]) {
//        [[self delegate] performSelector:@selector(expandingBarDidDisappear:) withObject:self afterDelay:delegateDelay];
//    }
//    _toggled = YES;
//    [self toggleMainButton];
//}

- (void) _expand:(NSDictionary*)properties
{
    UIView *view = [properties objectForKey:@"view"];
    CAAnimationGroup *animationGroup = [properties objectForKey:@"animation"];
    NSValue *val = [properties objectForKey:@"center"];
    CGPoint center = [val CGPointValue];
    [[view layer] addAnimation:animationGroup forKey:@"Expand"];
    [view setCenter:center];
    [view setAlpha:1.0f];
}

- (void) _close:(NSDictionary*)properties
{
    UIView *view = [properties objectForKey:@"view"];
    CAAnimationGroup *animationGroup = [properties objectForKey:@"animation"];
    CGPoint center = [[self button] center];
    [[view layer] addAnimation:animationGroup forKey:@"Collapse"];
    [view setAlpha:0.0f];
    [view setCenter:center];
}
- (int) getXoffset:(UIView*)view
{
    if (_horizontal) {
        return [view frame].size.height + _padding;
    }
    return 0;
}

- (int) getYoffset:(UIView*)view
{
    if ( ! _horizontal) {
        return [view frame].size.height + _padding;
    }
    return 0;
}

/* ----------------------------------------------
 * You probably do not want to edit anything under here
 * --------------------------------------------*/
- (void) setAnimationTime:(float)time
{
    if (time > 0) {
        _animationTime = time;
    }
}

- (void) setFadeTime:(float)time
{
    if (time > 0) {
        _fadeTime = time;
    }
}

- (void) setPadding:(float)padding
{
    if (padding > 0) {
        _padding = padding;
    }
}

- (void) setSpin:(BOOL)b
{
    _spin = b;
}

- (void) setHorizontal:(BOOL)b
{
    NSArray *reversedButtons = [[[self buttons] reverseObjectEnumerator] allObjects];
    [self setButtons:reversedButtons];
    
    _horizontal = b;
}

- (void) setFar:(float)num
{
    _far = num;
}

- (void) setNear:(float)num
{
    _near = num;
}

- (void) setDelay:(float)num
{
    _delay = num;
}

- (void) setExplode:(BOOL)b
{
    _explode = b;
}

/* ----------------------------------------------
 * DO NOT CHANGE
 * The following is a hack to allow touches outside
 * of this view. Use caution when changing.
 * --------------------------------------------*/
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    UIView *v = nil;
    v = [super hitTest:point withEvent:event];
    return v;
}

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    BOOL isInside = [super pointInside:point withEvent:event];
//    if (YES == isInside) {
//        return isInside;
//    }
//    for (UIButton *button in [self buttons]) {
//        CGPoint inButtonSpace = [self convertPoint:point toView:button];
//        BOOL isInsideButton = [button pointInside:inButtonSpace withEvent:nil];
//        if (YES == isInsideButton) {
//            return isInsideButton;
//        }
//    }
    return isInside;
}

CGPoint beginPoint;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    highlighted = YES;
    UITouch *touch = [touches anyObject];
    beginPoint = [touch locationInView:self];
    printf("Be**>%.2f, %.2f\n", beginPoint.x, beginPoint.y);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    highlighted = NO;
    
    UITouch *touch = [touches anyObject];
    
    CGPoint nowPoint = [touch locationInView:self];
    printf("No**>%.2f, %.2f\n", nowPoint.x, nowPoint.y);
    float offsetX = nowPoint.x - beginPoint.x;
    float offsetY = nowPoint.y - beginPoint.y;
    printf("off**>%.2f, %.2f\n", offsetX, offsetY);
    //    self.center = CGPointMake(self.center.x + offsetX, self.center.y + offsetY);
    UIInterfaceOrientation statusOrient= [UIApplication sharedApplication].statusBarOrientation;
    if (statusOrient == UIInterfaceOrientationLandscapeRight)
    {
        self.center = CGPointMake(self.center.x - offsetY, self.center.y + offsetX);
    }
    else if(statusOrient == UIInterfaceOrientationLandscapeLeft)
    {
        self.center = CGPointMake(self.center.x + offsetY, self.center.y - offsetX);
    }
    else if(statusOrient == UIInterfaceOrientationPortrait)
    {
        self.center = CGPointMake(self.center.x + offsetX, self.center.y + offsetY);
        
    }
    else
    {
        self.center= CGPointMake(self.center.x - offsetX, self.center.y - offsetY);
        NSLog(@"zou");
    }
    
    printf("Mo**>%.2f, %.2f\n", self.center.x, self.center.y);
    
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if (highlighted) {
        [self showButtonsAnimated:YES];
        highlighted = NO;
    }
    
    if (self.superview )
    {
        float marginLeft = self.frame.origin.y;
        float marginRight = self.superview.bounds.size.height /2;
        float marginLeftTwo = self.frame.origin.x;
        float marginRightTwo = self.superview.bounds.size.width /2;
        [UIView animateWithDuration:0.125 animations:^(void){
            self.frame = CGRectMake((marginLeftTwo<marginRightTwo)?0:(self.superview.bounds.size.width - self.frame.size.width),
                                    (marginLeft<marginRight)?0:(self.superview.bounds.size.height - self.frame.size.height),
                                    self.frame.size.width,   self.frame.size.height);
        }];
        if (marginLeftTwo<marginRightTwo&&marginLeft<marginRight)
        {
            SelfPostion=LEFTUP;
            STARTING_POINT=CGPointMake(35, 35);
            for (int i=0;i<3;i++)
            {
                UIButton *button=[_arrButtons objectAtIndex:i];
                switch (i) {
                    case 0:
                    {   [button setTag:0];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:0]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:0]] forState:UIControlStateHighlighted];
                        
                        break;
                    }
                    case 2:
                    {   [button setTag:2];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:2]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:2]] forState:UIControlStateHighlighted];
                        
                        break;
                    }
                    default:
                        break;
                }
                
                
                
            }
        }
        else if(marginLeftTwo<marginRightTwo)
        {
            
            SelfPostion=LEFTDOWN;
            STARTING_POINT=CGPointMake(35, self.superview.bounds.size.height-35);
            for (int i=0;i<3;i++)
            {
                UIButton *button=[_arrButtons objectAtIndex:i];
                switch (i) {
                    case 0:
                    { [button setTag:2];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:2]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:2]] forState:UIControlStateHighlighted];
                        break;
                    }
                    case 2:
                    { [button setTag:0];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:0]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:0]] forState:UIControlStateHighlighted];
                        break;
                    }
                    default:
                        break;
                }
                
                
                
            }
        }
        else if (marginLeftTwo>=marginRightTwo&&marginLeft>=marginRight)
        {
            SelfPostion=RIGHTDOWN;
            STARTING_POINT=CGPointMake( self.superview.bounds.size.width-35, self.superview.bounds.size.height-35);
        
            for (int i=0;i<3;i++)
            {
                UIButton *button=[_arrButtons objectAtIndex:i];
                switch (i) {
                    case 0:
                    { [button setTag:2];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:2]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:2]] forState:UIControlStateHighlighted];
                        break;
                    }
                    case 2:
                    { [button setTag:0];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:0]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:0]] forState:UIControlStateHighlighted];
                        break;
                    }
                    default:
                        break;
                }
                
                
                
            }
        }
        else
        {
            SelfPostion=RIGHTUP;
            STARTING_POINT=CGPointMake(self.superview.bounds.size.width-35, 35);
    
            for (int i=0;i<3;i++)
            {
                UIButton *button=[_arrButtons objectAtIndex:i];
                switch (i) {
                    case 0:
                    { [button setTag:0];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:0]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:0]] forState:UIControlStateHighlighted];
                        break;
                    }
                    case 2:
                    { [button setTag:2];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonImage objectAtIndex:2]] forState:UIControlStateNormal];
                        [button setBackgroundImage:[UIImage imageNamed:[buttonHightImage objectAtIndex:2]] forState:UIControlStateHighlighted];
                        break;
                    }
                    default:
                        break;
                }
                
                
                
            }
            
        }
        
        
        //        UIInterfaceOrientation statusOrient= [UIApplication sharedApplication].statusBarOrientation;
        //        if (statusOrient == UIInterfaceOrientationLandscapeRight)
        //        {
        //            float marginLeft = self.frame.origin.y;
        //            float marginRight = windowBounds.size.width - self.frame.origin.y - self.frame.size.width;
        //            float marginLeftTwo = self.frame.origin.x;
        //            float marginRightTwo = windowBounds.size.height - self.frame.origin.x - self.frame.size.height;
        //            [UIView animateWithDuration:0.125 animations:^(void){
        //                self.frame = CGRectMake((marginLeftTwo<marginRightTwo)?0:(windowBounds.size.height - self.frame.size.height),
        //                                        (marginLeft<marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        //                                        self.frame.size.width,
        //                                        self.frame.size.height);
        ////                self.frame = CGRectMake(CONSTRAINTDEMI(self.frame.origin.x, 0, windowBounds.size.height + windowBounds.origin.y - self.frame.size.height-windowBounds.origin.y),
        ////                                        (marginLeft<marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        ////                                        self.frame.size.width,
        ////                                        self.frame.size.height);
        //            }];
        //        }
        //        else if (statusOrient == UIInterfaceOrientationPortrait)
        //        {
        //            float marginLeft = self.frame.origin.x;
        //            float marginRight = windowBounds.size.width - self.frame.origin.x - self.frame.size.width;
        //            float marginLefttwo = self.frame.origin.y;
        //            float marginRightwo = windowBounds.size.height - self.frame.origin.y - self.frame.size.height;
        //            [UIView animateWithDuration:0.125 animations:^(void){
        //                self.frame = CGRectMake((marginLeft<marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        //                                        (marginLefttwo<marginRightwo)?0:(windowBounds.size.height - self.frame.size.height+20),
        //                                        self.frame.size.width,
        //                                        self.frame.size.height);
        ////                self.frame = CGRectMake((marginLeft<marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        ////                                        CONSTRAINTDEMI(self.frame.origin.y, windowBounds.origin.y, windowBounds.size.height + windowBounds.origin.y - self.frame.size.height),
        ////                                        self.frame.size.width,
        ////                                        self.frame.size.height);
        //            }];
        //        }
        //        else if (statusOrient == UIInterfaceOrientationLandscapeLeft)
        //        {
        //            float marginLeft = windowBounds.size.width - self.frame.origin.y - self.frame.size.width;
        //            float marginRight = self.frame.origin.y;
        //            float marginLefttwo = windowBounds.size.height - self.frame.origin.x - self.frame.size.height;
        //            float marginRighttwo = self.frame.origin.x;
        //            [UIView animateWithDuration:0.125 animations:^(void){
        //                self.frame = CGRectMake((marginLefttwo>marginRighttwo)?0:(windowBounds.size.height - self.frame.size.height+20),
        //                                        (marginLeft>marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        //                                        self.frame.size.width,
        //                                        self.frame.size.height);
        ////                self.frame = CGRectMake(CONSTRAINTDEMI(self.frame.origin.x, windowBounds.origin.y, windowBounds.size.height + windowBounds.origin.y - self.frame.size.height),
        ////                                        (marginLeft>marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        ////                                        self.frame.size.width,
        ////                                        self.frame.size.height);
        //            }];
        //        }
        //        else
        //        {
        //            float marginLeft = self.frame.origin.x;
        //            float marginRight = windowBounds.size.width - self.frame.origin.x -self.frame.size.width;
        //            float marginLeftTwo = self.frame.origin.y;
        //            float marginRightTwo = windowBounds.size.height - self.frame.origin.y -self.frame.size.height;
        //            [UIView animateWithDuration:0.125 animations:^(void){
        //                self.frame = CGRectMake((marginLeft<marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        //                                       (marginLeftTwo<marginRightTwo)?0:(windowBounds.size.height - self.frame.size.height),
        //                                        self.frame.size.width,
        //                                        self.frame.size.height);
        ////                self.frame = CGRectMake((marginLeft<marginRight)?0:(windowBounds.size.width - self.frame.size.width),
        ////                                        CONSTRAINTDEMI(self.frame.origin.y, 0, windowBounds.size.height - self.frame.size.height),
        ////                                        self.frame.size.width,
        ////                                        self.frame.size.height);
        //            }];
        //        }
    }
}
#pragma mark - Remove menu

- (void)hideMenu:(UIButton *)button
{
    //    if (sender &&
    //        sender != _buttonCorner &&
    //        _delegateCircularMenu &&
    //        [_delegateCircularMenu respondsToSelector:@selector(circularMenuClickedButtonAtIndex:)])
    {
        //UIButton *button = (UIButton*)sender;
        // [_delegateCircularMenu circularMenuClickedButtonAtIndex:(int)button.tag];
        NSLog(@"%dbuttonClick",button.tag);
    }
    if ([self delegate] && [[self delegate] respondsToSelector:@selector(toolButtonClick:)])
    { [self.delegate toolButtonClick:button.tag];
    }
    
    [self removeViewWithAnimation];
}


- (void)removeViewWithAnimation
{
    [self.superview layoutIfNeeded];
    
    [UIView animateWithDuration:0.25
                     animations:
     ^{
         for (int index = 0; index < _iNumberOfButtons; index++)
         {
             UIButton *button = [_arrButtons objectAtIndex:index];
             button.center = STARTING_POINT;
             button.alpha=0;
         }
     }
                     completion:^(BOOL finished)
     {
         _toggled = YES;
         [self toggleMainButton];
     }];
}

- (void)statusBarFrameOrOrientationChanged:(NSNotification *)notification
{
    /*
     This notification is most likely triggered inside an animation block,
     therefore no animation is needed to perform this nice transition.
     */
    [self rotateAccordingToStatusBarOrientationAndSupportedOrientations];
}

- (void)rotateAccordingToStatusBarOrientationAndSupportedOrientations
{
    UIInterfaceOrientation statusBarOrientation = [UIApplication sharedApplication].statusBarOrientation;
    CGFloat angle = UIInterfaceOrientationAngleOfOrientation(statusBarOrientation);
    //    CGFloat statusBarHeight = [GetStatusBarPara getStatusBarHeight];
    
    CGAffineTransform transform = CGAffineTransformMakeRotation(angle);
    //    CGRect frame = [[self class] rectInWindowBounds:self.window.bounds statusBarOrientation:statusBarOrientation statusBarHeight:statusBarHeight];
    
    //    [self setIfNotEqualTransform:transform frame:frame];
    if(!CGAffineTransformEqualToTransform(self.transform, transform))
    {
        self.transform = transform;
    }
}

//- (void)setIfNotEqualTransform:(CGAffineTransform)transform frame:(CGRect)frame
//{
//    if(!CGAffineTransformEqualToTransform(self.transform, transform))
//    {
//        self.transform = transform;
//    }
//    if(!CGRectEqualToRect(self.frame, frame))
//    {
//        self.frame = frame;
//    }
//}


+ (CGRect)rectInWindowBounds:(CGRect)windowBounds statusBarOrientation:(UIInterfaceOrientation)statusBarOrientation statusBarHeight:(CGFloat)statusBarHeight
{
    CGRect frame = windowBounds;
    frame.origin.x += statusBarOrientation == UIInterfaceOrientationLandscapeLeft ? statusBarHeight : 0;
    frame.origin.y += statusBarOrientation == UIInterfaceOrientationPortrait ? statusBarHeight : 0;
    frame.size.width -= UIInterfaceOrientationIsLandscape(statusBarOrientation) ? statusBarHeight : 0;
    frame.size.height -= UIInterfaceOrientationIsPortrait(statusBarOrientation) ? statusBarHeight : 0;
    return frame;
}

@end
