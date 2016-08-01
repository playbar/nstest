package flash.media
{
	[native(cls="VideoStreamSettingsClass",methods="auto")]
	
	public class VideoStreamSettings 
	{
		//Const
		
		//Properties
		
		public native function get bandwidth():int
		
		public native function get codec():String
		
		public native function get fps():Number
		
		public native function get height():int
		
		public native function get keyFrameInterval():int
		
		public native function get quality():int
		
		public native function get width():int
		
		//Methods
		
		public function VideoStreamSettings()
		{
			return;
		}
		
		public native function setKeyFrameInterval(keyFrameInterval:int):void
		//{
		//	if (keyFrameInterval <= 0 && keyFrameInterval != -1 )
		//		this.keyFrameInterval = 1;
		//	else if ( keyFrameInterval > 0 && keyFrameInterval < 300 )
		//		this.keyFrameInterval = keyFrameInterval;
		//	else if ( keyFrameInterval >= 300 )
		//		this.keyFrameInterval = 300;
		//	else
		//		this.keyFrameInterval = -1;
		//}
		public native function setMode(width:int, height:int, fps:Number):void
		/*{
			if ( width == -1 || (width >= 1 && width <= 1920) )
				this.width = width;
			else if ( width == 0 || width < -1 )
				this.width = 1;
			else
				this.width = 1920;
			if ( height == -1 || (height >= 1 && height <= 1920) )
				this.height = height;
			else if ( height == 0 || height < -1 )
				this.height = 1;
			else
				this.height = 1920;
				
			if ( fps < -1 )
				this.fps = 0;
			else if ( fps >= -1 && fps < 60 )
				this.fps = fps;
			else 
				this.fps = 60;
				
		}*/
		public native function setQuality(bandwidth:int, quality:int):void
		/*{
			if ( bandwidth >= 0 )
				this.bandwidth = bandwidth;
			else if ( bandwidth == -1 )
				this.bandwidth = -1;
			else 
				this.bandwidth = 0;

			if ( quality >= 0 && quality <100  )
				this.quality = quality;
			else if ( quality == -1 )
				this.quality = -1;
			else if ( quality >= 100 )
				this.quality = 100;
			else 
				this.quality = 0;
		}*/
		
	}
}