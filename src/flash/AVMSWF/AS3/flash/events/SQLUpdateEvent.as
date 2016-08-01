package flash.events
{
[native(cls="SQLUpdateEventClass", methods="auto")]

public class SQLUpdateEvent extends Event
{
	//Consts
	public static const DELETE : String = "delete"
	public static const INSERT : String = "insert"
	public static const UPDATE : String = "update"
	
	//Properties
	public native function get rowID() : Number
	
	public native function get table() : String
	
	//Methods
	public native function SQLUpdateEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, table:String = null, rowID:Number = 0)
	public native override function clone():Event
}
}