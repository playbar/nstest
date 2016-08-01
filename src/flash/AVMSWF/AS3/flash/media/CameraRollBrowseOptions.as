package flash.media
{
	[native(cls="CameraRollBrowseOptionsClass",methods="auto")]
	
	import flash.geom.Rectangle
	
	public class CameraRollBrowseOptions
	{
		//Const
		
		//Properties
		
		public native function get height():Number
		public native function set height(value:Number):void
	    
		public native function get origin():Rectangle
		public native function set origin(value:Rectangle):void
	    
		public native function get width():Number
		public native function set width(value:Number):void
    
    
		//Methods
		public function CameraRollBrowseOptions()
		{
			return;
		}
		
	}
}