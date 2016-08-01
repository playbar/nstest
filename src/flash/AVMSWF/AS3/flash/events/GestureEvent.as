package flash.events
{
[native(cls="GestureEventClass", methods="auto")]

public class GestureEvent extends Event
{
	//Consts
	public static const GESTURE_TWO_FINGER_TAP : String = "gestureTwoFingerTap"
	
	//Properties
	public native function get altKey() : Boolean
	public native function set altKey(altKey : Boolean):void
	
	public native function get commandKey() : Boolean
	public native function set commandKey(commandKey : Boolean):void
	
	public native function get controlKey() : Boolean
	public native function set controlKey(controlKey : Boolean):void
	
	public native function get ctrlKey() : Boolean
	public native function set ctrlKey(ctrlKey : Boolean):void
	
	public native function get localX() : Number
	public native function set localX(localX : Number):void
	
	public native function get localY() : Number
	public native function set localY(localY : Number):void
	
	public native function get phase() : String
	public native function set phase(phase : String):void
	
	public native function get shiftKey() : Boolean
	public native function set shiftKey(shiftKey : Boolean):void
	
	public native function get stageX() : Number
	
	public native function get stageY() : Number
	
	//Methods
	public native function GestureEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, phase:String = null, localX:Number = 0, localY:Number = 0, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, commandKey:Boolean = false, controlKey:Boolean = false)
	public native override function clone():Event
	public native override function toString():String
	public native function updateAfterEvent():void
}
}