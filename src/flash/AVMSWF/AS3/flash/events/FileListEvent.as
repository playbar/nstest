package flash.events
{
[native(cls="FileListEventClass", methods="auto")]

public class FileListEvent extends Event
{
	//Consts
	public static const DIRECTORY_LISTING : String = "directoryListing"
	public static const SELECT_MULTIPLE : String = "selectMultiple"
	
	//Properties
	public native function get files() : Array
	public native function set files(files : Array):void
	
	//Methods
	public native function FileListEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, files:Array = null)
}
}