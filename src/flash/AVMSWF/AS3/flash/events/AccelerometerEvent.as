package flash.events
{
[native(cls="AccelerometerEventClass", methods="auto")]

public class AccelerometerEvent extends Event
{
	//Consts
	public static const UPDATE : String = "update"
	
	//Properties
//	public native function get accelerationX() : Number
//	public native function set accelerationX(accelerationX : Number):void
	
//	public native function get accelerationY() : Number
//	public native function set accelerationY(accelerationY : Number):void
	
//	public native function get accelerationZ() : Number
//	public native function set accelerationZ(accelerationZ : Number):void
	
//	public native function get timestamp() : Number
//	public native function set timestamp(timestamp : Number):void
	
	//Methods
//	public native function AccelerometerEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, timestamp:Number = 0, accelerationX:Number = 0, accelerationY:Number = 0, accelerationZ:Number = 0)
//	public native override function toString():String
//	public native override function clone():Event
        private var m_accelerationX:Number;
        private var m_accelerationY:Number;
        private var m_accelerationZ:Number;
        private var m_timestamp:Number;

        public function AccelerometerEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, timestamp:Number = 0, accelerationX:Number = 0, accelerationY:Number = 0, accelerationZ:Number = 0)
        {
            super(type, bubbles, cancelable);
            this.m_accelerationX = accelerationX;
            this.m_accelerationY = accelerationY;
            this.m_accelerationZ = accelerationZ;
            this.m_timestamp = timestamp;
            return;
        }// end function

        override public function clone() : Event
        {
            return new AccelerometerEvent(type, bubbles, cancelable, this.m_timestamp, this.m_accelerationX, this.m_accelerationY, this.m_accelerationZ);
        }// end function

        override public function toString() : String
        {
            return formatToString("AccelerometerEvent", "type", "bubbles", "cancelable", "timestamp", "accelerationX", "accelerationY", "accelerationZ");
        }// end function

        public function get accelerationX() : Number
        {
            return this.m_accelerationX;
        }// end function

        public function set accelerationX(value:Number) : void
        {
            this.m_accelerationX = value;
            return;
        }// end function

        public function get accelerationY() : Number
        {
            return this.m_accelerationY;
        }// end function

        public function set accelerationY(value:Number) : void
        {
            this.m_accelerationY = value;
            return;
        }// end function

        public function get accelerationZ() : Number
        {
            return this.m_accelerationZ;
        }// end function

        public function set accelerationZ(value:Number) : void
        {
            this.m_accelerationZ = value;
            return;
        }// end function

        public function get timestamp() : Number
        {
            return this.m_timestamp;
        }// end function

        public function set timestamp(value:Number) : void
        {
            this.m_timestamp = value;
            return;
        }// end function
}

}