package flash.net.dns
{
[native(cls="DNSResolverClass", methods="auto")]

import flash.events.EventDispatcher

public class DNSResolver extends EventDispatcher
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	//Methods
	public native function DNSResolver()
	public native function lookup(host:String, recordType:Class):void
}
}