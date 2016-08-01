package flash.text.engine
{
[native(cls="TabStopClass", methods="auto")]

public class TabStop
{
	//Consts
	
	//Properties
	public native function get alignment() : String
	public native function set alignment(alignment : String):void
	
	public native function get decimalAlignmentToken() : String
	public native function set decimalAlignmentToken(decimalAlignmentToken : String):void
	
	public native function get position() : Number
	public native function set position(position : Number):void
	
	//Methods
//	public native function TabStop(alignment:String = "start", position:Number = 0.0, decimalAlignmentToken:String = "")

        public function TabStop(alignment:String = "start", position:Number = 0, decimalAlignmentToken:String = "")
        {
            this.alignment = alignment;
            this.position = position;
            this.decimalAlignmentToken = decimalAlignmentToken;
            return;
        }// end function
}
}