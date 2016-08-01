package flash.text.engine
{
[native(cls="TextLineMirrorRegionClass", methods="auto")]

import flash.geom.Rectangle
import flash.events.EventDispatcher

public class TextLineMirrorRegion
{
	//Consts
	
	//Properties
	public native function get bounds() : Rectangle
	
	public native function get element() : ContentElement
	
	public native function get mirror() : EventDispatcher
	
	public native function get nextRegion() : TextLineMirrorRegion
	
	public native function get previousRegion() : TextLineMirrorRegion
	
	public native function get textLine() : TextLine
	
	//Methods
}
}