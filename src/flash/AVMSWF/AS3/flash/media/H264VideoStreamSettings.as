package flash.media
{
	[native(cls="H264VideoStreamSettingsClass",methods="auto")]

	import flash.media.VideoStreamSettings
	import flash.media.H264Level
	import flash.media.H264Profile
	
	
	public class H264VideoStreamSettings extends VideoStreamSettings
	{
		//Const
		
		//Properties
		
		override public native function get codec():String
		
		public native function get level():String
		
		public native function get profile():String
		
		//Methods
		
		public function H264VideoStreamSettings()
		{
			return;
		}
		public native function setProfileLevel(profile:String, level:String):void
		//{
			//if ( profile == null )
			//	this.profile = H264Profile.BASELINE;
			//else
			//	this.profile = profile;
			//if ( level == null )
			//	this.level = H264Level.LEVEL_2_1;
			//else 
			//	this.level = level;
		//}
		
	}
}