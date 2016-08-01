package flash.events
{
[native(cls="NativeWindowDisplayStateEventClass", methods="auto")]

public class NativeWindowDisplayStateEvent extends Event
{
	//Consts
	public static const DISPLAY_STATE_CHANGE : String = "displayStateChange"
	public static const DISPLAY_STATE_CHANGING : String = "displayStateChanging"
	
	//Properties
	public native function get afterDisplayState() : String
	
	public native function get beforeDisplayState() : String
	
	//Methods
	public native function NativeWindowDisplayStateEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, beforeDisplayState:String = "", afterDisplayState:String = "")
	public native override function clone():Event
	public native override function toString():String
}
}