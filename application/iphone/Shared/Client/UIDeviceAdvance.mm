    //
//  UIDeviceAdvance.m
//  BaiWan
//
//  Created by handong on 11-6-21.
//  Copyright 2011 JQBar. All rights reserved.
//

#import "UIDeviceAdvance.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <sys/socket.h> // Per msqr
#include <net/if.h>
#include <net/if_dl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <dlfcn.h>
#import "AdSupport/AdSupport.h"
/*
 * iPhone Simulator = i386
 * iPhone           = iPhone1,1
 * iPhone 3G        = iPhone1,2
 * iPhone 3GS       = iPhone2,1
 * iPhone 4         = iPhone3,1
 * iPod Touch1      = iPod1,1
 * iPod Touch2      = iPod2,1
 * iPod Touch3      = iPod3,1
 * iPod Touch4      = iPod4,1
 * iPad
 */
static NSString *serviceName = @"com.jqbar.savedata";

@implementation UIDevice(machine)

- (NSString *)machine
{
    size_t size;
    
    // Set 'oldp' parameter to NULL to get the size of the data
    // returned so we can allocate appropriate amount of space
    sysctlbyname("hw.machine", NULL, &size, NULL, 0); 
    
    // Allocate the space to store name
    char *name = (char*)malloc(size);
    
    // Get the platform name
    sysctlbyname("hw.machine", name, &size, NULL, 0);
    
    // Place name into a string
    NSString *machine = [NSString stringWithCString:name encoding:NSUTF8StringEncoding];
    
    // Done with this
    free(name);
    
    return machine;
}


- (NSMutableDictionary *)newSearchDictionary:(NSString *)identifier {
    NSMutableDictionary *searchDictionary = [[NSMutableDictionary alloc] init];
    
    [searchDictionary setObject:(id)kSecClassGenericPassword forKey:(id)kSecClass];
    
    NSData *encodedIdentifier = [identifier dataUsingEncoding:NSUTF8StringEncoding];
    [searchDictionary setObject:encodedIdentifier forKey:(id)kSecAttrGeneric];
    [searchDictionary setObject:encodedIdentifier forKey:(id)kSecAttrAccount];
    [searchDictionary setObject:serviceName forKey:(id)kSecAttrService];
    
    return searchDictionary;
}

- (NSData *)searchKeychainCopyMatching:(NSString *)identifier {
    NSMutableDictionary *searchDictionary = [self newSearchDictionary:identifier];
    
    // Add search attributes
    [searchDictionary setObject:(id)kSecMatchLimitOne forKey:(id)kSecMatchLimit];
    
    // Add search return types
    [searchDictionary setObject:(id)kCFBooleanTrue forKey:(id)kSecReturnData];
    
    NSData *result = nil;
    OSStatus status = SecItemCopyMatching((CFDictionaryRef)searchDictionary,
                                          (CFTypeRef *)&result);
    
    [searchDictionary release];
    if (status == errSecSuccess)
        return result;
    else
        return nil;
}

- (BOOL)createKeychainValue:(NSString *)idfvword forIdentifier:(NSString *)identifier
{
    NSMutableDictionary *dictionary = [self newSearchDictionary:identifier];
    
    NSData *saveData = [idfvword dataUsingEncoding:NSUTF8StringEncoding];
    [dictionary setObject:saveData forKey:(id)kSecValueData];
    
    OSStatus status = SecItemAdd((CFDictionaryRef)dictionary, NULL);
    [dictionary release];
    
    if (status == errSecSuccess) {
        return YES;
    }
    return NO;
}

- (NSString*)MACAddress
{
    int                 mib[6];
	size_t              len;
	char                *buf;
	unsigned char       *ptr;
	struct if_msghdr    *ifm;
	struct sockaddr_dl  *sdl;
	
	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;
	
	if ((mib[5] = if_nametoindex("en0")) == 0) {
		printf("Error: if_nametoindex error/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
		printf("Error: sysctl, take 1/n");
		return NULL;
	}
	
	if ((buf = (char*)malloc(len)) == NULL) {
		printf("Could not allocate memory. error!/n");
		return NULL;
	}
	
	if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
		printf("Error: sysctl, take 2");
		return NULL;
	}
	
	ifm = (struct if_msghdr *)buf;
	sdl = (struct sockaddr_dl *)(ifm + 1);
	ptr = (unsigned char *)LLADDR(sdl);
	// NSString *outstring = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	NSString *outstring = [NSString stringWithFormat:@"%02x%02x%02x%02x%02x%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
	free(buf);
    // ios 7.0 后mac地址不可用返回identifierForVendor
    if (([[[UIDevice currentDevice] systemVersion] compare:@"6.9.9" options:NSNumericSearch]==NSOrderedDescending))
    {
        //idfv替代idfa
        return [self idfvUDID];
//        NSData *idfaData = [self searchKeychainCopyMatching:@"idfaData"];
//        NSString *idfa = @"";
//        if (idfaData) {
//            idfa = [[NSString alloc] initWithData:idfaData
//                                         encoding:NSUTF8StringEncoding];
//            [idfaData release];
//            return idfa;
//        }
//        else
//        {
//            [[ASIdentifierManager sharedManager] isAdvertisingTrackingEnabled];
//            NSString *idfa = [[[ASIdentifierManager sharedManager]advertisingIdentifier]UUIDString];
//            if (idfa)
//            {
//                [self createKeychainValue:idfa forIdentifier:@"idfaData"];
//                return [idfa uppercaseString];
//            }
//            else
//            {
//                return @"";
//            }
//        }
        
    }

	return [outstring uppercaseString];
}

