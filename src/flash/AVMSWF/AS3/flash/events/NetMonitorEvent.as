package flash.events
{
[native(cls="NetMonitorEventClass", methods="auto")]

import flash.net.*

public class NetMonitorEvent extends Event
{
	//Consts
	public static const NET_STREAM_CREATE : String = "netStreamCreate"
	
	//Properties
//	public native function get netStream() : NetStream
	
	//Methods
//	public native function NetMonitorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, netStream:NetStream = null)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_netStream:NetStream;

        public function NetMonitorEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, netStream:NetStream = null)
        {
            super(type, bubbles, cancelable);
            this.m_netStream = netStream;
            return;
        }// end function

        override public function clone() : Event
        {
            return new NetMonitorEvent(type, bubbles, cancelable, this.netStream);
        }// end function

        override public function toString() : String
        {
            return formatToString("NetMonitorEvent", "type", "bubbles", "cancelable", "eventPhase", "netStream");
        }// end function

        public function get netStream() : NetStream
        {
            return this.m_netStream;
        }// end function
}
}