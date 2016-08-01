package flash.display
{
	[native(cls="PNGEncoderOptionsClass",methods="auto")]
	
	public final class PNGEncoderOptions
	{
		//Properties
		
		public var fastCompression:Boolean
		
		//methods
		
		public function PNGEncoderOptions(fastCompression:Boolean = false)
		{
			this.fastCompression = fastCompression;
			return;
		}//end function
	}
}