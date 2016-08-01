package flash.display
{
[native(cls="ScreenClass", methods="auto")]

import flash.events.EventDispatcher

import flash.geom.Rectangle

public class Screen extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get bounds() : Rectangle
	
	public native function get colorDepth() : int
	
	public native static function get mainScreen() : Screen
	
	public native static function get screens() : Array
	
	public native function get visibleBounds() : Rectangle
	
	//Methods
	public native static function getScreensForRectangle(rect:Rectangle):Array
}
}