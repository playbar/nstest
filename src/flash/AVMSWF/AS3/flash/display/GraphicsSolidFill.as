package flash.display
{
[native(cls="GraphicsSolidFillClass", methods="auto")]

import flash.display.*

final public class GraphicsSolidFill implements IGraphicsFill, IGraphicsData
{
	//Consts
	
	//Properties
//	public native function get alpha() : Number
//	public native function set alpha(alpha : Number):void
	
//	public native function get color() : uint
//	public native function set color(color : uint):void
	
	//Methods
//	public native function GraphicsSolidFill(color:uint = 0, alpha:Number = 1.0)

        public var color:uint = 0;
        public var alpha:Number = 1;

        public function GraphicsSolidFill(color:uint = 0, alpha:Number = 1)
        {
            this.color = color;
            this.alpha = alpha;
            return;
        }// end function
}
}