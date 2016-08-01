package flash.display
{
[native(cls="GraphicsTrianglePathClass", methods="auto")]

import flash.display.*

final public class GraphicsTrianglePath implements IGraphicsPath, IGraphicsData
{
	//Consts
	
	//Properties
//	public native function get culling() : String
//	public native function set culling(culling : String):void
	
//	public native function get indices() : Vector.<int>
//	public native function set indices(indices : Vector.<int>):void
	
//	public native function get uvtData() : Vector.<Number>
//	public native function set uvtData(uvtData : Vector.<Number>):void
	
//	public native function get vertices() : Vector.<Number>
//	public native function set vertices(vertices : Vector.<Number>):void
	
	//Methods
//	public native function GraphicsTrianglePath(vertices:Vector.<Number> = null, indices:Vector.<int> = null, uvtData:Vector.<Number> = null, culling:String = "none")

        public var indices:Vector.<int>;
        public var vertices:Vector.<Number>;
        public var uvtData:Vector.<Number>;
        private var _culling:String;

        public function GraphicsTrianglePath(vertices:Vector.<Number> = null, indices:Vector.<int> = null, uvtData:Vector.<Number> = null, culling:String = "none")
        {
            this.vertices = vertices;
            this.indices = indices;
            this.uvtData = uvtData;
            this._culling = culling;
            
            if (culling != TriangleCulling.NONE)
            {
	            if (culling != TriangleCulling.POSITIVE)
	            {
		            if (culling != TriangleCulling.NEGATIVE)
		            {
		                Error.throwError(ArgumentError, 2008, "culling");
		            }            
	            }            
            }

            return;
        }// end function

        public function get culling() : String
        {
            return this._culling;
        }// end function

        public function set culling(value:String) : void
        {
            if (value != TriangleCulling.NONE)
            {
                   if (value != TriangleCulling.POSITIVE)
	            {
		            if (value != TriangleCulling.NEGATIVE)
		            {
		                Error.throwError(ArgumentError, 2008, "culling");
		            }            
	            }
            }
            this._culling = value;
            return;
        }// end function
}
}