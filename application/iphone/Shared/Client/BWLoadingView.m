//
//  LoadingView.m
//  TestAnimation
//
//  Created by taomy on 14/11/18.
//  Copyright (c) 2014年 Liuyu. All rights reserved.
//

#import "BWLoadingView.h"
#import "UIView+BWAnimation.h"

//动画持续时间
#define FIRST_SCENE_LAST 0.7
#define FIRST_SCENE_GAP 0.4
#define SECOND_SCENE_LAST 0.3
#define SECOND_SCENE_GAP 0.3
#define THIRD_SCENE_LAST 0.3

//图片的大小
#define FLO_CENTER_POINT_X 757/2
#define FLO_CENTER_POINT_Y 589/2
#define BLUE_BLOCK_POINTS {272/2,17/2,227/2,451/2,679/2,634/2}
#define YELLOW_BLOCK_POINTS {1523/2,497/2,1133/2,213/2,757/2,501/2}
#define RED_BLOCK_POINTS {355/2,1247/2,825/2,1119/2,835/2,636/2}
#define GREEN_BLOCK_POINTS {FLO_CENTER_POINT_X,FLO_CENTER_POINT_Y,679/2,545/2}
#define ORANGE_BLOCK_POINTS {FLO_CENTER_POINT_X,FLO_CENTER_POINT_Y,835/2,545/2}
#define PURPLE_BLOCK_POINTS {FLO_CENTER_POINT_X,FLO_CENTER_POINT_Y,757/2,680/2}
#define LOADING_TEXT_CENTER_Y 0
#define LOADING_LOGOUNDER_CENTER_Y 0
#define LOADING_BANNER_CENTER_Y 0
#define ADJUST_SCALE(x,cen_x) ((x>cen_x)?(ABS(x-cen_x)*(self.scaleFactor - 1)):(ABS(x-cen_x)*(1 - self.scaleFactor)))

@implementation BWLoadingView

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/

