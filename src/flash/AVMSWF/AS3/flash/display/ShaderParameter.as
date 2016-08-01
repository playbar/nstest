package flash.display
{
[native(cls="ShaderParameterClass", methods="auto")]

final dynamic public class ShaderParameter
{
	//Consts
	
	//Properties
	public native function get index() : int
	
	public native function get type() : String
	
	public native function get value() : Array
	public native function set value(value : Array):void
	
	//Methods
//	public native function ShaderParameter()
        public function ShaderParameter()
        {
            return;
        }// end function
}
}