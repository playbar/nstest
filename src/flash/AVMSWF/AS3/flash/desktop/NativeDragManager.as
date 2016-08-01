package flash.desktop
{
[native(cls="NativeDragManagerClass", methods="auto")]

import flash.display.InteractiveObject

public class NativeDragManager
{
	//Consts
	
	//Properties
	public native static function get dragInitiator() : InteractiveObject
	
	public native static function get dropAction() : String
	public native static function set dropAction(dropAction : String):void
	
	public native static function get isDragging() : Boolean
	
	public native static function get isSupported() : Boolean
	
	//Methods
}
}