-(void)initView
{
    CGRect screen = [[UIScreen mainScreen] bounds];
    UIImage * backgImag;
    if (UIInterfaceOrientationIsLandscape(self.viewOrentation))
    {
        backgImag = [UIImage imageNamed:@"loadingBackground.jpg"];
        screenWidth = MAX(screen.size.width, screen.size.height);
        screenHeight = MIN(screen.size.width, screen.size.height);
    }
    else
    {
        backgImag = [UIImage imageNamed:@"loadingBackgroundPortrait.jpg"];
        screenHeight = MAX(screen.size.width, screen.size.height);
        screenWidth = MIN(screen.size.width, screen.size.height);
    }
    UIImage *loadingTextImage = [UIImage imageNamed:@"loadingText"];
    if (((self.scaleFactor*loadingTextImage.size.width/2)>(screenWidth*2/3))||((self.scaleFactor*172)>(screenHeight*1/3)))
    {
        self.scaleFactor = MIN((screenWidth*2/3)/(self.scaleFactor*loadingTextImage.size.width/2),(screenHeight*1/3)/(self.scaleFactor*172));
    }
    self.backgroundColor = [UIColor redColor];
    float backgImagWidth = backgImag.size.width/2;
    float backgImagHeight = backgImag.size.height/2;
    CGRect backGroundFrame;
    if (backgImagWidth > screenWidth)
    {
        backGroundFrame.origin.x = (screenWidth-backgImagWidth)/2;
        backGroundFrame.size.width = backgImagWidth;
    }
    else
    {
        backGroundFrame.origin.x = 0;
        backGroundFrame.size.width = screenWidth;
    }
    if (backgImagHeight > screenHeight)
    {
        backGroundFrame.origin.y = (screenHeight-backgImagHeight)/2;
        backGroundFrame.size.height = backgImagHeight;
    }
    else
    {
        backGroundFrame.origin.y = 0;
        backGroundFrame.size.height = screenHeight;
    }
    
    backGround=[[UIImageView alloc]initWithFrame:backGroundFrame];
    backGround.image=backgImag;
    [self addSubview:backGround];
    
    xToMove = screenWidth/2 - FLO_CENTER_POINT_X;
    yToMove = screenHeight*1.2/3 - FLO_CENTER_POINT_Y;
    UIImage *blueImage = [UIImage imageNamed:@"loadingLan"];
    UIImageView *blueBlock=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, blueImage.size.width/2*self.scaleFactor, blueImage.size.height/2*self.scaleFactor)];
    blueBlock.image=blueImage;
    [self addSubview:blueBlock];
    float orignBluePoint[6] = BLUE_BLOCK_POINTS;
    float bluePoint[8] = {xToMove+orignBluePoint[0]+ADJUST_SCALE(orignBluePoint[0], FLO_CENTER_POINT_X),yToMove+orignBluePoint[1]+ADJUST_SCALE(orignBluePoint[1], FLO_CENTER_POINT_Y), xToMove+orignBluePoint[2]+ADJUST_SCALE(orignBluePoint[2], FLO_CENTER_POINT_X),yToMove+orignBluePoint[3]+ADJUST_SCALE(orignBluePoint[3], FLO_CENTER_POINT_Y), xToMove+orignBluePoint[2]+ADJUST_SCALE(orignBluePoint[2], FLO_CENTER_POINT_X),yToMove+orignBluePoint[3]+ADJUST_SCALE(orignBluePoint[3], FLO_CENTER_POINT_Y), xToMove+orignBluePoint[4]+ADJUST_SCALE(orignBluePoint[4], FLO_CENTER_POINT_X),yToMove+orignBluePoint[5]+ADJUST_SCALE(orignBluePoint[5], FLO_CENTER_POINT_Y)};
    NSMutableArray *bluePointArray = [[NSMutableArray alloc]initWithCapacity:8];
    for (int i = 0; i<8; i++)
    {
        [bluePointArray addObject:[NSNumber numberWithFloat:bluePoint[i]]];
    }
    [blueBlock beginAnimateViewWithPath:bluePointArray isAlpha:YES isScale:YES animateTime:FIRST_SCENE_LAST];
    
    UIImage *redImage = [UIImage imageNamed:@"loadingHong"];
    UIImageView *redBlock=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, redImage.size.width/2*self.scaleFactor, redImage.size.height/2*self.scaleFactor)];
    redBlock.image=redImage;
    [self addSubview:redBlock];
    float orignRedPoint[6] = RED_BLOCK_POINTS;
    float redPoint[8] = {xToMove+orignRedPoint[0]+ADJUST_SCALE(orignRedPoint[0], FLO_CENTER_POINT_X),yToMove+orignRedPoint[1]+ADJUST_SCALE(orignRedPoint[1], FLO_CENTER_POINT_Y), xToMove+orignRedPoint[2]+ADJUST_SCALE(orignRedPoint[2], FLO_CENTER_POINT_X),yToMove+orignRedPoint[3]+ADJUST_SCALE(orignRedPoint[3], FLO_CENTER_POINT_Y), xToMove+orignRedPoint[2]+ADJUST_SCALE(orignRedPoint[2], FLO_CENTER_POINT_X),yToMove+orignRedPoint[3]+ADJUST_SCALE(orignRedPoint[3], FLO_CENTER_POINT_Y), xToMove+orignRedPoint[4]+ADJUST_SCALE(orignRedPoint[4], FLO_CENTER_POINT_X),yToMove+orignRedPoint[5]+ADJUST_SCALE(orignRedPoint[5], FLO_CENTER_POINT_Y)};
    NSMutableArray *redPointArray = [[NSMutableArray alloc]initWithCapacity:8];
    for (int i = 0; i<8; i++)
    {
        [redPointArray addObject:[NSNumber numberWithFloat:redPoint[i]]];
    }
    [redBlock beginAnimateViewWithPath:redPointArray isAlpha:YES isScale:YES animateTime:FIRST_SCENE_LAST];
    
    UIImage *yellowImage = [UIImage imageNamed:@"loadingHuang"];
    UIImageView *yellowBlock=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, yellowImage.size.width/2*self.scaleFactor, yellowImage.size.height*self.scaleFactor/2)];
    yellowBlock.image=yellowImage;
    [self addSubview:yellowBlock];
    float orignYellowPoint[6] = YELLOW_BLOCK_POINTS;
    float yellowPoint[8] = {xToMove+orignYellowPoint[0]+ADJUST_SCALE(orignYellowPoint[0], FLO_CENTER_POINT_X),yToMove+orignYellowPoint[1]+ADJUST_SCALE(orignYellowPoint[1], FLO_CENTER_POINT_Y), xToMove+orignYellowPoint[2]+ADJUST_SCALE(orignYellowPoint[2], FLO_CENTER_POINT_X),yToMove+orignYellowPoint[3]+ADJUST_SCALE(orignYellowPoint[3], FLO_CENTER_POINT_Y), xToMove+orignYellowPoint[2]+ADJUST_SCALE(orignYellowPoint[2], FLO_CENTER_POINT_X),yToMove+orignYellowPoint[3]+ADJUST_SCALE(orignYellowPoint[3], FLO_CENTER_POINT_Y), xToMove+orignYellowPoint[4]+ADJUST_SCALE(orignYellowPoint[4], FLO_CENTER_POINT_X),yToMove+orignYellowPoint[5]+ADJUST_SCALE(orignYellowPoint[5], FLO_CENTER_POINT_Y)};
    NSMutableArray *yellowPointArray = [[NSMutableArray alloc]initWithCapacity:8];
    for (int i = 0; i<8; i++)
    {
        [yellowPointArray addObject:[NSNumber numberWithFloat:yellowPoint[i]]];
    }
    [yellowBlock beginAnimateViewWithPath:yellowPointArray isAlpha:YES isScale:YES animateTime:FIRST_SCENE_LAST];
    
    [self performSelector:@selector(loadSecondScene) withObject:nil afterDelay:FIRST_SCENE_LAST+FIRST_SCENE_GAP];
}

