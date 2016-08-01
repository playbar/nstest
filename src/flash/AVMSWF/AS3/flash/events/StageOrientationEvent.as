package flash.events
{
[native(cls="StageOrientationEventClass", methods="auto")]

public class StageOrientationEvent extends Event
{
	//Consts
	public static const ORIENTATION_CHANGE : String = "orientationChange"
	public static const ORIENTATION_CHANGING : String = "orientationChanging"
	
	//Properties
	public native function get afterOrientation() : String
	
	public native function get beforeOrientation() : String
	
	//Methods
	public native function StageOrientationEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, beforeOrientation:String = null, afterOrientation:String = null)
	public native override function clone():Event
	public native override function toString():String
}
}