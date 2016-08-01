package flash.events
{
[native(cls="NativeProcessExitEventClass", methods="auto")]

public class NativeProcessExitEvent extends Event
{
	//Consts
	public static const EXIT : String = "exit"
	
	//Properties
	public native function get exitCode() : Number
	public native function set exitCode(exitCode : Number):void
	
	//Methods
	public native function NativeProcessExitEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, exitCode:Number = NaN)
	public native override function clone():Event
	public native override function toString():String
}
}