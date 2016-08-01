package flash.display
{
[native(cls="ShaderInputClass", methods="auto")]

final dynamic public class ShaderInput
{
	//Consts
	
	//Properties
	public native function get channels() : int
	
	public native function get height() : int
	public native function set height(height : int):void
	
	public native function get index() : int
	
	public native function get input() : Object
	public native function set input(input : Object):void
	
	public native function get width() : int
	public native function set width(width : int):void
	
	//Methods
	public native function ShaderInput()
}
}