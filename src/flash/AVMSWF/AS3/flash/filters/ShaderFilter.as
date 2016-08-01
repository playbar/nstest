package flash.filters
{
[native(cls="ShaderFilterClass", methods="auto")]

import flash.display.*
import flash.geom.*

public class ShaderFilter extends BitmapFilter
{
	//Consts
	
	//Properties
	public native function get bottomExtension() : int
	public native function set bottomExtension(bottomExtension : int):void
	
	public native function get leftExtension() : int
	public native function set leftExtension(leftExtension : int):void
	
	public native function get rightExtension() : int
	public native function set rightExtension(rightExtension : int):void
	
	public native function get shader() : Shader
	public native function set shader(shader : Shader):void
	
	public native function get topExtension() : int
	public native function set topExtension(topExtension : int):void
	
	//Methods
	public native function ShaderFilter(shader:Shader = null)
}
}