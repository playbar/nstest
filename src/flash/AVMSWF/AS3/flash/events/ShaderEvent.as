package flash.events
{
[native(cls="ShaderEventClass", methods="auto")]

import flash.display.BitmapData
import flash.utils.ByteArray

public class ShaderEvent extends Event
{
	//Consts
	public static const COMPLETE : String = "complete"
	
	//Properties
//	public native function get bitmapData() : BitmapData
//	public native function set bitmapData(bitmapData : BitmapData):void
	
//	public native function get byteArray() : ByteArray
//	public native function set byteArray(byteArray : ByteArray):void
	
//	public native function get vector() : Vector.<Number>
//	public native function set vector(vector : Vector.<Number>):void
	
	//Methods
//	public native function ShaderEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, bitmap:BitmapData = null, array:ByteArray = null, vector:Vector.<Number> = null)
//	public native override function clone():Event
//	public native override function toString():String

        private var m_bitmapData:BitmapData;
        private var m_byteArray:ByteArray;
        private var m_vector:Vector.<Number>;
	
	public function ShaderEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, bitmap:BitmapData = null, array:ByteArray = null, vector:Vector.<Number> = null)
        {
            super(type, bubbles, cancelable);
            this.m_bitmapData = bitmap;
            this.m_byteArray = array;
            this.m_vector = vector;
            return;
        }// end function

        override public function clone() : Event
        {
            return new ShaderEvent(type, bubbles, cancelable, this.bitmapData, this.byteArray, this.vector);
        }// end function

        override public function toString() : String
        {
            return formatToString("ShaderEvent", "type", "bubbles", "cancelable", "eventPhase", "bitmapData", "byteArray", "vector");
        }// end function

        public function get bitmapData() : BitmapData
        {
            return this.m_bitmapData;
        }// end function

        public function set bitmapData(bmpData:BitmapData)
        {
            this.m_bitmapData = bmpData;
            return;
        }// end function

        public function get byteArray() : ByteArray
        {
            return this.m_byteArray;
        }// end function

        public function set byteArray(bArray:ByteArray)
        {
            this.m_byteArray = bArray;
            return;
        }// end function

        public function get vector() : Vector.<Number>
        {
            return this.m_vector;
        }// end function

        public function set vector(v:Vector.<Number>)
        {
            this.m_vector = v;
            return;
        }// end function
}
}