package flash.text
{
[native(cls="TextExtentClass", methods="auto")]

public class TextExtent
{
	//Consts
	
	//Properties
//	public native function get ascent() : Number
//	public native function set ascent(ascent : Number):void
	
//	public native function get descent() : Number
//	public native function set descent(descent : Number):void
	
//	public native function get height() : Number
//	public native function set height(height : Number):void
	
//	public native function get textFieldHeight() : Number
//	public native function set textFieldHeight(textFieldHeight : Number):void
	
//	public native function get textFieldWidth() : Number
//	public native function set textFieldWidth(textFieldWidth : Number):void
	
//	public native function get width() : Number
//	public native function set width(width : Number):void
	
	//Methods
//	public native function TextExtent(width:Number, height:Number, textFieldWidth:Number, textFieldHeight:Number, ascent:Number, descent:Number)
        public var width:Number;
        public var height:Number;
        public var textFieldWidth:Number;
        public var textFieldHeight:Number;
        public var ascent:Number;
        public var descent:Number;

        public function TextExtent(width:Number, height:Number, textFieldWidth:Number, textFieldHeight:Number, ascent:Number, descent:Number)
        {
            this.width = width;
            this.height = height;
            this.textFieldWidth = textFieldWidth;
            this.textFieldHeight = textFieldHeight;
            this.ascent = ascent;
            this.descent = descent;
            return;
        }// end function
}
}