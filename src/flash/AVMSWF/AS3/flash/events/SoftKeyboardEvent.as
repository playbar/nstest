package flash.events
{
[native(cls="SoftKeyboardEventClass", methods="auto")]

import flash.display.InteractiveObject

public class SoftKeyboardEvent extends Event
{
	//Consts
	public static const SOFT_KEYBOARD_ACTIVATE : String = "softKeyboardActivate"
	public static const SOFT_KEYBOARD_ACTIVATING : String = "softKeyboardActivating"
	public static const SOFT_KEYBOARD_DEACTIVATE : String = "softKeyboardDeactivate"
	
	//Properties
//	public native function get relatedObject() : InteractiveObject
//	public native function set relatedObject(relatedObject : InteractiveObject):void
	
//	public native function get triggerType() : String
	
	//Methods
//	public native function SoftKeyboardEvent(type:String, bubbles:Boolean, cancelable:Boolean, relatedObjectVal:InteractiveObject, triggerTypeVal:String)
//	public native override function clone():Event
//	public native override function toString():String
        private var m_relatedObject:InteractiveObject;
        private var m_triggerType:String;

        public function SoftKeyboardEvent(type:String, bubbles:Boolean, cancelable:Boolean, relatedObjectVal:InteractiveObject, triggerTypeVal:String)
        {
            super(type, bubbles, cancelable);
            this.relatedObject = relatedObjectVal;
            this.m_triggerType = triggerTypeVal;
            return;
        }// end function

        override public function clone() : Event
        {
            return new SoftKeyboardEvent(type, bubbles, cancelable, this.m_relatedObject, this.m_triggerType);
        }// end function

        override public function toString() : String
        {
            return formatToString("SoftKeyboardEvent", "type", "bubbles", "cancelable", "relatedObject", "triggerType");
        }// end function

        public function get triggerType() : String
        {
            return this.m_triggerType;
        }// end function

        public function get relatedObject() : InteractiveObject
        {
            return this.m_relatedObject;
        }// end function

        public function set relatedObject(value:InteractiveObject) : void
        {
            this.m_relatedObject = value;
            return;
        }// end function
}
}