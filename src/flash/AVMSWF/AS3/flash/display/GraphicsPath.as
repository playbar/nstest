package flash.display
{
[native(cls="ClassGraphicsPath", methods="auto")]

//import flash.display.*

public class GraphicsPath implements IGraphicsPath, IGraphicsData
{
	//Consts
	
	//Properties
//	public native function get commands() : Vector.<int>
//	public native function set commands(commands : Vector.<int>):void
	
//	public native function get data() : Vector.<Number>
//	public native function set data(data : Vector.<Number>):void
	
//	public native function get winding() : String
//	public native function set winding(winding : String):void
	
	//Methods
//	public native function GraphicsPath(commands:Vector.<int> = null, data:Vector.<Number> = null, winding:String = "evenOdd")
//	public native function curveTo(controlX:Number, controlY:Number, anchorX:Number, anchorY:Number):void
//	public native function lineTo(x:Number, y:Number):void
//	public native function moveTo(x:Number, y:Number):void
//	public native function wideLineTo(x:Number, y:Number):void
//	public native function wideMoveTo(x:Number, y:Number):void
        public var commands:Vector.<int>;
        public var data:Vector.<Number>;
        private var _winding:String;

        public function GraphicsPath(commands:Vector.<int> = null, data:Vector.<Number> = null, winding:String = "evenOdd")
        {
            this.commands = commands;
            this.data = data;
            if (winding != GraphicsPathWinding.EVEN_ODD)
            {
                   if (winding != GraphicsPathWinding.NON_ZERO)
	            {
	                Error.throwError(ArgumentError, 2008, "winding");
	            }
            }
            this._winding = winding;
            return;
        }// end function

        public function get winding() : String
        {
            return this._winding;
        }// end function

        public function set winding(value:String)
        {
            if (value != GraphicsPathWinding.EVEN_ODD)
            {
                   if (value != GraphicsPathWinding.NON_ZERO)
	            {
	                Error.throwError(ArgumentError, 2008, "winding");
	            }
            }
            this._winding = value;
            return;
        }// end function

        public function moveTo(x:Number, y:Number) : void
        {
            if (this.commands == null)
            {
                this.commands = new Vector.<int>;
            }
            if (this.data == null)
            {
                this.data = new Vector.<Number>;
            }
            this.commands.push(GraphicsPathCommand.MOVE_TO);
            this.data.push(x, y);
            return;
        }// end function

        public function lineTo(x:Number, y:Number) : void
        {
            if (this.commands == null)
            {
                this.commands = new Vector.<int>;
            }
            if (this.data == null)
            {
                this.data = new Vector.<Number>;
            }
            this.commands.push(GraphicsPathCommand.LINE_TO);
            this.data.push(x, y);
            return;
        }// end function

        public function curveTo(controlX:Number, controlY:Number, anchorX:Number, anchorY:Number) : void
        {
            if (this.commands == null)
            {
                this.commands = new Vector.<int>;
            }
            if (this.data == null)
            {
                this.data = new Vector.<Number>;
            }
            this.commands.push(GraphicsPathCommand.CURVE_TO);
            this.data.push(controlX, controlY, anchorX, anchorY);
            return;
        }// end function

        public function cubicCurveTo(controlX1:Number, controlY1:Number, controlX2:Number, controlY2:Number, anchorX:Number, anchorY:Number) : void
        {
            if (this.commands == null)
            {
                this.commands = new Vector.<int>;
            }
            if (this.data == null)
            {
                this.data = new Vector.<Number>;
            }
            this.commands.push(GraphicsPathCommand.CUBIC_CURVE_TO);
            this.data.push(controlX1, controlY1, controlX2, controlY2, anchorX, anchorY);
            return;
        }// end function

        public function wideLineTo(x:Number, y:Number) : void
        {
            if (this.commands == null)
            {
                this.commands = new Vector.<int>;
            }
            if (this.data == null)
            {
                this.data = new Vector.<Number>;
            }
            this.commands.push(GraphicsPathCommand.WIDE_LINE_TO);
            this.data.push(0, 0, x, y);
            return;
        }// end function

        public function wideMoveTo(x:Number, y:Number) : void
        {
            if (this.commands == null)
            {
                this.commands = new Vector.<int>;
            }
            if (this.data == null)
            {
                this.data = new Vector.<Number>;
            }
            this.commands.push(GraphicsPathCommand.WIDE_MOVE_TO);
            this.data.push(0, 0, x, y);
            return;
        }// end function
}
}