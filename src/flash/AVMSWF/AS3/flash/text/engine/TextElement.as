package flash.text.engine
{
[native(cls="TextElementClass", methods="auto")]

import flash.events.EventDispatcher

public class TextElement extends ContentElement
{
	//Consts
	
	//Properties
//	public native function get text() : String		not override
//	public native override function get text() : String
	public native function set text(text : String):void
	
	//Methods
//	public native function TextElement(text:String = null, elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")
	public native function replaceText(beginIndex:int, endIndex:int, newText:String):void

        public function TextElement(text:String = null, elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")
        {
            super(elementFormat, eventMirror, textRotation);
            this.text = text;
            return;
        }// end function
}
}