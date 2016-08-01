package flash.events
{
[native(cls="InvokeEventClass", methods="auto")]

import flash.filesystem.File

public class InvokeEvent extends Event
{
	//Consts
	public static const INVOKE : String = "invoke"
	
	//Properties
	public native function get arguments() : Array
	
	public native function get currentDirectory() : File
	
	public native function get reason() : String
	
	//Methods
	public native function InvokeEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, dir:File = null, argv:Array = null, reason:String = "standard")
	public native override function clone():Event
}
}