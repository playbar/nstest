package flash.data
{
[native(cls="SQLTableSchemaClass", methods="auto")]

public class SQLTableSchema extends SQLSchema
{
	//Consts
	
	//Properties
	public native function get columns() : Array
	
	//Methods
	public native function SQLTableSchema(database:String, name:String, sql:String, columns:Array)
}
}