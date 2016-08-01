package flash.data
{
[native(cls="SQLSchemaResultClass", methods="auto")]

public class SQLSchemaResult
{
	//Consts
	
	//Properties
	public native function get indices() : Array
	
	public native function get tables() : Array
	
	public native function get triggers() : Array
	
	public native function get views() : Array
	
	//Methods
	public native function SQLSchemaResult(tables:Array, views:Array, indices:Array, triggers:Array)
}
}