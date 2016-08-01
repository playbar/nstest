package flash.events
{
[native(cls="TransformGestureEventClass", methods="auto")]

public class TransformGestureEvent extends GestureEvent
{
	//Consts
	public static const GESTURE_PAN : String = "gesturePan"
	public static const GESTURE_ROTATE : String = "gestureRotate"
	public static const GESTURE_SWIPE : String = "gestureSwipe"
	public static const GESTURE_ZOOM : String = "gestureZoom"
	
	//Properties
//	public native function get offsetX() : Number
//	public native function set offsetX(offsetX : Number):void
	
//	public native function get offsetY() : Number
//	public native function set offsetY(offsetY : Number):void
	
//	public native function get rotation() : Number
//	public native function set rotation(rotation : Number):void
	
//	public native function get scaleX() : Number
//	public native function set scaleX(scaleX : Number):void
	
//	public native function get scaleY() : Number
//	public native function set scaleY(scaleY : Number):void
	
	//Methods
//	public native function TransformGestureEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, phase:String = null, localX:Number = 0, localY:Number = 0, scaleX:Number = 1.0, scaleY:Number = 1.0, rotation:Number = 0, offsetX:Number = 0, offsetY:Number = 0, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, commandKey:Boolean = false, controlKey:Boolean = false)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_scaleX:Number;
        private var m_scaleY:Number;
        private var m_rotation:Number;
        private var m_offsetX:Number;
        private var m_offsetY:Number;
	
	public function TransformGestureEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, phase:String = null, localX:Number = 0, localY:Number = 0, scaleX:Number = 1, scaleY:Number = 1, rotation:Number = 0, offsetX:Number = 0, offsetY:Number = 0, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false)
        {
            super(type, bubbles, cancelable, phase, localX, localY, ctrlKey, altKey, shiftKey);
            this.m_scaleX = scaleX;
            this.m_scaleY = scaleY;
            this.m_rotation = rotation;
            this.m_offsetX = offsetX;
            this.m_offsetY = offsetY;
            return;
        }// end function

        override public function clone() : Event
        {
            return new TransformGestureEvent(type, bubbles, cancelable, phase, localX, localY, this.m_scaleX, this.m_scaleY, this.m_rotation, this.m_offsetX, this.m_offsetY, ctrlKey, altKey, shiftKey);
        }// end function

        override public function toString() : String
        {
            return formatToString("TransformGestureEvent", "type", "bubbles", "cancelable", "phase", "args", "localX", "localY", "stageX", "stageY", "scaleX", "scaleY", "rotation", "offsetX", "offsetY", "ctrlKey", "altKey", "shiftKey");
        }// end function

        public function get scaleX() : Number
        {
            return this.m_scaleX;
        }// end function

        public function set scaleX(value:Number) : void
        {
            this.m_scaleX = value;
            return;
        }// end function

        public function get scaleY() : Number
        {
            return this.m_scaleY;
        }// end function

        public function set scaleY(value:Number) : void
        {
            this.m_scaleY = value;
            return;
        }// end function

        public function get rotation() : Number
        {
            return this.m_rotation;
        }// end function

        public function set rotation(value:Number) : void
        {
            this.m_rotation = value;
            return;
        }// end function

        public function get offsetX() : Number
        {
            return this.m_offsetX;
        }// end function

        public function set offsetX(value:Number) : void
        {
            this.m_offsetX = value;
            return;
        }// end function

        public function get offsetY() : Number
        {
            return this.m_offsetY;
        }// end function

        public function set offsetY(value:Number) : void
        {
            this.m_offsetY = value;
            return;
        }// end function
}
}