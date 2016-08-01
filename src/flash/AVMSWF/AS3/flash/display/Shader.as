package flash.display
{
[native(cls="ShaderClass", methods="auto")]

import flash.utils.*

public class Shader
{
	//Consts
	
	//Properties
//	public native function set byteCode(byteCode : ByteArray):void
	
        public function set byteCode(code:ByteArray) : void
        {
            this.data = new ShaderData(code);
            return;
        }// end function
	public native function get data() : ShaderData
	public native function set data(data : ShaderData):void
	
	public native function get precisionHint() : String
	public native function set precisionHint(precisionHint : String):void
	
	//Methods
//	public native function Shader(code:ByteArray = null)

        public function Shader(code:ByteArray = null)
        {
            if (code)
            {
                this.byteCode = code;
            }
            return;
        }// end function
}
}