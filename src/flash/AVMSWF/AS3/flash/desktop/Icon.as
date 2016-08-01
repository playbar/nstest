package flash.desktop
{
[native(cls="IconClass", methods="auto")]

import flash.events.EventDispatcher

public class Icon extends EventDispatcher
{
	//Consts
	
	//Properties
	public native function get bitmaps() : Array
	public native function set bitmaps(bitmaps : Array):void
	
	//Methods
}
}