package flash.data
{
[native(cls="SQLSchemaClass", methods="auto")]

public class SQLSchema
{
	//Consts
	
	//Properties
	public native function get database() : String
	
	public native function get name() : String
	
	public native function get sql() : String
	
	//Methods
	public native function SQLSchema(database:String, name:String, sql:String)
}
}