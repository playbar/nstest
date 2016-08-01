package flash.events
{
[native(cls="OutputProgressEventClass", methods="auto")]

public class OutputProgressEvent extends Event
{
	//Consts
	public static const OUTPUT_PROGRESS : String = "outputProgress"
	
	//Properties
//	public native function get bytesPending() : Number
//	public native function set bytesPending(bytesPending : Number):void
	
//	public native function get bytesTotal() : Number
//	public native function set bytesTotal(bytesTotal : Number):void
	
	//Methods
//	public native function OutputProgressEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, bytesPending:Number = 0, bytesTotal:Number = 0)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_bytesPending:Number;
        private var m_bytesTotal:Number;
	
	public function OutputProgressEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, bytesPending:Number = 0, bytesTotal:Number = 0)
        {
            super(type, bubbles, cancelable);
            this.m_bytesPending = bytesPending;
            this.m_bytesTotal = bytesTotal;
            return;
        }// end function

        override public function clone() : Event
        {
            return new OutputProgressEvent(type, bubbles, cancelable, this.m_bytesPending, this.m_bytesTotal);
        }// end function

        override public function toString() : String
        {
            return formatToString("OutputProgressEvent", "type", "bubbles", "cancelable", "eventPhase", "bytesPending", "bytesTotal");
        }// end function

        public function get bytesPending() : Number
        {
            return this.m_bytesPending;
        }// end function

        public function set bytesPending(value:Number) : void
        {
            this.m_bytesPending = value;
            return;
        }// end function

        public function get bytesTotal() : Number
        {
            return this.m_bytesTotal;
        }// end function

        public function set bytesTotal(value:Number) : void
        {
            this.m_bytesTotal = value;
            return;
        }// end function
}
}