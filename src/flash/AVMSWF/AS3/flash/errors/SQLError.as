package flash.errors
{
[native(cls="SQLErrorClass", instance="SQLErrorObject", methods="auto")]

public class SQLError extends Error
{
	//Consts
	
	//Properties
	public native function get detailArguments() : Array
	
	public native function get detailID() : int
	
	public native function get details() : String
	
	public native function get operation() : String
	
	//Methods
	public native function SQLError(operation:String, details:String = "", message:String = "", id:int = 0, detailID:int = -1, detailArgs:Array = null)
	public native function toString():String
}
}