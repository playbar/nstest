package flash.text.ime
{
[native(cls="CompositionAttributeRangeClass", methods="auto")]

public class CompositionAttributeRange
{
	//Consts
	
	//Properties
//	public native function get converted() : Boolean
//	public native function set converted(converted : Boolean):void
	
//	public native function get relativeEnd() : int
//	public native function set relativeEnd(relativeEnd : int):void
	
//	public native function get relativeStart() : int
//	public native function set relativeStart(relativeStart : int):void
	
//	public native function get selected() : Boolean
//	public native function set selected(selected : Boolean):void
	
	//Methods
//	public native function CompositionAttributeRange(relativeStart:int, relativeEnd:int, selected:Boolean, converted:Boolean)
        public var relativeStart:int;
        public var relativeEnd:int;
        public var selected:Boolean;
        public var converted:Boolean;

        public function CompositionAttributeRange(relativeStart:int, relativeEnd:int, selected:Boolean, converted:Boolean) : void
        {
            this.relativeStart = relativeStart;
            this.relativeEnd = relativeEnd;
            this.selected = selected;
            this.converted = converted;
            return;
        }// end function
}
}