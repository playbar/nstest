//
//  NetWorkDetector.m
//  BaiWan
//
//  Created by lining on 10-11-24.
//  Modified by handong on 11-05-10.
//  Copyright 2010 JQBar. All rights reserved.
//

#import "NetWorkDetector.h"
#import "Reachability.h"

static NetWorkDetector *sharedInstance = nil;

@implementation NetWorkDetector

#pragma mark -
#pragma mark class instance methods

#pragma mark -
#pragma mark Singleton methods



+ (NetWorkDetector*)sharedInstance
{
    @synchronized(self)
    {
        if (sharedInstance == nil)
		{
			sharedInstance = [[NetWorkDetector alloc] init];

		}
	}
    return sharedInstance;
}

+ (id)allocWithZone:(NSZone *)zone {
    @synchronized(self) {
        if (sharedInstance == nil) {
            sharedInstance = [super allocWithZone:zone];
            return sharedInstance;  // assignment and return on first allocation
        }
    }
    return nil; // on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain {
    return self;
}

- (unsigned)retainCount {
    return UINT_MAX;  // denotes an object that cannot be released
}

- (void)release {
    //do nothing
}

- (id)autorelease {
    return self;
}

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    [connection cancel];
}

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    // Append the new data to receivedData.
    [connection cancel];
}

- (void)connection:(NSURLConnection *)connection
  didFailWithError:(NSError *)error
{
    // release the connection, and the data object
    [connection cancel];
    // receivedData is declared as a method instance elsewhere
}

- (void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    // do something with the data
    // receivedData is declared as a method instance elsewhere
	// release the connection, and the data object
    [connection cancel];
}

- (void) reachabilityChanged: (NSNotification* )note
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	Reachability* curReach = [note object];
	NSParameterAssert([curReach isKindOfClass: [Reachability class]]);
	NetworkStatus netStatus = [curReach currentReachabilityStatus];
    BOOL connectionRequired= [curReach connectionRequired];
	if (netStatus == NotReachable)
	{
		connectionRequired = NO;
	}
	if(connectionRequired)
    {
        //
		if (m_connection == nil)
		{
			NSURLRequest *theRequest=[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.apple.com/"]
													  cachePolicy:NSURLRequestUseProtocolCachePolicy
												  timeoutInterval:0.1];
			
			m_connection=[[NSURLConnection alloc] initWithRequest:theRequest delegate:self];
			
		}
		[m_connection start];
    }
	[pool release];
	NSParameterAssert(curReach == m_reachability);
}

- (BOOL) canConnected
{
	Reachability* curReach = m_reachability;
	NSParameterAssert([curReach isKindOfClass: [Reachability class]]);
	NetworkStatus netStatus = [curReach currentReachabilityStatus];
	return netStatus != NotReachable;	
}

- (BOOL) needConnected
{
	Reachability* curReach = m_reachability;
	NSParameterAssert([curReach isKindOfClass: [Reachability class]]);
	NetworkStatus netStatus = [curReach currentReachabilityStatus];
	BOOL connectionRequired= [curReach connectionRequired];
	return (netStatus != NotReachable) && connectionRequired;
}

- (void) start
{
	if (m_reachability == nil)
	{
		[[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(reachabilityChanged:) name: kReachabilityChangedNotification object: nil];
		m_reachability = [[Reachability reachabilityWithHostName: @"www.apple.com"] retain];
		[m_reachability startNotifier];
	}
}

- (void) stop
{
	[m_reachability stopNotifier];
	[m_reachability release];
	m_reachability = nil;
	if (m_connection)
	{
		[m_connection release];
		m_connection = nil;
	}
}

@end