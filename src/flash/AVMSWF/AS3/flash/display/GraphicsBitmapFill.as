package flash.display
{
[native(cls="GraphicsBitmapFillClass", methods="auto")]

import flash.geom.*
import flash.display.*

final public class GraphicsBitmapFill implements IGraphicsFill, IGraphicsData
{
	//Consts
	
	//Properties
//	public native function get bitmapData() : BitmapData
//	public native function set bitmapData(bitmapData : BitmapData):void
	
//	public native function get matrix() : Matrix
//	public native function set matrix(matrix : Matrix):void
	
//	public native function get repeat() : Boolean
//	public native function set repeat(repeat : Boolean):void
	
//	public native function get smooth() : Boolean
//	public native function set smooth(smooth : Boolean):void
	
	//Methods
//	public native function GraphicsBitmapFill(bitmapData:BitmapData = null, matrix:Matrix = null, repeat:Boolean = true, smooth:Boolean = false)
        public var bitmapData:BitmapData;
        public var matrix:Matrix;
        public var repeat:Boolean;
        public var smooth:Boolean;

        public function GraphicsBitmapFill(bitmapData:BitmapData = null, matrix:Matrix = null, repeat:Boolean = true, smooth:Boolean = false)
        {
            this.bitmapData = bitmapData;
            this.matrix = matrix;
            this.repeat = repeat;
            this.smooth = smooth;
            return;
        }// end function
}
}