package flash.desktop
{
[native(cls="DockIconClass", methods="auto")]

import flash.display.NativeMenu

public class DockIcon extends InteractiveIcon
{
	//Consts
	
	//Properties
	public native override function get bitmaps() : Array
	public native override function set bitmaps(bitmaps : Array):void
	
	public native override function get height() : int
	
	public native function get menu() : NativeMenu
	public native function set menu(menu : NativeMenu):void
	
	public native override function get width() : int
	
	//Methods
	public native function bounce(priority:String = "informational"):void
}
}