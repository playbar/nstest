package flash.net
{
[native(cls="InterfaceAddressClass", methods="auto")]

public class InterfaceAddress
{
	//Consts
	
	//Properties
	public native function get address() : String
	public native function set address(address : String):void
	
	public native function get broadcast() : String
	public native function set broadcast(broadcast : String):void
	
	public native function get ipVersion() : String
	public native function set ipVersion(ipVersion : String):void
	
	public native function get prefixLength() : int
	public native function set prefixLength(prefixLength : int):void
	
	//Methods
}
}