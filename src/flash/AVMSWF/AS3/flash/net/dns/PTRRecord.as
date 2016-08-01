package flash.net.dns
{
[native(cls="PTRRecordClass", methods="auto")]

public class PTRRecord extends ResourceRecord
{
	//Consts
	
	//Properties
	public native function get ptrdName() : String
	public native function set ptrdName(ptrdName : String):void
	
	//Methods
	public native function PTRRecord()
}
}