package flash.data
{
[native(cls="SQLColumnSchemaClass", methods="auto")]

public class SQLColumnSchema
{
	//Consts
	
	//Properties
	public native function get allowNull() : Boolean
	
	public native function get autoIncrement() : Boolean
	
	public native function get dataType() : String
	
	public native function get defaultCollationType() : String
	
	public native function get name() : String
	
	public native function get primaryKey() : Boolean
	
	//Methods
	public native function SQLColumnSchema(name:String, primaryKey:Boolean, allowNull:Boolean, autoIncrement:Boolean, dataType:String, defaultCollationType:String)
}
}