package flash.ui
{
[native(cls="MouseCursorDataClass", methods="auto")]

import flash.display.BitmapData
import flash.geom.Point

public class MouseCursorData
{
	//Consts
	
	//Properties
	public native function get data() : Vector.<BitmapData>
	public native function set data(data : Vector.<BitmapData>):void
	
	public native function get frameRate() : Number
	public native function set frameRate(frameRate : Number):void
	
	public native function get hotSpot() : Point
	public native function set hotSpot(hotSpot : Point):void
	
	//Methods
//	public native function MouseCursorData()

        public function MouseCursorData()
        {
            return;
        }// end function
}
}