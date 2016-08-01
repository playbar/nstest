package flash.desktop
{
[native(cls="SystemTrayIconClass", methods="auto")]

import flash.display.NativeMenu

public class SystemTrayIcon extends InteractiveIcon
{
	//Consts
	public static const MAX_TIP_LENGTH : Number = 63
	
	//Properties
	public native override function get bitmaps() : Array
	public native override function set bitmaps(bitmaps : Array):void
	
	public native override function get height() : int
	
	public native function get menu() : NativeMenu
	public native function set menu(menu : NativeMenu):void
	
	public native function get tooltip() : String
	public native function set tooltip(tooltip : String):void
	
	public native override function get width() : int
	
	//Methods
	
}
}