-(void)loadSecondScene
{
    UIImage *greenImage = [UIImage imageNamed:@"loadingLv"];
    UIImageView *greenBlock=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, greenImage.size.width/2*self.scaleFactor, greenImage.size.height/2*self.scaleFactor)];
    greenBlock.image=greenImage;
    [self insertSubview:greenBlock aboveSubview:backGround];
    float orignGreenPoint[4] = GREEN_BLOCK_POINTS;
    float greenPoint[8] = {xToMove+orignGreenPoint[0]+ADJUST_SCALE(orignGreenPoint[0], FLO_CENTER_POINT_X),yToMove+orignGreenPoint[1]+ADJUST_SCALE(orignGreenPoint[1], FLO_CENTER_POINT_Y), xToMove+orignGreenPoint[2]+ADJUST_SCALE(orignGreenPoint[2], FLO_CENTER_POINT_X),yToMove+orignGreenPoint[3]+ADJUST_SCALE(orignGreenPoint[3], FLO_CENTER_POINT_Y), xToMove+orignGreenPoint[2]+ADJUST_SCALE(orignGreenPoint[2], FLO_CENTER_POINT_X),yToMove+orignGreenPoint[3]+ADJUST_SCALE(orignGreenPoint[3], FLO_CENTER_POINT_Y), xToMove+orignGreenPoint[2]+ADJUST_SCALE(orignGreenPoint[2], FLO_CENTER_POINT_X),yToMove+orignGreenPoint[3]+ADJUST_SCALE(orignGreenPoint[3], FLO_CENTER_POINT_Y)};
    NSMutableArray *greenPointArray = [[NSMutableArray alloc]initWithCapacity:8];
    for (int i = 0; i<8; i++)
    {
        [greenPointArray addObject:[NSNumber numberWithFloat:greenPoint[i]]];
    }
    [greenBlock beginAnimateViewWithPath:greenPointArray isAlpha:YES isScale:YES animateTime:SECOND_SCENE_LAST];
    
    UIImage *orangeImage = [UIImage imageNamed:@"loadingJu"];
    UIImageView *oranBlock=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, orangeImage.size.width/2*self.scaleFactor, orangeImage.size.height/2*self.scaleFactor)];
    oranBlock.image=orangeImage;
    [self insertSubview:oranBlock aboveSubview:backGround];
    float orignOrangePoint[4] = ORANGE_BLOCK_POINTS;
    float orangePoint[8] = {xToMove+orignOrangePoint[0]+ADJUST_SCALE(orignOrangePoint[0], FLO_CENTER_POINT_X),yToMove+orignOrangePoint[1]+ADJUST_SCALE(orignOrangePoint[1], FLO_CENTER_POINT_Y), xToMove+orignOrangePoint[2]+ADJUST_SCALE(orignOrangePoint[2], FLO_CENTER_POINT_X),yToMove+orignOrangePoint[3]+ADJUST_SCALE(orignOrangePoint[3], FLO_CENTER_POINT_Y), xToMove+orignOrangePoint[2]+ADJUST_SCALE(orignOrangePoint[2], FLO_CENTER_POINT_X),yToMove+orignOrangePoint[3]+ADJUST_SCALE(orignOrangePoint[3], FLO_CENTER_POINT_Y), xToMove+orignOrangePoint[2]+ADJUST_SCALE(orignOrangePoint[2], FLO_CENTER_POINT_X),yToMove+orignOrangePoint[3]+ADJUST_SCALE(orignOrangePoint[3], FLO_CENTER_POINT_Y)};
    NSMutableArray *orangePointArray = [[NSMutableArray alloc]initWithCapacity:8];
    for (int i = 0; i<8; i++)
    {
        [orangePointArray addObject:[NSNumber numberWithFloat:orangePoint[i]]];
    }
    [oranBlock beginAnimateViewWithPath:orangePointArray isAlpha:YES isScale:YES animateTime:SECOND_SCENE_LAST];
    
    UIImage *purpleImage = [UIImage imageNamed:@"loadingZi"];
    UIImageView *purpleBlock=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, purpleImage.size.width/2*self.scaleFactor, purpleImage.size.height/2*self.scaleFactor)];
    purpleBlock.image=purpleImage;
    [self insertSubview:purpleBlock aboveSubview:backGround];
    float orignPurplePoint[4] = PURPLE_BLOCK_POINTS;
    float purplePoint[8] = {xToMove+orignPurplePoint[0]+ADJUST_SCALE(orignPurplePoint[0], FLO_CENTER_POINT_X),yToMove+orignPurplePoint[1]+ADJUST_SCALE(orignPurplePoint[1], FLO_CENTER_POINT_Y), xToMove+orignPurplePoint[2]+ADJUST_SCALE(orignPurplePoint[2], FLO_CENTER_POINT_X),yToMove+orignPurplePoint[3]+ADJUST_SCALE(orignPurplePoint[3], FLO_CENTER_POINT_Y), xToMove+orignPurplePoint[2]+ADJUST_SCALE(orignPurplePoint[2], FLO_CENTER_POINT_X),yToMove+orignPurplePoint[3]+ADJUST_SCALE(orignPurplePoint[3], FLO_CENTER_POINT_Y), xToMove+orignPurplePoint[2]+ADJUST_SCALE(orignPurplePoint[2], FLO_CENTER_POINT_X),yToMove+orignPurplePoint[3]+ADJUST_SCALE(orignPurplePoint[3], FLO_CENTER_POINT_Y)};
    NSMutableArray *purplePointArray = [[NSMutableArray alloc]initWithCapacity:8];
    for (int i = 0; i<8; i++)
    {
        [purplePointArray addObject:[NSNumber numberWithFloat:purplePoint[i]]];
    }
    [purpleBlock beginAnimateViewWithPath:purplePointArray isAlpha:YES isScale:YES animateTime:SECOND_SCENE_LAST];
    ////////////////////////////////////////////////////////////
    UIImage *logoUnderImage = [UIImage imageNamed:@"loadingLogoUnder"];
    UIImageView *logoUnderView=[[UIImageView alloc]initWithFrame:CGRectMake(screenWidth/2-logoUnderImage.size.width*self.scaleFactor/4, yToMove+orignPurplePoint[3]+ADJUST_SCALE(orignPurplePoint[3], FLO_CENTER_POINT_Y)+purpleImage.size.height*self.scaleFactor/4, logoUnderImage.size.width/2*self.scaleFactor, logoUnderImage.size.height/2*self.scaleFactor)];
    logoUnderView.image=logoUnderImage;
    [self insertSubview:logoUnderView aboveSubview:backGround];
    NSMutableArray *animationOptions = [NSMutableArray array];
    CAKeyframeAnimation *scaleAnimation = [CAKeyframeAnimation animationWithKeyPath:@"transform.scale"];
    [scaleAnimation setValues:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f],[NSNumber numberWithFloat:1.0f], nil]];
    [scaleAnimation setDuration:SECOND_SCENE_LAST];
    [scaleAnimation setKeyTimes:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f], [NSNumber numberWithFloat:1.0f], nil]];
    [animationOptions addObject:scaleAnimation];
    CAAnimationGroup *animationGroup = [CAAnimationGroup animation];
    [animationGroup setAnimations: animationOptions];
    [animationGroup setDuration:SECOND_SCENE_LAST];
    [animationGroup setFillMode: kCAFillModeForwards];
    [animationGroup setTimingFunction: [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut]];
    [logoUnderView.layer addAnimation:animationGroup forKey:@"bwanimate"];
    
    
    UIImage *loadingTextImage = [UIImage imageNamed:@"loadingText"];
    UIImageView *loadingTextView=[[UIImageView alloc]initWithFrame:CGRectMake((screenWidth-loadingTextImage.size.width/2*self.scaleFactor)/2, yToMove+orignPurplePoint[3]+ADJUST_SCALE(orignPurplePoint[3], FLO_CENTER_POINT_Y), loadingTextImage.size.width*self.scaleFactor/2, loadingTextImage.size.height*self.scaleFactor/2)];
    loadingTextView.image=loadingTextImage;
    [self insertSubview:loadingTextView aboveSubview:backGround];
    NSMutableArray *animationOptions2 = [NSMutableArray array];
    CAKeyframeAnimation *scaleAnimation2 = [CAKeyframeAnimation animationWithKeyPath:@"transform.scale"];
    [scaleAnimation2 setValues:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f],[NSNumber numberWithFloat:1.0f], nil]];
    [scaleAnimation2 setDuration:SECOND_SCENE_LAST];
    [scaleAnimation2 setKeyTimes:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f], [NSNumber numberWithFloat:1.0f], nil]];
    [animationOptions2 addObject:scaleAnimation];
    CAAnimationGroup *animationGroup2 = [CAAnimationGroup animation];
    [animationGroup2 setAnimations: animationOptions2];
    [animationGroup2 setDuration:SECOND_SCENE_LAST];
    [animationGroup2 setFillMode: kCAFillModeForwards];
    [animationGroup2 setTimingFunction: [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut]];
    [loadingTextView.layer addAnimation:animationGroup forKey:@"bwanimate"];
    ////////////////////////////////////////////////////////////
    [self performSelector:@selector(loadThirdScene) withObject:nil afterDelay:SECOND_SCENE_GAP+SECOND_SCENE_LAST];
}

