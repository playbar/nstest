package flash.net
{
[native(cls="ServerSocketClass", methods="auto")]

import flash.events.EventDispatcher

public class ServerSocket extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get bound() : Boolean
	
	public native static function get isSupported() : Boolean
	
	public native function get listening() : Boolean
	
	public native function get localAddress() : String
	
	public native function get localPort() : int
	
	//Methods
	public native function ServerSocket()
	public native function bind(localPort:int = 0, localAddress:String = "0.0.0.0"):void
	public native function close():void
	public native function listen(backlog:int = 0):void
}
}