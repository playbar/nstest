package flash.events
{
[native(cls="NetDataEventClass", methods="auto")]

public class NetDataEvent extends Event
{
	//Consts
	public static const MEDIA_TYPE_DATA : String = "mediaTypeData"
	
	//Properties
//	public native function get info() : Object
	
//	public native function get timestamp() : Number
	
	//Methods
//	public native function NetDataEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, timestamp:Number = 0, info:Object = null)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_info:Object;
        private var m_timestamp:Number;
	
	public function NetDataEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, timestamp:Number = 0, info:Object = null)
        {
            super(type, bubbles, cancelable);
            this.m_timestamp = timestamp;
            this.m_info = info;
            return;
        }// end function

        override public function clone() : Event
        {
            return new NetDataEvent(type, bubbles, cancelable, this.timestamp, this.info);
        }// end function

        override public function toString() : String
        {
            return formatToString("NetDataEvent", "type", "bubbles", "cancelable", "eventPhase", "timestamp");
        }// end function

        public function get timestamp() : Number
        {
            return this.m_timestamp;
        }// end function

        public function get info() : Object
        {
            return this.m_info;
        }// end function
}
}