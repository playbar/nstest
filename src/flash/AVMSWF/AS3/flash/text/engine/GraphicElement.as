package flash.text.engine
{
[native(cls="GraphicElementClass", methods="auto")]

import flash.display.DisplayObject
import flash.events.EventDispatcher

public class GraphicElement extends ContentElement
{
	//Consts
	
	//Properties
	public native function get elementHeight() : Number
	public native function set elementHeight(elementHeight : Number):void
	
	public native function get elementWidth() : Number
	public native function set elementWidth(elementWidth : Number):void
	
	public native function get graphic() : DisplayObject
	public native function set graphic(graphic : DisplayObject):void
	
	//Methods
//	public native function GraphicElement(graphic:DisplayObject = null, elementWidth:Number = 15.0, elementHeight:Number = 15.0, elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")

        public function GraphicElement(graphic:DisplayObject = null, elementWidth:Number = 15, elementHeight:Number = 15, elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")
        {
            super(elementFormat, eventMirror, textRotation);
            this.elementWidth = elementWidth;
            this.elementHeight = elementHeight;
            this.graphic = graphic;
            return;
        }// end function
}
}