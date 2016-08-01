package flash.events
{
[native(cls="SampleDataEventClass", methods="auto")]

import flash.utils.ByteArray

public class SampleDataEvent extends Event
{
	//Consts
	public static const SAMPLE_DATA : String = "sampleData"
	
	//Properties
//	public native function get data() : ByteArray
//	public native function set data(data : ByteArray):void
	
//	public native function get position() : Number
//	public native function set position(position : Number):void
	
	//Methods
//	public native function SampleDataEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, theposition:Number = 0, thedata:ByteArray = null)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_position:Number;
        private var m_data:ByteArray;
	
	public function SampleDataEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, theposition:Number = 0, thedata:ByteArray = null)
        {
            super(type, bubbles, cancelable);
            this.m_position = theposition;
            this.m_data = thedata;
            return;
        }// end function

        override public function clone() : Event
        {
            return new SampleDataEvent(type, bubbles, cancelable, this.position, this.data);
        }// end function

        override public function toString() : String
        {
            return formatToString("SampleDataEvent", "type", "bubbles", "cancelable", "eventPhase", "position", "data");
        }// end function

        public function get position() : Number
        {
            return this.m_position;
        }// end function

        public function set position(theposition:Number)
        {
            this.m_position = theposition;
            return;
        }// end function

        public function get data() : ByteArray
        {
            return this.m_data;
        }// end function

        public function set data(thedata:ByteArray)
        {
            this.m_data = thedata;
            return;
        }// end function
}
}