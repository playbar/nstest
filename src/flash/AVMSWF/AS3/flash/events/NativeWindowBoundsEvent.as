package flash.events
{
[native(cls="NativeWindowBoundsEventClass", methods="auto")]

import flash.geom.Rectangle

public class NativeWindowBoundsEvent extends Event
{
	//Consts
	public static const MOVE : String = "move"
	public static const MOVING : String = "moving"
	public static const RESIZE : String = "resize"
	public static const RESIZING : String = "resizing"
	
	//Properties
	public native function get afterBounds() : Rectangle
	
	public native function get beforeBounds() : Rectangle
	
	//Methods
	public native function NativeWindowBoundsEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, beforeBounds:Rectangle = null, afterBounds:Rectangle = null)
	public native override function clone():Event
	public native override function toString():String
}
}