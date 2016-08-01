package flash.net.drm
{
	[native(cls="DRMDeviceGroupClass",methods="auto")]
	
	import flash.net.drm.AuthenticationMethod
	
	public final class DRMDeviceGroup
	{
		//Const
		
		//Properties
		
		public native function get authenticationMethod():String
		
		public native function get domain():String
		
		public native function get name():String
		public native function set name(value:String):void
		
		public native function get serverURL():String
		
		//Methods
		
	}
}