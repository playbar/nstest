package flash.events
{
	[native(cls="DRMDeviceGroupEventClass", methods="auto")]
	
	import flash.events.event
	import flash.net.drm.DRMDeviceGroup
	
	public class DRMDeviceGroupEvent extends Event
	{
		//const
		public static const ADD_TO_DEVICE_GROUP_COMPLETE:String = "addToDeviceGroupComplete"
		public static const REMOVE_FROM_DEVICE_GROUP_COMPLETE:String = "removeFromDeviceGroupComplete"
		
		//Properties
		//private var m_deviceGroup:DRMDeviceGroup
		public native function get deviceGroup():DRMDeviceGroup
		public native function set deviceGroup(value:DRMDeviceGroup):void
		
		//methods
		public function DRMDeviceGroupEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, deviceGroup:DRMDeviceGroup = null)
		{
			super(type, bubbles, cancelable);
			this.deviceGroup = deviceGroup;
			return;
		}
		override public native function clone():Event
		override public native function toString():String
	}
}