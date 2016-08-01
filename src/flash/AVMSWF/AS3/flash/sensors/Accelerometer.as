package flash.sensors
{
[native(cls="AccelerometerClass", methods="auto")]

import flash.events.EventDispatcher

public class Accelerometer extends EventDispatcher
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	public native function get muted() : Boolean
	
	//Methods
//	public native function Accelerometer()
	public native function setRequestedUpdateInterval(interval:Number):void
	
	public function Accelerometer()
        {
            return;
        }// end function
}
}