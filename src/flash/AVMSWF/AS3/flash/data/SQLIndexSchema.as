package flash.data
{
[native(cls="SQLIndexSchemaClass", methods="auto")]

public class SQLIndexSchema extends SQLSchema
{
	//Consts
	
	//Properties
	public native function get table() : String
	
	//Methods
	public native function SQLIndexSchema(database:String, name:String, sql:String, table:String)
}
}