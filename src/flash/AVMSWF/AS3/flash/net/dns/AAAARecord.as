package flash.net.dns
{
[native(cls="AAAARecordClass", methods="auto")]

public class AAAARecord extends ResourceRecord
{
	//Consts
	
	//Properties
	public native function get address() : String
	public native function set address(address : String):void
	
	//Methods
	public native function AAAARecord()
}
}