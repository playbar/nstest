package flash.ui
{
	[native(cls="GameInputControlClass",methods="auto")]
	
	import flash.events.EventDispatcher
	import flash.ui.GameInputDevice
	
	public dynamic class GameInputControl extends EventDispatcher
	{
		//Const
		
		//Properties
		
		public native function get device():GameInputDevice
		public native function get id():String
		public native function get maxValue():Number
		public native function get minValue():Number
		public native function get value():Number
		
		//Methods
		
	}
}