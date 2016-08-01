package flash.events
{
[native(cls="TouchEventClass", methods="auto")]

import flash.display.InteractiveObject
import flash.utils.ByteArray

public class TouchEvent extends Event
{
	//Consts
	public static const TOUCH_BEGIN : String = "touchBegin"
	public static const TOUCH_END : String = "touchEnd"
	public static const TOUCH_MOVE : String = "touchMove"
	public static const TOUCH_OUT : String = "touchOut"
	public static const TOUCH_OVER : String = "touchOver"
	public static const TOUCH_ROLL_OUT : String = "touchRollOut"
	public static const TOUCH_ROLL_OVER : String = "touchRollOver"
	public static const TOUCH_TAP : String = "touchTap"
        public static const PROXIMITY_BEGIN:String = "proximityBegin";
        public static const PROXIMITY_END:String = "proximityEnd";
        public static const PROXIMITY_MOVE:String = "proximityMove";
        public static const PROXIMITY_OUT:String = "proximityOut";
        public static const PROXIMITY_OVER:String = "proximityOver";
        public static const PROXIMITY_ROLL_OUT:String = "proximityRollOut";
        public static const PROXIMITY_ROLL_OVER:String = "proximityRollOver";
	
	//Properties
	public native function get altKey() : Boolean
	public native function set altKey(altKey : Boolean):void
	
	public native function get commandKey() : Boolean
	public native function set commandKey(commandKey : Boolean):void
	
	public native function get controlKey() : Boolean
	public native function set controlKey(controlKey : Boolean):void
	
	public native function get ctrlKey() : Boolean
	public native function set ctrlKey(ctrlKey : Boolean):void
	
	public native function get isPrimaryTouchPoint() : Boolean
	public native function set isPrimaryTouchPoint(isPrimaryTouchPoint : Boolean):void
	
	public native function get isRelatedObjectInaccessible() : Boolean
	public native function set isRelatedObjectInaccessible(isRelatedObjectInaccessible : Boolean):void
	
	public native function get localX() : Number
	public native function set localX(localX : Number):void
	
	public native function get localY() : Number
	public native function set localY(localY : Number):void
	
	public native function get pressure() : Number
	public native function set pressure(pressure : Number):void
	
	public native function get relatedObject() : InteractiveObject
	public native function set relatedObject(relatedObject : InteractiveObject):void
	
	public native function get shiftKey() : Boolean
	public native function set shiftKey(shiftKey : Boolean):void
	
	public native function get sizeX() : Number
	public native function set sizeX(sizeX : Number):void
	
	public native function get sizeY() : Number
	public native function set sizeY(sizeY : Number):void
	
	public native function get stageX() : Number
	
	public native function get stageY() : Number
	
	public native function get touchPointID() : int
	public native function set touchPointID(touchPointID : int):void
	
	//new adds 11
	public native function get isTouchPointCanceled():Boolean
    public native function set isTouchPointCanceled(value:Boolean):void
    public native function get timestamp():Number
    public native function set timestamp(value:Number):void
    public native function get touchIntent():String
    public native function set touchIntent(value:String):void
    //new adds 11 end 
	
	//Methods
	public native function TouchEvent(type:String, bubbles:Boolean = true, cancelable:Boolean = false, touchPointID:int = 0, isPrimaryTouchPoint:Boolean = false, localX:Number = NaN, localY:Number = NaN, sizeX:Number = NaN, sizeY:Number = NaN, pressure:Number = NaN, relatedObject:InteractiveObject = null, ctrlKey:Boolean = false, altKey:Boolean = false, shiftKey:Boolean = false, commandKey:Boolean = false, controlKey:Boolean = false, timestamp:Number = NaN, touchIntent:String = null, isTouchPointCanceled:Boolean = false)
	public native override function clone():Event
	public native override function toString():String
	public native function updateAfterEvent():void
	
	//new adds 11
	public native function getSamples(buffer:ByteArray, append:Boolean = false):uint
	public native function isToolButtonDown(index:int):Boolean
	//new adds 11 end
}
}