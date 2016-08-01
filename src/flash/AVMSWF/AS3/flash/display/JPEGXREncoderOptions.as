package flash.display
{
	[native(cls="JPEGXREncoderOptionsClass",methods="auto")]
	
	import flash.display.BitmapEncodingColorSpace
	
	public final class JPEGXREncoderOptions
	{
		//Properties
		public var colorSpace:String
		public var quantization:uint
		public var trimFlexBits:uint
		
		//methods
		public function JPEGXREncoderOptions(quantization:uint = 20, colorSpace:String = "auto", trimFlexBits:uint = 0)
		{
			this.quantization = quantization;
			this.colorSpace   = colorSpace;
			this.trimFlexBits = trimFlexBits;
			return;
		}//end function
	}
}