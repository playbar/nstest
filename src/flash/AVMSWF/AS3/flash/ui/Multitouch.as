package flash.ui
{
[native(cls="MultitouchClass", methods="auto")]

public class Multitouch
{
	//Consts
	
	//Properties
	public native static function get inputMode() : String
	public native static function set inputMode(inputMode : String):void
	
	public native static function get maxTouchPoints() : int
	
	public native static function get supportedGestures() : Vector.<String>
	
	public native static function get supportsGestureEvents() : Boolean
	
	public native static function get supportsTouchEvents() : Boolean
	
	public native static function get mapTouchToMouse() : Boolean

        public native static function set mapTouchToMouse(value:Boolean) : void
	//Methods
}
}