package flash.display
{
[native(cls="GraphicsGradientFillClass", methods="auto")]

//import flash.geom.Matrix
//import flash.display.SpreadMethod

//public class GraphicsGradientFill extends Object implements IGraphicsFill, IGraphicsData
//{
	//Consts
	
	//Properties
//	public native function get alphas() : Array
//	public native function set alphas(alphas : Array):void
	
//	public native function get colors() : Array
//	public native function set colors(colors : Array):void
	
//	public native function get focalPointRatio() : Number
//	public native function set focalPointRatio(focalPointRatio : Number):void
	
//	public native function get interpolationMethod() : String
//	public native function set interpolationMethod(interpolationMethod : String):void
	
//	public native function get matrix() : Matrix
//	public native function set matrix(matrix : Matrix):void
	
//	public native function get ratios() : Array
//	public native function set ratios(ratios : Array):void
	
//	public native function get spreadMethod() : String
//	public native function set spreadMethod(spreadMethod : String):void
	
//	public native function get type() : String
//	public native function set type(type : String):void
	
	//Methods
//	public native function GraphicsGradientFill(type:String = "linear", colors:Array = null, alphas:Array = null, ratios:Array = null, matrix:* = null, spreadMethod:* = SpreadMethod.PAD, interpolationMethod:String = "rgb", focalPointRatio:Number = 0.0)
//}

    import flash.geom.*;

    final public class GraphicsGradientFill implements IGraphicsFill, IGraphicsData
    {
        private var _type:String;
        public var colors:Array;
        public var alphas:Array;
        public var ratios:Array;
        public var matrix:Matrix;
        private var _spreadMethod:String;
        private var _interpolationMethod:String;
        public var focalPointRatio:Number;

        public function GraphicsGradientFill(type:String = "linear", colors:Array = null, alphas:Array = null, ratios:Array = null, matrix = null, spreadMethod = "pad", interpolationMethod:String = "rgb", focalPointRatio:Number = 0)
        {
            this._type = type;
            this.colors = colors;
            this.alphas = alphas;
            this.ratios = ratios;
            this.matrix = matrix;
            this._spreadMethod = spreadMethod;
            this._interpolationMethod = interpolationMethod;
            this.focalPointRatio = focalPointRatio;
            if (this._type != GradientType.LINEAR)
            {
                   if (this._type != GradientType.RADIAL)
	            {
	                Error.throwError(ArgumentError, 2008, "type");
	            }
            }
            if (this._spreadMethod != "none")
            {
                   if (this._spreadMethod != SpreadMethod.PAD)
	            {
	                   if (this._spreadMethod != SpreadMethod.REFLECT)
		            {
			            if (this._spreadMethod != SpreadMethod.REPEAT)
			            {
			                Error.throwError(ArgumentError, 2008, "spreadMethod");
			            }		            
		            }
	            }
            }
            if (this._interpolationMethod != InterpolationMethod.LINEAR_RGB)
            {
                   if (this._interpolationMethod != InterpolationMethod.RGB)
	            {
	                Error.throwError(ArgumentError, 2008, "interpolationMethod");
	            }
            }
            return;
        }// end function

        public function get type() : String
        {
            return this._type;
        }// end function

        public function set type(value:String)
        {
            if (value != GradientType.LINEAR)
            {
                   if (value != GradientType.RADIAL)
	            {
	                Error.throwError(ArgumentError, 2008, "type");
	            }
            }
            this._type = value;
            return;
        }// end function

        public function get spreadMethod() : String
        {
            return this._spreadMethod;
        }// end function

        public function set spreadMethod(value:String)
        {
            if (value != "none")
            {
	            if (value != SpreadMethod.PAD)
	            {
		            if (value != SpreadMethod.REFLECT)
		            {
		                   if (value != SpreadMethod.REPEAT)
			            {
			                Error.throwError(ArgumentError, 2008, "spreadMethod");
			            }
		            }       
	            }
            }
            this._spreadMethod = value;
            return;
        }// end function

        public function get interpolationMethod() : String
        {
            return this._interpolationMethod;
        }// end function

        public function set interpolationMethod(value:String)
        {
            if (value != InterpolationMethod.LINEAR_RGB)
            {
                   if (value != InterpolationMethod.RGB)
	            {
	                Error.throwError(ArgumentError, 2008, "interpolationMethod");
	            }
            }
            this._interpolationMethod = value;
            return;
        }// end function

    }
}