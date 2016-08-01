package flash.events
{
	[native(cls="DRMDeviceGroupErrorEventClass", methods="auto")]

	import flash.events.ErrorEvent
	import flash.events.TextEvent
	import flash.events.Event
	import flash.net.drm.DRMDeviceGroup
	

	public class DRMDeviceGroupErrorEvent extends ErrorEvent
	{
		//Consts
		public static const ADD_TO_DEVICE_GROUP_ERROR:String = "addToDeviceGroupError"
		public static const REMOVE_FROM_DEVICE_GROUP_ERROR:String = "removeFromDeviceGroupError"
		
		//Properties
		//private var m_deviceGroup:DRMDeviceGroup
		//private var m_drmUpdateNeeded:Boolean
		//private var m_subErrorID:int
		//private var m_systemUpdateNeeded:Boolean
		
		public native function get deviceGroup():DRMDeviceGroup
		public native function set deviceGroup(value:DRMDeviceGroup):void
		
		public native function get drmUpdateNeeded():Boolean
		
		public native function get subErrorID():int
		public native function set subErrorID(value:int):void
		
		public native function get systemUpdateNeeded():Boolean
		
		//Methods
		
		public function DRMDeviceGroupErrorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, errorDetail:String = "", errorCode:int = 0, subErrorID:int = 0, deviceGroup:DRMDeviceGroup = null, systemUpdateNeeded:Boolean = false, drmUpdateNeeded:Boolean = false)
		{
			super(type, bubbles, cancelable, errorDetail, errorCode);
			this.deviceGroup        = deviceGroup;
			this.subErrorID         = subErrorID;
			this.systemUpdateNeeded = systemUpdateNeeded;
			this.drmUpdateNeeded    = drmUpdateNeeded;
			return;
		}
	
		override public native function clone():Event
		override public native function toString():String
	}
}