package flash.sampler
{
[native(cls="StackFrameClass", methods="auto")]

public class StackFrame
{
	//Consts
	public static const file : String
	public static const line : uint
	public static const name : String
	public static const scriptID : Number
	
	//Properties
	
	//Methods
//	public native function toString():String
        public function toString() : String
        {
            return this.name + "()" + (this.file ? ("[" + this.file + ":" + this.line + "]") : (""));
        }// end function
}
}