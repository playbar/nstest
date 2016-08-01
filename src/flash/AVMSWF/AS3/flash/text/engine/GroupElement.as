package flash.text.engine
{
[native(cls="GroupElementClass", methods="auto")]

import flash.events.EventDispatcher

public class GroupElement extends ContentElement
{
	//Consts
	
	//Properties
	public native function get elementCount() : int
	
	//Methods
//	public native function GroupElement(elements:Vector.<ContentElement> = null, elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")
	public native function getElementAt(index:int):ContentElement
	public native function getElementAtCharIndex(charIndex:int):ContentElement
//	public native function getElementIndex(element:ContentElement):int
	public native function groupElements(beginIndex:int, endIndex:int):GroupElement
	public native function mergeTextElements(beginIndex:int, endIndex:int):TextElement
	public native function replaceElements(beginIndex:int, endIndex:int, newElements:Vector.<ContentElement>):Vector.<ContentElement>
	public native function setElements(value:Vector.<ContentElement>):void
	public native function splitTextElement(elementIndex:int, splitIndex:int):TextElement
	public native function ungroupElements(groupIndex:int):void
        public function GroupElement(elements:Vector.<ContentElement> = null, elementFormat:ElementFormat = null, eventMirror:EventDispatcher = null, textRotation:String = "rotate0")
        {
            super(elementFormat, eventMirror, textRotation);
            this.setElements(elements);
            return;
        }// end function
	
	public function getElementIndex(element:ContentElement) : int
        {
            var _loc_2:int = 0;
            while (_loc_2 < this.elementCount)
            {
                
                if (element == this.getElementAt(_loc_2))
                {
                    return _loc_2;
                }
                _loc_2 = _loc_2 + 1;
            }
            return -1;
        }// end function
}
}