package flash.net.dns
{
[native(cls="ResourceRecordClass", methods="auto")]

public class ResourceRecord
{
	//Consts
	
	//Properties
	public native function get name() : String
	public native function set name(name : String):void
	
	public native function get ttl() : int
	public native function set ttl(ttl : int):void
	
	//Methods
}
}