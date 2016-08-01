package flash.sensors
{
[native(cls="GeolocationClass", methods="auto")]

import flash.events.EventDispatcher

public class Geolocation extends EventDispatcher
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	public native function get muted() : Boolean
	
	//Methods
//	public native function Geolocation()
	public native function setRequestedUpdateInterval(interval:Number):void
	
	public function Geolocation()
        {
            return;
        }// end function
}
}