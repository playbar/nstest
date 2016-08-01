package flash.events
{
[native(cls="HTMLUncaughtScriptExceptionEventClass", methods="auto")]

public class HTMLUncaughtScriptExceptionEvent extends Event
{
	//Consts
//	public static const UNCAUGHT_SCRIPT_EXCEPTION : * = "uncaughtScriptException"	

	public static const UNCAUGHT_SCRIPT_EXCEPTION : String = "uncaughtScriptException"	
	
	//Properties
	public native function get exceptionValue() : *
	public native function set exceptionValue(exceptionValue : *):void
	
	public native function get stackTrace() : Array
	public native function set stackTrace(stackTrace : Array):void
	
	//Methods
	public native function HTMLUncaughtScriptExceptionEvent(exceptionValue:*)
	public native override function clone():Event
}
}