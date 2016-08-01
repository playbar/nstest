package flash.display
{
	[native(cls="JPEGEncoderOptionsClass",methods="auto")]
	
	public final class JPEGEncoderOptions
	{
		//Properties
		public var quality:uint
		
		//methods
		public function JPEGEncoderOptions(quality:uint = 80)
		{
			this.quality = quality;
			return;
		}
	}
}