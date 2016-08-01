package flash.events
{
	[native(cls="ThrottleEventClass", methods="auto")]
	
	import flash.events.Event
	import flash.events.ThrottleType
	import flash.display.Stage
	
	public class ThrottleEvent extends Event
	{
		//const
		
		public static const THROTTLE:String = "throttle"
		
		//Properties
		
		public native function get state():String
		public native function get targetFrameRate():Number
		
		//Methods
		public function ThrottleEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, state:String = null, targetFrameRate:Number = 0)
		{
			super(type, bubbles, cancelable);
			this.state = state;
			this.targetFrameRate = targetFrameRate;
			return;
		}
		
		override public native function clone():Event
		
		override public native function toString():String
		
	}
}