package flash.events
{
[native(cls="PressAndTapGestureEventClass", methods="auto")]

public class PressAndTapGestureEvent extends GestureEvent
{
	//Consts
	public static const GESTURE_PRESS_AND_TAP : String = "gesturePressAndTap"
	
	//Properties
//	public native function get tapLocalX() : Number
//	public native function set tapLocalX(tapLocalX : Number):void
	
//	public native function get tapLocalY() : Number
//	public native function set tapLocalY(tapLocalY : Number):void
	
//	public native function get tapStageX() : Number
	
//	public native function get tapStageY() : Number
	
	//Methods
//	public native function PressAndTapGestureEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, phase:String = null, localX:Number = 0, localY:Number = 0, tapLocalX:Number = 0, tapLocalY:Number = 0, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, commandKey:Boolean = false, controlKey:Boolean = false)
//	public native override function clone():Event
//	public native override function toString():String

        public function PressAndTapGestureEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, phase:String = null, localX:Number = 0, localY:Number = 0, tapLocalX:Number = 0, tapLocalY:Number = 0, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false)
        {
            super(type, bubbles, cancelable, phase, localX, localY, ctrlKey, altKey, shiftKey);
            this.tapLocalX = tapLocalX;
            this.tapLocalY = tapLocalY;
            return;
        }// end function

        override public function clone() : Event
        {
            return new PressAndTapGestureEvent(type, bubbles, cancelable, phase, localX, localY, this.tapLocalX, this.tapLocalY, ctrlKey, altKey, shiftKey);
        }// end function

        override public function toString() : String
        {
            return formatToString("PressAndTapGestureEvent", "type", "bubbles", "cancelable", "phase", "args", "localX", "localY", "tapLocalX", "tapLocalY", "ctrlKey", "altKey", "shiftKey");
        }// end function

        public native function get tapLocalX() : Number;

        public native function set tapLocalX(value:Number) : void;

        public native function get tapLocalY() : Number;

        public native function set tapLocalY(value:Number) : void;

        public function get tapStageX() : Number
        {
            if (!isNaN(this.tapLocalX))
            {
                isNaN(this.tapLocalX);
            }
            if (isNaN(this.tapLocalY))
            {
                return Number.NaN;
            }
            return this.getTapStageX();
        }// end function

        public function get tapStageY() : Number
        {
            if (!isNaN(this.tapLocalX))
            {
                isNaN(this.tapLocalX);
            }
            if (isNaN(this.tapLocalY))
            {
                return Number.NaN;
            }
            return this.getTapStageY();
        }// end function

        private native function getTapStageX() : Number;

        private native function getTapStageY() : Number;
}
}