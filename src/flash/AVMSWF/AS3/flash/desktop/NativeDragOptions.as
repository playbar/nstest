package flash.desktop
{
[native(cls="NativeDragOptionsClass", methods="auto")]

public class NativeDragOptions
{
	//Consts
	
	//Properties
	public native function get allowCopy() : Boolean
	public native function set allowCopy(allowCopy : Boolean):void
	
	public native function get allowLink() : Boolean
	public native function set allowLink(allowLink : Boolean):void
	
	public native function get allowMove() : Boolean
	public native function set allowMove(allowMove : Boolean):void
	
	//Methods
	public native function toString():String
}
}