    //
//  LocalViewController_iPhone.m
//  BaiWan
//
//  Created by JQBar on 11-7-15.
//  Copyright 2011 JQBar. All rights reserved.
//

#import "web_localViewController_ios.h"
#import "BaiWanViewController_ios.h"

#include "GlobalPortingFunction.h"
#include "GGBase.h"
#include "MultiStringUtil.h"

extern iOSDeviceRelateData g_deviceData;
@implementation web_localViewController_ios
-(id)init
{
    id returned = [super init];
    if(returned)
    {
        CGRect webViewBound;
        webViewBound.origin.x = 0;
        webViewBound.origin.y = 44;
        webViewBound.size.width = g_deviceData.m_screenWidth;
        webViewBound.size.height = g_deviceData.m_screenHeight - 44;
        fileList = [[UITableView alloc] initWithFrame:webViewBound style:UITableViewStyleGrouped];
        fileList.dataSource = self;
        fileList.delegate = self;
    }
    return returned;
}

- (void)initData
{
	if (!fileNameArray)
		fileNameArray = [[NSMutableArray alloc] init];
	else 
		[fileNameArray removeAllObjects];
	if (!fileLocationArray)
		fileLocationArray = [[NSMutableArray alloc] init];
	else 
		[fileLocationArray removeAllObjects];
	
	// load files from local
	const char* local_dir = g_getFlashLocalPath(mobileView);
	XFileFinder finder;
	bool find = finder.FindFirst(local_dir);
	while (find) 
	{
		XString8 name = finder.cFileName;
		int dotPos = name.ReverseFind('.');
		
		if (dotPos != -1)
		{
			XString8 right = name.GetData() + dotPos + 1;
			if (right == "swf")
			{
				XString8 local = local_dir;
				XString8 path = local + "/" + finder.cFileName;
				XString8 left = name;
				left.Left(left, dotPos);
				NSString* str = (NSString*)gbkToNSString(left);
				[fileNameArray addObject:str];
				[str release];
				str = (NSString*)gbkToNSString(path);
				[fileLocationArray addObject:str];
				[str release];
			}
		}
		find = finder.FindNext();
	}
	[fileList reloadData];
}

- (void)setMobileView:(void*)m
{
	mobileView = m;
}

- (void*)mobileView
{
	return mobileView;
}

- (BOOL)shouldAutorotate
{
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    else
    {
        return UIInterfaceOrientationMaskLandscapeRight;
    }
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (g_deviceData.m_deviceOrientation == UIInterfaceOrientationPortrait)
    {
        return interfaceOrientation == UIInterfaceOrientationPortrait;
    }
    else
    {
        return interfaceOrientation == UIInterfaceOrientationLandscapeRight;
    }
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}
-(void)viewDidLoad
{
    [super viewDidLoad];
    CGRect naviBarBoundIphone;
    naviBarBoundIphone.origin.x = 0;
    naviBarBoundIphone.origin.y = 0;
    naviBarBoundIphone.size.width = g_deviceData.m_screenWidth;
    naviBarBoundIphone.size.height = 44;

    UINavigationBar *navBar = [[UINavigationBar alloc]initWithFrame:naviBarBoundIphone];
    UINavigationItem *barItem;
    barItem = [[UINavigationItem alloc]initWithTitle:@"本地"];
    
    
    UIBarButtonItem * barButtonItem = [[UIBarButtonItem alloc]initWithTitle:@"完成" style:UIBarButtonItemStyleDone target:self action:@selector(onDoneButton)];
    [barItem setLeftBarButtonItem:barButtonItem];
    UIBarButtonItem * barButtonItemEdit = [[UIBarButtonItem alloc]initWithTitle:@"编辑" style:UIBarButtonItemStyleBordered target:self action:@selector(onEditButton)];
    [barItem setRightBarButtonItem:barButtonItemEdit];
    [barButtonItem release];
    [barButtonItemEdit release];
    [navBar setItems:[NSArray arrayWithObject:barItem]];
    [barItem release];
    [self.view addSubview:navBar];
    [navBar release];
    [self.view addSubview:fileList];
}
- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
	[fileNameArray release];
	[fileLocationArray release];
	[fileList release];
	[editButton release];
	[completeButton release];
	
    [super dealloc];
}

#pragma mark -
#pragma mark Buttons' Actions
- (void)onEditButton
{
	if (!fileList.editing)
	{
		[editButton setEnabled:NO];
		[fileList setEditing:YES animated:YES];
	}
	else 
	{
        if ([fileLocationArray count]==0)
            [editButton setEnabled:NO];
        else 
		[editButton setEnabled:YES];
		[fileList setEditing:NO animated:YES];
	}
}

- (void)onDoneButton
{
	if (fileList.editing)
	{
		[fileList reloadSectionIndexTitles];
		[self onEditButton];
        // 如果没有flash，直接隐藏本地
        if ([fileLocationArray count]==0)
            [self dismissModalViewControllerAnimated:YES];
	}
	else 
		[self dismissModalViewControllerAnimated:YES];
}

#pragma mark -
#pragma mark TableView Delegate
- (void)tableView:(UITableView*)aTableView didSelectRowAtIndexPath:(NSIndexPath*)indexPath
{
	if (fileList.editing)
		[self onDoneButton];
	[self onDoneButton];
    NSUInteger row = [indexPath row];
    [self performSelector:@selector(openFlashFile:)
               withObject:[fileLocationArray objectAtIndex:row] afterDelay:0.5];
}

- (void)openFlashFile:(void (^)(void))block
{
    char* utf8 = NULL;
	utf8 = NSStringToUtf8(block);
	g_mobileViewConnect(mobileView, utf8);
	delete[] utf8;
}

- (NSString *)tableView:(UITableView *)tableView titleForDeleteConfirmationButtonForRowAtIndexPath:(NSIndexPath *)indexPath
{	
	return @"删除";
}

#pragma mark -
#pragma mark TableView DataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	return 1;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
	return @"";
}

- (NSInteger)tableView:(UITableView*)tableView numberOfRowsInSection:(NSInteger)section 
{
	return [fileNameArray count];
}

- (UITableViewCell*)tableView:(UITableView*)aTableView cellForRowAtIndexPath:(NSIndexPath*)indexPath
{
	NSUInteger row = [indexPath row];
	UITableViewCell* cell = nil;
	
	static NSString* id_str = @"flash_id";
	cell = [fileList dequeueReusableCellWithIdentifier:id_str];
	if (cell == nil)
		cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:id_str] autorelease];
	
	cell.textLabel.text = [fileNameArray objectAtIndex:row];
	cell.textLabel.font = [UIFont boldSystemFontOfSize:25];
	
	return cell;
}

- (void)tableView:(UITableView *)aTableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{	
	NSUInteger row = [indexPath row];
	// real delete flash file.
	char* path = NULL;
	path = NSStringToGbk([fileLocationArray objectAtIndex:row]);

	XString8 cfg_path = path;
	int dotPos = cfg_path.ReverseFind('.');
	cfg_path.Left(cfg_path, dotPos);
	cfg_path = cfg_path + ".cfg";
	XFile::RemoveFile(cfg_path.GetData());
	XFile::RemoveFile(path);
	
	[fileLocationArray removeObjectAtIndex:row];
	[fileNameArray removeObjectAtIndex:row];
	
	[fileList deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
}
- (BOOL)prefersStatusBarHidden
{
    return YES;
}
@end
