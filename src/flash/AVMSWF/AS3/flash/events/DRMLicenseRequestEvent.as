package flash.events
{
	[native(cls="DRMLicenseRequestEventClass", methods="auto")]
	
	import flash.events.event
	
	public class DRMLicenseRequestEvent extends Event
	{
		//const
		public static const LICENSE_REQUEST:String = "licenseRequest"
		
		//Properties
		//private var m_serverURL:String
		public native function get serverURL():String
		public native function set serverURL(value:String):void
		
		//methods
		public function DRMLicenseRequestEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inServerURL:String = null)
		{
			super(type, bubbles, cancelable);
			this.serverURL = inServerURL;
			return;
		}
		override public native function clone():Event
		//{
		//	return new DRMLicenseRequestEvent( type, bubbles, cancelable, inServerURL );
		//}
	}
}