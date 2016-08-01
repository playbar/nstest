package flash.events
{
	[native(cls="RemoteNotificationEventClass", methods="auto")]
	
	import flash.events.Event
	flash.display.DisplayObject
	flash.display.Stage
	flash.events.ThrottleType
	
	public final class RemoteNotificationEvent extends Event
	{
		//const
		
		public static const NOTIFICATION:String = "notification"
		public static const TOKEN:String = "token"
		
		//Properties
		
		public native function get data():Object
		public native function get tokenId():String
		
		//Methods
		public function RemoteNotificationEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, data:Object = null, tokenId:String = null)
		{
			super( type, bubbles, cancelable )
			this.data = data;
			this.tokenId = tokenId;
			return;
		}
	}
}