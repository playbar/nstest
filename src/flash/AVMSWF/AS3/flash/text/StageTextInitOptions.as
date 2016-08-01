package flash.text
{
	[native(cls="StageTextInitOptionsClass",methods="auto")]
	
	
	public class StageTextInitOptions
	{
		//Const
		
		//Properties
		
		public native function get multiline():Boolean
		public native function set multiline(value:Boolean):void
		
		//Methods
		
		public function StageTextInitOptions(multiline:Boolean = false)
		{
			this.multiline = multiline;
			return;
		}
		
	}
}