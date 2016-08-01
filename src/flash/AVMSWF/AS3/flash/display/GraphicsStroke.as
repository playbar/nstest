package flash.display
{
[native(cls="GraphicsStrokeClass", methods="auto")]

final public class GraphicsStroke implements IGraphicsStroke, IGraphicsData
{
	//Consts
	
	//Properties
//	public native function get caps() : String
//	public native function set caps(caps : String):void
	
//	public native function get fill() : IGraphicsFill
//	public native function set fill(fill : IGraphicsFill):void
	
//	public native function get joints() : String
//	public native function set joints(joints : String):void
	
//	public native function get miterLimit() : Number
//	public native function set miterLimit(miterLimit : Number):void
	
//	public native function get pixelHinting() : Boolean
//	public native function set pixelHinting(pixelHinting : Boolean):void
	
//	public native function get scaleMode() : String
//	public native function set scaleMode(scaleMode : String):void
	
//	public native function get thickness() : Number
//	public native function set thickness(thickness : Number):void
	
	//Methods
//	public native function GraphicsStroke(thickness:Number = NaN, pixelHinting:Boolean = false, scaleMode:String = "normal", caps:String = "none", joints:String = "round", miterLimit:Number = 3.0, fill:IGraphicsFill = null)

        public var thickness:Number;
        public var pixelHinting:Boolean;
        private var _caps:String;
        private var _joints:String;
        public var miterLimit:Number;
        private var _scaleMode:String;
        public var fill:IGraphicsFill;

        public function GraphicsStroke(thickness:Number = NaN, pixelHinting:Boolean = false, scaleMode:String = "normal", caps:String = "none", joints:String = "round", miterLimit:Number = 3, fill:IGraphicsFill = null)
        {
            this.thickness = thickness;
            this.pixelHinting = pixelHinting;
            this._caps = caps;
            this._joints = joints;
            this.miterLimit = miterLimit;
            this._scaleMode = scaleMode;
            this.fill = fill;
            
            if (this._scaleMode != LineScaleMode.NORMAL)
            {
	            if (this._scaleMode != LineScaleMode.NONE)
	            {
		            if (this._scaleMode != LineScaleMode.VERTICAL)
		            {
			            if (this._scaleMode != LineScaleMode.HORIZONTAL)
			            {
			                Error.throwError(ArgumentError, 2008, "scaleMode");
			            }            
		            }            
	            }            
            }

            if (this._caps != CapsStyle.NONE)
            {
	            if (this._caps != CapsStyle.ROUND)
	            {
		            if (this._caps != CapsStyle.SQUARE)
		            {
		                Error.throwError(ArgumentError, 2008, "caps");
		            }           
	            }            
            }

            if (this._joints != JointStyle.BEVEL)
            {
	            if (this._joints != JointStyle.MITER)
	            {
		            if (this._joints != JointStyle.ROUND)
		            {
		                Error.throwError(ArgumentError, 2008, "joints");
		            }            
	            }            
            }

            return;
        }// end function

        public function get caps() : String
        {
            return this._caps;
        }// end function

        public function set caps(value:String) : void
        {
            if (value != CapsStyle.NONE)
            {
	             if (value != CapsStyle.ROUND)
	            {
		            if (value != CapsStyle.SQUARE)
		            {
		                Error.throwError(ArgumentError, 2008, "caps");
		            }            
	            }           
            }

            this._caps = value;
            return;
        }// end function

        public function get joints() : String
        {
            return this._joints;
        }// end function

        public function set joints(value:String)
        {
            if (value != JointStyle.BEVEL)
            {
	            if (value != JointStyle.MITER)
	            {
		            if (value != JointStyle.ROUND)
		            {
		                Error.throwError(ArgumentError, 2008, "joints");
		            }            
	            }           
            }
            this._joints = value;
            return;
        }// end function

        public function get scaleMode() : String
        {
            return this._scaleMode;
        }// end function

        public function set scaleMode(value:String) : void
        {
            if (value != LineScaleMode.NORMAL)
            {
	            if (value != LineScaleMode.NONE)
	            {
		            if (value != LineScaleMode.VERTICAL)
		            {
			            if (value != LineScaleMode.HORIZONTAL)
			            {
			                Error.throwError(ArgumentError, 2008, "scaleMode");
			            }            
		            }            
	            }            
            }
            this._scaleMode = value;
            return;
        }// end function
}
}