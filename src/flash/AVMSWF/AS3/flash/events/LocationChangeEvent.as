package flash.events
{
[native(cls="LocationChangeEventClass", methods="auto")]

public class LocationChangeEvent extends Event
{
	//Consts
	public static const LOCATION_CHANGE : String = "locationChange"
	public static const LOCATION_CHANGING : String = "locationChanging"
	
	//Properties
	public native function get location() : String
	public native function set location(location : String):void
	
	//Methods
	public native function LocationChangeEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, location:String = null)
	public native override function clone():Event
	public native override function toString():String
}
}