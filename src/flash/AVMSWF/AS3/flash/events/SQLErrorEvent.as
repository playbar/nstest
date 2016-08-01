package flash.events
{
[native(cls="SQLErrorEventClass", methods="auto")]

import flash.errors.SQLError

public class SQLErrorEvent extends ErrorEvent
{
	//Consts
	public static const ERROR : String = "error"
	
	//Properties
	public native function get error() : SQLError
	
	//Methods
	public native function SQLErrorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, error:SQLError = null)
	public native override function clone():Event
	public native override function toString():String
}
}