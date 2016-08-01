package flash.net
{
[native(cls="NetworkInfoClass", methods="auto")]

import flash.events.EventDispatcher

public class NetworkInfo extends EventDispatcher
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	public native static function get networkInfo() : NetworkInfo
	
	//Methods
	public native function findInterfaces():Vector.<NetworkInterface>
}
}