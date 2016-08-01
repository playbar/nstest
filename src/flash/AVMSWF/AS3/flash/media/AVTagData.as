package flash.media
{
	[native(cls="AVTagDataClass",methods="auto")]
	
	public class AVTagData
	{
		//Const
		
		//Properties
				
		public native function get data():String
		public native function get localTime():Number
		
		//Methods

		public native function AVTagData(init_data:String = null, init_localTime:Number = 0)
		//{
		//	this.data = init_data;
		//	this.localTime = init_localTime;
		//	return
		//}
		//private function set data(data:String)
		//{
		//	this.data = data;
		//}
		//private function set localTime(lt:Number)
		//{
		//	this.localTime = lt;
		//}
	}
}