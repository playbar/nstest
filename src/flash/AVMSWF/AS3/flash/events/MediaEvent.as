package flash.events
{
[native(cls="MediaEventClass", methods="auto")]

import flash.media.MediaPromise

public class MediaEvent extends Event
{
	//Consts
	public static const COMPLETE : String = "complete"
	public static const SELECT : String = "select"
	
	//Properties
	public native function get data() : MediaPromise
	
	//Methods
	public native function MediaEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, data:MediaPromise = null)
	public native override function clone():Event
	public native override function toString():String
}
}