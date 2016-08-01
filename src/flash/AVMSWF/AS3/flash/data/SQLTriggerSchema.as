package flash.data
{
[native(cls="SQLTriggerSchemaClass", methods="auto")]

public class SQLTriggerSchema extends SQLSchema
{
	//Consts
	
	//Properties
	public native function get table() : String
	
	//Methods
	public native function SQLTriggerSchema(database:String, name:String, sql:String, table:String)
}
}