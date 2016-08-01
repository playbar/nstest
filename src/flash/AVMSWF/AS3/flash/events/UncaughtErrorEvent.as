package flash.events
{
[native(cls="UncaughtErrorEventClass", methods="auto")]

public class UncaughtErrorEvent extends ErrorEvent
{
	//Consts
	public static const UNCAUGHT_ERROR : String = "uncaughtError"
	
	//Properties
//	public native function get error() : *
	
	//Methods
//	public native function UncaughtErrorEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = true, error_in:* = null)
//	public native override function clone():Event
//	public native override function toString():String
        private var _error:Object;

        public function UncaughtErrorEvent(type:String = "uncaughtError", bubbles:Boolean = true, cancelable:Boolean = true, error_in = null)
        {
            super(type, bubbles, cancelable);
            this._error = error_in;
            return;
        }// end function

        override public function clone() : Event
        {
            return new UncaughtErrorEvent(type, bubbles, cancelable, this._error);
        }// end function

        public function get error()
        {
            return this._error;
        }// end function

        override public function toString() : String
        {
            return formatToString("UncaughtErrorEvent", "type", "bubbles", "cancelable", "eventPhase");
        }// end function
}
}