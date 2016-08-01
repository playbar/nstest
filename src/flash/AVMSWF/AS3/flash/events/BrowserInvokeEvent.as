package flash.events
{
[native(cls="BrowserInvokeEventClass", methods="auto")]

public class BrowserInvokeEvent extends Event
{
	//Consts
	public static const BROWSER_INVOKE : String = "browserInvoke"
	
	//Properties
	public native function get arguments() : Array
	
	public native function get isHTTPS() : Boolean
	
	public native function get isUserEvent() : Boolean
	
	public native function get sandboxType() : String
	
	public native function get securityDomain() : String
	
	//Methods
	public native function BrowserInvokeEvent(type:String, bubbles:Boolean, cancelable:Boolean, arguments:Array, sandboxType:String, securityDomain:String, isHTTPS:Boolean, isUserEvent:Boolean)
	public native override function clone():Event
}
}