package flash.net.dns
{
[native(cls="ARecordClass", methods="auto")]

public class ARecord extends ResourceRecord
{
	//Consts
	
	//Properties
	public native function get address() : String
	public native function set address(address : String):void
	
	//Methods
	public native function ARecord()
}
}