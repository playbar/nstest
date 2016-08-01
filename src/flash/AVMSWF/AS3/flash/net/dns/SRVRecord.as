package flash.net.dns
{
[native(cls="SRVRecordClass", methods="auto")]

public class SRVRecord extends ResourceRecord
{
	//Consts
	
	//Properties
	public native function get port() : int
	public native function set port(port : int):void
	
	public native function get priority() : int
	public native function set priority(priority : int):void
	
	public native function get target() : String
	public native function set target(target : String):void
	
	public native function get weight() : int
	public native function set weight(weight : int):void
	
	//Methods
	public native function SRVRecord()
}
}