package flash.desktop
{
[native(cls="InteractiveIconClass", methods="auto")]

public class InteractiveIcon extends Icon
{
	//Consts
	
	//Properties
	public native override function get bitmaps() : Array
	public native override function set bitmaps(bitmaps : Array):void
	
	public native function get height() : int
	
	public native function get width() : int
	
	//Methods
}
}