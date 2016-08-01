package flash.net.dns
{
[native(cls="MXRecordClass", methods="auto")]

public class MXRecord extends ResourceRecord
{
	//Consts
	
	//Properties
	public native function get exchange() : String
	public native function set exchange(exchange : String):void
	
	public native function get preference() : int
	public native function set preference(preference : int):void
	
	//Methods
	public native function MXRecord()
}
}