-(void)loadThirdScene
{
    CGRect screen = [[UIScreen mainScreen] bounds];
    if (UIInterfaceOrientationIsLandscape(self.viewOrentation))
    {
        screenWidth = MAX(screen.size.width, screen.size.height);
        screenHeight = MIN(screen.size.width, screen.size.height);
    }
    else
    {
        screenHeight = MAX(screen.size.width, screen.size.height);
        screenWidth = MIN(screen.size.width, screen.size.height);
    }
    UIImage *image =[UIImage imageNamed:@"loadingBanner"];
    UIView *maskView = [[UIView alloc]initWithFrame:CGRectMake((screenWidth-image.size.width*self.scaleFactor/2)/2-image.size.width*self.scaleFactor/4, 4*screenHeight/5 - image.size.height*self.scaleFactor/4, image.size.width*self.scaleFactor/2, image.size.height*self.scaleFactor/2)];
    UIImageView *textBlock=[[UIImageView alloc]initWithFrame:CGRectMake(0, 0, image.size.width*self.scaleFactor/2, image.size.height*self.scaleFactor/2)];
    textBlock.image=image;
    [maskView addSubview:textBlock];
    maskView.clipsToBounds = YES;
    [self addSubview:maskView];
    maskView.layer.anchorPoint = CGPointMake(0, 0.5);
    NSMutableArray *animationOptions1 = [NSMutableArray array];
    CAKeyframeAnimation *alphAnimation = [CAKeyframeAnimation animationWithKeyPath:@"bounds"];
    [alphAnimation setValues:[NSArray arrayWithObjects:[NSValue valueWithCGRect:CGRectMake(0,0,0,image.size.height*self.scaleFactor/2)],[NSValue valueWithCGRect:CGRectMake(0,0,image.size.width*self.scaleFactor/2,image.size.height*self.scaleFactor/2)], nil]];
    [alphAnimation setDuration:THIRD_SCENE_LAST];
    [alphAnimation setKeyTimes:[NSArray arrayWithObjects:[NSNumber numberWithFloat:0.0f], [NSNumber numberWithFloat:1.0f], nil]];
    [animationOptions1 addObject:alphAnimation];
    CAAnimationGroup *animationGroup1 = [CAAnimationGroup animation];
    [animationGroup1 setAnimations: animationOptions1];
    [animationGroup1 setDuration:THIRD_SCENE_LAST];
    [animationGroup1 setFillMode: kCAFillModeForwards];
    [animationGroup1 setTimingFunction: [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut]];
    [maskView.layer addAnimation:animationGroup1 forKey:@"bwanimate"];
}
@end
