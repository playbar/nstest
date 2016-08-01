package flash.display
{
[native(cls="NativeWindowInitOptionsClass", methods="auto")]

public class NativeWindowInitOptions
{
	//Consts
	
	//Properties
	public native function get maximizable() : Boolean
	public native function set maximizable(maximizable : Boolean):void
	
	public native function get minimizable() : Boolean
	public native function set minimizable(minimizable : Boolean):void
	
	public native function get owner() : NativeWindow
	public native function set owner(owner : NativeWindow):void
	
	public native function get resizable() : Boolean
	public native function set resizable(resizable : Boolean):void
	
	public native function get systemChrome() : String
	public native function set systemChrome(systemChrome : String):void
	
	public native function get transparent() : Boolean
	public native function set transparent(transparent : Boolean):void
	
	public native function get type() : String
	public native function set type(type : String):void
	
	//new adds 11
	public native function get renderMode():String
    public native function set renderMode(value:String):void
	//new adds 11 end
	
	//Methods
	public native function NativeWindowInitOptions()
}
}