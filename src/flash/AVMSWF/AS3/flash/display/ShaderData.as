package flash.display
{
[native(cls="ShaderDataClass", methods="auto")]

import flash.utils.ByteArray

public final dynamic class ShaderData
{
	//Consts
	
	//Properties
	
	//Methods
//	public native function ShaderData(byteCode:ByteArray)

        private native function SetByteCode(code:ByteArray) : void
        public function ShaderData(byteCode:ByteArray)
        {
            this.SetByteCode(byteCode);
            return;
        }// end function
}
}