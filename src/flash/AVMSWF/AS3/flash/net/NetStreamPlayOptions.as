package flash.net
{
[native(cls="NetStreamPlayOptionsClass", methods="auto")]

import flash.events.EventDispatcher

public class NetStreamPlayOptions extends EventDispatcher
{
	//Consts
	
	//Properties
//	public native function get len() : Number
//	public native function set len(len : Number):void
	
//	public native function get offset() : Number
//	public native function set offset(offset : Number):void
	
//	public native function get oldStreamName() : String
//	public native function set oldStreamName(oldStreamName : String):void
	
//	public native function get start() : Number
//	public native function set start(start : Number):void
	
//	public native function get streamName() : String
//	public native function set streamName(streamName : String):void
	
//	public native function get transition() : String
//	public native function set transition(transition : String):void
	
	//Methods
//	public native function NetStreamPlayOptions()

        public var streamName:String;
        public var oldStreamName:String;
        public var start:Number;
        public var len:Number;
        public var offset:Number;
        public var transition:String;

        public function NetStreamPlayOptions()
        {
            this.start = -2;
            this.len = -1;
            this.offset = -1;
            return;
        }// end function
}
}