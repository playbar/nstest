package flash.events
{
[native(cls="SQLEventClass", methods="auto")]

public class SQLEvent extends Event
{
	//Consts
	public static const ANALYZE : String = "analyze"
	public static const ATTACH : String = "attach"
	public static const BEGIN : String = "begin"
	public static const CANCEL : String = "cancel"
	public static const CLOSE : String = "close"
	public static const COMMIT : String = "commit"
	public static const COMPACT : String = "compact"
	public static const DEANALYZE : String = "deanalyze"
	public static const DETACH : String = "detach"
	public static const OPEN : String = "open"
	public static const REENCRYPT : String = "reencrypt"
	public static const RELEASE_SAVEPOINT : String = "releaseSavepoint"
	public static const RESULT : String = "result"
	public static const ROLLBACK : String = "rollback"
	public static const ROLLBACK_TO_SAVEPOINT : String = "rollbackToSavepoint"
	public static const SCHEMA : String = "schema"
	public static const SET_SAVEPOINT : String = "setSavepoint"
	
	//Properties
	
	//Methods
	public native function SQLEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false)
	public native override function clone():Event
}
}