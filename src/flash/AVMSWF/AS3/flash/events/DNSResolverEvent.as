package flash.events
{
[native(cls="DNSResolverEventClass", methods="auto")]

public class DNSResolverEvent extends Event
{
	//Consts
	public static const LOOKUP : String = "lookup"
	
	//Properties
	public native function get host() : String
	public native function set host(host : String):void
	
	public native function get resourceRecords() : Array
	public native function set resourceRecords(resourceRecords : Array):void
	
	//Methods
	public native function DNSResolverEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, host:String = "", resourceRecords:Array = null)
	public native override function clone():Event
	public native override function toString():String
}
}