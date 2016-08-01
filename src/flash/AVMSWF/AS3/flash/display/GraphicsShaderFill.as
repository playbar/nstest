package flash.display
{
[native(cls="GraphicsShaderFillClass", methods="auto")]

import flash.geom.Matrix

//import flash.display.*;

public class GraphicsShaderFill implements IGraphicsFill, IGraphicsData
{
	//Consts
	
	//Properties
//	public native function get matrix() : Matrix
//	public native function set matrix(matrix : Matrix):void
	
//	public native function get shader() : Shader
//	public native function set shader(shader : Shader):void
	
	//Methods
//	public native function GraphicsShaderFill(shader:Shader = null, matrix:Matrix = null)

        public var shader:Shader;
        public var matrix:Matrix;

        public function GraphicsShaderFill(shader:Shader = null, matrix:Matrix = null)
        {
            this.shader = shader;
            this.matrix = matrix;
            return;
        }// end function
}
}