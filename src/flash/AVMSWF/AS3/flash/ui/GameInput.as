package flash.ui
{
	[native(cls="GameInputClass",methods="auto")]
	
	import flash.events.EventDispatcher
	import flash.ui.GameInputDevice
	
	public final class GameInput extends EventDispatcher
	{
		//Const
		
		//Properties
		
		public native static function get isSupported():Boolean
		public native static function get numDevices():int
		//Methods
		public native static function getDeviceAt(index:int):GameInputDevice
		
	}
}