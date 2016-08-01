package flash.events
{
	[native(cls="AVDictionaryDataEventClass", methods="auto")]
	
	import flash.events.Event
	import flash.utils.Dictionary

	public final class AVDictionaryDataEvent extends Event
	{
		//Consts
		public static const AV_DICTIONARY_DATA:String = "avDictionaryData"
		
		//Properties
		private var m_dictionary:Dictionary;
		private var m_time:Number;
		public function get dictionary():Dictionary
		{
			return this.m_dictionary;
		}
		public function get time():Number
		{
			return this.m_time;
		}
		
		//Methods
		public function AVDictionaryDataEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, init_dictionary:Dictionary = null, init_dataTime:Number = 0)
		{
			super(type, bubbles, cancelable);
			this.m_dictionary = init_dictionary;
			this.m_time       = init_dataTime;
			return;
		}//end function
	}
}