- (NSString*)WIFiIpAddress
{
    bool success;
    struct ifaddrs *addrs;
    const struct ifaddrs *cursor;
    success = getifaddrs( &addrs ) == 0;
    if (success) {
        cursor = addrs;
        while (cursor != NULL) {
            if ( cursor->ifa_addr->sa_family == AF_INET &&(cursor->ifa_flags & IFF_LOOPBACK) == 0 ) {
                NSString *name = [NSString stringWithUTF8String:cursor->ifa_name ];
                if ([name isEqualToString:@"en0"]) {
                    return [NSString stringWithUTF8String:inet_ntoa(((struct sockaddr_in *)cursor->ifa_addr)->sin_addr)];
                }
            }
            cursor = cursor->ifa_next;
        }
        freeifaddrs( addrs );
    }
    return @"192.168.0.1";
}

-(NSString *)idfvUDID
{
    NSData *idfvData = [self searchKeychainCopyMatching:@"idfvData"];
    NSString *idfv = @"";
    if (idfvData) {
        idfv = [[NSString alloc] initWithData:idfvData
                                     encoding:NSUTF8StringEncoding];
        [idfvData release];
    }
    else
    {
        if (([[[UIDevice currentDevice] systemVersion] compare:@"5.9.9" options:NSNumericSearch]==NSOrderedDescending))
        {
            idfv = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
            [self createKeychainValue:idfv forIdentifier:@"idfvData"];
        }
    }
    NSLog(@"IDFV:%@",idfv);
    return idfv;
}
@end

@implementation UIDevice(memory)

- (unsigned int)freeMemory
{
	vm_statistics_data_t vmStats;
	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
	kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
	
	if(kernReturn != KERN_SUCCESS) {
		return NSNotFound;
	}
	
	return vm_page_size * vmStats.free_count;
}

- (unsigned int)activeMemory
{
	vm_statistics_data_t vmStats;
	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
	kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
	
	if(kernReturn != KERN_SUCCESS) {
		return NSNotFound;
	}
	
	return vm_page_size * vmStats.active_count;
}

- (unsigned int)inactiveMemory
{
	vm_statistics_data_t vmStats;
	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
	kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
	
	if(kernReturn != KERN_SUCCESS) {
		return NSNotFound;
	}
	
	return vm_page_size * vmStats.inactive_count;
}

- (unsigned int)totalMemory
{
    host_basic_info basic_info;
    mach_msg_type_number_t basic_infoNumber;
    kern_return_t kernReturn = host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&basic_info, &basic_infoNumber);
    if (kernReturn == KERN_SUCCESS)
        return basic_info.memory_size;
    else
    {
        NSProcessInfo* info  = [NSProcessInfo processInfo];
        return [info physicalMemory];   // 丢失高32位
    }
}

- (unsigned int)usedMemory
{
	vm_statistics_data_t vmStats;
	mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
	kern_return_t kernReturn = host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStats, &infoCount);
	
	if(kernReturn != KERN_SUCCESS) {
		return NSNotFound;
	}
	
//	return vm_page_size*(vmStats.active_count + vmStats.inactive_count);
    return vm_page_size*(vmStats.active_count + vmStats.wire_count);
}

@end


@implementation NSData (NSData_Convert)

- (NSString*)stringWithHexData
{
	NSMutableString* stringBuffer = [NSMutableString stringWithCapacity:([self length]*2)];
	const unsigned char* dataBuffer = (const unsigned char*)[self bytes];
	
	for (int i=0; i<[self length]; i++)
	{
		[stringBuffer appendFormat:@"%02X", (unsigned long)dataBuffer[i]];
	}
	
	return [[stringBuffer retain] autorelease];
}

@end
