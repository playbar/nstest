package flash.events
{
[native(cls="ServerSocketConnectEventClass", methods="auto")]

import flash.net.Socket

public class ServerSocketConnectEvent extends Event
{
	//Consts
	public static const CONNECT : String = "connect"
	
	//Properties
	public native function get socket() : Socket
	public native function set socket(socket : Socket):void
	
	//Methods
	public native function ServerSocketConnectEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, socket:Socket = null)
	public native override function clone():Event
	public native override function toString():String
}
}