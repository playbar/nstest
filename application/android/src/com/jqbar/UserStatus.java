package com.jqbar;
/*
 * 定义玩家状态
 */

public class UserStatus
{
	//定义Flash状态
	public static final int USER_NORMAL = 1000;		//正常状态
	public static final int FLASH_HELP = USER_NORMAL + 101;
	public static final int FLASH_ORIG = USER_NORMAL + 102;
	public static final int FLASH_FIT = USER_NORMAL + 103;
	public static final int FLASH_CLOSE = USER_NORMAL + 104;
	public static final int FLASH_SCORE = USER_NORMAL + 105;
	public static final int FLASH_RESTART = USER_NORMAL + 106;
	public static final int FLASH_SAVE = USER_NORMAL + 107;
	public static final int FLASH_FULLSCREEN = USER_NORMAL + 108;
	public static final int FLASH_ZOOMMODE = USER_NORMAL + 109;
	public static final int BUTTON_SWITCH = USER_NORMAL + 110;
	public static final int UPDATE_SWITCH = USER_NORMAL + 111;
	public static final int FLASH_MOVEMODE = USER_NORMAL + 112;
	public static final int FLASH_ZOOMINOUT = USER_NORMAL + 113;
	public static final int FLASH_TYPE_GAME = USER_NORMAL + 114;
	public static final int FLASH_TYPE_MEDIA = USER_NORMAL + 115;
	public static final int FLASH_TYPE_WEBGAME = USER_NORMAL + 116;

	//窗口状态
	public static final int VIEW_NONE = 0;
	public static final int VIEW_NORMAL = 1;
	public static final int VIEW_SMALL = 2;
	public static final int VIEW_MIDDLE = 3;
	public static final int VIEW_LARGE = 4;
	public static final int VIEW_FULLSCREEN = 5;

	//弹出窗口状态
	public static final int VIEW_POP_NONE = 0;
	public static final int VIEW_POP_HELP = 1;
	//public static final int VIEW_POP_SCORE = 2;
	//public static final int VIEW_POP_SETTING = 3;
	public static final int VIEW_POP_PAY = 4;

	//用户操作动作
	public static final int USER_ACTION_NONE = 0;
	public static final int USER_ACTION_TOUCH_DOWN = 1;
	public static final int USER_ACTION_TOUCH_POINTER_DOWN = 2;
	public static final int USER_ACTION_TOUCH_ZOOM = 3;
	public static final int USER_ACTION_TOUCH_MOVE = 4;
	public static final int USER_ACTION_TOUCH_MOVE_POINTER = 5;
	public static final int USER_ACTION_TOUCH_POINTER_UP = 6;
	public static final int USER_ACTION_TOUCH_UP = 7;
    public static final int USER_ACTION_BTN_UP_DOWN    = USER_ACTION_TOUCH_UP + 1; // 点击向上按钮
    public static final int USER_ACTION_BTN_UP_UP      = USER_ACTION_TOUCH_UP + 2; // 点击向上按钮
    public static final int USER_ACTION_BTN_DOWN_DOWN  = USER_ACTION_TOUCH_UP + 3; // 点击向下按钮
    public static final int USER_ACTION_BTN_DOWN_UP    = USER_ACTION_TOUCH_UP + 4; // 点击向下按钮
    public static final int USER_ACTION_BTN_LEFT_DOWN  = USER_ACTION_TOUCH_UP + 5; // 点击向左按钮
    public static final int USER_ACTION_BTN_LEFT_UP    = USER_ACTION_TOUCH_UP + 6; // 点击向左按钮
    public static final int USER_ACTION_BTN_RIGHT_DOWN = USER_ACTION_TOUCH_UP + 7; // 点击向右按钮
    public static final int USER_ACTION_BTN_RIGHT_UP   = USER_ACTION_TOUCH_UP + 8; // 点击向右按钮

	//窗口菜单状态
	public static final int MENU_NONE = 0;
	public static final int MENU_BROWSER = 1;
	public static final int MENU_BROWSER_POP = 2;
	public static final int MENU_MEDIA = 3;
	public static final int MENU_GAME = 4;
	public static final int MENU_WEBGAME = 5;
	public static final int MENU_LOADING = 6;
	public static final int MENU_FLOAT_VIEW = 7;
	public static final int MENU_SETTING_FLASH = 8;
	public static final int MENU_SETTING_BROWSER= 9;
	public static final int MENU_GAME_OLD=10;
	public static final int MENU_MEDIA_OLD=11;
	public static final int MENU_WEBGAME_OLD=12;

	//编辑框风格
	public static final int XEDS_MULTILINE = 0x80;
	public static final int XEDS_PASSWORD = 0x40;
	public static final int XEDS_LCHAR = 0x20;
	public static final int XEDS_UCHAR = 0x10;
	public static final int XEDS_NUMBER = 0x08;
	public static final int XEDS_SIGN = 0x04;
	public static final int XEDS_WCHAR = 0x02;
	public static final int XEDS_UPLIST = 0x01;
	public static final int XEDS_ANY = 0x3e;

	//更新状态
	public static final int UPGRADE_NOMAL = 0;
	public static final int UPGRADE_MUST = 1;
	public static final int UPGRADE_CAN = 2;
	public static final int UPGRADE_CANCEL = 3;



}



