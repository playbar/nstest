package flash.events
{
	[native(cls="DRMMetadataEventClass", methods="auto")]
	
	import flash.events.Event
	import flash.net.drm.DRMContentData
	import flash.utils.ByteArray
	
	public class DRMMetadataEvent extends Event
	{
		//const
		public static const DRM_METADATA:String = "drmMetadata"
		
		//Properties
		public native function get drmMetadata():DRMContentData
		public native function get timestamp():Number
		
		//Methods
		public function DRMMetadataEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, inMetadata:ByteArray = null, inTimestamp:Number = 0)
		{
			super( type, bubbles, cancelable );
			this.drmMetadata = inMetadata;
			this.timestamp   = inTimestamp;
			return;
		}
	}
}