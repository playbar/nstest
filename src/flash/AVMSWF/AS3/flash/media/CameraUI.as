package flash.media
{
[native(cls="CameraUIClass", methods="auto")]

import flash.events.EventDispatcher

public class CameraUI extends EventDispatcher
{
	//Consts
	
	//Properties
	public native static function get isSupported() : Boolean
	
	//Methods
	public native function CameraUI()
	public native function launch(requestedMediaType:String):void
}
}