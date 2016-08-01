package flash.events
{
[native(cls="ScreenMouseEventClass", methods="auto")]

public class ScreenMouseEvent extends MouseEvent
{
	//Consts
	public static const CLICK : String = "click"
	public static const MOUSE_DOWN : String = "mouseDown"
	public static const MOUSE_UP : String = "mouseUp"
	public static const RIGHT_CLICK : String = "rightClick"
	public static const RIGHT_MOUSE_DOWN : String = "rightMouseDown"
	public static const RIGHT_MOUSE_UP : String = "rightMouseUp"
	
	//Properties
	public native function get screenX() : Number
	
	public native function get screenY() : Number
	
	//Methods
	public native function ScreenMouseEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, screenX:Number = NaN, screenY:Number = NaN, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, buttonDown:Boolean = false, commandKey:Boolean = false, controlKey:Boolean = false)
	public native override function clone():Event
	public native override function toString():String
}
}