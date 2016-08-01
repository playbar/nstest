package flash.data
{
[native(cls="SQLResultClass", methods="auto")]

public class SQLResult
{
	//Consts
	
	//Properties
	public native function get complete() : Boolean
	
	public native function get data() : Array
	
	public native function get lastInsertRowID() : Number
	
	public native function get rowsAffected() : Number
	
	//Methods
	public native function SQLResult(data:Array = null, rowsAffected:Number = 0, complete:Boolean = true, rowID:Number = 0)
}
}