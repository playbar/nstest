package flash.events
{
	[native(cls="GameInputEventClass", methods="auto")]
	
	import flash.events.Event
	import flash.ui.GameInputDevice
	
	public final class GameInputEvent extends Event
	{
		//const
		
		public static const DEVICE_ADDED:String = "deviceAdded"
		public static const DEVICE_REMOVED:String = "deviceRemoved"
		public static const DEVICE_UNUSABLE:String = "deviceUnusable"
		
		//Properties
		
		public native function get device():GameInputDevice
		
		//Methods
		public function GameInputEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, device:GameInputDevice = null)
		{
			super( type, bubbles, cancelable );
			this.device = device;
			return;
		}
	}
}