package flash.text.engine
{
[native(cls="ContentElementClass", methods="auto")]

import flash.events.EventDispatcher
import flash.utils.*;

public class ContentElement
{
	//Consts
	public var userData:Object;
	public static const GRAPHIC_ELEMENT : uint = 65007
	
	//Properties
	public native function get elementFormat() : ElementFormat
	public native function set elementFormat(elementFormat : ElementFormat):void
	
	public native function get eventMirror() : EventDispatcher
	public native function set eventMirror(eventMirror : EventDispatcher):void
	
	public native function get groupElement() : GroupElement
	
	public native function get rawText() : String
	
	public native function get text() : String
	
	public native function get textBlock() : TextBlock
	
	public native function get textBlockBeginIndex() : int
	
	public native function get textRotation() : String
	public native function set textRotation(textRotation : String):void
	
//	public native function get userData() : *
//	public native function set userData(userData : *):void
	
	//Methods
//	public native function ContentElement(elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")

        public function ContentElement(elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")
        {
            //if (getQualifiedClassName(this) == "flash.text.engine::ContentElement")
            //{
              //  Error.throwError(ArgumentError, 2012, "ContentElement");
            //}
            this.elementFormat = elementFormat;
            this.eventMirror = eventMirror;
            this.textRotation = textRotation;
            return;
        }// end function
}
}