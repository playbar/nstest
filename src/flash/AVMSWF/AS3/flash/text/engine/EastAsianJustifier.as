package flash.text.engine
{
[native(cls="EastAsianJustifierClass", methods="auto")]

final public class EastAsianJustifier extends TextJustifier
{
	//Consts
	
	//Properties
	public native function get justificationStyle() : String
	public native function set justificationStyle(justificationStyle : String):void
	
	//Methods
//	public native function EastAsianJustifier(locale:String = "ja", lineJustification:String = "allButLast", justificationStyle:String = "pushInKinsoku")
//	public native override function clone():TextJustifier
        public native function get composeTrailingIdeographicSpaces() : Boolean;
        public native function set composeTrailingIdeographicSpaces(value:Boolean) : void;
	
        public function EastAsianJustifier(locale:String = "ja", lineJustification:String = "allButLast", justificationStyle:String = "pushInKinsoku")
        {
            super(locale, lineJustification);
            this.justificationStyle = justificationStyle;
            return;
        }// end function
	
	override public function clone() : TextJustifier
        {
            var _loc_1:* = new EastAsianJustifier(this.locale, this.lineJustification, this.justificationStyle);
            _loc_1.composeTrailingIdeographicSpaces = this.composeTrailingIdeographicSpaces;
            return _loc_1;
        }// end function
}
}